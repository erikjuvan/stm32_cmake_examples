////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file     hisense_proto.h
 * @note
 * @author   Martin Pec
 * @date     06.06.2021
 * @brief    Washing machine command protocol implementation header
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef __HISENSE_PROTO_H
#define __HISENSE_PROTO_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "build_info.h"
#include "project_config.h"
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////

#define COMMAND_PROTO_MAX_PAYLOAD_SIZE 64
#define COMMAND_PROTO_MAX_PACKET_SIZE  COMMAND_PROTO_MAX_PAYLOAD_SIZE + 5
#define PU_START_BYTE                  0x55
#define PU_OK_MESSAGE_TIMEOUT          250 // 25ms x 10

/**
 * Type of wash machine remote protocol state
 */
typedef enum
{
    eCOMMAND_PARSER_STATE_UNDEFINED     = -1, /**< protocol in undefined state*/
    eCOMMAND_PARSER_STATE_INIT          = 0,  /**< protocol in init state*/
    eCOMMAND_PARSER_STATE_WAIT_STARTB   = 1,  /**< waiting for start byte*/
    eCOMMAND_PARSER_STATE_WAIT_CHEKSUM1 = 2,  /**< waiting for first cheksum byte */
    eCOMMAND_PARSER_STATE_WAIT_CHEKSUM2 = 3,  /**< waiting for second cheksum byte*/
    eCOMMAND_PARSER_STATE_WAIT_SIZE1    = 4,  /**< waiting for first data size byte*/
    eCOMMAND_PARSER_STATE_WAIT_SIZE2    = 5,  /**< waiting for second data size byte*/
    eCOMMAND_PARSER_STATE_WAIT_DATA     = 6,  /**< capturing data*/
    eCOMMAND_PARSER_STATE_PROCESS_DATA  = 7,  /**< processing valid frame*/
    eCOMMAND_PARSER_STATE_TIMEOUT       = 10, /**< data timeout*/
} command_parser_state_t;

/**
 * Type of wash machine remote protocol commands
 */
typedef enum
{
    eCOMMAND_PARSER_RQID_VERSION          = 1,  /**<  command request version*/
    eCOMMAND_PARSER_RQID_PING             = 2,  /**<  command request ping response*/
    eCOMMAND_PARSER_RQID_MOTOR_CONTROL    = 3,  /**<  command request motor control action*/
    eCOMMAND_PARSER_RQID_START_MEASURE    = 4,  /**<  command request to start measurements */
    eCOMMAND_PARSER_RQID_STATUS           = 5,  /**<  command request status*/
    eCOMMAND_PARSER_RQID_BLACK_CHANNEL    = 6,  /**<  command request black channel rx/tx*/
    eCOMMAND_PARSER_RQID_FAN_CONTROL      = 9,  /**<  command request fan control set*/
    eCOMMAND_PARSER_RQID_RESTART          = 10, /**<  command request system reset*/
    eCOMMAND_PARSER_RQID_READ_MEASUREMENT = 11, /**<  command request measurement*/
    eCOMMAND_PARSER_RQID_STANDBY          = 12, /**<  Enter low power mode */

    eCOMMAND_PARSER_RQID_SET_MEAS_MACHINE = 14, /**<  Sets table for measurements coefficients */
    eCOMMAND_PARSER_RQID_GET_MEAS_MACHINE = 15, /**<  Gets table for measurements coefficients */

    eCOMMAND_PARSER_RQID_SAFE_STATUS       = 20, /**<  Gets status of safety SW */
    eCOMMAND_PARSER_RQID_READ_RESET_LOG    = 21, /**<  Gets reset status log */
    eCOMMAND_PARSER_RQID_RECOVER_POWERFAIL = 22, /**<  Request to recover power-fail */

    eCOMMAND_PARSER_RQID_SET_PWM_PINS_HIZ = 0xF8, /**<  Set PWM pins to HI-Z */
    eCOMMAND_PARSER_RQID_SET_PWM_PINS_EN  = 0xF9, /**<  Set PWM pins to PWM outputs */
    eCOMMAND_PARSER_RQID_SET_CO_OBJ       = 0xFA, /**<  Set canopen object */
    eCOMMAND_PARSER_RQID_GET_CO_OBJ       = 0xFB, /**<  Get canopen object */
    eCOMMAND_PARSER_RQID_SET_GPIO_PIN     = 0xFC, /**<  Set GPIO pin */
    eCOMMAND_PARSER_RQID_GET_GPIO_PIN     = 0xFD, /**<  Get GPIO pin */
    eCOMMAND_PARSER_RQID_GET_ADC          = 0xFE, /**<  Get ADC value */
    eCOMMAND_PARSER_RQID_ENTER_TEST_MODE  = 0xFF, /**<  Enter test mode */
} command_parser_request_id_t;

/**
 * Type of wash machine remote protocol errors
 */
