////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_store.c
*@brief  contains store/restore functionality
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
#include "gen_define.h"
#include "co_datatype.h"
#include "co_store.h"
#include "co_odaccess.h"
#include "ico_odaccess.h"
#include "ico_store.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_EVENT_STORE
static CO_EVENT_STORE_T	storeLoadFunction = NULL;
# ifdef CO_STORE_SUPPORTED
static CO_EVENT_STORE_T	storeSaveFunction  = NULL;
static CO_EVENT_STORE_T	storeClearFunction = NULL;
# endif /* CO_STORE_SUPPORTED */
#endif /* CO_EVENT_STORE */

#ifdef CO_STORE_SUPPORTED
static UNSIGNED32	storeObj;
static UNSIGNED32	restoreObj;
#endif /* CO_STORE_SUPPORTED */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \internal
*
* \brief void icoStoreLoadReq - load parameter requested
*
*
* \return none
*
*/
RET_T icoStoreLoadReq(
		UNSIGNED8	subIndex
	)
{
    RET_T	retVal = RET_OK;

	(void)subIndex; /* avoid unused warning */

#ifdef CO_EVENT_STORE
	if (storeLoadFunction != NULL)  {
		/* inform od functions */
		icoOdSetLoadParaState(CO_TRUE);

		/* call load parameter indication */
		retVal = storeLoadFunction(subIndex);

		/* inform od functions */
		icoOdSetLoadParaState(CO_FALSE);
	}
#endif /* CO_EVENT_STORE */

	return(retVal);
}


#ifdef CO_EVENT_STORE
/***************************************************************************/
/**
* \brief coEventRegister_LOAD_PARA - register LOAD_PARA event
*
* register indication function for LOAD_PARA events
*
* \return RET_T
*
*/

RET_T coEventRegister_LOAD_PARA(
		CO_EVENT_STORE_T pFunction	/**< pointer to function */
    )
{
	storeLoadFunction = pFunction;		/* save function pointer */

	return(RET_OK);
}


# ifdef CO_STORE_SUPPORTED
/***************************************************************************/
/**
* \brief coEventRegister_SAVE_PARA - register SAVE_PARA event
*
* register indication function for SAVE_PARA events
*
* \return RET_T
*
*/

RET_T coEventRegister_SAVE_PARA(
		CO_EVENT_STORE_T pFunction	/**< pointer to function */
    )
{
	storeSaveFunction = pFunction;		/* save function pointer */

	return(RET_OK);
}


/***************************************************************************/
/**
* \brief coEventRegister_CLEAR_PARA - register CLEAR_PARA event
*
* register indication function for CLEAR_PARA events
*
* \return RET_T
*
*/

RET_T coEventRegister_CLEAR_PARA(
		CO_EVENT_STORE_T pFunction	/**< pointer to function */
    )
{
	storeClearFunction = pFunction;		/* save function pointer */

	return(RET_OK);
}
# endif /* CO_STORE_SUPPORTED */
#endif /* CO_EVENT_STORE */


#ifdef CO_STORE_SUPPORTED
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/**
* \internal
*
* \brief icoStorGetObjectAddr - get Store object address
*
*
* \return RET_T
*
*/
void *icoStoreGetObjectAddr(
		UNSIGNED16	idx,			/* index */
		UNSIGNED8	subIndex		/* subindex */
	)
{
    void	*pAddr = NULL;
    (void)subIndex;

	if (idx == 0x1010u)  {		/* store */
		pAddr = (void *)&storeObj;
		/* store supported only on command */
		storeObj = 1u;
	} else {
		if (idx == 0x1011u)  {		/* restore */
			pAddr = (void *)&restoreObj;
			/* restore supported only on command */
			restoreObj = 1u;
		}
	}

	return(pAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoStoreCheckObjLimit_u32 - check correct signature
*
*
* \return RET_T
*
*/
RET_T icoStoreCheckObjLimit_u32(
		UNSIGNED16	index,		/* index */
		UNSIGNED32	signature	/* written signature */
	)
{
    RET_T	retVal = RET_OK;

	if (index == 0x1010u)  {
		/* check for correct signature */
		if (signature != CO_STORE_SIGNATURE_SAVE)  {
			retVal = RET_ERROR_STORE;
		}
	} else {
		if (index == 0x1011u)  {
			/* check for correct signature */
			if (signature != CO_STORE_SIGNATURE_LOAD)  {
				retVal = RET_ERROR_STORE;
			}
		}
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoStoreObChanged - object was changed
*
*
* \return RET_T
*
*/
RET_T icoStoreObjChanged(
		UNSIGNED16	index,
		UNSIGNED8	subIndex
	)
{
    RET_T	retVal = RET_OK;

	if (index == 0x1010u)  {		/* store */
		if (storeSaveFunction != NULL)  {
			/* call save parameter indication */
			retVal = storeSaveFunction(subIndex);
		} else {
			/* inform about not saved */
			retVal = RET_HARDWARE_ERROR;
		}
	} else {
		if (index == 0x1011u)  {		/* restore */
			if (storeClearFunction != NULL)  {
				/* call Clear parameter indication */
				retVal = storeClearFunction(subIndex);
			} else {
				/* inform about not restored */
				retVal = RET_HARDWARE_ERROR;
			}
		}
	}

	/* use similar error message */
	if (retVal != RET_OK)  {
		retVal = RET_HARDWARE_ERROR;
	}

	return(retVal);
}

#endif /* CO_STORE_SUPPORTED */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
