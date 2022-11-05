/**
 * @file     drain.c
 * @brief    Drain pump module
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

/**
 * @addtogroup DRAIN
 * @{ <!-- BEGIN GROUP -->
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "drain.h"
#include "main.h"
#include "apptime.h"
#include "func_table.h"
#include "trace.h"
#include "emrgcy_drain.h"

/**
 * Used for qualification testing (standards compliance testing)
 */
#ifdef QUALIFICATION_TEST
#include "test_switch.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#if (CONF_DRAIN_LOG == 1)
#define DRAIN_LOG  /**< Debug macro for TRACE */
#endif

#define DRAIN_VERSION        "1.0.0"         /**< Module version. */
#define DRAIN_NAME           "Safe - DRAIN"  /**< Module name. */
#define DRAIN_NAME_SHORT     "SDR"           /**< SAFE Module short name. */

#define DRAIN_REQ_TO_MS ( 200 ) /**< Drain pump state request actualization timeout [ms]. */


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Drain module variables.
 */
typedef struct
{
    drain_state_t     state;    /**< Drain pump state. */
    struct
    {
        drain_state_t state;    /**< Drain pump requested state. */
        bool          is_start; /**< Drain pump change requested */
        apptime_t     start_t;  /**< Time of request. */
    } req;
} drain_t;


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
/**
 * Drain module variables instance.
 */
static drain_t g_drain =
{
    .state        = eDRAIN_OFF,
    .req          =
    {
        .state    = eDRAIN_OFF,
        .is_start = false,
        .start_t  = 0,
    },
};


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static drain_init_status_t drain_init_gpio(void);
static void                drain_set_gpio(void);
static void                drain_clr_gpio(void);


////////////////////////////////////////////////////////////////////////////////
// Debug
////////////////////////////////////////////////////////////////////////////////
#ifdef DRAIN_LOG

#define GET_TS() TIM2->CNT  /**< Get current timestamp in ms. This is what is used by USER for TRACE. */

/**
 * Struct used for debug TRACE which would be called from IRQ but is instead called in proc.
 */
static struct
{
    struct
    {
        uint32_t ts;            /**< drain_set timestamp. */
        drain_state_t state;    /**< drain_set requested drain state. */
        bool req;               /**< State of is_start varible when drain_set was called. */
    } vals[10];
    int idx;                    /**< Index for vals array. */
    int ovf;                    /**< Overflow flag for vals array. */
} trace_drain_set = { 0 };

/**
 * TRACE Debug helper which adds drain_set variables.
 *
 * @param[in]    state  Drain state.
 * @param[in]    req    State of request.
 * @return       None.
 */
static void trace_drain_set_add(drain_state_t state, bool req)
{
    if (trace_drain_set.idx >= 10)
    {
        trace_drain_set.ovf = 1;
        return;
    }

    int i = trace_drain_set.idx;

    trace_drain_set.vals[i].ts = GET_TS();
    trace_drain_set.vals[i].state = state;
    trace_drain_set.vals[i].req = req;

    trace_drain_set.idx++;
}

/**
 * TRACE Debug helper which prints what was gathered by trace_drain_set_add.
 *
 * @return       None.
 */
static void trace_drain_set_print(void)
{
    if (trace_drain_set.idx == 0)
        return;

    if (trace_drain_set.ovf)
    {
        TRACE_WAR("%s ::: drain_set OVF!", DRAIN_NAME_SHORT);
    }

    static const char* txt[] = {"OFF", "ON"};
    for (int i = 0; i < trace_drain_set.idx; ++i)
    {
        TRACE_DEB("%s ::: @%u drain_set: %s (req.is_start: %d)", DRAIN_NAME_SHORT,
            trace_drain_set.vals[i].ts, txt[trace_drain_set.vals[i].state], trace_drain_set.vals[i].req);
    }

    trace_drain_set.idx = 0;
    trace_drain_set.ovf = 0;
}

#endif // DRAIN_LOG

////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize drain module.
 *
 * @return       Drain init status.
 */
drain_init_status_t drain_init(void)
{
    drain_init_status_t status = eDRAIN_INIT_OK;

    status |= drain_init_gpio();

#ifdef DRAIN_LOG
    if (status == eDRAIN_INIT_OK)
        TRACE_INF("%s ::: drain_init success", DRAIN_NAME_SHORT);
    else
        TRACE_FAT("%s ::: drain_init failed (status: %d)", DRAIN_NAME_SHORT, status);
#endif

    return status;
}

