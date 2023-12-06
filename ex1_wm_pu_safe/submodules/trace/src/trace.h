/***********************************************************************************************************************
 *
 * @file        trace.h
 * @brief       Trace / Debug project code. Component use printf functionality and it needs to be used carefully.
 * @author      Blaz Baskovc
 * @copyright   Copyright 2021 Gorenje d.d.
 * @date        2021-05-03
 * @version     1.6.2
 * @details     Information about revisions history:
 *
 *  Author              Date            Version         Comments on that revision
 * --------------------------------------------------------------------------------------------------------------------
 *  Blaz Baskovc        03. 05. 2021    1.0.0           Generated file
 *  Blaz Baskovc        12. 05. 2021    1.1.0           Output printf without waiting for \r\n.
 *  Blaz Baskovc        2021-11-11      1.1.1           Fixed bug with trace output if trace module is disabled in config.
 *  Miha Breznik        2021-10-07      1.2.0           Added milliseconds to timer
 *  Miha Breznik        2021-10-19      1.3.0           Change to UTILS_printf
 *  Miha Breznik        2021-11-18      1.4.0           Change types
 *  Blaz Baskovc        2021-11-20      1.4.1           Added macro for printf to use UTILS_printf.
 *  Blaz Baskovc        2021-11-20      1.5.0           Added force print if trace is disabled and added configuration
 *                                                      to disable printing trace type.
 *  Miha Breznik        2022-01-03      1.5.2           Fixed #define snprintf typo
 *  Blaz Baskovc        2022-01-11      1.6.0           Added support to use stdio library.
 *  Blaz Baskovc        2022-01-13      1.6.1           Fixed vsnprintf input parameters syntax.
 *  Blaz Baskovc        2022-07-04      1.6.2           Added support to print time in ms only.
 *
 *********************************************************************************************************************/

#ifndef _TRACE_H_
#define _TRACE_H_

/* Includes --------------------------------------------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "project_conf.h"

#if( CONF_TRACE_USE_STDIO==1 )
#include <stdio.h>
#else
#include "utils.h"
#endif

/* Global Defines -------------------------------------------------------------------------------------------------- */

/* @brief   Component information */
#define TRACE_VERSION                       "1.6.2"     // Module version.
#define TRACE_NAME                          "TRACE"     // Module name.
#define TRACE_NAME_SHORT                    "TRA"       // Module short name.

/* @brief   Enable time stamp print in debug output. */
#if (CONF_TRACE_TIMESTAMP_ENABLE == 1)
    #define TRACE_TIMESTAMP_ENABLED
#endif

/* @brief   Use ms time stamp. */
#if (CONF_TRACE_TIMESTAMP_MS==1)
    #define TRACE_TIMESTAMP_MS
#endif 

/* @brief   Enable advanced debug print. */
#if (CONF_TRACE_ADVANCED==1)
#define TRACE_ADVANCED
#endif

/* @brief   Enable debug output. */
#if (CONF_TRACE_ENABLE==1)
    #define TRACE_ENABLED
#endif

/* @brief   Disable debug type print. */
#if (CONF_TRACE_LEVEL_DISABLE==1)
#define TRACE_LEVEL_DISABLE
#endif

/* Global types ----------------------------------------------------------------------------------------------------- */

/* @brief   Trace types. Each type has different output. */
typedef enum
{
  TRACE_LEVEL_FATAL,
  TRACE_LEVEL_ERROR,
  TRACE_LEVEL_WARNING,
  TRACE_LEVEL_TRACE,
  TRACE_LEVEL_DEBUG,
  TRACE_LEVEL_INFO
} trace_level_t;

/* @brief   Trace types. Each type has different output. */
typedef enum
{
  TRACE_OUTPUT_TYPE_NORMAL,
  TRACE_OUTPUT_TYPE_WITHOUT_NL,
  TRACE_OUTPUT_TYPE_WITHOUT_HEAD,
  TRACE_OUTPUT_TYPE_WITHOUT_HEAD_AND_NL
} trace_output_type_t;

/* Global Macros ---------------------------------------------------------------------------------------------------- */
/**
 * Trace debug output macros for easy usage.
 */
