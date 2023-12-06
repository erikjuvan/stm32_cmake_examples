/**
 * @file     pu_safe.c
 * @brief    PU SAFE main module.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup PU_SAFE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "main.h"
#include "pu_safe.h"
#include "apptime.h"
#include "drain.h"
#include "lock.h"
#include "valve.h"
#include "eps.h"
#include "black_channel.h"
#include "func_table.h"
#include "mcu_prph.h"
#include "trace.h"
#include "safe_info.h"
#include "emrgcy_drain.h"
#include "ui_black_channel.h"
#include "stm32xx_STLmain.h"
#include "stm32xx_STLlib.h"
#include "printf_support.h"
#include "watchdog.h"

/**
 * Used for qualification testing (standards compliance testing)
 */
#ifdef QUALIFICATION_TEST
#include "test_switch.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#if (CONF_PU_SAFE_LOG == 1)
#define PU_SAFE_LOG /**< Debug macro for TRACE */
#endif

#define PU_SAFE_VERSION        "1.0.0"               /**< Module version. */
#define PU_SAFE_NAME           "Safe - PU main app"  /**< Module name. */
#define PU_SAFE_NAME_SHORT     "SAP"                 /**< SAFE Module short name. */

#define PU_SAFE_BLKCH_SEND_MS              300    /**< Send black channel payload every milliseconds */
#define UI_START_PRESSED_WINDOW_MS         2000   /**< Timing window after receving start pressed from UI, during which door can be locked */


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * States of PU SAFE state machine.
 */
typedef enum
{
    PU_SAFE_STATE_RUNNING,                        /**< Running/active state. */
    PU_SAFE_STATE_GOTO_STANDBY,                   /**< Transitioning to standby state. */
    PU_SAFE_STATE_STANDBY,                        /**< Standby state. */
} pu_safe_state_t;

/**
 * States of PU SAFE standby state machine.
 */
typedef enum
{
    STANDBY_STATE_INIT,
    STANDBY_STATE_IDLE,
    STANDBY_STATE_PERIODIC_CHECK_WARMUP,
    STANDBY_STATE_PERIODIC_CHECK_RUNNING,
} pu_safe_standby_state_t;

/**
 * PU SAFE application internal structure.
 */
typedef struct
{
    pu_safe_state_t         state;               /**< State of PU SAFE module. */
    pu_safe_standby_state_t standby_state;       /**< State of PU SAFE module while in standby. */
    mcu_prph_status_t       mcu_prph_status;     /**< MCU periphery status. */
    struct
    {
        apptime_t blkch_tx;                      /**< Black channel transmit timestamp. */
    } time;                                      /**< Main time stamping struct. */
    struct
    {
        blackch_init_status_t blkch;             /**< Black channel initialization status. */
        lock_init_status_t    lock_init_status;  /**< Lock initialization status. */
        drain_init_status_t   drain_init_status; /**< Drain pump initialization status. */
        eps_init_status_t     eps_init_status;   /**< EPS initialization status. */
    } init;                                      /**< Module initialization status. */
    struct
    {
        blackch_pyld_mu_t blkch;                 /**< Black channel received payload. */
        uint8_t           ui_blkch_start;        /**< UI black channel signal that start key was pressed */
        apptime_t         ui_blkch_start_ts;     /**< UI black channel packet received timestamp */
        uint32_t          ui_blkch_start_remaining_ms; /**< Remaining time for ui start key window, in which lock request is accepted */
        bool              blkch_new_data;        /**< Flag to signal new data from MU */
        relay_state_extended_t power_relay_state_ex; /**< Flag to signal new data from MU */
        lock_state_t      lock_state;            /**< Lock state. */
        drain_state_t     drain_state;           /**< Drain pump state. */
        eps_level_t       eps_level;             /**< EPS level. */
        valve_state_t     valve_state;           /**< Inlet valves state. */
        bool              is_user_lock_req;      /**< Lock request flag. */
        bool              is_user_unlock_req;    /**< Unlock request flag. */
        bool              is_blkch_timeout;      /**< Black channel timeout flag. */
    } input;                                     /**< Safety application inputs. */
    struct
    {
        blackch_pyld_pu_t blkch;                 /**< Black channel payload to send. */
    } output;
} pu_safe_t;


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
/**
 * PU safety application internal information instance.
 */
