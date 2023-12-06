/**
 * @file     lock.c
 * @brief    Door lock module.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup LOCK
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <lock.h>
#include <stdbool.h>
#include "main.h"
#include "eps.h"
#include "apptime.h"
#include "func_table.h"
#include "tim.h"
#include "trace.h"
#include "mu_safe_types.h"

/**
 * Used for qualification testing (standards compliance testing)
 */
#ifdef QUALIFICATION_TEST
#include "test_switch.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#if (CONF_LOCK_LOG == 1)
#define LOCK_LOG    /**< Debug macro for TRACE */
#endif

#define LOCK_VERSION        "1.0.0"        /**< Module version. */
#define LOCK_NAME           "Safe - LOCK"  /**< Module name. */
#define LOCK_NAME_SHORT     "SLK"          /**< SAFE Module short name. */

#define LOCK_CHECK_ISR_EVERY_MS      100                         /**< Check isr is working every ms. */
#define LOCK_STATE_CHG_THR           4                           /**< Lock state change hysteresis threshold. */
#define LOCK_UNLOCKED_THR            (-(LOCK_STATE_CHG_THR / 2)) /**< Lock state change to unlocked threshold. */
#define LOCK_LOCKED_THR              (LOCK_STATE_CHG_THR / 2)    /**< Lock state change to locked threshold. */
#define LOCK_REQ_WINDOW_MS           4000                        /**< Lock state change request window time [ms]. */
#define LOCK_SIGNAL_MIN_PERIOD_US    15300                       /**< Minimum period [us] 65Hz. */
#define LOCK_SIGNAL_MAX_PERIOD_US    22200                       /**< Maximum period [us] 45Hz. */
#define LOCK_SIGNAL_MIN_PERIOD_TICKS LOCK_SIGNAL_MIN_PERIOD_US   /**< Minimum period [timer ticks]. */
#define LOCK_SIGNAL_MAX_PERIOD_TICKS LOCK_SIGNAL_MAX_PERIOD_US   /**< Maximum period [timer ticks]. */

#define HTIM_LOCK   htim15  /**< Timer handle macro. */


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Lock module internal information structure.
 */
typedef struct
{
    lock_triac_t triac;                 /**< Set TRIAC state. */
    door_lock_state_t door_lock_state;  /**< State of MU door lock. */
    bool is_state_meas_done;            /**< Flag indicating power relay has been on for sufficient amount of time. */
    struct
    {
        volatile uint32_t     period;   /**< Period measured by timer. */
        int8_t                chg_hyst; /**< State change hysteresis counter. */
        lock_state_t          state;    /**< Lock state. */
    } meas;                             /**< Lock signal measurement. */
    struct
    {
        apptime_t start_t;              /**< Request start time. */
        bool      is_start;             /**< Is request started flag. */
    } req;
    struct
    {
        struct
        {
            volatile uint32_t curr;     /**< Current value (incremented). */
            uint32_t prev;              /**< Previous value (saved). */
        } cnt;                          /**< ISR counters. */
        bool is_oper;                   /**< Is operational flag. */
    } isr;                              /**< Timer ISR check. */
    apptime_t isr_chk_time;             /**< Check ISR is working timestamp. */
    apptime_t power_relay_on_ts;        /**< Timestamp for power relay state */
} lock_t;


////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern TIM_HandleTypeDef htim15;


////////////////////////////////////////////////////////////////////////////////
// Private variables
////////////////////////////////////////////////////////////////////////////////
/**
 * Lock module internal information.
 */
static lock_t g_lock =
{
    .triac           = eLOCK_TRIAC_OFF,
    .door_lock_state = eDOOR_LOCK_POWERUP,
    .is_state_meas_done = false,
    .meas         =
    {
        .period   = 0,
        .chg_hyst = 0,
        .state    = eLOCK_UNKNOWN,  // this state is only present at boot, until we know in which state lock is
    },
    .req =
    {
        .start_t  = 0,
        .is_start = false,
    },
    .isr          =
    {
        .cnt      =
        {
            .curr = 0,
            .prev = 0,
        },
        .is_oper  = true,
    },
    .isr_chk_time      = 0,
    .power_relay_on_ts = 0,
};


