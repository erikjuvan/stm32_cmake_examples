////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_emcy.c
*@brief  contains emcy routines
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
#include "co_emcy.h"
#include "co_drv.h"
#include "co_timer.h"
#include "co_odaccess.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_nmt.h"
#include "ico_emcy.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_EMCY_CONSUMER_CNT
typedef struct {
    UNSIGNED32 cobId;
    COB_REFERENZ_T  cob;
} CO_EMCY_CONSUMER_T;
#endif /* CO_EMCY_CONSUMER_CNT */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static UNSIGNED8        errorRegister = 0u;
#ifdef CO_EMCY_PRODUCER
static UNSIGNED32       emcyCobId = 0ul;
static COB_REFERENZ_T   emcyCob;
# ifdef CO_EMCY_ERROR_HISTORY
static UNSIGNED32       errorHistory[CO_EMCY_ERROR_HISTORY];
static UNSIGNED8        errorHistoryCnt = 0u;
# endif /* CO_EMCY_ERROR_HISTORY */
static UNSIGNED16       inhibit = 0u;
# ifdef CO_EVENT_EMCY
static CO_EVENT_EMCY_T  emcyTable[CO_EVENT_EMCY];
static UNSIGNED8        emcyTableCnt = 0u;
# endif /* CO_EVENT_EMCY */
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
static UNSIGNED8        emcyConsumerCnt = 0u;
static CO_EMCY_CONSUMER_T   emcyCons[CO_EMCY_CONSUMER_CNT];
# ifdef CO_EVENT_EMCY_CONSUMER
static CO_EVENT_EMCY_CONS_T emcyConsTable[CO_EVENT_EMCY_CONSUMER];
static UNSIGNED8            emcyConsTableCnt = 0u;
# endif /* CO_EVENT_EMCY_CONSUMER */
#endif /* CO_EMCY_CONSUMER_CNT */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_EMCY_PRODUCER
/***************************************************************************/
/**
* \brief coEmcyWriteReq - write an emergency message
*
* With this function, an emergency message can be send.<br>
* The message is automatically composed and transmitted by the given parameter 
* and the error register value (index 0x1001:0).<br>
* After that, the error history (index 0x1003:n) is updated
* with the new data.
*
* The error register (index 0x1001:0) has to be updated by the application.
*
* \return RET_T
*
*/
RET_T coEmcyWriteReq(
		UNSIGNED16	emcyErrCode,		/**< emergency error code */
		CO_CONST UNSIGNED8	pData[]		/**< pointer to additional 5 data bytes */
	)
{
    RET_T	retVal;
    CO_TRANS_DATA_T	trData;
    CO_NMT_STATE_T	nmtState;
    UNSIGNED8	i;
#ifdef CO_EMCY_ERROR_HISTORY
    UNSIGNED8	cnt;
    UNSIGNED32	u32;
#endif /* CO_EMCY_ERROR_HISTORY */

	/* handling of generic error bit */
	/* set it if one of the other bits are set */
	if ((errorRegister & 0xfeu) == 0u)  {
		errorRegister = 0u;
	} else {
		errorRegister |= 1u;
	}

	/* OPERATIONAL ? */
	nmtState = coNmtGetState();
	if ((nmtState != CO_NMT_STATE_PREOP)
	 && (nmtState != CO_NMT_STATE_OPERATIONAL))  {
		return(RET_INVALID_NMT_STATE);
	}

	trData.data[0] = (UNSIGNED8)(emcyErrCode & 0xffu);
	trData.data[1] = (UNSIGNED8)(emcyErrCode >> 8u);
	trData.data[2] = errorRegister;
	/* set generic error bit temporyry if error code != 0x00xx */
	if ((emcyErrCode & 0xff00u) != 0u)  {
		trData.data[2] |= 1u;
	}
	
	for (i = 0u; i < 5u; i++)  {
		trData.data[3u + i] = pData[i];
	}

	/* transmit data */
	retVal = icoTransmitMessage(emcyCob, &trData, 0u);

#ifdef CO_EMCY_ERROR_HISTORY
	/* save at error history */
	cnt = errorHistoryCnt;
	if (errorHistoryCnt >= CO_EMCY_ERROR_HISTORY)  {
		cnt--;
	}
	while (cnt > 0u)  {
		/* access to out-of-bounds not possible while cnt > 0 */
		errorHistory[cnt] = errorHistory[cnt - 1u]; /*lint !e661 out-of-bound ensured */
		cnt--;
	} 
	u32 = (UNSIGNED32)emcyErrCode | ((UNSIGNED32)pData[1u] << 16u) | ((UNSIGNED32)pData[0u] << 24u);
	errorHistory[0u] = u32;
	if (errorHistoryCnt < CO_EMCY_ERROR_HISTORY)  {
		errorHistoryCnt++;
	}
#endif /* CO_EMCY_ERROR_HISTORY */

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoEmcyWriteReq - internal write of an emergency message
*
* This function writes an emergency message.
*
* \return RET_T
*
*/
RET_T icoEmcyWriteReq(
		UNSIGNED16	emcyErrCode,	/* emergency error code */
		UNSIGNED8	*pAddErrorBytes	/* pointer to additional 5 data bytes */
	)
{
    RET_T	retVal;

#ifdef CO_EVENT_EMCY
    UNSIGNED8	cnt;

	/* call all user indications */
	cnt = emcyTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		retVal = emcyTable[cnt](emcyErrCode, pAddErrorBytes);
		if (retVal != RET_OK)  {
			return(RET_INTERNAL_ERROR);
		}
	}
#endif /* CO_EVENT_EMCY */

	retVal = coEmcyWriteReq(emcyErrCode, pAddErrorBytes);

	return(retVal);
}


