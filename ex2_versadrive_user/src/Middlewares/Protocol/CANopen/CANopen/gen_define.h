////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      gen_define.h
*@brief     canopen stack defines - usually generated
*@author    Zdenko Mezgec
*@author    EMTAS
*@date      03.12.2020
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef GEN_DEFINE_H
#define GEN_DEFINE_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "../canopen_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Definition of numbers of CANopen services */
#define CO_NMT_MASTER	            1u
#define CO_SDO_CLIENT_CNT	        1u
#define CO_LSS_SUPPORTED	        1u
#define CO_SDO_SERVER_CNT	        1u
#define CO_SDO_CLIENT_CNT	        1u
#define CO_PDO_TRANSMIT_CNT	        4u
#define CO_PDO_RECEIVE_CNT	        4u
#define CO_MAX_MAP_ENTRIES	        8u
#define CO_TR_PDO_DYN_MAP_ENTRIES	4u
#define CO_REC_PDO_DYN_MAP_ENTRIES	4u
#define CO_HB_CONSUMER_CNT          1u
#define CO_SYNC_SUPPORTED	        1u
#define CO_EMCY_PRODUCER	        1u
#define CO_STORE_SUPPORTED	        1u
#define CO_EMCY_ERROR_HISTORY       5u
#define CO_CONFIG_LIMIT_CHECK		1u

/* Definition of number of call-back functions for each service*/
#define CO_EVENT_SDO_SERVER_READ	1u
#define CO_EVENT_SDO_SERVER_WRITE	2u
#define CO_EVENT_SDO_SERVER_CHECK_WRITE	1u
#define CO_EVENT_PDO	            2u
#define CO_EVENT_NMT	            1u
#define CO_EVENT_ERRCTRL	        1u
#define CO_EVENT_SYNC	            1u
#define CO_EVENT_SYNC_FINISHED	    1u
#define CO_EVENT_FINISHED	        1u
#define CO_EVENT_LED	            1u
#define CO_EVENT_CAN	            1u
#define CO_EVENT_SDO_CLIENT_READ	1u
#define CO_EVENT_SDO_CLIENT_WRITE	1u
#define CO_EVENT_EMCY	            1u
#define CO_EVENT_STORE	            1u

/* number of used COB objects */
#define CO_COB_CNT	                21u

/* Definition of CAN queue sizes */
#define CO_CONFIG_REC_BUFFER_CNT	20u
#define CO_CONFIG_TRANS_BUFFER_CNT	10u

#define CO_TIMER_INTERVAL	10000L

#define CO_EVENT_SDO_SERVER_DOMAIN_WRITE	1
#define CO_SDO_SERVER_DOMAIN_CNT	        10

#endif /* GEN_DEFINE_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