////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////
#ifdef LOCK_LOG

#define GET_TS() TIM2->CNT  /**< Get current timestamp in ms. This is what is used by USER for TRACE. */

/**
 * Struct used for debug TRACE which would be called from IRQ but is instead called in proc.
 */
static struct
{
    struct vals_t{
        uint32_t ts;     /**< Sample timestamp. */
        uint32_t period; /**< Sample period. */
        int hys;         /**< Sample hysteresis value. */
        uint32_t status; /**< Sample status: 0 - NO SIG, 1 - SIG, 2 - INVALID SIG */
    } vals[10];
    int idx;             /**< Index of last entry. */
    int ovf;             /**< Overflow flag for vals array. */
} trace_sig = { 0 };

/**
 * TRACE Debug helper which adds sample.
 *
 * @param[in]    period  Period.
 * @param[in]    hys     Hysteresis.
 * @param[in]    status  Status.
 * @return       None.
 */
static void trace_sig_add(uint32_t period, int hys, uint32_t status)
{
    if (trace_sig.idx >= 10)
    {
        trace_sig.ovf = 1;
        return;
    }

    int i = trace_sig.idx;

    trace_sig.vals[i].ts = GET_TS(); // used by user to measure ms
    trace_sig.vals[i].period = period;
    trace_sig.vals[i].hys = hys;
    trace_sig.vals[i].status = status;

    trace_sig.idx++;
}

/**
 * TRACE Debug helper which prints what was gathered by trace_sig_add.
 *
 * @return       None.
 */
static void trace_sig_print(void)
{
    if (trace_sig.idx == 0)
        return;

    if (trace_sig.ovf)
    {
        TRACE_WAR("%s ::: sig OVF!", LOCK_NAME_SHORT);
    }

    static const char* txt[] = {"NO SIG", "SIG", "INVALID SIG"};
    for (int i = 0; i < trace_sig.idx; ++i)
    {
        struct vals_t* v = &trace_sig.vals[i];
        TRACE_DEB("%s ::: @%u %s %u! hyst: %d", LOCK_NAME_SHORT, v->ts, txt[v->status], v->period, v->hys);
    }

    trace_sig.idx = 0;
    trace_sig.ovf = 0;
}

/**
 * Struct used for debug TRACE which would be called from IRQ but is instead called in proc.
 */
static struct
{
    struct
    {
        uint32_t ts;        /**< Sample timestamp. */
        lock_triac_t state; /**< Sample state. */
        bool req;           /**< Sample request. */
    } vals[10];
    int idx;                /**< Index of last entry. */
    int ovf;                /**< Overflow flag for vals array. */
} trace_triac_set = { 0 };

/**
 * TRACE Debug helper which adds triac_set variables.
 *
 * @param[in]    state  TRIAC state.
 * @param[in]    req    State of request.
 * @return       None.
 */
static void trace_triac_set_add(lock_triac_t state, bool req)
{
    if (trace_triac_set.idx >= 10)
    {
        trace_triac_set.ovf = 1;
        return;
    }

    int i = trace_triac_set.idx;

    trace_triac_set.vals[i].ts = GET_TS();
    trace_triac_set.vals[i].state = state;
    trace_triac_set.vals[i].req = req;

    trace_triac_set.idx++;
}

/**
 * TRACE Debug helper which prints what was gathered by trace_triac_set_add.
 *
 * @return       None.
 */
