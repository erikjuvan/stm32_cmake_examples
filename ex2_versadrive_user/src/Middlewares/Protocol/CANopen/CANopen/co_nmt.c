////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_nmt.c
*@brief  contains NMT services
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
#include <stdio.h>
#include <stddef.h>
#include "gen_define.h"
#include "co_canopen.h"
#include "co_datatype.h"
#include "co_timer.h"
#include "co_led.h"
#include "co_odaccess.h"
#include "co_drv.h"
#include "co_store.h"
#include "co_commtask.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_event.h"
#include "ico_sdo.h"
#include "ico_pdo.h"
#include "ico_nmt.h"
#include "ico_sync.h"
#include "ico_emcy.h"
#include "ico_odaccess.h"
#include "ico_time.h"
#include "ico_store.h"
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

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static UNSIGNED8        nodeId = 127u;
static CO_NMT_STATE_T   nmtState = CO_NMT_STATE_RESET_NODE;
#ifdef CO_EVENT_NMT
static CO_EVENT_NMT_T   nmtTable[CO_EVENT_NMT];
static UNSIGNED8        nmtTableCnt = 0u;
#endif /* CO_EVENT_NMT */
static COB_REFERENZ_T   nmtCobSlave;
static COB_REFERENZ_T   nmtCobMaster;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void     nmtReset(void);
static RET_T    nmtResetComm(void);
static void     nmtResetAppl(void);

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
void icoNmtMsgHandler(
		CO_CONST CO_REC_DATA_T *pNmtRec		/* pointer to receive data */
	)
{
    RET_T	retVal;

	/* switch depending on cob-id - start with NMT command */
	if (pNmtRec->msg.canCob.canId == 0u)  {

		/* check for correct message len */
		if (pNmtRec->msg.len != 2u)  {
			return;
		}

		/* node id = own node id or = 0 ? */
		if ((pNmtRec->msg.data[1] == nodeId) || (pNmtRec->msg.data[1] == 0u)) {
			/* new state command */
			switch (pNmtRec->msg.data[0])  {
				case 1u:
					icoNmtStateChange(CO_NMT_STATE_OPERATIONAL, CO_TRUE);
					break;
				case 2u:
					icoNmtStateChange(CO_NMT_STATE_STOPPED, CO_TRUE);
					break;
				case 128u:
					icoNmtStateChange(CO_NMT_STATE_PREOP, CO_TRUE);
					break;
				case 129u:
					icoNmtStateChange(CO_NMT_STATE_RESET_NODE, CO_TRUE);
					nmtResetAppl();
					retVal = nmtResetComm();
					if (retVal == RET_OK)  {
						icoNmtStateChange(CO_NMT_STATE_PREOP, CO_TRUE);
					}
					break;
				case 130u:
					retVal = nmtResetComm();
					if (retVal == RET_OK)  {
						icoNmtStateChange(CO_NMT_STATE_PREOP, CO_TRUE);
					}
					break;
				default:
					break;
			}
		}
	}

	/* NMT error control message ? */
	if (pNmtRec->msg.canCob.canId > 0x700u)  {
		/* check for correct message len */
		if (pNmtRec->msg.len != 1u)  {
			return;
		}

		icoNmtErrorCtrlHandler(pNmtRec);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief icoNmtStateChange - setup new state change
*
*
* \return none
*
*/
void icoNmtStateChange(
		CO_NMT_STATE_T	reqState,		/* new requested state */
		BOOL_T			cmdFromMaster	/* command comes from master */
	)
{
    RET_T	retVal;
    #ifdef CO_EVENT_NMT
    UNSIGNED8			cnt;
#endif /* CO_EVENT_NMT */

	/* ignore same command */
	if (nmtState == reqState)  {
		return;
	}

	/* no command from master ? */
	if (cmdFromMaster != CO_TRUE)  {
		/* only in state OPER allowed */
		if (nmtState != CO_NMT_STATE_OPERATIONAL)  {
			return;
		}
	}

	/* normally user indication is ok */
    retVal = RET_OK;

#ifdef CO_EVENT_NMT
	/* only in state OPER the change state can be aborted by user */
	if (reqState == CO_NMT_STATE_OPERATIONAL)  {
		/* check all indications for correct return value */
		cnt = nmtTableCnt;
		while (cnt > 0u)  {
			cnt--;

			/* check user indication */
			retVal = nmtTable[cnt](CO_FALSE, reqState);
			if (retVal != RET_OK)  {
				break;
			}
		}
	}
#endif /* CO_EVENT_NMT */

	/* execute only if retval was ok */
    if (retVal == RET_OK)  {
		/* set new nmt state */
		nmtState = reqState;

#ifdef CO_EVENT_LED
		switch (nmtState)  {
			case CO_NMT_STATE_PREOP:
				coLedSetGreen(CO_LED_STATE_BLINKING);
				break;
			case CO_NMT_STATE_OPERATIONAL:
				coLedSetGreen(CO_LED_STATE_ON);
				break;
			case CO_NMT_STATE_STOPPED:
				coLedSetGreen(CO_LED_STATE_FLASH_1);
				break;
			default:
				break;
		}
#endif /* CO_EVENT_LED */

#ifdef CO_EVENT_NMT
		/* call all user indications */
        cnt = nmtTableCnt;
		while (cnt > 0u)  {
			cnt--;
			/* call user indication */
			(void)nmtTable[cnt](CO_TRUE, reqState);
		}
#endif /* CO_EVENT_NMT */

#ifdef CO_PDO_TRANSMIT_CNT
		/* new state == OPER ? */
		if (reqState == CO_NMT_STATE_OPERATIONAL)  {
			/* transmit async PDOs and start pdo event timer */
			icoPdoOperational();



		} else {
			/* stop event timers */
			icoPdoPreOperational();
		}
#endif /* CO_PDO_TRANSMIT_CNT */

	}
}


#ifdef CO_EVENT_NMT
/***************************************************************************/
/**
* \brief coEventRegister_NMT - register NMT event
*
* register indication function for NMT events
*
* \return RET_T
*
*/

RET_T coEventRegister_NMT(
		CO_EVENT_NMT_T pFunction	/**< pointer to function */
    )
{
	/* set new indication function as first at the list */
	if (nmtTableCnt >= CO_EVENT_NMT) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	nmtTable[nmtTableCnt] = pFunction;	/* save function pointer */
	nmtTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_NMT */


/***************************************************************************/
/**
* \internal
*
* \brief reset application services
*
*
* \return none
*
*/
static void nmtResetAppl(
		void		/**< no parameter */
	)
{
	/* init event handler queue */
	icoEventInit();

#ifdef CO_LSS_SUPPORTED
	icoLssResetAppl();
#endif /* CO_LSS_SUPPORTED */

	icoOdReset(0x2000u, 0x9fffu);
	(void) icoStoreLoadReq(CO_STORE_AREA_APPL);

	coQueueInit();
}


/***************************************************************************/
/**
* \internal
*
* \brief reset communication parameter and services
*
*
* \return none
*
*/
static RET_T nmtResetComm(
		void		/* no parameter */
	)
{
	nodeId=(UNSIGNED8)CO_u8_Node_ID_E;
	icoLssResetAppl();

	/* disable all cobs */
	icoCobDisableAll();

	nmtReset();

#ifdef CO_LSS_SUPPORTED
	icoLssReset(&nodeId);
	if (nodeId == 255u)  {
		return(RET_INVALID_NODEID);
	}
#endif /* CO_LSS_SUPPORTED */

	/* reset objcect dictionary */
	icoOdReset(0x1000u, 0x1fffu);

	/* setup predefined connection set */
	icoSdoServerSetDefaultValue();

#ifdef CO_SDO_CLIENT_CNT
	icoSdoClientSetDefaultValue();
#endif /*  CO_SDO_CLIENT_CNT */

#if defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT)
	icoPdoSetDefaultValue();
#endif /* defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT) */

	icoErrorCtrlSetDefaultValue();

#ifdef CO_SYNC_SUPPORTED
	icoSyncSetDefaultValue();
#endif /* CO_SYNC_SUPPORTED */

#ifdef CO_EMCY_PRODUCER
	icoEmcyProducerSetDefaultValue();
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
	icoEmcyConsumerSetDefaultValue();
#endif /* CO_EMCY_CONSUMER_CNT */

#ifdef CO_TIME_SUPPORTED
	icoTimeSetDefaultValue();
#endif /* CO_TIME_SUPPORTED */

	// load stored parameter 
	(void) icoStoreLoadReq(CO_STORE_AREA_COMM);

	/* call reset services */
	icoSdoServerReset();

#ifdef CO_SDO_CLIENT_CNT
	icoSdoClientReset();
#endif /*  CO_SDO_CLIENT_CNT */

#if defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT)
	icoPdoReset();
#endif /* defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT) */

	icoNmtStateChange(CO_NMT_STATE_RESET_COMM, CO_TRUE);
	icoNmtStateChange(CO_NMT_STATE_PREOP, CO_TRUE);
	icoErrorCtrlReset();

#ifdef CO_SYNC_SUPPORTED
	icoSyncReset();
#endif /* CO_SYNC_SUPPORTED */

#ifdef CO_EMCY_PRODUCER
	icoEmcyReset();
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
	icoEmcyConsumerReset();
#endif /* CO_EMCY_CONSUMER_CNT */

#ifdef CO_TIME_SUPPORTED
	icoTimeReset();
#endif /* CO_TIME_SUPPORTED */

#ifdef CO_LSS_MASTER_SUPPORTED
	icoLssMasterReset();
#endif /* CO_LSS_MASTER_SUPPORTED */

	(void)icoHbProdStart();

	return(RET_OK);
}


/***************************************************************************/
/**
*
* \brief coNmtGetNodeId - returns actual node id 
*
*
* \return node-id
*
*/
UNSIGNED8 coNmtGetNodeId(void)
{
	return(nodeId);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoGetNmtCob - returns nmt cob ident
*
*
* \return COB_REFERENCE_T
*
*/
/***************************************************************************/
/**
* \internal
*
* \brief icoGetNmtCobSlave - returns nmt cob ident
*
*
* \return COB_REFERENCE_T
*
*/
COB_REFERENZ_T icoGetNmtCobSlave(void)
{
	return(nmtCobSlave);
}

/***************************************************************************/
/**
* \internal
*
* \brief icoGetNmtCobMaster - returns nmt cob ident
*
*
* \return COB_REFERENCE_T
*
*/
COB_REFERENZ_T icoGetNmtCobMaster(void)
{
	return(nmtCobMaster);
}

#ifdef CO_LSS_SUPPORTED
/***************************************************************************/
/**
* \internal
*
* \brief icoNmtResetNodeId - reset node id to invalid
*
* Set the internal node-id to 255.
* Communication is not more possible until a reset comm was called
*
* \return none
*
*/
void icoNmtResetNodeId(void)
{
    UNSIGNED8	node;

	node = CO_u8_Node_ID_E;
	if (node != 255u)  { /*lint !e774 depends on LSS configuration */
		return;
	}

	nodeId = 255u;

	/* disable all cobs */
	icoCobDisableAll();

	/* enable NMT COB */
	nmtReset();

	icoLssResetNodeId();
}
#endif /* CO_LSS_SUPPORTED */


/***************************************************************************/
/**
* \brief coGetNmtState - returns current NMT state
*
* This function returns the current NMT state of the local node.
*
* \return NMT state
*
*/
CO_NMT_STATE_T coNmtGetState(void)
{
	return(nmtState);
}


/***************************************************************************/
/**
* \internal
*
* \brief coResetNmt - reset comm for nmt
*
*
* \return none
*
*/
static void nmtReset(void)
{
	nmtState = CO_NMT_STATE_RESET_NODE;

	(void)icoCobSet(nmtCobSlave, 0u, CO_COB_RTR_NONE, 2u);
	(void)icoCobSet(nmtCobMaster, 0u, CO_COB_RTR_NONE, 2u);

#ifdef CO_EVENT_SLEEP
	icoSleepReset();
#endif /* CO_EVENT_SLEEP */
}


/***************************************************************************/
/**
*
* \brief coNmtLocalStateReq - request local NMT state change
*
* Be carfule - NMT commands should be generated only by the master
*
* \return RET_T
*
*/
RET_T coNmtLocalStateReq(
        CO_NMT_STATE_T	reqState		/**< new requested state */
	)
{
    CO_REC_DATA_T	nmtRec;
    UNSIGNED8		cmd;

	switch (reqState)  {
		case CO_NMT_STATE_OPERATIONAL:
			cmd = 1u;
			break;
		case CO_NMT_STATE_STOPPED:
			cmd = 2u;
			break;
		case CO_NMT_STATE_PREOP:
			cmd = 128u;
			break;
		case CO_NMT_STATE_RESET_NODE:
			cmd = 129u;
			break;
		case CO_NMT_STATE_RESET_COMM:
			cmd = 130u;
			break;
		case CO_NMT_STATE_UNKNOWN:
		default:
			return(RET_INVALID_PARAMETER);
	}

	/* simulate command for own node */
	nmtRec.msg.canCob.canId = 0u;
	nmtRec.msg.len = 2u;
	nmtRec.msg.data[0] = cmd;
	nmtRec.msg.data[1] = coNmtGetNodeId();
	icoNmtMsgHandler(&nmtRec);

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief coInitNmt - init NMT functionality
*
* This function initializes the NMT functionality
* and calls an internal reset communication.
*
* If parameter master is unequal 0 the node will be initialized as NMT master.
*
* \return RET_T
*
*/
RET_T coNmtInit(
		UNSIGNED8		master			/**< master mode */
	)
{
	if (master != 0u)
	{
		nmtCobSlave = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_NMT, 0u);
		nmtCobMaster = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_NMT, 0u);
	
		if (nmtCobSlave == 0xffffu)
		{
			return(RET_NO_COB_AVAILABLE);
		}			
		if (nmtCobMaster == 0xffffu)
		{
			return(RET_NO_COB_AVAILABLE);
		}
	}
	else
	{
		nmtCobMaster = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_NMT, 0u);
		if (nmtCobSlave == 0xffffu)
		{
			return(RET_NO_COB_AVAILABLE);
		}
	}

#ifdef CO_EVENT_SLEEP
	if (icoSleepInit(master) != RET_OK)  {
		return(RET_NO_COB_AVAILABLE);
	}
#endif /* CO_EVENT_SLEEP */

	nmtResetAppl();
	(void)nmtResetComm();

	return(RET_OK);
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
