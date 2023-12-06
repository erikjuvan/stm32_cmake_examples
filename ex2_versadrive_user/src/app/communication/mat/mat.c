/******************************************************************************
 *
 * @file        mat.c
 * @brief       MAT communication for project TD-R290
 * @author      Erik Juvan
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

#include "mat.h"
#include "mat_requests.h"
#include "trace/trace.h"
#include "version.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// GET
#define MAT_REQID_GET_STATUS          0x0001
#define MAT_REQID_GET_VERSIONS        0x0002
#define MAT_REQID_GET_PRODUCTION_INFO 0x0003
#define MAT_REQID_GET_GPIO            0x0004
#define MAT_REQID_GET_NTC             0x0005
#define MAT_REQID_GET_SUPPLY_VOLTAGE  0x0006
#define MAT_REQID_GET_ZERO_CROSS_FREQ 0x0007
#define MAT_REQID_GET_UART_TEST       0x0008
#define MAT_REQID_GET_VALUE_IN_RAM    0x00F1
#define MAT_REQID_GET_CALL_ADDRESS    0x00F2

// SET
#define MAT_REQID_SET_STATUS          0x0101
#define MAT_REQID_SET_PRODUCTION_INFO 0x0102
#define MAT_REQID_SET_RESET           0x0103
#define MAT_REQID_SET_SLEEP           0x0104
#define MAT_REQID_SET_WAKEUP          0x0105
#define MAT_REQID_SET_MOTOR           0x0106
#define MAT_REQID_SET_GPIO            0x0107
#define MAT_REQID_SET_INJECT_NTC      0x0108
#define MAT_REQID_SET_VALUE_IN_RAM    0x01F1
#define MAT_REQID_SET_CALL_ADDRESS    0x01F2

// MAT_REQID_GET_HW_PRINT_INFO         0xB00A
// MAT_REQID_GET_SAFE_STATUS           0xB00B
// MAT_REQID_GET_ERROR                 0xA013
// MAT_REQID_SET_ERROR                 0xA013
// MAT_REQID_SET_RESET_ALL_ERRORS      0xA013

typedef struct __attribute__((__packed__))
{
    uint16_t id;
    uint8_t *data;
    uint16_t data_length;
} mat_request_t;

typedef void (*req_fptr_t)(uint8_t *data);

typedef struct
{
    uint16_t   request_id;
    req_fptr_t fptr;
    uint16_t   data_length;
} mat_request_id_mapping;

#define DATA_LENGTH_UNKNOWN 0xFFFE

static mat_request_id_mapping _mapping_table[] = {
  // GET
    {MAT_REQID_GET_STATUS,          mat_request_get_status,          0                  },
    {MAT_REQID_GET_VERSIONS,        mat_request_get_versions,        0                  },
    {MAT_REQID_GET_PRODUCTION_INFO, mat_request_get_production_info, 0                  },
    {MAT_REQID_GET_GPIO,            mat_request_get_gpio,            0                  },
    {MAT_REQID_GET_NTC,             mat_request_get_ntc,             0                  },
    {MAT_REQID_GET_SUPPLY_VOLTAGE,  mat_request_get_supply_voltage,  0                  },
    {MAT_REQID_GET_ZERO_CROSS_FREQ, mat_request_get_zero_cross_freq, 0                  },
    {MAT_REQID_GET_UART_TEST,       mat_request_get_uart_test,       0                  },
    {MAT_REQID_GET_VALUE_IN_RAM,    mat_request_get_value_in_ram,    6                  },
    {MAT_REQID_GET_CALL_ADDRESS,    mat_request_get_call_address,    DATA_LENGTH_UNKNOWN},

 // SET
    {MAT_REQID_SET_STATUS,          mat_request_set_status,          0 /*TODO*/         },
    {MAT_REQID_SET_PRODUCTION_INFO, mat_request_set_production_info, 0 /*TODO*/         },
    {MAT_REQID_SET_RESET,           mat_request_set_reset,           0 /*TODO*/         },
    {MAT_REQID_SET_SLEEP,           mat_request_set_sleep,           0 /*TODO*/         },
    {MAT_REQID_SET_WAKEUP,          mat_request_set_wakeup,          0 /*TODO*/         },
    {MAT_REQID_SET_MOTOR,           mat_request_set_motor,           5                  },
    {MAT_REQID_SET_GPIO,            mat_request_set_gpio,            0 /*TODO*/         },
    {MAT_REQID_SET_INJECT_NTC,      mat_request_set_inject_ntc,      0 /*TODO*/         },
    {MAT_REQID_SET_VALUE_IN_RAM,    mat_request_set_value_in_ram,    DATA_LENGTH_UNKNOWN},
    {MAT_REQID_SET_CALL_ADDRESS,    mat_request_set_call_address,    DATA_LENGTH_UNKNOWN}
};

#define RESPONSE_BUFFER_SIZE 64
static uint8_t  _response_buffer[RESPONSE_BUFFER_SIZE] = {0};
static uint16_t _response_buffer_length                = 0;

static void response_buffer_clear()
{
    _response_buffer_length = 0;
}

static req_fptr_t _find_request_function(mat_request_t *request)
{
    const int mapping_table_size = sizeof(_mapping_table) / sizeof(_mapping_table[0]);
    for (int i = 0; i < mapping_table_size; ++i)
    {
        if (_mapping_table[i].request_id == request->id)
        {
            return _mapping_table[i].fptr;
        }
    }

    return NULL;
}

static bool _verify_requests_data_length(mat_request_t *request)
{
    const int mapping_table_size = sizeof(_mapping_table) / sizeof(_mapping_table[0]);
    for (int i = 0; i < mapping_table_size; ++i)
    {
        if (_mapping_table[i].request_id == request->id)
        {
            if (_mapping_table[i].data_length == request->data_length ||
                _mapping_table[i].data_length == DATA_LENGTH_UNKNOWN)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }

    return false;
}

static bool _verify_request(mat_request_t *request)
{
    // Check request pointer is not null
    if (request == NULL)
    {
        return false;
    }

    // Check valid request ID
    if (_find_request_function(request) == NULL)
    {
        TRACE_WAR("MAT : Unsupported request %d", request->id);

        return false;
    }

    // Check valid data length for the given request (without the ID)
    if (_verify_requests_data_length(request) == false)
    {
        TRACE_WAR("MAT : Invalid data length! ID:%d  len: %d", request->id, request->data_length);
        return false;
    }

    return true;
}

bool mat_handle_req(uint8_t *data, uint16_t length)
{
    mat_request_t request = {
        .id          = ((mat_request_t *)data)->id,
        .data        = &data[sizeof(request.id)],
        .data_length = length - sizeof(request.id)
    };

    bool ret = _verify_request(&request);

    if (ret)
    {
        req_fptr_t req_f = _find_request_function(&request);
        if (req_f != NULL)
        {
            // Prepare response buffer
            response_buffer_clear();
            mat_response_buffer_append(&request.id, sizeof(request.id));

            // Call request servicing function
            req_f(request.data);
        }
    }

    return ret;
}

uint8_t *mat_reponse_buffer_get()
{
    return _response_buffer;
}

uint16_t mat_reponse_buffer_length()
{
    return _response_buffer_length;
}

uint16_t mat_response_buffer_append(void *data, uint16_t length)
{
    uint16_t copy_length = _response_buffer_length + length <= sizeof(_response_buffer)
                               ? length
                               : sizeof(_response_buffer) - _response_buffer_length;

    memcpy(&_response_buffer[_response_buffer_length], data, copy_length);

    _response_buffer_length += copy_length;

    return copy_length;
}
