////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_sync.h
*@brief  containcs sync related fnc
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

#ifndef ICO_SYNC_H
#define ICO_SYNC_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	*icoSyncGetObjectAddr(UNSIGNED16 index);
RET_T	icoSyncObjChanged(UNSIGNED16 valIdx);
void	icoSyncReset(void);
void	icoSyncSetDefaultValue(void);
void	icoSyncHandler(UNSIGNED8 syncCounterVal);
RET_T	icoSyncCheckObjLimit_u8(UNSIGNED16 index, UNSIGNED8 value);
RET_T	icoSyncCheckObjLimit_u32(UNSIGNED16 index, UNSIGNED32 value);

#endif /* ICO_SYNC_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