typedef enum
{
    eCOMMAND_PARSER_RESPID_OK                  = 0,  /**<  general response status OK*/
    eCOMMAND_PARSER_RESPID_ERROR               = 1,  /**<  general response status ERROR*/
    eCOMMAND_PARSER_RESPID_ERROR_DATA          = 2,  /**<  response status DATA (wrong cheksum,
                                                           wrong size for a particular command...*/
    eCOMMAND_PARSER_RESPID_ERROR_NOT_SUPPORTED = 3,  /**<  not supported command*/
    eCOMMAND_PARSER_RESPID_ERROR_BUSY          = 4,  /**<  unit busy to response on request*/
    eCOMMAND_PARSER_RESPID_ERROR_MOTOR_TARGET  = 5,  /**<  target RPM can not be
                                                           achieved due to various reasons */
    eCOMMAND_PARSER_RESPID_ERROR_MEASURE       = 6,  /**<  error in measurement procedure*/
    eCOMMAND_PARSER_RESPID_ERROR_SELFTEST      = 7,  /**<  self test error present in application */
    eCOMMAND_PARSER_RESPID_ERROR_HW_FAULT      = 8,  /**<  error due to hardware fault*/
    eCOMMAND_PARSER_RESPID_ERROR_SENSORLESS    = 9,  /**<  sensorless engagement failure */
    eCOMMAND_PARSER_RESPID_ERROR_ERROR         = 10, /**<  Error present in application */
} command_parser_response_id_t;

/**
 * Type of CPR master request status
 */
typedef enum
{
    ePU_REQUEST_NO_COMMAND    = 0,
    ePU_REQUEST_VALID_COMMAND = 1,
} command_parser_request_type_t;

/**
 * GPIO Port
 */
typedef enum
{
    eGPIO_PORT_A   = 0,
    eGPIO_PORT_B   = 1,
    eGPIO_PORT_C   = 2,
    eGPIO_PORT_D   = 3,
    eGPIO_PORT_E   = 4,
    eGPIO_PORT_F   = 5,
    eGPIO_PORT_NUM = 6
} gpio_port_t;

/**
 * ADC measurement types
 */
typedef enum
{
    eADC_MEAS_CURR_A    = 0,
    eADC_MEAS_CURR_B    = 1,
    eADC_MEAS_CURR_C    = 2,
    eADC_MEAS_UDC       = 3,
    eADC_MEAS_IGBT_TEMP = 4,
    eADC_MEAS_CPU_TEMP  = 5,
    eADC_MEAS_NUM       = 6
} adc_measurements_t;

/**
 * Structure of remote command packet
 */
typedef struct __attribute__((packed))
{
    uint16_t cheksum;                                 /**<  packet cheksum*/
    uint16_t size;                                    /**<  packet size*/
    uint8_t  payload[COMMAND_PROTO_MAX_PAYLOAD_SIZE]; /**<  packet payload*/

} command_parser_packet_t;

/**
 * Structure of Motor control request packet
 */
typedef struct __attribute__((packed))
{
    uint8_t  direction;  /**<  motor control request direction*/
    uint16_t target_rpm; /**<  motor control request target RPMs*/
    uint16_t ramp;       /**<  motor control request ramp*/
    uint16_t time;       /**<  motor control request action time*/
    uint16_t timeout;    /**<  motor control request next command timeout*/
    uint8_t  measure;    /**<  motor control request for measurement */
} command_parser_motor_control_t;

/**
 * Supported motor types.
 */
typedef enum
{
    eMOTOR_TYPE_GMCC_75 = 0U,
    eMOTOR_TYPE_GMCC_66,
    eMOTOR_TYPE_RECHI_81,
    eMOTOR_TYPE_NUM
} motor_type_t;

/**
 * Structure of status response packet
 */
typedef struct __attribute__((packed))
{
    uint8_t  direction;  /**<  */
    uint16_t curr_rpm;   /**<  */
    uint16_t target_rpm; /**<  */
    uint16_t ramp;       /**<  */
    uint16_t time;       /**<  */

    uint8_t  supply;             /**<  */
    uint16_t voltage;            /**<  */
    uint16_t power;              /**<  */
    uint16_t motor_temperature;  /**<  */
    uint16_t stator_current;     /**<  */
    uint16_t bridge_temperature; /**<  */

    struct
    {
        union
        {
            struct
            {
                uint8_t safe_fsm_is_error    : 1; /**< Safe FSM state is ERROR */
                uint8_t safe_fsm_is_fault    : 1; /**< Safe FSM state is FAULT */
                uint8_t black_channel_not_ok : 1; /**< Black channel status not ok  */
                uint8_t current_meas_not_ok  : 1; /**< current measurement status not ok */
                uint8_t otdet_not_ok         : 1; /**< over temperature detection status not ok */
                uint8_t door_lock_in_error   : 1; /**< door lock state in error */
            };

            uint8_t not_ok;
        } errors;

        struct
        {
            uint8_t door_closed_state     : 1; /**< Door closed indication     */
            uint8_t black_channel_timeout : 1; /**< is_timeout                 */
            uint8_t otdet_overtemp        : 1; /**< is_overtemp                */
            uint8_t door_dc_valid         : 1; /**< is_dc_valid                */
            uint8_t door_relay            : 1; /**< Door relay_state           */
            uint8_t supply_relay          : 1; /**< Motor supply relay state   */
            uint8_t can_drive             : 1; /**< Can motor be driven        */
        } status;
    } safe;

    uint8_t  state;          /**<  */
    uint8_t  motor_type;     /**<  */
    uint16_t last_error;     /**<  */
    uint8_t  last_warning;   /**<  */
    uint8_t  self_test_code; /**<  */
    uint8_t  status;         /**<  */

} command_parser_status_t;