#ifdef TRACE_ENABLED
    #ifdef TRACE_ADVANCED
        #define TRACE_FAT(fmt, ...)     TRACE_Debug(TRACE_LEVEL_FATAL, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
        #define TRACE_ERR(fmt, ...)     TRACE_Debug(TRACE_LEVEL_ERROR, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
        #define TRACE_WAR(fmt, ...)     TRACE_Debug(TRACE_LEVEL_WARNING, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
        #define TRACE_TRA(fmt, ...)     TRACE_Debug(TRACE_LEVEL_TRACE, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
        #define TRACE_DEB(fmt, ...)     TRACE_Debug(TRACE_LEVEL_DEBUG, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
        #define TRACE_INF(fmt, ...)     TRACE_Debug(TRACE_LEVEL_INFO, TRACE_OUTPUT_TYPE_NORMAL, __FILE__, __LINE__,fmt, ## __VA_ARGS__)
    #else
        #define TRACE_FAT(fmt, ...)     TRACE_Debug(TRACE_LEVEL_FATAL, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
        #define TRACE_ERR(fmt, ...)     TRACE_Debug(TRACE_LEVEL_ERROR, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
        #define TRACE_WAR(fmt, ...)     TRACE_Debug(TRACE_LEVEL_WARNING, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
        #define TRACE_TRA(fmt, ...)     TRACE_Debug(TRACE_LEVEL_TRACE, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
        #define TRACE_DEB(fmt, ...)     TRACE_Debug(TRACE_LEVEL_DEBUG, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
        #define TRACE_INF(fmt, ...)     TRACE_Debug(TRACE_LEVEL_INFO, TRACE_OUTPUT_TYPE_NORMAL, NULL, 0,  fmt, ## __VA_ARGS__)
    #endif // TRACE_ADVANCED
        #define TRACE_EOL()             TRACE_Debug(TRACE_LEVEL_DEBUG, TRACE_OUTPUT_TYPE_WITHOUT_HEAD, NULL, 0, "");
#else
    #define TRACE_FAT(fmt, ...)
    #define TRACE_ERR(fmt, ...)
    #define TRACE_WAR(fmt, ...)
    #define TRACE_TRA(fmt, ...)
    #define TRACE_DEB(fmt, ...)
    #define TRACE_INF(fmt, ...)
    #define TRACE_EOL()
#endif // TRACE_ENABLED

/**
 * Use UTILS library for printing not STDIO.
 */
#if( CONF_TRACE_USE_STDIO==0 )
#define printf(fmt, ...)                        UTILS_printf(fmt, ## __VA_ARGS__)
#define snprintf(buff, buff_size, fmt, ...)     UTILS_snprintf(buff, buff_size, fmt, ## __VA_ARGS__)
#define vsnprintf(buff, buff_size, fmt, args)   UTILS_vsnprintf(buff, buff_size, fmt, args )
#define vprintf(fmt, ...)                       UTILS_vprintf(fmt, ## __VA_ARGS__)
#endif

/**
 * @brief   Macro to easy force printing when trace is disabled
 */
#ifdef TRACE_ADVANCED
    #define TRACE_debug_force(level, type, fmt, ...)                                                                \
        TRACE_force_print();                                                                                        \
        TRACE_Debug(level, type, __FILE__, __LINE__, fmt, ## __VA_ARGS__);
#else
    #define TRACE_debug_force(level, type, fmt, ...)                                                                \
        TRACE_force_print();                                                                                        \
        TRACE_Debug(level, type, NULL, 0, fmt, ## __VA_ARGS__);
#endif

/* Public Function Prototypes --------------------------------------------------------------------------------------- */

/*
 * @brief   Initialize trace functionality.
 * @return  void
 */
void TRACE_Init(void);

/*
 * @brief   Clear WDT timer. Call it in main loop.
 * @param   trace_level_t, Trace level.
 * @param   trace_output_type_t, Trace output type.
 * @param   const char*, File name.
 * @param   const char*, Line in file.
 * @param   ...,
 * @return  void
 */
void TRACE_Debug(trace_level_t level, trace_output_type_t type, const char * file, uint32_t line, const char * fmt, ...);

/**
 * @brief   Force trace print even if trace is disabled. Call before TRACE_Debug()!
 */
void TRACE_force_print(void);

/*
 * @brief   Read system time (epoch). Implement function in application.
 * @return  uint32_t, System time stamp in seconds.
 */
uint32_t TRACE_GetSysTimestamp(void);

/*
 * @brief   Read system miliseconds timestamp. Implement function in application.
 * @return  uint32_t, System time stamp in milliseconds.
 */
uint32_t TRACE_GetSysTimestampMs(void);

#endif // _WDT_H_

/* EOF -------------------------------------------------------------------------------------------------------------- */
