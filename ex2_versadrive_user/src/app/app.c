////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file     app.c
 * @note
 * @author   Martin Pec
 * @date     06.06.2021
 * @brief    Washing machine command state machine implementation
 */
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup APP_MODULE
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
#include <string.h>

#include "stm32g4xx_hal.h"

#include "adc/adc.h"
#include "comparator/comparator.h"
#include "gpio/gpio.h"
#include "motor_control/motor_control/motor_control.h"
#include "motor_control/motor_control/motor_control_fsm.h"
#include "motor_control/motor_control/protections.h"
#include "motor_control/motor_control/protections_obj.h"
#include "simulink_model/emDriveFOC_control_global.h"
#include "opamp/opamp.h"
#include "relay.h"
#include "timer/timer.h"

#include "communication/mat/mat_requests.h"
#include "communication/mat/mat_trace.h"
#include "func_table.h"
#include "hisense_app_public.h"
#include "hisense_proto.h"
#include "mains_meas.h"
#include "measurements.h"
#include "trace/trace.h"

#include "apptime.h"
#include "precharge.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define APP_LOOP_PERIOD             5 // 5 ms

#define APP_TIMEOUT_PREECHARGE      400 // 400ms

#define APP_RAMP_MAX                20 // Compressor Hz
#define APP_RAMP_EM_STOP            0xFFFF

#define APP_RESET_DELAY             100 // ms

#define ENTER_TEST_MODE_TIME_LIMIT  5000 // ms

#define APP_RPM_REACHED_MAX_DEV_PCT (2) // %
#define APP_RPM_REACHED_DEV_OVER    (1.0f + (APP_RPM_REACHED_MAX_DEV_PCT / 100.0f))
#define APP_RPM_REACHED_DEV_UNDER   (1.0f - (APP_RPM_REACHED_MAX_DEV_PCT / 100.0f))

#define DC_LINK_DISCHARGE_VOLTAGE   30.0F // discharge DC Link capacitor below this voltage [V]
#define PROTECTIONS_UNDERVOLTAGE_DISCHARGE_LEVEL \
    0.0F // discharge DC Link capacitor below this voltage [V]

#define LOW_POWER_CLK_FREQ                16000000UL // TIM1 input clock frequency when in Low power mode [Hz]

#define DISCHARGE_MAX_CURRENT_MEAS_ENABLE // if defined, include MAX discharge current measurement

#define DISCHARGE_DELAY_TIME_MS \
    2000UL // delay between low power state entry and start of the discharge procedure
#define DISCHARGE_MAX_TIME_MS 15000UL // max time to discharge DC link capacitor [ms]
#define DISCHARGE_PWM_AMPLITUDE \
    22.5F // discharge phase PWM amplitude [%] that is added/subtracted to/from 50% PWM value to
          // generate phase current

/*
 * Discharge related data structure
 *
 */
typedef struct
{
    float pwm_u; // phase pwm value, range 1.0F..0.0F
    float pwm_v; // phase pwm value, range 1.0F..0.0F
    float pwm_w; // phase pwm value, range 1.0F..0.0F

    float pwm; // pwm amplitude to to generate phase voltage, value is added/subtracted to/from 50%
               // [%]
    float pwm_ampl; // pwm amplitude value as float in range 1.0F..0.0F

    float    dclink_voltage;
    uint32_t start_time_ms;
    uint32_t timeout_ms;
    uint32_t elapsed_time_ms;

#ifdef DISCHARGE_MAX_CURRENT_MEAS_ENABLE
    motor_control_current_t curr_phase;   // phase currents, adc value minus offset
    motor_control_current_t curr_offsets; // phase current offsets, determined at startup
    motor_control_current_t curr_max;     // max phase currents measured during discharge
#endif

    bool is_active; // flag to indicate discharge is active

} discharge_data_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = app__state
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = SBYTE [-2 ... 10]
@! DESCRIPTION  = "Current state of motor control (-1 - Undefined, 0 - PowerUP, 1 - Self-test, 2 -\
Stop, 3 - StartUP, 4 - RUN, 5 - Slow Down, 6 - Measure)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile int8_t app__state = eAPP_FSM_STATE_UNDEF;

/*
@! SYMBOL       = app__state_duration
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "State duration "
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "sec"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__state_duration;

/*
@! SYMBOL       = app__measurement_request
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Request for a measurement (either reflecting RS232 request value or for direct\
manual request)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "/"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 3
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint8_t app__measurement_request = 0;

/*
@! SYMBOL       = app__direction_request
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "direction request (0-stop, 1-CCW, 2-CW)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "/"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 4
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint8_t app__direction_request = 2;

/*
@! SYMBOL       = app__target_rpm
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Target speed"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "RPM"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 5
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__target_rpm = 75;

/*
@! SYMBOL       = app__ramp
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Target ramp"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "RPM/s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 6
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__ramp = 50;

/*
@! SYMBOL       = app__duration
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Time for a whole command step (acceleration+maintaing RPMs)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 7
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__duration = UINT16_MAX; // By setting it to MAX we are decreasing safety, this
                                              // should be just a temporary hack/fix

/*
@! SYMBOL       = app__timeout
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Timeout for next command"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 8
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__timeout = UINT16_MAX; // By setting it to MAX we are decreasing safety, this
                                             // should be just a temporary hack/fix

/*
@! SYMBOL       = app__manual_request
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Manual request of motor control action (0- no action, 1-request, 2-request in\
progress)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "/"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 9
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint32_t app__manual_request = eMANUAL_REQ_NO_ACTION;

/*
@! SYMBOL       = app__emergency_stop
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Emergency stop command (0 - disabled, 1 - enabled)"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "/"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 10
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST         = 0
@! END
*/
volatile uint8_t app__emergency_stop = eEMERGENCY_STOP_DISABLED;

/*
@! SYMBOL       = app__selftest
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Selftest flags\
0x00 Self-test OK\
0x01 Shunt measurements offset out of valid range\
0x02 Safety self-test failed"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 12
@! PERSIST      = 0
@! END
*/
volatile uint16_t app__selftest = 0;

/*
@! SYMBOL       = app__errors
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Error flags\
0x0000 No error present\
0x0001 Overvoltage on DC capacitor\
0x0002 Undervoltage on DC capacitor\
0x0004 Overtemperature of bridge\
0x0008 Phase overcurrent\
0x0010 Power fail error\
0x0020 Motor control error (stall, blocked motor)\
0x0040 Command + timeout time elapsed\
0x0080 Safety checks error"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 13
@! PERSIST      = 0
@! END
*/
volatile uint32_t app__errors = 0;