static void trace_triac_set_print(void)
{
    if (trace_triac_set.idx == 0)
        return;

    if (trace_triac_set.ovf)
    {
        TRACE_WAR("%s ::: triac_set OVF!", LOCK_NAME_SHORT);
    }

    for (int i = 0; i < trace_triac_set.idx; ++i)
    {
        TRACE_DEB("%s ::: @%u lock_set_triac: %d (req.is_start: %d)", LOCK_NAME_SHORT,
            trace_triac_set.vals[i].ts, trace_triac_set.vals[i].state, trace_triac_set.vals[i].req);
    }

    trace_triac_set.idx = 0;
    trace_triac_set.ovf = 0;
}

#endif // LOCK_LOG


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static lock_init_status_t lock_init_gpio            (void);
static lock_init_status_t lock_init_timer           (void);
static lock_state_t       lock_dtrm_state           (lock_state_t old);
static void               lock_set_triac_gpio       (void);
static void               lock_clr_triac_gpio       (void);
static void               lock_timer_calc_chg_hyst  (void);
static uint32_t           lock_timer_get_period     (void);
static bool               lock_timer_chk_and_clr_ovf(void);
static bool               lock_timer_chk_and_clr_ic (void);
static void               lock_timer_ic_isr         (void);
static void               lock_timer_ovf_isr        (void);
static void               lock_timer_isr_incr       (void);
static void               lock_chk_timer_isr        (void);


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize door lock module.
 *
 * @return     Lock initialization status.
 */
lock_init_status_t lock_init(void)
{
    lock_init_status_t status = eLOCK_INIT_OK;

    status |= lock_init_gpio();

    status |= lock_init_timer();

    // Initialize time-stamp to ensure that 300ms elapse before reading lock state
    g_lock.power_relay_on_ts = apptime_get_ms();

#ifdef LOCK_LOG
    if (status == eLOCK_INIT_OK)
        TRACE_INF("%s ::: lock_init success", LOCK_NAME_SHORT);
    else
        TRACE_FAT("%s ::: lock_init failed (status: %d)", LOCK_NAME_SHORT, status);
#endif

    return status;
}

/**
 * Main run/proc function for Lock module.
 *
 * @param[in]  power_relay_state_ex   Extended state of power relay.
 * @return     None.
 */
void lock_proc(relay_state_extended_t power_relay_state_ex)
{
    if (apptime_is_elapsed(g_lock.isr_chk_time, LOCK_CHECK_ISR_EVERY_MS))
    {
        lock_chk_timer_isr();
        g_lock.isr_chk_time = apptime_get_ms();
    }

    if (g_lock.req.is_start == true &&
            apptime_is_elapsed(g_lock.req.start_t, LOCK_REQ_WINDOW_MS))
    {
        g_lock.req.is_start = false;
#ifdef LOCK_LOG
        TRACE_DEB("%s ::: g_lock.req.is_start: 0", LOCK_NAME_SHORT);
#endif
    }

    if (power_relay_state_ex == eRELAY_EX_OFF)
    {
        g_lock.power_relay_on_ts = apptime_get_ms();
        g_lock.meas.state = eLOCK_UNKNOWN;
        g_lock.is_state_meas_done = false;
    }
    else if (power_relay_state_ex == eRELAY_EX_UNKNOWN)
    {
        // Do nothing but wait to get valid relay state
    }
    else if (g_lock.is_state_meas_done || apptime_is_elapsed(g_lock.power_relay_on_ts, 300))
    {
        g_lock.meas.state = lock_dtrm_state(g_lock.meas.state);
        g_lock.is_state_meas_done = true;
    }

#ifdef LOCK_LOG
    trace_sig_print();
    trace_triac_set_print();

    static relay_state_extended_t prev = eRELAY_EX_OFF;
    static const char *rly_txt[] = {"OFF", "ON", "UKWN"};
    if (prev != power_relay_state_ex)
    {
        TRACE_INF("%s ::: power_relay %s -> %s", LOCK_NAME_SHORT, rly_txt[prev], rly_txt[power_relay_state_ex]);
        prev = power_relay_state_ex;
    }
#endif

#ifdef LOCK_LOG
    static lock_state_t prev_state;
    static const char * lock_txt[] = {
        "UNKNOWN",  // 0 Door state is not known.
        "UNLOCKED", // 1 Door is unlocked.
        "LOCKED",   // 2 Door is locked.
    };
    if (prev_state != g_lock.meas.state)
    {
        TRACE_DEB("%s ::: lock_dtrm_state: %s -> %s", LOCK_NAME_SHORT,
            lock_txt[prev_state], lock_txt[g_lock.meas.state]);
        prev_state = g_lock.meas.state;
    }
#endif
}

