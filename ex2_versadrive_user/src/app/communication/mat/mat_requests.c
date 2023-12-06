#include <math.h>
#include <string.h>

#include "adc/adc.h"
#include "app.h"
#include "func_table.h"
#include "hisense_proto.h"
#include "mains_meas.h"
#include "mat.h"
#include "measurements.h"
#include "motor_control/motor_control/motor_control.h"
#include "relay.h"
#include "version.h"

typedef struct __attribute__((__packed__))
{
    union
    {
        struct
        {
            uint8_t compressor      : 1;
            uint8_t fan             : 1;
            uint8_t precharge_relay : 1;
            uint8_t reserved        : 5;
        };

        uint16_t actuators_u16;
    } actuators;

    uint32_t up_time;
    uint16_t app_state;
    uint16_t app_warning_codes;
    uint16_t app_error_codes;
    uint16_t class_b_state;
    uint16_t class_b_otdet_status;
    uint16_t class_b_otdet_overtemp;
    uint16_t class_b_curr_meas_status;
    uint16_t class_b_can_drive_motor;
    uint16_t mains_voltage;
    uint16_t mains_frequency;
    int16_t  ntc_comp_temperature;
    int16_t  ntc_igbt_temperature;
    int16_t  tso_igbt_temperature;
    int16_t  mcu_temp_temperature;
    int16_t  motor_temperature;
    uint16_t motor_speed;
    uint16_t motor_ramp;
    uint16_t motor_current;
    uint16_t motor_power;
} mat_status_t;

typedef struct __attribute__((__packed__))
{
    uint8_t  version[3];
    uint32_t crc;
    uint8_t  safe_version[3];
    uint32_t safe_crc;
    uint8_t  pcb_version[3];
    uint8_t  variant_code[4]; // entire appliance variant code
} mat_versions_t;

typedef struct __attribute__((__packed__))
{
    uint8_t  stop_start;
    uint16_t speed;
    uint16_t ramp;
} mat_motor_t;

typedef struct __attribute__((__packed__))
{
    uint8_t  injectType;
    uint16_t voltage;
    int16_t  temperature;
} mat_ntc_t;

void mat_request_get_status(uint8_t *data)
{
    mat_status_t status = {0};

    const app_data_t *app_data = app_get_data();

    // Actuators
    status.actuators.compressor      = motor_control_get_motor_RPM() > 1; // rpm at stop is not 0
    status.actuators.fan             = app_data->fan;
    status.actuators.precharge_relay = relay_get_state(RELAY_PRECHARGE) == RELAY_ON;

    // Up time
    status.up_time = app_data->time / 1000; // in seconds

    // App
    status.app_state         = app_data->fsm.current_state;
    status.app_warning_codes = app_data->warnings;
    status.app_error_codes   = app_data->errors;

    // Class B
    status.class_b_state            = pg_safe_func_table->fp_Get_State();
    status.class_b_otdet_status     = pg_safe_func_table->fp_Get_Otdet_Status();
    status.class_b_otdet_overtemp   = pg_safe_func_table->fp_Get_Otdet_Overtemp();
    status.class_b_curr_meas_status = pg_safe_func_table->fp_Get_Currmeas_Status();
    status.class_b_can_drive_motor  = pg_safe_func_table->fp_Can_Drive_Motor();

    // Mains (Power) supply
    status.mains_voltage   = (uint16_t)adc_analog_get()->vdc;
    status.mains_frequency = mains_meas_get_freq_hz_100();

    // Temperatures
    status.ntc_comp_temperature = (int16_t)(adc_analog_get()->ntc_comp * 100.0f);
    status.ntc_igbt_temperature = (int16_t)(adc_analog_get()->ntc_igbt * 100.0f);
    status.tso_igbt_temperature = (int16_t)(adc_analog_get()->tso_igbt * 100.0f);
    status.mcu_temp_temperature = (int16_t)(adc_analog_get()->mcu_temp * 100.0f);
    status.motor_temperature    = (int16_t)(motor_control_get_motor_temperature() * 100.0f);

    // Motor
    uint16_t speed_hz    = (uint16_t)(fabsf(motor_control_get_motor_RPM()) / APP_DRUM_RPM_RATIO);
    status.motor_speed   = speed_hz;
    uint16_t ramp_hz     = (uint16_t)(fabsf(motor_control_get_ramp()) / APP_DRUM_RPM_RATIO);
    status.motor_ramp    = ramp_hz;
    status.motor_current = (uint16_t)(motor_control_get_phase_current() * 1000.0f);
    status.motor_power   = (uint16_t)motor_control_get_motor_power();

    mat_response_buffer_append(&status, sizeof(status));
}

