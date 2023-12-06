////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file     hisense_proto.c
 * @note
 * @author   Martin Pec
 * @date     06.06.2021
 * @brief    Washing machine command protocol implementation
 */
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup HISENSE_PROTO_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "hisense_proto.h"
#include "project_config.h"
#include "stm32g4xx_hal.h"

#include "func_table.h"
#include "motor_control/motor_control/motor_control.h"
#include "otdet_common.h"
#include "peripheral/adc/adc.h"
#include "version.h"

#include "app.h"
#include "black_channel_common.h"
#include "mains_meas.h"
#include "measurements.h"
#include "reset_status_log_common.h"

#include "build_info.h"

#include "co_canopen.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define PARSER_TIMEOUT (10000) // in ms

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
command_parser_packet_t pgu_parser_buffer;
command_parser_packet_t pgu_response_buffer;
bool                    g_proto_mc_cmd_pending = false;

uint8_t pgu_temp_buffer[COMMAND_PROTO_MAX_PACKET_SIZE];
uint8_t pgu_temp_size;

command_parser_motor_control_t g_motor_control_request;

static uint8_t g_test_mode = 0;

const GPIO_TypeDef *g_gpio_port_table[6] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF};

uint32_t g_last_packet_timestamp = 0;

// extern uint8_t can_enter_test_mode;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Check if test mode is enabled
 *
 * @param[in]    none
 * @return       true if test mode enabled
 */