/**
 * Door lock Timer periphery ISR callback.
 *
 * @return     None.
 */
void lock_timer_isr(void)
{
#ifdef QUALIFICATION_TEST
    if (g_test_switch.lock.isr_fail == true)
    {
        lock_timer_chk_and_clr_ic();
        lock_timer_chk_and_clr_ovf();
        return;
    }
#endif

    if (lock_timer_chk_and_clr_ic() == true)
    {
        lock_timer_ic_isr();
        lock_timer_isr_incr();
    }

    if (lock_timer_chk_and_clr_ovf() == true)
    {
        lock_timer_ovf_isr();
        lock_timer_isr_incr();
    }

    lock_timer_calc_chg_hyst();
}

/**
 * Response back to USER with the response, after the USER request.
 *
 * @param[in]  response   Type of response.
 * @return     None.
 */
void lock_response(lock_response_t response)
{
    if (response == eLOCK_REQ_LOCK_RESP_LOCK ||
        response == eLOCK_REQ_ULCK_RESP_ULCK)
    {
        g_lock.req.is_start = true;
        g_lock.req.start_t = apptime_get_ms();
    }
    else if ((response & 0x80) > 0) // denied bit is set
    {
    	// If MU sends eLOCK_RESPONSE_DENIED disable access to triac immediately
    	g_lock.req.is_start = false;
    }

    pg_user_func_table->fp_lock_response_callback(response);

#ifdef LOCK_LOG
    typedef struct
    {
        int i;
        char *str;
    } rsp_entry;
    static lock_response_t prev_rsp;
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
    if (prev_rsp != response)
    {
        prev_rsp = response;
        // Find response string
        char *rsp_str = "NOT_FOUND";
        for (size_t i = 0; i < sizeof(rsp) / sizeof(rsp_entry); ++i)
        {
            if (response == rsp[i].i)
                rsp_str = rsp[i].str;
        }
        TRACE_DEB("%s ::: lock_response: %s (req.is_start:%d)", LOCK_NAME_SHORT, rsp_str, g_lock.req.is_start);
    }
#endif
}

/**
 * Set TRIAC.
 *
 * @param[in]  triac   TRIAC state to be set (ON/OFF).
 * @return     None.
 */
void lock_set_triac(lock_triac_t triac)
{
#ifdef LOCK_LOG
    trace_triac_set_add(triac, g_lock.req.is_start);
#endif

    if (g_lock.isr.is_oper == true &&
        g_lock.req.is_start == true &&
        !apptime_is_elapsed(g_lock.req.start_t, LOCK_REQ_WINDOW_MS))
    {
        g_lock.triac = triac;

        if (triac == eLOCK_TRIAC_ON)
        {
            lock_set_triac_gpio();
        }
        else
        {
            lock_clr_triac_gpio();
        }
    }
}

/**
 * Get TRIAC state.
 *
 * @return     TRIAC ON or OFF.
 */
lock_triac_t lock_get_triac(void)
{
    return g_lock.triac;
}

/**
 * Save MU door lock module state.
 *
 * @param[in]  state   MU door lock module state.
 * @return     None.
 */
void lock_door_lock_state_set(door_lock_state_t state)
{
    g_lock.door_lock_state = state;
}

