////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sdoserv.c
*@brief  contains sdo server routines
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
#include "co_odaccess.h"
#include "co_sdo.h"
#include "co_drv.h"
#include "co_timer.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_odaccess.h"
#include "ico_sdo.h"
#include "ico_nmt.h"
#include "ico_event.h"
#include "ico_sdoserver.h"
#include "canopen.h"
#include "co_canopen.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    UNSIGNED16  index;
    UNSIGNED8   subIndex;
    void        *func_point;
} DATAEXCHANGE_Table_Child;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static UNSIGNED32       g_co_write_data_len=0;

static UNSIGNED8        sdoServerCnt = 0u;
static CO_SDO_SERVER_T  sdoServer[CO_SDO_SERVER_CNT];

#ifdef CO_EVENT_SDO_SERVER_READ
static UNSIGNED8                sdoServerReadTableCnt = 0u;
static CO_EVENT_SDO_SERVER_T    sdoServerReadTable[CO_EVENT_SDO_SERVER_READ];
#endif /* CO_EVENT_SDO_SERVER_READ */

#ifdef CO_EVENT_SDO_SERVER_WRITE
static CO_EVENT_SDO_SERVER_T    sdoServerWriteTable[CO_EVENT_SDO_SERVER_WRITE];
static UNSIGNED8                sdoServerWriteTableCnt = 0u;
#endif /* CO_EVENT_SDO_SERVER_WRITE */

#ifdef CO_EVENT_SDO_SERVER_CHECK_WRITE
static UNSIGNED8                            sdoServerCheckWriteTableCnt = 0u;
static CO_EVENT_SDO_SERVER_CHECK_WRITE_T    sdoServerCheckWriteTable[CO_EVENT_SDO_SERVER_CHECK_WRITE];
#endif /* CO_EVENT_SDO_SERVER_CHECK_WRITE */

#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
static UNSIGNED8                            sdoServerDomainWriteTableCnt = 0u;
static CO_EVENT_SDO_SERVER_DOMAIN_WRITE_T   sdoServerDomainWriteTable[CO_EVENT_SDO_SERVER_DOMAIN_WRITE];
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */

DATAEXCHANGE_Table_Child DataExchange_CallbackTable[CANOPEN_REGISTER_SDOWRITE_CALLBACK_SIZE] = {0};
UNSIGNED8    guc_current_callback_table_size=0;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static CO_INLINE void       sdoCodeMultiplexer(CO_CONST CO_SDO_SERVER_T *pSdo,UNSIGNED8 pMp[]);
static CO_INLINE UNSIGNED16 searchSdoServerIndex(UNSIGNED16 sdoNr);
static RET_T                sdoServerReadInit(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T *pRecData);
static RET_T                sdoServerReadSegment(CO_SDO_SERVER_T *pSdo,CO_CONST CO_CAN_MSG_T *pRecData);
static RET_T                sdoServerWriteInit(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T *pRecData);
static RET_T                sdoServerWriteSegment(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T *pRecData);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   Inicialization of Table for callbacks
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void canopen_sdowrite_init_callback (void)
{

}