static pu_safe_t g_pu_safe =
{
    .state = PU_SAFE_STATE_RUNNING,
    .mcu_prph_status = eMCU_PRPH_OK,
    .time =
    {
        .blkch_tx  = 0,
    },
    .init =
    {
        // Init modules to something that is not OK
        .blkch = eBLACKCH_INIT_CBK_FAIL,
        .lock_init_status = eLOCK_INIT_GPIO_FAIL,
        .drain_init_status = eDRAIN_INIT_GPIO_FAIL,
        .eps_init_status = eEPS_INIT_GPIO_FAIL,
    },
    .input =
    {
        .blkch = {{{ 0 }}},
        .ui_blkch_start = 0,
        .ui_blkch_start_ts = 0,
        .ui_blkch_start_remaining_ms = 0,
        .blkch_new_data = false,
        .power_relay_state_ex = eRELAY_EX_OFF,
        .lock_state = eLOCK_UNKNOWN,
        .drain_state = eDRAIN_OFF,
        .eps_level = eEPS_HIGH,
        .valve_state = eVALVE_OFF,
        .is_user_lock_req = false,
        .is_user_unlock_req = false,
        .is_blkch_timeout = false
    },
    .output =
    {
        .blkch = {{{ 0 }}}
    }
};


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static void            pu_safe_init_modules             (void);
static void            pu_safe_normal_proc              (void);
static void            pu_safe_standby_proc             (void);
static void            pu_safe_process_modules          (void);
static void            pu_safe_process_user_request     (void);
static void            pu_safe_process_mu_response      (void);
static lock_response_t map_blackchannel_rsp_to_lock_rsp (blackch_pyld_response_t bc_rsp);
static void            pu_safe_error                    (void);

// blackchannel
static void                      pu_safe_blkch_comm          (void);
static blackch_pyld_pu_status_t  pu_safe_get_blkch_pu_status (void);
static void                      pu_safe_blkch_send          (void);
static void                      pu_safe_blkch_rcvd          (const blackch_payload_t rx_payload);

// UI blackchannel
static void pu_safe_ui_blkch_comm(void);
static void pu_safe_ui_blkch_rcvd(const uint8_t key_state);

// lock, drain, eps
static lock_state_t    pu_safe_get_lock (void);
static drain_state_t   pu_safe_get_drain(void);
static eps_level_t     pu_safe_get_eps  (void);

// periphery check
static bool pu_safe_prph_check(void);
static bool pu_safe_standby_prph_check(void);

// Stand by
static void pu_safe_power_up(void);
static void pu_safe_power_down(void);
static void pu_safe_wait_stl_lsi(void);


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialization of safety (without STL).
 *
 * @return     None.
 */
void pu_safe_init(void)
{
    pu_safe_init_modules();
}

/**
 * Main run/proc routine for SAFE.
 *
 * @return     None.
 */
void pu_safe_proc(void)
{
    switch (g_pu_safe.state)
    {
    case PU_SAFE_STATE_RUNNING:
        pu_safe_normal_proc();
        break;

    case PU_SAFE_STATE_GOTO_STANDBY:
        g_pu_safe.standby_state = STANDBY_STATE_INIT;
        g_pu_safe.state = PU_SAFE_STATE_STANDBY;
        break;

    case PU_SAFE_STATE_STANDBY:
        pu_safe_standby_proc();
        break;

    default:
        g_pu_safe.state = PU_SAFE_STATE_RUNNING;
        break;
    }
}

/**
 * Request to lock door.
 *
 * @return     None.
 */
void pu_safe_req_lock(void)
{
#ifdef PU_SAFE_LOG
    TRACE_DEB("%s ::: request lock (ui start: %d)", PU_SAFE_NAME_SHORT, g_pu_safe.input.ui_blkch_start);
#endif
    if (g_pu_safe.input.ui_blkch_start == false)
    {
        lock_response(eLOCK_REQ_LOCK_RESP_DENIED_NO_UI_START);
    }
    else
    {
        g_pu_safe.input.is_user_lock_req = true;
    }
}

/**
 * Request to unlock door.
 *
 * @return     None.
 */
void pu_safe_req_unlock(void)
{
#ifdef PU_SAFE_LOG
    TRACE_DEB("%s ::: request unlock", PU_SAFE_NAME_SHORT);
#endif
    g_pu_safe.input.is_user_unlock_req = true;
}

/**
 * Clear request to lock/unlock door.
 *
 * @return     None.
 */
void pu_safe_req_clear(void)
{
    g_pu_safe.input.is_user_lock_req = false;
    g_pu_safe.input.is_user_unlock_req = false;
}

/**
 * SAFE go to standby mode.
 *
 * @return     None.
 */
