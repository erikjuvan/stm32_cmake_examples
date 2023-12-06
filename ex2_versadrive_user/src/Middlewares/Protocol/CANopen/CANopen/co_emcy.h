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
#ifndef CO_EMCY_H
#define CO_EMCY_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
* define for Emergency Error Code wrong PDO length 
*/
#define CO_EMCY_ERRCODE_PDO_LEN		0x8210u

/**
* define for Emergency Error Code communication error
*/
#define CO_EMCY_ERRCODE_COMM_ERROR	0x8130u


/** \brief function pointer to emergency function 
 *
 * \param errCode - emergency error code
 * \param addErrorCode - pointer to 5 bytes error code
 * 
 * \return RET_T
 * \retval RET_OK
 *	send emergency
 * \retval RET_xx
 *	don't send emergency
 */
typedef RET_T (* CO_EVENT_EMCY_T)(UNSIGNED16 errCode, UNSIGNED8 *addErrorCode);


/** \brief function pointer to emergency consumer function 
 *
 * \param node - node id of received emergency
 * \param errCode - emergency error code
 * \param errorRegister - emergency error register
 * \param addErrorCode - pointer to 5 bytes error code
 * 
 * \return none
 */
typedef void (* CO_EVENT_EMCY_CONS_T)(UNSIGNED8 node, UNSIGNED16 errCode, UNSIGNED8 errorRegister, UNSIGNED8 const *addErrorCode);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coEmcyProducerInit(void);
RET_T coEmcyConsumerInit(UNSIGNED8 emcyCnt);
RET_T coEmcyWriteReq(UNSIGNED16	emcyErrCode, CO_CONST UNSIGNED8 *pData);
RET_T coEventRegister_EMCY(CO_EVENT_EMCY_T pFunction);
RET_T coEventRegister_EMCY_CONSUMER(CO_EVENT_EMCY_CONS_T pFunction);

#endif /* CO_EMCY_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
