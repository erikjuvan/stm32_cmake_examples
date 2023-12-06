////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_heartbt.c
*@brief  contains heartbeat services
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
#include "gen_define.h"
#include "co_datatype.h"
#include "co_timer.h"
#include "co_odaccess.h"
#include "co_emcy.h"
#include "co_drv.h"
#include "co_led.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_emcy.h"
#include "ico_nmt.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    CO_TIMER_T          timer;
    COB_REFERENZ_T      cob;
    UNSIGNED16          hbTime;
    UNSIGNED8           node;
    UNSIGNED32          odVal;
    CO_NMT_STATE_T      nmtState;
} CO_HB_CONSUMER_T;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_EVENT_ERRCTRL
static CO_EVENT_ERRCTRL_T   errCtrlTable[CO_EVENT_ERRCTRL];
static UNSIGNED8            errCtrlTableCnt = 0u;
#endif /* CO_EVENT_ERRCTRL */
static COB_REFERENZ_T       errCtrlCob;
static UNSIGNED16           hbProdTime;
static CO_TIMER_T           hbProdTimer;
static UNSIGNED8            errorBehavior = 0u;
#ifdef CO_HB_CONSUMER_CNT
static CO_HB_CONSUMER_T     hbConsumer[CO_HB_CONSUMER_CNT];
#endif /* CO_HB_CONSUMER_CNT */
#ifdef CO_GUARDING_SLAVE
static UNSIGNED8            toggleBit;
static UNSIGNED16           guardTime;
static UNSIGNED8            lifeTimeFactor;
static UNSIGNED8            guardFailCnt;
#endif /* CO_GUARDING_SLAVE */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_HB_CONSUMER_CNT
static RET_T    setupHbConsumer(UNSIGNED8  hbConsIdx);
static void     hbConsFailure(void *pData);
#endif /* CO_HB_CONSUMER_CNT */
static void     hbProdSendMessage(void *ptr);
static void     errCtrlInd(UNSIGNED8 node, CO_ERRCTRL_T state,
        CO_NMT_STATE_T nmtState);
#ifdef CO_GUARDING_SLAVE
static void     guardFail(void *ptr);
#endif /* CO_GUARDING_SLAVE */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_HB_CONSUMER_CNT
/***************************************************************************/
/**
*
* \brief coHbConsumerSet - setup heartbeat consumer
*
* This function configures a hearbeat consumer for the given node-id 
* and the monitoring time.
* The data are automatically saved at the object dictionary.
* If an entry at the object dictionary already exist,
* then it will be overwritten.
* <br>The parameter node have to be valid,
* otherwise the function returns an error.
*
* \return RET_T
* \retval RET_PARAMETER_INCOMPATIBLE
*	invalid node id
*
*/

RET_T coHbConsumerSet(
		UNSIGNED8 node,			/**< node id */
		UNSIGNED16 hbTime		/**< heartbeat monitoring time */
	)
{
    UNSIGNED8	ownNodeId;
    UNSIGNED8	idx = 255u;
    UNSIGNED8	i;
    RET_T		retVal;

	/* check for valid node id */
	ownNodeId = coNmtGetNodeId();
	if ((node == 0u) || (node == ownNodeId) || (node > 127u))  {
		return(RET_PARAMETER_INCOMPATIBLE);
	}

	/* node already available ? */
	for (i = 0u; i < CO_HB_CONSUMER_CNT; i++)  {
		if (hbConsumer[i].node == node)  {
			idx = i;
			break;
		}
	}

	/* entry found ? */
	if (idx == 255u)  {
		/* no, try to find a empty one */
		for (i = 0u; i < CO_HB_CONSUMER_CNT; i++)  {
			if (hbConsumer[i].node == 0u)  {
				idx = i;
				break;
			}
		}
	}

	if (idx == 255u)  {
		retVal = RET_EVENT_NO_RESSOURCE;
	} else {
		hbConsumer[idx].odVal = ((UNSIGNED32)node << 16u) | hbTime;
		retVal = setupHbConsumer(idx);
	}
	return(retVal);
}
#endif /* CO_HB_CONSUMER_CNT */