# ifdef CO_EVENT_EMCY
/***************************************************************************/
/**
* \brief coEventRegister_EMCY - register emergency event function
*
* This function registers an emergency indication function.
*
* \return RET_T
*
*/
RET_T coEventRegister_EMCY(
		CO_EVENT_EMCY_T pFunction
	)
{
	if (emcyTableCnt >= CO_EVENT_EMCY) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	emcyTable[emcyTableCnt] = pFunction;	/* save function pointer */
	emcyTableCnt++;

	return(RET_OK);
}
# endif /* CO_EVENT_EMCY */
#endif /* CO_EMCY_PRODUCER */


#ifdef CO_EMCY_CONSUMER_CNT
void icoEmcyConsumerHandler(
		const CO_REC_DATA_T	*pRecData
	)
{
# ifdef CO_EVENT_EMCY_CONSUMER
    UNSIGNED8	cnt;
# endif /* CO_EVENT_EMCY_CONSUMER */

	/* check for correct message len */
	if (pRecData->msg.len != 8u)  {
		return;
	}

	if (pRecData->spec > emcyConsumerCnt)  {
		return;
	}

# ifdef CO_EVENT_EMCY_CONSUMER
	/* call all user indications */
	cnt = emcyConsTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		emcyConsTable[cnt]((UNSIGNED8)(pRecData->msg.canCob.canId & 0x7fu),
			pRecData->msg.data[0]
				| (UNSIGNED16)((UNSIGNED16)pRecData->msg.data[1] << 8u),
			pRecData->msg.data[2],
			&pRecData->msg.data[3]);
	}
# endif /* CO_EVENT_EMCY_CONSUMER */
}


