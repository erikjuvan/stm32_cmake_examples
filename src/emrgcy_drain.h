/**
 * @file     emrgcy_drain.h
 * @brief    Emergency drain module header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup EMRGCY_DRAIN_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _EMRGCY_DRAIN_H_
#define _EMRGCY_DRAIN_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void emrgcy_drain_proc(void);
bool emrgcy_drain_is_safe_window(void);
bool emrgcy_drain_is_active(void);
bool emrgcy_drain_something_is_going_on(void);

#endif // _EMRGCY_DRAIN_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
