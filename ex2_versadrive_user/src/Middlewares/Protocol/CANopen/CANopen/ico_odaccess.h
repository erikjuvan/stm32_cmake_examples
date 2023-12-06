////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_odaccess.h
*@brief  contains internal defines for od access
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
#ifndef ICO_OD_ACCESS_H
#define ICO_OD_ACCESS_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T	icoOdCheckObjLimits(CO_CONST CO_OBJECT_DESC_T *pDesc,CO_CONST UNSIGNED8	*pData);
RET_T	icoEventObjectChanged(CO_CONST CO_OBJECT_DESC_T *pDesc, UNSIGNED16 index,BOOL_T changed);
RET_T	icoOdGetObj(const CO_OBJECT_DESC_T *pDesc, void *pData,UNSIGNED32 offset, UNSIGNED32 maxData, BOOL_T internal);
RET_T	icoOdGetObjDomain(void *pData,UNSIGNED32 size, UNSIGNED16	index, UNSIGNED8	subIndex);
RET_T	icoOdPutObj(const CO_OBJECT_DESC_T *pDesc, CO_CONST void *pData,UNSIGNED32 offset, UNSIGNED32 maxData, BOOL_T internal, BOOL_T *pChanged);
RET_T	icoOdGetObjRecMapData(UNSIGNED16 index, UNSIGNED8 subIndex,void **pVar, UNSIGNED8	*pLen, BOOL_T *pNumeric);
RET_T	icoOdGetObjTrMapData(UNSIGNED16 index, UNSIGNED8 subIndex,CO_CONST void **pVar, UNSIGNED8	*pLen, BOOL_T *pNumeric);UNSIGNED8	icoOdGetNumberOfSubs(UNSIGNED16 index);
void	icoOdReset(UNSIGNED16 startIdx, UNSIGNED16 lastIdx);
RET_T	icoOdCheckObjAttr(UNSIGNED16 index, UNSIGNED8 subIndex,UNSIGNED16	checkAttr);
void	icoOdSetLoadParaState(BOOL_T newState);
CO_EVENT_OBJECT_CHANGED_FCT_T icoEventObjectChangedFunction(UNSIGNED16 index, UNSIGNED8	subIndex);

#endif /* IOD_ACCESS_H */