bool pu_safe_standby(void)
{
    static bool request_pending = 0;
    static apptime_t start_ts = 0;

    if (g_pu_safe.state == PU_SAFE_STATE_GOTO_STANDBY ||
        g_pu_safe.state == PU_SAFE_STATE_STANDBY)
    {
        return true;
    }

    if (request_pending == 0)
    {
#ifdef PU_SAFE_LOG
        TRACE_DEB("%s ::: PU SAFE standby request...", PU_SAFE_NAME_SHORT);
#endif
        request_pending = 1;
        start_ts = apptime_get_ms();
    }

    if (apptime_is_elapsed(start_ts, 1000) || g_pu_safe.input.lock_state != eLOCK_UNKNOWN)
    {
#ifdef PU_SAFE_LOG
        TRACE_DEB("%s ::: PU SAFE standby mode", PU_SAFE_NAME_SHORT);
#endif
        request_pending = 0;
        g_pu_safe.state = PU_SAFE_STATE_GOTO_STANDBY;
        pu_safe_power_down();
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Wake from standby mode.
 *
 * @return     None.
 */
void pu_safe_wakeup(void)
{
#ifdef PU_SAFE_LOG
    TRACE_DEB("%s ::: pu_safe_wakeup", PU_SAFE_NAME_SHORT);
#endif

    g_pu_safe.state = PU_SAFE_STATE_RUNNING;
    g_pu_safe.input.power_relay_state_ex = eRELAY_EX_UNKNOWN;
    pu_safe_power_up();
}

/**
 * Get remaining window time for UI start key
 *
 * @return Remaining time in milliseconds until window expires
 */
uint32_t pu_safe_get_ui_start_remaining_ms(void)
{
    return g_pu_safe.input.ui_blkch_start_remaining_ms;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize all safe modules.
 *
 * @return     None.
 */
static void pu_safe_init_modules(void)
{
        g_pu_safe.init.blkch = blackch_init(pu_safe_blkch_rcvd, pg_user_func_table->fp_blackch_send_frame);
#ifdef PU_SAFE_LOG
        TRACE_DEB("%s ::: blackch_init %s", PU_SAFE_NAME_SHORT,
                g_pu_safe.init.blkch == eBLACKCH_INIT_OK ? "OK" : "FAIL");
#endif
        ui_blackch_init(pu_safe_ui_blkch_rcvd);
        g_pu_safe.init.lock_init_status = lock_init();
        g_pu_safe.init.drain_init_status = drain_init();
        g_pu_safe.init.eps_init_status = eps_init();
}

/**
 * "Normal" proc routine for SAFE.
 *
 * @return     None.
 */
static void pu_safe_normal_proc(void)
{
#ifdef QUALIFICATION_TEST
    if (g_test_switch.lock.lock_req_without_ui_start == true)
    {
        g_test_switch.lock.lock_req_without_ui_start = false;
        pu_safe_req_lock();
    }
#endif

    pu_safe_process_modules();
    emrgcy_drain_proc();
    pu_safe_blkch_comm();
    pu_safe_ui_blkch_comm();

    if (pu_safe_prph_check() == false)
    {
        pu_safe_error();
    }
    else
    {
        pu_safe_process_user_request();
        pu_safe_process_mu_response();
    }
}

/**
 * Standby proc routine for SAFE.
 *
 * @return     None.
 */
static void pu_safe_standby_proc(void)
{
    static apptime_t periodic_check_ts = 0;
    static apptime_t periodic_check_warmup_ts = 0;
    static apptime_t periodic_check_duration_ts = 0;

    switch (g_pu_safe.standby_state)
    {
    case STANDBY_STATE_INIT:
        periodic_check_ts = apptime_get_ms();
        g_pu_safe.standby_state = STANDBY_STATE_IDLE;
        break;

    case STANDBY_STATE_IDLE:
        if (apptime_is_elapsed(periodic_check_ts, 10000))
        {
            periodic_check_warmup_ts = apptime_get_ms();
            eps_power_up();
            g_pu_safe.standby_state = STANDBY_STATE_PERIODIC_CHECK_WARMUP;
#ifdef PU_SAFE_LOG
            TRACE_INF("%s ::: [standby] Safety check!", PU_SAFE_NAME_SHORT);
#endif
        }
        break;

    case STANDBY_STATE_PERIODIC_CHECK_WARMUP:
        if (apptime_is_elapsed(periodic_check_warmup_ts, 20))
        {
            periodic_check_duration_ts = apptime_get_ms();
            g_pu_safe.standby_state = STANDBY_STATE_PERIODIC_CHECK_RUNNING;
        }
        break;

    case STANDBY_STATE_PERIODIC_CHECK_RUNNING:
        if (!apptime_is_elapsed(periodic_check_duration_ts, 150))
        {
            // Check periphery
            if (pu_safe_standby_prph_check() == false)
            {
                /**
                 * TODO think about this, since in standby we aren't doing anything safety critical
                 * do we need to wake up? Or do we even need to run periphery checks?
                 */
                // pu_safe_error(); // should we even call this??
            }

            // Check EPS and emergency drain
            pu_safe_get_eps();
            emrgcy_drain_proc();
        }
        else if (emrgcy_drain_something_is_going_on())
        {
            // if "something" is going on in ED, stay in the periodic check state. Don't wake up USER, instead Wait for USER to wake up SAFE
            periodic_check_duration_ts = apptime_get_ms();
        }
        else
        {
            eps_power_down();
            periodic_check_ts = apptime_get_ms();
            g_pu_safe.standby_state = STANDBY_STATE_IDLE;
        }
        break;

    default:
        g_pu_safe.standby_state = STANDBY_STATE_IDLE;
        break;
    }
}
/**
 * Process all SAFE modules.
 *
 * @return     None.
 */
static void pu_safe_process_modules(void)
{
    g_pu_safe.input.lock_state  = pu_safe_get_lock();
    g_pu_safe.input.drain_state = pu_safe_get_drain();
    g_pu_safe.input.valve_state = valve_get_state();
    g_pu_safe.input.eps_level   = pu_safe_get_eps();
}

/**
 * Set black channel data for TX from user door lock request.
 *
 * @return     None.
 */
static void pu_safe_process_user_request(void)
{
    if (g_pu_safe.input.is_user_lock_req == true)
    {
        g_pu_safe.output.blkch.request = eBLCH_REQ_LOCK;
    }
    else if (g_pu_safe.input.is_user_unlock_req == true)
    {
        g_pu_safe.output.blkch.request = eBLCH_REQ_ULCK;
    }
    else
    {
        g_pu_safe.output.blkch.request = eBLCH_REQ_NONE;
    }
}

/**
 * Process MU black channel response.
 *
 * @return     None.
 */
static void pu_safe_process_mu_response(void)
{
    if (g_pu_safe.input.blkch_new_data &&
            g_pu_safe.input.is_blkch_timeout == false)
    {
        lock_door_lock_state_set(g_pu_safe.input.blkch.status.door_lock_state);

        lock_response(map_blackchannel_rsp_to_lock_rsp(g_pu_safe.input.blkch.response));

        g_safe_info.door_relay_closed = g_pu_safe.input.blkch.status.relay_door_state == eRELAY_ON;
        g_safe_info.power_relay_closed = g_pu_safe.input.blkch.status.relay_power_state == eRELAY_ON;

        g_pu_safe.input.blkch_new_data = false;
    }
}

/**
 * Map black channel response to lock response.
 *
 * @param[in]  bc_rsp   Black channel response.
 * @return     Lock response.
 */
static lock_response_t map_blackchannel_rsp_to_lock_rsp(blackch_pyld_response_t bc_rsp)
{
    switch (bc_rsp)
    {
    case eBLCH_REQ_NONE_RESP_NONE:                     return eLOCK_REQ_NONE_RESP_NONE;
    case eBLCH_REQ_LOCK_RESP_LOCK:                     return eLOCK_REQ_LOCK_RESP_LOCK;
    case eBLCH_REQ_ULCK_RESP_ULCK:                     return eLOCK_REQ_ULCK_RESP_ULCK;
    case eBLCH_REQ_LOCK_RESP_IN_PROGRESS:              return eLOCK_REQ_LOCK_RESP_IN_PROGRESS;
    case eBLCH_REQ_ULCK_RESP_IN_PROGRESS:              return eLOCK_REQ_ULCK_RESP_IN_PROGRESS;
    case eBLCH_REQ_LOCK_RESP_LOCKED:                   return eLOCK_REQ_LOCK_RESP_LOCKED;
    case eBLCH_REQ_ULCK_RESP_UNLOCKED:                 return eLOCK_REQ_ULCK_RESP_UNLOCKED;
    case eBLCH_REQ_LOCK_RESP_DENIED_NOT_UNLOCKED:      return eLOCK_REQ_LOCK_RESP_DENIED_NOT_UNLOCKED;
    case eBLCH_REQ_LOCK_RESP_DENIED_IMPOSSIBLE:        return eLOCK_REQ_LOCK_RESP_DENIED_IMPOSSIBLE;
    case eBLCH_REQ_LOCK_RESP_DENIED_NO_TRIAC_TRIG:     return eLOCK_REQ_LOCK_RESP_DENIED_NO_TRIAC_TRIG;
    case eBLCH_REQ_ULCK_RESP_DENIED_DOOR_NOT_LOCKED:   return eLOCK_REQ_ULCK_RESP_DENIED_DOOR_NOT_LOCKED;
    case eBLCH_REQ_ULCK_RESP_DENIED_SPINNING:          return eLOCK_REQ_ULCK_RESP_DENIED_SPINNING;
    case eBLCH_REQ_ULCK_RESP_DENIED_UNLOCK_IMPOSSIBLE: return eLOCK_REQ_ULCK_RESP_DENIED_UNLOCK_IMPOSSIBLE;
    case eBLCH_REQ_ULCK_RESP_DENIED_EPS_HIGH:          return eLOCK_REQ_ULCK_RESP_DENIED_EPS_HIGH;
    default: ASSERT(0);
    }

    return eLOCK_REQ_NONE_RESP_NONE;
}

/**
 * Error handling for pu_safe module. All errors are of type that disable SAFE functionality and does not reset processor.
 *
 * @return     None.
 */
static void pu_safe_error(void)
{
#ifdef PU_SAFE_LOG
    static int onetime = 1;
    if (onetime)
    {
        onetime = 0;
        TRACE_ERR("%s ::: FATAL! pu_safe_error - SAFETY FUNCTIONS DISABLED!", PU_SAFE_NAME_SHORT);
    }
#endif

    // Signal to USER that SAFE is in ERROR state
    error_handler(eSAFE_ERROR_DISABLED);
}

// black channel
////////////////////////////////////////////////////////////////////////////////
/**
 * Black channel main run/proc routine.
 *
 * @return     None.
 */
static void pu_safe_blkch_comm(void)
{
    if (g_pu_safe.init.blkch == eBLACKCH_INIT_OK)
    {
        blackch_proc();
        g_pu_safe.input.is_blkch_timeout = blackch_is_timeout();

        // Send black channel packet
        if (apptime_is_elapsed(g_pu_safe.time.blkch_tx, PU_SAFE_BLKCH_SEND_MS))
        {
            g_pu_safe.time.blkch_tx = apptime_get_ms();
            pu_safe_blkch_send();
        }

#ifdef PU_SAFE_LOG
        static bool bc_timeout_prev = false;
        if (g_pu_safe.input.is_blkch_timeout != bc_timeout_prev)
        {
            bc_timeout_prev = g_pu_safe.input.is_blkch_timeout;
            TRACE_WAR("%s ::: black channel %s", PU_SAFE_NAME_SHORT,
                g_pu_safe.input.is_blkch_timeout ? "TIMED OUT" : "received a packet successfully after TIMEOUT");
        }
#endif
    }
}

/**
 * Get all safety module statuses (lock, eps, drain) to send via black channel.
 *
 * @return     None.
 */
static blackch_pyld_pu_status_t pu_safe_get_blkch_pu_status(void)
{
    blackch_pyld_pu_status_t status = {0};

    // drain
    status.drain_state = g_pu_safe.input.drain_state;

    // eps
    status.eps_level = g_pu_safe.input.eps_level;

    // lock
    status.lock_state = g_pu_safe.input.lock_state;

    status.reserved = 0;

    return status;
}

/**
 * Black channel transmit routine.
 *
 * @return     None.
 */
static void pu_safe_blkch_send(void)
{
    g_pu_safe.output.blkch.status = pu_safe_get_blkch_pu_status();
    blackch_payload_t payld = {
        .status = g_pu_safe.output.blkch.status.byte,
        .req_resp = g_pu_safe.output.blkch.request
    };

#ifdef PU_SAFE_LOG
    static bool first_call_after_reset = true;
    static uint8_t prev_status = 0, prev_req = 0;
    static uint8_t prev_lck = 0, prev_eps = 0, prev_drn = 0;
    if (first_call_after_reset || payld.status != prev_status || prev_req != payld.req_resp)
    {
        first_call_after_reset = false;
        static const char *drain_state[2] = {"OFF", "ON"};
        static const char *eps_level[2] = {"HIGH", "LOW"};
        static const char *lock_state[3] = {"UKWN", "ULKD", "LCKD"};
        static const char *req[4] = {"NONE", "LOCK", "ULCK"};
        TRACE_DEB("%s ::: TX | sts(0x%02X->0x%02X): lck=%d->%d/%s->%s eps=%d->%d/%s->%s drn=%d->%d/%s->%s | req: %d->%d/%s->%s",
            PU_SAFE_NAME_SHORT,
            prev_status, g_pu_safe.output.blkch.status.byte,
            prev_lck, g_pu_safe.output.blkch.status.lock_state,  lock_state[prev_lck], lock_state[g_pu_safe.output.blkch.status.lock_state],
            prev_eps, g_pu_safe.output.blkch.status.eps_level,   eps_level[prev_eps], eps_level[g_pu_safe.output.blkch.status.eps_level],
            prev_drn, g_pu_safe.output.blkch.status.drain_state, drain_state[prev_drn], drain_state[g_pu_safe.output.blkch.status.drain_state],
            prev_req, g_pu_safe.output.blkch.request,            req[prev_req], req[g_pu_safe.output.blkch.request]);

        prev_status = payld.status;
        prev_req    = payld.req_resp;

        prev_lck = g_pu_safe.output.blkch.status.lock_state;
        prev_eps = g_pu_safe.output.blkch.status.eps_level;
        prev_drn = g_pu_safe.output.blkch.status.drain_state;
        prev_req = g_pu_safe.output.blkch.request;
    }
#endif

    blackch_send_payload(payld);
}

/**
 * Black channel receive callback.
 *
 * @param[in]  rx_payload   Received payload.
 * @return     None.
 */
static void pu_safe_blkch_rcvd(const blackch_payload_t rx_payload)
{
    blackch_pyld_mu_t pyld_mu = {
        .status.byte = rx_payload.status,
        .response = rx_payload.req_resp
    };

#ifdef PU_SAFE_LOG
    if (g_pu_safe.input.blkch_new_data == true && g_pu_safe.mcu_prph_status == eMCU_PRPH_OK)
    {
        TRACE_DEB("%s ::: pu_safe_blkch_rcvd warning! - overwriting previous data!", PU_SAFE_NAME_SHORT);
    }
#endif

    g_pu_safe.input.blkch = pyld_mu;
    g_pu_safe.input.power_relay_state_ex = pyld_mu.status.relay_power_state == eRELAY_ON ? eRELAY_EX_ON : eRELAY_EX_OFF;
    g_pu_safe.input.blkch_new_data = true;

#ifdef PU_SAFE_LOG
    typedef struct
    {
        int i;
        char *str;
    } rsp_entry;

    static uint8_t prev_status = 0, prev_rsp = 0;
    static uint8_t prev_rlp = 0, prev_rld = 0, prev_dor = 0, prev_dls = 0, prev_drs = 0;
    if (rx_payload.status != prev_status || prev_rsp != rx_payload.req_resp)
    {
        static const char *door_state[] = {"OPEN", "CLSD"};
        static const char *rly_state[] = {"OFF", "ON"};
        static const rsp_entry rsp[] = {
            {0,    "NONE"},
            {1,    "LOCK"},
            {2,    "ULCK"},
            {3,    "IN_PROGRESS"},
            {4,    "IN_PROGRESS"},
            {5,    "LOCKED"},
            {6,    "UNLOCKED"},
            {0x80, "DENIED_NOT_UNLOCKED"},
            {0x81, "DENIED_IMPOSSIBLE"},
            {0x82, "DENIED_NO_TRIAC_TRIG"},
            {0x83, "DENIED_DOOR_NOT_LOCKED"},
            {0x84, "DENIED_SPINNING"},
            {0x85, "DENIED_UNLOCK_IMPOSSIBLE"},
            {0x86, "DENIED_EPS_HIGH"},
            {0x8F, "DENIED_NO_UI_START"}
        };
        static const char *door_lock_state[] = {
            "DL_POWERUP", /**< Powerup procedure ongoing. */
            "DL_LOCKING", /**< Locking procedure ongoing. */
            "DL_TRIAC_LOCK", /**< Requesting to trigger TRIAC and lock the door. */
            "DL_LOCKED", /**< Door is locked. */
            "DL_UNLOCKING", /**< Unlocking procedure ongoing. */
            "DL_TRIAC_UNLOCK", /**< Requesting to trigger TRIAC and unlock the door. */
            "DL_UNLOCKED", /**< Door is unlocked. */
            "DL_ERROR" /**< Error occurred and further locking and unlocking is prevented. */
        };
        static const char *drumspin_state[] = {
            "eDSPN_STOPPED",   /**< Drum is stopped. */
            "eDSPN_MEASURING", /**< Measurement is ongoing. */
            "eDSPN_ROTATING"  /**< Drum is rotating. */
        };

        // Find correct string for response
        char *prev_rsp_str = "NOT_FOUND";
        char *rsp_str = "NOT_FOUND";
        for (size_t i = 0; i < sizeof(rsp) / sizeof(rsp_entry); ++i)
        {
            if (prev_rsp == rsp[i].i)
                prev_rsp_str = rsp[i].str;

            if (pyld_mu.response == rsp[i].i)
                rsp_str = rsp[i].str;
        }
        TRACE_DEB("%s ::: RX | sts(0x%02X->0x%02X): rlp=%d->%d/%s->%s rld=%d->%d/%s->%s dor=%d->%d/%s->%s dls=%d->%d/%s->%s drs=%d->%d/%s->%s | rsp: %d->%d/%s->%s",
            PU_SAFE_NAME_SHORT,
            prev_status, g_pu_safe.input.blkch.status.byte,
            prev_rlp, pyld_mu.status.relay_power_state, rly_state[prev_rlp], rly_state[pyld_mu.status.relay_power_state],
            prev_rld, pyld_mu.status.relay_door_state,  rly_state[prev_rld], rly_state[pyld_mu.status.relay_door_state],
            prev_dor, pyld_mu.status.door_state,        door_state[prev_dor], door_state[pyld_mu.status.door_state],
            prev_dls, pyld_mu.status.door_lock_state,   door_lock_state[prev_dls], door_lock_state[pyld_mu.status.door_lock_state],
            prev_drs, pyld_mu.status.drumspin_state,    drumspin_state[prev_drs], drumspin_state[pyld_mu.status.drumspin_state],
            prev_rsp, pyld_mu.response,                 prev_rsp_str, rsp_str);

        prev_status = pyld_mu.status.byte;
        prev_rsp    = pyld_mu.response;

        prev_rlp = pyld_mu.status.relay_power_state;
        prev_rld = pyld_mu.status.relay_door_state;
        prev_dor = pyld_mu.status.door_state;
        prev_dls = pyld_mu.status.door_lock_state;
        prev_drs = pyld_mu.status.drumspin_state;
    }
#endif
}

// UI black channel
////////////////////////////////////////////////////////////////////////////////
/**
 * UI black channel main run/proc routine.
 *
 * @return     None.
 */
static void pu_safe_ui_blkch_comm(void)
{
    if (g_pu_safe.input.ui_blkch_start == 1)
    {
        apptime_t elapsed = apptime_get_elapsed(g_pu_safe.input.ui_blkch_start_ts);
        if (elapsed >= UI_START_PRESSED_WINDOW_MS)
        {
            g_pu_safe.input.ui_blkch_start = 0;
            g_pu_safe.input.ui_blkch_start_remaining_ms = 0;
#ifdef PU_SAFE_LOG
            TRACE_DEB("%s ::: UI START (window over)", PU_SAFE_NAME_SHORT);
#endif
        }
        else
        {
            g_pu_safe.input.ui_blkch_start_remaining_ms = (uint32_t)UI_START_PRESSED_WINDOW_MS - (uint32_t)elapsed;
        }
    }
}

/**
 * UI black channel receive callback.
 *
 * @param[in]  key_state   State of start key.
 * @return     None.
 */
static void pu_safe_ui_blkch_rcvd(const uint8_t key_state)
{
    if (key_state == 1)
    {
        g_pu_safe.input.ui_blkch_start_ts = apptime_get_ms();
        g_pu_safe.input.ui_blkch_start = 1;
#ifdef PU_SAFE_LOG
        TRACE_DEB("%s ::: UI START", PU_SAFE_NAME_SHORT);
#endif
    }
}


// lock, drain, EPS
////////////////////////////////////////////////////////////////////////////////
/**
 * Run LOCK module and get state of door lock.
 *
 * @return     State of door lock.
 */
static lock_state_t pu_safe_get_lock(void)
{
    lock_state_t state = eLOCK_UNKNOWN;

    if (g_pu_safe.init.lock_init_status == eLOCK_INIT_OK)
    {
        lock_proc(g_pu_safe.input.power_relay_state_ex);
        state = lock_get_state();
    }

    return state;
}

/**
 * Run DRAIN module and get state of drain pump.
 *
 * @return     State of drain pump.
 */
static drain_state_t pu_safe_get_drain(void)
{
    drain_state_t state = eDRAIN_OFF;

    if (g_pu_safe.init.drain_init_status == eDRAIN_INIT_OK)
    {
        drain_proc();
        state = drain_get_state();
    }

    return state;
}

/**
 * Run EPS module and get state of EPS level.
 *
 * @return     EPS level LOW/HIGH.
 */
static eps_level_t pu_safe_get_eps(void)
{
    eps_level_t level = eEPS_HIGH;

    if (g_pu_safe.init.eps_init_status == eEPS_INIT_OK)
    {
        eps_proc(g_pu_safe.input.lock_state, g_pu_safe.input.drain_state, g_pu_safe.input.valve_state);
        level = eps_get_level();
    }

    return level;
}

// periphery check
////////////////////////////////////////////////////////////////////////////////
/**
 * Run MCU periphery check.
 *
 * @return     True if everything is OK, false otherwise.
 */
static bool pu_safe_prph_check(void)
{
    mcu_prph_proc();
    g_pu_safe.mcu_prph_status = mcu_prph_get_status();

    if (g_pu_safe.mcu_prph_status != eMCU_PRPH_OK)
    {
#ifdef PU_SAFE_LOG
        static mcu_prph_status_t prev_status = eMCU_PRPH_OK;
        if (prev_status != g_pu_safe.mcu_prph_status)
        {
            TRACE_ERR("%s ::: Class B MCU Periphery check failed (status: 0x%X)", PU_SAFE_NAME_SHORT, (int)g_pu_safe.mcu_prph_status);
            prev_status = g_pu_safe.mcu_prph_status;
        }
#endif
        return false;
    }

    return true;
}

/**
 * Run MCU periphery check while in standby.
 *
 * @return     True if everything is OK, false otherwise.
 */
static bool pu_safe_standby_prph_check(void)
{
    mcu_prph_proc_standby();
    g_pu_safe.mcu_prph_status = mcu_prph_get_status();

    if (g_pu_safe.mcu_prph_status != eMCU_PRPH_OK)
    {
#ifdef PU_SAFE_LOG
        static mcu_prph_status_t prev_status = eMCU_PRPH_OK;
        if (prev_status != g_pu_safe.mcu_prph_status)
        {
            TRACE_ERR("%s ::: Class B MCU Standby Periphery check failed (status: 0x%X)", PU_SAFE_NAME_SHORT, (int)g_pu_safe.mcu_prph_status);
            prev_status = g_pu_safe.mcu_prph_status;
        }
#endif
        return false;
    }

    return true;
}

// Stand by
////////////////////////////////////////////////////////////////////////////////
/**
 * Power everything back up.
 *
 * @return     None.
 */
static void pu_safe_power_up(void)
{
    StartUpClock_Config();
    eps_set_timer_mode(EPS_TIMER_FREQ_NODIV);
    watchdog_reinit_for_run();
    pu_safe_wait_stl_lsi();
    eps_power_up();
    lock_power_up();
    USART_Reconfiguration(); // This also reconfigures the UART for USER TRACE (dooh)

#ifdef PU_SAFE_LOG
    TRACE_DEB("%s ::: pu_safe_power_up", PU_SAFE_NAME_SHORT);
#endif
}

/**
 * Power down to lower power consumption for standby mode.
 *
 * @return     None.
 */
static void pu_safe_power_down(void)
{
    StandbyClock_Config();
    eps_set_timer_mode(EPS_TIMER_FREQ_DIV4);
    watchdog_reinit_for_standby();
    pu_safe_wait_stl_lsi();
    eps_power_down();
    lock_power_down();
    USART_Reconfiguration(); // This also reconfigures the UART for USER TRACE (dooh)

#ifdef PU_SAFE_LOG
    TRACE_DEB("%s ::: pu_safe_power_down", PU_SAFE_NAME_SHORT);
#endif
}


/**
* Wait for two consecutive STL clock period measurements with a timeout of
* 10 ms for each.
*
* @return       None.
*/
static void pu_safe_wait_stl_lsi(void)
{
    volatile apptime_t start_t;

    // Wait for two subsequent LSI periods measurements
    // If timeout reached continue anyway - STL clock check shall fail in this case
    start_t = apptime_get_ms();
    LSIPeriodFlag = 0u;
    while (LSIPeriodFlag == 0u)
    {
        if (apptime_is_elapsed(start_t, 10))
        {
            break;
        }
    }
    start_t = apptime_get_ms();
    LSIPeriodFlag = 0u;
    while (LSIPeriodFlag == 0u)
    {
        if (apptime_is_elapsed(start_t, 10))
        {
            break;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
