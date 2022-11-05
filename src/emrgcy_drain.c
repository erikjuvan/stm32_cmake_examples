/**
 * @file     emrgcy_drain.c
 * @brief    Emergency drain (ED) module
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

/**
 * @addtogroup EMRGCY_DRAIN
 * @{ <!-- BEGIN GROUP -->
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include "emrgcy_drain.h"
#include "trace.h"
#include "project_conf.h"
#include "apptime.h"
#include "drain.h"
#include "eps.h"
#include "lock.h"
#include "constants_common.h"
#include "safe_info.h"


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#if (CONF_EMRGCY_DRAIN_LOG == 1)
#define EMRGCY_DRAIN_LOG    /**< Debug macro for TRACE */
#endif

#define EMRGCY_DRAIN_VERSION         "1.0.0"            /**< Module version. */
#define EMRGCY_DRAIN_NAME            "Emergency Drain"  /**< Module name. */
#define EMRGCY_DRAIN_NAME_SHORT      "SED"              /**< SAFE Module short name. */

#define EXTRA_DRAIN_TIME_AFTER_EMPTY_MS  5000  /**< Drain for additional time after all water has been drained out. */
#define ED_CONDITION_TRUE_DURATION_MS    3000  /**< Duration for ED condition to be true before we start with ED. */


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * States of ED state machine.
 */
typedef enum
{
    ED_STATE_IDLE = 0,        /**< Idle state. */
    ED_STATE_CONDITION_TRUE,  /**< Condition for ED is true. */
    ED_STATE_ACTIVE,          /**< ED is active (drain pump is on). */
    ED_STATE_FINISHING,       /**< ED finishing... reached water level empty. */
    ED_STATE_FINISHED,        /**< ED finished. */
    ED_STATE_WAIT_COND_FALSE, /**< Wait for ED condition to go to false, before accepting another one. */
} emrgcy_drain_state_t;


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
/**
 * ED module variables
 */
static struct
{
    emrgcy_drain_state_t state;        /**< Current state of ED state machine. */
    apptime_t condition_true_ts;       /**< Time when ED condition was last true. */
    apptime_t start_time;              /**< Time when ED started. */
    apptime_t safe_window_start_time;  /**< Time when safe window started (drain pump was ON for sufficient length of time). */
    apptime_t finishing_ts;            /**< Time when water level got to empty. */
    apptime_t active_ts;               /**< Time when ED started. */
    bool active;                       /**< ED is active. */
    bool safe_window;                  /**< Safe window is active. If EPS is "faulty" then during this time EPS is overwritten to LOW. */
} emrgcy_drain = { 0 };


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static void emrgcy_drain_check(void);
static void emrgcy_drain_safe_window(void);
static bool emrgcy_drain_condition(void);

////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Main run/proc function for ED module and safe window functionality.
 *
 * @return       None.
 */
void emrgcy_drain_proc(void)
{
    emrgcy_drain_check();
    emrgcy_drain_safe_window();
}

/**
 * Is safe window active.
 *
 * @return    True of safe window is active, otherwise false.
 */
bool emrgcy_drain_is_safe_window(void)
{
    return emrgcy_drain.safe_window;
}

/**
 * Is ED active.
 *
 * @return     True if ED is active, otherwise false.
 */
bool emrgcy_drain_is_active(void)
{
    return emrgcy_drain.active;
}

/**
 * Check if there is anything going on vis-a-vis emergency draining.
 *
 * @return     True if some activity is going on in ED that should not let SAFE go back to sleep.
 */
