////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file  co_sync.h
*@brief  contains defines for sync services
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
#ifndef CO_SYNC_H
#define CO_SYNC_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/** \brief function pointer to SYNC indication
 * \param nothing
 * 
 * \return void
 */
typedef void (* CO_EVENT_SYNC_T)(void);


/** \brief function pointer to SYNC Finished indication
 * \param nothing
 * 
 * \return void
 */
typedef void (* CO_EVENT_SYNC_FINISHED_T)(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coSyncInit(UNSIGNED32	cobId);
RET_T coEventRegister_SYNC(void (*pFunction)(void));
RET_T coEventRegister_SYNC_FINISHED(void (*pFunction)(void));

#endif /* CO_SYNC_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////