/*
@! SYMBOL       = app__warnings
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = ULONG
@! DESCRIPTION  = "Warning flags\
0x00 No warning preset \
0x01 Temperature above limit, output limited \
0x04 Power at limit \
0x08 Command time elapsed\
0x10 Machine not selected, no coefficients for measurements"
@! GROUP        = app
@! DIMENSION    = 1 1
@! UNIT         = "s"
@! CAN_INDEX    = 0x500A
@! CAN_SUBINDEX = 14
@! PERSIST      = 0
@! END
*/
volatile uint32_t app__warnings = 0;

/**
 * Application data
 */
app_data_t app_data = {0};

/**
 * Flag - if possible to enter test mode
 */
static uint8_t g_can_enter_test_mode = 0;

/**
 * Discharge data structure
 */
discharge_data_t g_discharge_data = {
    .pwm_u = 0.5F,
    .pwm_v = 0.5F,
    .pwm_w = 0.5F,

    .pwm      = DISCHARGE_PWM_AMPLITUDE,
    .pwm_ampl = 0.0F,

#ifdef DISCHARGE_MAX_CURRENT_MEAS_ENABLE
    .curr_phase   = {0.0F},
    .curr_offsets = {0.0F},
    .curr_max     = {0.0F},
#endif

    .dclink_voltage  = 0.0F,
    .start_time_ms   = 0,
    .timeout_ms      = 0,
    .elapsed_time_ms = 0,
    .is_active       = false
};

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_hndl(const app_data_t *const papp_data);
static void app_fsm_state_power_up(void);
static void app_fsm_state_stop(void);
static void app_fsm_state_run(void);
static void app_fsm_state_slow_down(void);
static void app_fsm_state_low_power(void);
static void app_fsm_state_reset(void);
static void app_fsm_state_power_fail(void);

static void  app_run_motor(app_mc_cmd_t *const cmd);
static void  app_stop_motor(app_mc_cmd_t *const cmd);
static void  app_release_motor(void);
static void  app_set_drum_speed(float speed);
static float app_get_drum_speed(void);
static void  app_check_rpm_reached(void);

static void app_update_self_tests(void);

static void app_check_errors(void);
static void app_set_error(app_error_t error);
static void app_clear_error(app_error_t error);

static void app_update_warnings(void);
static void app_set_warning(app_warning_t warning);
static void app_clear_warning(app_warning_t warning);

static void app_check_fan(void);

static bool app_check_is_safe_operational(void);

