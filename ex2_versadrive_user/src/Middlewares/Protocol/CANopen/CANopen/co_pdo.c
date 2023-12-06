////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_pdo.c
*@brief  contains pdo routines
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
#include <string.h>
#include "gen_define.h"
#if defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT)
#include "co_datatype.h"
#include "co_timer.h"
#include "co_pdo.h"
#include "co_emcy.h"
#include "co_drv.h"
#include "co_odaccess.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_emcy.h"
#include "ico_odaccess.h"
#include "ico_pdo.h"
#include "ico_nmt.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define CO_PDO_MAX_DATA_LEN CO_CAN_MAX_DATA_LEN
#define CO_COB_RTR_BIT      0x40000000UL

typedef enum {
    CO_PDO_STATE_READY,
    CO_PDO_STATE_TO_TRANSMIT,
    CO_PDO_STATE_DATA_RECEIVED
} CO_PDO_STATE_T;

/* local defined data types
---------------------------------------------------------------------------*/
typedef struct {
    UNSIGNED16      pdoNr;          /* pdo number */
    CO_PDO_STATE_T  state;          /* pdo state */
    COB_REFERENZ_T  cob;
    UNSIGNED32      cobId;
    UNSIGNED8       transType;
    UNSIGNED16      inhibit;
    UNSIGNED16      eventTime;
    UNSIGNED8       syncStartVal;
    CO_TIMER_T      pdoTimer;
    UNSIGNED8       actSyncCnt;
    CO_CONST PDO_TR_MAP_TABLE_T *mapTableConst;
    PDO_TR_MAP_TABLE_T  *mapTable;
    BOOL_T          dynMapping;     /* dynamic mapped PDO */
#ifdef CO_TR_PDO_DYN_MAP_ENTRIES
    UNSIGNED16      mapTableIdx;    /* index at mapping table */
#endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
} CO_TR_PDO_T;

typedef struct {
    UNSIGNED16      pdoNr;          /* pdo number */
    CO_PDO_STATE_T  state;          /* pdo state */
    COB_REFERENZ_T  cob;
    UNSIGNED32      cobId;
    UNSIGNED8       transType;
    UNSIGNED16      inhibit;
    UNSIGNED16      eventTime;
    CO_TIMER_T      pdoTimer;
    UNSIGNED8       shadowData[CO_PDO_MAX_DATA_LEN];
    CO_CONST PDO_REC_MAP_TABLE_T    *mapTableConst;
    PDO_REC_MAP_TABLE_T *mapTable;
    UNSIGNED8       mapLen;
    BOOL_T          dynMapping;     /* dynamic mapped PDO */
#ifdef CO_REC_PDO_DYN_MAP_ENTRIES
    UNSIGNED16      mapTableIdx;    /* index at mapping table */
#endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
} CO_REC_PDO_T;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_PDO_TRANSMIT_CNT
static CO_TR_PDO_T  pdoTransmit[CO_PDO_TRANSMIT_CNT];
static UNSIGNED16   pdoTrCnt = 0u;
#endif /* CO_PDO_TRANSMIT_CNT */

#ifdef CO_PDO_RECEIVE_CNT
static CO_REC_PDO_T pdoReceive[CO_PDO_RECEIVE_CNT];
static UNSIGNED16   pdoRecCnt = 0u;
# ifdef CO_EVENT_PDO
static CO_EVENT_PDO_T   pdoEventTable[CO_EVENT_PDO];
static UNSIGNED16       pdoEventTableCnt = 0u;
static CO_EVENT_PDO_T   pdoRecEventTable[CO_EVENT_PDO];
static UNSIGNED16       pdoRecEventTableCnt = 0u;
#  ifdef CO_SYNC_SUPPORTED
static CO_EVENT_PDO_T   pdoEventSyncTable[CO_EVENT_PDO];
static UNSIGNED16       pdoEventSyncTableCnt = 0u;
#  endif /* CO_SYNC_SUPPORTED */
# endif /*  CO_EVENT_PDO */
#endif /* CO_PDO_RECEIVE_CNT */

/* Mapping tables TPDO */
#ifdef CO_TR_PDO_DYN_MAP_ENTRIES
static PDO_TR_MAP_TABLE_T   mapTablesTrPDO[CO_TR_PDO_DYN_MAP_ENTRIES];
static UNSIGNED16           mapTableTrPdoCnt = 0u;
#endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
#ifdef CO_REC_PDO_DYN_MAP_ENTRIES
static PDO_REC_MAP_TABLE_T  mapTablesRecPDO[CO_REC_PDO_DYN_MAP_ENTRIES];
static UNSIGNED16           mapTableRecPdoCnt = 0u;
#endif /* CO_REC_PDO_DYN_MAP_ENTRIES */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_PDO_RECEIVE_CNT
static CO_INLINE UNSIGNED16 searchRecPdoIndex(UNSIGNED16    pdoNr);
static UNSIGNED8            checkRecPdoMappingTable(UNSIGNED16 pdoNr, UNSIGNED16 idx, UNSIGNED8 mapCnt);
static RET_T                pdoReceiveData(UNSIGNED16 pdoIdx, CO_CONST UNSIGNED8 *pData);
static void                 stopRecPdoTimer(const CO_REC_PDO_T *pPdo);
static void                 pdoReceiveEventTimer(void *ptr);
# ifdef CO_EVENT_PDO
static void                 pdoInd(UNSIGNED16 pdoNr, UNSIGNED8 transType);
static void                 pdoRecEventInd(UNSIGNED16 pdoNr);
# endif /*  CO_EVENT_PDO */
#endif /* CO_PDO_RECEIVE_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
static CO_INLINE UNSIGNED16 searchTrPdoIndex(UNSIGNED16 pdoNr);
static UNSIGNED8            checkTrPdoMappingTable(UNSIGNED16 pdoNr, UNSIGNED16 idx,UNSIGNED8 mapCnt);
static RET_T                setupTrPdoTimer(CO_TR_PDO_T *pPdo);
static void                 pdoEventTimer(void *ptr);
static RET_T                pdoTransmitData(UNSIGNED16 pdoIdx, UNSIGNED8 flags);
#endif /* CO_PDO_TRANSMIT_CNT */

#ifdef CO_TR_PDO_DYN_MAP_ENTRIES
static UNSIGNED16           createPdoTrMapTable(UNSIGNED16 index);
static RET_T                setupPdoTrMapTable(UNSIGNED16 mapTableIdx, UNSIGNED16 index);
#endif /* CO_TR_PDO_DYN_MAP_ENTRIES */

#ifdef CO_REC_PDO_DYN_MAP_ENTRIES
static UNSIGNED16           createPdoRecMapTable(UNSIGNED16 index);
static RET_T                setupPdoRecMapTable(UNSIGNED16 mapTableIdx, UNSIGNED16 index);
#endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
static RET_T                setPdoCob(COB_REFERENZ_T cobRef, UNSIGNED32 cobId, UNSIGNED8 len);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_PDO_TRANSMIT_CNT
/***************************************************************************/
/**
* \brief coPdoReqNr - request PDO transmission by PDO number
*
* This function requests the transmission of an PDO given by its number.<br>
* All mapped objects are automatically copied into the CAN message.
* If the inhibit time is not active,
* then the message is transmitted immediately.
*
* If the inhibit time is not ellapsed yet,
* the transmission depends on the parameter flags:
*
* MSG_OVERWRITE - if the last PDO is not transmitted yet,
*	overwrite the last data with the new data
* MSG_RET_INHIBIT - return the function with RET_INHIBIT_ACTIVE,
*	if the inhibit is not ellapsed yet
*
* with the same or 
* \return RET_T
* \retval RET_INVALID_NMT_STATE
*	invalid NMT state
* \retval RET_INVALID_PARAMETER
*	unknown PDO number
* \retval RET_COB_DISABLED
*	PDO is disabled
* \retval RET_INHIBIT_ACTIVE
*	inhibit time is not yet ellapsed
* \retval RET_OK
*	all function are ok, but have not to be transmitted yet
*
*/
RET_T coPdoReqNr(
		UNSIGNED16 pdoNr,		/**< PDO number */
		UNSIGNED8 flags			/**< transmit flags */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

	/* OPERATIONAL ? */
	if (coNmtGetState() != CO_NMT_STATE_OPERATIONAL)  {
		return(RET_INVALID_NMT_STATE);
	}

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_INVALID_PARAMETER);
	}

	/* check, if PDO is not disabled */
	if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) == CO_COB_INVALID)  {
		return(RET_COB_DISABLED);
	}

	/* synchronous PDO ? */
	if (pdoTransmit[idx].transType <= 240u)  {
		if (pdoTransmit[idx].transType == 0u)  {
			pdoTransmit[idx].state = CO_PDO_STATE_TO_TRANSMIT;
		}
		return(retVal);
	}

	/* only RTR ? */
	if (pdoTransmit[idx].transType == 253u)  {
		return(RET_PARAMETER_INCOMPATIBLE);
	}

	retVal = pdoTransmitData(idx, flags);
	return(retVal);
}


