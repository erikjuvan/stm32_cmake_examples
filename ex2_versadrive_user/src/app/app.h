////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file        hisense_fsm.h
 * @note
 * @author     Martin Pec
 * @date     06.06.2021
 * @brief    Washing machine command state machine implementation header
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef __HISENSE_FSM_H
#define __HISENSE_FSM_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define APP_DRUM_RPM_RATIO \
    (60.0f) // We are converting from Hz to motor RPMs (e.g. 50Hz * 60 = 3000 RPM)
// Minimum speed command for motor in [ rpm ]
#define APP_MIN_SPEED_CMD     (25.0f)
// Minimum speed for ASCX breaking before turning off[ rpm ]
#define APP_MIN_SPEED_CMD_ASC (35.0f)

/**
 * Definitions of FSM states
 */
typedef enum
{
    eAPP_FSM_STATE_UNDEF      = -1, /**< Undefined state */
    eAPP_FSM_STATE_POWERUP    = 0,  /**< System in power up*/
    eAPP_FSM_STATE_STOP       = 2,  /**< System in STOP state waiting for valid command*/
    eAPP_FSM_STATE_RUN        = 4,  /**< Sensorless valid and motor is running*/
    eAPP_FSM_STATE_SLOW_DOWN  = 5,  /**< No valid command or stop request, motor ramping to 0 RPM*/
    eAPP_FSM_STATE_LOW_POWER  = 6,  /**< Low power state, where power consumption is minimized */
    eAPP_FSM_STATE_RESET      = 7,  /**< Reset state, will reset in 100ms */
    eAPP_FSM_STATE_POWER_FAIL = 8,  /**< Power fail - exit only on Exit-Powerfail command */
    eAPP_FSM_STATE_DISCHARGE  = 9,  /**< Discharge DC capacitor before entering low power state */
} app_fsm_state_t;

/**
 * Enumeration for motor direction command
 */
typedef enum
{
    eMOTOR_DIRECTION_STOP = 0, /**< Command to stop the motor */
    eMOTOR_DIRECTION_CCW  = 1, /**< Command to spin the motor in counter clockwise direction */
    eMOTOR_DIRECTION_CW   = 2, /**< Command to spin the motor in clockwise direction */
} motor_direction_t;

/**
 * Enumeration for fan relay
 */
typedef enum
{
    eFAN_DISABLED = 0, /**< Relay disabled */
    eFAN_ENABLED  = 1  /**< Relay enabled */
} fan_state_t;

/**
 * Enumeration for emergency stop status
 */
typedef enum
{
    eEMERGENCY_STOP_DISABLED = 0, /**< Emergency stop enabled */
    eEMERGENCY_STOP_ENABLED  = 1, /**< Emergency stop disabled */
} emergency_stop_status_t;

/**
 * Enumeration for manual request
 */
typedef enum
{
    eMANUAL_REQ_NO_ACTION   = 0, /**< No action */
    eMANUAL_REQ_REQUESTED   = 1, /**< Manual request is requested */
    eMANUAL_REQ_IN_PROGRESS = 2, /**< Manual request is in progress */
} eManual_request_status_t;

/**
 * Error definitions
 */
typedef enum
{
    eAPP_ERROR_NONE                  = 0x0000, /**< No error present */
    eAPP_ERROR_OVERVOLTAGE           = 0x0001, /**< Overvoltage on DC capacitor */
    eAPP_ERROR_UNDERVOLTAGE          = 0x0002, /**< Undervoltage on DC capacitor */
    eAPP_ERROR_OVERTEMPERATURE       = 0x0004, /**< Overtemperature of bridge */
    eAPP_ERROR_OVERCURRENT           = 0x0008, /**< Phase overcurrent */
    eAPP_ERROR_POWER_FAIL            = 0x0010, /**< Power fail error */
    eAPP_ERROR_MOTOR_CONTROL         = 0x0020, /**< Motor control error (stall, blocked motor) */
    eAPP_ERROR_TIMEOUT               = 0x0040, /**< Command + timeout time elapsed */
    eAPP_ERROR_SAFETY_ERROR          = 0x0080, /**< Safety checks error */
    eAPP_ERROR_MOTOR_DISCONNECTED    = 0x0100, /**< Motor disconnected error */
    eAPP_ERROR_MOTOR_OVERTEMPERATURE = 0x0200, /**< Motor over temperature */
} app_error_t;

