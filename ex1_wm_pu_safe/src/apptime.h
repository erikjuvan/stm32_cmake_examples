/**
 * @file     apptime.h
 * @brief    Application time functions
 * @author   Dusan Slavinec
 * @date     01.02.2022
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup APPTIME_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __APPTIME_H
#define __APPTIME_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "stm32g0xx_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Type for time keeping.
 */
typedef uint32_t apptime_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/**
 * Get application time in milliseconds - since boot.
 *
 * @return       Time.
 */
static inline apptime_t apptime_get_ms(void)
{
    return HAL_GetTick();
}

/**
 * Get elapsed time in milliseconds since starting time.
 *
 * @param[in]    start_t       Starting time.
 * @return       Elapsed time.
 */
static inline apptime_t apptime_get_elapsed(apptime_t start_t)
{
    return (apptime_get_ms() - start_t);
}

/**
 * Check if at least expected amount of milliseconds elapsed since starting time.
 *
 * @param[in]    start_t       Starting time.
 * @param[in]    elapsed_t     Expected elapsed time.
 * @return       True if elapsed.
 */
static inline bool apptime_is_elapsed(apptime_t start_t, apptime_t elapsed_t)
{
    return (bool)(apptime_get_elapsed(start_t) >= elapsed_t);
}

#endif // __APPTIME_H

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
