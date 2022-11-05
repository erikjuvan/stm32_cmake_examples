/**
 * @file     func_table.c
 * @brief    SAFE function table definition.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup FUNC_TABLE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "func_table.h"
#include "pu_safe.h"
#include "eps.h"
#include "lock.h"
#include "drain.h"
#include "black_channel.h"
#include "safe_info.h"
#include "ui_black_channel.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////
/**
 * Pointer to USER function pointer table.
 */
const user_func_table_t* const pg_user_func_table = \
    (const user_func_table_t* const)(&_MEMPART_FLASH_USER_FNC_TABLE_ADDR);

/**
 * SAFE function pointer table definition.
 */
static volatile const safe_func_table_t safe_func_table \
    __attribute__ ((section (".safe_ft"))) =
{
    // pu_safe (lock)
    .fp_pu_safe_req_lock                  = pu_safe_req_lock,
    .fp_pu_safe_req_unlock                = pu_safe_req_unlock,
    .fp_pu_safe_req_clear                 = pu_safe_req_clear,
    .fp_pu_safe_standby                   = pu_safe_standby,
    .fp_pu_safe_wakeup                    = pu_safe_wakeup,
    .fp_pu_safe_get_ui_start_remaining_ms = pu_safe_get_ui_start_remaining_ms,

    // info
    .fp_safe_info_get = safe_info_get,

    // eps
    .fp_eps_get_level    = eps_get_level,
    .fp_eps_get_period   = eps_get_period,
    .fp_eps_get_isr_cnt  = eps_get_isr_cnt,
    .fp_eps_is_plausible = eps_is_plausible,
    .fp_eps_is_error     = eps_is_error,
    .fp_eps_calibration_timer_freq_set        = eps_calibration_timer_freq_set,
    .fp_eps_calibration_timer_freq_reset      = eps_calibration_timer_freq_reset,
    .fp_eps_calibration_eps_freq_offset_set   = eps_calibration_eps_freq_offset_set,
    .fp_eps_calibration_eps_freq_offset_reset = eps_calibration_eps_freq_offset_reset,

    // lock
    .fp_lock_set_triac = lock_set_triac,
    .fp_lock_get_state           = lock_get_state,
    .fp_lock_door_lock_state_get = lock_door_lock_state_get,

    // drain
    .fp_drain_req_set   = drain_req_set,
    .fp_drain_set       = drain_set,
    .fp_drain_get_state = drain_get_state,

    // black channel
    .fp_blackch_rcv_frame = blackch_rcv_frame,

    // UI black channel
    .fp_ui_blackch_rcv_frame = ui_blackch_rcv_frame,
};

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
