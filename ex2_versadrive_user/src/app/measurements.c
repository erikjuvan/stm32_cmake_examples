////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file    measurements.c
 * @note
 * @author  Marko Rman
 * @date    27.12.2021
 * @brief   Functions for handling inertia and unbalance measurementes
 */
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup MEASUREMENTS_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32g4xx_hal.h"

#include "motor_control/motor_control/motor_control.h"

#include "hisense_proto.h"
#include "measurements.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = app__measure_state
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "measurement state (0 - idle, 1 - measuring)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "/"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 11
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 0
@! END
*/
volatile uint8_t app__measure_state;

/*
@! SYMBOL       = measurement__raw_inertia
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Raw Inertia"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 1
@! END
*/
volatile float measurement__raw_inertia;

/*
@! SYMBOL       = measurement__raw_unbalance
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Raw Ubalance"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 2
@! END
*/
volatile float measurement__raw_unbalance;

/*
@! SYMBOL       = measurement__raw_friction
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Raw Friction"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 3
@! END
*/
volatile float measurement__raw_friction;

/*
@! SYMBOL       = measurement__overload
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "OVerload"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 4
@! END
*/
volatile float measurement__overload;

/*
@! SYMBOL       = measurement__inertia
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Inertia"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 5
@! END
*/
volatile float measurement__inertia;

/*
@! SYMBOL       = measurement__unbalance
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Ubalance"
@! GROUP        = measurement
@! DIMENSION    = 1 1
@! UNIT         = "g"
@! CAN_INDEX    = 0x5010
@! CAN_SUBINDEX = 6
@! END
*/
volatile float measurement__unbalance;

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

/**
 * Measurements module data holder
 */
measurements_data_t g_measurements_data = {0};

/**
 * Configurations for measurements
 */
measurement_config_t g_measurements_config[] = {
    {.type = eMEAS_CONST_LOW,  .start_rpm = 75,  .amplitude = 0,  .turns = 6},
    {.type = eMEAS_WAVE_LOW,   .start_rpm = 75,  .amplitude = 25, .turns = 6},
    {.type = eMEAS_CONST_HIGH, .start_rpm = 100, .amplitude = 0,  .turns = 8},
    {.type = eMEAS_WAVE_HIGH,  .start_rpm = 100, .amplitude = 25, .turns = 8},
};
#define MEASUREMENTS_CONFIG_SIZE (sizeof(g_measurements_config) / sizeof(measurement_config_t))

/**
 * Selected machine type
 */
static wm_config_t g_wm_config = {.type = eMACHINE_TYPE_NONE, .configured = false};

/**
 * Array with measurement coefficients for machine types
 */
