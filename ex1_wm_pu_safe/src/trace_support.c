/**
 * @file     trace_support.c
 * @brief    Support functions for TRACE module
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stm32g0xx_hal.h>
#include "func_table.h"


////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern int g_user_app_init_is_done;


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Read system time in seconds, call to USER to get it.
 *
 * @return  System timestamp in seconds.
 */
uint32_t TRACE_GetSysTimestamp(void)
{
    uint32_t sec = 0;
    if (g_user_app_init_is_done &&
        pg_user_func_table->fp_TRACE_GetSysTimestamp != NULL)
    {
        sec = pg_user_func_table->fp_TRACE_GetSysTimestamp();
    }

    return sec;
}

/**
 * Read system timestamp in milliseconds, call to USER to get it.
 *
 * @return  System timestamp in milliseconds.
 */
uint32_t TRACE_GetSysTimestampMs(void)
{
    uint32_t ms = 0;
    if (g_user_app_init_is_done &&
        pg_user_func_table->fp_TRACE_GetSysTimestampMs != NULL)
    {
        ms = pg_user_func_table->fp_TRACE_GetSysTimestampMs();
    }

    return ms;
}
