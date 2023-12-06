#include "mat_trace.h"
#include "Communication/wifi.h"
#include "project_cfg.h" // for FW_UPDATE_BIGDATA_MAX_DATA_SIZE
#include "stm32g4xx_hal.h"
#include "types.h"
#include <stdbool.h>
#include <string.h>

#define HISENSE_TYPE_TRACE              0x68
#define HISENSE_BROADCAST_ADDRESSCODE_1 0xFF
#define HISENSE_BROADCAST_ADDRESSCODE_2 0x00
#define MIN_SEND_SIZE                   (FW_UPDATE_BIGDATA_MAX_DATA_SIZE / 2)
#define MAX_SEND_SIZE                   FW_UPDATE_BIGDATA_MAX_DATA_SIZE

/* -------------------------------------- */
/* Private Circular buffer implementation */
/* -------------------------------------- */
#define CIRC_BUF_SIZE                   1024

typedef struct
{
    uint8_t  buf[CIRC_BUF_SIZE];
    uint16_t head;     /**< Index in buffer where next data gets stored to. */
    uint16_t tail;     /**< Index in buffer where next data gets read from. */
    uint16_t count;    /**< Number of elements currently stored. */
    uint16_t capacity; /**< Number of elements buffer can hold. */
} cb_t;

static cb_t _cb = {.buf = {0}, .head = 0, .tail = 0, .count = 0, .capacity = sizeof(_cb.buf) - 1};

static struct
{
    uint32_t traces_dropped;
    uint32_t traces_sent;
} mat_trace_debug = {0};

static inline bool cb_is_full(cb_t *cb)
{
    return cb->count == cb->capacity;
}

static void cb_init(cb_t *cb)
{
    cb->head     = 0;
    cb->tail     = 0;
    cb->count    = 0;
    cb->capacity = sizeof(cb->buf) - 1;
}

static inline void cb_reset(cb_t *cb)
{
    cb->head  = 0;
    cb->tail  = 0;
    cb->count = 0;
}

static inline uint16_t cb_available(cb_t *cb)
{
    return cb->capacity - cb->count;
}

static inline uint16_t cb_count(cb_t *cb)
{
    return cb->count;
}

static uint16_t cb_add(cb_t *cb, uint8_t *data, uint16_t size)
{
    if (size > cb_available(cb))
    {
        return 0;
    }

    // If buffer is empty reset head and tail so data starts from beginning
    if (cb_count(cb) == 0)
    {
        cb_reset(cb);
    }

    uint16_t space_to_end = cb->capacity - cb->head;
    // The data can be added without wrapping around.
    if (size <= space_to_end)
    {
        memcpy(&cb->buf[cb->head], data, size);
        cb->head += size;
    }
    else
    {
        memcpy(&cb->buf[cb->head], data, space_to_end);
        uint16_t remaining = size - space_to_end;
        memcpy(cb->buf, &data[space_to_end], remaining);
        cb->head = remaining;
    }
    cb->count += size;

    return size;
}

static uint16_t cb_pop(cb_t *cb, uint8_t *outbuf, uint16_t size)
{
    // Not enough data in cb
    if (cb->count < size)
    {
        return 0;
    }

    // Requested number of data not wrapped around
    uint16_t space_to_end = cb->capacity - cb->tail;
    if (size <= space_to_end)
    {
        memcpy(outbuf, &cb->buf[cb->tail], size);
        cb->tail += size;
        if (cb->tail == cb->capacity)
        {
            cb->tail = 0;
        }
    }
    else // Data wrapped around
    {
        memcpy(outbuf, &cb->buf[cb->tail], space_to_end);
        uint16_t remaining = size - space_to_end;
        memcpy(&outbuf[space_to_end], cb->buf, remaining);
        cb->tail = remaining;
    }
    cb->count -= size;

    return size;
}

/* -------------------------------------- */

static void _master_rx_callback(void *app_al_rsp_id, void *data, void *size)
{
    static int _master_rx_callback_cnt = 0;

    _master_rx_callback_cnt++;
}

static bool _master_send(uint8_t *data, int len)
{
    WIFI_MASTER_REQUEST_DST_DATA req_data = {0};
    WIFI_MASTER_REQUEST_RESULT   result;

    // Ready message packet data
    req_data.nl_AddressCode1[0]     = HISENSE_BROADCAST_ADDRESSCODE_1;
    req_data.nl_AddressCode1[1]     = HISENSE_BROADCAST_ADDRESSCODE_2;
    req_data.nl_AddressCode1_length = 2;
    req_data.al_message_type        = HISENSE_TYPE_TRACE;
    req_data.al_data                = data;
    req_data.al_data_length         = len;
    req_data.al_message_subtype     = 0x0;

    // Master request
    result = wifi_master_request(SERIALPORT_NUMBER_1, req_data, _master_rx_callback, _master_send);

    if (result != WIFI_MASTER_REQUEST_RESULT_OK)
    {
        return false;
    }

    return true;
}

static void _send_at_least(uint16_t n_bytes)
{
    static uint8_t  buf[MAX_SEND_SIZE];
    static uint16_t sending_size        = 0;
    static bool     sending_in_progress = false;

    if (!sending_in_progress)
    {
        int cbcnt = cb_count(&_cb);
        if (cbcnt >= n_bytes)
        {
            sending_size = cbcnt > MAX_SEND_SIZE ? MAX_SEND_SIZE : cbcnt;

            if (cb_pop(&_cb, buf, sending_size))
            {
                sending_in_progress = true;
            }
        }
    }

    if (sending_in_progress)
    {
        if (_master_send(buf, sending_size))
        {
            sending_in_progress = false;
        }
    }
}

// when trace-ing send to mat only if enough data, to optimize sending
void mat_trace_write(uint8_t *data, int len)
{
    if (len <= 0)
    {
        return;
    }

    if (cb_add(&_cb, data, len) == 0) // failed
    {
        mat_trace_debug.traces_dropped++;
    }
    else
    {
        mat_trace_debug.traces_sent++;
    }

    _send_at_least(MIN_SEND_SIZE);
}

// When processing send any available bytes
void mat_trace_process()
{
    _send_at_least(1);
}

void mat_trace_flush()
{
    // somehow flush all trace data if possible
    while (cb_count(&_cb) > 0)
    {
        mat_trace_process();
        hisense_app_run();
    }
}
