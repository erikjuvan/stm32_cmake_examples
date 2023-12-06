/******************************************************************************
 *
 * @file        trace.c
 * @brief       Trace module for printing out text
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

#ifndef _TRACE_H_
#define _TRACE_H_

/* Global Defines
 * --------------------------------------------------------------------------------------------------
 */

/* @brief   Enable debug output. */
#define TRACE_ENABLED

/* Global types
 * -----------------------------------------------------------------------------------------------------
 */

/* @brief   Trace types. Each type has different output. */
typedef enum
{
    TRACE_LEVEL_NONE = 0,
    TRACE_LEVEL_FATAL,
    TRACE_LEVEL_ERROR,
    TRACE_LEVEL_WARNING,
    TRACE_LEVEL_TRACE,
    TRACE_LEVEL_DEBUG,
    TRACE_LEVEL_INFO
} trace_level_t;

/* Global Macros
 * ----------------------------------------------------------------------------------------------------
 */
/**
 * Trace debug output macros for easy usage.
 */
#ifdef TRACE_ENABLED
#define TRACE_RAW(fmt, ...) TRACE_PrintRaw(fmt, ##__VA_ARGS__)
#define TRACE_NON(fmt, ...) TRACE_Print(TRACE_LEVEL_NONE, fmt, ##__VA_ARGS__)
#define TRACE_FAT(fmt, ...) TRACE_Print(TRACE_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define TRACE_ERR(fmt, ...) TRACE_Print(TRACE_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define TRACE_WAR(fmt, ...) TRACE_Print(TRACE_LEVEL_WARNING, fmt, ##__VA_ARGS__)
#define TRACE_TRA(fmt, ...) TRACE_Print(TRACE_LEVEL_TRACE, fmt, ##__VA_ARGS__)
#define TRACE_DEB(fmt, ...) TRACE_Print(TRACE_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define TRACE_INF(fmt, ...) TRACE_Print(TRACE_LEVEL_INFO, fmt, ##__VA_ARGS__)
#else
#define TRACE_RAW(fmt, ...)
#define TRACE_NON(fmt, ...)
#define TRACE_FAT(fmt, ...)
#define TRACE_ERR(fmt, ...)
#define TRACE_WAR(fmt, ...)
#define TRACE_TRA(fmt, ...)
#define TRACE_DEB(fmt, ...)
#define TRACE_INF(fmt, ...)
#endif // TRACE_ENABLED

/*
 * Global Functions
 */

int TRACE_Print(trace_level_t level, const char *fmt, ...);
int TRACE_PrintRaw(const char *fmt, ...);

#endif // _TRACE_H_

/*
 * EOF
 */
