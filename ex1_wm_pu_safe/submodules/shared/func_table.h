/**
* @file     func_table.h
* @brief    Class A (User) and class B (Safe) function pointer table.
* @author   Erik Juvan
* @date     23.12.2021
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup FUNC_TABLE
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef _FUNC_TABLE_H_
#define _FUNC_TABLE_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>

#include "eps_common.h"
#include "drain_common.h"
#include "lock_common.h"
#include "mu_safe_types.h"
#include "black_channel_common.h"
#include "safe_info_common.h"
#include "ui_black_channel_common.h"  // really don't like this, that we need external file, but atm this needs least work
#include "error_common.h"

////////////////////////////////////////////////////////////////////////////////
// User function pointer table
////////////////////////////////////////////////////////////////////////////////

typedef void     (*p_user_context_init_t             )(void);
typedef uint32_t (*p_user_app_init_t                 )(void);
typedef void     (*p_user_app_loop_t                 )(void);
typedef void     (*p_user_safe_error_handler_t       )(safe_error_t error);

// valve
typedef bool     (*p_user_valve_any_is_on_t          )(void);

// lock
typedef void     (*p_user_lock_response_callback_t   )(lock_response_t response);

// drain
typedef void     (*p_user_drain_req_set_t            )(drain_state_t state);

// black channel
typedef void     (*p_user_blackch_send_frame_t       )(blackch_frame_t tx_frame);

// DEBUG (TRACE timestamp)
typedef uint32_t (*p_user_TRACE_GetSysTimestamp_t    )(void);
typedef uint32_t (*p_user_TRACE_GetSysTimestampMs_t  )(void);

/**
 * USER function pointer table.
 */
typedef struct
{
    p_user_context_init_t               fp_context_init;
    p_user_app_init_t                   fp_app_init;
    p_user_app_loop_t                   fp_app_loop;
    p_user_safe_error_handler_t         fp_safe_error_handler;

    // valve
    p_user_valve_any_is_on_t            fp_valve_any_is_on;

    // lock
    p_user_lock_response_callback_t     fp_lock_response_callback;

    // drain
    p_user_drain_req_set_t              fp_drain_req_set;

    // black channel
    p_user_blackch_send_frame_t         fp_blackch_send_frame;

    // DEBUG (TRACE timestamp)
    p_user_TRACE_GetSysTimestamp_t      fp_TRACE_GetSysTimestamp;
    p_user_TRACE_GetSysTimestampMs_t    fp_TRACE_GetSysTimestampMs;

} user_func_table_t;




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Safe function pointer table
///////////////////////////////////////////////////////////////////////////////

// pu_safe (lock)
typedef void               (*p_pu_safe_req_lock_t               )(void);
typedef void               (*p_pu_safe_req_unlock_t             )(void);
typedef void               (*p_pu_safe_req_clear_t              )(void);
typedef bool               (*p_pu_safe_standby_t                )(void);
typedef void               (*p_pu_safe_wakeup_t                 )(void);
typedef uint32_t           (*p_pu_safe_get_ui_start_remaining_ms)(void);

// info
typedef const safe_info_t* (*p_safe_info_get_t           )(void);

// eps
typedef eps_level_t        (*p_safe_eps_get_level_t      )(void);
typedef uint32_t           (*p_safe_eps_get_period_t     )(void);
typedef uint32_t           (*p_safe_eps_get_isr_cnt_t    )(void);
typedef bool               (*p_safe_eps_is_plausible_t   )(void);
typedef bool               (*p_safe_eps_is_error_t       )(void);
typedef bool               (*p_safe_eps_calibration_timer_freq_set       )(uint32_t);
typedef void               (*p_safe_eps_calibration_timer_freq_reset     )(void);
typedef bool               (*p_safe_eps_calibration_eps_freq_offset_set  )(int32_t);
typedef void               (*p_safe_eps_calibration_eps_freq_offset_reset)(void);

// lock
typedef void               (*p_safe_lock_set_triac_t          )(lock_triac_t triac);
typedef lock_state_t       (*p_safe_lock_get_state_t          )(void);
typedef door_lock_state_t  (*p_safe_lock_door_lock_state_get_t)(void);

// drain
typedef void               (*p_safe_drain_req_set_t      )(drain_state_t state);
typedef void               (*p_safe_drain_set_t          )(drain_state_t state);
typedef drain_state_t      (*p_safe_drain_get_state_t    )(void);

// black channel
typedef void               (*p_safe_blackch_rcv_frame_t  )(const blackch_frame_t rx_frame);

// UI black channel
typedef void               (*p_safe_ui_blackch_rcv_frame_t)(const ui_blackch_frame_t rx_frame);

/**
 * SAFE function pointer table.
 */
typedef struct
{
    // pu_safe (lock)
    p_pu_safe_req_lock_t                fp_pu_safe_req_lock;
    p_pu_safe_req_unlock_t              fp_pu_safe_req_unlock;
    p_pu_safe_req_clear_t               fp_pu_safe_req_clear;
    p_pu_safe_standby_t                 fp_pu_safe_standby;
    p_pu_safe_wakeup_t                  fp_pu_safe_wakeup;
    p_pu_safe_get_ui_start_remaining_ms fp_pu_safe_get_ui_start_remaining_ms;

    // info
    p_safe_info_get_t                   fp_safe_info_get;

    // eps
    p_safe_eps_get_level_t              fp_eps_get_level;
    p_safe_eps_get_period_t             fp_eps_get_period;
    p_safe_eps_get_isr_cnt_t            fp_eps_get_isr_cnt;
    p_safe_eps_is_plausible_t           fp_eps_is_plausible;
    p_safe_eps_is_error_t               fp_eps_is_error;
    p_safe_eps_calibration_timer_freq_set        fp_eps_calibration_timer_freq_set;
    p_safe_eps_calibration_timer_freq_reset      fp_eps_calibration_timer_freq_reset;
    p_safe_eps_calibration_eps_freq_offset_set   fp_eps_calibration_eps_freq_offset_set;
    p_safe_eps_calibration_eps_freq_offset_reset fp_eps_calibration_eps_freq_offset_reset;

    // lock
    p_safe_lock_set_triac_t             fp_lock_set_triac;
    p_safe_lock_get_state_t             fp_lock_get_state;
    p_safe_lock_door_lock_state_get_t   fp_lock_door_lock_state_get;

    // drain
    p_safe_drain_req_set_t              fp_drain_req_set;
    p_safe_drain_set_t                  fp_drain_set;
    p_safe_drain_get_state_t            fp_drain_get_state;

    // black channel
    p_safe_blackch_rcv_frame_t          fp_blackch_rcv_frame;

    // UI black channel
    p_safe_ui_blackch_rcv_frame_t       fp_ui_blackch_rcv_frame;

} safe_func_table_t;



////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * SAFE function pointer table start address in Flash.
 */
extern const uint32_t _MEMPART_FLASH_SAFE_FNC_TABLE_ADDR;

/**
 * USER function pointer table start address in Flash.
 */
extern const uint32_t _MEMPART_FLASH_USER_FNC_TABLE_ADDR;

/**
 * Pointer to SAFE function pointer table.
 * Used as global variable across USER project.
 */
extern const safe_func_table_t* const pg_safe_func_table;

/**
 * Pointer to USER function pointer table.
 * Used as global variable across SAFE project.
 */
extern const user_func_table_t* const pg_user_func_table;

#endif // __FUNC_TABLE_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
