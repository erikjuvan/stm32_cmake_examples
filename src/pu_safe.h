/**
 * @file     pu_safe.h
 * @brief    Main PU SAFE module header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup PU_SAFE_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _PU_SAFE_H_
#define _PU_SAFE_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void     pu_safe_init                      (void);
void     pu_safe_proc                      (void);
void     pu_safe_req_lock                  (void);
void     pu_safe_req_unlock                (void);
void     pu_safe_req_clear                 (void);
bool     pu_safe_standby                   (void);
void     pu_safe_wakeup                    (void);
uint32_t pu_safe_get_ui_start_remaining_ms (void);

#ifdef __cplusplus
}
#endif

#endif // _PU_SAFE_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