# ifdef CO_EVENT_EMCY_CONSUMER
/***************************************************************************/
/**
* \brief coEventRegister_EMCY_CONSUMER - register emergency consumer event function
*
* This function registers an emergency consumer indication function.
*
* \return RET_T
*
*/
RET_T coEventRegister_EMCY_CONSUMER(
		CO_EVENT_EMCY_CONS_T pFunction
	)
{
	if (emcyConsTableCnt >= CO_EVENT_EMCY_CONSUMER) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	emcyConsTable[emcyConsTableCnt] = pFunction;	/* save function pointer */
	emcyConsTableCnt++;

	return(RET_OK);
}
# endif /* CO_EVENT_EMCY_CONSUMER */
#endif /* CO_EMCY_CONSUMER_CNT */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief icoEmcyGetObjectAddr - get Emcy object address
*
*
* \return RET_T
*
*/
void *icoEmcyGetObjectAddr(
		UNSIGNED16	idx,				/* index */
		UNSIGNED8	subIndex			/* subindex */
	)
{
    void	*pAddr = NULL;
    (void)subIndex;

	switch (idx) {
		case 0x1001u:		/* error register */
			pAddr = (void *)&errorRegister;
			break;

#ifdef CO_EMCY_PRODUCER
# ifdef CO_EMCY_ERROR_HISTORY
		case 0x1003u:		/* error history */
			if (subIndex == 0u)  {
				pAddr = &errorHistoryCnt;
			} else {
				if (subIndex > errorHistoryCnt)  {
					pAddr = NULL;
				} else {
					pAddr = &errorHistory[subIndex - 1u];
				}
			}
			break;
# endif /* CO_EMCY_ERROR_HISTORY */
		case 0x1014u:	/* cob-id emcy producer */
			pAddr = &emcyCobId;
			break;
		case 0x1015u:	/* inhibit */
			pAddr = &inhibit;
			break;
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
		case 0x1028u:	/* emcy consumer */
			pAddr = (void *)&emcyCons[subIndex - 1u].cobId;
			break;
#endif /* CO_EMCY_CONSUMER_CNT */

		default:
			break;
	}

	return(pAddr);
}


