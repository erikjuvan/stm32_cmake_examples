/**
 * @file     project_conf.h
 * @brief    Project configuration file
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup PROJECT_CONF_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _PROJECT_CONF_H_
#define _PROJECT_CONF_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <cmsis_gcc.h>
#include "pu_safe_version_common.h"

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#define CONF_VERSION_MAJOR               PU_SAFE_VERSION_MAJOR      /**< [uint16_t] SW version - Major */
#define CONF_VERSION_MINOR               PU_SAFE_VERSION_MINOR      /**< [uint16_t] SW version - Minor */
#define CONF_VERSION_PATCH               PU_SAFE_VERSION_PATCH      /**< [uint16_t] SW version - Patch */
// CRC
#define CONF_VERSION_CRC32               ((uint32_t)0xFFFFFFFF)  /**< [uint16_t] CRC */

#ifdef  DEBUG
#define ASSERT(x) do { if (!(x)) __BKPT(0); } while (0)
#else
#define ASSERT(x)
#endif

/**
 * code from linux/build_bug.h ( #define BUILD_BUG_ON_ZERO(e) ((int)(sizeof(struct { int:(-!!(e)); }))) )
 */
#define STATIC_ASSERT(x) ((void)sizeof(char[1 - 2*!(x)]))

#ifdef LOG_VIA_UART

/**
 * Module level debug output control
 */
#define CONF_EPS_LOG                      1           /**< [bool] EPS Disable/Enable debug output. */
#define CONF_LOCK_LOG                     1           /**< [bool] LOCK Disable/Enable debug output. */
#define CONF_DRAIN_LOG                    1           /**< [bool] DRAIN Disable/Enable debug output. */
#define CONF_VALVE_LOG                    1           /**< [bool] VALVE Disable/Enable debug output. */
#define CONF_PU_SAFE_LOG                  1           /**< [bool] PU_SAFE Disable/Enable debug output. */
#define CONF_EMRGCY_DRAIN_LOG             1           /**< [bool] ED Disable/Enable debug output. */

/**
 * TRACE specific macros
 */
#define CONF_TRACE_ENABLE                   1           /**< To enable trace output set to 1. */
#define CONF_TRACE_ADVANCED                 0           /**< Set to 1 to get file + line in message header. */
#define CONF_TRACE_TIMESTAMP_ENABLE         1           /**< To enable timestamp with log. */
#define CONF_TRACE_USE_STDIO                1           /**< Use C stdio library for printf. */
#define CONF_TRACE_TIMESTAMP_MS             1           /**< Print timestamp as ms tick. */

#endif // LOG_VIA_UART

/* Trace workaround for release builds to not try and include utils.h */
#define CONF_TRACE_USE_STDIO                1           /**< Use C stdio library for printf instead of UTILS_printf. */

#endif // _PROJECT_CONF_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