static void app_discharge_dc_link_hndl(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Initialisation of statemachine for handling remote commands from PU
 *
 * @param[in]    none
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void app_fsm_init(void)
{
    app_data.fsm.current_state = eAPP_FSM_STATE_UNDEF;
    app_data.fsm.next_state    = eAPP_FSM_STATE_POWERUP;

    timer7_update_irq_enable();

    // Read pin for test mode
    g_can_enter_test_mode = !((uint8_t)gpio_get_test_pin());
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     state machine task body
 *
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void app_loop(void)
{
    static uint32_t                last_loop_tick   = 0;
    static uint32_t                mat_process_tick = 0;
    command_parser_motor_control_t mc_request = {0};

    const uint32_t current_tick = HAL_GetTick();

    if ((current_tick - last_loop_tick) >= APP_LOOP_PERIOD)
    {
        last_loop_tick = current_tick;

        app_data.time += APP_LOOP_PERIOD;

        hisense_app_run();

        app_check_fan();

        app_check_errors();
        app_update_warnings();

        // Check if we received motor control command and get the needed data
        if (true == proto_is_mc_cmd_pending())
        {
            // Get latest command data
            pu_command_parser_last_request(&mc_request);

            app_data.mc_cmd.new_cmd = true;

            app__manual_request = eMANUAL_REQ_NO_ACTION; // cancel manual request (should be 0)
        }

        // Check if we received motor control command from MAT
        if (mat_request_get_mc_request(&mc_request) == true)
        {
            app_data.mc_cmd.new_cmd = true;

            app__manual_request = eMANUAL_REQ_NO_ACTION; // cancel manual request (should be 0)
        }

        // Check if we got motor control request from CAN PC Tool
        if (eMANUAL_REQ_REQUESTED == app__manual_request)
        {
            mc_request.direction  = app__direction_request;
            mc_request.target_rpm = app__target_rpm;
            mc_request.ramp       = app__ramp;
            mc_request.time       = app__duration;
            mc_request.timeout    = app__timeout;

            app_data.mc_cmd.new_cmd = true;

            app__manual_request = eMANUAL_REQ_IN_PROGRESS; // notify PC tool, that manual request
                                                           // has been accepted and is in execution
        }

        // Check if we have new command to execute
        if (app_data.mc_cmd.new_cmd)
        {
            // Copy command request to local structure that will be used in FSM
            app_data.mc_cmd.direction =
                mc_request.direction == eMOTOR_DIRECTION_STOP
                    ? eMOTOR_DIRECTION_STOP
                    : eMOTOR_DIRECTION_CW; // any non STOP direction is converted to CW (2)
            app_data.mc_cmd.target_rpm = mc_request.target_rpm;
            app_data.mc_cmd.duration   = mc_request.time;
            app_data.mc_cmd.timeout    = mc_request.timeout;

            // Limit ramp to max, also allow 0xFFFF for emergency brake
            if (mc_request.ramp == APP_RAMP_EM_STOP)
            {
                app_data.mc_cmd.ramp           = mc_request.ramp;
                app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_ENABLED;
            }
            else
            {
                if (mc_request.ramp > APP_RAMP_MAX)
                {
                    app_data.mc_cmd.ramp = APP_RAMP_MAX;
                }
                else
                {
                    app_data.mc_cmd.ramp = mc_request.ramp;
                }

                app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_DISABLED;
            }

            // Copy command to flow variables for CAN debug
            app__direction_request = app_data.mc_cmd.direction;
            app__duration          = app_data.mc_cmd.duration;
            app__target_rpm        = app_data.mc_cmd.target_rpm;
            app__ramp              = app_data.mc_cmd.ramp;
            app__timeout           = app_data.mc_cmd.timeout;
            app__emergency_stop    = app_data.mc_cmd.emergency_stop;
        }

        // Execute Main FSM
        app_fsm_hndl(&app_data);

        // Clear requests after FSM was handled, either request was handled or it cannot be
        app_data.mc_cmd.new_cmd = false;
    }

    // Test mode can be entered only within time limit
    if ((g_can_enter_test_mode == 1) && (HAL_GetTick() > ENTER_TEST_MODE_TIME_LIMIT))
    {
        g_can_enter_test_mode = 0;
    }

    // Call MAT Trace processing function, that flushes characters left in the trace buffer
    const int MAT_TRACE_FLUSH_PERIOD = 500;
    if (apptime_is_elapsed_ms(mat_process_tick, MAT_TRACE_FLUSH_PERIOD))
    {
        mat_process_tick = apptime_get_ms();
        mat_trace_process();
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    state machine main state handler
 *
 * @param[in]    const app_data_t *const papp_data       - app data structure (state machine inputs)
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_hndl(const app_data_t *const papp_data)
{
    // Update first entry and state duration
    if (app_data.fsm.current_state != app_data.fsm.next_state)
    {
        app_data.fsm.first_entry   = true;
        app_data.fsm.duration      = 0;
        app_data.fsm.current_state = app_data.fsm.next_state;
    }
    else
    {
        app_data.fsm.first_entry  = false;
        app_data.fsm.duration    += APP_LOOP_PERIOD;
    }

    app__state_duration = app_data.fsm.duration;

    // Call requested state handler
    switch (app_data.fsm.current_state)
    {
    case eAPP_FSM_STATE_UNDEF:
        app_data.fsm.next_state = eAPP_FSM_STATE_POWERUP;
        break;
    case eAPP_FSM_STATE_POWERUP:
        app_fsm_state_power_up();
        break;
    case eAPP_FSM_STATE_STOP:
        app_fsm_state_stop();
        break;
    case eAPP_FSM_STATE_RUN:
        app_fsm_state_run();
        break;
    case eAPP_FSM_STATE_SLOW_DOWN:
        app_fsm_state_slow_down();
        break;
    case eAPP_FSM_STATE_LOW_POWER:
        app_fsm_state_low_power();
        break;
    case eAPP_FSM_STATE_RESET:
        app_fsm_state_reset();
        break;
    case eAPP_FSM_STATE_POWER_FAIL:
        app_fsm_state_power_fail();
        break;
    default: // Do nothing
        break;
    }

    app__state = app_data.fsm.current_state; // propagate state to flow variable
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     state machine power up handler
 *
 * @param[in]     const app_data_t *const papp_data - app data structure (state machine inputs)
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_power_up(void)
{
    if (app_data.fsm.first_entry == true)
    {
        pg_safe_func_table->fp_Set_Power_Mode(ePOWER_MODE_NORMAL);

        // Enable peripherals
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLCFGR_PLLPEN);
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLCFGR_PLLQEN);
        __HAL_RCC_PLLCLKOUT_ENABLE(RCC_PLLCFGR_PLLREN);

        __HAL_RCC_FDCAN_CLK_ENABLE();
        __HAL_RCC_DMAMUX1_CLK_ENABLE();
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_ADC12_CLK_ENABLE();
        __HAL_RCC_DAC1_CLK_ENABLE();
        __HAL_RCC_DAC3_CLK_ENABLE();
        __HAL_RCC_TIM4_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();

        // Turn back on IGBT
        gpio_turn_on_off_IGBT((uint8_t)1);

        // Start precharge
        precharge_start();
    }

    // Check if Safety finished boot up procedure and that precharge is done
    if ((app_check_is_safe_operational() == true) &&
        (motor_control_fsm_get_state() > eMOTOR_CONTROL_FSM_STATE_INIT) &&
        (precharge_is_done() == true))
    {
        // Check Selftest results
        app_update_self_tests();

        // Finish precharge
        precharge_finish();

        app_data.fsm.next_state = eAPP_FSM_STATE_STOP;
    }

    // If power fail is detected, FSM shall enter Power-Fail State
    if (mains_meas_get_powerfail_state() == eMAINS_MEAS_POWERFAIL_ACTIVE)
    {
        app_data.fsm.next_state = eAPP_FSM_STATE_POWER_FAIL;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     state machine stop handler
 *
 * @param[in]     const app_data_t *const papp_data - app data structure (state machine inputs)
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_stop(void)
{
    if (app_data.fsm.first_entry == true)
    {
        // Clear previous request data
        app_data.mc_cmd.duration   = 0;
        app_data.mc_cmd.direction  = eMOTOR_DIRECTION_STOP;
        app_data.mc_cmd.target_rpm = 0;
        app_data.mc_cmd.timeout    = 0;
        app_data.mc_cmd.ramp       = 0;

        app__manual_request = 0; // notify PC tool, that manual request has been canceled

        measurements_cancel();

        motor_control_disable();
    }

    // Check if safety entered low power and relay is off
    if ((pg_safe_func_table->fp_Get_Power_Mode() == ePOWER_MODE_LOW) &&
        (relay_get_state(RELAY_PRECHARGE) == RELAY_OFF))
    {
        app_data.fsm.next_state = eAPP_FSM_STATE_LOW_POWER;
    }
    // When command has been received start motor
    else if (app_data.mc_cmd.new_cmd)
    {
        // Only accept new command if we have no errors present
        if (app_data.errors == eAPP_ERROR_NONE)
        {
            // Stop command in stop state?
            if (app_data.mc_cmd.direction == eMOTOR_DIRECTION_STOP)
            {
                app_data.mc_cmd.rpm_reached = true;
            }
            else
            {
                app_run_motor(&app_data.mc_cmd);
            }
        }
    }

    // If power fail is detected, FSM shall enter Power-Fail State
    if (mains_meas_get_powerfail_state() == eMAINS_MEAS_POWERFAIL_ACTIVE)
    {
        app_data.fsm.next_state = eAPP_FSM_STATE_POWER_FAIL;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     state machine run handler
 *
 * @param[in]     const app_data_t *const papp_data - app data structure (state machine inputs)
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_run(void)
{
    if (app_data.fsm.first_entry == true)
    {
    }

    // First check if we have new command
    if (app_data.mc_cmd.new_cmd == true)
    {
        // Stop command
        if (app_data.mc_cmd.direction == eMOTOR_DIRECTION_STOP)
        {
            app_stop_motor(&app_data.mc_cmd);

            // Exit this state
            return;
        }
        // Run command
        else
        {
            // Updated motor command
            app_run_motor(&app_data.mc_cmd);

            // clear state duration to able to respond with correct movement duration and timeout
            app_data.fsm.duration = 0;
        }
    }

    // Is motor control enabled ??
    if (motor_control_fsm_get_state() != eMOTOR_CONTROL_FSM_STATE_RUN)
    {
        // If its not, then there is probably some error, which is read in app loop
        app_data.fsm.next_state = eAPP_FSM_STATE_STOP;
        return;
    }

    // Safety still allows driving??
    if (pg_safe_func_table->fp_Can_Drive_Motor() == false)
    {
        // Motor is in ASC from safety
        motor_control_break_enable(true);

        app_data.fsm.next_state = eAPP_FSM_STATE_SLOW_DOWN;
        return;
    }

    // Safety timeout of black channel?
    if ((test_mode_is_enabled() == false) &&
        false /* ( pg_safe_func_table->fp_Get_Blckch_Timeout() == true ) STUB */
    )
    {
        // Create emergency STOP command
        app_data.mc_cmd.direction      = eMOTOR_DIRECTION_STOP;
        app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_ENABLED;
        app_stop_motor(&app_data.mc_cmd);
    }

    // Check if we have reached the target RPM only if we are still expected to rotate (not
    // stopping) RPM reached flag is cleared in app_stop_motor() function thats why do not execute
    // app_check_rpm_reached() function to keep the flag cleared
    if (app_data.mc_cmd.direction != eMOTOR_DIRECTION_STOP)
    {
        app_check_rpm_reached();
    }

    // If time for command is expired set timeout warning
    if ((app_data.fsm.duration) > (app_data.mc_cmd.duration * 1000))
    {
        app_set_warning(eAPP_WARNING_COMMAND_TIMEOUT);

        // If even timeout is expired, set error and stop the motor
        if (app_data.fsm.duration >
            ((app_data.mc_cmd.timeout * 1000) + (app_data.mc_cmd.duration * 1000)))
        {
            app_set_error(eAPP_ERROR_TIMEOUT);

            // Create emergency STOP command
            app_data.mc_cmd.direction      = eMOTOR_DIRECTION_STOP;
            app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_ENABLED;
            app_stop_motor(&app_data.mc_cmd);
            return;
        }

        // If until this time RPM was not reached set warning
        if (false == app_data.mc_cmd.rpm_reached)
        {
            app_set_warning(eAPP_WARNING_RPM_NOT_REACHED);
        }
    }

    // If drum speed is less than limit, stop the motor
    if ((fabsf(app_get_drum_speed()) < APP_MIN_SPEED_CMD) &&
        (fabsf((float)app_data.mc_cmd.target_rpm) < APP_MIN_SPEED_CMD
        )) /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */
    {
        app_data.mc_cmd.direction = eMOTOR_DIRECTION_STOP;
        app_stop_motor(&app_data.mc_cmd);
    }

    // If power fail is detected, FSM shall enter Power-Fail State
    if (mains_meas_get_powerfail_state() == eMAINS_MEAS_POWERFAIL_ACTIVE)
    {
        // Create Normal stop command
        app_data.mc_cmd.ramp           = APP_RAMP_MAX;
        app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_DISABLED;
        app_data.mc_cmd.direction      = eMOTOR_DIRECTION_STOP;
        app_stop_motor(&app_data.mc_cmd);

        app_data.fsm.next_state = eAPP_FSM_STATE_POWER_FAIL;
    }

    // Handle measurements
    measurements_handle();
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    state machine slow down handler
 *
 * @param[in]    const app_data_t *const papp_data       - app data structure (state machine inputs)
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_slow_down(void)
{
    if (app_data.fsm.first_entry == true)
    {
    }

    if (pg_safe_func_table->fp_Can_Drive_Motor())
    {
        // We allow to update slow down ramp
        if (app_data.mc_cmd.new_cmd == true)
        {
            // Updated motor command
            app_stop_motor(&app_data.mc_cmd);
        }

        // If time for command is expired set timeout warning
        if ((app_data.fsm.duration) > (app_data.mc_cmd.duration * 1000))
        {
            app_set_warning(eAPP_WARNING_COMMAND_TIMEOUT);

            // If even timeout is expired, set error and put motor in ASC to stop it faster
            if (app_data.fsm.duration >
                ((app_data.mc_cmd.timeout * 1000) + (app_data.mc_cmd.duration * 1000)))
            {
                app_set_error(eAPP_ERROR_TIMEOUT);

                if (pg_safe_func_table->fp_Can_Drive_Motor())
                {
                    app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_ENABLED;

                    motor_control_break_enable(true);
                    timer1_break_event_generate();
                }
            }

            // If until this time RPM was not reached set warning
            if (false == app_data.mc_cmd.rpm_reached)
            {
                app_set_warning(eAPP_WARNING_RPM_NOT_REACHED);
            }
        }
    }

#if (ALT_POWER_TEST != 1)
    if (app_data.mc_cmd.emergency_stop == eEMERGENCY_STOP_ENABLED)
    {
        if (fabsf(app_get_drum_speed()) < APP_MIN_SPEED_CMD_ASC)
        {
            app_data.mc_cmd.rpm_reached = true;

            app_release_motor();
        }
    }
    else
    {
        if (fabsf(app_get_drum_speed()) < APP_MIN_SPEED_CMD)
        {
            app_data.mc_cmd.rpm_reached = true;

            app_release_motor();
        }
    }
#else
    app_data.mc_cmd.rpm_reached = true;

    app_release_motor();
#endif

    // If power fail is detected, FSM shall enter Power-Fail State
    if (mains_meas_get_powerfail_state() == eMAINS_MEAS_POWERFAIL_ACTIVE)
    {
        app_data.fsm.next_state = eAPP_FSM_STATE_POWER_FAIL;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    state machine "low power" handler
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_low_power(void)
{
    TIM_HandleTypeDef              htim1                = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    if (app_data.fsm.first_entry == true)
    {
        // raise TIM16 IRQ priority otherwise STL runtime clock test can fail
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 0UL, 0UL);

        // lower ADC IRQ priority because TIM16 IRQ must have highest
        HAL_NVIC_SetPriority(ADC1_2_IRQn, 2UL, 0UL);

        // remove FOC callback and insert custom discharge callback,
        // this way ADC is still running but FOC will not interfere with discharge procedure
        adc_set_callback(eADC_CALLBACK_INJECTED_CHANNELS_DONE, &app_discharge_dc_link_hndl);

        // raise UART IRQ priority to keep communication with PU working
        HAL_NVIC_SetPriority(USART1_IRQn, 1UL, 0UL);

        // reconfigure TIM1 dead times to low input clock frequency in LOW power mode
        htim1.Instance                        = TIM1;
        sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
        sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
        sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
        sBreakDeadTimeConfig.DeadTime =
            (0x0080U +
             (((uint16_t)((PWM_DEADTIME / (1.0 / (LOW_POWER_CLK_FREQ / 1000.0 / 1000.0))) / 2.0)) -
              64U));
        sBreakDeadTimeConfig.BreakState    = TIM_BREAK_ENABLE;
        sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
        sBreakDeadTimeConfig.BreakFilter =
            0xF; // TODO: filter because BKIN is now additionally connected to external GPO_SD??
        sBreakDeadTimeConfig.BreakAFMode     = TIM_BREAK_AFMODE_INPUT;
        sBreakDeadTimeConfig.Break2State     = TIM_BREAK2_ENABLE /*TIM_BREAK2_DISABLE*/;
        sBreakDeadTimeConfig.Break2Polarity  = TIM_BREAK2POLARITY_HIGH;
        sBreakDeadTimeConfig.Break2Filter    = 0xF;
        sBreakDeadTimeConfig.Break2AFMode    = TIM_BREAK2_AFMODE_INPUT;
        sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

        HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig);

        // reconfigure TIM1 PWM to low input clock frequency in LOW power mode
        htim1.Init.Prescaler         = 0;
        htim1.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
        htim1.Init.Period            = 533;
        htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
        htim1.Init.RepetitionCounter = 1;
        htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

        // re-initialize TIM1 with new configuration
        HAL_TIM_Base_Init(&htim1);

        // lower UV protection voltage limit to not trigger alarm
        protections__undervoltage = PROTECTIONS_UNDERVOLTAGE_DISCHARGE_LEVEL;

        // just to be sure, disable motor control
        motor_control_disable();

        // set initial PWM values, phase voltages are 0V
        timer1_clear_break_flag();
        timer1_set_pwms(0.5F, 0.5F, 0.5F);

#ifdef DISCHARGE_MAX_CURRENT_MEAS_ENABLE
        // get phase current offsets
        motor_control_get_current_offset(&g_discharge_data.curr_offsets);

        // reset max currents
        g_discharge_data.curr_max.phase_u = 0.0F;
        g_discharge_data.curr_max.phase_v = 0.0F;
        g_discharge_data.curr_max.phase_w = 0.0F;
#endif

        // disable fan in case it is still on
        app_set_fan(eFAN_DISABLED);

        // log current time plus delay
        g_discharge_data.start_time_ms = app_data.fsm.duration + DISCHARGE_DELAY_TIME_MS;

        // set MAX discharge time
        g_discharge_data.timeout_ms =
            app_data.fsm.duration + DISCHARGE_MAX_TIME_MS + DISCHARGE_DELAY_TIME_MS;

        // enable discharge
        g_discharge_data.is_active = true;
    }

    // get DClink voltage
    g_discharge_data.dclink_voltage = adc_analog_get()->vdc;

    // wait until DC Link voltage drops below DC_LINK_DISCHARGE_VOLTAGE level
    // or discharge time is elapsed to prevent indefinite discharge state
    if ((g_discharge_data.is_active == true) &&
        ((g_discharge_data.dclink_voltage < DC_LINK_DISCHARGE_VOLTAGE) ||
         (app_data.fsm.duration > g_discharge_data.timeout_ms)))
    {
        g_discharge_data.is_active = false;

        // disable PWMs
        timer1_disable_pwms();

        // Disable IGBT supply
        gpio_turn_on_off_IGBT((uint8_t)0);

        // Disable compressor NTC power supply TODO

        // Disable peripheral clocks
        __HAL_RCC_TIM1_CLK_DISABLE();
        __HAL_RCC_TIM4_CLK_DISABLE();
        __HAL_RCC_FDCAN_CLK_DISABLE();
        __HAL_RCC_ADC12_CLK_DISABLE();
        __HAL_RCC_DMAMUX1_CLK_DISABLE();
        __HAL_RCC_DMA1_CLK_DISABLE();
        __HAL_RCC_DAC1_CLK_DISABLE();
        __HAL_RCC_DAC3_CLK_DISABLE();
        __HAL_RCC_TIM17_CLK_DISABLE();

        (void)timer7_init(0);

        __HAL_RCC_TIM1_CLK_SLEEP_DISABLE();
        __HAL_RCC_TIM2_CLK_SLEEP_DISABLE();
        __HAL_RCC_TIM3_CLK_SLEEP_DISABLE();
        __HAL_RCC_TIM4_CLK_SLEEP_DISABLE();
        __HAL_RCC_FDCAN_CLK_SLEEP_DISABLE();
        __HAL_RCC_ADC12_CLK_SLEEP_DISABLE();
        __HAL_RCC_DMAMUX1_CLK_SLEEP_DISABLE();
        __HAL_RCC_DMA1_CLK_SLEEP_DISABLE();
        __HAL_RCC_DAC1_CLK_SLEEP_DISABLE();
        __HAL_RCC_DAC3_CLK_SLEEP_DISABLE();
        __HAL_RCC_TIM17_CLK_SLEEP_DISABLE();

        opamp_stop();
        (void)comparator_stop();

        // Configure PWM outputs into floating input
        GPIOA->MODER &=
            ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk |
              GPIO_MODER_MODE12_Msk);
        GPIOB->MODER &= ~(GPIO_MODER_MODE15_Msk);
        GPIOC->MODER &= ~(GPIO_MODER_MODE13_Msk);

        // Configure ADC inputs into floating input
        GPIOA->MODER &=
            ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk |
              GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk);
        GPIOB->MODER &= ~(GPIO_MODER_MODE12_Msk | GPIO_MODER_MODE13_Msk | GPIO_MODER_MODE14_Msk);
        GPIOB->MODER &= ~(GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE8_Msk);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    state machine "reset" handler
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_reset(void)
{
    if (app_data.fsm.duration > APP_RESET_DELAY)
    {
        NVIC_SystemReset();
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    state machine powerfail handler
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_fsm_state_power_fail(void)
{
    if (app_data.fsm.first_entry == true)
    {
        pg_safe_func_table->fp_Enter_Powerfail();
    }

    // If Highsense protocol eCOMMAND_PARSER_RQID_RECOVER_POWERFAIL received
    // leave this state and enter STOP state
    if (app_event_is_set(eAPP_EVT_EXIT_PWR_FAIL) == true)
    {
        app_event_clear(eAPP_EVT_EXIT_PWR_FAIL);
        pg_safe_func_table->fp_Recover_Powerfail();
        mains_meas_clear_powerfail();
    }

    // Check if Safety finished powerfail recover procedure
    if (app_check_is_safe_operational() == true)
    {
        app_data.fsm.next_state = eAPP_FSM_STATE_STOP;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Returns current state of motor control state machine
 *
 *@return    current state of motor control FSM
 */
////////////////////////////////////////////////////////////////////////////////
app_fsm_state_t app_fsm_get_state(void)
{
    return app_data.fsm.current_state;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Set the new state for FSM
 *
 *@param[in] app_fsm_state_t state - new state
 *@return    void
 */
////////////////////////////////////////////////////////////////////////////////
void app_fsm_set_state(app_fsm_state_t state)
{
    app_data.fsm.next_state = state;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function will start motor control if the direction and requested rpm are
 *   valid values.
 *
 * @param[in]    motor_direction direction - direction of desired rotation
 * @param[in]    float ramp - desired ramp in RPM of Drum
 * @param[in]    float rpm  - desired RPM of drum
 */
////////////////////////////////////////////////////////////////////////////////
static void app_run_motor(app_mc_cmd_t *const cmd)
{
    if ((cmd->direction != eMOTOR_DIRECTION_STOP) && (cmd->target_rpm >= APP_MIN_SPEED_CMD))
    {
        if (cmd->direction == eMOTOR_DIRECTION_CW)
        {
            app_set_drum_speed(cmd->target_rpm);
        }
        else if (cmd->direction == eMOTOR_DIRECTION_CCW)
        {
            app_set_drum_speed(-cmd->target_rpm);
        }

        // Set ramp calculated to motor frame, Limit to max
        if (cmd->ramp > APP_RAMP_MAX)
        {
            motor_control_set_ramp(APP_RAMP_MAX * APP_DRUM_RPM_RATIO);
        }
        else
        {
            motor_control_set_ramp(cmd->ramp * APP_DRUM_RPM_RATIO);
        }

        // motor_control_set_ramp( cmd->ramp * APP_DRUM_RPM_RATIO );

        // Start motor
        motor_control_enable();

        // If we are stopped move to startup state
        if (app_data.fsm.current_state == eAPP_FSM_STATE_STOP)
        {
            app_data.fsm.next_state = eAPP_FSM_STATE_RUN;
        }

        // Reset rpm reached flag
        app_data.mc_cmd.rpm_reached        = false;
        app_data.mc_cmd.is_over_target_rpm = false;
        // Calculate required speed change to reach the target RPM
        // this is needed to determine the target RPM reached flag value
        // to know from which direction we are approaching and are going to overshoot the target RPM
        if (app_data.mc_cmd.direction == eMOTOR_DIRECTION_CW)
        {
            app_data.mc_cmd.speed_change = app_data.mc_cmd.target_rpm - app_get_drum_speed();
        }
        else if (app_data.mc_cmd.direction == eMOTOR_DIRECTION_CCW)
        {
            app_data.mc_cmd.speed_change = (-app_data.mc_cmd.target_rpm) - app_get_drum_speed();
        }
        else
        {
            app_data.mc_cmd.speed_change = -app_get_drum_speed();
        }

        // Clear warnings with new command
        app_clear_warning(eAPP_WARNING_COMMAND_TIMEOUT);
        app_clear_warning(eAPP_WARNING_RPM_NOT_REACHED);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function will stop the motor, either by controlled ramp or by short
 *   circuiting the phases
 *
 * @param[in]    app_mc_cmd_t *const cmd - command structure
 */
////////////////////////////////////////////////////////////////////////////////
static void app_stop_motor(app_mc_cmd_t *const cmd)
{
    if (cmd->direction == eMOTOR_DIRECTION_STOP)
    {
        app_set_drum_speed(0.0f);

        // Set ramp calculated to motor frame, Limit to max
        if (cmd->ramp > APP_RAMP_MAX)
        {
            motor_control_set_ramp(APP_RAMP_MAX * APP_DRUM_RPM_RATIO);
        }
        else
        {
            motor_control_set_ramp(cmd->ramp * APP_DRUM_RPM_RATIO);
        }

        if (cmd->emergency_stop == eEMERGENCY_STOP_ENABLED)
        {
            motor_control_break_enable(true);
            timer1_break_event_generate();
        }

        app_data.fsm.next_state = eAPP_FSM_STATE_SLOW_DOWN;

        measurements_cancel();

        // Reset rpm reached flag
        app_data.mc_cmd.rpm_reached = false;

        // Clear warnings with new command
        app_clear_warning(eAPP_WARNING_COMMAND_TIMEOUT);
        app_clear_warning(eAPP_WARNING_RPM_NOT_REACHED);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Set desired speed of drum to motor control.
 *   Function takes into account gear ratio
 *
 * @param[in]    float speed - desired drump speed in RPM
 * @return       void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_set_drum_speed(float speed)
{
    motor_control_set_rpm(speed * APP_DRUM_RPM_RATIO);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Gets speed of drum in RPM.
 *   Function takes into account gear ratio from motor
 *
 * @param[in]    void
 * @return       float - speed of drum in RPM
 */
////////////////////////////////////////////////////////////////////////////////
static float app_get_drum_speed(void)
{
    return (motor_control_get_motor_RPM() / APP_DRUM_RPM_RATIO);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Check if motor has reached the target RPM.
 *
 *   Latches a flag indicating motor has reached the target RPM. Target RPM is
 *   reached when the motor speed reaches at least the target RPM (or more) and
 *   is within the allowed deviation.
 *
 * @return       void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_check_rpm_reached(void)
{
    float32_t speed;
    float32_t target;
    float32_t target_min;
    float32_t target_max;

    speed  = app_get_drum_speed();
    target = app_data.mc_cmd.target_rpm;
    if (app_data.mc_cmd.direction == eMOTOR_DIRECTION_CCW)
    {
        target = -target;
    }

    if (target > 0)
    {
        // Because target is a positive number
        // maximum deviation is over the target RPM (larger number) and
        // minimum deviation is under the target RPM (smaller number)
        target_max = target * APP_RPM_REACHED_DEV_OVER;
        target_min = target * APP_RPM_REACHED_DEV_UNDER;
    }
    else if (target < 0)
    {
        // Because target is a negative number
        // maximum deviation is under the target RPM (larger number) and
        // minimum deviation is over the target RPM (smaller number)
        target_max = target * APP_RPM_REACHED_DEV_UNDER;
        target_min = target * APP_RPM_REACHED_DEV_OVER;
    }
    else
    {
        // This case shall be handled by another function (SLOW_DOWN state)
        // here only add the possibility to set RPM reached flag in case speed matches exactly to 0
        target_max = 0;
        target_min = 0;
    }

    if (((app_data.mc_cmd.speed_change > 0) && (speed >= target)
        ) || // We are approaching the target RPM from from below and so need to have speed larger
             // than target RPM
        ((app_data.mc_cmd.speed_change < 0) && (speed <= target)
        ) || // We are approaching the target RPM from from above and so need to have speed smaller
             // than target RPM
        (app_data.mc_cmd.speed_change == 0)) // Speed is matched exactly to target RPM
    {
        app_data.mc_cmd.is_over_target_rpm = true;
    }

    // Speed over-reached the target RPM and is now inside of allowed deviation?
    if ((app_data.mc_cmd.is_over_target_rpm) && (speed <= target_max) && (speed >= target_min))
    {
        app_data.mc_cmd.rpm_reached = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function is used when motor is stopped to transition to stopped state
 *   It will also clear short circuit if emergency brake was used
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_release_motor(void)
{
    if (pg_safe_func_table->fp_Can_Clear_Break())
    {
        timer1_break_event_clear();
    }

    motor_control_disable();

    app_data.mc_cmd.emergency_stop = eEMERGENCY_STOP_DISABLED;

    app_data.fsm.next_state = eAPP_FSM_STATE_STOP;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Updates selft test flags
 *
 *@param    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_update_self_tests(void)
{
    const protections_t motor_control_protections = protections_get_active();

    if (motor_control_protections & ePROTECTIONS_CURRENT_OFFSET)
    {
        app_data.self_test |= eAPP_SELFTEST_CURRENT_OFFSET;
    }

    app__selftest = app_data.self_test;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Updates all sources of possible errors
 *
 *@param    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_check_errors(void)
{
    const protections_t motor_control_protections = protections_get_active();
    static uint32_t     timestamp                 = 0;

    if (motor_control_protections & ePROTECTIONS_OVERCURRENT)
    {
        app_set_error(eAPP_ERROR_OVERCURRENT);
    }

    if (motor_control_protections & ePROTECTIONS_OVERVOLTAGE)
    {
        app_set_error(eAPP_ERROR_OVERVOLTAGE);
    }

    if (motor_control_protections & ePROTECTIONS_UNDERVOLTAGE)
    {
        app_set_error(eAPP_ERROR_UNDERVOLTAGE);
    }

    if (motor_control_protections & ePROTECTIONS_OVERTEMP_BRIDGE)
    {
        app_set_error(eAPP_ERROR_OVERTEMPERATURE);
    }

    if (motor_control_protections & ePROTECTIONS_MOTOR_CONTROL_ERROR)
    {
        app_set_error(eAPP_ERROR_MOTOR_CONTROL);
    }

    if ((motor_control_protections & ePROTECTIONS_MOTOR_ERROR) &&
        (mains_meas_get_powerfail_state() != eMAINS_MEAS_POWERFAIL_ACTIVE))
    {
        app_set_error(eAPP_ERROR_MOTOR_DISCONNECTED);
    }

    if (motor_control_protections & ePROTECTIONS_MOTOR_OVERTEMP)
    {
        app_set_error(eAPP_ERROR_MOTOR_OVERTEMPERATURE);
    }

    if (mains_meas_get_powerfail_state() == eMAINS_MEAS_POWERFAIL_ACTIVE)
    {
        app_set_error(eAPP_ERROR_POWER_FAIL);

        // Clear false detection of disconnected motor
        app_clear_error(eAPP_ERROR_MOTOR_DISCONNECTED);
    }

    // Only check for safety error when no power fail is present
    if (false == (app_data.errors & eAPP_ERROR_POWER_FAIL))
    {
        mu_safe_fsm_t safety_state = pg_safe_func_table->fp_Get_State();

        if ((safety_state == eMU_SAFE_FSM_FAULT) || (safety_state == eMU_SAFE_FSM_ERROR))
        {
            if (timestamp == 0)
            {
                timestamp = HAL_GetTick();
            }
            // Only set Safety error, when it is present for more than 500ms
            else if ((HAL_GetTick() - timestamp) >= 200)
            {
                app_set_error(eAPP_ERROR_SAFETY_ERROR);
            }
        }
        else
        {
            timestamp = 0;
        }
    }
    else
    {
        timestamp = 0;
    }

    app__errors = app_data.errors;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Checks if any errors are cleared
 *
 *@param    void
 */
////////////////////////////////////////////////////////////////////////////////
void app_clear_errors(void)
{
    const protections_t motor_control_protections = protections_get_active();

    if (false == (motor_control_protections & ePROTECTIONS_OVERCURRENT))
    {
        app_clear_error(eAPP_ERROR_OVERCURRENT);
    }

    if (false == (motor_control_protections & ePROTECTIONS_OVERVOLTAGE))
    {
        app_clear_error(eAPP_ERROR_OVERVOLTAGE);
    }

    if (false == (motor_control_protections & ePROTECTIONS_UNDERVOLTAGE))
    {
        app_clear_error(eAPP_ERROR_UNDERVOLTAGE);
    }

    if (false == (motor_control_protections & ePROTECTIONS_OVERTEMP_BRIDGE))
    {
        app_clear_error(eAPP_ERROR_OVERTEMPERATURE);
    }

    if (false == (motor_control_protections & ePROTECTIONS_MOTOR_CONTROL_ERROR))
    {
        app_clear_error(eAPP_ERROR_MOTOR_CONTROL);
    }

    if (false == (motor_control_protections & ePROTECTIONS_MOTOR_ERROR))
    {
        app_clear_error(eAPP_ERROR_MOTOR_DISCONNECTED);
    }

    if (false == (motor_control_protections & ePROTECTIONS_MOTOR_OVERTEMP))
    {
        app_clear_error(eAPP_ERROR_MOTOR_OVERTEMPERATURE);
    }

    if (eMAINS_MEAS_POWERFAIL_ACTIVE != mains_meas_get_powerfail_state())
    {
        app_clear_error(eAPP_ERROR_POWER_FAIL);
    }

    mu_safe_fsm_t safety_state = pg_safe_func_table->fp_Get_State();
    if ((safety_state != eMU_SAFE_FSM_FAULT) && (safety_state != eMU_SAFE_FSM_ERROR))
    {
        app_clear_error(eAPP_ERROR_SAFETY_ERROR);
    }

    app_clear_error(eAPP_ERROR_TIMEOUT);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Sets specific error
 *
 *@param[in] app_error_r error - error flag to set
 */
////////////////////////////////////////////////////////////////////////////////
static void app_set_error(app_error_t error)
{
    app_data.errors |= error;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Clears specific error
 *
 *@param[in] app_error_r error - error flag to clear
 */
////////////////////////////////////////////////////////////////////////////////
static void app_clear_error(app_error_t error)
{
    app_data.errors &= ~error;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Updates warning flags
 *
 *@param    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_update_warnings(void)
{
    const warnings_t warnings = warings_get_active();

    // Temperature limits triggered
    if (warnings & eMC_WARNING_BRIDGE_TEMP_LIMIT)
    {
        app_set_warning(eAPP_WARNING_BRIDGE_TEMP_LIMIT);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_BRIDGE_TEMP_LIMIT);
    }

    if (warnings & eMC_WARNING_MOTOR_TEMP_LIMIT)
    {
        app_set_warning(eAPP_WARNING_MOTOR_TEMP_LIMIT);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_MOTOR_TEMP_LIMIT);
    }

    // Power limit
    if (warnings & eMC_WARNING_POWER_LIMIT)
    {
        app_set_warning(eAPP_WARNING_POWER_LIMIT);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_POWER_LIMIT);
    }

    // Machine type coeficients not set
    if (false == measurements_is_machine_set())
    {
        app_set_warning(eAPP_WARNING_MEAS_MACHINE);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_MEAS_MACHINE);
    }

    // Get power limit state
    const motor_control_power_limit_state power_limit_state = motor_control_get_power_limit_state();
    if (power_limit_state == eMC_POWER_LIMIT_STATE_TEMPERATURE_REDUCED)
    {
        app_set_warning(eAPP_WARNING_POWER_LIMIT_TEMP_REDUCED);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_POWER_LIMIT_TEMP_REDUCED);
    }

    if (power_limit_state == eMC_POWER_LIMIT_STATE_TIME_REDUCED)
    {
        app_set_warning(eAPP_WARNING_POWER_LIMIT_TIME_REDUCED);
    }
    else
    {
        app_clear_warning(eAPP_WARNING_POWER_LIMIT_TIME_REDUCED);
    }

    app__warnings = app_data.warnings;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Sets specific warning
 *
 *@param[in] app_warning_t - warning flag to set
 */
////////////////////////////////////////////////////////////////////////////////
static void app_set_warning(app_warning_t warning)
{
    app_data.warnings |= warning;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Clears specific warning
 *
 *@param[in] app_warning_t - warning flag to clear
 */
////////////////////////////////////////////////////////////////////////////////
static void app_clear_warning(app_warning_t warning)
{
    app_data.warnings &= ~warning;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Returns state if we can driver motor from SAFE code
 *
 *@return bool - true if we can drive motor
 */
////////////////////////////////////////////////////////////////////////////////
bool app_can_drive(void)
{
    return pg_safe_func_table->fp_Can_Drive_Motor();
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Checks if any condition to turn off fan is present
 *
 *@return void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_check_fan(void)
{
    // Turn off fan in following cases:
    // TODO
    // if( (pu_parser_is_timeout() == true) || (pg_safe_func_table->fp_Get_State() ==
    // eMU_SAFE_FSM_ERROR))
    // {
    //     app_set_fan(eFAN_DISABLED);
    // }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Turn fan ON or OFF
 *
 *@param[in] state  Turn fan ON or OFF
 *@return    None
 */
////////////////////////////////////////////////////////////////////////////////
void app_set_fan(fan_state_t state)
{
    app_data.fan = state;

    if (state == eFAN_ENABLED)
    {
        HAL_GPIO_WritePin(GPIO_PORT_FAN, GPIO_PIN_FAN, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIO_PORT_FAN, GPIO_PIN_FAN, GPIO_PIN_RESET);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  REturns application data structure
 *
 *@param[]  void
 */
////////////////////////////////////////////////////////////////////////////////
const app_data_t *const app_get_data(void)
{
    return &app_data;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function returns value of g_can_enter_test_mode.
 *
 *@return    uint8_t
 */
////////////////////////////////////////////////////////////////////////////////
uint8_t app_can_enter_test_mode(void)
{
    return g_can_enter_test_mode;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function updated duration and timetout values for running command and reset
 *  timer of those
 *
 *@param[in]  uint16_t duration - motor control command duration
 *@param[in]  uint16_t timeout  - motor control command timeout
 *@return     bool - true if values are updated
 */
////////////////////////////////////////////////////////////////////////////////
bool app_update_timeouts(const uint16_t duration, const uint16_t timeout)
{
    if (app_data.fsm.current_state == eAPP_FSM_STATE_RUN)
    {
        app_data.fsm.duration    = 0;
        app_data.mc_cmd.duration = duration;
        app_data.mc_cmd.timeout  = timeout;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function to set application event
 *
 *@param[in] app_evt_t - Sets the event message to be parsed in Application
 *@return    None
 */
////////////////////////////////////////////////////////////////////////////////
void app_event_set(app_evt_t evt)
{
    app_data.evt |= evt;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function to clear application event
 *
 *@param[in] app_evt_t - Clears the event message from Application
 *@return    None
 */
////////////////////////////////////////////////////////////////////////////////
void app_event_clear(app_evt_t evt)
{
    app_data.evt &= ~evt;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Function to check if event flag is set
 *
 *@param[in] app_evt_t - Event flag to be checked
 *@return    None
 */
////////////////////////////////////////////////////////////////////////////////
bool app_event_is_set(app_evt_t evt)
{
    bool ret_val = false;

    if (app_data.evt & evt)
    {
        ret_val = true;
    }

    return ret_val;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Check if Safety is operational
 *
 *@param[in] None
 *@return    true if Safety is operational
 */
////////////////////////////////////////////////////////////////////////////////
bool app_check_is_safe_operational(void)
{
    bool is_safe_ok = false;

    mu_safe_fsm_t safety_state = pg_safe_func_table->fp_Get_State();

    if ((safety_state != eMU_SAFE_FSM_ERROR) && (safety_state != eMU_SAFE_FSM_POWERFAIL) &&
        (safety_state != eMU_SAFE_FSM_BOOT) && (safety_state != eMU_SAFE_FSM_POST_INIT) &&
        (safety_state != eMU_SAFE_FSM_SETUP))
    {
        is_safe_ok = true;
    }

    return is_safe_ok;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    ADC Injected Conversion Complete IRQ handler when discharge is active
 *
 * @param[in]    void
 */
////////////////////////////////////////////////////////////////////////////////
static void app_discharge_dc_link_hndl(void)
{
    if ((g_discharge_data.is_active == true) &&
        (g_discharge_data.start_time_ms < app_data.fsm.duration))
    {
        // calculate time when dc link capacitor is discharging
        g_discharge_data.elapsed_time_ms = app_data.fsm.duration - g_discharge_data.start_time_ms;

#ifdef DISCHARGE_MAX_CURRENT_MEAS_ENABLE
        // calculate phase currents, absolute value
        g_discharge_data.curr_phase.phase_u =
            fabs(adc_analog_get()->current_u - g_discharge_data.curr_offsets.phase_u);
        g_discharge_data.curr_phase.phase_v =
            fabs(adc_analog_get()->current_v - g_discharge_data.curr_offsets.phase_v);
        g_discharge_data.curr_phase.phase_w =
            fabs(adc_analog_get()->current_w - g_discharge_data.curr_offsets.phase_w);

        // update max currents
        if (g_discharge_data.curr_max.phase_u < g_discharge_data.curr_phase.phase_u)
        {
            g_discharge_data.curr_max.phase_u = g_discharge_data.curr_phase.phase_u;
        }
        if (g_discharge_data.curr_max.phase_v < g_discharge_data.curr_phase.phase_v)
        {
            g_discharge_data.curr_max.phase_v = g_discharge_data.curr_phase.phase_v;
        }
        if (g_discharge_data.curr_max.phase_w < g_discharge_data.curr_phase.phase_w)
        {
            g_discharge_data.curr_max.phase_w = g_discharge_data.curr_phase.phase_w;
        }
#endif

        // PWM amplitude
        g_discharge_data.pwm_ampl = g_discharge_data.pwm / 100.0F;

        // calculate phase PWMs according to PWM amplitude
        g_discharge_data.pwm_u = 0.50F + g_discharge_data.pwm_ampl * (0.50F);
        g_discharge_data.pwm_v = 0.50F + g_discharge_data.pwm_ampl * (-0.25F);
        g_discharge_data.pwm_w = 0.50F + g_discharge_data.pwm_ampl * (-0.25F);

        // apply PWM values to TIM1
        timer1_set_pwms(g_discharge_data.pwm_u, g_discharge_data.pwm_v, g_discharge_data.pwm_w);
        timer1_enable_pwms();
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