void mat_request_get_versions(uint8_t *data)
{
    mat_versions_t versions = {0};

    versions.version[0] = get_system_version()->major;
    versions.version[1] = get_system_version()->minor;
    versions.version[2] = get_system_version()->patch;
    // versions.crc        = get_system_crc(); TODO

    versions.safe_version[0] = pg_safe_func_table->fp_Get_Version_Info()->major;
    versions.safe_version[1] = pg_safe_func_table->fp_Get_Version_Info()->minor;
    versions.safe_version[2] = pg_safe_func_table->fp_Get_Version_Info()->patch;
    versions.safe_crc        = pg_safe_func_table->fp_Get_Crc();

    // versions.pcb_version[3]; TODO
    // versions.variant_code[4]; TODO

    mat_response_buffer_append(&versions, sizeof(versions));
}

void mat_request_get_production_info(uint8_t *data)
{
}

void mat_request_get_gpio(uint8_t *data)
{
}

void mat_request_get_ntc(uint8_t *data)
{
}

void mat_request_get_supply_voltage(uint8_t *data)
{
}

void mat_request_get_zero_cross_freq(uint8_t *data)
{
}

void mat_request_get_uart_test(uint8_t *data)
{
}

void mat_request_get_value_in_ram(uint8_t *data)
{
}

void mat_request_get_call_address(uint8_t *data)
{
}

void mat_request_set_status(uint8_t *data)
{
}

void mat_request_set_production_info(uint8_t *data)
{
}

void mat_request_set_reset(uint8_t *data)
{
    app_fsm_set_state(eAPP_FSM_STATE_RESET);
}

void mat_request_set_sleep(uint8_t *data)
{
    app_fsm_set_state(eAPP_FSM_STATE_LOW_POWER);
}

void mat_request_set_wakeup(uint8_t *data)
{
    // TODO. Currently MU can get out of low power state only with a reset.
    app_fsm_set_state(eAPP_FSM_STATE_RESET);
}

static command_parser_motor_control_t mat_motor_control_request;
static bool                           mat_motor_control_request_pending;

void mat_request_set_motor(uint8_t *data)
{
    mat_motor_t *preq = (mat_motor_t *)data;

    mat_motor_control_request.direction  = preq->stop_start;
    mat_motor_control_request.ramp       = preq->ramp;
    mat_motor_control_request.target_rpm = preq->speed;
    mat_motor_control_request.time       = 0xFFFF;
    mat_motor_control_request.timeout    = 0xFFFF;
    mat_motor_control_request.measure    = 0;

    mat_motor_control_request_pending = true;
}

void mat_request_set_gpio(uint8_t *data)
{
}

void mat_request_set_inject_ntc(uint8_t *data)
{
}

void mat_request_set_value_in_ram(uint8_t *data)
{
}

void mat_request_set_call_address(uint8_t *data)
{
}

bool mat_request_get_mc_request(command_parser_motor_control_t *req_buf)
{
    if (mat_motor_control_request_pending)
    {
        mat_motor_control_request_pending = false;
        memcpy(req_buf, &mat_motor_control_request, sizeof(command_parser_motor_control_t));
        return true;
    }

    return false;
}
