////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_lssmaster.c
*@brief  contains LSS master services
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
#include "gen_define.h" /*lint !e766 depends on configuration for CO_LSS_MASTER_SUPPORTED */
#include <stdlib.h>
#include <string.h>
#include "co_datatype.h"

#ifdef CO_LSS_MASTER_SUPPORTED
#include <co_nmt.h>
#include <co_drv.h>
#include <co_timer.h>
#include <co_lss.h>
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_lss.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
typedef struct  {
    UNSIGNED32  idNr;           /* complete id number */
    UNSIGNED8   bitCheck;       /* number of valid bits */
} FASTSCAN_DATA_T;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static COB_REFERENZ_T   lssTransCob;
static COB_REFERENZ_T   lssRecCob;
static CO_TIMER_T       lssTimer;
static CO_TIMER_T       lssNonConfigSlavesTimer;
static CO_TIMER_T       lssBitrateTimer;
static UNSIGNED8        lssCmd = 0;
static FASTSCAN_DATA_T  vendor;
static FASTSCAN_DATA_T  product;
static FASTSCAN_DATA_T  version;
static FASTSCAN_DATA_T  serial;
static UNSIGNED8        lssSub = 0;
static UNSIGNED16       fsTimeOut;
static UNSIGNED8        completeAgain = 0;
static UNSIGNED8        identifyBitVal = 1;
static CO_LSS_MASTER_SERVICE_T  bitrateSwitch;
#ifdef CO_EVENT_LSS
    static UNSIGNED16               lssEventTableCnt = 0u;
    static CO_EVENT_LSS_MASTER_T    lssEventTable[CO_EVENT_LSS];