////////////////////////////////////////////////////////////////////////////////
/**
*   register callback fnc desired object write (sdo or pdo external change)
*
* @param[in]    index       - index of desired object
* @param[in]    subIndex    - subindex of desired object
* @param[in]    func_point  - callback fnc
* @return       canopen_sdo_reg_t - register callback success
*/
////////////////////////////////////////////////////////////////////////////////
canopen_sdo_reg_t icanopen_sdowrite_register_callback(UNSIGNED16  index,UNSIGNED8   subIndex,void * func_point)
{
    if (guc_current_callback_table_size >= CANOPEN_REGISTER_SDOWRITE_CALLBACK_SIZE)
    {
        return eCANOPEN_SDO_REG_ERR;
    }

    DataExchange_CallbackTable[guc_current_callback_table_size].index = index;
    DataExchange_CallbackTable[guc_current_callback_table_size].subIndex = subIndex;
    DataExchange_CallbackTable[guc_current_callback_table_size].func_point = func_point;

    //add, at last
    guc_current_callback_table_size++;

    return eCANOPEN_SDO_REG_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   call desired fnc at callback object write
*
* @param[in]    index       - index of desired object
* @param[in]    subIndex    - subindex of desired object
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void canopen_sdowrite_call_callback(UNSIGNED16  index,UNSIGNED8   subIndex)
{
    INTEGER16   i;
    INTEGER32   *pTemp;
    INTEGER32   *ppointer;
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    UNSIGNED32  *pu32_value_temp;
    UNSIGNED32  u32_value;
    UNSIGNED16  u16_value;
    UNSIGNED8   u8_value;
    INTEGER32   i32_value;
    INTEGER16   i16_value;
    INTEGER8    i8_value;
    REAL32      r32_value;
    RET_T       retVal;

    void* (*pfunction)(void * p_value)=NULL;

    //check table size
    if (guc_current_callback_table_size == 0)
    {
        //finish searching
        return;
    }

    //search callback
    for(i=0;i<guc_current_callback_table_size;i++)
    {
        //check index
        if (DataExchange_CallbackTable[i].index == index)
        {
            //check subindex
            if (DataExchange_CallbackTable[i].subIndex == subIndex)
            {
                //callback found

                // check if object exist ?
                if (coOdGetObjDescPtr(index, subIndex, &pDesc) == RET_OK)
                {
                    //check type and other things of object
                    switch(pDesc->dType){
                        case CO_DTYPE_U32_CONST:
                        case CO_DTYPE_U32_VAR:
                        case CO_DTYPE_U32_PTR:
                        case CO_DTYPE_U32_SDO_SERVER:
                        case CO_DTYPE_U32_SDO_CLIENT:
                        case CO_DTYPE_U32_TPDO:
                        case CO_DTYPE_U32_RPDO:
                        case CO_DTYPE_U32_TMAP:
                        case CO_DTYPE_U32_RMAP:
                        case CO_DTYPE_U32_ERRCTRL:
                        case CO_DTYPE_U32_EMCY:
                        case CO_DTYPE_U32_SYNC:
                        case CO_DTYPE_U32_STORE:
                             retVal = coOdGetObj_u32(index, subIndex, &u32_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&u32_value;
                             break;

                        case CO_DTYPE_U16_CONST:
                        case CO_DTYPE_U16_VAR:
                        case CO_DTYPE_U16_PTR:
                        case CO_DTYPE_U16_TPDO:
                        case CO_DTYPE_U16_RPDO:
                        case CO_DTYPE_U16_ERRCTRL:
                        case CO_DTYPE_U16_EMCY:
                             retVal = coOdGetObj_u16(index, subIndex, &u16_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&u16_value;
                             break;

                        case CO_DTYPE_U8_CONST:
                        case CO_DTYPE_U8_VAR:
                        case CO_DTYPE_U8_PTR:
                        case CO_DTYPE_U8_SDO_SERVER:
                        case CO_DTYPE_U8_SDO_CLIENT:
                        case CO_DTYPE_U8_TPDO:
                        case CO_DTYPE_U8_RPDO:
                        case CO_DTYPE_U8_TMAP:
                        case CO_DTYPE_U8_RMAP:
                        case CO_DTYPE_U8_SYNC:
                        case CO_DTYPE_U8_EMCY:
                        case CO_DTYPE_U8_ERRCTRL:
                             retVal = coOdGetObj_u8(index, subIndex, &u8_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&u8_value;
                             break;

                        case CO_DTYPE_I32_VAR:
                        case CO_DTYPE_I32_PTR:
                        case CO_DTYPE_I32_CONST:
                             retVal = coOdGetObj_i32(index, subIndex, &i32_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&i32_value;
                             break;

                        case CO_DTYPE_I16_VAR:
                        case CO_DTYPE_I16_PTR:
                        case CO_DTYPE_I16_CONST:
                             retVal = coOdGetObj_i16(index, subIndex, &i16_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&i16_value;
                             break;

                        case CO_DTYPE_I8_VAR:
                        case CO_DTYPE_I8_CONST:
                        case CO_DTYPE_I8_PTR:
                             retVal = coOdGetObj_i8(index, subIndex, &i8_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&i8_value;
                             break;

                        case CO_DTYPE_R32_VAR:
                        case CO_DTYPE_R32_PTR:
                        case CO_DTYPE_R32_CONST:
                             retVal = coOdGetObj_r32(index, subIndex, &r32_value);
                             if (retVal != RET_OK)
                                    return;
                             pu32_value_temp=(UNSIGNED32 *)&r32_value;
                             break;

                        default:
                            return;
                    }

                    //play with pointer to jump to function
                    pTemp=(INTEGER32 *)(DataExchange_CallbackTable[i].func_point);
                    ppointer=(INTEGER32 *)&pfunction;
                    *ppointer=(INTEGER32)pTemp;

                    //jump to callback with pointer to value. function itself takes care about proper pointer handling to final data type
                    (void)pfunction((void *)pu32_value_temp);
                }

                //finish searching
                return;
            }
        }
    }
}

/***************************************************************************/
/**
* \brief return info about write size
*
*
* \return nothing
*
*/
UNSIGNED32 co_sdowrite_datalen(void)
{
	return g_co_write_data_len;
}

/***************************************************************************/
/**
* \internal
*
* \brief sdoCodeMultiplexer - write multiplexer into message format
*
*
* \return nothing
*
*/
static CO_INLINE void sdoCodeMultiplexer(
		CO_CONST CO_SDO_SERVER_T *pSdo,		/* pointer to sdo */
		UNSIGNED8	pMp[]					/* pointer to multiplexer */
	)
{
	pMp[0] = (UNSIGNED8)(pSdo->index & 0xffu);
	pMp[1] = (UNSIGNED8)(pSdo->index >> 8u);
	pMp[2] = pSdo->subIndex;
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoDeCodeMultiplexer - write multiplexer into message format
*
*
* \return nothing
*
*/
CO_INLINE void icoSdoDeCodeMultiplexer(
		CO_CONST UNSIGNED8	pMp[],	/* pointer to multiplexer */
		CO_SDO_SERVER_T	    *pSdo	/* pointer to sdo */
	)
{
	pSdo->index = (UNSIGNED16)(((UNSIGNED16)(pMp[1])) << 8u) | (UNSIGNED16)pMp[0];
	pSdo->subIndex = pMp[2];
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoServerHandler - sdo server handler
*
*
* \return nothing
*
*/
void icoSdoServerHandler(
		const CO_REC_DATA_T	*pRecData			/* pointer to received data */
	)
{
    CO_SDO_SERVER_T	*pSdo;
    RET_T	retVal = RET_OK;
    CO_NMT_STATE_T nmtState;

	nmtState = coNmtGetState();

	/* OPERATIONAL ? */
	if ((nmtState != CO_NMT_STATE_PREOP)
	 && (nmtState != CO_NMT_STATE_OPERATIONAL))  {
		return;
	}

	/* check service index */
	if (pRecData->spec >= sdoServerCnt)  {
		return;
	}
	pSdo = &sdoServer[pRecData->spec];

	/* check for correct message len */
	if (pRecData->msg.len != 8u)  {
#ifdef CO_CANFD_SDO_SUPPORTED
		/* exception for some states */
		if (
			((pSdo->state == CO_SDO_STATE_FD_UPLOAD_SEGMENT)
		  && (pRecData->msg.len != 2u))
		 ||	((pSdo->state == CO_SDO_STATE_FD_DOWNLOAD_SEGMENT)
		  && (pRecData->msg.len < 3u))
		  )
#endif /* CO_CANFD_SDO_SUPPORTED */
		{
			return;
		}
	}

#ifdef CO_SDO_BLOCK
	/* block transfer active and not an abort */
	if ((pSdo->state == CO_SDO_STATE_BLOCK_DOWNLOAD) 
	 && (pRecData->msg.data[0] != CO_SDO_CS_ABORT)) {
		(void)icoSdoServerBlockWrite(pSdo, &pRecData->msg);
		return;
	}
#endif /* CO_SDO_BLOCK */

	switch (pRecData->msg.data[0] & CO_SDO_CCS_MASK)  {
		case CO_SDO_CS_ABORT:
			/* sdo abort handler */
			pSdo->state = CO_SDO_STATE_FREE;
			break;
		case CO_SDO_CCS_DOWNLOAD_INIT:		/* init download */
			retVal = sdoServerWriteInit(pSdo, &pRecData->msg);
			break;
		case CO_SDO_CCS_DOWNLOAD_SEGMENT:	/* segment download */
			retVal = sdoServerWriteSegment(pSdo, &pRecData->msg);
			break;
		case CO_SDO_CCS_UPLOAD_INIT:		/* init upload */
			retVal = sdoServerReadInit(pSdo, &pRecData->msg);
			break;
		case CO_SDO_CCS_UPLOAD_SEGMENT:		/* segment upload */
			retVal = sdoServerReadSegment(pSdo, &pRecData->msg);
			break;

#ifdef CO_SDO_BLOCK
		case CO_SDO_CCS_BLOCK_UPLOAD:		/* block upload */
			switch (pRecData->msg.data[0] & CO_SDO_CCS_BLOCK_SC_MASK)  {
				case CO_SDO_CCS_BLOCK_SC_UL_INIT:	/* block upload init */
					retVal = icoSdoServerBlockReadInit(pSdo, &pRecData->msg);
					break;
				case CO_SDO_CCS_BLOCK_SC_UL_BLK:	/* block upload blk */
					retVal = icoSdoServerBlockRead(pSdo);
					break;
				case CO_SDO_CCS_BLOCK_SC_UL_CON: 	/* block confirmation */
					retVal = icoSdoServerBlockReadCon(pSdo, &pRecData->msg);
					break;
				case CO_SDO_CCS_BLOCK_SC_UL_END:	/* block upload finished */
					pSdo->state = CO_SDO_STATE_FREE;
					break;
				default:
					break;
			}
			break;
		case CO_SDO_CCS_BLOCK_DOWNLOAD:		/* block download */
			switch (pRecData->msg.data[0] & CO_SDO_CCS_BLOCK_CS_MASK)  {
				case CO_SDO_CCS_BLOCK_CS_DL_INIT:	/* block download init */
					retVal = icoSdoServerBlockWriteInit(pSdo, &pRecData->msg);
					break;
				case CO_SDO_CCS_BLOCK_CS_DL_END:	/* block download end */
					retVal = icoSdoServerBlockWriteEnd(pSdo, &pRecData->msg);
					break;
				default:
					break;
			}
			break;
#endif /* CO_SDO_BLOCK */

		default:
			/* ignore wrong CCS */
			retVal = RET_SDO_UNKNOWN_CCS;
			icoSdoDeCodeMultiplexer(&pRecData->msg.data[1], pSdo);
			break;
	}

	if (retVal != RET_OK)  {
		icoSdoServerAbort(pSdo, retVal, CO_FALSE);
	}

	return;
}


/***************************************************************************/
/**
* \internal
*
* \brief sdoServerReadInit
*
* \return RET_T
*
*/
static RET_T sdoServerReadInit(
		CO_SDO_SERVER_T	    *pSdo,			/* pointer to sdo */
		const CO_CAN_MSG_T	*pRecData	/* pointer to received data */
	)
{
    RET_T	retVal;
    CO_TRANS_DATA_T	trData;
    UNSIGNED16	objAttr;
    #ifdef CO_CANFD_SDO_SUPPORTED
    UNSIGNED8	dataStartIdx;
    UNSIGNED8	dataSize;
    #endif /* CO_CANFD_SDO_SUPPORTED */

	/* transfer already actice ? */
	if (pSdo->state != CO_SDO_STATE_FREE)  {
		return(RET_SERVICE_BUSY);
	}

	/* save multiplexer */
	icoSdoDeCodeMultiplexer(&pRecData->data[1], pSdo);

	/* check index/subindex/attribute/size/limits */
	retVal = coOdGetObjDescPtr(pSdo->index, pSdo->subIndex, &pSdo->pObjDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* check attribute */
	objAttr = coOdGetObjAttribute(pSdo->pObjDesc);
	if ((objAttr & CO_ATTR_READ) == 0u)  {
		return(RET_NO_READ_PERM);
	}

	/* call all user indications */
	retVal = icoSdoCheckUserReadInd(pSdo->sdoNr, pSdo->index, pSdo->subIndex);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	pSdo->objSize = coOdGetObjSize(pSdo->pObjDesc);

	trData.data[1] = (UNSIGNED8)(pSdo->index & 0xffu);
	trData.data[2] = (UNSIGNED8)(pSdo->index >> 8);
	trData.data[3] = pSdo->subIndex;
	memset(&trData.data[4], 0, CO_CAN_MAX_DATA_LEN - 4u);


#ifdef CO_CANFD_SDO_SUPPORTED
#ifdef xxx
	/* multiple subindex access requested ? */
	if ((pRecData->data[6] & CO_SDO_CCS_FD_MSUB) != 0)  {
		pSdo->mSub = CO_TRUE;
	} else {
		pSdo->mSub = CO_FALSE;
	}
#endif

	/* maximum frame length from client */
	pSdo->maxCanLen = icoGetCanMsgLen(pRecData->data[7] & CO_SDO_CCS_FD_MFL_MASK);
	/* if client len > own len, use our len */
	if (pSdo->maxCanLen > CO_CAN_MAX_DATA_LEN)  {
		pSdo->maxCanLen = CO_CAN_MAX_DATA_LEN;
	}

	/* can fd request ? */
	if (pSdo->maxCanLen > 8u)  {
		trData.data[0] = CO_SDO_SCS_FD_UPLOAD_INIT;

		/* more than 4 bytes ?*/
		if (pSdo->objSize > 4u)  {
			
			dataStartIdx = CO_SDO_SCS_FD_HDR_LEN + CO_SDO_SCS_FD_SIZE_LEN;
			dataSize = pSdo->maxCanLen
				- (CO_SDO_SCS_FD_HDR_LEN + CO_SDO_SCS_FD_SIZE_LEN);

			/* overall size information */
			trData.data[4] = CO_SDO_SCS_FD4_LEN_INDICATED;
			coNumMemcpyUnpack(&trData.data[CO_SDO_SCS_FD_HDR_LEN],
				&pSdo->objSize, CO_SDO_SCS_FD_SIZE_LEN, CO_ATTR_NUM, 0u);

			/* segmented or fd-expedited mode ? */
			if (pSdo->objSize > dataSize)  {
				/* segmented mode */
				pSdo->toggle = 0u;
				pSdo->state = CO_SDO_STATE_FD_UPLOAD_SEGMENT;
			} else {
				/* expedited fd mode */
				dataSize = pSdo->objSize;

				/* padding bytes */
				trData.data[5] =
					icoGetCanTransferLen(dataStartIdx + pSdo->objSize)
					- (dataStartIdx + pSdo->objSize);
			}
		} else {
			/* no, use always expetided fd transfer */
			/* up to 4 data bytes */
			dataStartIdx = CO_SDO_SCS_FD_HDR_LEN;
			dataSize = pSdo->objSize;

			/* padding bytes */
			trData.data[5] = 4u - pSdo->objSize;
		}
			
		/* copy first data package */
		retVal = icoOdGetObj(pSdo->pObjDesc, &trData.data[dataStartIdx], 0u,
			dataSize, CO_FALSE);
		if (retVal != RET_OK)  {
			return(retVal);
		}
		pSdo->objSize -= dataSize;
		pSdo->transferedSize = dataSize;

		/* setup new data len */
		icoCobSetLen(pSdo->trCob, dataStartIdx + dataSize);
	} else
	{
		/* standard transfer */
		icoCobSetLen(pSdo->trCob, 8u);
#else /* CO_CANFD_SDO_SUPPORTED */
	{
#endif /* CO_CANFD_SDO_SUPPORTED */
		/* segmented transfer */
		if (pSdo->objSize > 4u)  {
			trData.data[0] = CO_SDO_SCS_UPLOAD_INIT | CO_SDO_SCS_LEN_INDICATED;
			coNumMemcpyUnpack(&trData.data[4], &pSdo->objSize, 4u, CO_ATTR_NUM, 0u);
			pSdo->transferedSize = 0u;
			pSdo->toggle = 0u;

			pSdo->state = CO_SDO_STATE_UPLOAD_SEGMENT;
		} else {

			/* expetided transfer */

			/* SCS - number of bytes specified */
			trData.data[0] = (UNSIGNED8)(CO_SDO_SCS_UPLOAD_INIT
				| ((4u - pSdo->objSize) << 2)
				| CO_SDO_SCS_EXPEDITED | CO_SDO_SCS_LEN_INDICATED);
			pSdo->state = CO_SDO_STATE_FREE;

			retVal = icoOdGetObj(pSdo->pObjDesc, &trData.data[4], 0u, 4u, CO_FALSE);
			if (retVal != RET_OK)  {
				return(retVal);
			}
		}
	}

	/* transmit answer */
	retVal = icoTransmitMessage(pSdo->trCob, &trData, 0u);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief sdoServerReadSegment
*
* \return RET_T
*/
static RET_T sdoServerReadSegment(
		CO_SDO_SERVER_T	        *pSdo,		/* pointer to sdo */
		CO_CONST CO_CAN_MSG_T	*pRecData	/* pointer to received data */
	)
{
    RET_T	retVal;
    CO_TRANS_DATA_T	trData;
    UNSIGNED32	size;
    UNSIGNED8	toggle;
    #ifdef CO_CANFD_SDO_SUPPORTED
    UNSIGNED8	transferSize;
    #endif /* CO_CANFD_SDO_SUPPORTED */
    UNSIGNED8	dataOffs;
    UNSIGNED8   regular_read_segment;

	if ((pSdo->state != CO_SDO_STATE_UPLOAD_SEGMENT)
#ifdef CO_CANFD_SDO_SUPPORTED
	 && (pSdo->state != CO_SDO_STATE_FD_UPLOAD_SEGMENT)
#endif /* CO_CANFD_SDO_SUPPORTED */
	)  {
		return(RET_SDO_UNKNOWN_CCS);
	}

	memset(&trData.data[1], 0, CO_CAN_MAX_DATA_LEN - 1u);

	toggle = pRecData->data[0] & CO_SDO_CCS_TOGGLE_BIT;
	if (pSdo->toggle != toggle)  {
		return(RET_TOGGLE_MISMATCH);
	}
	if (toggle == 0u)  {
		pSdo->toggle = CO_SDO_CCS_TOGGLE_BIT;
	} else {
		pSdo->toggle = 0u;
	}
	trData.data[0] = CO_SDO_SCS_UPLOAD_SEGMENT;
	trData.data[0] |= toggle;

#ifdef CO_CANFD_SDO_SUPPORTED
	/* fd ransfer ? */
	if (pSdo->maxCanLen > 2)  {
//printf("sdoServerReadSegment: objsize %d, transferedsize %d",
//	pSdo->objSize, pSdo->transferedSize);
		transferSize = icoGetCanTransferLen(pSdo->maxCanLen);
		if (pSdo->objSize > transferSize)  {
			size = transferSize - 2u;
//printf(" transfersize %d, datasize %d\n", transferSize, size);
//			trData.data[1] = 0;
		} else {
			transferSize = icoGetCanTransferLen(pSdo->objSize + 2u);
			trData.data[0] |= (UNSIGNED8)(CO_SDO_SCS_FD_CONT_FLAG);
			trData.data[1] = transferSize - pSdo->objSize;
			size = pSdo->objSize;
			pSdo->state = CO_SDO_STATE_FREE;
		}
		dataOffs = 2;
	} else
#endif /* CO_CANFD_SDO_SUPPORTED */
	{
		/* last transfer ? */
		if (pSdo->objSize > 7u)  {
			size = 7u;
		} else {
			/* last transfer */
			size = pSdo->objSize;
			trData.data[0] |= (UNSIGNED8)(CO_SDO_SCS_CONT_FLAG | ((7u - size) << 1));

			pSdo->state = CO_SDO_STATE_FREE;
		}
#ifdef CO_CANFD_SDO_SUPPORTED
		transferSize = 8u;
#endif /* CO_CANFD_SDO_SUPPORTED */
		dataOffs = 1u;
	}

    regular_read_segment = 0;
    retVal = co_odgetobjdomain(&trData.data[dataOffs],size, pSdo->index,pSdo->subIndex,&regular_read_segment);
    if (regular_read_segment == 1)
    {
        retVal = icoOdGetObj(pSdo->pObjDesc, &trData.data[dataOffs],pSdo->transferedSize, size, CO_FALSE);
    }
	
	if (retVal != RET_OK)
	{
		return(retVal);
	}

	pSdo->objSize -= size;
	pSdo->transferedSize += size;

#ifdef CO_CANFD_SDO_SUPPORTED
	/* standard transfer */
	icoCobSetLen(pSdo->trCob, transferSize);
#endif /* CO_CANFD_SDO_SUPPORTED */

	/* transmit answer */
	retVal = icoTransmitMessage(pSdo->trCob, &trData, 0u);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief sdoServerWriteInit
*
* \return RET_T
*
*/
static RET_T sdoServerWriteInit(
		CO_SDO_SERVER_T	    *pSdo,			/* pointer to sdo */
		const CO_CAN_MSG_T	*pRecData		/* pointer to receive data */
	)
{
    RET_T	retVal;
    CO_TRANS_DATA_T	trData;
    UNSIGNED16	objAttr;
    UNSIGNED32	len = 0u;
    BOOL_T		segmented = CO_FALSE;
    UNSIGNED8	u8;
    BOOL_T		changed = CO_FALSE;
    UNSIGNED8	dataStartIdx = 4u;
    #ifdef CO_CANFD_SDO_SUPPORTED
    BOOL_T		canFd = CO_FALSE;
    #endif /* CO_CANFD_SDO_SUPPORTED */

	/* transfer already actice ? */
	if (pSdo->state != CO_SDO_STATE_FREE)  {
		return(RET_SERVICE_BUSY);
	}

	/* save multiplexer */
	icoSdoDeCodeMultiplexer(&pRecData->data[1], pSdo);

	/* check index/subindex/attribute/size/limits */
	retVal = coOdGetObjDescPtr(pSdo->index, pSdo->subIndex, &pSdo->pObjDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}
	pSdo->objSize = coOdGetObjSize(pSdo->pObjDesc);

	/* check attribute */
	objAttr = coOdGetObjAttribute(pSdo->pObjDesc);
	if ((objAttr & CO_ATTR_WRITE) == 0u)  {
		return(RET_NO_WRITE_PERM);
	}

	/* check data size */
	switch (pRecData->data[0] & CO_SDO_CCS_WRITE_TYPE_MASK)  {
		case 1u:		/* count in byte 5..8 */
			(void)coNumMemcpyPack(&len, &pRecData->data[4], 4u, CO_ATTR_NUM, 0u);
			segmented = CO_TRUE;
			break;
		case 3u:		/* count = 4-n */
			u8 = (pRecData->data[0] & CO_SDO_CCS_LEN_MASK) >> 2;
			len = 4ul - u8;
			break;
		case 2u:		/* size unspecified */
			len = pSdo->objSize;
			break;
#ifdef CO_CANFD_SDO_SUPPORTED
		case 0u:		/* can fd frame */
			canFd = CO_TRUE;
			/* size indicated ? */
			if ((pRecData->data[4] & CO_SDO_CCS_FD4_LEN_INDICATED) != 0)  {
				/* len is indicated */
				(void)coNumMemcpyPack(&len,
						&pRecData->data[CO_SDO_SCS_FD_HDR_LEN], 4u,
						CO_ATTR_NUM, 0u);
				dataStartIdx = CO_SDO_SCS_FD_HDR_LEN + CO_SDO_SCS_FD_SIZE_LEN;
				/* segmented transfer ? */
				if (len > (pRecData->len - 
						(CO_SDO_SCS_FD_HDR_LEN + pRecData->data[5])))  {
					segmented = CO_TRUE;
				}
			} else {
				/* no size information, use
					padding	len		len
					0		max		objsize		segmented
					0		< max	msgsize		fd exp
					!=				msgsize		fd exp
				*/
				if ((pRecData->len == CO_CAN_MAX_DATA_LEN)
				 && (pRecData->data[5] == 0))  {
					/* segmented transfer */
					segmented = CO_TRUE;
					len = pSdo->objSize;
				} else {
					len = pRecData->len - 
						(CO_SDO_SCS_FD_HDR_LEN + pRecData->data[5]);
				}
				dataStartIdx = CO_SDO_SCS_FD_HDR_LEN;
			}
			break;
#endif /* CO_CANFD_SDO_SUPPORTED */
		default:
			return(RET_SDO_UNKNOWN_CCS);
	}

	g_co_write_data_len=len;
		
	/* len doesnt match */
	if (len != pSdo->objSize)  {
		/* not enough memory ? */
		if (len > pSdo->objSize)  {
			return(RET_SDO_DATA_TYPE_NOT_MATCH);
		} else {
			if ((objAttr & CO_ATTR_NUM) != 0u)   {
				return(RET_SDO_DATA_TYPE_NOT_MATCH);
			}
		}
	}

	/* check limits */
/*	if (segmented != CO_TRUE)  { */
	retVal = icoOdCheckObjLimits(pSdo->pObjDesc, &pRecData->data[dataStartIdx]);
	if (retVal != RET_OK)  {
		return(retVal);
	}
/*	} */

	/* call all user indications */
	retVal = icoSdoCheckUserCheckWriteInd(pSdo->sdoNr, pSdo->index,
			pSdo->subIndex, &pRecData->data[dataStartIdx]);
	if (retVal != RET_OK)  {
		/* return error */
		return(retVal);
	}

#ifdef CO_CANFD_SDO_SUPPORTED
	/* standard transfer */
	icoCobSetLen(pSdo->trCob, 8);
#endif /* CO_CANFD_SDO_SUPPORTED */

	/* segmented transfer ? */
	if (segmented == CO_TRUE)  {
		trData.data[0] = CO_SDO_SCS_DOWNLOAD_INIT;
		trData.data[1] = (UNSIGNED8)(pSdo->index & 0xffu);
		trData.data[2] = (UNSIGNED8)(pSdo->index >> 8u);
		trData.data[3] = pSdo->subIndex;
		memset(&trData.data[4], 0, 4u);

		/* transmit answer */
		retVal = icoTransmitMessage(pSdo->trCob, &trData, 0u);

		pSdo->transferedSize = 0u;
#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
		pSdo->domainTransferedSize = 0u;
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */
		pSdo->toggle = 0u;

#ifdef CO_CANFD_SDO_SUPPORTED
		if (canFd == CO_TRUE)  {

			/* put data */
			len = pRecData->len - dataStartIdx;
			retVal = icoOdPutObj(pSdo->pObjDesc, &pRecData->data[dataStartIdx],
				pSdo->transferedSize, len, CO_FALSE, &changed);
			if (retVal != RET_OK)  {
				/* return error */
				return(retVal);
			}
			pSdo->objSize -= len;
			pSdo->transferedSize += len;
#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
			pSdo->domainTransferedSize += len;
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */

			pSdo->state = CO_SDO_STATE_FD_DOWNLOAD_SEGMENT;
		} else 
#endif /* CO_CANFD_SDO_SUPPORTED */
		{
			pSdo->state = CO_SDO_STATE_DOWNLOAD_SEGMENT;
		}

#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
		if (pSdo->pObjDesc->dType == CO_DTYPE_DOMAIN)  {	
			pSdo->domainTransfer = CO_TRUE;
		} else {
			pSdo->domainTransfer = CO_FALSE;
		}
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */
		return(retVal);
	}

	/* expedited transfer */

	/* save orginal value */
	if (pSdo->objSize <= MAX_SAVE_DATA)  {
		retVal = icoOdGetObj(pSdo->pObjDesc, &pSdo->saveData[0], 0u,
			MAX_SAVE_DATA, CO_FALSE);
	}

	if (retVal == RET_OK)  {
		/* put new value */
		retVal = icoOdPutObj(pSdo->pObjDesc, &pRecData->data[dataStartIdx],
			0u, len, CO_FALSE, &changed);
	}
	if (retVal != RET_OK)  {
		/* return error */
		return(retVal);
	}

	/* call user indication */
	retVal = icoSdoCheckUserWriteInd(pSdo);
	if (retVal != RET_OK)  {

		/* restore data, if user ind returns error */
		/* restore original entry */
		if (pSdo->objSize <= MAX_SAVE_DATA)  {
			(void)icoOdPutObj(pSdo->pObjDesc, &pSdo->saveData[0], 0u,
				MAX_SAVE_DATA, CO_FALSE, &changed);
		}

		/* return error */
		return(retVal);
	}

	/* call object change indication */
	retVal = icoEventObjectChanged(pSdo->pObjDesc, pSdo->index, changed);
	if (retVal != RET_OK)  {
		/* return error */
		return(retVal);
	}

	/* SCS - number of bytes specified */
	trData.data[0] = CO_SDO_SCS_DOWNLOAD_INIT;
	trData.data[1] = (UNSIGNED8)(pSdo->index & 0xffu);
	trData.data[2] = (UNSIGNED8)(pSdo->index >> 8u);
	trData.data[3] = pSdo->subIndex;

	memset(&trData.data[4], 0, 4u);

	/* transmit answer */
	retVal = icoTransmitMessage(pSdo->trCob, &trData, 0u);

	pSdo->state = CO_SDO_STATE_FREE;

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief sdoServerWriteSegment
*
* \return RET_T
*
*/
static RET_T sdoServerWriteSegment(
		CO_SDO_SERVER_T	    *pSdo,			/* pointer to sdo */
		const CO_CAN_MSG_T	*pRecData		/* pointer to received data */
	)
{
    RET_T	    retVal;
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	toggle;
    UNSIGNED8	u8;
    UNSIGNED32	size;
    BOOL_T		changed;
    UNSIGNED8	dataOffs = 1u;

	/* transfer already actice ? */
	if ((pSdo->state != CO_SDO_STATE_DOWNLOAD_SEGMENT)
	 && (pSdo->state != CO_SDO_STATE_FD_DOWNLOAD_SEGMENT))  {
		return(RET_SERVICE_BUSY);
	}

	toggle = pRecData->data[0] & CO_SDO_CCS_TOGGLE_BIT;
	if (pSdo->toggle != toggle)  {
		return(RET_TOGGLE_MISMATCH);
	}
	if (toggle == 0u)  {
		pSdo->toggle = CO_SDO_CCS_TOGGLE_BIT;
	} else {
		pSdo->toggle = 0u;
	}

#ifdef CO_CANFD_SDO_SUPPORTED
	/* can fd transfer ? */
	if (pSdo->state == CO_SDO_STATE_FD_DOWNLOAD_SEGMENT)  {
		size = pRecData->len - pRecData->data[1] - 2u;
		dataOffs = 2u;
	} else 
#endif /* CO_CANFD_SDO_SUPPORTED */
	{
		u8 = (pRecData->data[0] & CO_SDO_CCS_LEN_MASK) >> 1;
		size = 7ul - u8;
	}
	if (size > pSdo->objSize)  {
		return(RET_OUT_OF_MEMORY);
	}

	/* put data */
	retVal = icoOdPutObj(pSdo->pObjDesc, &pRecData->data[dataOffs],
		pSdo->transferedSize, size, CO_FALSE, &changed);
	if (retVal != RET_OK)  {
		/* return error */
		return(retVal);
	}
	pSdo->objSize -= size;
	pSdo->transferedSize += size;

#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
	pSdo->domainTransferedSize += size;

	/* domain transfer ? */
	if (pSdo->domainTransfer == CO_TRUE)  {
		/* size reached or last segment */
		if (((pSdo->transferedSize % (7 * CO_SDO_SERVER_DOMAIN_CNT)) == 0)
		 ||	((pRecData->data[0] & CO_SDO_CCS_CONT_BIT) != 0u))  {
			/* domain indication */
			retVal=icoSdoDomainUserWriteInd(pSdo);
			/* reset domain pointer */
			pSdo->transferedSize = 0;
			 
			if (retVal != RET_OK)
				return retVal;
		}
	}
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */

#ifdef CO_CANFD_SDO_SUPPORTED
	/* can fd transfer ? */
	if (pSdo->state == CO_SDO_STATE_FD_DOWNLOAD_SEGMENT)  {
		icoCobSetLen(pSdo->trCob, 2u);
	} else {
		icoCobSetLen(pSdo->trCob, 8u);
	}
#endif /* CO_CANFD_SDO_SUPPORTED */

	/* last transfer ? */
	if ((pRecData->data[0] & CO_SDO_CCS_CONT_BIT) != 0u)  {
		/* last segment */

		/* call user indication */
		retVal = icoSdoCheckUserWriteInd(pSdo);
		if (retVal != RET_OK)  {
			/* return error */
			return(retVal);
		}

		pSdo->state = CO_SDO_STATE_FREE;

		retVal = icoEventObjectChanged(pSdo->pObjDesc, pSdo->index, changed);
		if (retVal != RET_OK)  {
			return(retVal);
		}
	}


	trData.data[0] = CO_SDO_SCS_DOWNLOAD_SEGMENT | toggle;
	memset(&trData.data[1], 0, 7u);

	/* transmit answer */
	retVal = icoTransmitMessage(pSdo->trCob, &trData, 0u);

	return(retVal);
}



/***************************************************************************/
/**
* \internal
*
* \brief icoSdoServerAbort - abort sdo transfer
*
* \return none
*
*/
void icoSdoServerAbort(
		CO_SDO_SERVER_T	*pSdo,			/* pointer to sdo */
		RET_T	errorReason,			/* error reason */
		BOOL_T	fromClient				/* called from client */
    )
{
    typedef struct {
        RET_T	reason;			/* error reason */
        UNSIGNED32	abortCode;	/* sdo abort code */
    } SDO_ABORT_CODE_TABLE_T;

    const SDO_ABORT_CODE_TABLE_T	abortCodeTable[] = {
        { RET_TOGGLE_MISMATCH,				0x05030000uL	},
        { RET_SDO_UNKNOWN_CCS,				0x05040001uL	},
        { RET_SERVICE_BUSY,					0x05040001uL	},
        { RET_SDO_WRONG_BLOCKSIZE,			0x05040002uL	},
        { RET_SDO_WRONG_SEQ_NR,				0x05040003uL	},
        { RET_SDO_CRC_ERROR,				0x05040004uL	},
        { RET_OUT_OF_MEMORY,				0x05040005uL	},
        { RET_SDO_TRANSFER_NOT_SUPPORTED,	0x06010000uL	},
        { RET_NO_READ_PERM,					0x06010001uL	},
        { RET_NO_WRITE_PERM,				0x06010002uL	},
        { RET_IDX_NOT_FOUND,				0x06020000uL	},
        { RET_OD_ACCESS_ERROR,				0x06040047uL	},
        { RET_HARDWARE_ERROR,				0x06060000uL	},
        { RET_SDO_DATA_TYPE_NOT_MATCH,		0x06070010uL	},
        { RET_SUBIDX_NOT_FOUND,				0x06090011uL	},
        { RET_SDO_INVALID_VALUE,			0x06090030uL	},
        { RET_MAP_ERROR,					0x06040041uL	},
        { RET_MAP_LEN_ERROR,				0x06040042uL	},
        { RET_PARAMETER_INCOMPATIBLE,		0x06040043uL	},
        { RET_ERROR_STORE,					0x08000020uL	},
        { RET_ERROR_PRESENT_DEVICE_STATE,	0x08000022uL	},
        { RET_VALUE_NOT_AVAILABLE,			0x08000024uL	},
    };
    CO_TRANS_DATA_T	trData;
    UNSIGNED32	abortCode = 0x08000000UL;
    UNSIGNED16	i;

	if (fromClient == CO_TRUE)
	{
		/* user indication */

	}
	else
	{
		for (i = 0u; i < (sizeof(abortCodeTable) / sizeof(SDO_ABORT_CODE_TABLE_T)); i++)
		{
			if (abortCodeTable[i].reason == errorReason)
			{
				abortCode = abortCodeTable[i].abortCode;
				break;
			}
		}

		trData.data[0] = CO_SDO_CS_ABORT;
		sdoCodeMultiplexer(pSdo, &trData.data[1]);
		coNumMemcpyUnpack(&trData.data[4], &abortCode, 4u, CO_ATTR_NUM, 0u);
#ifdef CO_CANFD_SDO_SUPPORTED
		icoCobSetLen(pSdo->trCob, 8);
#endif /* CO_CANFD_SDO_SUPPORTED */
		(void) icoTransmitMessage(pSdo->trCob, &trData, 0u);
	}

	pSdo->state = CO_SDO_STATE_FREE;

}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
#ifdef CO_EVENT_SDO_SERVER_READ
/***************************************************************************/
/**
* \brief coEventRegister_SdoServer - register SDO server event
*
* This function registers a sdo server indication function,
* which is called before a SDO read request is executed,
* so the application can update the data before the response is
* sent.
*
* \return RET_T
*
*/

RET_T coEventRegister_SDO_SERVER_READ(
		CO_EVENT_SDO_SERVER_T pFunction		/**< pointer to function */
    )
{
	if (sdoServerReadTableCnt >= CO_EVENT_SDO_SERVER_READ)
	{
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	sdoServerReadTable[sdoServerReadTableCnt] = pFunction;
	sdoServerReadTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_SDO_SERVER_READ */


#ifdef CO_EVENT_SDO_SERVER_CHECK_WRITE
/***************************************************************************/
/**
* \brief coEventRegister_SdoServerCheckWrite - register SDO server write event
*
* This function register a sdo server indication function,
* which is called before SDO write access is executed,
* so the application can reject an SDO write access. 
*
*
* \return RET_T
*
*/

RET_T coEventRegister_SDO_SERVER_CHECK_WRITE(
		CO_EVENT_SDO_SERVER_CHECK_WRITE_T pFunction /**< pointer to function */
    )
{
	if (sdoServerCheckWriteTableCnt >= CO_EVENT_SDO_SERVER_CHECK_WRITE)
	{
		return(RET_EVENT_NO_RESSOURCE);
	}

	sdoServerCheckWriteTable[sdoServerCheckWriteTableCnt] = pFunction;
	sdoServerCheckWriteTableCnt++;

	return(RET_OK);
}
#endif /*  CO_EVENT_SDO_SERVER_CHECK_WRITE */


#ifdef CO_EVENT_SDO_SERVER_WRITE
/***************************************************************************/
/**
* \brief coEventRegister_SdoServerWrite - register SDO server write event
*
* This function registers a SDO server write indication function.
* It is called, after a SDO write access was finished.
*
* \return RET_T
*
*/

RET_T coEventRegister_SDO_SERVER_WRITE(
		CO_EVENT_SDO_SERVER_T	pFunction	/**< pointer to function */
    )
{
	if (sdoServerWriteTableCnt >= CO_EVENT_SDO_SERVER_WRITE)
	{
		return(RET_EVENT_NO_RESSOURCE);
	}

	sdoServerWriteTable[sdoServerWriteTableCnt] = pFunction;
	sdoServerWriteTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_SDO_SERVER_WRITE */

#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
/***************************************************************************/
/**
* \brief coEventRegister_SdoServerDomainWrite - register SDO server domain write event
*
* This function registers a SDO server write domain indication function.
* It is called, after a specified portion of bytes at a domain transfer
* was receiced.
* After the function was called, the domain pointer start address is refreshed,
* so new data are saved at start of the domain again.
* The application can use this function to save may be the data at flash.
*
*
* \return RET_T
*
*/

RET_T coEventRegister_SDO_SERVER_DOMAIN_WRITE(
		CO_EVENT_SDO_SERVER_DOMAIN_WRITE_T	pFunction	/**< pointer to function */
    )
{
	if (sdoServerDomainWriteTableCnt >= CO_EVENT_SDO_SERVER_DOMAIN_WRITE)
	{
		return(RET_EVENT_NO_RESSOURCE);
	}

	sdoServerDomainWriteTable[sdoServerDomainWriteTableCnt] = pFunction;
	sdoServerDomainWriteTableCnt++;

	return(RET_OK);
}
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoCheckUserReadInd - check user indication
*
* \return RET_T
*
*/
RET_T icoSdoCheckUserReadInd(
		UNSIGNED8	sdoNr,			/* sdo number */
		UNSIGNED16	index,			/* index */
		UNSIGNED8	subIndex		/* subindex */
	)
{
#ifdef CO_EVENT_SDO_SERVER_READ
    RET_T	retVal = RET_OK;
    UNSIGNED8	cnt;

	/* check all indications for correct return value */
	cnt = sdoServerReadTableCnt;
	while (cnt > 0u)
	{
		cnt--;

		/* check user indication */
		retVal = sdoServerReadTable[cnt](CO_FALSE, sdoNr, index, subIndex);
		if (retVal != RET_OK)
		{
			break;
		}
	}

	/* return, if not ok */
	if (retVal != RET_OK)
	{
		return(retVal);
	 }

	/* call indication to execute */
	cnt = sdoServerReadTableCnt;
	while (cnt > 0u)
	{
		cnt--;
		/* call user indication */
		(void)sdoServerReadTable[cnt](CO_TRUE, sdoNr, index, subIndex);
	}
#endif /* CO_EVENT_SDO_SERVER_READ */

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoCheckUserCheckWriteInd - check user write indication
*
* \return RET_T
*
*/
RET_T icoSdoCheckUserCheckWriteInd(
		UNSIGNED8	sdoNr,			/* sdo number */
		UNSIGNED16	index,			/* index */
		UNSIGNED8	subIndex,		/* subindex */
		const UNSIGNED8	*pData		/* pointer to received data */
	)
{
	/* check all indications for correct return value */
    #ifdef CO_EVENT_SDO_SERVER_CHECK_WRITE
    RET_T	retVal = RET_OK;
    UNSIGNED8	cnt;

	cnt = sdoServerCheckWriteTableCnt;
	while (cnt > 0u)
	{
		cnt--;

		/* check user indication */
		retVal = sdoServerCheckWriteTable[cnt](CO_FALSE, sdoNr, index, subIndex, pData);
		if (retVal != RET_OK)
		{
			break;
		}
	}

	/* return, if not ok */
	if (retVal != RET_OK)
	{
		return(retVal);
	}

	/* call indication to execute */
	cnt = sdoServerCheckWriteTableCnt;
	while (cnt > 0u)
	{
		cnt--;
		/* call user indication */
		(void)sdoServerCheckWriteTable[cnt](CO_TRUE, sdoNr, index, subIndex, pData);
	}
#endif /* CO_EVENT_SDO_SERVER_CHECK_WRITE */

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoCheckUserWriteInd - check user write indication
*
* \return RET_T
*
*/
RET_T icoSdoCheckUserWriteInd(
		const CO_SDO_SERVER_T	*pSdo		/* pointer to sdo */
	)
{
    RET_T	retVal = RET_OK;

#ifdef CO_EVENT_SDO_SERVER_WRITE
    UNSIGNED8	cnt;

	/* check all indications for correct return value */
	cnt = sdoServerWriteTableCnt;
	while (cnt > 0u)  {
		cnt--;

		/* check user indication */
		retVal = sdoServerWriteTable[cnt](CO_FALSE, pSdo->sdoNr, pSdo->index,
				pSdo->subIndex);
		if (retVal != RET_OK)  {
			break;
		}
	}

	/* return, if not ok */
	if (retVal != RET_OK)  {
		return(retVal);
	 }

	/* call indication to execute */
	cnt = sdoServerWriteTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		(void)sdoServerWriteTable[cnt](CO_TRUE, pSdo->sdoNr, pSdo->index,
				pSdo->subIndex);

	}

    canopen_sdowrite_call_callback(pSdo->index,pSdo->subIndex);
#endif /* CO_EVENT_SDO_SERVER_WRITE */

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoDomainUserWriteInd - check user domain write indication
*
* \return RET_T
*
*/
RET_T icoSdoDomainUserWriteInd(
		const CO_SDO_SERVER_T	*pSdo		/* pointer to sdo */
	)
{
#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
    UNSIGNED8	cnt;
	RET_T retVal;

	/* check all indications for correct return value */
	cnt = sdoServerDomainWriteTableCnt;
	while (cnt > 0u)  {
		cnt--;

		retVal=sdoServerDomainWriteTable[cnt](pSdo->index, pSdo->subIndex,
				pSdo->transferedSize, pSdo->domainTransferedSize);
		
		if (retVal != RET_OK)  {
			return retVal;
		}
	}
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */

	return RET_OK;
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief icoSdoGetObjectAddr - get sdo server object address
*
* \return pointer to address
*
*/
void *icoSdoGetObjectAddr(
		UNSIGNED16	sdoNr,			/* sdo number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    void	*pAddr = NULL;

    idx = searchSdoServerIndex(sdoNr);
	if (idx == 0xffffu)  {
		return(NULL);
	}

	switch (subIndex)  {
		case 1:
			/* copy from cob handler */
			pAddr = (void *)&sdoServer[idx].recCobId;
			break;
		case 2:
			/* copy from cob handler */
			pAddr = (void *)&sdoServer[idx].trCobId;
			break;
		case 3:
			pAddr = (void *)&sdoServer[idx].node;
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
* \brief icoSdoCheckObjLimitNode - check object limits node entry
*
*
* \return RET_T
*/
RET_T icoSdoCheckObjLimitNode(
		UNSIGNED16	sdoNr		/* sdo number */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchSdoServerIndex(sdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoCheckObjLimitCobId - check cob-id
*
*
* \return RET_T
*/
RET_T icoSdoCheckObjLimitCobId(
		UNSIGNED16	sdoNr,		/* sdo number */
		UNSIGNED8	subIndex,	/* sub index */
		UNSIGNED32	canId		/* pointer to receive data */
	)
{
    RET_T	retVal = RET_OK;
    UNSIGNED16	idx;

    idx = searchSdoServerIndex(sdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	if (subIndex == 1u)  {
		if ((canId & CO_COB_VALID_MASK) == 0u)  {
			/* new cobid is valid, only allowed if cob was disabled before*/
			if ((sdoServer[idx].recCobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
				return(RET_SDO_INVALID_VALUE);
			}
			if (icoCheckRestrictedCobs(canId, 0x600ul, 0x600ul) == CO_TRUE)  {
				return(RET_SDO_INVALID_VALUE);
			}
		}
	} else 
	if (subIndex == 2u)  {
		if ((canId & CO_COB_VALID_MASK) == 0u)  {
			/* new cobid is valid, only allowed if cob was disabled before*/
			if ((sdoServer[idx].trCobId & CO_COB_VALID_MASK) != CO_COB_INVALID)  {
				return(RET_SDO_INVALID_VALUE);
			}
			if (icoCheckRestrictedCobs(canId, 0x580ul, 0x580ul) == CO_TRUE)  {
				return(RET_SDO_INVALID_VALUE);
			}
		}
	} else {
		retVal = RET_SUBIDX_NOT_FOUND;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoObjChanged - sdo object was changed
*
*
* \return nothing
*/
RET_T icoSdoObjChanged(
		UNSIGNED16	sdoNr,			/* sdo number */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    UNSIGNED16	idx;
    RET_T		retVal = RET_OK;

    idx = searchSdoServerIndex(sdoNr);
	if (idx == 0xffffu)  {
		return(RET_SDO_TRANSFER_NOT_SUPPORTED);
	}

	switch (subIndex)  {
		case 1u:
			retVal = icoCobSet(sdoServer[idx].recCob, sdoServer[idx].recCobId,
				CO_COB_RTR_NONE, 8u);
			break;
		case 2u:
			retVal = icoCobSet(sdoServer[idx].trCob, sdoServer[idx].trCobId,
				CO_COB_RTR_NONE, 8u);
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
* \brief searchSdoServerIndex - search for sdo server index
*
*
* \return index
*
*/
static CO_INLINE UNSIGNED16 searchSdoServerIndex(
		UNSIGNED16	sdoNr
	)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < sdoServerCnt; cnt++)  {
		if (sdoNr == sdoServer[cnt].sdoNr)  {
			return(cnt);
		}
	}

	return(0xffffu);
}


/***************************************************************************/
/**
* \internal
*
* \brief coResetSdoServer - reset comm for sdo server
*
* \return none
*
*/
void icoSdoServerReset(
		void
	)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < sdoServerCnt; cnt++)  {
		sdoServer[cnt].state = CO_SDO_STATE_FREE;

		/* ignore temporary cob-ids */
		if ((sdoServer[cnt].recCobId & CO_SDO_DYN_BIT) != 0ul)  {
			sdoServer[cnt].recCobId = CO_COB_INVALID;
			sdoServer[cnt].trCobId = CO_COB_INVALID;
		}

		(void) icoCobSet(sdoServer[cnt].recCob, sdoServer[cnt].recCobId,
				CO_COB_RTR_NONE, 8u);
		(void) icoCobSet(sdoServer[cnt].trCob, sdoServer[cnt].trCobId,
				CO_COB_RTR_NONE, 8u);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief coSdoServerSetDefault - reset comm for sdo server
*
* \return none
*
*/
void icoSdoServerSetDefaultValue(
		void
	)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < sdoServerCnt; cnt++)  {
		if (sdoServer[cnt].sdoNr == 1u)  {
			/* calculate cobs for 1. server sdo */
			sdoServer[cnt].recCobId = 0x600ul + coNmtGetNodeId();
			sdoServer[cnt].trCobId = 0x580ul + coNmtGetNodeId();
		} else {
			/* disable sdo */
			sdoServer[cnt].recCobId |= CO_COB_INVALID;
			sdoServer[cnt].trCobId |= CO_COB_INVALID;
		}
	}
}


/***************************************************************************/
/**
* \brief coInitSdoServer - init sdo server functionality
*
* This function initializes the given sdo server.
* If the sdo number = 1,
* then the default COB-IDs are set for this SDO.
*
* \return RET_T
*/
RET_T coSdoServerInit(
		UNSIGNED8	sdoServerNr			/**< sdo server number */
	)
{
	if (sdoServerCnt >= CO_SDO_SERVER_CNT)  {
		return(RET_INVALID_PARAMETER);
	}

	sdoServer[sdoServerCnt].sdoNr = sdoServerNr;
	sdoServer[sdoServerCnt].trCob =
		icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_SDO_SERVER,
			(UNSIGNED16)sdoServerCnt);
	if (sdoServer[sdoServerCnt].trCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}
	sdoServer[sdoServerCnt].recCob =
		icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_SDO_SERVER,
			(UNSIGNED16)sdoServerCnt);
	if (sdoServer[sdoServerCnt].recCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	sdoServerCnt++;

	return(RET_OK);
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
