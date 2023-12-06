/*
 * co_direct_read.h
 *
 *  Created on: 30 Mar 2022
 *      Author: amadeja
 */

#ifndef PROTOCOL_CANOPEN_CANOPEN_CO_DIRECT_READ_H_
#define PROTOCOL_CANOPEN_CANOPEN_CO_DIRECT_READ_H_

#include "co_canopen.h"
#include "stddef.h"
#include "string.h"

#if !defined(DEBUG)
/** \brief function pointer to object changed function
 * \param index - object index
 * \param subindex - object subindex
 *
 * \return RET_T
 */
typedef RET_T (* CO_EVENT_OBJECT_CHANGED_FCT_T)(UNSIGNED16, UNSIGNED8); /*lint !e960 customer specific parameter names */
#endif

void co_direct_coOdInitOdPtr(
		const CO_OD_ASSIGN_T 				*pOdAssing,
			  UNSIGNED16 					 odCnt,
		const CO_OBJECT_DESC_T 				*pObjdesc,
			  UNSIGNED16 					 descCnt,
			  CO_EVENT_OBJECT_CHANGED_FCT_T *pEventPtr,
		const CO_OD_DATA_VARIABLES_T 		*pOdVarPointers
		);

#endif /* PROTOCOL_CANOPEN_CANOPEN_CO_DIRECT_READ_H_ */
