////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   codrv_error.h
*@brief  can error handling
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

#ifndef CODRV_ERROR_H
#define CODRV_ERROR_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef enum {
	Error_Offline,
	Error_Active,
	Error_Passive,
	Error_Busoff
} CAN_ERROR_STATES_T;

typedef struct {
	BOOL_T canErrorRxOverrun; 

	BOOL_T canErrorPassive;
	BOOL_T canErrorActive;
	BOOL_T canErrorBusoff;

	CAN_ERROR_STATES_T canOldState; /* last signaled state */
	CAN_ERROR_STATES_T canNewState; /* last signaled state */

} CAN_ERROR_FLAGS_T;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
CAN_ERROR_FLAGS_T * codrvCanErrorGetFlags(void);
void                codrvCanErrorInit(void);
RET_T               codrvCanErrorInformStack(void);

#endif /* CODRV_ERROR_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

