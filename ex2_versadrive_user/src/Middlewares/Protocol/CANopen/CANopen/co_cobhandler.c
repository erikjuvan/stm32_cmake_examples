////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_cobhandler.c
*@brief  contains functions for cob handling
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
#include "co_timer.h"
#include "co_drv.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static COB_REFERENZ_T   cobRefCnt = 0u;
static CO_COB_T         cobData[CO_COB_CNT];

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_DRV_FILTER
static RET_T icoCobFilterSet(COB_REFERENZ_T cobRef);
#endif /* CO_DRV_FILTER */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_DRV_FILTER
/***************************************************************************/
/**
*
* \internal
*
* \brief icoCobFilterSet - configure filter
*
* \return RET_T
*
*/
static RET_T icoCobFilterSet(
		COB_REFERENZ_T	cobRef
	)
{
    RET_T retval = RET_OK;
    BOOL_T	enable;

		/* special case: change to TRANSMIT without RTR
	 	 * disable it temporary, call filter function, enable it again
	 	*/
	if ((cobData[cobRef].type == CO_COB_TYPE_TRANSMIT)
	 	&& (cobData[cobRef].canCob.rtr == CO_FALSE))  
	{
		/* save cob enable state */
		enable = cobData[cobRef].canCob.enabled;
		cobData[cobRef].canCob.enabled = CO_FALSE;
		/* inform driver - disable filter */
		retval = codrvCanSetFilter(&cobData[cobRef].canCob);
		/* restore enable state */
		cobData[cobRef].canCob.enabled = enable;
	} else 
		/* special case: change to Receive with RTR Request
	 	 * -> receive data frames
	 	*/

	if ((cobData[cobRef].type == CO_COB_TYPE_RECEIVE)
	 	&& (cobData[cobRef].canCob.rtr == CO_TRUE))  
	{	
		/* receive data frame */
		cobData[cobRef].canCob.rtr = CO_FALSE;
		retval = codrvCanSetFilter(&cobData[cobRef].canCob);
		cobData[cobRef].canCob.rtr = CO_TRUE; /* restore old value */

	} else
	{
		/* inform driver */
		retval = codrvCanSetFilter(&cobData[cobRef].canCob);
	}

	return retval;
}
#endif /* CO_DRV_FILTER */

/***************************************************************************/
/**
*
* \internal
*
* \brief icoCobCreate
*
* \return COB_REFERENZ_T
*
*/
COB_REFERENZ_T icoCobCreate(
		CO_COB_TYPE_T	cobType,			/* cob type */
		CO_SERVICE_T	service,			/* service type */
		UNSIGNED16		serviceNr			/* service number */
	)
{
    COB_REFERENZ_T	cobRef;

	if (cobRefCnt >= CO_COB_CNT)  {
		return(0xffffu);
	}

	cobRef = cobRefCnt;
	cobRefCnt++;

	/* fill into cob structure */
	cobData[cobRef].cobNr = cobRef + 1u;
	cobData[cobRef].type = cobType;
	cobData[cobRef].canCob.enabled = CO_FALSE;
	cobData[cobRef].canCob.canId = CO_COB_INVALID;
	cobData[cobRef].canCob.canChan = 0xFFFFu;
	cobData[cobRef].service = service;
	cobData[cobRef].serviceNr = serviceNr;
	cobData[cobRef].inhibit = 0u;

	return(cobRef);
}


/***************************************************************************/
/**
*
* \internal
*
* \brief icoCobSet - set cob data 
*
*
* \return RET_T
*
*/
RET_T icoCobSet(
		COB_REFERENZ_T	cobRef,			/* cob reference */
		UNSIGNED32		cobId,			/* cob-id */
		CO_COB_RTR_T	rtr,			/* rtr flag */
		UNSIGNED8		len				/* data len */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}

	if ((cobId & CO_COB_VALID_MASK) == CO_COB_INVALID)  {
		cobData[cobRef].canCob.enabled = CO_FALSE;
	} else {

		if (len > CO_CAN_MAX_DATA_LEN)  {
			return(RET_SDO_INVALID_VALUE);
		}
		cobData[cobRef].canCob.enabled = CO_TRUE;
	}

	cobData[cobRef].canCob.canId = cobId & CO_COB_ID_MASK;

	if ((cobId & CO_COB_29BIT_MASK) == CO_COB_29BIT)  {
		cobData[cobRef].canCob.extended = CO_TRUE;
	} else {
		cobData[cobRef].canCob.extended = CO_FALSE;
	}

	if (rtr == CO_COB_RTR_NONE)  {
		cobData[cobRef].canCob.rtr = CO_FALSE;
	} else {
		cobData[cobRef].canCob.rtr = CO_TRUE;
	}

	cobData[cobRef].len = len;

