////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_pdo.h
*@brief  contains internal defines for PDO
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

#ifndef ICO_PDO_H
#define ICO_PDO_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	*icoTrPdoGetObjectAddr(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
void	*icoRecPdoGetObjectAddr(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
void	*icoTrMapGetObjectAddr(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
const void	*icoTrMapGetObjectAddr_R(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
void	*icoRecMapGetObjectAddr(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
const void	*icoRecMapGetObjectAddr_R(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
RET_T	icoTrPdoCheckObjLimit_u8(UNSIGNED16 pdoNr, UNSIGNED8 subIndex,UNSIGNED8 u8);
RET_T	icoRecPdoCheckObjLimit_u8(UNSIGNED16 pdoNr, UNSIGNED8 subIndex,UNSIGNED8 u8);
RET_T	icoTrPdoCheckObjLimit_u16(UNSIGNED16 pdoNr, UNSIGNED8	subIndex);
RET_T	icoRecPdoCheckObjLimit_u16(UNSIGNED16 pdoNr, UNSIGNED8	subIndex);
RET_T	icoTrPdoCheckObjLimitCobid(UNSIGNED16 pdoNr, UNSIGNED32 canId);
RET_T	icoRecPdoCheckObjLimitCobid(UNSIGNED16 pdoNr, UNSIGNED32 canId);
RET_T	icoTrPdoObjChanged(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
RET_T	icoRecPdoObjChanged(UNSIGNED16 pdoNr, UNSIGNED8 subIndex);
RET_T	icoTrMapCheckObjLimitMapTable(UNSIGNED16 pdoNr, UNSIGNED8 subIndex,UNSIGNED8 value);
RET_T	icoTrMapCheckObjLimitMapEntry(UNSIGNED16 pdoNr, UNSIGNED32 value);
RET_T	icoRecMapCheckObjLimitMapTable(UNSIGNED16 pdoNr, UNSIGNED8 subIndex,UNSIGNED8 value);
RET_T	icoRecMapCheckObjLimitMapEntry(UNSIGNED16 pdoNr, UNSIGNED32 mapEntry);
RET_T	icoTrPdoMapChanged(UNSIGNED16 pdoNr);
RET_T	icoRecPdoMapChanged(UNSIGNED16 pdoNr);
void	icoPdoReset(void);
void	icoPdoSetDefaultValue(void);
void	icoPdoOperational(void);
void	icoPdoPreOperational(void);
void	icoPdoSyncHandling(UNSIGNED8  syncCounterVal);
void	icoPdoReceiveHandler(CO_CONST CO_REC_DATA_T *pRecData);
void	icoPdoRtrHandler(CO_CONST CO_REC_DATA_T	*pRecData);

#endif /* ICO_PDO_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
