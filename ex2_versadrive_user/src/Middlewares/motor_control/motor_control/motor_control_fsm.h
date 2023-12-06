////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      motor_control_fsm.h
*@brief     motor control state machine
*@author    Marko Rman
*@date      1.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup MOTOR_CONTROL_FSM
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MOTOR_CONTROL_FSM_H
#define __MOTOR_CONTROL_FSM_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "motor_control.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Definitions of FSM states
 */
typedef enum
{
    eMOTOR_CONTROL_FSM_STATE_UNDEF = -2, /**< Undefined state, after power up */
    eMOTOR_CONTROL_FSM_STATE_INIT  = -1, /**< Initialization state for FSM */
    eMOTOR_CONTROL_FSM_STATE_OFF   =  0, /**< State where motor control is off and bridge disabled */
    eMOTOR_CONTROL_FSM_STATE_RUN   =  1, /**< State where motor control in enabled and bridge is enabled */
    eMOTOR_CONTROL_FSM_STATE_ERROR =  2, /**< State when there is error and motor control is off and bridge disabled */
}motor_control_fsm_state_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void motor_control_fsm_init(void);
motor_control_fsm_state_t motor_control_fsm_hndl(const motor_control_data_t *const motor_control_data);
motor_control_fsm_state_t motor_control_fsm_get_state(void);
void motor_control_fsm_set_state(const motor_control_fsm_state_t state);

#endif // __MOTOR_CONTROL_FSM_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
