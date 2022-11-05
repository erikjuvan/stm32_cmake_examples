#ifndef _SAFE_INFO_COMMON_H_
#define _SAFE_INFO_COMMON_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    bool power_relay_closed;
    bool door_relay_closed;

    bool emrgcy_drain_active;

    struct {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    } version;

    uint32_t crc32;

} safe_info_t;

#endif
