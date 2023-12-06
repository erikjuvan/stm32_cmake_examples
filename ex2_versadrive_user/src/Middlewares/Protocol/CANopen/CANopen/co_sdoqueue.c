////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sdoqueue.c
*@brief  SDO handling with queuing
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
#include <stdio.h>
#include <string.h>
#include "gen_define.h"
#ifdef CO_SDO_QUEUE
#include <co_commtask.h>
#include <co_drv.h>
#include <co_timer.h>
#include <co_sdo.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef enum {
    CO_SDO_QUEUE_STATE_NEW,
    CO_SDO_QUEUE_STATE_TRANSMITTING,
    CO_SDO_QUEUE_STATE_FINISHED
} CO_SDO_QUEUE_STATE_T;


CO_SDO_QUEUE_IND_T  pFct;       /* pointer to function */
/* SDO queue entry */
typedef struct {
    BOOL_T      write;              /* read/write */
    UNSIGNED8   sdoNr;              /* sdo number */
    UNSIGNED16  index;              /* index */
    UNSIGNED8   subIndex;           /* subindex */
    UNSIGNED8   data[4];            /* data (saved for write access) */
    UNSIGNED8   *pData;             /* pointer of data */
    UNSIGNED32  dataLen;            /* datalen */
    CO_SDO_QUEUE_IND_T  pFct;       /* pointer to function */
    void        *pFctPara;          /* function parameter */
    CO_SDO_QUEUE_STATE_T state;     /* internal state */
} EB_SDO_QUEUE_T;

/* function pointer to internal SDO indication
 */
typedef void (* CO_SDO_QUEUE_IND_T)(void *pData, UNSIGNED32 result);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static EB_SDO_QUEUE_T   sdoBuf[CO_SDO_QUEUE_LEN];
static UNSIGNED16       rdIdx = 0;
static UNSIGNED16       wrIdx = 0;
static UNSIGNED32       timeOut = 1000;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void startSdoTransfer(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
*
* \brief coSdoQueueAddTransfer - add sdo transfer to sdo queue handler
*
* This function can be used to add sdo transfers to a queue.
* If a tranfer was finished, the next will start automatically.
* After each transfer, the given function with the parameter are called.
*
* Please note: Only allowed for expedited transfers with initialized sdo channel.
* Transmit data are saved internally.
*
* \return
*	EB_RET_T
*/
RET_T coSdoQueueAddTransfer(
		BOOL_T		write,				/* write/read access */
		UNSIGNED8	sdoNr,				/* sdo number */
		UNSIGNED16	index,				/* index */
		UNSIGNED8	subIndex,			/* subIndex */
		UNSIGNED8	*pData,				/* pointer to transfer data */
		UNSIGNED32	dataLen,			/* len of transfer data */
		CO_SDO_QUEUE_IND_T	pFct,		/* pointer to finish function */
		void		*pFctPara			/* pointer to data field for finish function */
	)
{
    UNSIGNED16	idx;

	if ((sdoNr < 1) || (sdoNr > 128) || (dataLen > 4))  {
		return(RET_INVALID_PARAMETER);
	}

	idx = wrIdx + 1;
	if (idx == CO_SDO_QUEUE_LEN)  {
		idx = 0;
	}
	if (idx == rdIdx)  {
		return(RET_OUT_OF_MEMORY);
	}

	sdoBuf[wrIdx].write = write;
	sdoBuf[wrIdx].sdoNr = sdoNr;
	sdoBuf[wrIdx].index = index;
	sdoBuf[wrIdx].subIndex = subIndex;
	sdoBuf[wrIdx].pData =  pData;
	memcpy(&sdoBuf[wrIdx].data[0], pData, dataLen);
	sdoBuf[wrIdx].dataLen = dataLen;
	sdoBuf[wrIdx].pFct = pFct;
	sdoBuf[wrIdx].pFctPara = pFctPara;
	sdoBuf[wrIdx].state = CO_SDO_QUEUE_STATE_NEW;

	wrIdx = idx;

	/* try to start transfer */
	startSdoTransfer();

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief startSdoTransfer - try to start next sdo transfer
*
*
* \return
*	void
*/
static void startSdoTransfer(void)
{
    RET_T	retVal;

	/* data to transmit ? */
	if (rdIdx != wrIdx)  {
		/* buffer waiting ? */
		if (sdoBuf[rdIdx].state == CO_SDO_QUEUE_STATE_NEW)  {
			if (sdoBuf[rdIdx].write == CO_TRUE)  {
				/* write */
				retVal = coSdoWrite(sdoBuf[rdIdx].sdoNr,
					sdoBuf[rdIdx].index, sdoBuf[rdIdx].subIndex,
					&sdoBuf[rdIdx].data[0], sdoBuf[rdIdx].dataLen, 1, timeOut);
			} else {
				/* read */
				/* check for internal transfers */
				retVal = coSdoRead(sdoBuf[rdIdx].sdoNr,
					sdoBuf[rdIdx].index, sdoBuf[rdIdx].subIndex,
					sdoBuf[rdIdx].pData, sdoBuf[rdIdx].dataLen, 1, timeOut);
			}
			if (retVal != RET_OK)  {
				return;
			}
			sdoBuf[rdIdx].state = CO_SDO_QUEUE_STATE_TRANSMITTING;
		}
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief icoSdoClientQueueInd
*
*
* \return
*	void
*/
void icoSdoClientQueueInd(
		UNSIGNED8	sdoNr,			/* sdo number */
		UNSIGNED16	index,			/* index */
		UNSIGNED8	subIndex,		/* subindex */
		UNSIGNED32	result			/* result of transfer */
	)
{
	if ((sdoNr == sdoBuf[rdIdx].sdoNr) && (index == sdoBuf[rdIdx].index))  {

		if (sdoBuf[rdIdx].state == CO_SDO_QUEUE_STATE_TRANSMITTING)  {
			sdoBuf[rdIdx].state = CO_SDO_QUEUE_STATE_FINISHED;

			/* call indication */
			if (sdoBuf[rdIdx].pFct != NULL)  {
				sdoBuf[rdIdx].pFct(sdoBuf[rdIdx].pFctPara, result);
			}

			rdIdx++;
			if (rdIdx == CO_SDO_QUEUE_LEN)  {
				rdIdx = 0;
			}
		}
	}

	/* start next transfer */
	startSdoTransfer();
}

#endif /* CO_SDO_QUEUE */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
