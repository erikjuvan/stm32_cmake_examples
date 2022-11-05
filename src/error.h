/**
 * @file     error.h
 * @brief    Error handler that forwards error type to USER header
 * @author   Erik Juvan
 * @date     13.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup ERROR_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _ERROR_H_
#define _ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "error_common.h"


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void error_handler(safe_error_t error);


#ifdef __cplusplus
extern "C" {
#endif

#endif // _ERROR_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
