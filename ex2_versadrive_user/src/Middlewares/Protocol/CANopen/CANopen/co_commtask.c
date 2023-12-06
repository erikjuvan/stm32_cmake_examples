////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_commtask.c
*@brief  contains communication task of canopen library
*@author Zdenko Mezgec
*@author EMTAS
*@date   03.12.2020
*/
/********************************************************************/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "gen_define.h"
#include "co_datatype.h"
#include "co_timer.h"
#include "co_drv.h"
#include "co_led.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_nmt.h"
#include "ico_sdo.h"
#include "ico_pdo.h"
#include "ico_commtask.h"
#include "ico_timer.h"
#include "ico_sync.h"
#include "ico_emcy.h"
#include "ico_time.h"
#include "ico_event.h"
#ifdef CO_LSS_SUPPORTED
# include "ico_lss.h"
#endif /* CO_LSS_SUPPORTED */
#ifdef CO_LSS_MASTER_SUPPORTED
# include "ico_lss.h"
#endif /* CO_LSS_MASTER_SUPPORTED */
#ifdef CO_EVENT_SLEEP
# include "ico_sleep.h"
#endif /* CO_EVENT_SLEEP */

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define COMM_STATE_TABLE_CNT    12u

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static BOOL_T   eventFlagMsg = CO_FALSE;
static BOOL_T   eventFlagTimer = CO_FALSE;
static BOOL_T   eventFlagEventHandler = CO_FALSE;
#ifdef CO_EVENT_CAN
static UNSIGNED8    canStateTableCnt = 0u;
static UNSIGNED8    commEventTableCnt = 0u;
static CO_EVENT_CAN_STATE_T canStateTable[CO_EVENT_CAN];
static CO_EVENT_COMM_T      commEventTable[CO_EVENT_CAN];
#endif /* CO_EVENT_CAN */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static BOOL_T serviceManagement(void);
static BOOL_T receiveMessageHandler(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \brief coCommTask - main communication task
*
* This is the main communication task for the CANopen stack.
* It has to be called cyclically by the application or signal
* driven after each received CAN message or timer event.
*
* \return void
*
*/
void coCommTask(
		void
	)
{
	BOOL_T messages_present;
	
	do
	{
  	/* call protocol handler */
		messages_present=serviceManagement();

		/* call cob handler */

		/* call queue handler */
		icoQueueHandler();
	}while (messages_present == CO_TRUE);
}


/***************************************************************************/
/**
*
* \internal
*
* \brief serviceManagement - call canopen protocol handler
*
* handles all canopen protocol services
* like service management, timer handling, communication state changes....
*
*
* \return void
*
*/
static BOOL_T serviceManagement(
		void
	)
{
	BOOL_T receive_msg_present=CO_FALSE;
	
	/* message handler - if new messages are available */
	if (eventFlagMsg != CO_FALSE)  {
		if (receiveMessageHandler() == CO_FALSE)
		{
			eventFlagMsg = CO_FALSE;
		}
		else
		{
			receive_msg_present=CO_TRUE;
		}
	}

	/* timer handling - if timer has elapsed */
	if (eventFlagTimer != CO_FALSE)  {
		eventFlagTimer = CO_FALSE;
		icoTimerCheck();
	}

	if (eventFlagEventHandler != CO_FALSE)  {
		eventFlagEventHandler = CO_FALSE;
		icoEventCheck();
	}

	/* communication state changes */
/*	commStateHandler();*/
	
	return receive_msg_present;

}


/***************************************************************************/
/**
*
* \internal
*
* \brief receiveMessageHandler - handles new receive messages
*
* handles new message from CAN
* read it from queue handler and call approproate service function
*
* \return message available
* \retval CO_FALSE
*	no message available
* \retval CO_TRUE
*	message available
*/
static BOOL_T receiveMessageHandler(
		void
	)
{
    CO_REC_DATA_T	recData;
    BOOL_T		avail;

	/* get next message from Queue handler */
	avail = icoQueueGetReceiveMessage(&recData);
	if (avail == CO_FALSE)  {
		return(avail);
	}

	switch (recData.service)  
	{
		
		case CO_SERVICE_NMT:
			icoNmtMsgHandler(&recData);
			break;

#ifdef CO_SYNC_SUPPORTED
		case CO_SERVICE_SYNC_RECEIVE:
			/* check for correct message len */
			if (recData.msg.len < 2u)  {
				icoSyncHandler(recData.msg.data[0]);
			}
			break;
#endif /* CO_SYNC_SUPPORTED */

#ifdef CO_PDO_RECEIVE_CNT
		case CO_SERVICE_PDO_RECEIVE:
			icoPdoReceiveHandler(&recData);
			break;
#endif /* CO_PDO_RECEIVE_CNT */

		case CO_SERVICE_ERRCTRL:
			icoNmtErrorCtrlHandler(&recData);
			break;

		case CO_SERVICE_SDO_SERVER:
			icoSdoServerHandler(&recData);
			break;

#ifdef CO_SDO_CLIENT_CNT
		case CO_SERVICE_SDO_CLIENT:
			icoSdoClientHandler(&recData);
			break;
#endif /* CO_SDO_CLIENT_CNT */

#ifdef CO_EMCY_CONSUMER_CNT
		case CO_SERVICE_EMCY_RECEIVE:
			icoEmcyConsumerHandler(&recData);
			break;
#endif /* CO_EMCY_CONSUMER_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
		case CO_SERVICE_PDO_TRANSMIT:
			icoPdoRtrHandler(&recData);
			break;
#endif /* CO_PDO_RECEIVE_CNT */

#ifdef CO_TIME_SUPPORTED
		case CO_SERVICE_TIME_RECEIVE:
			/* check for correct message len */
			if (recData.msg.len == 6u)  {
				icoTimeHandler(&recData.msg.data[0]);
			}

			break;
#endif /* CO_TIME_SUPPORTED */

#ifdef CO_LSS_SUPPORTED
		case CO_SERVICE_LSS_RECEIVE:
			icoLssHandler(&recData.msg.data[0]);
			break;
#endif /* CO_LSS_SUPPORTED */
#ifdef CO_LSS_MASTER_SUPPORTED
		case CO_SERVICE_LSS_RECEIVE:
			icoLssMasterHandler(&recData.msg.data[0]);
			break;
#endif /* CO_LSS_MASTER_SUPPORTED */

#ifdef CO_EVENT_SLEEP
		case CO_SERVICE_SLEEP:
			icoSleepMsgHandler(&recData);
			break;
#endif /* CO_EVENT_SLEEP */

		default:
			break;
	}

	return(avail);
}


/***************************************************************************/
/**
*
* \internal
*
* \brief coCommTaskSet - set signal from CAN or timer
*
* This function signals new available messages from CAN or from timer
*
* \return void
*
*/
CO_INLINE void coCommTaskSet(
		CO_COMMTASK_EVENT_T	event
	)
{
	switch (event)  {
		case CO_COMMTASK_EVENT_TIMER:
			eventFlagTimer = CO_TRUE;
			break;
		case CO_COMMTASK_EVENT_MSG_AVAIL:
			eventFlagMsg = CO_TRUE;
			break;
		case CO_COMMTASK_EVENT_NEW_EVENT:
			eventFlagEventHandler = CO_TRUE;
			break;
	}
}


/***************************************************************************/
/**
* \brief coCommStateEvent - set a new communication state
*
* This function should be called,
* if a new communication state has been reached.
* <br>
* It sets the LEDs and informs the application about the event.
*
* \return void
*
*/
void coCommStateEvent(
		CO_COMM_STATE_EVENT_T	newEvent		/**< new communication event */
	)
{
    typedef enum {
        CST_CAN, CST_TRQ, CST_RECQ
    } COMM_STATE_TYPE_T;

    typedef struct {
        CO_COMM_STATE_EVENT_T	event;		/* comm event */
        COMM_STATE_TYPE_T		type;
        CO_CAN_STATE_T			state;		/* goto CAN state */
    } COMM_STATE_TABLE_T;

    static const COMM_STATE_TABLE_T	commTable[COMM_STATE_TABLE_CNT] = {
        { CO_COMM_STATE_EVENT_REC_QUEUE_FULL,	CST_RECQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_REC_QUEUE_OVERFLOW,CST_RECQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_REC_QUEUE_EMPTY,	CST_RECQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_TR_QUEUE_FULL,	CST_TRQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_TR_QUEUE_OVERFLOW,CST_TRQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_TR_QUEUE_EMPTY,	CST_TRQ, 	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_CAN_OVERRUN,		CST_RECQ,	CO_CAN_STATE_UNCHANGED},
        { CO_COMM_STATE_EVENT_BUS_OFF,			CST_CAN,	CO_CAN_STATE_BUS_OFF },
        { CO_COMM_STATE_EVENT_BUS_OFF_RECOVERY, CST_CAN,	CO_CAN_STATE_BUS_OFF },
        { CO_COMM_STATE_EVENT_BUS_ON,			CST_CAN,	CO_CAN_STATE_BUS_ON },
        { CO_COMM_STATE_EVENT_PASSIVE,			CST_CAN,	CO_CAN_STATE_PASSIVE },
        { CO_COMM_STATE_EVENT_ACTIVE,			CST_CAN,	CO_CAN_STATE_BUS_ON }
    };
    static CO_CAN_STATE_T	        canState = CO_CAN_STATE_BUS_OFF;
    static CO_COMM_STATE_EVENT_T    recQuState = CO_COMM_STATE_EVENT_REC_QUEUE_EMPTY;
    static CO_COMM_STATE_EVENT_T    trQuState = CO_COMM_STATE_EVENT_BUS_OFF_RECOVERY;
    UNSIGNED8	i;
    CO_CAN_STATE_T	        newCanState = CO_CAN_STATE_UNCHANGED;
#ifdef CO_EVENT_CAN
    CO_COMM_STATE_EVENT_T   newState = CO_COMM_STATE_EVENT_NONE;
    UNSIGNED8	cnt;
#endif /* CO_EVENT_CAN */

	i = 0u;
	/* search event at event table */
	while (i < COMM_STATE_TABLE_CNT)  {
		if (commTable[i].event == newEvent)  {
			break;
		}
		i++;
	}

	/* no state found ? */
	if (i == COMM_STATE_TABLE_CNT)  {
		return;
	}

	/* event changes CAN state ? */
	if (commTable[i].state != CO_CAN_STATE_UNCHANGED)  {
		newCanState = commTable[i].state;

		/* new CAN state reached ? */
		if (canState != newCanState)  {
			/* if bus-off, go to preop */
			if (newCanState == CO_CAN_STATE_BUS_OFF)  {
				icoErrorBehavior();
			}

#ifdef CO_EVENT_CAN
			cnt = canStateTableCnt;
			while (cnt > 0u)  {
				cnt--;
				/* call user indication */
				canStateTable[cnt](newCanState);
			}
#endif /* CO_EVENT_CAN */

#ifdef CO_EVENT_LED
			/* set led */
			switch (newCanState)  {
				case CO_CAN_STATE_BUS_OFF:
					coLedSetState(CO_LED_STATE_ON, CO_TRUE);
					break;
				case CO_CAN_STATE_BUS_ON:
					coLedSetState(CO_LED_STATE_ON, CO_FALSE);
					coLedSetState(CO_LED_STATE_FLASH_1, CO_FALSE);
					break;
				case CO_CAN_STATE_PASSIVE:
					coLedSetState(CO_LED_STATE_FLASH_1, CO_TRUE);
					break;
				case CO_CAN_STATE_UNCHANGED:
				default:
					break;
			}
#endif /* CO_EVENT_LED */

		}
		canState = newCanState;
	}

	/* check for transmit queue event */
	if (commTable[i].type == CST_TRQ)  {
		if (trQuState != newEvent)  {
			trQuState = newEvent;
#ifdef CO_EVENT_CAN
			newState = newEvent;
#endif /* CO_EVENT_CAN */
		}
	}

	/* check for receive queue event */
	if (commTable[i].type == CST_RECQ)  {
		if (recQuState != newEvent)  {
			recQuState = newEvent;
#ifdef CO_EVENT_CAN
			newState = newEvent;
#endif /* CO_EVENT_CAN */
		}
	}

#ifdef CO_EVENT_CAN
	/* call user indication */
	if (newState != CO_COMM_STATE_EVENT_NONE)  {
		cnt = commEventTableCnt;
		while (cnt > 0u)  {
			cnt--;
			/* call user indication */
			commEventTable[cnt](newEvent);
		}
	}
#endif /* CO_EVENT_CAN */
}


#ifdef CO_EVENT_CAN
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \brief coEventRegister_CAN_STATE - register can state changes
*
* With this function the application can register a function which is called,
* when the CAN state was changed.<br>
* CAN states are:<br>
*	- BUS_OFF
*	- BUS_ON
*	- PASSIV
*	- UNCHANGED
*
* \return	RET_T
*
*/

RET_T coEventRegister_CAN_STATE(
		CO_EVENT_CAN_STATE_T pFunction		/**< pointer to function */
    )
{
	/* set new indication function as first at the list */
	if (canStateTableCnt >= CO_EVENT_CAN) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	canStateTable[canStateTableCnt] = pFunction;	/* save function pointer */
	canStateTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief coEventRegister_COMM_EVENT - register communication event changes
*
* With this function the application can register a function which is called,
* when the communication state has been changed.
*	- BUS OFF - no communication possible
*	- CAN OVERRUN - messages was lost
*	- Receice queue full - receive messages is full
*	- Receice queue overrun - receive messages was lost
*	- Transmit queue full - no more messages can be send
*	- Transmit queue overflow - transmit messages was lost
*	- Transmit queue empty - new transmit messages can be send
*
* \return RET_T
*
*/

RET_T coEventRegister_COMM_EVENT(
		CO_EVENT_COMM_T	pFunction	/**< pointer to function */
    )
{
	/* set new indication function as first at the list */
	if (commEventTableCnt >= CO_EVENT_CAN) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	commEventTable[commEventTableCnt] = pFunction;	/* save function pointer */
	commEventTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_CAN */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
