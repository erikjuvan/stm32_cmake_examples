////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sleep.c
*@brief  contains sleep services
*@author Zdenko Mezgec
*@author EMTAS
*@date   03.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stddef.h>
#include <string.h>
#include "gen_define.h"
#ifdef CO_EVENT_SLEEP
#include <co_datatype.h>
#include <co_timer.h>
#include <co_drv.h>
#include <co_commtask.h>
#include <co_sleep.h>
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_nmt.h"
#include "ico_sleep.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define CO_SLEEP_MASTER_CAN_ID		0x691u
#define CO_SLEEP_SLAVE_CAN_ID		0x690u
#define CO_SLEEP_REQ_SLEEP_CAN_ID	0x692u

#define CO_SLEEP_CMD_QUERY			1u
#define CO_SLEEP_CMD_SLEEP			2u

#define CO_SLEEP_STATE_REQ			0x03u
#define CO_SLEEP_STATE_OBJECTION	0x81u

/* local defined data types
---------------------------------------------------------------------------*/
typedef struct {
		UNSIGNED8	status;		/* wake up state */
		UNSIGNED8	reason;		/* wake up reason */
		COB_REFERENZ_T	sleepCob;
} WAKEUP_T;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static CO_EVENT_SLEEP_T sleepTable[CO_EVENT_SLEEP];
static UNSIGNED8        sleepTableCnt = 0u;
static CO_TIMER_T       sleepTimer;
static CO_TIMER_T       awakeTimer;
static COB_REFERENZ_T   sleepMasterCob;
static COB_REFERENZ_T   sleepSlaveCob;
static COB_REFERENZ_T   sleepRequestCob;
static BOOL_T           sleepActive = CO_FALSE;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static UNSIGNED8    callSleepInd(CO_SLEEP_MODE_T sleepMode);
static void         sleepSilent(void *ptr);
static void         sleepDoze(void *ptr);
static void         sleepWakeUpMessage(void *ptr);
#ifdef CO_NMT_MASTER
static void         sleepNoObjection(void *ptr);
#endif /* CO_NMT_MASTER */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \internal
*
* \brief coNmtMsgHandler - handle received NMT mesages
*
*
* \return none
*
*/
void icoSleepMsgHandler(
		CO_CONST CO_REC_DATA_T *pRec		/* pointer to receive data */
	)
{
    UNSIGNED8	error;
    CO_TRANS_DATA_T trData;

	if (pRec->msg.len != 8u)  {
		return;
	}

	/* message from master */
	if (pRec->msg.canCob.canId == CO_SLEEP_MASTER_CAN_ID)  {
		/* query sleep objection ? */
		if (pRec->msg.data[0] == CO_SLEEP_CMD_QUERY)  {
			/* check, if sleep is possible */
			error = callSleepInd(CO_SLEEP_MODE_CHECK);
			if (error != 0u)  {
				/* answer error */
				memset(&trData.data[0], 0, 8u);
				trData.data[0] = CO_SLEEP_STATE_OBJECTION;
				trData.data[1] = error;
				(void)icoTransmitMessage(sleepSlaveCob, &trData, 0u);
			}
		}
		/* sleep mode ? */
		if (pRec->msg.data[0] == CO_SLEEP_CMD_SLEEP)  {
			/* goto sleep start mode */
			coSleepModeStart(CO_SLEEP_WAITTIME);
		}
	}

	/* objection from slave */
	if (pRec->msg.canCob.canId == CO_SLEEP_SLAVE_CAN_ID)  {
		if (pRec->msg.data[0] == CO_SLEEP_STATE_OBJECTION)  {
			(void)coTimerStop(&sleepTimer);
			(void)callSleepInd(CO_SLEEP_MODE_OBJECTION);
		}
	}

	if (pRec->msg.canCob.canId == CO_SLEEP_REQ_SLEEP_CAN_ID)  {
		(void)callSleepInd(CO_SLEEP_MODE_REQUEST_SLEEP);
	}
}