/**
 * Door lock state.
 *
 * @return     lock_state_t.
 */
lock_state_t lock_get_state(void)
{
    return g_lock.meas.state;
}

/**
 * Get MU door lock module state.
 *
 * @return     door_lock_state_t.
 */
door_lock_state_t lock_door_lock_state_get(void)
{
    return g_lock.door_lock_state;
}

/**
 * Power up lock module
 *
 * @return    None.
 */
void lock_power_up(void)
{
    __HAL_RCC_TIM15_CLK_ENABLE();
    g_lock.isr_chk_time = apptime_get_ms(); // update time so to not immediately check for ISR error since we would get it (since ISR was not triggering in power down)
}

/**
 * Power down lock module
 *
 * @return    None.
 */
void lock_power_down(void)
{
    __HAL_RCC_TIM15_CLK_DISABLE();
}



////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Determine state of door locked state.
 *
 * @param[in]  old   Current state of door locked status.
 * @return     Door locked status.
 */
static lock_state_t lock_dtrm_state(lock_state_t old)
{
    lock_state_t new;

    if (g_lock.isr.is_oper == true)
    {
        if (g_lock.meas.chg_hyst <= LOCK_UNLOCKED_THR)
        {
            new = eLOCK_UNLOCKED;
        }
        else if (g_lock.meas.chg_hyst >= LOCK_LOCKED_THR)
        {
            new = eLOCK_LOCKED;
        }
        else
        {
            new = old;
        }
    }
    else
    {
        new = eLOCK_UNLOCKED;
    }

    return new;
}

/**
 * Set GPIO of door lock TRIAC to HIGH.
 *
 * @return     None.
 */
static void lock_set_triac_gpio(void)
{
    HAL_GPIO_WritePin(LOCK_TRIAC_PORT, LOCK_TRIAC_PIN, GPIO_PIN_SET);
}

/**
 * Set GPIO of door lock TRIAC to LOW.
 *
 * @return     None.
 */
static void lock_clr_triac_gpio(void)
{
    HAL_GPIO_WritePin(LOCK_TRIAC_PORT, LOCK_TRIAC_PIN, GPIO_PIN_RESET);
}

/**
 * Determine, based on signal period, if door locked signal is present, invalid or not present.
 *
 * @return     None.
 */
static void lock_timer_calc_chg_hyst(void)
{
    uint32_t period = lock_timer_get_period();

    // Valid signal present
    if (period >= LOCK_SIGNAL_MIN_PERIOD_TICKS &&
        period <= LOCK_SIGNAL_MAX_PERIOD_TICKS)
    {
        if (g_lock.meas.chg_hyst < LOCK_LOCKED_THR)
        {
            g_lock.meas.chg_hyst++;
#ifdef LOCK_LOG
            trace_sig_add(period, g_lock.meas.chg_hyst, 1);
#endif
        }
    }
    else if (period == 0) // No signal present
    {
        if (g_lock.meas.chg_hyst > LOCK_UNLOCKED_THR)
        {
            g_lock.meas.chg_hyst--;
#ifdef LOCK_LOG
            trace_sig_add(period, g_lock.meas.chg_hyst, 0);
#endif
        }
    }
    else // Invalid signal present
    {
        // Ignore noisy captures
#ifdef LOCK_LOG
        trace_sig_add(period, g_lock.meas.chg_hyst, 2);
#endif
    }
}

/**
 * Get door lock timer period.
 *
 * @return     Door lock timer period.
 */
static uint32_t lock_timer_get_period(void)
{
    return g_lock.meas.period;
}

/**
 * Door lock, timer input capture callback.
 *
 * @return     None.
 */
static void lock_timer_ic_isr(void)
{
    g_lock.meas.period = HAL_TIM_ReadCapturedValue(&HTIM_LOCK, LOCK_PERIOD_TIM_IC_CHANNEL);
}

/**
 * Door lock, timer overflow callback.
 *
 * @return     None.
 */
