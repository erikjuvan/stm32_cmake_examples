////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_sdoserver.h
*@brief  contains sdo server related fnc
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
#ifndef ICO_SDO_SERVER_H
#define ICO_SDO_SERVER_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define MAX_SAVE_DATA	4u


typedef enum {
	CO_SDO_STATE_FREE,
/*	CO_SDO_STATE_UPLOAD_INIT, */
	CO_SDO_STATE_UPLOAD_SEGMENT,
/*	CO_SDO_STATE_DOWNLOAD_INIT, */
	CO_SDO_STATE_DOWNLOAD_SEGMENT,
	CO_SDO_STATE_BLOCK_UPLOAD_INIT,
	CO_SDO_STATE_BLOCK_UPLOAD,
	CO_SDO_STATE_BLOCK_UPLOAD_RESP,
	CO_SDO_STATE_BLOCK_UPLOAD_LAST,
	CO_SDO_STATE_BLOCK_UPLOAD_END,
	CO_SDO_STATE_BLOCK_DOWNLOAD,
	CO_SDO_STATE_BLOCK_DOWNLOAD_END,
	CO_SDO_STATE_FD_UPLOAD_SEGMENT,
	CO_SDO_STATE_FD_DOWNLOAD_SEGMENT
} CO_SDO_STATE_T;

typedef struct {
	UNSIGNED8		sdoNr;			/* sdo number */
	CO_SDO_STATE_T	state;			/* sdo state */
	UNSIGNED16		index;			/* index */
	UNSIGNED8		subIndex;		/* sub index */
	UNSIGNED32		objSize;		/* object size */
	UNSIGNED32		transferedSize;	/* transfered size */
	CO_CONST CO_OBJECT_DESC_T *pObjDesc;		/* object description pointer */
	UNSIGNED8		node;			/* subindex 3 */
	UNSIGNED8		toggle;
	UNSIGNED8		saveData[MAX_SAVE_DATA];
	COB_REFERENZ_T	trCob;
	COB_REFERENZ_T	recCob;
	UNSIGNED32		trCobId;		/* cob id */
	UNSIGNED32		recCobId;		/* cob id */
# ifdef CO_SDO_BLOCK
	UNSIGNED8		seqNr;			/* sequence number */
	UNSIGNED8		blockSize;		/* max number of blocks for one transfer */
	BOOL_T			blockCrcUsed;	/* use CRC */
	UNSIGNED16		blockCrc;		/* CRC */
	UNSIGNED32		blockCrcSize;	/* size of calculated crc sum */
	CO_EVENT_T		blockEvent;		/* event structure */
	UNSIGNED8		blockSaveData[7]; /* save data for last block */
# endif /* CO_SDO_BLOCK */
#ifdef CO_EVENT_SDO_SERVER_DOMAIN_WRITE
	BOOL_T			domainTransfer;	/* object with domain transfer */
	UNSIGNED32		domainTransferedSize;	/* overall transfered size */
#endif /* CO_EVENT_SDO_SERVER_DOMAIN_WRITE */
# ifdef CO_CANFD_SDO_SUPPORTED
	UNSIGNED8		maxCanLen;		/* maximum CAN Data len */
# endif /* CO_CAN_FD_SDO_SUPPORTED */
} CO_SDO_SERVER_T;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	icoSdoDeCodeMultiplexer(CO_CONST UNSIGNED8 pMp[],CO_SDO_SERVER_T *pSdo);
RET_T	icoSdoCheckUserWriteInd(const CO_SDO_SERVER_T *pSdo);
RET_T	icoSdoCheckUserReadInd(UNSIGNED8	sdoNr, UNSIGNED16 index,UNSIGNED8 subIndex);
RET_T	icoSdoCheckUserCheckWriteInd(UNSIGNED8 sdoNr, UNSIGNED16 index,UNSIGNED8 subIndex, const UNSIGNED8 *pData);
RET_T	icoSdoDomainUserWriteInd(const CO_SDO_SERVER_T	*pSdo);
void	icoSdoServerAbort(CO_SDO_SERVER_T *pSdo, RET_T	errorReason,BOOL_T fromClient);

# ifdef CO_SDO_BLOCK
RET_T	icoSdoServerBlockReadInit(CO_SDO_SERVER_T	*pSdo,const CO_CAN_MSG_T	*pRecData);
RET_T	icoSdoServerBlockRead(CO_SDO_SERVER_T	*pSdo);
RET_T	icoSdoServerBlockReadCon(CO_SDO_SERVER_T	*pSdo,const CO_CAN_MSG_T	*pRecData);
RET_T	icoSdoServerBlockWriteInit(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T	*pRecData);
RET_T	icoSdoServerBlockWrite(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T *pRecData);
RET_T	icoSdoServerBlockWriteEnd(CO_SDO_SERVER_T *pSdo,const CO_CAN_MSG_T	*pRecData);
# endif /* CO_SDO_BLOCK */

#endif /* ICO_SDO_SERVER_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
///////////////////////////////////////////////////////////////////////////////
