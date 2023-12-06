////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      motor_control.h
*@brief     Header file for API to generated motor control model
*@author    Marko Rman
*@date      05.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup MOTOR_CONTROL_API
* @{ <!-- BEGIN GROUP -->
*
* Following module is part of API, which is available to the user of Motor Control Module
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MOTOR_CONTROL_H_
#define __MOTOR_CONTROL_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include "project_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Ctrl_Stim1/2 GenTarget
typedef enum
{
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_OFF = 0,    /**< Stimuli disabled */
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_UD,         /**< Stimuli set to control D axis of voltage in rotating frame */
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_UQ,         /**< Stimuli set to control Q axis of voltage in rotating frame */
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_ID,         /**< Stimuli set to control D axis of current in rotating frame */
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_IQ,         /**< Stimuli set to control Q axis of current in rotating frame */
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_IQ_REF,       /**< Stimuli set to control Torque reference value*/
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_RPM_REF,    /**< Stimuli set to control RPM reference value*/
	eMOTOR_CONTROL_CTRL_STIMx_GENTARGET_THETA,      /**< Stimuli set to control thetha angle value*/
} motor_control_ctrl_stimx_gen_target_t;

// Ctrl_Stim1/2 Override
typedef enum
{
    eMOTOR_CONTROL_CTRL_STIMx_OVERRIDE_ADD_TO_ORIGINAL = 0, /**< Stimuli signal is added to original value */
    eMOTOR_CONTROL_CTRL_STIMx_OVERRIDE_ORIGINAL             /**< Stimuli signal replaces original value */
} motor_control_ctrl_stimx_override_t;

// Ctrl_Stim1/2 Override
typedef enum
{
    eMOTOR_CONTROL_CTRL_STIMx_RECTANGLE_PULSES = 0, /**< Stimuli signal is rectangle shape */
    eMOTOR_CONTROL_CTRL_STIMx_SINE_PULSES,          /**< Stimuli signal is sinusoidal shape */
} motor_control_ctrl_stimx_sineorrect_t;


// Ctrl_RotorAngle Reverse
typedef enum
{
    eMOTOR_CONTROL_CTRL_ROTORANGLE_REVERSE_OFF = 0, /**< Rotor angle value is normal */
    eMOTOR_CONTROL_CTRL_ROTORANGLE_REVERSE_ON       /**< Rotor angle value is reversed */
} motor_control_ctrl_rotorangle_reverse_t;

// Ctrl_AngleGenerator Enable
typedef enum
{
    eMOTOR_CONTROL_CTRL_ANGLEGENERATOR_OFF = 0, /**< Disable angle generator */
    eMOTOR_CONTROL_CTRL_ANGLEGENERATOR_ON       /**< Enable angle generator */
} motor_control_ctrl_anglegenerator_enable_t;


/*
 * Motor Control Status
 */
typedef enum
{
    eMOTOR_CONTROL_OK   = 0,            /**< Initialization passed OK */
    eMOTOR_CONTROL_PERIPH_INIT_ERROR,   /**< Error in initialization of peripheral modules */
    eMOTOR_CONTROL_MODULE_INIT_ERROR,   /**< Error in initialization of modules */
}motor_control_status_t;

/**
 * Power limit states
 */
typedef enum
{
    eMC_POWER_LIMIT_STATE_INIT                = 0x00,
    eMC_POWER_LIMIT_STATE_NORMAL              = 0x01,
    eMC_POWER_LIMIT_STATE_TIME_REDUCED        = 0x02,
    eMC_POWER_LIMIT_STATE_TEMPERATURE_REDUCED = 0x03,
}motor_control_power_limit_state;

/**
 * Structure for duty cycles of motor phases
 */
typedef struct
{
    float32_t phase_u;  /**< Duty cycle of motor phase U <0-1>*/
    float32_t phase_v;  /**< Duty cycle of motor phase V <0-1>*/
    float32_t phase_w;  /**< Duty cycle of motor phase W <0-1>*/
}motor_control_duty_t;

/**
 * Definition of motor control callback
 */
typedef void (*motor_control_callback_t)(void);

/**
 * Structure for current values
 */
typedef struct
{
    float32_t phase_u;  /**< Current on phase U */
    float32_t phase_v;  /**< Current on phase V */
    float32_t phase_w;  /**< Current on phase W */
}motor_control_current_t;

/**
 * Structure for temperature values
 */
typedef struct
{
    float32_t bridge;
    float32_t motor;
}motor_control_temperature_t;

/**
 * Structure for motor control data
 */
typedef struct
{
    motor_control_current_t     current;        /**< Actual currents */
    motor_control_current_t     current_offset; /**< Current offsets */
    float32_t                   dc_link;        /**< DC Link voltage */
    float32_t                   angle;          /**< Current stator angle */
    motor_control_temperature_t temperature;    /**< Actual temperatures */
    motor_control_duty_t        duty;           /**< Actualy PWM duty cycles */
    motor_control_status_t      status;         /**< Current status */
}motor_control_data_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
motor_control_status_t motor_control_init(void);
void motor_control_background_hndl(void);

void motor_control_enable(void);
void motor_control_disable(void);

void motor_control_set_rpm(float32_t speed);
void motor_control_set_ramp(float32_t ramp);
float32_t motor_control_get_ramp(void);
void motor_control_set_current_offset(const motor_control_current_t *const current_offsets);
void motor_control_get_current_offset(motor_control_current_t *const current_offsets);
void motor_control_set_measurement_turns(float32_t turns);
void motor_control_set_measurement_start(bool measure);
void motor_control_set_measurement_loadRPM_amplitude(float loadRPM_amplitude);

float32_t motor_control_get_frequency(void);
int32_t   motor_control_get_pole_pairs(void);
int32_t   motor_control_get_velocity_downsample(void);

float32_t motor_control_get_phase_voltage(void);
float32_t motor_control_get_phase_current(void);
float32_t motor_control_get_motor_power(void);
float32_t motor_control_get_motor_RPM(void);
float32_t motor_control_get_udc(void);
int32_t   motor_control_get_sensorless_state(void);
int32_t   motor_control_get_run_state(void);
float32_t motor_control_get_motor_temperature(void);
motor_control_power_limit_state motor_control_get_power_limit_state(void);

float32_t 	motor_control_get_dc_torque(void);
float32_t 	motor_control_get_harmonic_torque(void);
float32_t   motor_control_get_overload(void);
float32_t   motor_control_get_friction();
float32_t   motor_control_get_auball();

float32_t 	motor_control_get_modulation_factor(void);
bool        motor_control_is_measurement_done(void);

void motor_control_register_callback(motor_control_callback_t callback);

void motor_control_break_enable(const bool enable);
bool motor_control_is_break_enable(void);

#endif /* __MOTOR_CONTROL_H_ */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