#endif /* CO_EVENT_LSS */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static RET_T lssStartCommand(const CO_TRANS_DATA_T *pData,void timeOutFct(void *p), UNSIGNED16 timeOutVal);
static void lssMasterTimeout(void *pData);
static void lssMasterFastScanTimeout(void *pData);
static void answerRemoteSlave(void);
static void answerIdentifySlave(void);
static void answerFastScan(UNSIGNED8 bitval);
static void answerNodeId(UNSIGNED8 errorCode, UNSIGNED8 errorSpec);
static void answerBitrate(UNSIGNED8 errorCode, UNSIGNED8 errorSpec);
static void lssEvet(CO_LSS_MASTER_SERVICE_T service, UNSIGNED16 errorCode,UNSIGNED8 errorSpec);
static RET_T lssNonConfiguredSlaves(UNSIGNED16  timeOutVal);
static void nonConfiguredSlaves(void *pData);
static void answerStore(UNSIGNED8 errorCode, UNSIGNED8 errorSpec);
static void answerInquireNodeId(UNSIGNED8 nodeId);
static void bitrateSwitchFct(void *pData);
static void answerSwitchSelective(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \internal
*
* \brief icoLssMasterHandler - lss master handler
*
*
* \return none
*
*/
void icoLssMasterHandler(const UNSIGNED8	pData[])
{
	/* start service depending on lss command */
	switch (pData[0])  {
		case LSS_CS_NON_CONFIG_SLAVE:
			answerRemoteSlave();
			break;

		case LSS_CS_IDENTITY_SLAVE:
			answerIdentifySlave();
			break;

		case LSS_CS_NODE_ID:
			answerNodeId(pData[1], pData[2]);
			break;

		case LSS_CS_BITRATE:
			answerBitrate(pData[1], pData[2]);
			break;

		case LSS_CS_STORE:
			answerStore(pData[1], pData[2]);
			break;

		case LSS_CS_INQUIRE_NODEID:
			answerInquireNodeId(pData[1]);
			break;

		case LSS_CS_SWITCH_SEL_ANSWER:
			answerSwitchSelective();
			break;

		default:
			/* ignore unknown commands */
			break;
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief answerRemoteSlave - answer for non configured slaves
*
*
* \return none
*
*/
static void answerRemoteSlave(void)
{
	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

#ifdef CO_EVENT_LSS
	lssEvent(CO_LSS_MASTER_SERVICE_NON_CONFIG_SLAVE, 0, 0);
#endif /* CO_EVENT_LSS */
}


/***************************************************************************/
/**
* \internal
*
* \brief answerIdentifySlave - identify slave received
*
*
* \return none
*
*/
static void answerIdentifySlave(void)
{
	if (lssCmd == LSS_CS_FAST_SCAN)  {
		identifyBitVal = 0;
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief answerNodeId - answer for node id request
*
*
* \return none
*
*/
static void answerNodeId(
		UNSIGNED8	errorCode,
		UNSIGNED8	errorSpec
	)
{
	if (lssCmd != LSS_CS_NODE_ID)  {
		return;
	}

	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

	/* call user indication */
	lssEvent(CO_LSS_MASTER_SERVICE_SET_NODEID, errorCode, errorSpec);
}


/***************************************************************************/
/**
* \internal
*
* \brief answerBitrate - answer for bitrate request
*
*
* \return none
*
*/
static void answerBitrate(
		UNSIGNED8	errorCode,
		UNSIGNED8	errorSpec
	)
{
	if (lssCmd != LSS_CS_BITRATE)  {
		return;
	}

	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

	/* call user indication */
	lssEvent(CO_LSS_MASTER_SERVICE_SET_BITRATE, errorCode, errorSpec);
}


/***************************************************************************/
/**
* \internal
*
* \brief answerStore - answer for store request
*
*
* \return none
*
*/
static void answerStore(
		UNSIGNED8	errorCode,
		UNSIGNED8	errorSpec
	)
{
	if (lssCmd != LSS_CS_STORE)  {
		return;
	}

	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

	/* call user indication */
	lssEvent(CO_LSS_MASTER_SERVICE_STORE, errorCode, errorSpec);
}


/***************************************************************************/
/**
* \internal
*
* \brief answerInqureNodeId
*
*
* \return none
*
*/
static void answerInquireNodeId(
		UNSIGNED8	nodeId
	)
{
	if (lssCmd != LSS_CS_INQUIRE_NODEID)  {
		return;
	}

	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

	/* call user indication */
	lssEvent(CO_LSS_MASTER_SERVICE_INQUIRE_NODEID, 0, nodeId);
}


/***************************************************************************/
/**
* \internal
*
* \brief answerInqureNodeId
*
*
* \return none
*
*/
static void answerSwitchSelective(void)
{
	if (lssCmd != LSS_CS_SWITCH_SEL_ANSWER)  {
		return;
	}

	/* stop timer */
	(void)coTimerStop(&lssTimer);

	/* reset command */
	lssCmd = 0;

	/* call user indication */
	lssEvent(CO_LSS_MASTER_SERVICE_SWITCH_SELECTIVE, 0, 0);
}


/***************************************************************************/
/**
* \brief coLssIdentifyNonConfiguredSlaves - identify unconfigured remote slaves
*
* Identify unconfigured remote slaves by sending the LSS command.
* If no slave is available, the indication function is called
* after the time is up, given by the parameter timeoutvalue.
*
* The LSS command is transmitted in a cycle of n seconds,
* given by parameter interval.
* If the parameter is 0, the LSS command is transmitted only once.
*
* \return RET_T
*
*/
RET_T coLssIdentifyNonConfiguredSlaves(
		UNSIGNED16	timeOutVal,			/**< timeout value in msec */
		UNSIGNED16	interval			/**< interval in seconds */
	)
{
    RET_T	retVal;
    static UNSIGNED16	timeOut;

	retVal = lssNonConfiguredSlaves(timeOutVal);
	if (retVal != RET_OK)   {
		return(retVal);
	}

	if (interval != 0u)  {
		timeOut = timeOutVal;
		retVal = coTimerStart(&lssNonConfigSlavesTimer, interval * 1000000ul,
			nonConfiguredSlaves, &timeOut, CO_TIMER_ATTR_ROUNDUP_CYCLIC);
	} else {
		(void)coTimerStop(&lssNonConfigSlavesTimer);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssFastScan - start fastscan 
*
* start fastscan for the given parameter
* if no node was found, the indication will be return 0
*
* \return RET_T
*
*/
RET_T coLssFastScan(
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	/* send lss reset to all nodes */
	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_FAST_SCAN;
	trData.data[5] = 128;
	retVal = lssStartCommand(&trData, lssMasterFastScanTimeout, timeOutVal);
	identifyBitVal = 1;

	lssSub = 0;
	completeAgain = 0;
	fsTimeOut = timeOutVal;
	vendor.idNr = 0;
	vendor.bitCheck = 32;
	product.idNr = 0;
	product.bitCheck = 32;
	version.idNr = 0;
	version.bitCheck = 32;
	serial.idNr = 0;
	serial.bitCheck = 32;

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssFastScanKnownDevice - start fastscan for known device
*
* start fastscan for a well known device
* if no node was found, the indication will be return 0
*
* \return RET_T
*
*/
RET_T coLssFastScanKnownDevice(
		UNSIGNED32	vendorId,			/**< vendor number */
		UNSIGNED32	productCode,		/**< product code */
		UNSIGNED32	versionNr,			/**< version number */
		UNSIGNED32	serNr,				/**< serial number */
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	/* send lss reset to all nodes */
	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_FAST_SCAN;
	trData.data[5] = 128;
	retVal = lssStartCommand(&trData, lssMasterFastScanTimeout, timeOutVal);
	identifyBitVal = 1;

	lssSub = 0;
	completeAgain = 1;
	fsTimeOut = timeOutVal;
	vendor.idNr = vendorId;
	vendor.bitCheck = 0;
	product.idNr = productCode;
	product.bitCheck = 0;
	version.idNr = versionNr;
	version.bitCheck = 0;
	serial.idNr = serNr;
	serial.bitCheck = 0;

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssNodeId - set node id for remote node
*
* Send a new node id to an remote slave
* The node has to be set before in configuration mode
*
* \return RET_T
*
*/
RET_T coLssSetNodeId(
		UNSIGNED8	nodeId,				/**< new node id */
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	/* send lss reset to all nodes */
	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_NODE_ID;
	trData.data[1] = nodeId;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssSetBitrate - set bitrate for remote nodes
*
* Send a new bitrate to an remote slaves.
* Allowed bitrates are:
* 1000, 800, 500, 250, 125, 50, 20, 10, 0 (for autobaud)
*
* The nodes have to be set before in configuration mode
*
* \return RET_T
*
*/
RET_T coLssSetBitrate(
        UNSIGNED16	bitRate,			/**< new bitrate */
        UNSIGNED16	timeOutVal			/**< timeout value in msec */
    )
{
    RET_T	retVal;
    UNSIGNED8   tableIndex = 0xff;

    switch (bitRate)  {
        case 1000:  tableIndex = 0;
            break;
        case 800:   tableIndex = 1;
            break;
        case 500:   tableIndex = 2;
            break;
        case 250:   tableIndex = 3;
            break;
        case 125:   tableIndex = 4;
            break;
        case 50:    tableIndex = 6;
            break;
        case 20:    tableIndex = 7;
            break;
        case 10:    tableIndex = 8;
            break;
        case 0:     tableIndex = 9;
            break;
        default:    tableIndex = 0xff;
            break;
    }

    if (tableIndex == 0xff)  {
        return(RET_VALUE_NOT_AVAILABLE);
    }

    retVal = coLssSetBitrateTable(0, tableIndex, timeOutVal);

    return(retVal);
}


/***************************************************************************/
/**
* \brief coLssSetBitrate - set bitrate for remote nodes
*
* Send a new bitrate to an remote slaves.
* Parameter are not checked!
* The nodes have to be set before in configuration mode
*
* \return RET_T
*
*/
RET_T coLssSetBitrateTable(
        UNSIGNED8 tableSelector,        /**< table selector */
        UNSIGNED8 tableIndex,           /**< table index */
        UNSIGNED16	timeOutVal			/**< timeout value in msec */
    )
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_BITRATE;
	trData.data[1] = tableSelector;
	trData.data[2] = tableIndex;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssActivateBitrate - activate bitratenodes
*
* Start service activate bitrate for remote and local node.
* The function transmits the command to the remote slaves
* and start the timer for bitrate switch itself.
* After the time was elapsed, the indication is called.
*
* \return RET_T
*
*/
RET_T coLssActivateBitrate(
        UNSIGNED16 switchDelay     /**< switch delay time */
    )
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_ACTIVATE_BITRATE;
	trData.data[1] = switchDelay & 0xffu;
	trData.data[2] = switchDelay >> 8;
	retVal = lssStartCommand(&trData, NULL, 0);
	lssCmd = 0;
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* start timer for own switching */
	retVal = coTimerStart(&lssBitrateTimer, switchDelay * 1000u,
			bitrateSwitchFct, NULL, CO_TIMER_ATTR_ROUNDUP_CYCLIC);
	bitrateSwitch = CO_LSS_MASTER_SERVICE_BITRATE_OFF;
	lssEvent(CO_LSS_MASTER_SERVICE_BITRATE_OFF, 0, 0);

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssStoreConfig - store configuration
*
* Send the LSS store configuration command to a slave.
*
* \return RET_T
*
*/
RET_T coLssStoreConfig(
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	/* send lss reset to all nodes */
	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_STORE;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssSwitchGlobal - send global switch command
*
* Send the global switch command - no answer expected
*
* \return RET_T
*
*/
RET_T coLssSwitchGlobal(
		UNSIGNED8	mode				/**< mode */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	if ((mode != LSS_MODE_WAITING) && (mode != LSS_MODE_CONFIG))  {
		return(RET_PARAMETER_INCOMPATIBLE);
	}

	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_SWITCH_GLOBAL;
	trData.data[1] = mode;
	retVal = lssStartCommand(&trData, NULL, 0);
	lssCmd = 0;

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssSwitchSelective - send Selective switch command
*
* Send the Selective switch command - the detected node
* should be answer and go into CONFIGURATION mode
*
* \return RET_T
*
*/
RET_T coLssSwitchSelective(
		UNSIGNED32	vendorId,			/**< vendor number */
		UNSIGNED32	productCode,		/**< product code */
		UNSIGNED32	versionNr,			/**< version number */
		UNSIGNED32	serNr,				/**< serial number */
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	memset(&trData.data[0], 0, 8);

	/* vendor id */
	trData.data[0] = LSS_CS_SWITCH_SEL_VENDOR;
	memcpy(&trData.data[1], &vendorId, 4);
	retVal = lssStartCommand(&trData, NULL, 0);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* product code */
	trData.data[0] = LSS_CS_SWITCH_SEL_PRODUCT;
	memcpy(&trData.data[1], &productCode, 4);
	lssCmd = 0;
	retVal = lssStartCommand(&trData, NULL, 0);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* revision */
	trData.data[0] = LSS_CS_SWITCH_SEL_REVISION;
	memcpy(&trData.data[1], &versionNr, 4);
	lssCmd = 0;
	retVal = lssStartCommand(&trData, NULL, 0);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* serialNr */
	trData.data[0] = LSS_CS_SWITCH_SEL_SERIAL;
	memcpy(&trData.data[1], &serNr, 4);
	lssCmd = 0;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);
	if (retVal != RET_OK)  {
		return(retVal);
	}
	lssCmd = LSS_CS_SWITCH_SEL_ANSWER;

	return(retVal);
}


/***************************************************************************/
/**
* \brief coLssInquireNodeId - inquire actual node ID
*
* Send the inquire node id command
*
* \return RET_T
*
*/
RET_T coLssInquireNodeId(
		UNSIGNED16	timeOutVal			/**< timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_INQUIRE_NODEID;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssStartCommand - start command 
*
* send command and start time out function
* If an other service is already running, returns busy
*
*
* \return none
*
*/
static RET_T lssStartCommand(
		const CO_TRANS_DATA_T	*pData,	/* pointer to transmit data */
		void timeOutFct(void *p),		/* time out function */
		UNSIGNED16	timeOutVal			/* timeout value in msec */
	)
{
    RET_T	retVal;

	if (lssCmd != 0)  {
		return(RET_SERVICE_BUSY);
	}

	lssCmd = pData->data[0];

	/* transmit data */
	retVal = icoTransmitMessage(lssTransCob, pData, 0);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	/* start timer for answer */
	if (timeOutVal != 0)  {
		retVal = coTimerStart(&lssTimer, timeOutVal * 1000u, timeOutFct, NULL,
			CO_TIMER_ATTR_ROUNDUP);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssMasterFastScanTimeout - timeout for fast scan mode
*
*
* \return none
*
*/
static void lssMasterFastScanTimeout(
		void *pData
	)
{
    (void)pData;

	lssCmd = 0;

	/* time out are only allowed, in not complete mode */
	if ((completeAgain != 0) && (identifyBitVal != 0))  {
		/* complete again test - and no answer received */
#ifdef CO_EVENT_LSS
		lssEvent(CO_LSS_MASTER_SERVICE_FASTSCAN, 0xffff, 0);
#endif /* CO_EVENT_LSS */
	} else {
		answerFastScan(identifyBitVal);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief lssMasterTimeout - timeout for lss request
*
*
* \return none
*
*/
static void lssMasterTimeout(
	void	*pData
	)
{
    typedef struct {
        UNSIGNED8	cs;
        CO_LSS_MASTER_SERVICE_T	service;
    } SERV_CS_T;
    const SERV_CS_T	servCs[] = {
        { LSS_CS_NODE_ID,			CO_LSS_MASTER_SERVICE_SET_NODEID },
        { LSS_CS_NON_CONFIG_REMOTE_SLAVE, CO_LSS_MASTER_SERVICE_NON_CONFIG_SLAVE },
        { LSS_CS_STORE,				CO_LSS_MASTER_SERVICE_STORE },
        { LSS_CS_INQUIRE_NODEID,	CO_LSS_MASTER_SERVICE_INQUIRE_NODEID },
        { LSS_CS_SWITCH_SEL_ANSWER, CO_LSS_MASTER_SERVICE_SWITCH_SELECTIVE}
    };
    CO_LSS_MASTER_SERVICE_T	service = CO_LSS_MASTER_SERVICE_INQUIRE_NODEID;
    UNSIGNED8	i;

    (void)pData;

	for (i = 0; i < (sizeof(servCs) / sizeof(SERV_CS_T)); i++)  {
		if (servCs[i].cs == lssCmd)  {
			service = servCs[i].service;
		}
	}

	/* reset command */
	lssCmd = 0;

#ifdef CO_EVENT_LSS
	lssEvent(service, 0xffff, 0);
#endif /* CO_EVENT_LSS */
}


/***************************************************************************/
/**
* \internal
*
* \brief bitrateSwitchFct - switch bitrate function
*
*
* \return none
*
*/
static void bitrateSwitchFct(
		void *pData
	)
{
    (void)pData;

	if (bitrateSwitch == CO_LSS_MASTER_SERVICE_BITRATE_OFF)  {
		lssEvent(CO_LSS_MASTER_SERVICE_BITRATE_SET, 0, 0);
		bitrateSwitch = CO_LSS_MASTER_SERVICE_BITRATE_SET;
	} else if (bitrateSwitch == CO_LSS_MASTER_SERVICE_BITRATE_SET)  {
		lssEvent(CO_LSS_MASTER_SERVICE_BITRATE_ACTIVE, 0, 0);
		(void)coTimerStop(&lssBitrateTimer);
		bitrateSwitch = CO_LSS_MASTER_SERVICE_BITRATE_ACTIVE;
	}
}

/***************************************************************************/
/**
* \internal
*
* \brief nonConfiguredSlaves - call non configured slaves request cyclic
*
*
* \return none
*
*/
static void nonConfiguredSlaves(
		void *pData
	)
{
    (void)pData;

	(void)lssNonConfiguredSlaves(*(UNSIGNED16 *)pData);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssMasterTimeout - timeout for lss request
*
*
* \return none
*
*/
static void answerFastScan(
		UNSIGNED8	bitVal
	)
{
    FASTSCAN_DATA_T	*pFastScan;
    CO_TRANS_DATA_T	trData;

	/* depending on actual lssPos */
	switch (lssSub)  {
		case 0:	pFastScan = &vendor;
			break;
		case 1:	pFastScan = &product;
			break;
		case 2:	pFastScan = &version;
			break;
		case 3:	pFastScan = &serial;
			break;
		case 4:	/* all data are detected - user indication */
#ifdef CO_EVENT_LSS
			lssEvent(CO_LSS_MASTER_SERVICE_FASTSCAN, 0, 0);
#endif /* CO_EVENT_LSS */
			return;
		default:
			return;
	}

	/* set new bitval */
	/* except for first call ... */
	if (pFastScan->bitCheck < 32)  {
		pFastScan->idNr |= (bitVal << pFastScan->bitCheck);
	}

	trData.data[0] = LSS_CS_FAST_SCAN;
	trData.data[1] = pFastScan->idNr & 0xff;
	trData.data[2] = (pFastScan->idNr >> 8) & 0xff;
	trData.data[3] = (pFastScan->idNr >> 16) & 0xff;
	trData.data[4] = (pFastScan->idNr >> 24) & 0xff;
	trData.data[6] = lssSub;
	trData.data[7] = lssSub; 

	/* next bit */
	if (pFastScan->bitCheck > 0)  {
		pFastScan->bitCheck --;

		completeAgain = 0;
	} else {
		/* bitcheck=0 second call */
		completeAgain ++;

		lssSub ++;
		if (lssSub < 4)  {
			trData.data[7] = lssSub; 
		} else {
			trData.data[7] = 0; 
		}
	}
	trData.data[5] = pFastScan->bitCheck;

	(void)lssStartCommand(&trData, lssMasterFastScanTimeout, fsTimeOut);
	identifyBitVal = 1;
}


/***************************************************************************/
/**
* \internal
*
* \brief lssNonConfiguredSlaves - non configured slaves call
*
*
* \return none
*
*/
static RET_T lssNonConfiguredSlaves(
		UNSIGNED16	timeOutVal			/* timeout value in msec */
	)
{
    CO_TRANS_DATA_T	trData;
    RET_T	retVal;

	memset(&trData.data[0], 0, 8);
	trData.data[0] = LSS_CS_NON_CONFIG_REMOTE_SLAVE;
	retVal = lssStartCommand(&trData, lssMasterTimeout, timeOutVal);

	return(retVal);
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

# ifdef CO_EVENT_LSS
/***************************************************************************/
/**
* \brief coEventRegister_LSS_MASTER - register LSS master event
*
* This function registers an indication function for LSS Master events.
*
* \return RET_T
*
*/

RET_T coEventRegister_LSS_MASTER(
		CO_EVENT_LSS_MASTER_T pFunction	/**< pointer to function */
    )
{
	if (lssEventTableCnt >= CO_EVENT_LSS) {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* set new indication function as first at the list */
	lssEventTable[lssEventTableCnt] = pFunction;
	lssEventTableCnt++;

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssEvent - call user indication
*
*
* \return RET_T
*
*/
static void lssEvent(
		CO_LSS_MASTER_SERVICE_T service,
		UNSIGNED16	errorCode,
		UNSIGNED8	errorSpec
	)
{
    UNSIGNED16	cnt;
    static UNSIGNED32	identity[4];

	/* save identity value */
	identity[0] = vendor.idNr;
	identity[1] = product.idNr;
	identity[2] = version.idNr;
	identity[3] = serial.idNr;

	/* inform application */
	cnt = lssEventTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		if (service == CO_LSS_MASTER_SERVICE_FASTSCAN)  {
			lssEventTable[cnt](service, errorCode, errorSpec, &identity[0]);
		} else {
			lssEventTable[cnt](service, errorCode, errorSpec, NULL);
		}
	}
}
# endif /* CO_EVENT_LSS */


/***************************************************************************/
/**
* \internal
*
* \brief icoLssMasterDisable - disable LSS master services
*
* \return none
*
*/
void coLssMasterDisable(
		void
	)
{
	/* stop timer */
	(void)coTimerStop(&lssNonConfigSlavesTimer);

	/* disable cobs */
	(void)icoCobDisable(lssTransCob);
	(void)icoCobDisable(lssRecCob);

	/* save state */
	/* lssMasterEnabled = CO_FALSE; */
}


/***************************************************************************/
/**
* \internal
*
* \brief icoLssMasterEnable - enable LSS master services
*
* (Re)enable LSS master services after the was disabled by coLssMasterDisable()
*
* \return none
*
*/
void coLssMasterEnable(
		void
	)
{
	icoLssMasterReset();
	/* lssMasterEnabled = CO_TRUE; */
}


/***************************************************************************/
/**
* \internal
*
* \brief coResetTime - reset comm for Time
*
* \return none
*
*/
void icoLssMasterReset(
		void
	)
{
	(void)icoCobSet(lssTransCob, LSS_CAN_ID_MASTER, CO_COB_RTR_NONE, 8);
	(void)icoCobSet(lssRecCob, LSS_CAN_ID_SLAVE, CO_COB_RTR_NONE, 8);
}


/***************************************************************************/
/**
* \brief coLssMasterInit - init LSS functionality
*
* This function initializes the LSS functionality,
* depending on the define CO_LSS_SLAVE_SUPPORTED or CO_LSS_MASTER_SUPPORTED
* as slave or master.
*
* \return RET_T
*
*/
RET_T coLssMasterInit(
	)
{
	lssTransCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_LSS_TRANSMIT, 0u);
	if (lssTransCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	lssRecCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_LSS_RECEIVE, 0u);
	if (lssRecCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	icoLssMasterReset();

	return(RET_OK);
}
#endif
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

