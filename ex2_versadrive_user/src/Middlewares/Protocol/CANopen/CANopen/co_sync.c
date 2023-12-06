////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sync.c
*@brief  contains SYNC services
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
#ifdef CO_SYNC_SUPPORTED
#include "co_datatype.h"
#include "co_timer.h"
#include "co_odaccess.h"
#include "co_sync.h"
#include "co_drv.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_pdo.h"
#include "ico_nmt.h"
#include "ico_sync.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define CO_SYNC_PRODUCER_BIT	0x40000000ul

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static COB_REFERENZ_T   syncCob;
static UNSIGNED32       syncCobId = 0x80ul;
static UNSIGNED32       cyclePeriod = 0ul;
static UNSIGNED8        counterValue = 0u;
static UNSIGNED8        actCounterValue = 1u;
static UNSIGNED32       windowLength = 0ul;
static CO_TIMER_T       syncTimer;

# ifdef CO_EVENT_SYNC
static UNSIGNED16       syncEventTableCnt = 0u;
static CO_EVENT_SYNC_T  syncEventTable[CO_EVENT_SYNC];
# endif /* CO_EVENT_SYNC */

# ifdef CO_EVENT_SYNC_FINISHED
static CO_EVENT_SYNC_FINISHED_T syncFinishedEventTable[CO_EVENT_SYNC_FINISHED];
static UNSIGNED16               syncFinishedEventTableCnt = 0u;
# endif /* CO_EVENT_SYNC_FINISHED */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void syncEventTimer(void *ptr);

# ifdef CO_EVENT_SYNC
static void syncInd(void);
# endif /* CO_EVENT_SYNC */

# ifdef CO_EVENT_SYNC_FINISHED
static void syncFinishedInd(void);
# endif /* CO_EVENT_SYNC_FINISHED */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \internal
*
* \brief icoSynandler - handle SYNC
*
*
* \return none
*
*/
void icoSyncHandler(
		UNSIGNED8	syncCounterVal			/* sync counter value */
	)
{
# ifdef CO_EVENT_SYNC
	/* start user indication */
	syncInd();
# endif /* CO_EVENT_SYNC */

# if defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT)
	/* work PDO handling */
	if (counterValue != 0u)  {
		icoPdoSyncHandling(syncCounterVal);
	} else {
		icoPdoSyncHandling(0u);
	}
# endif /* defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT) */

# ifdef CO_EVENT_SYNC_FINISHED
	/* end user indication */
	syncFinishedInd();
# endif /* CO_EVENT_SYNC_FINISHED */
}


