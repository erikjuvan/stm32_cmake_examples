/******************************************************************************
 *
 * @file        trace.c
 * @brief       Trace module for printing out text
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

/*
 * Includes
 */
#include "trace.h"
#include "stm32g4xx_hal.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Private Macros
 */
// If using own printf implementation you can change it here
#define _printf(fmt, ...)         printf(fmt, ##__VA_ARGS__)
#define _snprintf(buf, fmt, ...)  snprintf(buf, fmt, ##__VA_ARGS__)
#define _vsnprintf(buf, fmt, ...) vsnprintf(buf, fmt, ##__VA_ARGS__)

/*
 * Private Functions
 */

static inline uint32_t TRACE_GetSysTimestampMs(void)
{
    return HAL_GetTick();
}

/*
 * Public Functions
 */

static char buffer[512];

int TRACE_Print(trace_level_t level, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uint32_t ms        = TRACE_GetSysTimestampMs();
    char    *str_level = 0;

    // Trace level
    switch (level)
    {
    case TRACE_LEVEL_FATAL:
        str_level = "[FAT] ";
        break;
    case TRACE_LEVEL_ERROR:
        str_level = "[ERR] ";
        break;
    case TRACE_LEVEL_WARNING:
        str_level = "[WAR] ";
        break;
    case TRACE_LEVEL_TRACE:
        str_level = "[TRA] ";
        break;
    case TRACE_LEVEL_DEBUG:
        str_level = "[DEB] ";
        break;
    case TRACE_LEVEL_INFO:
        str_level = "[INF] ";
        break;
    case TRACE_LEVEL_NONE:
    default:
        str_level = "";
        break;
    }

    int len  = _snprintf(buffer, sizeof(buffer), "%lu %s", ms, str_level);
    len     += _vsnprintf(buffer + len, sizeof(buffer) - len, fmt, args);
    va_end(args);

    // newline
    buffer[len++] = '\n';
    // and terminating character
    buffer[len]   = 0;

    // Write with _printf
    _printf("%s", buffer);

    return len;
}

int TRACE_PrintRaw(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = _printf(fmt, args);
    va_end(args);

    return len;
}