#ifdef CO_DRV_FILTER
	{
	RET_T retval;

		retval = icoCobFilterSet(cobRef);
		if (retval != RET_OK) {
			return retval;
		}
	}
#endif /* CO_DRV_FILTER */

	return(RET_OK);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobChangeType - change cob type
*
*
* \return RET_T
*
*/
RET_T icoCobChangeType(
		COB_REFERENZ_T	cobRef,			/* cob reference */
		CO_COB_TYPE_T	cobType			/* new cob type */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}

	cobData[cobRef].type = cobType;

#ifdef CO_DRV_FILTER
	{
	RET_T retval;

		retval = icoCobFilterSet(cobRef);
		if (retval != RET_OK) {
			return retval;
		}
	}
#endif /* CO_DRV_FILTER */

	return(RET_OK);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobSetInhibit - set inhibit time
*
*
* \return RET_T
*
*/
RET_T icoCobSetInhibit(
		COB_REFERENZ_T	cobRef,			/* cob reference */
		UNSIGNED16	inhibit				/* inhibit time */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}

	cobData[cobRef].inhibit = inhibit;

	/* delete old messages from inhibit list if new time = 0 */
	if (inhibit == 0u)  {
		icoQueueDeleteInhibit(cobRef);
	}

	return(RET_OK);
}


#ifdef CO_CANFD_SUPPORTED
/***************************************************************************/
/**
*
* \internal
*
* \brief icoCobSetLen - update len information for COB
*
*
* \return RET_T
*
*/
RET_T icoCobSetLen(
		COB_REFERENZ_T	cobRef,			/* cob reference */
		UNSIGNED8		len				/* data len */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}

	if (len > CO_CAN_MAX_DATA_LEN)  {
		return(RET_SDO_INVALID_VALUE);
	}

	cobData[cobRef].len = len;

	return(RET_OK);
}
#endif /* CO_CANFD_SUPPORTED */