// clang-format off
wm_factors_t g_wm_factors[eMACHINE_TYPE_NUM] =
{
    // NONE
    [eMACHINE_TYPE_NONE] =
    {
        .half_load_raw_inertia  = 0.0f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 0.0f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 0.0f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 0.0f, .p11 = 0.0f, .p21 = 0.0f },
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 0.0f, .p11 = 0.0f, .p21 = 0.0f },
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 0.0f, .p11 = 0.0f, .p21 = 0.0f },
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 0.0f, .p11 = 0.0f, .p21 = 0.0f },
    },

    // #1 - 838535 - K33 - 60cm, 10kg, 1600rpm
    [eMACHINE_TYPE_1_838535] =
    {
        .half_load_raw_inertia  = 0.014f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 7.638505e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 6.117468e05f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 5.351444e03f, .p11 = -1.264023e05f, .p21 = 1.689427e06f },
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 5.710488e03f, .p11 = -1.817756e05f, .p21 = 3.248253e06f },
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 5.385813e03f, .p11 = -1.332222e05f, .p21 = 2.272584e06f },
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 5.711126e03f, .p11 = -1.696037e05f, .p21 = 3.446739e06f },
    },

    // #2 - 825308 - S22 - 54cm, 8kg, 1400rpm
    [eMACHINE_TYPE_2_825308] =
    {
        .half_load_raw_inertia  = 0.014f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 6.014921e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 4.924715e05f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 4.903496e03f, .p11 = -1.404471e05f, .p21 = 2.070521e06f },
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 4.882491e03f, .p11 = -1.516354e05f, .p21 = 2.770391e06f },
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 5.067727e03f, .p11 = -1.576913e05f, .p21 = 2.915498e06f },
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 5.171150e03f, .p11 = -1.788804e05f, .p21 = 4.094562e06f },
    },

    // #6 - 825673 - S22 - 49cm, 8kg, 1400rpm
    [eMACHINE_TYPE_3_825673] =
	{
	    .half_load_raw_inertia  = 0.014f,
		.inertia_low            = { .zero_inertia   = 0.0f, .p10 = 6.950977e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 5.632919e05f },
		.unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 4.762183e03f, .p11 = -1.200441e05f, .p21 = 1.755455e06f },
		.unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 4.803157e03f, .p11 = -1.401910e05f, .p21 = 2.714967e06f },
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 4.965699e03f, .p11 = -1.471613e05f, .p21 = 2.881468e06f },
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 5.255491e03f, .p11 = -1.885659e05f, .p21 = 4.571095e06f },
	},

	//
	[eMACHINE_TYPE_4_828601] =
	{
	},

	// #5 - 600235 - K33 - 60cm, 9kg, 1600rpm
	[eMACHINE_TYPE_5_600235] =
    {
        .half_load_raw_inertia  = 0.014f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 7.672705e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 6.487780e05f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 5.642267e03f, .p11 = -1.366356e05f, .p21 = 1.789207e06f },
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 5.821097e03f, .p11 = -1.659877e05f, .p21 = 2.837041e06f },
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 5.904766e03f, .p11 = -1.587408e05f, .p21 = 3.002731e06f},
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 5.744020e03f, .p11 = -1.323157e05f, .p21 = 2.869850e06f},
    },

    // #3 - 825228 - K33 - 60cm, 9kg, 1400rpm
    [eMACHINE_TYPE_6_825228] =
	{
        .half_load_raw_inertia  = 0.014f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 8.609046e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 6.793051e05f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 6.076538e03f, .p11 = -1.701539e05f, .p21 = 2.224636e06f},
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 6.528781e03f, .p11 = -2.467750e05f, .p21 = 4.933144e06f},
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 6.283534e03f, .p11 = -1.798967e05f, .p21 = 3.048061e06f},
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 7.007843e03f, .p11 = -2.606798e05f, .p21 = 5.500898e06f},
	},

	// #4 - 838609 - K33 - 54cm, 8kg, 1600rpm
	[eMACHINE_TYPE_7_838609] =
	{
        .half_load_raw_inertia  = 0.014f,
        .inertia_low            = { .zero_inertia   = 0.0f, .p10 = 7.636302e05f },
        .inertia_high           = { .zero_inertia   = 0.0f, .p10 = 6.036201e05f },
        .unbalance_low_const    = { .zero_unbalance = 0.0f, .p01 = 5.456246e03f, .p11 = -1.277265e05f, .p21 = 1.828023e06f},
        .unbalance_low_wave     = { .zero_unbalance = 0.0f, .p01 = 5.544838e03f, .p11 = -1.485255e05f, .p21 = 2.734379e06f},
        .unbalance_high_const   = { .zero_unbalance = 0.0f, .p01 = 5.816099e03f, .p11 = -1.605562e05f, .p21 = 3.129976e06f},
        .unbalance_high_wave    = { .zero_unbalance = 0.0f, .p01 = 5.768163e03f, .p11 = -1.540427e05f, .p21 = 3.800784e06f},
	},
};
// clang-format on

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void measurements_clear(void);
static float32_t
measurements_calculate_inertia(const inertia_factors_t *const factors, const float32_t raw_inertia);
static float32_t measurements_calculate_unbalance(
    const unbalance_factors_t *const factors, const float32_t raw_inertia,
    const float32_t raw_unbalance
);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function will check and decide which measurement is to be performed
 *
 * @param[in]    *p_measure_req - pointer to new start measurement request
 * @return       bool - returns true if command was accepted
 */