// timer overflow (door lock signal not present)
static void lock_timer_ovf_isr(void)
{
    g_lock.meas.period = 0;
}

/**
 * Check for and clear door lock timer input capture interrupt.
 *
 * @return     True if the source of timer IRQ was door lock signal (input capture).
 */
static bool lock_timer_chk_and_clr_ic(void)
{
    bool ret = false;

    if (__HAL_TIM_GET_FLAG(&HTIM_LOCK, TIM_FLAG_CC2) != RESET &&
        __HAL_TIM_GET_IT_SOURCE(&HTIM_LOCK, TIM_IT_CC2) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&HTIM_LOCK, TIM_IT_CC2);

        if ((HTIM_LOCK.Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U)
        {
            ret = true;
        }
    }

    return ret;
}

/**
 * Check for and clear door lock timer overflow interrupt.
 *
 * @return     True if the source of timer IRQ was an overflow of timer counter (no door locked signal present).
 */
static bool lock_timer_chk_and_clr_ovf(void)
{
    bool ret = false;

    if (__HAL_TIM_GET_FLAG(&HTIM_LOCK, TIM_FLAG_UPDATE) != RESET &&
        __HAL_TIM_GET_IT_SOURCE(&HTIM_LOCK, TIM_IT_UPDATE) != RESET
        )
    {
        __HAL_TIM_CLEAR_IT(&HTIM_LOCK, TIM_IT_UPDATE);

        ret = true;
    }

    return ret;
}

/**
 * Increment timer IRQ counter, used to detect issues if timer IRQ wasn't working properly.
 *
 * @return     None.
 */
static void lock_timer_isr_incr(void)
{
    g_lock.isr.cnt.curr++;
}

/**
 * Check if door lock timer periphery is working properly (constantly triggering).
 *
 * @return     None.
 */
static void lock_chk_timer_isr(void)
{
    if (g_lock.isr.cnt.curr == g_lock.isr.cnt.prev)
    {
        g_lock.isr.is_oper = false;
#ifdef LOCK_LOG
        static uint32_t timeit = 0;
        if (apptime_is_elapsed(timeit, 1000))
        {
            timeit = apptime_get_ms();
            TRACE_ERR("%s ::: lock ISR not triggering!", LOCK_NAME_SHORT);
        }
#endif
    }
    else
    {
        // Do not set g_lock.isr.is_oper = true since it should never happen (thought by EMSISO)
        // Perhaps we should set it to true, to avoid latching uC on some transient errors?
        g_lock.isr.cnt.prev = g_lock.isr.cnt.curr;
    }
}

/**
 * Initialize door lock GPIO periphery.
 *
 * @return     Lock initialization status.
 */
static lock_init_status_t lock_init_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Set output pins to 0
    HAL_GPIO_WritePin(LOCK_TRIAC_PORT, LOCK_TRIAC_PIN, GPIO_PIN_RESET);

    // Configure output GPIO pin for door lock triac
    GPIO_InitStruct.Pin = LOCK_TRIAC_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LOCK_TRIAC_PORT, &GPIO_InitStruct);

    // GPIO pin for door indicator is configured in HAL_TIM_Base_MspInit

    return eLOCK_INIT_OK;
}

/**
 * Initialize door lock TIM periphery.
 *
 * @return     Lock initialization status.
 */
static lock_init_status_t lock_init_timer(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    // initialize HAL timer
    status |= MX_TIM15_Init();

    // start timer
    status |= HAL_TIM_Base_Start_IT(&HTIM_LOCK); // enable update irq

    // start period input capture (enable ic irq)
    status |= HAL_TIM_IC_Start_IT(&HTIM_LOCK, LOCK_PERIOD_TIM_IC_CHANNEL);

    if (status != HAL_OK)
    {
        return eLOCK_INIT_TIMER_FAIL;
    }

    return eLOCK_INIT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