#ifdef CO_NMT_MASTER
/***************************************************************************/
/**
*
* \brief coSleepModeCheck - check nodes for sleep mode
*
* This function sends the sleep objection command
* If an answer is received, the registered indication function is called.
* If no answer was received, the function starts automatically 
* coSleepModeStart() - to start sleep mode.
*
*
* \return none
*
*/
void coSleepModeCheck(
		UNSIGNED16	waitTime		/**< time timeout value in ms */
	)
{
    CO_TRANS_DATA_T trData;

	/* send objection command */
	memset(&trData.data[0], 0, 8u);
	trData.data[0] = CO_SLEEP_CMD_QUERY;
	(void)icoTransmitMessage(sleepMasterCob, &trData, 0);

	/* start timeout timer */
	coTimerStart(&sleepTimer, waitTime * 1000ul, sleepNoObjection, NULL,
		CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
}


/***************************************************************************/
/**
* \internal
*
* \brief sleepNoObjection - timeout and no objection received
*
* \return none
*
*/
static void sleepNoObjection(void *ptr)
{
    UNSIGNED8	err;
    (void)ptr;

	err = callSleepInd(CO_SLEEP_MODE_PREPARE);
	if (err == 0)  {
		coSleepModeStart(1000u);
	}
}
#endif /* CO_NMT_MASTER */


/***************************************************************************/
/**
*
* \brief coSleepModeStart - start sleep mode
*
* This function starts the sleep mode.
* First a timer is started, then the CAN traffic is stopped
* and the CPU is going to sleep.
*
* Each step is indicated by the function
* registered by coEventRegister_SLEEP().
*
*
* \return none
*
*/
void coSleepModeStart(
		UNSIGNED16	waitTime		/**< wait time before stop CAN in ms */
	)
{
#ifdef CO_NMT_MASTER
    CO_TRANS_DATA_T trData;

	/* if master, send sleep command */
	memset(&trData.data[0], 0, 8u);
	trData.data[0] = CO_SLEEP_CMD_SLEEP;
	(void)icoTransmitMessage(sleepMasterCob, &trData, 0);
#endif /* CO_NMT_MASTER */

	/* if waitTime == 0, set ist to 1 */
	if (waitTime == 0u)  {
		waitTime++;
	}

	(void)coTimerStart(&sleepTimer, waitTime * 1000ul, sleepSilent, NULL,
		CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
	sleepActive = CO_TRUE;
}


/***************************************************************************/
/**
* \internal
*
* \brief sleepSilent
*
*
* \return none
*
*/
static void sleepSilent(
		void *ptr
	)
{
    UNSIGNED8	error;
    (void) ptr;

	error = callSleepInd(CO_SLEEP_MODE_SILENT);
	if (error == 0u)  {
		/* ok, disable CAN traffic */
		icoQueueDisable(CO_TRUE);



		/* start timer before sleep will be active */
		(void)coTimerStart(&sleepTimer, 1000000ul, sleepDoze, NULL,
			CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
	} else {
		sleepActive = CO_FALSE;
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief sleepDoze
*
*
* \return none
*
*/
static void sleepDoze(void *ptr)
{
    UNSIGNED8	error;
    (void) ptr;

	error = callSleepInd(CO_SLEEP_MODE_DOZE);
	if (error != 0u)  {
		icoQueueDisable(CO_FALSE);
		sleepActive = CO_FALSE;
	}
}


/***************************************************************************/
/**
*
* \brief coSleepAwake - awake from sleep
*
* This function is called after the processor is awaked.
* It transmit the wake up message, repeat it after the given repeatTime
* and reinitializes the communication handling.
*
* \return none
*
*/
void coSleepAwake(
		BOOL_T		master,		/**< wake up master */
		UNSIGNED8	status,		/**< wake up state */
		UNSIGNED8	reason,		/**< wake up reason */
		UNSIGNED16	repeatTime	/**< time interval for repeat wake up message*/
	)
{
static WAKEUP_T	wakeUpData;
#ifdef CO_LSS_SUPPORTED
    UNSIGNED8	nodeId = CO_NODE_ID;
#endif /* CO_LSS_SUPPORTED */

	/* if already awaked, return */
	if (sleepActive == CO_FALSE)  {
		return;
	}

	coCommTask();
	icoQueueDisable(CO_FALSE);
	sleepActive = CO_FALSE;

	/* transmit wakeup */
	wakeUpData.status = status;
	wakeUpData.reason = reason;
	if (master == CO_TRUE)  {
		wakeUpData.sleepCob = sleepMasterCob;
	} else {
		wakeUpData.sleepCob = sleepSlaveCob;
	}
	sleepWakeUpMessage(&wakeUpData);

	/* start timer for repeat wake up message */
	(void)coTimerStart(&awakeTimer, repeatTime * 1000ul, sleepWakeUpMessage,
		&wakeUpData, CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */

#ifdef CO_LSS_SUPPORTED
	/* fixes node-id ? */
	if (nodeId == 255u)  { /*lint !e774 depends on LSS configuration */
		icoNmtResetNodeId();
	} else
#endif /* CO_LSS_SUPPORTED */
	{
		(void)coNmtLocalStateReq(CO_NMT_STATE_RESET_COMM);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief sleepWakeUpMessage - transmit sleep wake message
*
* \return none
*
*/
static void sleepWakeUpMessage(void *ptr)
{
    CO_TRANS_DATA_T trData;
    WAKEUP_T	*pWakeUp = (WAKEUP_T *)ptr;

	memset(&trData.data[0], 0, 8u);
	trData.data[0] = pWakeUp->status;
	trData.data[1] = pWakeUp->reason;
	(void)icoTransmitMessage(pWakeUp->sleepCob, &trData, 0u);
}


/***************************************************************************/
/**
*
* \brief coSleepModeActive - check if sleep mode is active
*
*
* \return none
*
*/
BOOL_T coSleepModeActive(void)
{
	return(sleepActive);
}


#ifdef CO_NMT_MASTER
#else /* CO_NMT_MASTER */
/***************************************************************************/
/**
*
* \brief coSleepRequestSleep - request sleep mode to master
*
* Request sleep mode from master by sending sleep request.
*
* \return none
*
*/
void coSleepRequestSleep(void)
{
    CO_TRANS_DATA_T trData;

	/* send objection command */
	memset(&trData.data[0], 0, 8u);
	trData.data[0] = CO_SLEEP_STATE_REQ;
	(void)icoTransmitMessage(sleepRequestCob, &trData, 0u);
}
#endif /* CO_NMT_MASTER */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \brief coEventRegister_SLEEP - register SLEEP event
*
* register indication function for SLEEP events
*
* \return RET_T
*
*/

RET_T coEventRegister_SLEEP(
		CO_EVENT_SLEEP_T pFunction	/**< pointer to function */
    )
{
	/* set new indication function as first at the list */
	if (sleepTableCnt >= CO_EVENT_SLEEP) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	sleepTable[sleepTableCnt] = pFunction;	/* save function pointer */
	sleepTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief callSleepInd - call user specific indications
*
*
* \return RET_T
*
*/
static UNSIGNED8 callSleepInd(CO_SLEEP_MODE_T	sleepMode)
{
    UNSIGNED8	i;
    UNSIGNED8	retVal;

	for (i = 0u; i < sleepTableCnt; i++)  {
		retVal = sleepTable[i](sleepMode);
		if (retVal != 0u)  {
			return(retVal);
		}
	}

	return(0u);
}


/***************************************************************************/
/**
* \internal
*
* \brief coResetSleep - reset comm for Sleep
*
*
* \return none
*
*/
void icoSleepReset(void)
{
	(void)icoCobSet(sleepMasterCob, CO_SLEEP_MASTER_CAN_ID, CO_COB_RTR_NONE, 8u);
	(void)icoCobSet(sleepSlaveCob, CO_SLEEP_SLAVE_CAN_ID, CO_COB_RTR_NONE, 8u);
	(void)icoCobSet(sleepRequestCob, CO_SLEEP_REQ_SLEEP_CAN_ID, CO_COB_RTR_NONE, 8u);
}


/***************************************************************************/
/*
* \internal
*
* \brief icoInitNmt - init NMT functionality
*
* This function initializes the NMT functionality
* and calls an internal reset communication.
*
* If parameter master is unequal 0 the node will be initialized as NMT master.
*
* \return RET_T
*
*/
RET_T icoSleepInit(
		UNSIGNED8		master			/* master mode */
	)
{
	if (master != 0u)  {
		sleepMasterCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_SLEEP, 0u);
		sleepSlaveCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_SLEEP, 0u);
		sleepRequestCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_SLEEP, 0u);
	} else {
		sleepMasterCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_SLEEP, 0u);
		sleepSlaveCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_SLEEP, 0u);
		sleepRequestCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_SLEEP, 0u);
	}
	if ((sleepMasterCob == 0xffffu) || (sleepSlaveCob == 0xffffu)
 	 || (sleepRequestCob == 0xffffu))  {
		return(RET_NO_COB_AVAILABLE);
	}

	return(RET_OK);
}
#endif /* CO_EVENT_SLEEP */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
