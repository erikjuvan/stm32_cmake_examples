////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_lss.h
*@brief  contains defines for LSS services
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
#ifndef CO_LSS_H
#define CO_LSS_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
 * \brief LSS slave states
 */
typedef enum {
	LSS_STATE_WAITING,			/**< LSS state waiting */
	LSS_STATE_CONFIGURATION		/**< LSS state configuration */
} CO_LSS_STATE_T;

/**
 * \brief LSS slave services for indication functions 
 */
typedef enum {
	CO_LSS_SERVICE_STORE,		/**< LSS service indication store node id */
	CO_LSS_SERVICE_NEW_BITRATE,	/**< LSS service indication new bitrate */
	CO_LSS_SERVICE_BITRATE_OFF, /**< LSS service indication bitrate off */
	CO_LSS_SERVICE_BITRATE_SET, /**< LSS service indication set new bitrate*/
	CO_LSS_SERVICE_BITRATE_ACTIVE/**< LSS service indication bitrate active */

} CO_LSS_SERVICE_T;

/**
 * \brief LSS master services for indication functions 
 */
typedef enum {
	CO_LSS_MASTER_SERVICE_NON_CONFIG_SLAVE,	/**< LSS Master service non-config slave */
	CO_LSS_MASTER_SERVICE_SET_NODEID,	/**< LSS Master service set node id*/
	CO_LSS_MASTER_SERVICE_SET_BITRATE,	/**< LSS Master service set bitrate*/
	CO_LSS_MASTER_SERVICE_FASTSCAN,		/**< LSS Master service fastscan */
	CO_LSS_MASTER_SERVICE_STORE,		/**< LSS Master service store */
	CO_LSS_MASTER_SERVICE_INQUIRE_NODEID,/**< LSS Master service inquire node*/
	CO_LSS_MASTER_SERVICE_BITRATE_OFF,	/**< LSS Master service indication bitrate off */
	CO_LSS_MASTER_SERVICE_BITRATE_SET,	/**< LSS Master service indication set new bitrate*/
	CO_LSS_MASTER_SERVICE_BITRATE_ACTIVE,/**< LSS Master service indication bitrate active */
	CO_LSS_MASTER_SERVICE_SWITCH_SELECTIVE/**< LSS Master service switch selektive */
} CO_LSS_MASTER_SERVICE_T;


/**
 * \brief function pointer to LSS indication
 * \param service - answer for service LSS_SERVICE_xxx
 * \param bitrate - new bitrate 
 * 		1000, 500, ... 10	standard bitrates
 * 		0					autobaud
 * 		0					table specific, values in pErrCode and pErrSpec)
 * \param pErrCode - pointer to error code
 * \param pErrSpec - pointer to error spec
 * 
 * \return UNSIGNED8
 * \retval 0 - success
 * \retval 1 - store not supported
 * \retval 2 - media access error
 * \retval 255 - implementation specific (value in parameter pErr)
 */
typedef void (* CO_EVENT_LSS_T)(CO_LSS_SERVICE_T service, UNSIGNED16 bitrate,
		UNSIGNED8 *pErrCode, UNSIGNED8 *pErrSpec);	 /*lint !e960 customer specific parameter names */

/**
 * \brief function pointer to LSS master indication
 * \param service - answer for service LSS_MASTER_SERVICE_xxx
 * \param errorCode	== 65535 - timeout
 * \param errorCode == 1..255 - error code
 * \param errorCode == 0 - ok
 * \param errorSpec - error spec (if errorCode == 65365)
 * \param pIdentity == NULL - no data available
 * \param pIdentity =! NULL - pIdentity valid
 * 
 * \return void
 */
typedef void (* CO_EVENT_LSS_MASTER_T)(CO_LSS_MASTER_SERVICE_T,
		UNSIGNED16 errorCode, UNSIGNED8 errorSpec, UNSIGNED32 *pIdentity);	/*lint !e960 customer specific parameter names */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coLssInit(void);
RET_T coLssMasterInit(void);
RET_T coEventRegister_LSS(CO_EVENT_LSS_T); 
RET_T coEventRegister_LSS_MASTER(CO_EVENT_LSS_MASTER_T);
RET_T coLssIdentifyNonConfiguredSlaves(UNSIGNED16 timeOutVal,UNSIGNED16	interval);
void  coLssNonConfigSlave(void);
RET_T coLssFastScan(UNSIGNED16 timeOutVal);
RET_T coLssFastScanKnownDevice(UNSIGNED32 vendorId, UNSIGNED32 productCode,UNSIGNED32 versionNr, UNSIGNED32 serNr,	UNSIGNED16 timeOutVal);
RET_T coLssSetNodeId(UNSIGNED8 nodeId, UNSIGNED16 timeOutVal);
RET_T coLssSetBitrate(UNSIGNED16 bitRate, UNSIGNED16 timeOutVal);
RET_T coLssSetBitrateTable(UNSIGNED8 tableSelector, UNSIGNED8 tableIndex, UNSIGNED16 timeOutVal);
RET_T coLssActivateBitrate(UNSIGNED16 switchDelay);
RET_T coLssSwitchGlobal(UNSIGNED8 mode);
RET_T coLssSwitchSelective(UNSIGNED32 vendorId, UNSIGNED32 productCode,UNSIGNED32 versionNr, UNSIGNED32 serNr, UNSIGNED16 timeOutVal);
RET_T coLssStoreConfig(UNSIGNED16 timeOutVal);
RET_T coLssInquireNodeId(UNSIGNED16 timeOutVal);
void  coLssMasterDisable(void);
void  coLssMasterEnable(void);

#endif /* CO_LSS_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
