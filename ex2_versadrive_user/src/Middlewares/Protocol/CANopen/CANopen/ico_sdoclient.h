////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_sdoclient.h
*@brief  contains sdo client related fnc
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
#ifndef ICO_SDO_CLIENT_H
#define ICO_SDO_CLIENT_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum {
	CO_SDO_CLIENT_STATE_FREE,
	CO_SDO_CLIENT_STATE_UPLOAD_INIT,
	CO_SDO_CLIENT_STATE_UPLOAD_SEGMENT,
	CO_SDO_CLIENT_STATE_DOWNLOAD_INIT,
	CO_SDO_CLIENT_STATE_DOWNLOAD_SEGMENT,
	CO_SDO_CLIENT_STATE_BLK_UL_INIT,
	CO_SDO_CLIENT_STATE_BLK_UL_BLK,
	CO_SDO_CLIENT_STATE_BLK_UL_END,
	CO_SDO_CLIENT_STATE_BLK_DL_INIT,
	CO_SDO_CLIENT_STATE_BLK_DL,
	CO_SDO_CLIENT_STATE_BLK_DL_ACQ,
	CO_SDO_CLIENT_STATE_BLK_DL_END,
	CO_SDO_CLIENT_STATE_FD_UPLOAD_INIT,
	CO_SDO_CLIENT_STATE_FD_UPLOAD_SEGMENT,
	CO_SDO_CLIENT_STATE_FD_DOWNLOAD_INIT
} CO_SDO_CLIENT_STATE_T;

typedef struct {
	UNSIGNED8		sdoNr;			/* sdo number */
	CO_SDO_CLIENT_STATE_T	state;	/* sdo state */
	UNSIGNED16		index;			/* index */
	UNSIGNED8		subIndex;		/* sub index */
	UNSIGNED8		node;			/* subindex 3 */
	COB_REFERENZ_T	trCob;
	COB_REFERENZ_T	recCob;
	UNSIGNED32		trCobId;
	UNSIGNED32		recCobId;
	UNSIGNED8		*pData;
	UNSIGNED32		size;
	UNSIGNED32		restSize;
	UNSIGNED8		toggle;
	UNSIGNED32		timeOutVal;
	CO_TIMER_T		timer;
	UNSIGNED16		numeric;
	BOOL_T			domain;			/* domain transfer */
#ifdef CO_SDO_BLOCK
	UNSIGNED8		blockSize;
	UNSIGNED8		seqNr;
	UNSIGNED8		saveData[7];
	CO_EVENT_T		blockEvent;
# ifdef CO_SDO_BLOCK_CRC
	UNSIGNED16		blockCrc;
	BOOL_T			blockCrcUsed;
# endif /* CO_SDO_BLOCK_CRC */
#endif /* CO_SDO_BLOCK */
} CO_SDO_CLIENT_T;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	icoSdoClientAbort(CO_SDO_CLIENT_T *pSdo, RET_T errorReason);
void	icoSdoClientTimeOut(void *pData);
void	icoSdoClientUserInd(const CO_SDO_CLIENT_T	*pSdo,CO_SDO_CLIENT_STATE_T	state, UNSIGNED32 result, UNSIGNED32 len	);

# ifdef CO_SDO_BLOCK
void	icoSdoClientReadBlockInit(CO_SDO_CLIENT_T *pSdo,const CO_CAN_MSG_T	*pRecData);
void	icoSdoClientReadBlock(CO_SDO_CLIENT_T	*pSdo,const CO_CAN_MSG_T	*pRecData);
void	icoSdoClientReadBlockEnd(CO_SDO_CLIENT_T *pSdo,const CO_CAN_MSG_T	*pRecData);
void	icoSdoClientWriteBlockInit(CO_SDO_CLIENT_T *pSdo,const CO_CAN_MSG_T	*pRecData);
void	icoSdoClientWriteBlockAcq(CO_SDO_CLIENT_T *pSdo,const CO_CAN_MSG_T	*pRecData);
# endif /* CO_SDO_BLOCK */
#endif /* ICO_SDO_CLIENT_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
///////////////////////////////////////////////////////////////////////////////
