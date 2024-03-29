////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_emcy.h
*@brief  contains defines for emcy services
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

#ifndef ICO_EMCY_H
#define ICO_EMCY_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	icoEmcyReset(void);
void	icoEmcyProducerSetDefaultValue(void);
RET_T	icoEmcyCheckObjLimitCobid(UNSIGNED16 index, UNSIGNED8 subIndex,UNSIGNED32 canId);
RET_T	icoEmcyCheckObjLimitHist(UNSIGNED8 subIndex, UNSIGNED8	u8);
void	*icoEmcyGetObjectAddr(UNSIGNED16 idx, UNSIGNED8	subIndex);
RET_T	icoEmcyWriteReq(UNSIGNED16 emcyErrCode, UNSIGNED8 *pAddErrorBytes);
RET_T	icoEmcyObjChanged(UNSIGNED16 index, UNSIGNED8 subIndex);
void	icoEmcyConsumerHandler(const CO_REC_DATA_T *pRecData);
void	icoEmcyConsumerReset(void);
void	icoEmcyConsumerSetDefaultValue(void);

#endif /* ICO_EMCY_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