////////////////////////////////////////////////////////////////////////////////
bool measurements_check_request(const command_parser_start_measure_t *const p_measure_req)
{
    measurement_type_t measurement_type = eMEAS_NONE;

    const app_data_t *const p_app_data = app_get_data();

    if ((p_measure_req != NULL) && (p_app_data != NULL))
    {
        // Clear measurements if desired
        if (p_measure_req->reset == true)
        {
            measurements_clear();

            // If reset was request end commnad, no other action is possible
            return true;
        }

        // Measurement can only be started if start RPM is the same as our current target and actual
        // speed Also ignore request if motor is not running Also check if the target RPM is reached
        // Also no active measurement shall be active already
        if ((p_measure_req->start_rpm == p_app_data->mc_cmd.target_rpm) &&
            (p_app_data->fsm.current_state == eAPP_FSM_STATE_RUN) &&
            (p_app_data->mc_cmd.rpm_reached == true) &&
            (g_measurements_data.state != eMEASUREMENT_IN_PROGRESS))
        {
            // For now measurement is custom, following search will check if this is predefined
            // measurement
            measurement_type = eMEAS_CUSTOM;

            // Find what measurement are we looking for
            for (int i = 0; i < MEASUREMENTS_CONFIG_SIZE; i++)
            {
                if ((p_measure_req->start_rpm == g_measurements_config[i].start_rpm) &&
                    (p_measure_req->amplitude == g_measurements_config[i].amplitude) &&
                    (p_measure_req->turns == g_measurements_config[i].turns))
                {
                    // We have found valid config for measurement
                    measurement_type = g_measurements_config[i].type;
                    break;
                }
            }
        }
    }

    g_measurements_data.type = measurement_type;

    // Return false if measurements request is not valid
    if (measurement_type == eMEAS_NONE)
    {
        return false;
    }
    else
    {
        // Start measurements
        motor_control_set_measurement_loadRPM_amplitude(p_measure_req->amplitude);
        motor_control_set_measurement_turns(p_measure_req->turns);

        // Reset duration and timeout
        app_update_timeouts(p_measure_req->duration, p_measure_req->timeout);

        g_measurements_data.request = true;

        return true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Handler for inertia and unbalance measurement
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
void measurements_handle(void)
{
    static uint32_t start_timestamp = 0;

    switch (g_measurements_data.state)
    {
    case eMEASUREMENT_IDLE:
    {
        // Wait for start of measurements
        if (g_measurements_data.request)
        {
            motor_control_set_measurement_start(true);
            g_measurements_data.state = eMEASUREMENT_IN_PROGRESS;

            start_timestamp = HAL_GetTick();

            g_measurements_data.request = false;
        }
    }
    break;

    case eMEASUREMENT_IN_PROGRESS:
    {
        // wait for measurement to start (100ms)
        if ((HAL_GetTick() - start_timestamp) > 100)
        {
            // If measurement is already done, then something is wrong
            if ((HAL_GetTick() - start_timestamp) < 150)
            {
                if (true == motor_control_is_measurement_done())
                {
                    motor_control_set_measurement_start(false);
                    g_measurements_data.success = false;

                    g_measurements_data.type  = eMEAS_NONE;
                    g_measurements_data.state = eMEASUREMENT_IDLE;
                }
            }
            else
            {
                // Was measurement done?
                if (true == motor_control_is_measurement_done())
                {
                    motor_control_set_measurement_start(false);
                    g_measurements_data.success = true;

                    // Store following values for every measurement
                    g_measurements_data.values.raw_unbalance = motor_control_get_harmonic_torque();
                    g_measurements_data.values.raw_friction  = motor_control_get_friction();
                    g_measurements_data.values.overload      = motor_control_get_overload();
                    g_measurements_data.values.au_ball       = motor_control_get_auball();
                    g_measurements_data.values.raw_au_ball   = motor_control_get_auball();

                    // Parse measured data
                    switch (g_measurements_data.type)
                    {
                    // This case shall not be possible anyway??
                    case eMEAS_NONE:
                        break;

                    // Measurement at constant low speed
                    //==================================================
                    case eMEAS_CONST_LOW:
                    {
                        // Only calculate unbalance
                        g_measurements_data.values.unbalance = measurements_calculate_unbalance(
                            &g_wm_factors[g_wm_config.type].unbalance_low_const,
                            g_measurements_data.values.raw_inertia,
                            g_measurements_data.values.raw_unbalance
                        );
                    }
                    break;

                    // Measurement at constant high speed
                    //==================================================
                    case eMEAS_CONST_HIGH:
                    {
                        // Only calculate unbalance
                        g_measurements_data.values.unbalance = measurements_calculate_unbalance(
                            &g_wm_factors[g_wm_config.type].unbalance_high_const,
                            g_measurements_data.values.raw_inertia,
                            g_measurements_data.values.raw_unbalance
                        );
                    }
                    break;

                    // Measurement at wave at low speed
                    //==================================================
                    case eMEAS_WAVE_LOW:
                    {
                        // In this measurement also update raw inertia value
                        g_measurements_data.values.raw_inertia = motor_control_get_dc_torque();

                        // Calculate inertia and unbalance
                        g_measurements_data.values.inertia = measurements_calculate_inertia(
                            &g_wm_factors[g_wm_config.type].inertia_low,
                            g_measurements_data.values.raw_inertia
                        );

                        g_measurements_data.values.unbalance = measurements_calculate_unbalance(
                            &g_wm_factors[g_wm_config.type].unbalance_low_wave,
                            g_measurements_data.values.raw_inertia,
                            g_measurements_data.values.raw_unbalance
                        );
                    }
                    break;

                    // Measurement at wave at high speed
                    //==================================================
                    case eMEAS_WAVE_HIGH:
                    {
                        // In this measurement also update raw inertia value
                        g_measurements_data.values.raw_inertia = motor_control_get_dc_torque();

                        // Calculate inertia and unbalance
                        g_measurements_data.values.inertia = measurements_calculate_inertia(
                            &g_wm_factors[g_wm_config.type].inertia_high,
                            g_measurements_data.values.raw_inertia
                        );

                        g_measurements_data.values.unbalance = measurements_calculate_unbalance(
                            &g_wm_factors[g_wm_config.type].unbalance_high_wave,
                            g_measurements_data.values.raw_inertia,
                            g_measurements_data.values.raw_unbalance
                        );
                    }
                    break;

                    // 4) Custom measurement
                    //==================================================
                    case eMEAS_CUSTOM:
                    {
                        // In this measurement also update raw inertia value
                        g_measurements_data.values.raw_inertia = motor_control_get_dc_torque();

                        // Clear calculated values
                        g_measurements_data.values.inertia   = NAN;
                        g_measurements_data.values.unbalance = NAN;
                    }

                    default:
                        // Do nothing
                        break;
                    }

                    g_measurements_data.type  = eMEAS_NONE;
                    g_measurements_data.state = eMEASUREMENT_IDLE;
                }
            }
        }
    }
    break;

    default:
        // Do nothing
        break;
    }

    // update can variable
    app__measure_state = g_measurements_data.state;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function will calculate inertia from provided measured data and factor
 *
 */
////////////////////////////////////////////////////////////////////////////////
static float32_t
measurements_calculate_inertia(const inertia_factors_t *const factors, const float32_t raw_inertia)
{
    // Calculate fitted inertia using equation
    //    inertia = p00 + (p10 * x) [g]
    //    x = measured inertia - calibration value
    //    inertia = p10 * (x - zero_inertia_raw) [g]

    // Calculate measured inertia value using zero value (x)
    const float32_t calibrated_raw_inertia = raw_inertia - factors->zero_inertia;

    // Calculate inertia
    const float32_t inertia = factors->p10 * calibrated_raw_inertia;

    return inertia;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function will calculate unbalance from provided measured data and factor
 *
 */
////////////////////////////////////////////////////////////////////////////////
static float32_t measurements_calculate_unbalance(
    const unbalance_factors_t *const factors, const float32_t raw_inertia,
    const float32_t raw_unbalance
)
{
    // Calculate fitted unbalance using equation
    //    unbalance = p00 + (p01 * y) + (p11 * x * y) + (p21 * x^2 * y) [g]
    //    x = raw measured inertia
    //    y = measured  - calibration value
    //    unbalance = (p01 * (y-zero_unbalance_raw)) + (p11 * x * (y-zero_unbalance_raw)) + (p21 *
    //    x^2 * (y-zero_unbalance_raw)) [g]

    // Calculate measured inertia value using zero value (x)
    const float32_t calibrated_raw_unbalance = raw_unbalance - factors->zero_unbalance;

    // Calculate inertia
    const float32_t unbalance = factors->p01 * calibrated_raw_unbalance +
                                factors->p11 * raw_inertia * calibrated_raw_unbalance +
                                factors->p21 * raw_inertia * raw_inertia * calibrated_raw_unbalance;

    return unbalance;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Clears all measurement done flags
 *
 * @param void
 */
////////////////////////////////////////////////////////////////////////////////
static void measurements_clear(void)
{
    // Raw inertia with half load clothes (this is needed for first unbalance measurement at 75 RPM
    g_measurements_data.values.raw_inertia = g_wm_factors[g_wm_config.type].half_load_raw_inertia;

    // Clear other measured values
    g_measurements_data.values.raw_unbalance = NAN;
    g_measurements_data.values.raw_friction  = NAN;
    g_measurements_data.values.raw_au_ball   = NAN;
    g_measurements_data.values.overload      = NAN;

    // Clear calculated values
    g_measurements_data.values.inertia   = NAN;
    g_measurements_data.values.unbalance = NAN;
    g_measurements_data.values.au_ball   = NAN;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Returns current state of measurement procedurre
 *
 *@return    current state of measurement procedurre
 */
////////////////////////////////////////////////////////////////////////////////
measurement_state_t measurements_get_state(void)
{
    return g_measurements_data.state;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function returns pointer to measusrement data structure
 *
 *@param[in] void
 *@return    pointer of type measurements_data_t
 */
////////////////////////////////////////////////////////////////////////////////
const measurements_data_t *const measurements_get_data(void)
{
    return &g_measurements_data;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Functions stops and reset any ongoing measurements
 *
 *@param[in] void
 */
////////////////////////////////////////////////////////////////////////////////
void measurements_cancel(void)
{
    g_measurements_data.success = false;

    motor_control_set_measurement_start(false);

    g_measurements_data.state = eMEASUREMENT_IDLE;
    g_measurements_data.type  = eMEAS_NONE;
    app__measure_state        = g_measurements_data.state;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Sets the machine configuration for measuremetns.
 *
 * @param[in]    command_parser_machine_set_t - packet with machine setttings
 * @return       bool - true if set is succeful
 */
////////////////////////////////////////////////////////////////////////////////
bool measurements_set_machine(const command_parser_machine_set_t *const machine_set_packet)
{
    if ((machine_set_packet->type > eMACHINE_TYPE_NONE) &&
        (machine_set_packet->type < eMACHINE_TYPE_NUM))
    {
        g_wm_config.type = machine_set_packet->type;

        // Set calibration values
        g_wm_factors[g_wm_config.type].inertia_low.zero_inertia =
            machine_set_packet->zero_inertia_low;
        g_wm_factors[g_wm_config.type].inertia_high.zero_inertia =
            machine_set_packet->zero_inertia_high;
        // Copy the same value to both const and wave tables, since both uses the same value
        g_wm_factors[g_wm_config.type].unbalance_low_const.zero_unbalance =
            machine_set_packet->zero_unbalance_low;
        g_wm_factors[g_wm_config.type].unbalance_low_wave.zero_unbalance =
            machine_set_packet->zero_unbalance_low;
        g_wm_factors[g_wm_config.type].unbalance_high_const.zero_unbalance =
            machine_set_packet->zero_unbalance_high;
        g_wm_factors[g_wm_config.type].unbalance_high_wave.zero_unbalance =
            machine_set_packet->zero_unbalance_high;

        g_wm_config.configured = true;

        measurements_clear();

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Returns currently selected machine type.
 *
 * @param[in]    void
 * @return       machine_config_t
 */
////////////////////////////////////////////////////////////////////////////////
const wm_config_t *const measurements_get_machine(void)
{
    return &g_wm_config;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Returns measurements factors table
 *
 * @param[in]    void
 * @return       wm_factors_t
 */
////////////////////////////////////////////////////////////////////////////////
const wm_factors_t *const measurements_get_factors(wm_type_t type)
{
    return &g_wm_factors[type];
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Returns true if selected_machine_type is different from eMACHINE_TYPE_NONE or
 * eMACHINE_TYPE_NUM.
 *
 * @param[in]    void
 * @return       bool
 */
////////////////////////////////////////////////////////////////////////////////
bool measurements_is_machine_set(void)
{
    return g_wm_config.configured;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
