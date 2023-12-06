////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sdo.h
*@brief  contains defines for sdo services
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

#ifndef CO_SDO_H
#define CO_SDO_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define CO_SDO_DYN_BIT   0x40000000ul

/** \brief function pointer to SDO server event
 * \param execute - execute or test only
 * \param sdoNr - sdo number
 * \param index - object index
 * \param subindex - object subindex
 * 
 * \return RET_T
 */
typedef	RET_T (* CO_EVENT_SDO_SERVER_T)(BOOL_T, UNSIGNED8, UNSIGNED16, UNSIGNED8); /*lint !e960 customer specific parameter names */

/** \brief function pointer to SDO server write check event
 * \param execute - execute or test only
 * \param sdoNr - sdo number
 * \param index - object index
 * \param subindex - object subindex
 * \param pData - pointer to receive buffer
 * 
 * \return RET_T
 */
typedef RET_T (* CO_EVENT_SDO_SERVER_CHECK_WRITE_T)(BOOL_T, UNSIGNED8, UNSIGNED16, UNSIGNED8, const UNSIGNED8 *); /*lint !e960 customer specific parameter names */

/** \brief function pointer to SDO server write domain event
 * \param index - object index
 * \param subindex - object subindex
 * \param domainBufSize - actual size at domain buffer
 * \param transferSize - actual transfered size
 * 
 * \return RET_T
 */
typedef RET_T (* CO_EVENT_SDO_SERVER_DOMAIN_WRITE_T)(UNSIGNED16, UNSIGNED8, UNSIGNED32, UNSIGNED32);

/** \brief function pointer to SDO client read event
 * \param sdoNr - sdo number
 * \param index - object index
 * \param subindex - object subindex
 * \param size - size of received data
 * \param result - result of transfer
 * 
 * \return void
 */
typedef	void (* CO_EVENT_SDO_CLIENT_READ_T)(UNSIGNED8, UNSIGNED16, UNSIGNED8, UNSIGNED32, UNSIGNED32); /*lint !e960 customer specific parameter names */

/** \brief function pointer to SDO client write event
 * \param sdoNr - sdo number
 * \param index - object index
 * \param subindex - object subindex
 * \param result - result of transfer
 * 
 * \return void
 */
typedef	void (* CO_EVENT_SDO_CLIENT_WRITE_T)(UNSIGNED8, UNSIGNED16, UNSIGNED8, UNSIGNED32); /*lint !e960 customer specific parameter names */


/* \brief function pointer to SDO queue indication function
 * \param pData - pointer to data
 * \param result - result of sdo transfer
 */
typedef void (* CO_SDO_QUEUE_IND_T)(void *pData, UNSIGNED32 result);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coSdoServerInit(UNSIGNED8);
RET_T coEventRegister_SDO_SERVER_READ(CO_EVENT_SDO_SERVER_T);
RET_T coEventRegister_SDO_SERVER_WRITE(CO_EVENT_SDO_SERVER_T);
RET_T coEventRegister_SDO_SERVER_CHECK_WRITE(CO_EVENT_SDO_SERVER_CHECK_WRITE_T);
RET_T coEventRegister_SDO_SERVER_DOMAIN_WRITE(CO_EVENT_SDO_SERVER_DOMAIN_WRITE_T);

RET_T coSdoClientInit(UNSIGNED8);
RET_T coSdoRead(UNSIGNED8 sdoNr, UNSIGNED16	index, UNSIGNED8 subIndex,UNSIGNED8 *pData, UNSIGNED32 dataLen, UNSIGNED16 numeric,UNSIGNED32 timeout);
RET_T coSdoWrite(UNSIGNED8 sdoNr, UNSIGNED16 index, UNSIGNED8 subIndex,UNSIGNED8 *pData, UNSIGNED32 dataLen, UNSIGNED16 numeric,UNSIGNED32 timeout);
RET_T coSdoQueueAddTransfer(BOOL_T write, UNSIGNED8	sdoNr,UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED8 *pData, UNSIGNED32 dataLen,CO_SDO_QUEUE_IND_T	pFct, void *pFctPara);

RET_T coEventRegister_SDO_CLIENT_READ(CO_EVENT_SDO_CLIENT_READ_T);
RET_T coEventRegister_SDO_CLIENT_WRITE(CO_EVENT_SDO_CLIENT_WRITE_T);

#endif /* CO_SDO_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