bool emrgcy_drain_something_is_going_on(void)
{
    return emrgcy_drain.state != ED_STATE_IDLE && emrgcy_drain.state != ED_STATE_WAIT_COND_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Check if ED condition is fulfilled.
 *
 * @return     True if ED condition is fulfilled.
 */
static bool emrgcy_drain_condition(void)
{
    return ((lock_get_state() != eLOCK_LOCKED) && (eps_is_safe() == false));
}

/**
 * ED state machine routine.
 *
 * @return       None.
 */
static void emrgcy_drain_check(void)
{
    switch (emrgcy_drain.state)
    {
    case ED_STATE_IDLE:
        if (emrgcy_drain_condition() == true)
        {
#ifdef EMRGCY_DRAIN_LOG
            TRACE_DEB("%s ::: Class B: Emergency Drain condition true!!!", EMRGCY_DRAIN_NAME_SHORT);
#endif
            emrgcy_drain.condition_true_ts = apptime_get_ms();
            emrgcy_drain.state = ED_STATE_CONDITION_TRUE;
        }
        break;

    case ED_STATE_CONDITION_TRUE:
        if (emrgcy_drain_condition() == false) // ED condition is no longer true
        {
#ifdef EMRGCY_DRAIN_LOG
            TRACE_DEB("%s ::: Class B: Emergency Drain condition no longer true!", EMRGCY_DRAIN_NAME_SHORT);
#endif
            emrgcy_drain.state = ED_STATE_IDLE;
        }
        else if (apptime_is_elapsed(emrgcy_drain.condition_true_ts, ED_CONDITION_TRUE_DURATION_MS))
        {
#ifdef EMRGCY_DRAIN_LOG
            TRACE_DEB("%s ::: Class B: Emergency Drain Started!", EMRGCY_DRAIN_NAME_SHORT);
#endif
            emrgcy_drain.active = true;
            g_safe_info.emrgcy_drain_active = true;
            drain_req_set(eDRAIN_ON);
            emrgcy_drain.state = ED_STATE_ACTIVE;
            emrgcy_drain.active_ts = apptime_get_ms();
        }
        break;

    case ED_STATE_ACTIVE:
        // If emergency drain (ED) was activated, stop draining if water is completely drained or ED timeout elapsed.
        // Do not stop draining if door locks during ED.
        if (eps_is_level_empty() == true ||
            apptime_is_elapsed(emrgcy_drain.active_ts, SAFETY_DRAIN_TIME_MS))
        {
#ifdef EMRGCY_DRAIN_LOG
            TRACE_DEB("%s ::: Class B: Emergency Drain finishing, EPS: %dum", EMRGCY_DRAIN_NAME_SHORT, eps_get_sig_um());
#endif
            emrgcy_drain.state = ED_STATE_FINISHING;
            emrgcy_drain.finishing_ts = apptime_get_ms();
        }
        break;

    case ED_STATE_FINISHING:
        if (apptime_is_elapsed(emrgcy_drain.finishing_ts, EXTRA_DRAIN_TIME_AFTER_EMPTY_MS))
        {
            emrgcy_drain.state = ED_STATE_FINISHED;
        }
        break;

    case ED_STATE_FINISHED:
#ifdef EMRGCY_DRAIN_LOG
        TRACE_DEB("%s ::: Class B: Emergency Drain finished!", EMRGCY_DRAIN_NAME_SHORT);
#endif
        emrgcy_drain.active = false;
        g_safe_info.emrgcy_drain_active = false;
        drain_req_set(eDRAIN_OFF);
        emrgcy_drain.state = ED_STATE_WAIT_COND_FALSE;
        break;

    case ED_STATE_WAIT_COND_FALSE:
        if (emrgcy_drain_condition() == false)
        {
#ifdef EMRGCY_DRAIN_LOG
            TRACE_DEB("%s ::: Class B: emrgcy_drain_condition is false! Returning to ED_STATE_IDLE.", EMRGCY_DRAIN_NAME_SHORT);
#endif
            emrgcy_drain.state = ED_STATE_IDLE;
        }
        break;

    default:
#ifdef EMRGCY_DRAIN_LOG
        TRACE_WAR("%s ::: Invalid Emergency Drain State!", EMRGCY_DRAIN_NAME_SHORT);
#endif
        emrgcy_drain.state = ED_STATE_IDLE;
        break;
    }
}

/**
 * Safe window checking routine.
 *
 * If drain pump is on for 180s non-stop then safe window starts.
 * Safe window lasts 20s after drain pump turns off.
 *
 * @return       None.
 */
static void emrgcy_drain_safe_window(void)
{
    if (drain_get_state() == eDRAIN_OFF)
    {
        emrgcy_drain.start_time = apptime_get_ms();
    }
    else if (apptime_is_elapsed(emrgcy_drain.start_time, SAFETY_DRAIN_TIME_MS))
    {
        emrgcy_drain.safe_window = true;
        emrgcy_drain.safe_window_start_time = apptime_get_ms();
    }

    if (apptime_is_elapsed(emrgcy_drain.safe_window_start_time, SAFE_WINDOW_AFTER_DRAIN_TIME_MS))
    {
        emrgcy_drain.safe_window = false;
    }

#ifdef EMRGCY_DRAIN_LOG
    static bool safe_win_prev = false;
    if (emrgcy_drain.safe_window == true && safe_win_prev == false)
        TRACE_DEB("%s ::: safety window started", EMRGCY_DRAIN_NAME_SHORT);
    else if (emrgcy_drain.safe_window == false && safe_win_prev == true)
        TRACE_DEB("%s ::: safety window over", EMRGCY_DRAIN_NAME_SHORT);
    safe_win_prev = emrgcy_drain.safe_window;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