/**
 * Main run/proc function for Drain module.
 *
 * @return   None.
 */
void drain_proc(void)
{
    // force
    if (g_drain.req.is_start == true && apptime_is_elapsed(g_drain.req.start_t, DRAIN_REQ_TO_MS))
    {
#ifdef DRAIN_LOG
        TRACE_DEB("%s ::: drain force set", DRAIN_NAME_SHORT);
#endif
        drain_set(g_drain.req.state);
    }

#ifdef DRAIN_LOG
    trace_drain_set_print();
#endif

#ifdef QUALIFICATION_TEST
    if (g_test_switch.drain.off == true)
    {
        g_test_switch.drain.off = false;
        drain_req_set(eDRAIN_OFF);
    }
#endif
}

/**
 * Request a change in drain pump state.
 *
 * @param[in]    state    New state of drain pump.
 * @return       None.
 */
void drain_req_set(drain_state_t state)
{
    if (emrgcy_drain_is_active() == true &&
        state == eDRAIN_OFF) // Safe can not allow this
    {
#ifdef DRAIN_LOG
        TRACE_DEB("%s ::: EM drain is active cannot service OFF request", DRAIN_NAME_SHORT);
#endif
        // Callback with what the drain state is/should be?
        pg_user_func_table->fp_drain_req_set(eDRAIN_ON);
        // or just return
        return;
    }

    g_drain.req.state = state;
    g_drain.req.start_t = apptime_get_ms();
    g_drain.req.is_start = true;

    pg_user_func_table->fp_drain_req_set(state);

#ifdef DRAIN_LOG
    TRACE_DEB("%s ::: drain request: %s", DRAIN_NAME_SHORT, state == eDRAIN_ON ? "ON" : "OFF");
#endif
}

/**
 * Set the state of drain pump.
 *
 * @param[in]    state    State of drain pump to be set.
 * @return       None.
 */
void drain_set(drain_state_t state)
{
#ifdef DRAIN_LOG
    trace_drain_set_add(state, g_drain.req.is_start);
#endif

    if (g_drain.req.is_start == false || g_drain.req.state != state)
    {
        return;
    }

    // First set the actual IO
    if (state == eDRAIN_OFF)
    {
        drain_clr_gpio();
    }
    else
    {
        drain_set_gpio();
    }

    // Then set variables
    g_drain.state = state;
    g_drain.req.is_start = false;
}

/**
 * Get current drain pump state.
 *
 * @return       Current drain state. NOTE: it returns the state of internal variable not the actual GPIO pin.
 */
drain_state_t drain_get_state(void)
{
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(DRAIN_PORT, DRAIN_PIN);

    if ((int)pin_state != (int)g_drain.state)
    {
#ifdef DRAIN_LOG
        TRACE_ERR("%s ::: drain state error!!! state:%d is NOT GPIO:%d! Setting state to %d.", DRAIN_NAME_SHORT, g_drain.state, pin_state, pin_state);
#endif
        g_drain.state = (drain_state_t)pin_state;
    }

    return g_drain.state;
}


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Set GPIO that controls drain pump to HIGH.
 *
 * @return       None.
 */
static void drain_set_gpio(void)
{
    HAL_GPIO_WritePin(DRAIN_PORT, DRAIN_PIN, GPIO_PIN_SET);
}

/**
 * Set GPIO that controls drain pump to LOW.
 *
 * @return       None.
 */
static void drain_clr_gpio(void)
{
    HAL_GPIO_WritePin(DRAIN_PORT, DRAIN_PIN, GPIO_PIN_RESET);
}

/**
 * Initialize GPIO of drain module.
 *
 * @return       Drain init status.
 */
static drain_init_status_t drain_init_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // GPIO Ports Clock Enable
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Set output pins to 0
    HAL_GPIO_WritePin(DRAIN_PORT, DRAIN_PIN, GPIO_PIN_RESET);

    // Configure output GPIO pin for drain pump
    GPIO_InitStruct.Pin = DRAIN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DRAIN_PORT, &GPIO_InitStruct);

    return eDRAIN_INIT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