/***************************************************************************/
/**
* \internal
*
* \brief syncEventTimer - called from timer to transmit sync message
*
*
* \return none
*
*/
static void syncEventTimer(
		void *ptr			/* compatibility entry */
	)
{
    CO_TRANS_DATA_T	trData;
    CO_NMT_STATE_T	nmtState;
    (void)ptr;

	/* OPERATIONAL ? */
	nmtState = coNmtGetState();
	if ((nmtState != CO_NMT_STATE_PREOP)
	 && (nmtState != CO_NMT_STATE_OPERATIONAL))  {
		return;
	}

	trData.data[0] = actCounterValue;
	actCounterValue ++;
	if (actCounterValue > counterValue)  {
		actCounterValue = 1u;
	}

	/* transmit data */
	(void) icoTransmitMessage(syncCob, &trData, MSG_OVERWRITE);

	icoSyncHandler(trData.data[0]);

	return;
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief icoSyncGetObjectAddr - get sync object address
*
* \return none
*
*/
void *icoSyncGetObjectAddr(
		UNSIGNED16	index			/* index */
	)
{
    void	*pAddr = NULL;

	switch (index)  {
		case 0x1005u:
			pAddr = (void *)&syncCobId;
			break;
		case 0x1006u:
			pAddr = (void *)&cyclePeriod;
			break;
		case 0x1007u:
			pAddr = (void *)&windowLength;
			break;
		case 0x1019u:
			pAddr = (void *)&counterValue;
			break;
		default:
			break;
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSyncCheckObjLimit_u8 - check UNSIGNED8 object limits
*
*
* \return RET_T
*
*/
RET_T	icoSyncCheckObjLimit_u8(
		UNSIGNED16	index,			/* index */
		UNSIGNED8	value			/* new value */
	)
{
	if (index == 0x1019u)  {
		if ((value == 1u) || (value > 240u))  {
			return(RET_SDO_INVALID_VALUE);
		}
		if (cyclePeriod != 0u)  {
			return(RET_ERROR_PRESENT_DEVICE_STATE);
		}
	}

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSyncCheckObjLimit_u32 - check UNSIGNED32 object limits
*
*
* \return RET_T
*
*/
RET_T	icoSyncCheckObjLimit_u32(
		UNSIGNED16	index,			/* index */
		UNSIGNED32	value			/* new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDescPtr;

	if (index == 0x1005u)  {
		/* sync producer only allowed if 1006 is available */
		if ((value & CO_SYNC_PRODUCER_BIT) != 0u)  {
			if (coOdGetObjDescPtr(0x1006u, 0u, &pDescPtr) != RET_OK)  {
				return(RET_SDO_INVALID_VALUE);
			}
		}
		/* check maximum value */
		if (value > 0x4ffffffful)  {
			return(RET_SDO_INVALID_VALUE);
		}
		/* check restricted CAN ids */
		if (icoCheckRestrictedCobs(value, 0x80u, 0x80u) == CO_TRUE)  {
			return(RET_SDO_INVALID_VALUE);
		}
	}

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSyncObjChanged - sync object changed
*
*
* \return RET_T
*
*/
RET_T icoSyncObjChanged(
		UNSIGNED16	valIdx				/* index */
	)
{
    RET_T	retVal = RET_OK;

	switch (valIdx)  {
		case 0x1005u:	/* cob-id or cycle period */
		case 0x1006u:
			if ((cyclePeriod != 0u)
			 && ((syncCobId & CO_SYNC_PRODUCER_BIT) != 0u))  {
				/* sync producer */
				retVal = icoCobChangeType(syncCob, CO_COB_TYPE_TRANSMIT);
				if (retVal != RET_OK)  {
					return(retVal);
				}
				if (counterValue != 0u)  {
					retVal = icoCobSet(syncCob, syncCobId, CO_COB_RTR_NONE, 1u);
				} else {
					retVal = icoCobSet(syncCob, syncCobId, CO_COB_RTR_NONE, 0u);
				}
				if (retVal != RET_OK)  {
					return(retVal);
				}
				/* start timer */
				retVal = coTimerStart(&syncTimer, cyclePeriod,
					syncEventTimer, NULL, CO_TIMER_ATTR_ROUNDDOWN_CYCLIC);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
			} else {
				retVal = icoCobChangeType(syncCob, CO_COB_TYPE_RECEIVE);
				if (retVal != RET_OK)  {
					return(retVal);
				}
				if (counterValue != 0u)  {
					retVal = icoCobSet(syncCob, syncCobId, CO_COB_RTR_NONE, 1u);
				} else {
					retVal = icoCobSet(syncCob, syncCobId, CO_COB_RTR_NONE, 0u);
				}
				(void) coTimerStop(&syncTimer);
			}
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
# ifdef CO_EVENT_SYNC
/***************************************************************************/
/**
* \brief coEventRegister_SYNC - register SYNC event
*
* This function registers an indication function for SYNC events.
*
* It is called every time a sync message was received or generated
* before PDOs are handled.
*
* \return RET_T
*
*/

RET_T coEventRegister_SYNC(
		CO_EVENT_SYNC_T pFunction	/**< pointer to function */
    )
{
	if (syncEventTableCnt >= CO_EVENT_SYNC) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	syncEventTable[syncEventTableCnt] = pFunction;
	syncEventTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief syncInd - call sync indications
*
* \return
*	RET_T
*/
static void syncInd(
		void
	)
{
    UNSIGNED16	cnt;

	/* call indication to execute */
	cnt = syncEventTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		syncEventTable[cnt]();
	}

	return;
}
# endif /* CO_EVENT_SYNC */


# ifdef CO_EVENT_SYNC_FINISHED
/***************************************************************************/
/**
* \brief coEventRegister_SYNC_FINISHED - register SYNC finished event
*
* This function registers an indication function for finished SYNC handling.
*
* It is called every time a sync message was received or generated
* and PDO handling is completed.
*
* \return RET_T
*
*/

RET_T coEventRegister_SYNC_FINISHED(
		CO_EVENT_SYNC_FINISHED_T pFunction	/**< pointer to function */
    )
{
	if (syncFinishedEventTableCnt >= CO_EVENT_SYNC_FINISHED) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	syncFinishedEventTable[syncFinishedEventTableCnt] = pFunction;
	syncFinishedEventTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief syncFinishedInd - call sync finish indication
*
* \return
*	RET_T
*/
static void syncFinishedInd(void)
{
    UNSIGNED16	cnt;

	/* call indication to execute */
	cnt = syncFinishedEventTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		syncFinishedEventTable[cnt]();
	}

	return;
}
# endif /* CO_EVENT_SYNC_FINISHED */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief coResetNmt - reset comm for nmt
*
* \return none
*
*/
void icoSyncReset(void)
{
	(void) icoSyncObjChanged(0x1005u);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSyncSetDefaultValue - set predefined connection set
*
* \return none
*
*/
void icoSyncSetDefaultValue(void)
{
    RET_T	    retVal;
    UNSIGNED32	u32;
    UNSIGNED8	u8;

	/* setup reset values */
	retVal = coOdGetDefaultVal_u32(0x1005u, 0u, &u32);
	if (retVal == RET_OK)  {
		syncCobId = u32;
	} else {
		syncCobId = 0x80ul;
	}
	retVal = coOdGetDefaultVal_u32(0x1006u, 0u, &u32);
	if (retVal == RET_OK)  {
		cyclePeriod = u32;
	}
	retVal = coOdGetDefaultVal_u32(0x1007u, 0u, &u32);
	if (retVal == RET_OK)  {
		windowLength = u32;
	}
	retVal = coOdGetDefaultVal_u8(0x1019u, 0u, &u8);
	if (retVal == RET_OK)  {
		counterValue = u8;
	}
}


/***************************************************************************/
/**
* \brief coSyncInit - init sync functionality
*
* This function initializes the SYNC functionality.
*
* If the node is a sync producer or a sync consumer
* depends on the value of the object dictionary index 0x1005.
* <br>Sync counter value can also be set/reset by the value
* at the object dictionary at index 0x1019
*
* \return RET_T
*
*/
RET_T coSyncInit(
		UNSIGNED32	cobId					/**< sync cob-id */
	)
{
	/* if cyclePeriod != 0 than sync producer */
	if ((cobId & CO_SYNC_PRODUCER_BIT) != 0u)  {
		syncCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_SYNC_TRANSMIT, 0u);
	} else {
		syncCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_SYNC_RECEIVE, 0u);
	}
	if (syncCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	return(RET_OK);
}
#endif /* CO_SYNC_SUPPORTED */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