/**
 * Structure of measurement request packet
 */
typedef struct __attribute__((packed))
{
    uint8_t  start_rpm;
    uint8_t  amplitude;
    uint8_t  turns;
    uint16_t duration;
    uint16_t timeout;
    uint8_t  reset;
} command_parser_start_measure_t;

/**
 * Structure of status response packet
 */
typedef struct __attribute__((packed))
{
    float32_t raw_inertia;
    float32_t raw_unbalance;
    float32_t raw_friction;
    float32_t raw_au_ball;
    uint8_t   overload;

    float32_t inertia;
    float32_t unbalance;
    float32_t au_ball;
} command_parser_measurement_t;

/**
 * Structure of machine configuration packet
 */
typedef struct __attribute__((packed))
{
    uint8_t   type;
    float32_t zero_inertia_low;
    float32_t zero_inertia_high;
    float32_t zero_unbalance_low;
    float32_t zero_unbalance_high;
} command_parser_machine_set_t;

/**
 * Structure of version response packet
 */
typedef struct __attribute__((packed)
) // structure well alligned, no need to be packed, but beware when new elements will be added
{
    uint16_t safe_major; /**< safety SW major version*/
    uint16_t safe_minor; /**< safety SW minor verison*/
    uint16_t safe_patch; /**< safety SW patch*/
    uint32_t safe_crc;   /**< safety SW CRC value*/

    uint16_t app_major; /**< major verion*/
    uint16_t app_minor; /**< minor version*/
    uint16_t app_patch; /**< patch number*/

} command_parser_version_t;

/**
 * Structure of SAFE status packet
 */
typedef struct __attribute__((packed))
{
    uint8_t state; /**< safety FSM state */                         // mu_safe_fsm_t next;
    uint8_t black_channel; /**< Black channel state */              // blackch_pyld_pu_t
    uint8_t current_meas_status; /**< current measurement status */ // curr_meas_status_t
    uint8_t otdet_status; /**< over temperature status */           // otdet_status_t
    uint8_t door_locked_status; /**< door locked indication */      // door_lock_state_t
    uint8_t drum_spinning_state; /**< Drum sppinning status */      // drumspin_state_t

    struct
    {
        uint8_t door_closed_state     : 1; /**< door closed indication */ // fp_Get_Door_State
        uint8_t black_channel_timeout : 1;                                // is_timeout
        uint8_t otdet_overtemp        : 1;                                // is_overtemp
        uint8_t door_dc_valid         : 1;                                // is_dc_valid
        uint8_t door_relay            : 1;                                // fp_Get_Relay_State
        uint8_t supply_relay          : 1;                                // fp_Get_Relay_State
        uint8_t can_drive             : 1;                                // fp_Can_Drive_Motor
    } status;
} command_parser_safe_status_t;

/**
 * Structure of reset status log request packet
 */
typedef struct __attribute__((packed))
{
    uint8_t idx_from_last; /**< Index of log entry from the last stored to read. */
} command_parser_rst_status_req_t;

/**
 * Structure of reset status log response packet
 */
typedef struct __attribute__((packed))
{
    uint32_t time_chkpt;    /**< Time [ms] at which the checkpoint was reached. */
    uint8_t  chkpt;         /**< Last reached checkpoint. */
    uint8_t  cause;         /**< Reset cause flags. */
    uint8_t  mu_safe_state; /**< MU safe FSM state. */
} command_parser_rst_status_rsp_t;

/**
 * Definitions of possible standby command request types
 */
typedef enum
{
    eSTANBY_REQUEST_ENTER = 0, /**< Request to enter low power mode */
    eSTANBY_REQUEST_EXIT  = 1, /**< Request to exit low power mode */
} command_standby_request_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void     pu_parse_packet(uint8_t const *data, uint16_t len);
int32_t  pu_command_parser(uint8_t ch, uint8_t timeout);
void     pu_command_parser_last_request(command_parser_motor_control_t *preq);
uint8_t *pu_get_response_buffer(void);
uint8_t  pu_get_response_size(void);
bool     proto_is_mc_cmd_pending(void);
bool     test_mode_is_enabled(void);
bool     pu_parser_is_timeout(void);

#endif // __HISENSE_PROTO_H