/***************************************************************************/
/**
* \brief coPdoReqObj - request PDO transmission by object
*
* This function requests the transmission of the PDO,
* which the given object is mapped into.<br>
* All mapped objects are automatically copied into the CAN message.
* If the inhibit time is not active,
* then the message is transmitted immediately.
*
* If the inhibit time is not ellapsed yet,
* the transmission depends on the parameter flags:
*
* MSG_OVERWRITE - if the last PDO is not transmitted yet,
*	overwrite the last data with the new data
* MSG_RET_INHIBIT - return the function with RET_INHIBIT_ACTIVE,
*	if the inhibit is not ellapsed yet
*
* \return RET_T
* \retval RET_INVALID_NMT_STATE
*	invalid NMT state
* \retval RET_INVALID_PARAMETER
*	unknown PDO number
* \retval RET_COB_DISABLED
*	PDO is disabled
* \retval RET_INHIBIT_ACTIVE
*	inhibit time is not yet ellapsed
* \retval RET_OK
*	all function are ok, but have not to be transmitted yet
*
*/
RET_T coPdoReqObj(
		UNSIGNED16 index,		/**< index of mapped object */
		UNSIGNED8 subIndex,		/**< subindex of mapped object */
		UNSIGNED8 flags			/**< transmit flags */
	)
{
    RET_T	retVal;
    UNSIGNED16	i, m;
    CO_CONST CO_OBJECT_DESC_T	*pDesc;
    UNSIGNED32		mapEntry;
    UNSIGNED32		size;

	/* get object description and size */
	retVal = coOdGetObjDescPtr(index, subIndex, &pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}
	size = coOdGetObjSize(pDesc);
	mapEntry = ((UNSIGNED32)index << 16) | ((UNSIGNED32)subIndex << 8) | (size << 3);

	/* for all PDOs */
	for (i = 0u; i < pdoTrCnt; i++)  {
		/* for all map entries */
		for (m = 0u; m < pdoTransmit[i].mapTableConst->mapCnt; m++)  {
			if (pdoTransmit[i].mapTableConst->mapEntry[m].val == mapEntry)  {
				retVal = coPdoReqNr(pdoTransmit[i].pdoNr, flags);
			}
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief pdoTransmitData - transmit data
*
* \return RET_T
*
*/
static RET_T pdoTransmitData(
		UNSIGNED16	pdoIdx,
		UNSIGNED8	flags
	)
{
    RET_T	retVal;
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	cnt;
    UNSIGNED8	offs;

	memset(&trData, 0, sizeof(trData));

	/* map pdo data */
	offs = 0u;
	for (cnt = 0u; cnt < pdoTransmit[pdoIdx].mapTableConst->mapCnt; cnt++)  {
		/* numeric value ? */
		coNumMemcpyUnpack(&trData.data[offs],
			pdoTransmit[pdoIdx].mapTableConst->mapEntry[cnt].pVar,
			(UNSIGNED32)pdoTransmit[pdoIdx].mapTableConst->mapEntry[cnt].len,
			(UNSIGNED16)pdoTransmit[pdoIdx].mapTableConst->mapEntry[cnt].numeric, 0u);
		offs += pdoTransmit[pdoIdx].mapTableConst->mapEntry[cnt].len;
	}

	/* transmit data */
	retVal = icoTransmitMessage(pdoTransmit[pdoIdx].cob, &trData, flags);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoPdoRtrHandler - PDO RTR receive handler
*
*
* \return none
*
*/
void icoPdoRtrHandler(
		CO_CONST CO_REC_DATA_T	*pRecData		/* pointer to received data */
	)
{
    CO_TR_PDO_T	*pPdo;

	/* OPERATIONAL ? */
	if (coNmtGetState() != CO_NMT_STATE_OPERATIONAL)  {
		return;
	}

	if (pRecData->spec > pdoTrCnt)  {
		return;
	}

	pPdo = &pdoTransmit[pRecData->spec];

	/* synchronous PDO ? */
	if (pPdo->transType <= 240u)  {
		if (pPdo->transType == 0u)  {
			pPdo->state = CO_PDO_STATE_TO_TRANSMIT;
		}
	} else {
		(void) pdoTransmitData(pRecData->spec, 0u);
	}

	return;
}
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
/***************************************************************************/
/**
* \brief coPdoObjIsMapped - check, if object mapped to given PDO
*
* This function checks the PDO,
* if the given object is actual mapped to this PDO.
* (Only valid for receive PDOs)
*
* \return BOOL_T
* \retval CO_TRUE
*	object is mapped
* \retval CO_FALSE
*	object is not mapped
*
*/
BOOL_T coPdoObjIsMapped(
		UNSIGNED16 pdoNr,		/**< PDO number */
		UNSIGNED16 index,		/**< index of mapped object */
		UNSIGNED8 subIndex		/**< subindex of mapped object */
	)
{
    RET_T	retVal;
    UNSIGNED16	m;
    CO_CONST CO_OBJECT_DESC_T	*pDesc;
    UNSIGNED32	mapEntry;
    UNSIGNED32	size;
    UNSIGNED16	idx;

	/* search PDO */
    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(CO_FALSE);
	}

	/* get object description and size */
	retVal = coOdGetObjDescPtr(index, subIndex, &pDesc);
	if (retVal != RET_OK)  {
		return(CO_FALSE);
	}
	size = coOdGetObjSize(pDesc);
	mapEntry = ((UNSIGNED32)index << 16) | ((UNSIGNED32)subIndex << 8) | (size << 3);

	/* for all map entries */
	for (m = 0u; m < pdoReceive[idx].mapTableConst->mapCnt; m++)  {
		if (pdoReceive[idx].mapTableConst->mapEntry[m].val == mapEntry)  {
			return(CO_TRUE);
		}
	}

	return(CO_FALSE);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoPdoReceiveHandler - PDO receive handler
*
*
* \return none
*
*/
void icoPdoReceiveHandler(
		CO_CONST CO_REC_DATA_T	*pRecData		/* pointer to received data */
	)
{
    CO_REC_PDO_T	*pPdo;

	/* OPERATIONAL ? */
	if (coNmtGetState() != CO_NMT_STATE_OPERATIONAL)  {
		return;
	}

	if (pRecData->spec > pdoRecCnt)  {
		return;
	}

	pPdo = &pdoReceive[pRecData->spec];

	/* check for correct received data length */
	if (pRecData->msg.len < pPdo->mapLen)  {
#ifdef CO_EMCY_PRODUCER
		UNSIGNED8 strg[5] = { 0u, 0u, 0u, 0u, 0u };
		strg[0] = (UNSIGNED8)(pPdo->pdoNr & 0xffu);
		strg[1] = (UNSIGNED8)(pPdo->pdoNr >> 8u);
/*
		strg[2] = pRecData->len;
		strg[3] = pdoReceive[idx].mapTable->mapCnt;
*/
		(void)icoEmcyWriteReq(CO_EMCY_ERRCODE_PDO_LEN, strg);
#endif /* CO_EMCY_PRODUCER */

		/* ignore message */
		return;
	}

	/* start event timer */
	if (pPdo->eventTime != 0u)  {
		(void) coTimerStart(&pPdo->pdoTimer,
				(UNSIGNED32)pPdo->eventTime * 1000u,
				pdoReceiveEventTimer, pPdo, CO_TIMER_ATTR_ROUNDDOWN);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
	}

	/* synchronous PDO ? */
	if (pPdo->transType <= 240u)  {
		/* save data at shadow buffer */
		memcpy(pPdo->shadowData, pRecData->msg.data, CO_PDO_MAX_DATA_LEN	);

		/* sign data as received for next sync */
		pPdo->state = CO_PDO_STATE_DATA_RECEIVED;
		return;
	}

	/* async data - unmap data */
	(void)pdoReceiveData(pRecData->spec, pRecData->msg.data);

	return;
}


/***************************************************************************/
/**
* \internal
*
* \brief pdoReceiveData - pdo data received 
*
* \return RET_T
*
*/
static RET_T pdoReceiveData(
		UNSIGNED16 pdoIdx,				/* PDO index */
		CO_CONST UNSIGNED8	pData[]		/* pointer to received data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED8	cnt;
    UNSIGNED8	offs;
    CO_REC_PDO_T	*pRecPdo;
    BOOL_T		changed;

	(void)(changed);

	pRecPdo = &pdoReceive[pdoIdx];

	/* map pdo data */
	offs = 0u;
	for (cnt = 0u; cnt < pRecPdo->mapTableConst->mapCnt; cnt++)  {
		/* not for dummy mapping */
		if (pRecPdo->mapTableConst->mapEntry[cnt].pVar != NULL)  {
			changed = coNumMemcpyPack(
				pRecPdo->mapTableConst->mapEntry[cnt].pVar,
				&pData[offs],
				(UNSIGNED32)pRecPdo->mapTableConst->mapEntry[cnt].len,
				(UNSIGNED16)pRecPdo->mapTableConst->mapEntry[cnt].numeric, 0u);

#ifdef CO_EVENT_OBJECT_CHANGED
			if (changed == CO_TRUE)  {
				UNSIGNED16	index = (UNSIGNED16)(pRecPdo->mapTableConst->mapEntry[cnt].val >> 16u);
				UNSIGNED8	subIndex = (UNSIGNED8)((pRecPdo->mapTableConst->mapEntry[cnt].val >> 8u) & 0xffu);
				CO_CONST CO_OBJECT_DESC_T *pDesc;

				(void)coOdGetObjDescPtr(index, subIndex, &pDesc);
				(void)icoEventObjectChanged(pDesc,
					(UNSIGNED16)(pRecPdo->mapTableConst->mapEntry[cnt].val >> 16u),
					changed);
			}
#else
#endif /* CO_EVENT_OBJECT_CHANGED */

		}
		offs += pRecPdo->mapTableConst->mapEntry[cnt].len;
	}

# ifdef CO_EVENT_PDO
	/* user indication */
	pdoInd(pRecPdo->pdoNr, pRecPdo->transType);
# endif /*  CO_EVENT_PDO */

	return(retVal);
}


/***************************************************************************/
/**
* \internal stopRecPdoTimer - stop receive timer
*
* \brief 
*
* \return none
*
*/
static void stopRecPdoTimer(
		const CO_REC_PDO_T *pPdo				/* pointer to pdo */
	)
{
	(void) coTimerStop(&pPdo->pdoTimer);
}


/***************************************************************************/
/**
* \internal pdoReceiceEventTimer - pdo receive timer ellapsed
* wird vom timer aufgerufen
*
* \brief 
*
* \return none
*
*/
static void pdoReceiveEventTimer(
		void *ptr
	)
{
# ifdef CO_EVENT_PDO
    CO_REC_PDO_T	*pPdo = ptr;

	pdoRecEventInd(pPdo->pdoNr);
# endif /*  CO_EVENT_PDO */
}
#endif /* CO_PDO_RECEIVE_CNT */


#ifdef CO_SYNC_SUPPORTED
/***************************************************************************/
/**
* \internal 
*
* \brief icoPdoSyncHandling - handle sync pdos
*
* \return none
*
*/
void icoPdoSyncHandling(
		UNSIGNED8	syncCounterVal			/* sync counter val */
	)
{
    UNSIGNED16	cnt;
# ifdef CO_PDO_TRANSMIT_CNT
    CO_TR_PDO_T	*pTrPdo;
# endif /* CO_PDO_TRANSMIT_CNT */
# ifdef CO_PDO_RECEIVE_CNT
    CO_REC_PDO_T	*pRecPdo;
# endif /* CO_PDO_RECEIVE_CNT */

	/* return if not OPER */
	if (coNmtGetState() != CO_NMT_STATE_OPERATIONAL)  {
		return;
	}

# ifdef CO_PDO_TRANSMIT_CNT
	/* handle TPDOs */
	for (cnt = 0u; cnt < pdoTrCnt; cnt++)  {
		pTrPdo = &pdoTransmit[cnt];
		/* check, if PDO is not disabled */
		if ((pTrPdo->cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
			/* sync pdo ? */
			if (pTrPdo->transType <= 240u)  {
				/* sync pdo found */
				/* acyclic PDO ? */
				if (pTrPdo->transType == 0u)  {
					/* to transmit ? */
					if (pTrPdo->state == CO_PDO_STATE_TO_TRANSMIT)  {
						/* transmit data */
						(void)pdoTransmitData(cnt, 0u);

						pTrPdo->state = CO_PDO_STATE_READY;
					}
				} else {
					/* cyclic PDO */
					if (pTrPdo->syncStartVal >= syncCounterVal)  {
						pTrPdo->actSyncCnt--;
						if ((syncCounterVal != 0u)
						 && (pTrPdo->syncStartVal == syncCounterVal))  {
							pTrPdo->actSyncCnt = 0u;
						}
						if (pTrPdo->actSyncCnt == 0u)  {
							pTrPdo->actSyncCnt = pTrPdo->transType;
							/* transmit */
							(void)pdoTransmitData(cnt, 0u);
						}
					}
				}
			}
		}
	}
# endif /* CO_PDO_TRANSMIT_CNT */

# ifdef CO_PDO_RECEIVE_CNT
	/* handle RPDOs */
	for (cnt = 0u; cnt < pdoRecCnt; cnt++)  {
		pRecPdo = &pdoReceive[cnt];
		if (pRecPdo->transType <= 240u)  {
			/* sync pdo found */
			if (pRecPdo->state == CO_PDO_STATE_DATA_RECEIVED)  {
				/* unmap data from shadow buffer */
				(void)pdoReceiveData(cnt, pRecPdo->shadowData);

				pRecPdo->state = CO_PDO_STATE_READY;
			}
		}
	}
# endif /* CO_PDO_RECEIVE_CNT */
}
#endif /* CO_SYNC_SUPPORTED */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


#ifdef CO_PDO_TRANSMIT_CNT
/***************************************************************************/
/**
* \internal
*
* \brief icoTrPdoGetObjectAddr - get TPDO object address
*
* \return none
*
*/
void *icoTrPdoGetObjectAddr(
		UNSIGNED16	pdoNr,			/* PDO number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    void	*pAddr = NULL;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 1:
			/* copy from cob handler */
			pAddr = (void *)&pdoTransmit[idx].cobId;
			break;
		case 2:
			pAddr = (void *)&pdoTransmit[idx].transType;
			break;
		case 3:
			pAddr = (void *)&pdoTransmit[idx].inhibit;
			break;
		case 5:
			pAddr = (void *)&pdoTransmit[idx].eventTime;
			break;
		case 6:
			pAddr = (void *)&pdoTransmit[idx].syncStartVal;
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
* \brief icoTrMapGetObjectAddr - get address of mapping data
*
* \return none
*
*/
void *icoTrMapGetObjectAddr(
		UNSIGNED16	pdoNr,			/* PDO number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    void *pAddr = NULL;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 0u:
			pAddr = &pdoTransmit[idx].mapTable->mapCnt;
			break;
		default:
			pAddr = &pdoTransmit[idx].mapTable->mapEntry[subIndex - 1u].val;
			break;
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrMapGetObjectAddr_R - get address of mapping data for reading
*
* \return none
*
*/
const void *icoTrMapGetObjectAddr_R(
		UNSIGNED16	pdoNr,			/* PDO number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    const void *pAddr = NULL;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 0u:
			pAddr = &pdoTransmit[idx].mapTableConst->mapCnt;
			break;
		default:
			pAddr = &pdoTransmit[idx].mapTableConst->mapEntry[subIndex - 1u].val;
			break;
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrPdoCheckObjLimit_u8 - check object limits u8 value
*
*
* \return RET_T
*
*/
RET_T icoTrPdoCheckObjLimit_u8(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	subIndex,	/* subindex */
		UNSIGNED8	u8			/* pointer to receive data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

#ifdef CIA301_V3
#else /* CIA301_V3 */
	/* only allowed if pdo is disabled */
	if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_SDO_INVALID_VALUE);
	}
#endif /* CIA301_V3 */

	/* transmission type */
	if (subIndex == 2u)  {
#ifdef CO_SYNC_SUPPORTED
		if ((u8 > 240u) && (u8 < 252u))  {
#else /* CO_SYNC_SUPPORTED */
		if (u8 < 253u)  {
#endif /* CO_SYNC_SUPPORTED */

			retVal = RET_SDO_INVALID_VALUE;

# ifdef CO_RTR_NOT_SUPPORTED
		} else {
			if ((u8 == 252u) || (u8 == 253u))  {
				retVal = RET_SDO_INVALID_VALUE;
			}
# endif /* CO_RTR_NOT_SUPPORTED */
		}
	} else

	/* sync start value */
	if (subIndex == 6u)  {
	} else
	{
		retVal = RET_SUBIDX_NOT_FOUND;
	}
	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoPdoCheckObjLimit_u16 - check object limits u16 value
*
*
* \return RET_T
*
*/
RET_T icoTrPdoCheckObjLimit_u16(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	subIndex	/* subindex */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if (subIndex == 3u)  {
		/* only allowed if pdo is disabled */
		if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
			return(RET_SDO_INVALID_VALUE);
		}
	} else
	if (subIndex == 5u)  {
	} else
	{
		retVal = RET_SUBIDX_NOT_FOUND;
	}
	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrPdoCheckObjLimitCobId - check cob-id
*
*
* \return RET_T
*
*/
RET_T icoTrPdoCheckObjLimitCobid(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED32	canId		/* pointer to receive data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if ((canId & CO_COB_VALID_MASK) == 0u)  {
		/* new cobid is valid, only allowed if cob was disabled before*/
		if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
			/* cct checks for the same cob-od - allow it */
			if (pdoTransmit[idx].cobId != canId) {
				return(RET_SDO_INVALID_VALUE);
			}
		}
#ifdef CO_RTR_NOT_SUPPORTED
		if ((canId & CO_COB_RTR_BIT) == 0u)  {
			return(RET_SDO_INVALID_VALUE);
		}
#endif

		/* check for reserved cobs */
		if (icoCheckRestrictedCobs(canId, 0x181u, 0x181u) == CO_TRUE)  {
			return(RET_SDO_INVALID_VALUE);
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrMapCheckObjLimit_u8 - check object limits mapping value
*
*
* \return RET_T
*
*/
RET_T icoTrMapCheckObjLimitMapTable(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	subIndex,
		UNSIGNED8	value
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;
    UNSIGNED8	mapLen;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

#ifdef CIA301_V3
#else /* CIA301_V3 */
	/* change mapping is only allowed if PDO is disabled */
	if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_MAP_ERROR);
	}
#endif /* CIA301_V3 */

	/* changing sub > 0 is only allowed if subindex 0 = 0 */
	if (subIndex == 0u)  {
		if (value != 0u)  {
			/* check mapping table */
			mapLen = checkTrPdoMappingTable(pdoNr, idx, value);
			if (mapLen > CO_PDO_MAX_DATA_LEN)  {
				return(RET_MAP_LEN_ERROR);
			}
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrMapCheckObjLimitMapEntry - check object limits mapping entry
*
*
* \return RET_T
*
*/
RET_T icoTrMapCheckObjLimitMapEntry(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED32	value
	)
{
    RET_T	retVal;
    UNSIGNED16	idx;
    CO_CONST void		*pVar;
    UNSIGNED8	len;
    BOOL_T		numeric;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

#ifdef CIA301_V3
#else /* CIA301_V3 */
	/* change mapping is only allowed if PDO is disabled */
	if ((pdoTransmit[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_MAP_ERROR);
	}
#endif /* CIA301_V3 */

	/* changing sub > 0 is only allowed if subindex 0 = 0 */
	if (pdoTransmit[idx].mapTableConst->mapCnt != 0u)  {
		return(RET_MAP_ERROR);
	}

	/* check mapping entry */
	retVal = icoOdGetObjTrMapData(
			(UNSIGNED16)(value >> 16u),
			(UNSIGNED8)((value >> 8u) & 0xffu),
			&pVar,
			&len,
			&numeric);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrPdoObjChanged - TPdo object was changed
*
*
* \return RET_T
*
*/
RET_T icoTrPdoObjChanged(
		UNSIGNED16	pdoNr,			/* PDO number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    UNSIGNED8	mapLen;
    RET_T		retVal = RET_OK;

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	switch (subIndex)  {
		case 1u:
			/* get number of mapped bytes */
			mapLen = checkTrPdoMappingTable(pdoNr, idx, 0u);
			retVal = setPdoCob(pdoTransmit[idx].cob, pdoTransmit[idx].cobId, mapLen);
			if (retVal == RET_OK)  {
				/* setup timer */
				retVal = setupTrPdoTimer(&pdoTransmit[idx]);
			}
			if (retVal == RET_OK)  {
				/* set inhibit - only for transtype fe and ff (otherwise 0) */
				if (pdoTransmit[idx].transType >= 254u)  {
					retVal = icoCobSetInhibit(pdoTransmit[idx].cob,
								pdoTransmit[idx].inhibit);
				} else {
					retVal = icoCobSetInhibit(pdoTransmit[idx].cob, 0u);
				}
			}
			break;
		case 2u:
			pdoTransmit[idx].actSyncCnt = pdoTransmit[idx].transType;
			retVal = setupTrPdoTimer(&pdoTransmit[idx]);
			break;
		case 3u:
			break;
		case 5u:
			retVal = setupTrPdoTimer(&pdoTransmit[idx]);
			break;
		default:
			break;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoTrPdoMapChanged - TPdo mapping was changed
*
*
* \return RET_T
*
*/
RET_T icoTrPdoMapChanged(
		UNSIGNED16	pdoNr			/* PDO number */
	)
{
    UNSIGNED16	idx;
    RET_T		retVal = RET_OK;
# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
    PDO_TR_MAP_TABLE_T	*pMap;
    UNSIGNED8	cnt;
    UNSIGNED32	entry;
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */

    idx = searchTrPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
	if (pdoTransmit[idx].dynMapping == CO_TRUE)  {
		pMap = &mapTablesTrPDO[pdoTransmit[idx].mapTableIdx];

		/* for all mapping entries */
		for (cnt = 0u; cnt < pMap->mapCnt; cnt++)  {
			entry = pMap->mapEntry[cnt].val;

			retVal = icoOdGetObjTrMapData(
				(UNSIGNED16)(entry >> 16u),
				(UNSIGNED8)((entry >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
			if (retVal != RET_OK)  {
				return(retVal);
			}
		}
	}
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */

	return(retVal);
}
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoGetObjectAddr - get RPDO object address
*
* \return RET_T
*
*/
void *icoRecPdoGetObjectAddr(
		UNSIGNED16	pdoNr,			/* pdo number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    void	*pAddr = NULL;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 1u:
			/* copy from cob handler */
			pAddr = (void *)&pdoReceive[idx].cobId;
			break;
		case 2u:
			pAddr = (void *)&pdoReceive[idx].transType;
			break;
		case 3u:
			pAddr = (void *)&pdoReceive[idx].inhibit;
			break;
		case 5u:
			pAddr = (void *)&pdoReceive[idx].eventTime;
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
* \brief icoRecMapGetObjectAddr - get address of mapping data
*
* \return none
*
*/
void *icoRecMapGetObjectAddr(
		UNSIGNED16	pdoNr,			/* pdo number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    void *pAddr = NULL;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 0u:
			pAddr = &pdoReceive[idx].mapTable->mapCnt;
			break;
		default:
			pAddr = &pdoReceive[idx].mapTable->mapEntry[subIndex - 1u].val;
			break;
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecMapGetObjectAddr_R - get address of mapping data for reading
*
* \return none
*
*/
const void *icoRecMapGetObjectAddr_R(
		UNSIGNED16	pdoNr,			/* pdo number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    const void *pAddr = NULL;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 0u:
			pAddr = &pdoReceive[idx].mapTableConst->mapCnt;
			break;
		default:
			pAddr = &pdoReceive[idx].mapTableConst->mapEntry[subIndex - 1u].val;
			break;
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoCheckObjLimit_u8 - check object limits u8 value for RPDO
*
*
* \return RET_T
*
*/
RET_T icoRecPdoCheckObjLimit_u8(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	subIndex,	/* subindex */
		UNSIGNED8	u8			/* pointer to receive data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	/* only allowed if pdo is disabled */
	if ((pdoReceive[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_SDO_INVALID_VALUE);
	}

	/* transmission type */
	if (subIndex == 2u)  {
#ifdef CO_SYNC_SUPPORTED
		if ((u8 > 240u) && (u8 < 254u)) {
#else /* CO_SYNC_SUPPORTED */
		if (u8 < 254u) {
#endif /* CO_SYNC_SUPPORTED */
			return(RET_SDO_INVALID_VALUE);
		}
	} else

	/* sync start value */
	if (subIndex == 6u)  {
	} else
	{
		retVal = RET_SUBIDX_NOT_FOUND;
	}
	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoCheckObjLimit_u16 - check object limits u16 value
*
*
* \return RET_T
*
*/
RET_T icoRecPdoCheckObjLimit_u16(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	subIndex	/* subindex */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if (subIndex == 3u)  {
		/* only allowed if pdo is disabled */
		if ((pdoReceive[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
			return(RET_SDO_INVALID_VALUE);
		}
	} else
	if (subIndex == 5u)  {
	} else
	{
		retVal = RET_SUBIDX_NOT_FOUND;
	}
	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoCheckObjLimitCobId - check cob-id
*
*
* \return RET_T
*
*/
RET_T icoRecPdoCheckObjLimitCobid(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED32	canId		/* pointer to receive data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

	idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if ((canId & CO_COB_VALID_MASK) == 0u)  {
		/* new cobid is valid, only allowed if cob was disabled before*/
		if ((pdoReceive[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
			/* cct checks for the same cob-od - allow it */
			if (pdoReceive[idx].cobId != canId) {
				return(RET_SDO_INVALID_VALUE);
			}
		}
		/* check for reserved cobs */
		if (icoCheckRestrictedCobs(canId, 0x181u, 0x181u) == CO_TRUE)  {
			return(RET_SDO_INVALID_VALUE);
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecMapCheckObjLimitMapTable - check object limits mapping table
*
*
* \return RET_T
*
*/
RET_T icoRecMapCheckObjLimitMapTable(
		UNSIGNED16	pdoNr,			/* pdo number */
		UNSIGNED8	subIndex,
		UNSIGNED8	value
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;
    UNSIGNED8	mapLen;
    PDO_REC_MAP_ENTRY_T	CO_CONST *pEntry;
    void		*pVar;
    UNSIGNED8	len;
    BOOL_T		numeric;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	/* change mapping is only allowed if PDO is disabled */
	if ((pdoReceive[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_MAP_ERROR);
	}

	/* changing sub > 0 is only allowed if subindex 0 = 0 */
	if (subIndex != 0u)  {
		if (pdoReceive[idx].mapTableConst->mapCnt != 0u)  {
			return(RET_MAP_ERROR);
		}
		/* check mapping entry */
		pEntry = &pdoReceive[idx].mapTableConst->mapEntry[subIndex - 1u];
		retVal = icoOdGetObjRecMapData(
				(UNSIGNED16)(pEntry->val >> 16u),
				(UNSIGNED8)((pEntry->val >> 8u) & 0xffu),
				&pVar,
				&len,
				&numeric);
	} else {
		if (value != 0u)  {
			/* check mapping table */
			mapLen = checkRecPdoMappingTable(pdoNr, idx, value);
			if (mapLen > CO_PDO_MAX_DATA_LEN)  {
				return(RET_MAP_LEN_ERROR);
			}
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecMapCheckObjLimitMapEntry - check object limits mapping value
*
*
* \return RET_T
*
*/
RET_T icoRecMapCheckObjLimitMapEntry(
		UNSIGNED16	pdoNr,			/* pdo number */
		UNSIGNED32	mapEntry		/* mapping entry */
	)
{
    RET_T	retVal;
    UNSIGNED16	idx;
    UNSIGNED16	mapIdx;
    UNSIGNED8	mapSubIdx;
    void 		*pVar;
    UNSIGNED8	len;
    BOOL_T		numeric;

	/* search PDO data */
    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	/* change mapping is only allowed if PDO is disabled */
	if ((pdoReceive[idx].cobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
		return(RET_MAP_ERROR);
	}

	/* changing sub > 0 is only allowed if subindex 0 = 0 */
	if (pdoReceive[idx].mapTableConst->mapCnt != 0u)  {
		return(RET_MAP_ERROR);
	}

	mapIdx = (UNSIGNED16)(mapEntry >> 16);
	mapSubIdx = (UNSIGNED8)(mapEntry >> 8);

	/* check mapping entry */
	retVal = icoOdGetObjRecMapData(mapIdx, mapSubIdx, &pVar, &len, &numeric);
	if (retVal != RET_OK)  {
		return(RET_MAP_ERROR);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoObjChanged - RecPdo object was changed
*
*
* \return RET_T
*
*/
RET_T icoRecPdoObjChanged(
		UNSIGNED16	pdoNr,		/*  pdo number */
		UNSIGNED8	subIndex	/* subindex */
	)
{
    UNSIGNED16	idx;
    UNSIGNED8	mapLen;
    RET_T		retVal = RET_OK;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	switch (subIndex)  {
		case 1u:
			/* get number of mapped bytes */
			mapLen = checkRecPdoMappingTable(pdoNr, idx, 0u);
			retVal = setPdoCob(pdoReceive[idx].cob, pdoReceive[idx].cobId, mapLen);
			stopRecPdoTimer(&pdoReceive[idx]);
			if (retVal == RET_OK)  {
				pdoReceive[idx].mapLen = mapLen;
			}
			break;
		case 5u:
			stopRecPdoTimer(&pdoReceive[idx]);
			break;
		default:
			break;
	}

	return(retVal);
}


# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
/***************************************************************************/
/**
* \internal
*
* \brief icoRecPdoMapChanged - RPdo mapping was changed
*
*
* \return RET_T
*
*/
RET_T icoRecPdoMapChanged(
		UNSIGNED16	pdoNr			/* PDO number */
	)
{
    UNSIGNED16	idx;
    RET_T		retVal = RET_OK;
    PDO_REC_MAP_TABLE_T	*pMap;
    UNSIGNED8	cnt;
    UNSIGNED32	entry;

    idx = searchRecPdoIndex(pdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if (pdoReceive[idx].dynMapping == CO_TRUE)  {
		pMap = &mapTablesRecPDO[pdoReceive[idx].mapTableIdx];

		/* for all mapping entries */
		for (cnt = 0u; cnt < pMap->mapCnt; cnt++)  {
			entry = pMap->mapEntry[cnt].val;

			retVal = icoOdGetObjRecMapData(
				(UNSIGNED16)(entry >> 16u),
				(UNSIGNED8)((entry >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
			if (retVal != RET_OK)  {
				return(retVal);
			}
		}
	}

	return(retVal);
}
# endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
#endif /* CO_PDO_RECEIVE_CNT */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


#ifdef CO_PDO_RECEIVE_CNT
# ifdef CO_EVENT_PDO
/***************************************************************************/
/**
* \brief coEventRegister_PDO - register asynchronous PDO event
*
* Register an indication function for asynchrounous PDOs.
*
* After a PDO has been received, the data are stored in the object dictionary,
* and then the given indication function is called.
* This function is only valid for asynchronous PDOs.
*
* \return RET_T
*
*/

RET_T coEventRegister_PDO(
		CO_EVENT_PDO_T pFunction	/**< pointer to function */
    )
{
	if (pdoEventTableCnt >= CO_EVENT_PDO) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	pdoEventTable[pdoEventTableCnt] = pFunction;
	pdoEventTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief coEventRegister_PDO_REC_EVENT - register receive PDO event
*
* Register an indication function for receive PDO event.
*
* For monitoring of receive PDOs the event timer can be used.
* If the event timer value is unequal 0 then after the reception of a PDO
* the monitoring is started automatically.
* if no further PDO in the given time was received,
* the indication function given to this function is called.
*
* \return RET_T
*
*/

RET_T coEventRegister_PDO_REC_EVENT(
		CO_EVENT_PDO_T pFunction	/**< pointer to function */
    )
{
	if (pdoRecEventTableCnt >= CO_EVENT_PDO) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	pdoRecEventTable[pdoRecEventTableCnt] = pFunction;
	pdoRecEventTableCnt++;

	return(RET_OK);
}


#  ifdef CO_SYNC_SUPPORTED
/***************************************************************************/
/**
* \brief coEventRegister_PDO_SYNC - register PDO SYNC event
*
* Register an indication function for received synchronous PDOs.
*
* After the SYNC was received,
* the received data are stored in the object dictionary,
* and then this given indication function is called.
*
* \return RET_T
*
*/

RET_T coEventRegister_PDO_SYNC(
		CO_EVENT_PDO_T pFunction	/**< pointer to function */
    )
{
	if (pdoEventSyncTableCnt >= CO_EVENT_PDO) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	pdoEventSyncTable[pdoEventSyncTableCnt] = pFunction;
	pdoEventSyncTableCnt++;

	return(RET_OK);
}
#  endif /* CO_SYNC_SUPPORTED */


/***************************************************************************/
/**
* \internal
*
* \brief pdoInd - check user indication
*
* \return RET_T
*
*/
static void pdoInd(
		UNSIGNED16	pdoNr,		/* pdo number */
		UNSIGNED8	transType	/* transmission type */
	)
{
    UNSIGNED16	cnt;

	if (transType > 240u)  {
		/* asynchronous */
		/* call indication to execute */
		cnt = pdoEventTableCnt;
		while (cnt > 0u)  {
			cnt--;
			/* call user indication */
			pdoEventTable[cnt](pdoNr);
		}
#  ifdef CO_SYNC_SUPPORTED
	} else {
		/* synchronous */
		/* call indication to execute */
		cnt = pdoEventSyncTableCnt;
		while (cnt > 0u)  {
			cnt--;
			/* call user indication */
			pdoEventSyncTable[cnt](pdoNr);
		}
#  endif /* CO_SYNC_SUPPORTED */
	}

	return;
}


/***************************************************************************/
/**
* \internal
*
* \brief pdoRecEventInd - receive event occured
*
* This function is called, if receive event timer was timed out
*
* \return RET_T
*
*/
static void pdoRecEventInd(
		UNSIGNED16	pdoNr		/* pdo number */
	)
{
    UNSIGNED16	cnt;

	/* call indication to execute */
	cnt = pdoRecEventTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		pdoRecEventTable[cnt](pdoNr);
	}

	return;
}
# endif /*  CO_EVENT_PDO */
#endif /* CO_PDO_RECEIVE_CNT */


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/


#ifdef CO_PDO_TRANSMIT_CNT
/***************************************************************************/
/**
* \internal
*
* \brief setupTrPdoTimer - setup transmit PDO timer
*
* \return RET_T
*
*/
static RET_T setupTrPdoTimer(
		CO_TR_PDO_T *pPdo			/* pointer to Transmit PDO */
	)
{
    RET_T	retVal = RET_OK;
    CO_NMT_STATE_T nmtState;

	nmtState = coNmtGetState();

    /* start event timer */
    /* start sync start value */

	if (((pPdo->cobId & CO_COB_VALID_MASK) == CO_COB_INVALID) 
	  || (pPdo->eventTime == 0u)
	  || (pPdo->transType < 254u)
	  || (nmtState != CO_NMT_STATE_OPERATIONAL))  {
		(void)coTimerStop(&pPdo->pdoTimer);
	} else {
		if (pPdo->eventTime != 0u)  {
			retVal = coTimerStart(&pPdo->pdoTimer,
				(UNSIGNED32)pPdo->eventTime * 1000u,
				pdoEventTimer, pPdo, CO_TIMER_ATTR_ROUNDDOWN_CYCLIC);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
		}
	}

	return(retVal);
}

/***************************************************************************/
/**
* \internal
*
* \brief pdoEventTimer
*
* \return none
*
*/
static void pdoEventTimer(void *ptr)
{
    CO_TR_PDO_T	*pPdo = ptr;

	(void)coPdoReqNr(pPdo->pdoNr, 0u);
}


/***************************************************************************/
/**
* \internal
*
* \brief checkTrPdoMappingTable - check PDO transmit mapping table
*
* if mapCnt == 0, use value from pdoTransmit[idx].mapTable->mapCnt
*
* \return mapcount
*/
static UNSIGNED8 checkTrPdoMappingTable(
		UNSIGNED16	pdoNr,				/* PDO nr */
		UNSIGNED16	idx,				/* index at transmit data table */
		UNSIGNED8	mapCnt
	)
{
    UNSIGNED8	offs;
    UNSIGNED8	cnt;
    PDO_TR_MAP_ENTRY_T	CO_CONST *pEntry;
    RET_T		retVal;
    PDO_TR_MAP_TABLE_T *pMap;
    PDO_TR_MAP_TABLE_T map;

	if (mapCnt == 0u) {
		mapCnt = pdoTransmit[idx].mapTableConst->mapCnt;
	}
	/* mapCnt > max sub index */
	if (mapCnt > icoOdGetNumberOfSubs(0x1A00u + (pdoNr - 1u)))  {
		return(255u);
	}

	offs = 0u;
	for (cnt = 0u; cnt < mapCnt; cnt++)  {
		/* check mapentry */
		pEntry = &pdoTransmit[idx].mapTableConst->mapEntry[cnt];
# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
		if (pdoTransmit[idx].mapTableIdx == 0xffffu)  {
			pMap = &map;
		} else {
			pMap = &mapTablesTrPDO[pdoTransmit[idx].mapTableIdx];
		}
# else /* CO_TR_PDO_DYN_MAP_ENTRIES */
		pMap = &map;
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
		retVal = icoOdGetObjTrMapData(
				(UNSIGNED16)(pEntry->val >> 16u),
				(UNSIGNED8)((pEntry->val >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
		if (retVal != RET_OK)  {
			return(255u);
		}

		offs += pdoTransmit[idx].mapTableConst->mapEntry[cnt].len;
	}

	return(offs);
}


/***************************************************************************/
/**
* \internal
*
* \brief searchTrPdoIndex - search for pdo transmit index
*
*
* \return index
*
*/
static CO_INLINE UNSIGNED16 searchTrPdoIndex(
		UNSIGNED16	pdoNr			/* pdo number */
	)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < CO_PDO_TRANSMIT_CNT; cnt++)  {
		if (pdoNr == pdoTransmit[cnt].pdoNr)  {
			return(cnt);
		}
	}

	return(0xffffu);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoPdoOperational - new OPERATIONAL state
*
* transmit all async pdos and start event timers
*
* \return none
*
*/
void icoPdoOperational(
		void
	)
{
    UNSIGNED16	cnt;
    CO_TR_PDO_T	*pPdo;

	for (cnt = 0u; cnt < pdoTrCnt; cnt++)  {
		pPdo = &pdoTransmit[cnt];

		/*  if async or sync non-cyclic PDO ? */
		(void)coPdoReqNr(pPdo->pdoNr, 0u);
	
		(void)setupTrPdoTimer(pPdo);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief icoPdoPreOperational - new PRE_OPERATIONAL state
*
* stop event timers
*
* \return none
*
*/
void icoPdoPreOperational(
		void
	)
{
    UNSIGNED16	cnt;
    CO_TR_PDO_T	*pPdo;

	for (cnt = 0u; cnt < pdoTrCnt; cnt++)  {
		pPdo = &pdoTransmit[cnt];

		(void)setupTrPdoTimer(pPdo);
	}
}


# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
/***************************************************************************/
/**
* \internal
*
* \brief createPdoMapTable - create new dynamic mapping table
*
* \return none
*
*/
static UNSIGNED16 createPdoTrMapTable(
		UNSIGNED16	index
	)
{
    RET_T		retVal;
    UNSIGNED16	mapTableIdx;

	/* mapping table available ? */
	if (mapTableTrPdoCnt == CO_TR_PDO_DYN_MAP_ENTRIES)  {
		/* no, return */
		return(0xffffu);
	}
	mapTableIdx = mapTableTrPdoCnt;

	retVal = setupPdoTrMapTable(mapTableIdx, index);
	if (retVal != RET_OK)  {
		return(0xffffu);
	}

	mapTableTrPdoCnt++;

	return(mapTableIdx);
}


/***************************************************************************/
/**
* \internal
*
* \brief setupPdoMapTable - create new dynamic mapping table
*
* \return none
*
*/
static RET_T setupPdoTrMapTable(
		UNSIGNED16	mapTableIdx,
		UNSIGNED16	index
	)
{
    UNSIGNED8	cnt;
    RET_T		retVal;
    UNSIGNED32	mapEntry;
    UNSIGNED8	numberOfSubs;
    PDO_TR_MAP_TABLE_T *pMap;

	pMap = &mapTablesTrPDO[mapTableIdx];

	/* get number of mappings */
	retVal = coOdGetDefaultVal_u8(index, 0u, &pMap->mapCnt);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* get number of subindex for this mapping table */
	numberOfSubs = icoOdGetNumberOfSubs(index) - 1u;

	/* for all mapping entries */
	for (cnt = 0u; cnt < numberOfSubs; cnt++)  {
		retVal = coOdGetDefaultVal_u32(index, cnt + 1u, &mapEntry);
		if (retVal != RET_OK)  {
			return(retVal);
		}
		pMap->mapEntry[cnt].val = mapEntry;

		/* check mapping entry only for valid mappings */
		if (cnt < pMap->mapCnt)  {
			retVal = icoOdGetObjTrMapData(
				(UNSIGNED16)(mapEntry >> 16u),
				(UNSIGNED8)((mapEntry >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
			if (retVal != RET_OK)  {
				return(retVal);
			}
		}
	}

	return(retVal);
}
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
/***************************************************************************/
/**
* \internal
*
* \brief checkRecPdoMappingTable - check PDO receive mapping table
*
* if mapCnt == 0, use value from pdoTransmit[idx].mapTable->mapCnt
*
* \return mapping count
*
*/
static UNSIGNED8 checkRecPdoMappingTable(
		UNSIGNED16	pdoNr,
		UNSIGNED16	idx,
		UNSIGNED8	mapCnt
	)
{
    UNSIGNED8	offs;
    UNSIGNED8	cnt;
    PDO_REC_MAP_ENTRY_T	CO_CONST *pEntry;
    RET_T		retVal;
    PDO_REC_MAP_TABLE_T *pMap;
    PDO_REC_MAP_TABLE_T map;

	if (mapCnt == 0u) {
		mapCnt = pdoReceive[idx].mapTableConst->mapCnt;
	}
	/* mapCnt > max sub index */
	if (mapCnt > icoOdGetNumberOfSubs(0x1600u + (pdoNr - 1u)))  {
		return(255u);
	}
	offs = 0u;
	for (cnt = 0u; cnt < mapCnt; cnt++)  {
		/* check mapentry */
		pEntry = &pdoReceive[idx].mapTableConst->mapEntry[cnt];
# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
		if (pdoReceive[idx].mapTableIdx == 0xffffu)  {
			pMap = &map;
		} else {
			pMap = &mapTablesRecPDO[pdoReceive[idx].mapTableIdx];
		}
# else /* CO_REC_PDO_DYN_MAP_ENTRIES */
		pMap = &map;
# endif /* CO_REC_PDO_DYN_MAP_ENTRIES */

		retVal = icoOdGetObjRecMapData(
				(UNSIGNED16)(pEntry->val >> 16u),
				(UNSIGNED8)((pEntry->val >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
		if (retVal != RET_OK)  {
			return(255u);
		}

		offs += pdoReceive[idx].mapTableConst->mapEntry[cnt].len;
	}

	return(offs);
}


/***************************************************************************/
/**
* \internal
*
* \brief searchRecPdoIndex - search for pdo receive index
*
*
* \return PDO index
*
*/
static CO_INLINE UNSIGNED16 searchRecPdoIndex(UNSIGNED16	pdoNr)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < CO_PDO_RECEIVE_CNT; cnt++)  {
		if (pdoNr == pdoReceive[cnt].pdoNr)  {
			return(cnt);
		}
	}

	return(0xffffu);
}

# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
/***************************************************************************/
/**
* \internal
*
* \brief createPdoMapTable - create new dynamic mapping table
*
* \return none
*
*/
static UNSIGNED16 createPdoRecMapTable(
		UNSIGNED16	index
	)
{
    RET_T		retVal;
    UNSIGNED16	mapTableIdx;

	/* mapping table available ? */
	if (mapTableRecPdoCnt == CO_REC_PDO_DYN_MAP_ENTRIES)  {
		/* no, return */
		return(0xffffu);
	}
	mapTableIdx = mapTableRecPdoCnt;

	retVal = setupPdoRecMapTable(mapTableIdx, index);
	if (retVal != RET_OK)  {
		return(0xffffu);
	}

	mapTableRecPdoCnt++;

	return(mapTableIdx);
}


/***************************************************************************/
/**
* \internal
*
* \brief setupPdoMapTable - create new dynamic mapping table
*
* \return none
*
*/
static RET_T setupPdoRecMapTable(
		UNSIGNED16	mapTableIdx,
		UNSIGNED16	index
	)
{
    UNSIGNED8	cnt;
    RET_T		retVal;
    UNSIGNED32	mapEntry;
    UNSIGNED8	numberOfSubs;
    PDO_REC_MAP_TABLE_T *pMap;

	pMap = &mapTablesRecPDO[mapTableIdx];

	/* get number of mappings */
	retVal = coOdGetDefaultVal_u8(index, 0u, &pMap->mapCnt);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* get number of subindex for this mapping table */
	numberOfSubs = icoOdGetNumberOfSubs(index) - 1u;

	/* for all mapping entries */
	for (cnt = 0u; cnt < numberOfSubs; cnt++)  {
		retVal = coOdGetDefaultVal_u32(index, cnt + 1u, &mapEntry);
		if (retVal != RET_OK)  {
			return(retVal);
		}
		pMap->mapEntry[cnt].val = mapEntry;

		/* check mapping entry only for valid mappings */
		if (cnt < pMap->mapCnt)  {
			retVal = icoOdGetObjRecMapData(
				(UNSIGNED16)(mapEntry >> 16u),
				(UNSIGNED8)((mapEntry >> 8u) & 0xffu),
				&pMap->mapEntry[cnt].pVar,
				&pMap->mapEntry[cnt].len,
				&pMap->mapEntry[cnt].numeric);
			if (retVal != RET_OK)  {
				return(retVal);
			}
		}
	}

	return(retVal);
}
# endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
#endif /* CO_PDO_RECEIVE_CNT */

/***************************************************************************/
/**
* \internal
*
* \brief setPdoCob - set PDO cob id
*
* \return none
*	RET_T
*/
static RET_T setPdoCob(
		COB_REFERENZ_T	cobRef,			/* cob reference */
		UNSIGNED32		cobId,			/* cob-id */
		UNSIGNED8		len				/* data len */
	)
{
    RET_T	retVal;

	if ((cobId & CO_COB_RTR_BIT) != 0ul)  {
		retVal = icoCobSet(cobRef, cobId, CO_COB_RTR_NONE, len);
	} else {
		retVal = icoCobSet(cobRef, cobId, CO_COB_RTR, len);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief coPdoReset - reset comm for Pdo
*
* \return none
*	RET_T
*/
void icoPdoReset(
		void
	)
{
    UNSIGNED16	    cnt;
#ifdef CO_PDO_RECEIVE_CNT
    UNSIGNED8	    mapLen;
    CO_REC_PDO_T	*pRecPdo;
#endif /* CO_PDO_RECEIVE_CNT */
#ifdef CO_PDO_TRANSMIT_CNT
    CO_TR_PDO_T	    *pTrPdo;
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_TRANSMIT_CNT
	for (cnt = 0u; cnt < pdoTrCnt; cnt++)  {
		pTrPdo = &pdoTransmit[cnt];

		/* reset mapping */
		if ((pTrPdo->cobId & CO_COB_INVALID) != 0u)  {
			(void)icoCobDisable(pTrPdo->cob);
		} else {
			/* get number of mapped bytes */
			(void)icoTrPdoObjChanged(pTrPdo->pdoNr, 1u);
		}

		pTrPdo->actSyncCnt = pTrPdo->transType;
		pTrPdo->state = CO_PDO_STATE_READY;

		(void) setupTrPdoTimer(pTrPdo);
	}
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
	/* receive PDO */
	for (cnt = 0u; cnt < pdoRecCnt; cnt++)  {
		pRecPdo = &pdoReceive[cnt];

		/* reset mapping */
		if ((pRecPdo->cobId & CO_COB_INVALID) != 0u)  {
			/* disable sdo */
			(void) icoCobDisable(pRecPdo->cob);
		} else {
			/* get number of mapped bytes */
			mapLen = checkRecPdoMappingTable(pRecPdo->pdoNr, cnt, 0u);
			pRecPdo->mapLen = mapLen;
			(void) setPdoCob(pRecPdo->cob, pRecPdo->cobId, mapLen);
		}

		pRecPdo->state = CO_PDO_STATE_READY;

		stopRecPdoTimer(pRecPdo);
	}
#endif /* CO_PDO_RECEIVE_CNT */

	return;
}


/***************************************************************************/
/**
* \internal
*
* \brief coPdoReset - reset comm for Pdo
*
* \return none
*	RET_T
*/
void icoPdoSetDefaultValue(
		void
	)
{
    UNSIGNED16	cnt;
    RET_T		retVal;
    UNSIGNED32	u32;
    UNSIGNED16	u16;
    UNSIGNED8	u8;
#ifdef CO_PDO_RECEIVE_CNT
    CO_REC_PDO_T	*pRecPdo;
#endif /* CO_PDO_RECEIVE_CNT */
#ifdef CO_PDO_TRANSMIT_CNT
    CO_TR_PDO_T	*pTrPdo;
#endif /* CO_PDO_TRANSMIT_CNT */
    UNSIGNED16	pdoIdx;


#ifdef CO_PDO_TRANSMIT_CNT
	for (cnt = 0u; cnt < pdoTrCnt; cnt++)  {
		pTrPdo = &pdoTransmit[cnt];

		/* get disable and RTR bit from OD */
		retVal = coOdGetDefaultVal_u32((0x1800u + pTrPdo->pdoNr) - 1u, 1u, &u32);
		if (retVal == RET_OK)  {
			u32 &= (CO_COB_INVALID | CO_COB_RTR_BIT);
		} else {
			u32 = 0u;
		}

		if ((pTrPdo->pdoNr >= 1u) && (pTrPdo->pdoNr <= 4u))  {
			/* calculate cobs for predefined connection set */
			pdoIdx = pTrPdo->pdoNr - 1u;
			pTrPdo->cobId = u32
				| ((0x180ul + (0x100ul * pdoIdx)) + coNmtGetNodeId());
		} else {
			/* disable sdo */
			pTrPdo->cobId = u32 | CO_COB_INVALID;
		}

		/* transmission type */
		retVal = coOdGetDefaultVal_u8((0x1800u + pTrPdo->pdoNr) - 1u, 2u, &u8);
		if (retVal == RET_OK)  {
			pTrPdo->transType = u8;
		}
		retVal = coOdGetDefaultVal_u16((0x1800u + pTrPdo->pdoNr) - 1u, 3u, &u16);
		if (retVal == RET_OK)  {
			pTrPdo->inhibit = u16;
		}
		retVal = coOdGetDefaultVal_u16((0x1800u + pTrPdo->pdoNr) - 1u, 5u, &u16);
		if (retVal == RET_OK)  {
			pTrPdo->eventTime = u16;
		}
		retVal = coOdGetDefaultVal_u8((0x1800u + pTrPdo->pdoNr) - 1u, 6u, &u8);
		if (retVal == RET_OK)  {
			pTrPdo->syncStartVal = u8;
		}

# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
		if (pTrPdo->dynMapping == CO_TRUE)  {
			(void)setupPdoTrMapTable(pTrPdo->mapTableIdx,
				(0x1a00u + pTrPdo->pdoNr) - 1u);
		}
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
	}
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
	/* receive PDO */
	for (cnt = 0u; cnt < pdoRecCnt; cnt++)  {
		pRecPdo = &pdoReceive[cnt];

		/* get disable and RTR bit from OD */
		retVal = coOdGetDefaultVal_u32((0x1400u + pRecPdo->pdoNr) - 1u, 1u, &u32);
		if (retVal == RET_OK)  {
			u32 &= (CO_COB_INVALID | CO_COB_RTR_BIT);
		} else {
			u32 = 0u;
		}

		if ((pRecPdo->pdoNr >= 1u)  && (pRecPdo->pdoNr <= 4u))  {
			/* calculate cobs for predefined connection set */
			pdoIdx = pRecPdo->pdoNr - 1u;
			pRecPdo->cobId = u32
				| ((0x200ul + (0x100ul * pdoIdx)) + coNmtGetNodeId());
		} else {
			/* disable sdo */
			pRecPdo->cobId = u32 | CO_COB_INVALID;
		}

		/* transmission type */
		retVal = coOdGetDefaultVal_u8((0x1400u + pRecPdo->pdoNr) - 1u, 2u, &u8);
		if (retVal == RET_OK)  {
			pRecPdo->transType = u8;
		}
		retVal = coOdGetDefaultVal_u16((0x1400u + pRecPdo->pdoNr) - 1u, 3u, &u16);
		if (retVal == RET_OK)  {
			pRecPdo->inhibit = u16;
		}
		retVal = coOdGetDefaultVal_u16((0x1400u + pRecPdo->pdoNr) - 1u, 5u, &u16);
		if (retVal == RET_OK)  {
			pRecPdo->eventTime = u16;
		}

# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
		if (pRecPdo->dynMapping == CO_TRUE)  {
			(void)setupPdoRecMapTable(pRecPdo->mapTableIdx,
					(0x1600u + pRecPdo->pdoNr) - 1u);
		}
# endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
	}
#endif /* CO_PDO_RECEIVE_CNT */

	return;
}


#ifdef CO_PDO_TRANSMIT_CNT
/***************************************************************************/
/**
* \brief coPdoTransmitInit - init transmit pdo functionality
*
* This function initializes a transmit PDO.
* The COB-ID is set at reset communication or at load parameter.
*
* Note: All parameters are reset to their default values at reset communication.
*
* \return RET_T
*
*/
RET_T coPdoTransmitInit(
		UNSIGNED16	pdoNr,					/**< PDO number */
		UNSIGNED8	transType,				/**< transmission type */
		UNSIGNED16	inhibit,				/**< inhibit time 100 usec */
		UNSIGNED16	eventTime,				/**< event timer in msec */
		UNSIGNED8	syncStartVal,			/**< sync start value */
		CO_CONST PDO_TR_MAP_TABLE_T	*mapTable	/**< pointer to mapping table */
	)
{
    UNSIGNED8	mapLen;
    RET_T		retVal = RET_OK;
# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
    UNSIGNED16	mapTableIdx;
# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */

	if (pdoTrCnt >= CO_PDO_TRANSMIT_CNT)  {
		return(RET_INVALID_PARAMETER);
	}

	pdoTransmit[pdoTrCnt].pdoNr = pdoNr;
	pdoTransmit[pdoTrCnt].cob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_PDO_TRANSMIT, pdoTrCnt);
	if (pdoTransmit[pdoTrCnt].cob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}
	pdoTransmit[pdoTrCnt].transType = transType;
	pdoTransmit[pdoTrCnt].actSyncCnt = transType;
	pdoTransmit[pdoTrCnt].inhibit = inhibit;
	pdoTransmit[pdoTrCnt].eventTime = eventTime;
	pdoTransmit[pdoTrCnt].syncStartVal = syncStartVal;
	pdoTransmit[pdoTrCnt].state = CO_PDO_STATE_READY;

	if (mapTable == NULL)  {
# ifdef CO_TR_PDO_DYN_MAP_ENTRIES
		/* dynamic mapping */
		mapTableIdx = createPdoTrMapTable((0x1a00u + pdoNr) - 1u);
		if (mapTableIdx == 0xffffu)  {
			return(RET_MAP_LEN_ERROR);
		}
		pdoTransmit[pdoTrCnt].mapTable = &mapTablesTrPDO[mapTableIdx];
		pdoTransmit[pdoTrCnt].mapTableConst = pdoTransmit[pdoTrCnt].mapTable;
		pdoTransmit[pdoTrCnt].mapTableIdx = mapTableIdx;
		pdoTransmit[pdoTrCnt].dynMapping = CO_TRUE;

# endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
	} else {
		/* static mapping */
		pdoTransmit[pdoTrCnt].mapTableConst = mapTable;
		pdoTransmit[pdoTrCnt].mapTable = NULL;
		pdoTransmit[pdoTrCnt].dynMapping = CO_FALSE;
#ifdef CO_TR_PDO_DYN_MAP_ENTRIES
		pdoTransmit[pdoTrCnt].mapTableIdx = 0xffffu;
#endif /* CO_TR_PDO_DYN_MAP_ENTRIES */
	}

	/* check mapping table */
	mapLen = checkTrPdoMappingTable(pdoNr, pdoTrCnt, 0u);
	if (mapLen <= CO_PDO_MAX_DATA_LEN)  {
		retVal = setPdoCob(pdoTransmit[pdoTrCnt].cob, CO_COB_INVALID, mapLen);
	} else {
		(void)setPdoCob(pdoTransmit[pdoTrCnt].cob, CO_COB_INVALID, 0u);
		retVal = RET_MAP_LEN_ERROR;
	}

	/* only for 254 255 */
	if (transType >= 254u)  {
		(void)icoCobSetInhibit(pdoTransmit[pdoTrCnt].cob, pdoTransmit[pdoTrCnt].inhibit);
	} else {
		(void)icoCobSetInhibit(pdoTransmit[pdoTrCnt].cob, 0u);
	}

	pdoTrCnt++;

	return(retVal);
}
#endif /* CO_PDO_TRANSMIT_CNT */


#ifdef CO_PDO_RECEIVE_CNT
/***************************************************************************/
/**
* \brief coPdoReceiveInit - init receive pdo functionality
*
* This function initializes a receive PDO.
* The COB-ID is set at reset communication or at load parameter.
*
* Note: All parameter are reset by their default values at reset communication.
*
* \return RET_T
*
*/
RET_T coPdoReceiveInit(
		UNSIGNED16	pdoNr,					/**< PDO number */
		UNSIGNED8	transType,				/**< transmission type */
		UNSIGNED16	inhibit,				/**< inhibit time 100 usec */
		UNSIGNED16	eventTime,				/**< event timer in msec */
		CO_CONST PDO_REC_MAP_TABLE_T *mapTable /**< pointer to mapping table */
	)
{
    UNSIGNED8	mapLen;
    RET_T		retVal = RET_OK;
#ifdef CO_REC_PDO_DYN_MAP_ENTRIES
    UNSIGNED16	mapTableIdx;
#endif /* CO_REC_PDO_DYN_MAP_ENTRIES */

	if (pdoRecCnt >= CO_PDO_RECEIVE_CNT)  {
		return(RET_INVALID_PARAMETER);
	}

	pdoReceive[pdoRecCnt].pdoNr = pdoNr;
	pdoReceive[pdoRecCnt].cob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_PDO_RECEIVE, pdoRecCnt);
	if (pdoReceive[pdoRecCnt].cob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}
	pdoReceive[pdoRecCnt].transType = transType;
	pdoReceive[pdoRecCnt].inhibit = inhibit;
	pdoReceive[pdoRecCnt].eventTime = eventTime;
	pdoReceive[pdoRecCnt].state = CO_PDO_STATE_READY;

	if (mapTable == NULL)  {
# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
		/* dynamic mapping */
		mapTableIdx = createPdoRecMapTable((0x1600u + pdoNr) - 1u);
		if (mapTableIdx == 0xffffu)  {
			return(RET_MAP_LEN_ERROR);
		}
		pdoReceive[pdoRecCnt].mapTable = &mapTablesRecPDO[mapTableIdx];
		pdoReceive[pdoRecCnt].mapTableConst = pdoReceive[pdoRecCnt].mapTable;
		pdoReceive[pdoRecCnt].mapTableIdx = mapTableIdx;
		pdoReceive[pdoRecCnt].dynMapping = CO_TRUE;
# endif /* CO_REC_PDO_DYN_MAP_ENTRIES */

	} else {
		/* static mapping */
		pdoReceive[pdoRecCnt].mapTable = NULL;
		pdoReceive[pdoRecCnt].mapTableConst = mapTable;
		pdoReceive[pdoRecCnt].dynMapping = CO_FALSE;
#ifdef CO_REC_PDO_DYN_MAP_ENTRIES
		pdoReceive[pdoRecCnt].mapTableIdx = 0xffffu;
#endif /* CO_REC_PDO_DYN_MAP_ENTRIES */
	}

	/* check mapping table */
	mapLen = checkRecPdoMappingTable(pdoNr, pdoRecCnt, 0u);
	if (mapLen <= CO_PDO_MAX_DATA_LEN)  {
		retVal = setPdoCob(pdoReceive[pdoRecCnt].cob, CO_COB_INVALID, mapLen);
	} else {
		(void)setPdoCob(pdoReceive[pdoRecCnt].cob, CO_COB_INVALID, 0u);
		retVal = RET_MAP_LEN_ERROR;
	}

	pdoRecCnt++;

	return(retVal);
}
#endif /* CO_PDO_RECEIVE_CNT */

#endif /* defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT) */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
