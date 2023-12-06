/******************************************************************************
 *
 * @file        mat_requests.h
 * @brief       MAT communication for project TD-R290
 * @author      Erik Juvan
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

#ifndef _MAT_REQUESTS_H_
#define _MAT_REQUESTS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hisense_proto.h"
#include <stdint.h>

void mat_request_get_status(uint8_t *data);
void mat_request_get_versions(uint8_t *data);
void mat_request_get_production_info(uint8_t *data);
void mat_request_get_gpio(uint8_t *data);
void mat_request_get_ntc(uint8_t *data);
void mat_request_get_supply_voltage(uint8_t *data);
void mat_request_get_zero_cross_freq(uint8_t *data);
void mat_request_get_uart_test(uint8_t *data);
void mat_request_get_value_in_ram(uint8_t *data);
void mat_request_get_call_address(uint8_t *data);

void mat_request_set_status(uint8_t *data);
void mat_request_set_production_info(uint8_t *data);
void mat_request_set_reset(uint8_t *data);
void mat_request_set_sleep(uint8_t *data);
void mat_request_set_wakeup(uint8_t *data);
void mat_request_set_motor(uint8_t *data);
void mat_request_set_gpio(uint8_t *data);
void mat_request_set_inject_ntc(uint8_t *data);
void mat_request_set_value_in_ram(uint8_t *data);
void mat_request_set_call_address(uint8_t *data);

// Application functions
bool mat_request_get_mc_request(command_parser_motor_control_t *req_buf);

#ifdef __cplusplus
}
#endif

#endif // _MAT_REQUESTS_H_
