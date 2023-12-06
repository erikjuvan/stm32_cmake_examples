////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     mu_safe_common.h
* @brief    Shared MU safe layer definitions.
* @author   Amadej Arnus
* @date     13.04.2022
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup MU_SAFE_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MU_SAFE_COMMON_H
#define __MU_SAFE_COMMON_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Safety application FSM states.
 */
typedef enum __attribute__((packed))
{
    eMU_SAFE_FSM_BOOT        =  0,  /**< First state after boot (never executed). */
    eMU_SAFE_FSM_POST_INIT   =  1,  /**< Post initialization routine including stopping the motor (executed only once after boot). */
    eMU_SAFE_FSM_SETUP       =  2,  /**< Hardware and software setup (executed only once after boot). */
    eMU_SAFE_FSM_NORMAL      =  3,  /**< Normal - waiting for PU requests. */
    eMU_SAFE_FSM_STANDBY     =  4,  /**< Standby mode. */
    eMU_SAFE_FSM_RESTART     =  5,  /**< Restart device. */
    eMU_SAFE_FSM_ACTIVE      =  6,  /**< Active - USER can drive the motor. */
    eMU_SAFE_FSM_FAULT       =  7,  /**< Motor in fault. */
    eMU_SAFE_FSM_STOP        =  8,  /**< Stopping the motor. */
    eMU_SAFE_FSM_TEST_MODE	 =  9,	/**< Production test mode. */
    eMU_SAFE_FSM_ERROR       = 10,  /**< Unrecoverable error occurred. */
    eMU_SAFE_FSM_POWERFAIL   = 11,  /**< Power-fail occurred. */
} mu_safe_fsm_t;

#endif // __MU_SAFE_COMMON_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
