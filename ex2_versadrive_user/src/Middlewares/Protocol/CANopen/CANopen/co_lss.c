////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_lss.c
*@brief  contains LSS slave services
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
#include "gen_define.h"
#include "co_canopen.h"
#ifdef CO_LSS_SUPPORTED
#include "co_datatype.h"
#include "co_drv.h"
#include "co_timer.h"
#include "co_odaccess.h"
#include "co_lss.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_nmt.h"
#include "ico_lss.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static COB_REFERENZ_T   lssTransCob;
static COB_REFERENZ_T   lssRecCob;
static CO_LSS_STATE_T   lssState;
static UNSIGNED8    lssPendingNodeId;
static UNSIGNED8    lssNewNodeId = 0u;
static UNSIGNED16   lssPendingBitrate;
static CO_TIMER_T   lssBitrateTimer;
static CO_LSS_SERVICE_T bitrateSwitch;

#ifdef CO_EVENT_LSS
static UNSIGNED16       lssEventTableCnt = 0u;
static CO_EVENT_LSS_T   lssEventTable[CO_EVENT_LSS];
#endif /* CO_EVENT_LSS */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void lssSlaveIdentify(void);
static void lssFastScan(const UNSIGNED8 pData[]);
static void lssNodeIdReceived(UNSIGNED8 newId);
static void lssSwitschGlobal(UNSIGNED8 mode);
static void lssStoreReceived(void);
static void lssInquireNodeId(void);
static void lssBitrateReceived(UNSIGNED8 tableSelector, UNSIGNED8 tableIndex);
static void lssBitrateActivate(UNSIGNED16 switchDelay);
static void bitrateSwitchFct(void *pData);
static void lssSwitchSelective(UNSIGNED8 lssCs, const UNSIGNED8 *pData);
#ifdef CO_EVENT_LSS
static void lssEvent(CO_LSS_SERVICE_T service, UNSIGNED16 bitrate,UNSIGNED8 *pErrorCode, UNSIGNED8    *pErrorSpec);
#endif /* CO_EVENT_LSS */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////


/***************************************************************************/
/**
* \internal
*
* \brief icoSynandler - handle TIME
*
*
* \return none
*
*/
void icoLssHandler(
		const UNSIGNED8	pData[]
	)
{
	/* start service depending on lss command */
	switch (pData[0])  {
		case LSS_CS_NON_CONFIG_REMOTE_SLAVE:
			coLssNonConfigSlave();
			break;

		case LSS_CS_FAST_SCAN:
			lssFastScan(pData);
			break;

		case LSS_CS_NODE_ID:
			lssNodeIdReceived(pData[1]);
			break;

		case LSS_CS_BITRATE:
			lssBitrateReceived(pData[1], pData[2]);
			break;

		case LSS_CS_STORE:
			lssStoreReceived();
			break;

		case LSS_CS_SWITCH_GLOBAL:
			lssSwitschGlobal(pData[1]);
			break;

		case LSS_CS_INQUIRE_NODEID:
			lssInquireNodeId();
			break;

		case LSS_CS_ACTIVATE_BITRATE:
			lssBitrateActivate(pData[1]
				| (UNSIGNED16)((UNSIGNED16)pData[2] << 8u));
			break;

		case LSS_CS_SWITCH_SEL_VENDOR:
		case LSS_CS_SWITCH_SEL_PRODUCT:
		case LSS_CS_SWITCH_SEL_REVISION:
		case LSS_CS_SWITCH_SEL_SERIAL:
			lssSwitchSelective(pData[0], &pData[1]);
			break;

		default:
			/* ignore unknown commands */
			break;
	}
}