/******************************************************************/
/**
* \internal
*
* \brief icoCobGet - get a cob rerenced by cobref
*
*
* \return cob
*
*/
UNSIGNED32 icoCobGet(
		COB_REFERENZ_T	cobRef			/* cob reference */
	)
{
    UNSIGNED32	cobId;

	if (cobRef > cobRefCnt)  {
		return(0ul);
	}

	cobId = cobData[cobRef].canCob.canId;
	if (cobData[cobRef].canCob.extended == CO_TRUE)  {
		cobId |= CO_COB_29BIT;
	}
	if (cobData[cobRef].canCob.enabled == CO_FALSE)  {
		cobId |= CO_COB_INVALID;
	}

	return(cobId);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobEnable - enable a cob
*
*
* \return RET_T
*
*/
RET_T icoCobEnable(
		COB_REFERENZ_T	cobRef			/* cob reference */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}
	cobData[cobRef].canCob.enabled = CO_TRUE;

#ifdef CO_DRV_FILTER
	{
	RET_T retval;

		retval = icoCobFilterSet(cobRef);
		if (retval != RET_OK) {
			return retval;
		}
	}
#endif /* CO_DRV_FILTER */

	return(RET_OK);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobDisable - disable a cob
*
*
* \return RET_T
*
*/
RET_T icoCobDisable(
		COB_REFERENZ_T	cobRef			/* cob reference */
	)
{
	if (cobRef > cobRefCnt)  {
		return(RET_NO_COB_AVAILABLE);
	}

	cobData[cobRef].canCob.enabled = CO_FALSE;

#ifdef CO_DRV_FILTER
	{
	RET_T retval;

		retval = icoCobFilterSet(cobRef);
		if (retval != RET_OK) {
			return retval;
		}
	}
#endif /* CO_DRV_FILTER */

	return(RET_OK);
}


/******************************************************************/
/**
* \internal
*
* \brief icoCobGetPointer - get a pointer to cob
*
*
* \return CO_COB_T
*	pointer to cob data
*/
CO_COB_T	*icoCobGetPointer(
		COB_REFERENZ_T	cobRef			/* cob reference */
	)
{
	if (cobRef > cobRefCnt)  {
		return(NULL);
	}

	return(&cobData[cobRef]);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobCheck - get cob structure for given can id
*
*
* \return RET_T
*
*/
CO_COB_T	*icoCobCheck(
		const CO_CAN_COB_T	*pCanCob		/* can cob data */
	)
{
    UNSIGNED16	cnt;

	for (cnt = 0u; cnt < cobRefCnt; cnt++)  {
		if ((pCanCob->canId == cobData[cnt].canCob.canId)
		 && (pCanCob->extended == cobData[cnt].canCob.extended)
		 && (CO_TRUE == cobData[cnt].canCob.enabled))  {
			/* receive cob or transmit_rtr */
			if ((CO_COB_TYPE_RECEIVE == cobData[cnt].type) 
			 || (  (CO_COB_TYPE_TRANSMIT == cobData[cnt].type)
			 	&& (pCanCob->rtr == CO_TRUE)
			 	&& (pCanCob->rtr == cobData[cnt].canCob.rtr)))  {
				return(&cobData[cnt]);
			}
		}
	}
	
	return(NULL);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoCobDisableAll - disable all cobs
*
*
* \return none
*
*/
void icoCobDisableAll(
	)
{
    COB_REFERENZ_T	i;

	for (i = 0u; i < cobRefCnt; i++)  {
		(void)icoCobDisable(i);
	}
}


#ifdef CO_CANFD_SUPPORTED
/******************************************************************/
/**
*
* \internal
*
* \brief icoGetCanMsgLen - return msg len from DLC
*
*
* \return message len in bytes
*
*/
UNSIGNED8 icoGetCanMsgLen(
		UNSIGNED8	dlc			/* DLC */
	)
{
    UNSIGNED8	len;

	if (dlc < 9)  {
		return(dlc);
	}

	switch (dlc)  {
		case 9:	 len = 12; break;
		case 10: len = 16; break;
		case 11: len = 20; break;
		case 12: len = 24; break;
		case 13: len = 32; break;
		case 14: len = 48; break;
		case 15: len = 64; break;
		default: break;
	}

	return(len);
}


/******************************************************************/
/**
*
* \internal
*
* \brief icoGetCanTransferLen - return used transfer len for CAN-FD message
*
*
* \return used transfer len in bytes
*
*/
UNSIGNED8 icoGetCanTransferLen(
		UNSIGNED8	len				/* number of bytes */
	)
{
    UNSIGNED8	i;
    static const UNSIGNED8	t[7] = { 12, 16, 20, 24, 32, 48, 64 };
    UNSIGNED8	transferLen = len;

	if (len > 8)  {
		i = 0;
		while (len > t[i])  {
			i++;
		}
		transferLen = t[i];
	}

	return(transferLen);
}
#endif /* CO_CANFD_SUPPORTED */


/******************************************************************/
/**
*
* \internal
*
* \brief icoCheckRestrictedCobs
*
*
* \return BOOL_T
*
*/
BOOL_T icoCheckRestrictedCobs(
		UNSIGNED32	canId,			/* new cobid */
		UNSIGNED32	exceptFirst,	/* allow cobid range from */
		UNSIGNED32	exceptLast		/* allow cobid range to */
	)
{
    typedef struct {
        UNSIGNED32	first;
        UNSIGNED32	last;
    } RESTRICTED_COBS_T;
    static const RESTRICTED_COBS_T	restrictCobs[] = {
        { 0x000u,	0x000u },
        { 0x001u,	0x07fu },
        { 0x101u,	0x180u },
        { 0x581u,	0x5ffu },
        { 0x601u,	0x67fu },
        { 0x6e0u,	0x6ffu },
        { 0x701u,	0x77fu },
        { 0x780u,	0x7ffu },
    };
    UNSIGNED8	i;

	for (i = 0u; i < (sizeof(restrictCobs) / sizeof(RESTRICTED_COBS_T)); i++)  {
		if ((canId >= restrictCobs[i].first)
		 && (canId <= restrictCobs[i].last))  {
			if ((canId < exceptFirst) || (canId > exceptLast))  {
				return(CO_TRUE);
			}
		}
	}

	return(CO_FALSE);
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
