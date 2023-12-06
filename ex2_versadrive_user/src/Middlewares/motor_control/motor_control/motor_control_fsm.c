////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      motor_control_fsm.c
*@brief     Module with protections of drive
*@author    Marko Rman
*@date      07.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup MOTOR_CONTROL_FSM
* @{ <!-- BEGIN GROUP -->
*
* Following group is part of motor control state machine used to enable/disable
* bridge
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "motor_control_fsm.h"
#include "protections.h"
#include "../motor_control_cfg.h"
#include "system.h"
#include "func_table.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
 * Define section to link FAST loop functions to RAM.
 * Add "select "foc_fast_code";" to linker file in coresponding RAM
 */
#define __FOC_FAST_CODE__   __attribute__((section(".ccmram")))

/**
 * Define section to link SLOW loop functions to RAM.
 * Add "select "foc_slow_code";";" to linker file in coresponding RAM
 */
#define __FOC_SLOW_CODE__   __attribute__((section ("foc_slow_code")))


/**
 * Motor Control FSM type
 */
typedef struct
{
    motor_control_fsm_state_t   current_state;  /**< Current state of motor control */
    motor_control_fsm_state_t   next_state;     /**< Next state of motor control */
    bool                        first_entry;    /**< Only true when entering the state */
    uint32_t                    duration;       /**< Duration of state in counts called ie FOC loops */
}motor_control_fsm_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = motor_control__state
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = SBYTE [-2 ... 2]
@! DESCRIPTION  = "Current state of motor control (-2 - Undefined, 1 - Initialization, 0 - Off, 1 - Run, 2 - Error)"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile uint8_t motor_control__state = -2;

/*
@! SYMBOL       = motor_control__control
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = SBYTE [0 ... 1]
@! DESCRIPTION  = "Control state of motor control (0 - Off, 1 - Run)"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! DEFAULT      = 0
@! END
*/
volatile uint8_t motor_control__control = 0;
/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

/**
 * Global data holder
 */
static motor_control_fsm_t g_motor_control_fsm = {0};


////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void motor_control_fsm_state_init(const motor_control_data_t *const motor_control_data);
static void motor_control_fsm_state_off(void);
static void motor_control_fsm_state_on(void);
static void motor_control_fsm_state_error(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *  Handler for state Undefined
 *
 *  Initialization state is used to prepare driver for operation.
 *      - Calibration of current sensors offset
 *
 *@param[in] motor_control_data - pointer to data of motor control
 */
////////////////////////////////////////////////////////////////////////////////
static /*__FOC_FAST_CODE__*/ void motor_control_fsm_state_init(const motor_control_data_t *const motor_control_data)
{
    static motor_control_current_t current_offset;
    static uint32_t current_offset_cnt;

    if( g_motor_control_fsm.first_entry )
    {
        current_offset.phase_u = 0.0f;
        current_offset.phase_v = 0.0f;
        current_offset.phase_w = 0.0f;

        current_offset_cnt = 0;
    }

    // Wait for safety to complete boot before measuring offset
    if( pg_safe_func_table->fp_Get_State() > eMU_SAFE_FSM_SETUP )
    {
        if( current_offset_cnt < MOTOR_CONTORL_CURRENT_OFFSET_SAMPLES )
        {
            current_offset.phase_u += motor_control_data->current.phase_u;
            current_offset.phase_v += motor_control_data->current.phase_v;
            current_offset.phase_w += motor_control_data->current.phase_w;

            current_offset_cnt++;
        }
        else
        {
            current_offset.phase_u = current_offset.phase_u / MOTOR_CONTORL_CURRENT_OFFSET_SAMPLES;
            current_offset.phase_v = current_offset.phase_v / MOTOR_CONTORL_CURRENT_OFFSET_SAMPLES;
            current_offset.phase_w = current_offset.phase_w / MOTOR_CONTORL_CURRENT_OFFSET_SAMPLES;

            protections_check_current_offset(&current_offset);

            motor_control_set_current_offset(&current_offset);

            g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_OFF;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Handler for state OFF
 */
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_fsm_state_off(void)
{
    if( g_motor_control_fsm.first_entry )
    {
        motor_control_break_enable(false);
    }

    // Always enter Error state if protections is activated
    if( protections_get_active() != ePROTECTIONS_NONE )
    {
        g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_ERROR;
        return;
    }

    if( system_get_init_status() != eSYSTEM_INIT_OK )
    {
        g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_OFF;
    }
    else
    {
        // Update desired state from FLOW variable
        g_motor_control_fsm.next_state = motor_control__control;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Handler for state RUN
 */
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_fsm_state_on(void)
{
    if( g_motor_control_fsm.first_entry )
    {

    }

    // Always enter Error state if protections is activated
    if( protections_get_active() != ePROTECTIONS_NONE )
    {
        g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_ERROR;
        return;
    }

    // Update desired state from FLOW variable
    g_motor_control_fsm.next_state = motor_control__control;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Handler for state ERROR
 */
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_fsm_state_error(void)
{
    if( g_motor_control_fsm.first_entry )
    {

    }

    if( motor_control__control == eMOTOR_CONTROL_FSM_STATE_OFF )
    {
        // Only switch back to off, if there is no active protection
        if( protections_get_active() == ePROTECTIONS_NONE )
        {
            g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_OFF;
        }
        // Otherwise try to get it cleared
        else
        {
            protections_clear();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Initialization of motor control FSM
 *
 *@return void
 */
////////////////////////////////////////////////////////////////////////////////
void motor_control_fsm_init(void)
{
    g_motor_control_fsm.current_state = eMOTOR_CONTROL_FSM_STATE_UNDEF;
    g_motor_control_fsm.next_state    = eMOTOR_CONTROL_FSM_STATE_INIT;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Motor control state machine handler
 *
 *@param[in] motor_control_data - pointer to data of motor control
 *@return    current state of motor control FSM
 */
////////////////////////////////////////////////////////////////////////////////
motor_control_fsm_state_t __FOC_FAST_CODE__ motor_control_fsm_hndl(const motor_control_data_t *const motor_control_data)
{
    // Update first entry and state duration
    if( g_motor_control_fsm.current_state != g_motor_control_fsm.next_state )
    {
        g_motor_control_fsm.first_entry = true;
        g_motor_control_fsm.duration = 0;
        g_motor_control_fsm.current_state = g_motor_control_fsm.next_state;
    }
    else
    {
        g_motor_control_fsm.first_entry = false;
        g_motor_control_fsm.duration++;
    }

    // Call requested state handler
    switch (g_motor_control_fsm.current_state)
    {
        case eMOTOR_CONTROL_FSM_STATE_UNDEF:    g_motor_control_fsm.next_state = eMOTOR_CONTROL_FSM_STATE_INIT; break;
        case eMOTOR_CONTROL_FSM_STATE_INIT:     motor_control_fsm_state_init(motor_control_data); break;
        case eMOTOR_CONTROL_FSM_STATE_OFF:      motor_control_fsm_state_off(); break;
        case eMOTOR_CONTROL_FSM_STATE_RUN:      motor_control_fsm_state_on();  break;
        case eMOTOR_CONTROL_FSM_STATE_ERROR:    motor_control_fsm_state_error(); break;
        default: 								// Do nothing
        	break;
    }

    // Store current state to FLOW variable
    motor_control__state = g_motor_control_fsm.current_state;

    return g_motor_control_fsm.current_state;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Sets next state for motor control FSM
 *
 *@param[in] state - desired next state of motor control FSM
 *@return    void
 */
////////////////////////////////////////////////////////////////////////////////
void __FOC_FAST_CODE__ motor_control_fsm_set_state(const motor_control_fsm_state_t state)
{
    motor_control__control = state;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Returns current state of motor control state machine
 *
 *@return    current state of motor control FSM
 */
////////////////////////////////////////////////////////////////////////////////
motor_control_fsm_state_t __FOC_FAST_CODE__ motor_control_fsm_get_state(void)
{
    return g_motor_control_fsm.current_state;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