#if defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT)
/***************************************************************************/
/**
* \internal
*
* \brief icoEmcyCheckObjLimitCobId - check cob-id
*
*
* \return RET_T
*
*/
RET_T icoEmcyCheckObjLimitCobid(
		UNSIGNED16	index,		/* index */
		UNSIGNED8	subIndex,	/* subIndex */
		UNSIGNED32	canId		/* new cob ID*/
	)
{
    RET_T	retVal = RET_OK;
    (void)subIndex;

#ifdef CO_EMCY_PRODUCER
	if (index == 0x1014u)  {
		/* write the same value again is allowed */
		if (canId != emcyCobId)  {
			if ((canId & CO_COB_VALID_MASK) == 0u)  {
				/* new cobid is valid, only allowed if cob was disabled before*/
				if ((emcyCobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
					return(RET_SDO_INVALID_VALUE);
				}
			}
			if (icoCheckRestrictedCobs(canId, 0x81u, 0x81u) == CO_TRUE)  {
				return(RET_SDO_INVALID_VALUE);
			}
		}
	}
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
	if (index == 0x1028u)  {
		if ((canId & CO_COB_VALID_MASK) == 0u)  {
			/* new cobid is valid, only allowed if cob was disabled before*/
			if ((emcyCons[subIndex -1u].cobId & CO_COB_VALID_MASK)
					!= CO_COB_INVALID)  {
				return(RET_SDO_INVALID_VALUE);
			}
		}
	}
#endif /* CO_EMCY_CONSUMER_CNT */

	return(retVal);
}



/***************************************************************************/
/**
* \internal
*
* \brief icoEmcyObjChanged - EMCY object was changed
*
*
* \return RET_T
*
*/
RET_T icoEmcyObjChanged(
		UNSIGNED16	index,			/* object index */
		UNSIGNED8	subIndex		/* object subIndex */
	)
{
    RET_T	retVal = RET_OK;
    (void)subIndex;

	switch (index)  {
#ifdef CO_EMCY_PRODUCER
# ifdef CO_EMCY_ERROR_HISTORY
		case 0x1003u:
			errorHistoryCnt = 0u;
			break;
# endif /* CO_EMCY_ERROR_HISTORY */
		case 0x1014u:
			retVal = icoCobSet(emcyCob, emcyCobId, CO_COB_RTR_NONE, 8u);
			break;
		case 0x1015u:	/* inhibit */
			retVal = icoCobSetInhibit(emcyCob, inhibit);
			break;
#endif /* CO_EMCY_PRODUCER */

#ifdef CO_EMCY_CONSUMER_CNT
		case 0x1028u:	/* emcy consumer */
			(void)icoCobSet(emcyCons[subIndex - 1u].cob,
					emcyCons[subIndex - 1u].cobId, CO_COB_RTR_NONE, 8u);
			break;
#endif /* CO_EMCY_CONSUMER_CNT */
		default:
			break;
	}

	return(retVal);
}
#endif /* defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT) */


#ifdef CO_EMCY_PRODUCER
/***************************************************************************/
/**
* \internal
*
* \brief icoEmcyCheckObjLimitHist - check object limits u8 value
*
*
* \return RET_T
*
*/
RET_T icoEmcyCheckObjLimitHist(
		UNSIGNED8	subIndex,	/* subindex */
		UNSIGNED8	u8			/* received data */
	)
{
    RET_T	retVal = RET_OK;

	/* sync start value */
	if ((subIndex != 0u) || (u8 != 0u))  {
		retVal = RET_SDO_INVALID_VALUE;
	}
	return(retVal);
}


/***************************************************************************/
/**
*
* \internal
*
* \brief coEmcyReset - reset comm for Emcy
*
* \return none
*/
void icoEmcyReset(
		void
	)
{
	(void)icoCobSet(emcyCob, emcyCobId, CO_COB_RTR_NONE, 8u);
}


/***************************************************************************/
/**
*
* \internal
*
* \brief coEmcySetDefaultValue - set default (predefined connection set)
*
* \return none
*/
void icoEmcyProducerSetDefaultValue(
		void
	)
{
    RET_T	retVal;
    UNSIGNED16	u16;
    UNSIGNED32	u32;

	emcyCobId = 0x80ul + coNmtGetNodeId();

	retVal = coOdGetDefaultVal_u32(0x1014u, 0u, &u32);
	if (retVal == RET_OK)  {
		if ((u32 & CO_COB_VALID_MASK) != 0u)  {
			/* cob invalid */
			emcyCobId |= CO_COB_INVALID;
		}
	}
	retVal = coOdGetDefaultVal_u16(0x1015u, 0u, &u16);
	if (retVal == RET_OK)  {
		inhibit = u16;
		(void)icoCobSetInhibit(emcyCob, inhibit);
	}

	return;
}


/***************************************************************************/
/**
* \brief coEmcyProducerInit - initialization for emergency producer
*
* This function initializes the emergency producer functionality.
*
* \return RET_T
*
*/
RET_T coEmcyProducerInit(
		void
	)
{
	emcyCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_EMCY_TRANSMIT, 0u);
	if (emcyCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	return(RET_OK);
}

#endif /* CO_EMCY_PRODUCER */


#ifdef CO_EMCY_CONSUMER_CNT
/***************************************************************************/
/**
* \internal
*
* \brief coEmcyConsumerReset - reset comm for Emcy
*
* \return none
*/
void icoEmcyConsumerReset(
		void
	)
{
    UNSIGNED8	i;

	for (i = 0u; i < emcyConsumerCnt; i++)  {
		(void)icoCobSet(emcyCons[i].cob, emcyCons[i].cobId, CO_COB_RTR_NONE, 8u);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief coEmcyConsumerSetDefaultValue - reset comm for Emcy
*
* \return none
*/
void icoEmcyConsumerSetDefaultValue(
		void
	)
{
    RET_T	retVal;
    UNSIGNED8	i;

	for (i = 0u; i < emcyConsumerCnt; i++)  {
		retVal = coOdGetDefaultVal_u32(0x1028u, i + 1u, &emcyCons[i].cobId);
		if (retVal != RET_OK)  {
			emcyCons[i].cobId |= CO_COB_INVALID;
		}
	}
}


/***************************************************************************/
/**
* \brief coEmcyConsumerInit - initialization for emergency consumer
*
* This function initializes the emergency consumers.
*
* \return RET_T
*
*/
RET_T coEmcyConsumerInit(
		UNSIGNED8	emcyCnt			/**< number of emergency consumers */
	)
{
    UNSIGNED8	cnt;

	if (emcyCnt > CO_EMCY_CONSUMER_CNT)  {
		return(RET_EVENT_NO_RESSOURCE);
	}
 
	for (cnt = 0u; cnt < emcyCnt; cnt++)  {
		emcyCons[cnt].cob = icoCobCreate(CO_COB_TYPE_RECEIVE,
				CO_SERVICE_EMCY_RECEIVE, (UNSIGNED16)cnt);
		if (emcyCons[cnt].cob == 0xffffu)  {
			return(RET_NO_COB_AVAILABLE);
		}
	}

	emcyConsumerCnt = emcyCnt;

	return(RET_OK);
}
#endif /* CO_EMCY_CONSUMER_CNT */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

