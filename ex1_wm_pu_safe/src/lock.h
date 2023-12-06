/**
 * @file     lock.h
 * @brief    Door lock module header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup LOCK_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _LOCK_H_
#define _LOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "lock_common.h"
#include "mu_safe_types.h"


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Module initialization status.
 */
typedef enum
{
    eLOCK_INIT_OK          = 0x00, /**< Initialization successful. */
    eLOCK_INIT_GPIO_FAIL   = 0x01, /**< GPIO initialization failed. */
    eLOCK_INIT_TIMER_FAIL  = 0x02, /**< Timer initialization failed. */
} lock_init_status_t;

/**
 * Relay state, based on relay_state_t from mu_common_types but extended with UNKNOWN state
 */
typedef enum
{
    eRELAY_EX_OFF     = 0,
    eRELAY_EX_ON      = 1,
    eRELAY_EX_UNKNOWN = 2
} relay_state_extended_t;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
lock_init_status_t lock_init                (void);
void               lock_proc                (relay_state_extended_t power_relay_state_ex);
void               lock_timer_isr           (void);
void               lock_response            (lock_response_t state);
void               lock_set_triac           (lock_triac_t triac);
lock_triac_t       lock_get_triac           (void);
void               lock_door_lock_state_set (door_lock_state_t state); // God awful function name, because of lock_state_t, door_lock_state_t<- should be renamed its too confusing
door_lock_state_t  lock_door_lock_state_get (void);
lock_state_t       lock_get_state           (void);
void               lock_power_up            (void);
void               lock_power_down          (void);

#ifdef __cplusplus
}
#endif

#endif // _LOCK_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
