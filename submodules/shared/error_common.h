/**
* @file     error_common.h
* @brief    Common types for error handler
* @author   Erik Juvan
* @date     13.07.2022
* @version  V1.0.0
*/

#ifndef _ERROR_COMMON_H_
#define _ERROR_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Error types
 */
typedef enum
{
    eSAFE_ERROR_NONE      = 0x0,  /**< No error */
    eSAFE_ERROR_RESET     = 0x1,  /**< Error that triggers a reset */
    eSAFE_ERROR_DISABLED  = 0x2,  /**< Error that disables safety functionality (door lock), it will not reset*/
} safe_error_t;

#ifdef __cplusplus
}
#endif

#endif
