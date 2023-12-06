////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_nmt.h
*@brief  contains defines for nmt services
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
#ifndef CO_NMT_H
#define CO_NMT_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
* error control states
*/
typedef enum {
	CO_ERRCTRL_BOOTUP,		/**< bootup */
	CO_ERRCTRL_NEW_STATE,	/**< NMT state changed */
	CO_ERRCTRL_HB_STARTED,	/**< heartbeat started */
	CO_ERRCTRL_HB_FAILED,	/**< heartbeat failed */
	CO_ERRCTRL_GUARD_FAILED	/**< Lifetime failure from master detected */
} CO_ERRCTRL_T;


/**
* NMT states 
*/
typedef enum {
	CO_NMT_STATE_UNKNOWN = 0,		/**< unknown */
	CO_NMT_STATE_OPERATIONAL = 5,	/**< OPERATIONAL */
	CO_NMT_STATE_STOPPED = 4,		/**< STOPPED */
	CO_NMT_STATE_PREOP = 127,		/**< PRE-OPERATIONAL */
	CO_NMT_STATE_RESET_NODE = 128,	/**< Reset NODE */
	CO_NMT_STATE_RESET_COMM = 129	/**< Reset Communication */
} CO_NMT_STATE_T;

/** \brief function pointer to error control event function 
 * \param nodeId - node Id
 * \param errCtrlState - error control state
 * \param nmtState - actual NMT state
 * 
 * \return void
 */
typedef void (* CO_EVENT_ERRCTRL_T)(UNSIGNED8, CO_ERRCTRL_T, CO_NMT_STATE_T); /*lint !e960 customer specific parameter names */

/** \brief function pointer to NMT event function 
 * \param execute - execute status change y/n
 * \param nmtState - new NMT state
 * 
 * \return RET_T
 * \retval RET_OK - state change allowed (only valid for OPERATIONAL)
 * \retval RET_ - state change not allowed (only valid for OPERATIONAL)
 */
typedef RET_T (* CO_EVENT_NMT_T)(BOOL_T, CO_NMT_STATE_T); /*lint !e960 customer specific parameter names */


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T       coNmtInit(UNSIGNED8);
UNSIGNED8   coNmtGetNodeId(void);
RET_T       coErrorCtrlInit(UNSIGNED16, UNSIGNED8);

RET_T coEventRegister_ERRCTRL(CO_EVENT_ERRCTRL_T pFunction);
RET_T coEventRegister_NMT(CO_EVENT_NMT_T pFunction);

CO_NMT_STATE_T coNmtGetState(void);
CO_NMT_STATE_T coNmtGetRemoteNodeState(UNSIGNED8 nodeId);
RET_T coNmtStateReq(UNSIGNED8 node, CO_NMT_STATE_T	reqState, BOOL_T master);
RET_T coNmtLocalStateReq(CO_NMT_STATE_T	reqState);

RET_T coHbConsumerSet(UNSIGNED8 node, UNSIGNED16 hbTime);

#endif /* CO_NMT_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