////////////////////////////////////////////////////////////////////////////////
bool test_mode_is_enabled(void)
{
    return g_test_mode != 0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     cheksum calculation (data + frame)
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_frame(command_parser_packet_t *pbuf)
{
    uint16_t cnt;
    uint8_t *pptr;

    // start byte
    pgu_temp_buffer[0] = 0x55;

    // size
    pptr               = (uint8_t *)&pbuf->size;
    pgu_temp_buffer[1] = *pptr;

    pptr++;
    pgu_temp_buffer[2] = *pptr;

    const uint16_t size = pbuf->size;
    for (cnt = 0; cnt < size; cnt++)
    {
        pgu_temp_buffer[3 + cnt] = pbuf->payload[cnt];
    }

    // cheksum
    pptr                     = (uint8_t *)&pbuf->cheksum;
    pgu_temp_buffer[3 + cnt] = *pptr;
    pptr++;
    pgu_temp_buffer[3 + cnt + 1] = *pptr;

    pgu_temp_size = 3 + cnt + 2;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     cheksum calculation (data + frame)
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         uint16_t                            - cheksum value
 */
////////////////////////////////////////////////////////////////////////////////
uint16_t pu_cheksum(command_parser_packet_t const *pbuf)
{
    uint16_t checksum = 0;
    uint16_t cnt;

    const uint16_t size = pbuf->size;
    for (cnt = 0; cnt < size; cnt++)
    {
        checksum += pbuf->payload[cnt];
    }

    checksum += PU_START_BYTE;
    checksum += pbuf->size & 0xff;
    checksum += (pbuf->size >> 8) & 0xff;

    return checksum;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     simple response creation
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @param[in]     uint8_t resp                         - error code
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_simple(command_parser_packet_t const *pbuf, command_parser_response_id_t resp)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = resp;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on valid command
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_ok(command_parser_packet_t const *pbuf)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on not supported command
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_not_supported(command_parser_packet_t const *pbuf)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_NOT_SUPPORTED;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on data error
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_error_data(command_parser_packet_t const *pbuf)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_DATA;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on version request
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_version(command_parser_packet_t const *pbuf)
{
    command_parser_version_t *pver;
    uint16_t                  checksum = 0;

    pgu_response_buffer.size       = sizeof(command_parser_version_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    pver = (command_parser_version_t *)&pgu_response_buffer.payload[2];

    const version_t *const      version      = get_system_version();
    const safe_version_t *const safe_version = pg_safe_func_table->fp_Get_Version_Info();

    pver->safe_major = safe_version->major;
    pver->safe_minor = safe_version->minor;
    pver->safe_patch = safe_version->patch;
    pver->safe_crc   = *((uint32_t *)(uint32_t)SAFE_CRC_ADDR);

    pver->app_major = version->major;
    pver->app_minor = version->minor;
    pver->app_patch = version->patch;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on status request
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_status(command_parser_packet_t const *pbuf)
{
    command_parser_status_t *pstat;
    uint16_t                 checksum = 0;

    pgu_response_buffer.size       = sizeof(command_parser_status_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    const app_data_t *const p_data = app_get_data();

    pstat = (command_parser_status_t *)&pgu_response_buffer.payload[2];
    memset(pstat, 0x00, sizeof(command_parser_status_t));

    pstat->target_rpm = p_data->mc_cmd.target_rpm;
    pstat->direction  = p_data->mc_cmd.direction;
    pstat->time       = p_data->mc_cmd.duration;
    pstat->ramp       = p_data->mc_cmd.ramp;
    pstat->curr_rpm   = (uint16_t)(fabsf(motor_control_get_motor_RPM()) / APP_DRUM_RPM_RATIO);
    pstat->voltage    = (uint16_t)adc_analog_get()->vdc;
    pstat->power      = (uint16_t)motor_control_get_motor_power();
    pstat->motor_temperature  = (int16_t)motor_control_get_motor_temperature();
    pstat->stator_current     = (uint16_t)(motor_control_get_phase_current() * 10.0f);
    pstat->bridge_temperature = (int16_t)adc_analog_get()->tso_igbt;

    pstat->safe.errors.safe_fsm_is_error =
        (pg_safe_func_table->fp_Get_State() == eMU_SAFE_FSM_ERROR);
    pstat->safe.errors.safe_fsm_is_fault =
        (pg_safe_func_table->fp_Get_State() == eMU_SAFE_FSM_FAULT);
    pstat->safe.errors.black_channel_not_ok =
        (pg_safe_func_table->fp_Blckch_Get_Status() != eBLACKCH_OK);
    pstat->safe.errors.current_meas_not_ok =
        (pg_safe_func_table->fp_Get_Currmeas_Status() != eCURR_MEAS_OK);
    pstat->safe.errors.otdet_not_ok = (pg_safe_func_table->fp_Get_Otdet_Status() != eOTDET_OK);

    pstat->safe.status.black_channel_timeout = pg_safe_func_table->fp_Get_Blckch_Timeout();
    pstat->safe.status.otdet_overtemp        = pg_safe_func_table->fp_Get_Otdet_Overtemp();
    pstat->safe.status.can_drive             = pg_safe_func_table->fp_Can_Drive_Motor();

    pstat->supply = mains_meas_get_powerline_freq();

    if (p_data->fan == eFAN_ENABLED)
    {
        pstat->status |= 0x40;
    }
    if (eMEASUREMENT_IDLE != measurements_get_state())
    {
        pstat->status |= 0x10;
    }
    if (p_data->mc_cmd.rpm_reached)
    {
        pstat->status |= 0x01;
    }

    pstat->state          = p_data->fsm.current_state;
    pstat->last_error     = p_data->errors;
    pstat->last_warning   = p_data->warnings;
    pstat->self_test_code = p_data->self_test;

#if defined(MOTOR_GMCC_75)
    pstat->motor_type = eMOTOR_TYPE_GMCC_75;
#elif defined(MOTOR_GMCC_66)
    pstat->motor_type = eMOTOR_TYPE_GMCC_66;
#elif defined(MOTOR_RECHI_81)
    pstat->motor_type = eMOTOR_TYPE_RECHI_81;
#else
    pstat->motor_type = 0xFF;
#endif

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     response creation on measurement status request
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_measurement(command_parser_packet_t const *pbuf)
{
    command_parser_measurement_t *const pmes =
        (command_parser_measurement_t *)&pgu_response_buffer.payload[2];
    const measurements_data_t *const p_measurements = measurements_get_data();
    uint16_t                         checksum       = 0;

    // Flush data
    memset(pmes, 0x00, sizeof(command_parser_measurement_t));

    // Prepare frame header
    pgu_response_buffer.size       = sizeof(command_parser_measurement_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID

    // Send busy if measurement is in progress
    if (measurements_is_machine_set() == false)
    {
        pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_DATA;
    }
    else if (measurements_get_state() != eMEASUREMENT_IDLE)
    {
        pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_BUSY;
    }
    else if (p_measurements->success == false)
    {
        pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_MEASURE;
    }
    else
    {
        pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;
    }

    // Populate results
    pmes->raw_inertia   = p_measurements->values.raw_inertia;
    pmes->raw_unbalance = p_measurements->values.raw_unbalance;
    pmes->raw_friction  = p_measurements->values.raw_friction;
    pmes->raw_au_ball   = p_measurements->values.raw_au_ball;
    pmes->overload      = p_measurements->values.overload;

    pmes->inertia   = p_measurements->values.inertia;
    pmes->unbalance = p_measurements->values.unbalance;
    pmes->au_ball   = p_measurements->values.au_ball;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief   Response creation on black channel request
 *
 * @param[in]    command_parser_packet_t *pbuf       - received command buffer structure
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_blackch(command_parser_packet_t *pbuf)
{
    blackch_frame_t *p_blackch_response_frame;
    blackch_frame_t *p_blackch_request_frame;

    uint16_t checksum = 0;

    pgu_response_buffer.size       = sizeof(blackch_frame_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    p_blackch_response_frame = (blackch_frame_t *)&pgu_response_buffer.payload[2];
    p_blackch_request_frame  = (blackch_frame_t *)&pbuf->payload[1];

    // pass black channel request frame to SAFE and get response frame
    if (pg_safe_func_table->fp_BlackCh_Rcv_n_Resp_Frame(
            *p_blackch_request_frame, p_blackch_response_frame
        ))
    {
        // send response if SAFE needs to send response
        checksum                    = pu_cheksum(&pgu_response_buffer);
        pgu_response_buffer.cheksum = checksum;
        pu_send_frame(&pgu_response_buffer);
    }
    else
    {
        // not rending response because black channel frame was not valid
        pu_send_error_data(&pgu_response_buffer);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief   Response creation for safe status
 *
 * @param[in] command_parser_packet_t *pbuf - received command buffer structure
 * @return    none
 */
////////////////////////////////////////////////////////////////////////////////
void proto_send_safe_status(command_parser_packet_t *pbuf)
{
    command_parser_safe_status_t *const p_safe_status_frame =
        (command_parser_safe_status_t *)&pgu_response_buffer.payload[2];

    pgu_response_buffer.size       = sizeof(command_parser_safe_status_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0];
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    // Flush data buffer
    memset(p_safe_status_frame, 0x00, sizeof(command_parser_safe_status_t));

    p_safe_status_frame->state               = pg_safe_func_table->fp_Get_State();
    p_safe_status_frame->black_channel       = pg_safe_func_table->fp_Get_Blckch_Pyld().status.byte;
    p_safe_status_frame->current_meas_status = pg_safe_func_table->fp_Get_Currmeas_Status();
    p_safe_status_frame->otdet_status        = pg_safe_func_table->fp_Get_Otdet_Status();
    p_safe_status_frame->drum_spinning_state = pg_safe_func_table->fp_Get_DrumSpin_State();

    p_safe_status_frame->status.black_channel_timeout = pg_safe_func_table->fp_Get_Blckch_Timeout();
    p_safe_status_frame->status.otdet_overtemp        = pg_safe_func_table->fp_Get_Otdet_Overtemp();
    p_safe_status_frame->status.can_drive             = pg_safe_func_table->fp_Can_Drive_Motor();

    pgu_response_buffer.cheksum = pu_cheksum(&pgu_response_buffer);
    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief   Response creation for reset status log
 *
 * @param[in] command_parser_packet_t *pbuf - received command buffer structure
 * @return    none
 */
////////////////////////////////////////////////////////////////////////////////
void proto_send_reset_status_log(command_parser_packet_t *pbuf)
{
    command_parser_rst_status_rsp_t *p_rst_status_rsp_frame =
        (command_parser_rst_status_rsp_t *)&pgu_response_buffer.payload[2];
    reset_status_log_entry_t log_entry = pg_safe_func_table->fp_Get_Reset_Status_Log_Entry(
        ((command_parser_rst_status_req_t *)&pbuf->payload[1])->idx_from_last
    );

    pgu_response_buffer.size       = sizeof(command_parser_rst_status_rsp_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0];
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    p_rst_status_rsp_frame->time_chkpt    = log_entry.time_chkpt;
    p_rst_status_rsp_frame->cause         = log_entry.info.cause;
    p_rst_status_rsp_frame->chkpt         = log_entry.info.chkpt;
    p_rst_status_rsp_frame->mu_safe_state = log_entry.info.mu_safe_state;

    pgu_response_buffer.cheksum = pu_cheksum(&pgu_response_buffer);
    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief   Response creation for get pin state
 *
 * @param[in]    command_parser_packet_t *pbuf       - received command buffer structure
 * @param[in]     uint8_t pin_state                     - state of requested pin
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_gpio_pin(command_parser_packet_t const *pbuf, uint8_t pin_state)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = 3;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;
    pgu_response_buffer.payload[2] = pin_state;

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief   Response creation for get adc measuremetn
 *
 * @param[in]    command_parser_packet_t *pbuf       - received command buffer structure
 * @param[in]     float val                            - requested adc measurement
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_send_adc_meas(command_parser_packet_t const *pbuf, float val)
{
    uint16_t checksum = 0;

    pgu_response_buffer.size       = sizeof(float) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;
    void *dest                     = &pgu_response_buffer.payload[2];
    void *src                      = &val;
    memcpy(dest, src, sizeof(float));

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

void pu_send_co_obj(command_parser_packet_t const *pbuf, CO_DATA_TYPE_T data_type, void *data)
{
    pgu_response_buffer.payload[0] = pbuf->payload[0]; // copy request ID
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    uint16_t checksum = 0;

    uint8_t data_size;
    switch (data_type)
    {
    case CO_DTYPE_U8_VAR:
    case CO_DTYPE_U8_CONST:
    case CO_DTYPE_U8_PTR:
    case CO_DTYPE_I8_VAR:
    case CO_DTYPE_I8_CONST:
    case CO_DTYPE_I8_PTR:
        data_size = 1;
        break;
    case CO_DTYPE_U16_VAR:
    case CO_DTYPE_U16_CONST:
    case CO_DTYPE_U16_PTR:
    case CO_DTYPE_I16_VAR:
    case CO_DTYPE_I16_CONST:
    case CO_DTYPE_I16_PTR:
        data_size = 2;
        break;
    case CO_DTYPE_U32_VAR:
    case CO_DTYPE_U32_CONST:
    case CO_DTYPE_U32_PTR:
    case CO_DTYPE_I32_VAR:
    case CO_DTYPE_I32_CONST:
    case CO_DTYPE_I32_PTR:
    case CO_DTYPE_R32_VAR:
    case CO_DTYPE_R32_CONST:
    case CO_DTYPE_R32_PTR:
        data_size = 4;
        break;
    default:
        data_size                      = 1;
        pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_ERROR_NOT_SUPPORTED;
        break;
    }

    pgu_response_buffer.size       = 3 + data_size;
    pgu_response_buffer.payload[2] = (uint8_t)data_type;

    memset(&pgu_response_buffer.payload[3], 0x00, data_size);
    memcpy(&pgu_response_buffer.payload[3], data, data_size);

    checksum                    = pu_cheksum(&pgu_response_buffer);
    pgu_response_buffer.cheksum = checksum;

    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * Returns configuration of machine
 *
 * @param[in] command_parser_packet_t *pbuf - received command buffer structure
 * @return    none
 */
////////////////////////////////////////////////////////////////////////////////
void proto_send_machine_settings(command_parser_packet_t *pbuf)
{
    command_parser_machine_set_t *const p_machine =
        (command_parser_machine_set_t *)&pgu_response_buffer.payload[2];

    const wm_config_t *const  machine_setting = measurements_get_machine();
    const wm_factors_t *const factors         = measurements_get_factors(machine_setting->type);

    pgu_response_buffer.size       = sizeof(command_parser_machine_set_t) + 2;
    pgu_response_buffer.payload[0] = pbuf->payload[0];
    pgu_response_buffer.payload[1] = eCOMMAND_PARSER_RESPID_OK;

    // Flush data buffer
    memset(p_machine, 0x00, sizeof(command_parser_machine_set_t));

    // Set values
    p_machine->type = machine_setting->type;

    p_machine->zero_inertia_low    = factors->inertia_low.zero_inertia;
    p_machine->zero_inertia_high   = factors->inertia_high.zero_inertia;
    // Only send zero unbalance from const, since both const and wave uses the same value
    p_machine->zero_unbalance_low  = factors->unbalance_low_const.zero_unbalance;
    p_machine->zero_unbalance_high = factors->unbalance_high_const.zero_unbalance;

    pgu_response_buffer.cheksum = pu_cheksum(&pgu_response_buffer);
    pu_send_frame(&pgu_response_buffer);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     packet size checked (based on command ID)
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         command_parser_response_id_t        - parsing status
 */
////////////////////////////////////////////////////////////////////////////////
command_parser_response_id_t pu_size_check(command_parser_packet_t const *pbuf)
{
    uint16_t expected_size = 0;
    switch (pbuf->payload[0])
    {
    case eCOMMAND_PARSER_RQID_VERSION:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_PING:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_MOTOR_CONTROL:
        expected_size = 11;
        break;
    case eCOMMAND_PARSER_RQID_START_MEASURE:
        expected_size = 9;
        break;
    case eCOMMAND_PARSER_RQID_STATUS:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_BLACK_CHANNEL:
        expected_size = sizeof(blackch_frame_t) + 1;
        break;
    case eCOMMAND_PARSER_RQID_FAN_CONTROL:
        expected_size = 2;
        break;
    case eCOMMAND_PARSER_RQID_RESTART:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_READ_MEASUREMENT:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_STANDBY:
        expected_size = 2;
        break;
    case eCOMMAND_PARSER_RQID_SET_MEAS_MACHINE:
        expected_size = 18;
        break;
    case eCOMMAND_PARSER_RQID_GET_MEAS_MACHINE:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_READ_RESET_LOG:
        expected_size = sizeof(command_parser_rst_status_req_t) + 1;
        break;
    case eCOMMAND_PARSER_RQID_RECOVER_POWERFAIL:
        expected_size = 1;
        break;

    case eCOMMAND_PARSER_RQID_SET_GPIO_PIN:
        expected_size = 4;
        break;
    case eCOMMAND_PARSER_RQID_GET_GPIO_PIN:
        expected_size = 3;
        break;
    case eCOMMAND_PARSER_RQID_GET_ADC:
        expected_size = 2;
        break;
    case eCOMMAND_PARSER_RQID_ENTER_TEST_MODE:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_SET_PWM_PINS_HIZ:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_SET_PWM_PINS_EN:
        expected_size = 1;
        break;
    case eCOMMAND_PARSER_RQID_SET_CO_OBJ:
        expected_size = 8;
        break;
    case eCOMMAND_PARSER_RQID_GET_CO_OBJ:
        expected_size = 4;
        break;
    case eCOMMAND_PARSER_RQID_SAFE_STATUS:
        expected_size = 1;
        break;

    default:
        return eCOMMAND_PARSER_RESPID_ERROR_NOT_SUPPORTED;
        break;
    }

    if (expected_size == pbuf->size)
    {
        return eCOMMAND_PARSER_RESPID_OK;
    }
    else
    {
        return eCOMMAND_PARSER_RESPID_ERROR_DATA;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     cheksum checking function
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         int32_t                              - parsing error
 */
////////////////////////////////////////////////////////////////////////////////
int32_t pu_cheksum_check(command_parser_packet_t const *pbuf)
{
    uint16_t checksum = 0;

    checksum = pu_cheksum(pbuf);

    if (checksum == pbuf->cheksum)
    {
        return (int32_t)eCOMMAND_PARSER_RESPID_OK;
    }
    else
    {
        return (int32_t)eCOMMAND_PARSER_RESPID_ERROR;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     Copy last valid command to supplied buffer
 *
 * @param[in]     command_parser_motor_control_t *preq         - buffer for command copy
 * @return         none
 */
////////////////////////////////////////////////////////////////////////////////
void pu_command_parser_last_request(command_parser_motor_control_t *preq)
{
    memcpy(preq, &g_motor_control_request, sizeof(command_parser_motor_control_t));
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     returns of current response pointer
 *
 * @param[in]     NONE
 * @return         uint8_t *
 */
////////////////////////////////////////////////////////////////////////////////
uint8_t *pu_get_response_buffer(void)
{
    return pgu_temp_buffer;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     returns current response size
 *
 * @param[in]     NONE
 * @return         uint8_t
 */
////////////////////////////////////////////////////////////////////////////////
uint8_t pu_get_response_size(void)
{
    return pgu_temp_size;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     returns current last request status
 *
 * @param[in]     NONE
 * @return         uint8_t
 */
////////////////////////////////////////////////////////////////////////////////
bool proto_is_mc_cmd_pending(void)
{
    if (g_proto_mc_cmd_pending)
    {
        g_proto_mc_cmd_pending = false;

        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief     Processing allready cheched valid command
 *
 * @param[in]     command_parser_packet_t *pbuf         - received command buffer structure
 * @return         command_parser_request_type         - parsing result
 */
////////////////////////////////////////////////////////////////////////////////
command_parser_request_type_t pu_process_command(command_parser_packet_t *pbuf)
{
    command_parser_motor_control_t *preq;
    command_parser_machine_set_t   *set_machine_req;
    command_parser_request_type_t   pu_req_type = ePU_REQUEST_NO_COMMAND;

    app_fsm_state_t fsm_state;

    switch (pbuf->payload[0])
    {
    //==================================================
    case eCOMMAND_PARSER_RQID_VERSION:
        pu_send_version(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_PING:
        pu_send_ok(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_MOTOR_CONTROL:

        preq = (command_parser_motor_control_t *)&pbuf->payload[1];

        const app_data_t *const app_data = app_get_data();

        if (app_data->errors != eAPP_ERROR_NONE)
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_ERROR);
        }
        else if (app_data->self_test != eAPP_SELFTEST_NONE)
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_SELFTEST);
        }
        else if ((true == pg_safe_func_table->fp_Can_Drive_Motor()) && (measurements_get_state() == eMEASUREMENT_IDLE))
        {
            g_motor_control_request.direction  = preq->direction;
            g_motor_control_request.ramp       = preq->ramp;
            g_motor_control_request.target_rpm = preq->target_rpm;
            g_motor_control_request.time       = preq->time;
            g_motor_control_request.timeout    = preq->timeout;
            g_motor_control_request.measure    = preq->measure;

            g_proto_mc_cmd_pending = true;

            pu_send_ok(pbuf);
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_BUSY);
        }
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_START_MEASURE:
        if (true == measurements_check_request((command_parser_start_measure_t *)&pbuf->payload[1]))
        {
            pu_send_ok(pbuf);
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_MEASURE);
        }
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_STATUS:
        // Send status packet
        pu_send_status(pbuf);

        // After, try to clear errors
        app_clear_errors();

        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_BLACK_CHANNEL:
        pu_send_blackch(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_FAN_CONTROL:
    {
        app_fsm_state_t app_state = app_fsm_get_state();

        // Only allow to turn on in following states
        if ((fan_state_t)pbuf->payload[1] == eFAN_ENABLED)
        {
            if ((app_state == eAPP_FSM_STATE_STOP) || (app_state == eAPP_FSM_STATE_RUN) ||
                (app_state == eAPP_FSM_STATE_SLOW_DOWN))
            {
                app_set_fan((fan_state_t)pbuf->payload[1]);
                pu_send_ok(pbuf);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_BUSY);
            }
        }
        // Always allow to turn off
        else if ((fan_state_t)pbuf->payload[1] == eFAN_DISABLED)
        {
            app_set_fan((fan_state_t)pbuf->payload[1]);
            pu_send_ok(pbuf);
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
        }

        pu_req_type = ePU_REQUEST_VALID_COMMAND;
    }
    break;

    //==================================================
    case eCOMMAND_PARSER_RQID_RESTART:
    {
        // Allow reset from STOP or LOW POWER STATES
        app_fsm_state_t app_state = app_fsm_get_state();

        if ((app_state == eAPP_FSM_STATE_POWERUP) || (app_state == eAPP_FSM_STATE_LOW_POWER) ||
            (app_state == eAPP_FSM_STATE_STOP || (app_state == eAPP_FSM_STATE_POWER_FAIL)))
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
            app_fsm_set_state(eAPP_FSM_STATE_RESET);
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_BUSY);
        }

        pu_req_type = ePU_REQUEST_VALID_COMMAND;
    }
    break;

    //==================================================
    case eCOMMAND_PARSER_RQID_READ_MEASUREMENT:
        pu_send_measurement(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_STANDBY:
    {
        command_standby_request_t request = (command_standby_request_t)(pbuf->payload[1]);

        app_fsm_state_t app_state    = app_fsm_get_state();
        mu_safe_fsm_t   safety_state = pg_safe_func_table->fp_Get_State();

        if (request == eSTANBY_REQUEST_ENTER)
        {
            // If we are already in low power, notify back
            if (app_state == eAPP_FSM_STATE_LOW_POWER)
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
            }
            // Request can only be accepted in some of the states of application and safety
            else if ((app_state == eAPP_FSM_STATE_STOP) || (app_state == eAPP_FSM_STATE_POWERUP))
            {
                switch (safety_state)
                {
                case eMU_SAFE_FSM_NORMAL:
                case eMU_SAFE_FSM_SETUP:
                case eMU_SAFE_FSM_ERROR:
                case eMU_SAFE_FSM_FAULT:
                case eMU_SAFE_FSM_ACTIVE:
                case eMU_SAFE_FSM_STANDBY:
                case eMU_SAFE_FSM_STOP:
                {
                    // send request to safety to enter low power mode
                    pg_safe_func_table->fp_Set_Power_Mode(ePOWER_MODE_LOW);

                    pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_BUSY);
                }
                break;

                default:
                {
                    // Request denied, since only previous state are allowed to enter to low power
                    pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR);
                }
                }
            }
            // Otherwise request is denied since no valid state
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR);
            }
        }
        else if (request == eSTANBY_REQUEST_EXIT)
        {
            switch (app_state)
            {
            // Exit is only possible if we are now in low power state
            case eAPP_FSM_STATE_LOW_POWER:
            case eAPP_FSM_STATE_POWERUP:
            case eAPP_FSM_STATE_STOP:
            {
                if (app_state == eAPP_FSM_STATE_LOW_POWER)
                {
                    app_fsm_set_state(eAPP_FSM_STATE_POWERUP);
                }

                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
            }
            break;

            default:
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR);
            }
            }
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
        }
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
    }
    break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SET_MEAS_MACHINE:

        set_machine_req = (command_parser_machine_set_t *)&pbuf->payload[1];

        if (true == measurements_set_machine(set_machine_req))
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
        }
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_GET_MEAS_MACHINE:
        proto_send_machine_settings(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_READ_RESET_LOG:
        proto_send_reset_status_log(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_RECOVER_POWERFAIL:

        fsm_state = app_fsm_get_state();

        if (fsm_state == eAPP_FSM_STATE_POWER_FAIL)
        {
            app_event_set(eAPP_EVT_EXIT_PWR_FAIL);

            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR);
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }

        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SET_GPIO_PIN:
        // Command only supported in test mode
        if (g_test_mode)
        {
            // Verify validity of command
            if ((pbuf->payload[1] < eGPIO_PORT_NUM) && (pbuf->payload[2] < 16))
            {
                // Get port and pin from payload
                GPIO_TypeDef *port = (GPIO_TypeDef *)g_gpio_port_table[pbuf->payload[1]];
                uint16_t      pin  = 1 << pbuf->payload[2];

                // Set or reset pin
                if (pbuf->payload[3])
                {
                    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
                }
                else
                {
                    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
                }

                uint8_t pin_state = (uint8_t)HAL_GPIO_ReadPin(port, pin);

                pu_send_gpio_pin(pbuf, pin_state);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
            }
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_GET_GPIO_PIN:
        // Command only supported in test mode
        if (g_test_mode)
        {
            // Verify validity of command
            if ((pbuf->payload[1] < eGPIO_PORT_NUM) && (pbuf->payload[2] < 16))
            {
                // Get port and pin from payload
                GPIO_TypeDef *port = (GPIO_TypeDef *)g_gpio_port_table[pbuf->payload[1]];
                uint16_t      pin  = 1 << pbuf->payload[2];

                // Get desired pin state
                uint8_t pin_state = (uint8_t)HAL_GPIO_ReadPin(port, pin);

                pu_send_gpio_pin(pbuf, pin_state);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
            }
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_GET_ADC:
        // Command only supported in test mode
        if (g_test_mode)
        {
            adc_measurements_t adc_meas = (adc_measurements_t)pbuf->payload[1];

            // Verify validity of command
            if (adc_meas < eADC_MEAS_NUM)
            {
                float val;

                if (adc_meas < eADC_MEAS_UDC)
                {
                    curr_meas_phases_t phases = pg_safe_func_table->fp_Get_Curr_Meas();

                    switch (adc_meas)
                    {
                    case eADC_MEAS_CURR_A:
                        val = phases.phase[eCURR_MEAS_PHASE_U];
                        break;
                    case eADC_MEAS_CURR_B:
                        val = phases.phase[eCURR_MEAS_PHASE_V];
                        break;
                    case eADC_MEAS_CURR_C:
                        val = phases.phase[eCURR_MEAS_PHASE_W];
                        break;
                    default:
                        // Never reaches this code
                        val = 0.0f;
                        break;
                    }
                }
                else if (adc_meas == eADC_MEAS_UDC)
                {
                    val = pg_safe_func_table->fp_Get_VDC();
                }
                else
                {
                    otdet_temps_t temperatures = pg_safe_func_table->fp_Get_Otdet();

                    if (adc_meas == eADC_MEAS_CPU_TEMP)
                    {
                        val = (float)temperatures.sensor[eOTDET_TEMP_CH_MCU_TEMP];
                    }
                    else // eADC_MEAS_IGBT_TEMP
                    {
                        val = (float)temperatures.sensor[eOTDET_TEMP_CH_TSO_IGBT];
                    }
                }

                pu_send_adc_meas(pbuf, val);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
            }
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_ENTER_TEST_MODE:
        // Can only be set in the first 2 seconds
        if (app_can_enter_test_mode())
        {
            g_test_mode = 1;

            pg_safe_func_table->fp_Enter_Test_Mode();

            pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);

            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SET_CO_OBJ:
    {
        // Command only supported in test mode
        if (g_test_mode)
        {
            // Get index, sub-index and value
            uint16_t index     = (uint16_t)pbuf->payload[1] + (((uint16_t)pbuf->payload[2]) << 8);
            uint8_t  sub_index = pbuf->payload[3];
            uint32_t val;
            memcpy(&val, &pbuf->payload[4], 4);

            // Set canopen object
            RET_T ret_val = co_set_obj_data(index, sub_index, (void *)&val);

            if (ret_val == RET_OK)
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_OK);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
            }
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
    }
    break;

    //==================================================
    case eCOMMAND_PARSER_RQID_GET_CO_OBJ:
    {
        // Command only supported in test mode
        if (g_test_mode)
        {
            // Get index and sub-index
            uint16_t index     = (uint16_t)pbuf->payload[1] + (((uint16_t)pbuf->payload[2]) << 8);
            uint8_t  sub_index = pbuf->payload[3];

            // Return pointers
            CO_DATA_TYPE_T data_type;
            uint32_t       data;

            RET_T ret_val = co_get_obj_data(index, sub_index, &data_type, &data);

            if (ret_val == RET_OK)
            {
                // Send data
                pu_send_co_obj(pbuf, data_type, &data);
            }
            else
            {
                pu_send_simple(pbuf, eCOMMAND_PARSER_RESPID_ERROR_DATA);
            }
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
    }
    break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SET_PWM_PINS_EN:

        if (g_test_mode)
        {
            GPIOA->MODER |= (GPIO_MODE_AF_PP
                             << GPIO_MODER_MODE8_Pos) /* parasoft-suppress MISRAC2012-RULE_12_2-a
                                                         "Value is macro expansion that is < 32" */
                            | (GPIO_MODE_AF_PP << GPIO_MODER_MODE9_Pos
                              ) /* parasoft-suppress MISRAC2012-RULE_12_2-a "Value is macro
                                   expansion that is < 32" */
                            | (GPIO_MODE_AF_PP << GPIO_MODER_MODE10_Pos
                              ) /* parasoft-suppress MISRAC2012-RULE_12_2-a "Value is macro
                                   expansion that is < 32" */
                            | (GPIO_MODE_AF_PP << GPIO_MODER_MODE12_Pos
                              ); /* parasoft-suppress MISRAC2012-RULE_12_2-a "Value is macro
                                    expansion that is < 32" */
            GPIOB->MODER |= ( GPIO_MODE_AF_PP << GPIO_MODER_MODE15_Pos ); /* parasoft-suppress MISRAC2012-RULE_12_2-a "Value is macro expansion that is < 32" */ /* parasoft-suppress MISRAC2012-RULE_12_4-b "Just a register, signedness doesn't matter" */
            GPIOC->MODER |=
                (GPIO_MODE_AF_PP << GPIO_MODER_MODE13_Pos
                ); /* parasoft-suppress MISRAC2012-RULE_12_2-a "Value is macro expansion that is <
                      32" */

            pu_send_ok(pbuf);
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }

        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SET_PWM_PINS_HIZ:
        if (g_test_mode)
        {
            // set pins to inputs, effectively HI-Z
            GPIOA->MODER &=
                ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk |
                  GPIO_MODER_MODE12_Msk);
            GPIOB->MODER &= ~(GPIO_MODER_MODE15_Msk);
            GPIOC->MODER &= ~(GPIO_MODER_MODE13_Msk);

            pu_send_ok(pbuf);
            pu_req_type = ePU_REQUEST_VALID_COMMAND;
        }
        else
        {
            pu_req_type = ePU_REQUEST_NO_COMMAND;
        }
        break;

    //==================================================
    case eCOMMAND_PARSER_RQID_SAFE_STATUS:
        proto_send_safe_status(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;

    //==================================================
    default:
        pu_send_not_supported(pbuf);
        pu_req_type = ePU_REQUEST_VALID_COMMAND;
        break;
    }

    return pu_req_type;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Parser for commands received via CPR protocol
 *
 * @param[in]    uint8_t * - pointer to received data
 * @param[in]    uint16_t  - length of data received
 */
////////////////////////////////////////////////////////////////////////////////
void pu_parse_packet(uint8_t const *data, uint16_t len)
{
    pgu_parser_buffer.cheksum = 0;
    pgu_parser_buffer.size    = 0;

    // Check start byte
    if (data[0] != PU_START_BYTE)
    {
        return;
    }

    // Copy data payload, after size and before checksum into payload array
    memcpy(pgu_parser_buffer.payload, (void *)(data + 3), (size_t)(len - 3));

    // Check if packet size is as expected
    pgu_parser_buffer.size = data[1] + ((uint16_t)data[2] << 8);

    command_parser_response_id_t check_size_result = pu_size_check(&pgu_parser_buffer);

    if (eCOMMAND_PARSER_RESPID_ERROR_DATA == check_size_result)
    {
        pu_send_error_data(&pgu_parser_buffer);
        return;
    }
    if (eCOMMAND_PARSER_RESPID_ERROR_NOT_SUPPORTED == check_size_result)
    {
        pu_send_not_supported(&pgu_parser_buffer);
        return;
    }

    // Check checksum
    pgu_parser_buffer.cheksum = data[len - 2] + ((uint16_t)data[len - 1] << 8);
    uint16_t       checksum   = 0;
    const uint16_t temp_len   = len - 2;
    for (uint16_t i = 0; i < temp_len; i++)
    {
        checksum += data[i];
    }

    if (pgu_parser_buffer.cheksum == checksum)
    {
        if (ePU_REQUEST_NO_COMMAND == pu_process_command(&pgu_parser_buffer))
        {
            pu_send_not_supported(&pgu_parser_buffer);
        }
        else
        {
            // Store timestamp of last valid packet
            g_last_packet_timestamp = HAL_GetTick();
        }
    }
    else
    {
        pu_send_error_data(&pgu_parser_buffer);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Returns status of parser timeout check
 *
 *@return    bool - true if communication has timeouted
 */
////////////////////////////////////////////////////////////////////////////////
bool pu_parser_is_timeout(void)
{
    if ((HAL_GetTick() - g_last_packet_timestamp) > PARSER_TIMEOUT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