/***************************************************************************/
/**
* \internal
*
* \brief icoHbProdStart - start HB transmission
*
*
* \return RET_T
*
*/
RET_T icoHbProdStart(
		void
	)
{
    RET_T	retVal;

	if (hbProdTime == 0u)  {
		(void)coTimerStop(&hbProdTimer);
		return(RET_OK);
	}

	/* start timer */
	retVal = coTimerStart(&hbProdTimer, (UNSIGNED32)hbProdTime * 1000u,
		hbProdSendMessage, NULL, CO_TIMER_ATTR_ROUNDUP_CYCLIC);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */

	/* send first heartbeat */
	hbProdSendMessage(NULL);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief hbProdSendMessage - send HB message
*
*
* is called from timer
*
* \return none
*
*/
static void hbProdSendMessage(
		void *ptr				/* compatibility entry from timer */
	)
{
    CO_TRANS_DATA_T	trData;
    CO_NMT_STATE_T	nmtState;
    (void)ptr;

	nmtState = coNmtGetState();
	trData.data[0u] = (UNSIGNED8)nmtState;

#ifdef CO_GUARDING_SLAVE
	/* heartbeat disabled */
	if (hbProdTime == 0u)  {
		trData.data[0u] |= toggleBit;
		if (toggleBit == 0u)  {
			toggleBit = 0x80u;
		} else {
			toggleBit = 0x0u;
		}

		/* start guarding timer, if guardingTime and lifeTime is set */
		if ((guardTime != 0u) && (lifeTimeFactor != 0u))  {
			(void)coTimerStart(&hbProdTimer, (UNSIGNED32)guardTime * 1000u,
						guardFail, NULL, CO_TIMER_ATTR_ROUNDUP_CYCLIC);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
			guardFailCnt = 0u;
		}
	}
#endif /* CO_GUARDING_SLAVE */

	(void)icoTransmitMessage(errCtrlCob, &trData, 0u);
}


/***************************************************************************/
/**
* \internal
*
* \brief nmtErrorCtrlHandler - error control handler
*
* is called, if new error control message was received
*
*
* \return none
*
*/
void icoNmtErrorCtrlHandler(
		CO_CONST CO_REC_DATA_T	*pRecData	/* pointer to receive data */
	)
{
#ifdef CO_HB_CONSUMER_CNT
    BOOL_T	startMonitoring = CO_FALSE;
    CO_NMT_STATE_T		oldState;
    CO_HB_CONSUMER_T	*pHbC;
    BOOL_T				timerActive;
#endif /* CO_HB_CONSUMER_CNT */
    UNSIGNED32	ownNodeId;

	/* RTR request ? */
	if (pRecData->msg.canCob.rtr == CO_TRUE)  {
		/* for own node ? */
		ownNodeId = coNmtGetNodeId();
		if (pRecData->msg.canCob.canId == (0x700u + ownNodeId))  {
			hbProdSendMessage(NULL);
		}
		return;
	}

	/* check for correct message len */
	if (pRecData->msg.len != 1u)  {
		return;
	}

	/* unknown bootup message ? */
	if (pRecData->service != CO_SERVICE_ERRCTRL)  {
		if (pRecData->msg.data[0u] == 0u) {
			errCtrlInd((UNSIGNED8)(pRecData->msg.canCob.canId & 0x7fu),
				CO_ERRCTRL_BOOTUP,
				CO_NMT_STATE_PREOP);
		}
		return;
	}

#ifdef CO_HB_CONSUMER_CNT
	pHbC = &hbConsumer[pRecData->spec];
	oldState = pHbC->nmtState;

	/* save nmt state */
	switch (pRecData->msg.data[0u])  {
		case 0u:
			pHbC->nmtState = CO_NMT_STATE_PREOP;
			/* now further indication */
			oldState = pHbC->nmtState;

			/* user indication */
			errCtrlInd(pHbC->node, CO_ERRCTRL_BOOTUP, pHbC->nmtState);
			break;
		case 4u:
			pHbC->nmtState = CO_NMT_STATE_STOPPED;
			startMonitoring = CO_TRUE;
			break;
		case 5u:
			pHbC->nmtState = CO_NMT_STATE_OPERATIONAL;
			startMonitoring = CO_TRUE;
			break;
		case 127u:
			pHbC->nmtState = CO_NMT_STATE_PREOP;
			startMonitoring = CO_TRUE;
			break;
		default:
			pHbC->nmtState = CO_NMT_STATE_UNKNOWN;
			break;
	}

	/* start heartbeat monitoring (again) */
	if (startMonitoring == CO_TRUE)  {

		/* heartbeat already started ? */
		timerActive = coTimerIsActive(&pHbC->timer);

		/* start timer */
		(void)coTimerStart(&pHbC->timer, (UNSIGNED32)pHbC->hbTime * 1000u,
				hbConsFailure, pHbC, CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */

		if (timerActive == CO_FALSE)  {
			/* no, inform user about hearbeat start */
			errCtrlInd(pHbC->node, CO_ERRCTRL_HB_STARTED, pHbC->nmtState);

#ifdef CO_EVENT_LED
			/* set led */
			coLedSetState(CO_LED_STATE_FLASH_2, CO_FALSE);
#endif /* CO_EVENT_LED */
		}

	}

	/* user indication */
	if (oldState != pHbC->nmtState)  {
		/* user indication */
		errCtrlInd(pHbC->node, CO_ERRCTRL_NEW_STATE, pHbC->nmtState);
	}
#endif /* CO_HB_CONSUMER_CNT */
}


#ifdef CO_HB_CONSUMER_CNT
/***************************************************************************/
/**
* \internal
*
* \brief hbConsFailure - heartbeat consumer failure
*
* Function is called, if heartbeat consumer has been failed
*
*
* \return none
*
*/
static void hbConsFailure(
		void *pData				/* pointer heartbeat consumer */
	)
{
    CO_HB_CONSUMER_T	*pHbC;

	pHbC = pData;

	/* no, inform user about hearbeat start */
	errCtrlInd(pHbC->node, CO_ERRCTRL_HB_FAILED, CO_NMT_STATE_UNKNOWN);

#ifdef CO_EMCY_PRODUCER
	{
	UNSIGNED8 strg[5] = { 0u, 0u, 0u, 0u, 0u };

	(void)icoEmcyWriteReq(CO_EMCY_ERRCODE_COMM_ERROR, strg);
	}
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EVENT_LED
	/* set led */
	coLedSetState(CO_LED_STATE_FLASH_2, CO_TRUE);
#endif /* CO_EVENT_LED */

	/* change to preop */
	icoErrorBehavior();
}


/***************************************************************************/
/**
*
* \brief coNmtGetRemoteNodeState - get remote node state
*
* This function returns the NMT state of a remote node.
* If heartbeat monitoring of this node is disabled or has been failed,
* CO_NMT_STATE_UNKNOWN is returned.
*
*
* \return CO_NMT_STATE_T 
*
*/
CO_NMT_STATE_T coNmtGetRemoteNodeState(
		UNSIGNED8 nodeId		/**< remote node id */
	)
{
    UNSIGNED8	cnt;

	for (cnt = 0u; cnt < CO_HB_CONSUMER_CNT; cnt++)  {
		/* node found ? */
		if (hbConsumer[cnt].node == nodeId)  {
			/* heartbeat active ? */
			if (coTimerIsActive(&hbConsumer[cnt].timer) == CO_TRUE)  {
				return(hbConsumer[cnt].nmtState);
			}
		}
	}
	return(CO_NMT_STATE_UNKNOWN);
}
#endif /* CO_HB_CONSUMER_CNT */


/***************************************************************************/
/**
* \internal
*
* \brief icoErrorBehavior - execute error behavoir
*
* Function is called, if communication event is occured.
* Depending on error behavoir, go to preop, stopped or standby at actual state
*
*
* \return none
*
*/
void icoErrorBehavior(
	)
{
	if (coNmtGetState() == CO_NMT_STATE_OPERATIONAL)  {
		if (errorBehavior == 0u)  {
			icoNmtStateChange(CO_NMT_STATE_PREOP, CO_FALSE);
		} else {
			if (errorBehavior == 2u)  {
				icoNmtStateChange(CO_NMT_STATE_STOPPED, CO_FALSE);
			}
		}
	}
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief icoErrCtrlGetObjectAddr - get address of heartbeat entry
*
*
* \return none
*
*/
void *icoErrCtrlGetObjectAddr(
		UNSIGNED16	index,				/* index */
		UNSIGNED8	subIndex			/* subindex */
	)
{
    void	*pAddr = NULL;

	switch (index)  {
#ifdef CO_GUARDING_SLAVE
		case 0x100cu:
			pAddr = (void *)&guardTime;
			break;
		case 0x100du:
			pAddr = (void *)&lifeTimeFactor;
			break;
#endif /* CO_GUARDING_SLAVE */

#ifdef CO_HB_CONSUMER_CNT
		case 0x1016u:
			/* hb consumer */
			pAddr = &hbConsumer[subIndex - 1u].odVal;
			break;
#endif /* CO_HB_CONSUMER_CNT */

		/* hb producer */
		case 0x1017u:
			pAddr = (void *)&hbProdTime;
			break;

		case 0x1029u:
			pAddr = (void *)&errorBehavior;
			break;

		default:
			pAddr = NULL;
			break;
	}

	return(pAddr);
}


#ifdef CO_HB_CONSUMER_CNT
/***************************************************************************/
/**
* \internal
*
* \brief icoErrCtrlCheckObjLimits - check limits
*
*
* \return RET_T
*
*/
RET_T icoErrCtrlCheckObjLimit(
		UNSIGNED8	subIndex,			/* subindex */
		UNSIGNED32	newValue			/* newvalue */
	)
{
    UNSIGNED8	node;
    UNSIGNED16	newTime;
    UNSIGNED8	i;

	node = (UNSIGNED8)((newValue >> 16u) & 0x7fu);
	newTime = (UNSIGNED16)(newValue & 0xffffu);

	/* reset value is already allowed */
	if ((node != 0u) && (newTime != 0u))  {
		/* own node id is not allowed */
		if (node == coNmtGetNodeId())  {
			return(RET_PARAMETER_INCOMPATIBLE);
		}

		/* check, the same node is not in use */
		for (i = 0u; i < CO_HB_CONSUMER_CNT; i++)  {
			/* ignore own subindex */
			if (i != (subIndex - 1u))  {
				/* same node and time != ? */
				if ((hbConsumer[i].node == node) && (hbConsumer[i].hbTime != 0u)) {
					return(RET_PARAMETER_INCOMPATIBLE);
				}
			}
		}
	}

	return(RET_OK);
}
#endif /* CO_HB_CONSUMER_CNT */


/***************************************************************************/
/**
* \internal
*
* \brief icoErrCtrlObjChanged - object was changed
*
*
* \return RET_T
*
*/
RET_T icoErrCtrlObjChanged(
		UNSIGNED16	index,			/* index of changed value */
		UNSIGNED8	subIndex		/* subindex of changed value */
	)
{
    RET_T	retVal = RET_OK;

	switch (index)  {
#ifdef CO_GUARDING_SLAVE
		case 0x100cu:
			/* if heartbeat is disabled and guarding is active,
			 * start guarding timer with new value */
			if (hbProdTime == 0u)  {
				if (coTimerIsActive(&hbProdTimer) == CO_TRUE)  {
					(void)coTimerStart(&hbProdTimer, (UNSIGNED32)guardTime * 1000u,
						guardFail, NULL, CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
				}
			}
			break;
		case 0x100du:
			break;
#endif /* CO_GUARDING_SLAVE */
#ifdef CO_HB_CONSUMER_CNT
		case 0x1016u:
			retVal = setupHbConsumer(subIndex - 1u);
			break;
#endif /* CO_HB_CONSUMER_CNT */

		case 0x1017u:
			retVal = icoHbProdStart();
			break;

		default:
			break;
	}

	return(retVal);
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#ifdef CO_EVENT_ERRCTRL
/***************************************************************************/
/**
* \brief coEventRegister_ERRCTRL - register error control event
*
* \return RET_T
*
*/

RET_T coEventRegister_ERRCTRL(
		CO_EVENT_ERRCTRL_T	pFunction			/**< pointer to function */
    )
{
	if (errCtrlTableCnt >= CO_EVENT_ERRCTRL) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	errCtrlTable[errCtrlTableCnt] = pFunction;	/* save function pointer */
	errCtrlTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_ERRCTRL */


/***************************************************************************/
/**
* \internal
*
* \brief errCtrlInd - call error control user indication
*
*
* \return none
*
*/
static void errCtrlInd(
		UNSIGNED8		node,			/**< node */
		CO_ERRCTRL_T	state,			/**< error control event */
		CO_NMT_STATE_T	nmtState		/**< nmt state */
	)
{
#ifdef CO_EVENT_ERRCTRL
    UNSIGNED8	cnt;

	cnt = errCtrlTableCnt;
	while (cnt > 0u)  {
		cnt--;

		/* check user indication */
		errCtrlTable[cnt](node, state, nmtState);
	}
#endif /* CO_EVENT_ERRCTRL */
}


#ifdef CO_GUARDING_SLAVE
/***************************************************************************/
/**
* \internal
*
* \brief guardFail - master guarding failure
*
*
* \return none
*
*/
static void guardFail(
		void *ptr
	)
{
	(void)ptr;

	if ((guardTime == 0u) || (lifeTimeFactor == 0u))  {
		return;
	}

	guardFailCnt++; 
	if (guardFailCnt >= lifeTimeFactor)  {
		errCtrlInd(0u, CO_ERRCTRL_GUARD_FAILED, CO_NMT_STATE_UNKNOWN);

#ifdef CO_EMCY_PRODUCER
		{
		UNSIGNED8 strg[5] = { 0u, 0u, 0u, 0u, 0u };

		(void)icoEmcyWriteReq(CO_EMCY_ERRCODE_COMM_ERROR, strg);
		}
#endif /* CO_EMCY_PRODUCER */
		/* change to preop */
		icoErrorBehavior();
	} else {
		(void)coTimerStart(&hbProdTimer, (UNSIGNED32)guardTime * 1000u,
				guardFail, NULL, CO_TIMER_ATTR_ROUNDUP);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
	}
}
#endif /* CO_GUARDING_SLAVE */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief coErrCtrlReset - reset comm for errctrl
*
*
* \return none
*
*/
void icoErrorCtrlReset(
		void
	)
{
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	nodeId;
#ifdef CO_HB_CONSUMER_CNT
    UNSIGNED8	i;
#endif /* CO_HB_CONSUMER_CNT */

	nodeId = coNmtGetNodeId();
	(void)icoCobSet(errCtrlCob, 0x700ul + nodeId, CO_COB_RTR, 1u);

	/* send bootup message */
	trData.data[0u] = 0u;
	(void)icoTransmitMessage(errCtrlCob, &trData, 0u);

#ifdef CO_HB_CONSUMER_CNT
	/* reset heartbeat consumer */
	for (i = 0u; i < CO_HB_CONSUMER_CNT; i++)  {
		(void)setupHbConsumer(i);
	}
#endif /* CO_HB_CONSUMER_CNT */
}


/***************************************************************************/
/**
* \internal
*
* \brief coErrCtrlSetDefaultValue
*
*
* \return none
*
*/
void icoErrorCtrlSetDefaultValue(
		void
	)
{
    RET_T	retVal;
    UNSIGNED16	u16;
#ifdef CO_HB_CONSUMER_CNT
    UNSIGNED8	i;
    UNSIGNED32	u32;
#endif /* CO_HB_CONSUMER_CNT */
#ifdef CO_GUARDING_SLAVE
    UNSIGNED8	u8;
#endif /* CO_GUARDING_SLAVE */

	/* reset heartbeat producer */
	retVal = coOdGetDefaultVal_u16(0x1017u, 0u, &u16);
	if (retVal == RET_OK)  {
		hbProdTime = u16;
	}

#ifdef CO_GUARDING_SLAVE
	toggleBit = 0u;
	retVal = coOdGetDefaultVal_u16(0x100cu, 0u, &u16);
	if (retVal == RET_OK)  {
		guardTime = u16;
	}
	retVal = coOdGetDefaultVal_u8(0x100du, 0u, &u8);
	if (retVal == RET_OK)  {
		lifeTimeFactor = u8;
	}
#endif /* CO_GUARDING_SLAVE */

#ifdef CO_HB_CONSUMER_CNT
	/* reset heartbeat consumer */
	for (i = 0u; i < CO_HB_CONSUMER_CNT; i++)  {
		retVal = coOdGetDefaultVal_u32(0x1016u, i+1u, &u32);
		if (retVal == RET_OK)  {
			hbConsumer[i].odVal = u32;
		}
	}
#endif /* CO_HB_CONSUMER_CNT */

	(void)coOdGetDefaultVal_u8(0x1029u, 1u, &errorBehavior);
}


#ifdef CO_HB_CONSUMER_CNT
/***************************************************************************/
/**
* \internal
*
* \brief setupHbConsumer - setup a heartbeat consumer
*
*
* \return none
*
*/
static RET_T setupHbConsumer(
		UNSIGNED8	hbConsIdx		/* heartbeat consumer index */
	)
{
    RET_T	retVal = RET_OK;

	/* stop timer alltimes, so the monitoring starts again */
	(void)coTimerStop(&hbConsumer[hbConsIdx].timer);

	hbConsumer[hbConsIdx].node = (UNSIGNED8)((hbConsumer[hbConsIdx].odVal >> 16u) & 0x7fu);
	hbConsumer[hbConsIdx].hbTime = (UNSIGNED16)(hbConsumer[hbConsIdx].odVal & 0xffffu);

	if ((hbConsumer[hbConsIdx].node == 0u)
	 || (hbConsumer[hbConsIdx].hbTime == 0u))  {
		/* disable HB consumer */
		retVal = icoCobSet(hbConsumer[hbConsIdx].cob, 0x80000000ul,
			CO_COB_RTR_NONE, 1u);
		return(retVal);
	}

	/* setup monitoring */
	retVal = icoCobSet(hbConsumer[hbConsIdx].cob,
		0x700ul + (UNSIGNED32)hbConsumer[hbConsIdx].node, CO_COB_RTR_NONE, 1u);
	hbConsumer[hbConsIdx].nmtState = CO_NMT_STATE_UNKNOWN;

	return(retVal);
}
#endif /* CO_HB_CONSUMER_CNT */


/***************************************************************************/
/**
* \brief coInitNmt - init error control
*
* Setup error control handling for local node (transmit heartbeat)
* and remote node (heartbeat monitoring)
*
* \return RET_T
*
*/
RET_T coErrorCtrlInit(
		UNSIGNED16	hbTime,				/**< heartbeat producer time */
		UNSIGNED8	hbConsCnt			/**< heartbeat consumer count */
	)
{
#ifdef CO_HB_CONSUMER_CNT
    UNSIGNED8	i;
#endif /* CO_HB_CONSUMER_CNT */

	errCtrlCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_ERRCTRL, 0u);
	if (errCtrlCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}
    hbProdTime = hbTime;

#ifdef CO_GUARDING_SLAVE
	toggleBit = 0u;
#endif /* CO_GUARDING_SLAVE */

#ifdef CO_HB_CONSUMER_CNT
	for (i = 0u; i < hbConsCnt; i++)  {
		hbConsumer[i].cob = icoCobCreate(CO_COB_TYPE_RECEIVE,
				CO_SERVICE_ERRCTRL, (UNSIGNED16)i);
		if (hbConsumer[i].cob == 0xffffu)  {
			return(RET_NO_COB_AVAILABLE);
		}
	}
#endif /* CO_HB_CONSUMER_CNT */

	(void)coOdGetDefaultVal_u8(0x1029u, 1u, &errorBehavior);

	return(RET_OK);
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