/**
 * Warnings definitions
 */
typedef enum
{
    eAPP_WARNING_NONE              = 0x00, /**< No warning preset */
    eAPP_WARNING_BRIDGE_TEMP_LIMIT = 0x01, /**< Bridge Temperature above limit, output limited */
    eAPP_WARNING_MOTOR_TEMP_LIMIT  = 0x02, /**< Motor Temperature above limit, output limited */
    eAPP_WARNING_POWER_LIMIT       = 0x04, /**< Power at limit */
    eAPP_WARNING_COMMAND_TIMEOUT   = 0x08, /**< Command time elapsed */
    eAPP_WARNING_MEAS_MACHINE = 0x10, /**< Machine not selected, no coefficients for measurements */
    eAPP_WARNING_RPM_NOT_REACHED = 0x20, /**< Target RPM was not reached in defined duration time */
    eAPP_WARNING_POWER_LIMIT_TEMP_REDUCED = 0x40, /**< Power limit is reduced due to temperature */
    eAPP_WARNING_POWER_LIMIT_TIME_REDUCED = 0x80, /**< Power limit is reduced due to time */
} app_warning_t;

typedef enum
{
    eAPP_EVT_EXIT_PWR_FAIL =
        (1 << 0), /**< Event flag. Will cause exit from Power-Fail state if present */
} app_evt_t;

/**
 * Selftest definitions
 */
typedef enum
{
    eAPP_SELFTEST_NONE           = 0x00, /**< Self-test OK */
    eAPP_SELFTEST_CURRENT_OFFSET = 0x01, /**< Shunt measurements offset out of valid range */
} app_selftest_t;

/**
 * Application FSM type
 */
typedef struct
{
    app_fsm_state_t current_state; /**< Current state of motor control */
    app_fsm_state_t next_state;    /**< Next state of motor control */
    bool            first_entry;   /**< Only true when entering the state */
    uint32_t        duration;      /**< Duration of state in counts called ie APP machine loops */
} app_fsm_t;

/**
 * Motor Control Command
 */
typedef struct
{
    motor_direction_t direction;  /**< motor direction */
    uint16_t          target_rpm; /**< requested RPMs */
    uint16_t          ramp;       /**< ramp to target RPMs */
    emergency_stop_status_t
             emergency_stop; /**< Flag to indicate we need to stop with emergency brake */
    uint16_t duration;    /**< Time for whole step for acceleration and keeping the target RPMs. */
    uint16_t timeout;     /**< Timeout [s] (if new command is not received in timeout time, motor is
                             automatically shut down). */
    uint8_t new_cmd;      /**< Flag to indicate we have new motor command to execute */
    uint8_t rpm_reached;  /**< Flag that indicated that RPM for current command was reached */
    float   speed_change; /**< Required speed change to reach target RPM */
    uint8_t is_over_target_rpm; /**< Flag indicating that target RPM or more has been reached */
} app_mc_cmd_t;

/**
 * Application data structure
 */
typedef struct
{
    app_fsm_t    fsm;    /**< Application state machine */
    app_mc_cmd_t mc_cmd; /**< Motor control command */
    app_evt_t    evt;    /**< FSM Event Flags */
    uint32_t     time;   /**< Total time of application */

    fan_state_t fan; /**< Fan relay state */

    app_selftest_t self_test; /**< Selftest results */
    app_error_t    errors;    /**< Current present errors */
    app_warning_t  warnings;  /**< Current present warnings */
} app_data_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void app_fsm_init(void);
void app_loop(void);

void app_fsm_set_state(app_fsm_state_t state);
void app_set_fan(fan_state_t state);
void app_clear_errors(void);

void app_event_set(app_evt_t evt);
void app_event_clear(app_evt_t evt);
bool app_event_is_set(app_evt_t evt);

const app_data_t *const app_get_data(void);
app_fsm_state_t         app_fsm_get_state(void);
uint8_t                 app_can_enter_test_mode(void);
bool                    app_update_timeouts(const uint16_t duration, const uint16_t timeout);

#endif // __HISENSE_FSM_H
