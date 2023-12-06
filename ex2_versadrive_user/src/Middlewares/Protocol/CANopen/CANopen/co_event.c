////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_event.c
*@brief  contains event routines
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
#include <string.h>
#include "co_datatype.h"
#include "ico_commtask.h"
#include "ico_event.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static CO_EVENT_T   *pCoEvent = NULL;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \brief coEventStart - start a event
*
* This function add an event at end of the event list
*
* \return
*	RET_T
*/
RET_T icoEventStart(
		CO_EVENT_T	*pEvent,			/**< pointer to eventstruct */
		void		ptrToFct(void *para),/**< function for event */
		void		*pData				/**< pointer for own data */
	)
{
    CO_EVENT_T	*pT;

	pEvent->pFct = ptrToFct;
	pEvent->pData = pData;

	/* first event ? */
	if (pCoEvent == NULL)  {
		pCoEvent = pEvent;
	} else {
		/* add at end of list */
		pT = pCoEvent;
		while (pT->pNext != NULL) {
			pT = pT->pNext;
		}
		pT->pNext = pEvent;
	}

	pEvent->pNext = NULL;
	coCommTaskSet(CO_COMMTASK_EVENT_NEW_EVENT);

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief coEventIsActive - check if event is active
*
* With this function can be ckecked,
* if a event is currently in the event list.
*
* \return BOOL_T
* \retval CO_TRUE
*	event is active
* \retval CO_FALSE
*	event is not active
*
*/
BOOL_T icoEventIsActive(
		CO_CONST CO_EVENT_T	*pEvent			/**< pointer to event struct */
	)
{
    CO_EVENT_T	*pT = pCoEvent;

	while (pT != NULL)  {
		if (pT == pEvent)  {
			return(CO_TRUE);
		}
		pT = pT->pNext;
	}

	return(CO_FALSE);
}



/***************************************************************************/
/**
* \internal
*
* \brief icoEventCheck - check next event
*
* \return none
*
*/
void icoEventCheck(
		void
	)
{
    CO_EVENT_T	*pAct = NULL;
    CO_EVENT_T	*pEv = pCoEvent;

	pCoEvent = NULL;

	while (pEv != NULL)  {
		pAct = pEv;

		/* remove event from list */
		pEv = pAct->pNext;
		pAct->pNext = NULL;

		/* call event functions */
		if (pAct->pFct != NULL)  {
			pAct->pFct(pAct->pData);
		}
	}
}


/***************************************************************************/
/**
* \brief icoEventInit - init event interval
*
* This function initializes the internal event handling.
*
* \return none
*
*/
void icoEventInit(
		void
	)
{
	pCoEvent = NULL;
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