/***************************************************************************/
/**
*
* \brief coLssNonConfigSlave - request for unconfigured slaves
*
* get answer, if node-id == 255
*
* \return none
*
*/
void coLssNonConfigSlave(
	)
{
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	nodeId;

	/* if node-id=255, answer with non config slave */
	nodeId = coNmtGetNodeId();
	if (nodeId == 255u)  {
		trData.data[0] = LSS_CS_NON_CONFIG_SLAVE;
		memset(&trData.data[1], 0, (size_t)7u);

		/* transmit data */
		(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief lssSlaveIdentify - request for unconfigured slaves
*
* get answer, if node-id == 255
*
* \return none
*
*/
static void lssSlaveIdentify(
		void
	)
{
    CO_TRANS_DATA_T	trData;

	memset(&trData.data[0], 0, (size_t)8u);
	trData.data[0] = LSS_CS_IDENTITY_SLAVE;

	/* transmit data */
	(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssFastScan - fast scan indication
*
*
* \return none
*
*/
static void lssFastScan(
		const UNSIGNED8	pData[]
	)
{
    UNSIGNED32	idNr;
    UNSIGNED8	bitCheck;
    UNSIGNED8	lssSub;
    UNSIGNED8	lssNext;
    static UNSIGNED8	lssPos = 0u;
    UNSIGNED8	nodeId;
    RET_T		retVal;
    UNSIGNED8	i;
    UNSIGNED32	val, ex;

	/* only non configured nodes are allowed */
	nodeId = coNmtGetNodeId();
	if (nodeId != 255u)  {
		return;
	}

	/* not allowed in CONFIGURATION state */
	if (lssState == LSS_STATE_CONFIGURATION)  {
		return;
	}

	idNr = pData[1];
	val = pData[2];
	idNr |= (val << 8);
	val = pData[3];
	idNr |= (val << 16);
	val = pData[4];
	idNr |= (val << 24);
	bitCheck = pData[5];
	lssSub = pData[6];
	lssNext = pData[7];

	/* reset fastscan, then LSSPos = 0 (vendor id) */
	if (bitCheck == 128u)  {
		lssPos = 0u;
		lssSlaveIdentify();
		return;
	}

	if (lssPos != lssSub)  {
		/* we are no more active */
		return;
	}

	/* get object from OD */
	retVal = coOdGetObj_u32(0x1018u, lssSub + 1u, &val);
	if (retVal != RET_OK)  {
		return;
	}
	/* mask value */
	ex = val ^ idNr;
	i = 32u;
	while (i > bitCheck)  {
		i--;
		/* bit unequal, then abort */
		if ((ex & (1ul << i)) != 0ul)  {
			return;
		}
	}

	/* value are ok, send answer */
	lssSlaveIdentify();

	lssPos = lssNext;

	if ((bitCheck == 0u) && (lssNext < lssSub))  {
		/* enter configuration mode */
		lssState = LSS_STATE_CONFIGURATION;
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief lssNodeIdReceived
*
*
* \return none
*
*/
static void lssNodeIdReceived(
		UNSIGNED8	newId
	)
{
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	errCode;

	/* only in config mode */
	if (lssState != LSS_STATE_CONFIGURATION)  {
		return;
	}

	if ((newId < 1u) || ((newId > 127u) && (newId != 255u)))  {
		/* invalid node-id */
		errCode = 1u;
	} else {
		lssPendingNodeId = newId;
		lssNewNodeId = 1u;
		errCode = 0u;
	}

	memset(&trData.data[0], 0, 8u);
	trData.data[0] = LSS_CS_NODE_ID;
	trData.data[1] = errCode;
	trData.data[2] = 0u;

	/* transmit data */
	(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssBitrateReceived
*
*
* \return none
*
*/
static void lssBitrateReceived(
		UNSIGNED8	tableSelector,
		UNSIGNED8	tableIndex
	)
{
    CO_TRANS_DATA_T	trData;
    const UNSIGNED16 bitTable[10] = { 1000u, 800u, 500u, 250u, 125u, 0xffffu, 50u, 20u, 10u, 0u};
    UNSIGNED16	bitrate = 1u;
    UNSIGNED8	errCode = 0u;
    UNSIGNED8	errSpec = 0u;

	/* only in config mode */
	if (lssState != LSS_STATE_CONFIGURATION)  {
		return;
	}

	/* check bitrate for CiA301 */
	if (tableSelector == 0u)  {
		if (tableIndex > 9u)  {
			bitrate = 0xffffu;
			errCode = 1u;
		} else {
			bitrate = bitTable[tableIndex];
		}
	} else {
		bitrate = 0u;
	}

	if (bitrate != 0xffffu)  {
		errCode = tableSelector;
		errSpec = tableIndex;
#ifdef CO_EVENT_LSS
		lssEvent(CO_LSS_SERVICE_NEW_BITRATE, bitrate, &errCode, &errSpec);
#endif /* CO_EVENT_LSS */
	}

	if (errCode == 0u)  {
		/* set new bitrate */
		lssPendingBitrate = bitrate;
	}

	memset(&trData.data[0], 0, 8u);
	trData.data[0] = LSS_CS_BITRATE;
	trData.data[1] = errCode;
	trData.data[2] = errSpec;

	/* transmit data */
	(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssBitrateActivate
*
*
* \return none
*
*/
static void lssBitrateActivate(
		UNSIGNED16	switchDelay
	)
{
#ifdef CO_EVENT_LSS
    UNSIGNED8	errCode, errSpec;
#endif /* CO_EVENT_LSS */

	/* start timer for switching */
	(void)coTimerStart(&lssBitrateTimer, switchDelay * 1000ul,
			bitrateSwitchFct, NULL, CO_TIMER_ATTR_ROUNDUP_CYCLIC);	/*lint !e960 function identifier used without '&' or parenthesized parameter list */
	bitrateSwitch = CO_LSS_SERVICE_BITRATE_OFF;

#ifdef CO_EVENT_LSS
	lssEvent(CO_LSS_SERVICE_BITRATE_OFF, 0u, &errCode, &errSpec);
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

	if (bitrateSwitch == CO_LSS_SERVICE_BITRATE_OFF)  {
#ifdef CO_EVENT_LSS
		lssEvent(CO_LSS_SERVICE_BITRATE_SET, 0u, 0u, 0u);
#endif /* CO_EVENT_LSS */
		bitrateSwitch = CO_LSS_SERVICE_BITRATE_SET;
	} else {
		if (bitrateSwitch == CO_LSS_SERVICE_BITRATE_SET)  {
#ifdef CO_EVENT_LSS
			lssEvent(CO_LSS_SERVICE_BITRATE_ACTIVE, lssPendingBitrate, 0u, 0u);
#endif /* CO_EVENT_LSS */
			(void)coTimerStop(&lssBitrateTimer);
			bitrateSwitch = CO_LSS_SERVICE_BITRATE_ACTIVE;
		}
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief lssStoreReceived
*
*
* \return none
*
*/
static void lssStoreReceived(
		void
	)
{
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	errCode, errSpec;

	/* only in config mode */
	if (lssState != LSS_STATE_CONFIGURATION)  {
		return;
	}

	/* if no indication is available, return service not supported */
#ifdef CO_EVENT_LSS
	if (lssEventTableCnt == 0u)  {
		errCode = 1u;
		errSpec = 0u;
	} else {
		lssEvent(CO_LSS_SERVICE_STORE, 0u, &errCode, &errSpec);
	}
#else /* CO_EVENT_LSS */
	errCode = 1;
	errSpec = 0;
#endif /* CO_EVENT_LSS */

	memset(&trData.data[0], 0, 8u);
	trData.data[0] = LSS_CS_STORE;
	trData.data[1] = errCode;
	trData.data[2] = errSpec;

	/* transmit data */
	(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssSwitschGlobal - switsch mode global1
*
*
* \return none
*
*/
static void	lssSwitschGlobal(
		UNSIGNED8	mode
	)
{
    CO_REC_DATA_T nmtRecData;

	if (mode == LSS_MODE_WAITING)  {
		if (lssState == LSS_STATE_CONFIGURATION)  {
			lssState = LSS_STATE_WAITING;
			/* valid lss node id available ? */
			if (lssNewNodeId == 1u)  {
				/* start new reset communication */
				nmtRecData.msg.canCob.canId = 0u;
				nmtRecData.msg.len = 2u;
				nmtRecData.msg.data[0] = 130u;
				nmtRecData.msg.data[1] = 0u;
				icoNmtMsgHandler(&nmtRecData);

				lssNewNodeId = 0u;
			}
		}
	}
	if (mode == LSS_MODE_CONFIG	)  {
		if (lssState == LSS_STATE_WAITING)  {
			lssState = LSS_STATE_CONFIGURATION;
		}
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief lssInquireNodeId
*
*
* \return none
*
*/
static void lssInquireNodeId(
		void
	)
{
    CO_TRANS_DATA_T	trData;

	/* only in config mode */
	if (lssState != LSS_STATE_CONFIGURATION)  {
		return;
	}

	memset(&trData.data[0], 0, 8u);
	trData.data[0] = LSS_CS_INQUIRE_NODEID;
	trData.data[1] = coNmtGetNodeId();

	/* transmit data */
	(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);
}


/***************************************************************************/
/**
* \internal
*
* \brief lssSwitchSelective - switch selective parameter
*
*
* \return none
*
*/
static void lssSwitchSelective(
		UNSIGNED8	lssCs,		/* command specifier */
		const UNSIGNED8	*pData		/* pointer to received data */
	)
{
    static UNSIGNED32	vendor[4];
    static UNSIGNED8	nextCmd = LSS_CS_SWITCH_SEL_VENDOR;
    CO_TRANS_DATA_T	trData;
    UNSIGNED8	i;
    RET_T		retVal;
    UNSIGNED32	val = 0u;

	/* only commands in correct order are allowed */
	if (lssCs != nextCmd)  {
		nextCmd = LSS_CS_SWITCH_SEL_VENDOR;
		return;
	}

	switch (lssCs)  {
		case LSS_CS_SWITCH_SEL_VENDOR:
			memcpy(&vendor[0], pData, 4u);
			nextCmd = LSS_CS_SWITCH_SEL_PRODUCT;
			break;

		case LSS_CS_SWITCH_SEL_PRODUCT:
			memcpy(&vendor[1], pData, 4u);
			nextCmd = LSS_CS_SWITCH_SEL_REVISION;
			break;

		case LSS_CS_SWITCH_SEL_REVISION:
			memcpy(&vendor[2], pData, 4u);
			nextCmd = LSS_CS_SWITCH_SEL_SERIAL;
			break;

		case LSS_CS_SWITCH_SEL_SERIAL:
			memcpy(&vendor[3], pData, 4u);
			nextCmd = LSS_CS_SWITCH_SEL_VENDOR;

			/* now compare data with 0x1018 object */
			for (i = 0u; i < 4u; i++)  {
				/* get object from OD */
				retVal = coOdGetObj_u32(0x1018u, i + 1u, &val);
				if (retVal != RET_OK)  {
					return;
				}
				if (val != vendor[i])  {
					return;
				}
			}
			/* all parameter fit, send answer */
			memset(&trData.data[0], 0, 8u);
			trData.data[0] = LSS_CS_SWITCH_SEL_ANSWER;

			/* transmit data */
			(void) icoTransmitMessage(lssTransCob, &trData, MSG_OVERWRITE);

			/* enter configuration mode */
			lssState = LSS_STATE_CONFIGURATION;
			break;

		default:
			nextCmd = LSS_CS_SWITCH_SEL_VENDOR;
			break;
	}
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/

# ifdef CO_EVENT_LSS
/***************************************************************************/
/**
* \brief coEventRegister_LSS - register LSS event
*
* This function registers an indication function for LSS events.
*
* \return RET_T
*
*/

RET_T coEventRegister_LSS(
		CO_EVENT_LSS_T pFunction	/**< pointer to function */
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
		CO_LSS_SERVICE_T service,
		UNSIGNED16	bitrate,
		UNSIGNED8	*pErrorCode,
		UNSIGNED8	*pErrorSpec
	)
{
    UNSIGNED16	cnt;

	/* inform application */
	cnt = lssEventTableCnt;
	while (cnt > 0u)  {
		cnt--;
		/* call user indication */
		lssEventTable[cnt](service, bitrate, pErrorCode, pErrorSpec);
	}
}
# endif /* CO_EVENT_LSS */


/***************************************************************************/
/**
* \internal
*
* \brief coLssResetNodeId - reset LSS pending node id
*
* This function set the pending node id to 255 (invalid),
* so at reset communication no node id is available.
*
* Additional, the coLssNonConfigSlave() is called.
*
* \return none
*
*/
void icoLssResetNodeId(
		void
	)
{
    UNSIGNED8	nodeId;

	//EMSISO CHANGE
	lssPendingNodeId = (UNSIGNED8)CO_u8_Node_ID_E;
	//lssPendingNodeId = (UNSIGNED8)CO_NODE_ID;

	/* allow communication for LSS */
	icoLssReset(&nodeId);

	/* if node id == 255, request new node id */
	if (lssPendingNodeId == 255u)  {
		/* call for new node id */
		/* coLssNonConfigSlave(); */
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief icoLssReset - lss for reset communication
*
* \return none
*
*/
void icoLssReset(
		UNSIGNED8	*pNodeId
	)
{
	(void)icoCobSet(lssTransCob, LSS_CAN_ID_SLAVE, CO_COB_RTR_NONE, 8u);
	(void)icoCobSet(lssRecCob, LSS_CAN_ID_MASTER, CO_COB_RTR_NONE, 8u);

	*pNodeId = lssPendingNodeId;
}


/***************************************************************************/
/**
* \internal
*
* \brief coLssResetAppl() - call at reset application
*
* \return none
*
*/
void icoLssResetAppl(
	)
{
	lssPendingNodeId = (UNSIGNED8)CO_u8_Node_ID_E;
	//lssPendingNodeId = (UNSIGNED8)CO_NODE_ID;	
}


/***************************************************************************/
/**
* \brief coLssInit - init LSS functionality
*
* This function initializes the LSS functionality,
* depending on the define CO_LSS_SLAVE_SUPPORTED or CO_LSS_MASTER_SUPPORTED
* as slave or master.
*
* \return RET_T
*
*/
RET_T coLssInit(
	)
{
    UNSIGNED8	nodeId = 255u;

	lssTransCob = icoCobCreate(CO_COB_TYPE_TRANSMIT, CO_SERVICE_LSS_TRANSMIT, 0u);
	if (lssTransCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	lssRecCob = icoCobCreate(CO_COB_TYPE_RECEIVE, CO_SERVICE_LSS_RECEIVE, 0u);
	if (lssRecCob == 0xffffu)  {
		return(RET_NO_COB_AVAILABLE);
	}

	icoLssReset(&nodeId);

	return(RET_OK);
}
#endif /* CO_LSS_SUPPORTED */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

