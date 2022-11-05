/***********************************************************************************************************************
 *
 * @file        trace.c
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
 *  Miha Breznik        2021-10-19      1.3.0           Change to _printf
 *  Miha Breznik        2021-11-18      1.4.0           Change types
 *  Blaz Baskovc        2021-11-20      1.4.1           Added macro for printf to use _printf.
 *  Blaz Baskovc        2021-11-20      1.4.2           Bugfix: Multi-line macro error.
 *  Blaz Baskovc        2021-11-24      1.5.0           Added force print if trace is disabled and added configuration
 *                                                      to disable printing trace type.
 *  Miha Breznik        2022-01-03      1.5.2           Fixed #define snprintf typo
 *  Blaz Baskovc        2022-01-11      1.6.0           Added support to use stdio library.
 *  Blaz Baskovc        2022-01-13      1.6.1           Fixed vsnprintf input parameters syntax.
 *  Blaz Baskovc        2022-07-04      1.6.2           Added support to print time in ms only.
 *
 **********************************************************************************************************************/

/* Includes -------------------------------------------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>	
#include <stdarg.h>
#include "trace.h"

#if( CONF_TRACE_USE_STDIO==1 )
#include <stdio.h>
#else
#include "utils.h"
#endif

/* Private Macros --------------------------------------------------------------------------------------------------- */
/**
 * Use UTILS or stdio for printing
 */
#if( CONF_TRACE_USE_STDIO==1 )
    #define _printf(fmt, ...)                   printf(fmt, ## __VA_ARGS__)
    #define _vprintf(fmt, ...)                  vprintf(fmt, ## __VA_ARGS__)
#else
    #define _printf(fmt, ...)                   UTILS_printf(fmt, ## __VA_ARGS__)
    #define _vprintf(fmt, ...)                  UTILS_vprintf(fmt, ## __VA_ARGS__)
#endif

/* Private Variables, ... ------------------------------------------------------------------------------------------- */
static bool flgInitialized_ = false;
static bool flg_force_print = false;

/* Public Functions ------------------------------------------------------------------------------------------------- */

void TRACE_Init(void)
{
	/* Initialized flag indication */
	flgInitialized_ = true;

	/* Print blank line. */
	TRACE_Debug(0, TRACE_OUTPUT_TYPE_WITHOUT_HEAD, NULL, 0, "");
}

void TRACE_Debug(trace_level_t level, trace_output_type_t type, const char *file, uint32_t line, const char *fmt, ...)
{
	if (flgInitialized_ == false)
	{
		return;
	}

	// Do not print anything if trace is not enabled!
	if( flg_force_print == false )
	{
        #ifndef TRACE_ENABLED
        return;
        #endif
	}
	else
	{
	    // Flag needs to be set every time!
	    flg_force_print = false;
	}

	// Print trace head
	if (type != TRACE_OUTPUT_TYPE_WITHOUT_HEAD && type != TRACE_OUTPUT_TYPE_WITHOUT_HEAD_AND_NL)
	{
		// Time stamp
		#ifdef TRACE_TIMESTAMP_ENABLED
            #ifdef TRACE_TIMESTAMP_MS
            _printf("%lu ", TRACE_GetSysTimestampMs());
            #else
            _printf("%lu.%03d ", TRACE_GetSysTimestamp(), (TRACE_GetSysTimestampMs() % 1000));
            #endif // TRACE_TIMESTAMP_MS
		#endif // TRACE_TIMESTAMP_ENABLED

		// Trace level
        #ifndef TRACE_LEVEL_DISABLE
		switch (level)
		{
		case TRACE_LEVEL_FATAL:
			_printf("[FAT]");
			break;
		case TRACE_LEVEL_ERROR:
			_printf("[ERR]");
			break;
		case TRACE_LEVEL_WARNING:
			_printf("[WAR]");
			break;
		case TRACE_LEVEL_TRACE:
			_printf("[TRA]");
			break;
		case TRACE_LEVEL_DEBUG:
			_printf("[DEB]");
			break;
		case TRACE_LEVEL_INFO:
			_printf("[INF]");
			break;
		default:
			break;
		}
        #endif // TRACE_LEVEL_DISABLE

		// Print file and line
		if (file != NULL)
		{
			_printf("[%s : %lu]", file, line);
		}

		/* Message start */
		_printf(": ");
	}

	// Message print
	va_list args;
	va_start(args, fmt);
	_vprintf((char*)fmt, args);
	va_end(args);

	// New line print
	if (type != TRACE_OUTPUT_TYPE_WITHOUT_NL && type != TRACE_OUTPUT_TYPE_WITHOUT_HEAD_AND_NL)
	{
		_printf("\r\n");
	}
}

void TRACE_force_print(void)
{
    flg_force_print = true;
}

/* EOF -------------------------------------------------------------------------------------------------------------- */
