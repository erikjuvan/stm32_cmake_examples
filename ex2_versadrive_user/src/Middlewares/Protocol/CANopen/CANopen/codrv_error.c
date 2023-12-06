////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   codrv_error.c
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

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stddef.h>
#include <stdio.h>
#include "gen_define.h"
#include "co_datatype.h"
#include "co_drv.h"
#include "codrv_error.h"
#include "ico_commtask.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static CAN_ERROR_FLAGS_T canErrorFlags;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \brief codrvCanErrorgetFlags - Reference to the error flags
*
* \retval
*	pointer to error flags
*
*/

CAN_ERROR_FLAGS_T * codrvCanErrorGetFlags(void)
{
	return &canErrorFlags;
}


/***************************************************************************/
/**
* \brief codrvCanErrorInit - init Error variables
*
*/

void codrvCanErrorInit(void)
{
    CAN_ERROR_FLAGS_T * pError;

	pError = codrvCanErrorGetFlags();

	pError->canErrorRxOverrun = CO_FALSE; 

	pError->canErrorPassive = CO_FALSE;
	pError->canErrorActive = CO_FALSE;
	pError->canErrorBusoff = CO_FALSE;

	pError->canOldState = Error_Offline; /* last signaled state */

}

/***********************************************************************/
/**
* codrvCanErrorInformStack - inform the stack about changes
*
* Call outside of interrupts!
* Typical call in codrvCanDriverHandler().
*
*/

RET_T codrvCanErrorInformStack(void) 
{
    CAN_ERROR_FLAGS_T * pError;

	pError = codrvCanErrorGetFlags();

	if (pError->canErrorRxOverrun == CO_TRUE) {		
		pError->canErrorRxOverrun = CO_FALSE;
		/* printf("RX Overrun\n");*/
		coCommStateEvent(CO_COMM_STATE_EVENT_CAN_OVERRUN);
	}

	/* signal CAN Error chanhed events 
	* 	(Active <-> Passive -> Busoff -> Active)
	*/

	if ((pError->canNewState == Error_Busoff) 
			|| (pError->canErrorBusoff == CO_TRUE)) 
	{
		if (pError->canOldState == Error_Active) {
			/* melde Passive */
			/* printf("Passive\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_PASSIVE);
		}
		if (pError->canOldState != Error_Busoff) {
			/* melde Busoff */
			/* printf("Busoff\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_BUS_OFF);
		}
	}

	if ((pError->canOldState == Error_Busoff) 
		|| (pError->canErrorBusoff == CO_TRUE))
	{
		if (pError->canNewState == Error_Passive) {
			/* melde Active */
			/* printf("Active\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_ACTIVE);
			/* melde Passive */
			/* printf("Passive\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_PASSIVE);
		}

	}


	if ((pError->canOldState == Error_Active) 
			&& (pError->canNewState == Error_Passive)
			&& (pError->canErrorBusoff == CO_FALSE))
	{
		/* melde Passive */
		/* printf("Passive\n"); */
		coCommStateEvent(CO_COMM_STATE_EVENT_PASSIVE);
	}


	if ((pError->canOldState == Error_Active) 
		&& (pError->canNewState == Error_Active))
	{
		if (pError->canErrorBusoff == CO_TRUE) {
			/* melde Active */
			/* printf("Active\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_ACTIVE);
		} else
		if (pError->canErrorPassive == CO_TRUE) {
			/* melde Passive */
			/* printf("Passive\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_PASSIVE);
			/* melde Active */
			/* printf("Active\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_ACTIVE);
		} else {
			/* nothing */
		}
	}

	if (pError->canOldState != Error_Active) 
	{
		if (pError->canNewState == Error_Active) {
			/* melde Active */
			/* printf("Active\n"); */
			coCommStateEvent(CO_COMM_STATE_EVENT_ACTIVE);
		}

	}

	pError->canErrorPassive = CO_FALSE;
	pError->canErrorActive = CO_FALSE;
	pError->canErrorBusoff = CO_FALSE;
	pError->canOldState = pError->canNewState;

	return RET_OK;
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

