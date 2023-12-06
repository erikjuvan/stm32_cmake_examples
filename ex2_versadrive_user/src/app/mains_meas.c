////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     mains_meas.c
 * @brief    Mains power supply measurement.
 * @author   Haris Kovacevic
 * @date     10.03.2022
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MAINS_MEAS
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "stm32g4xx_hal.h"

#include "func_table.h"
#include "mains_meas.h"
#include "priorities.h"
#include "project_config.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Enable/disable zero-cross detection measurement using GPIO
 * #MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PORT,#MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PIN.
 */
#define MAINS_MEAS_EN_ZERO_CROSS_DETECT_MEAS   (0)

//@{
/**
 * GPIO used for measuring zero-cross detection by toggling it.
 */
#define MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PORT ()
#define MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PIN  ()
//@}

/**
 * Maximum number of registered negative zero-cross callback functions.
 */
#define MAINS_MEAS_MAX_NEG_ZERO_CROSS_CBKS     (4)

/**
 * Main timer input capture filter.
 * Valid values are from 0 to 15 inclusive with 15 being the strongest filter.
 */
#define MAINS_MEAS_TIM_IC_FILTER               (15)

/**
 * Main timer prescaler.
 * At 170 MHz clock frequency it prescales it to 400 kHz.
 */
#define MAINS_MEAS_TIM_PRESCALER               (425)

/**
 * Main timer period [ticks].
 * Absolute max period (@20Hz) for power fail detection: 400k / 20Hz = 20 000 ticks.
 */
#define MAINS_MEAS_TIM_MAX_PERIOD              (20000)

//@{
/**
 * Minimum and maximum allowed frequency at 50 Hz nominal power line.
 */
#define MAINS_MEAS_MIN_FREQ_AT_50HZ            (45)
#define MAINS_MEAS_MAX_FREQ_AT_50HZ            (55)
//@}

//@{
/**
 * Minimum and maximum allowed frequency at 60 Hz nominal power line.
 */
#define MAINS_MEAS_MIN_FREQ_AT_60HZ            (55)
#define MAINS_MEAS_MAX_FREQ_AT_60HZ            (65)
//@}

//@{
/**
 * Minimum and maximum main timer periods [tick] for detecting a 50 Hz power line signal.
 * @note This value must not be changed.
 */
#define MAINS_MEAS_MIN_TIM_PERIOD_AT_50HZ \
    (SYSTEM_CORE_CLOCK_HZ / MAINS_MEAS_TIM_PRESCALER / MAINS_MEAS_MAX_FREQ_AT_50HZ)
#define MAINS_MEAS_MAX_TIM_PERIOD_AT_50HZ \
    (SYSTEM_CORE_CLOCK_HZ / MAINS_MEAS_TIM_PRESCALER / MAINS_MEAS_MIN_FREQ_AT_50HZ)
//@}

//@{
/**
 * Minimum and maximum main timer periods [tick] for detecting a 60 Hz power line signal.
 * @note This value must not be changed.
 */
#define MAINS_MEAS_MIN_TIM_PERIOD_AT_60HZ \
    (SYSTEM_CORE_CLOCK_HZ / MAINS_MEAS_TIM_PRESCALER / MAINS_MEAS_MAX_FREQ_AT_60HZ)
#define MAINS_MEAS_MAX_TIM_PERIOD_AT_60HZ \
    (SYSTEM_CORE_CLOCK_HZ / MAINS_MEAS_TIM_PRESCALER / MAINS_MEAS_MIN_FREQ_AT_60HZ)
//@}

/**
 * Delay [us] between the detection of negative zero-cross and executing the zero-cross callback
 * functions.
 * @note Maximum delay with current timer configuration is 385 us.
 */
#define MAINS_MEAS_NEG_ZERO_CROSS_DELAY_US (270)

/**
 * Delay [ticks] between the detection of negative zero-cross and executing the zero-cross callback
 * functions.
 * @note This value must not be changed.
 */
#define MAINS_MEAS_TIM_NEG_ZERO_CROSS_DELAY_PERIOD \
    ((MAINS_MEAS_NEG_ZERO_CROSS_DELAY_US * SYSTEM_CORE_CLOCK_HZ) / 1000000ULL)

/**
 * Number of captured MAINS_TO_MCU signal periods for determining the power line frequency.
 */
#define MAINS_MEAS_PERIOD_BUFFER_SIZE         (4)

/**
 * Minimum number of valid power line period measurement to determine the power line frequency.
 */
#define MAINS_MEAS_MIN_N_VALID_POWERLINE_MEAS (MAINS_MEAS_PERIOD_BUFFER_SIZE / 2)

//@{
/**
 * MAINS_TO_MCU GPIO used for measuring power line signal frequency.
 */
#define MAINS_MEAS_MAINS_TO_MCU_PORT          (GPIOA)
#define MAINS_MEAS_MAINS_TO_MCU_PIN           (GPIO_PIN_11)
//@}

/**
 * How many times can there be no powerline signal present.
 */
#define MAINS_MEAS_POWERLINE_NO_SIGNAL_LIMIT  (10)

/**
 * Mains power line measurement structure.
 */
typedef struct
{
    mains_meas_powerfail_t powerfail; /**< Power fail detection state. */

    struct
    {
        const struct
        {
            struct
            {
                uint16_t min; /**< Minimum [ticks]. */
                uint16_t max; /**< Maximum [ticks]. */
            } hz50;           /**< 50 Hz nominal power line. */

            struct
            {
                uint16_t min; /**< Minimum [ticks]. */
                uint16_t max; /**< Maximum [ticks]. */
            } hz60;           /**< 60 Hz nominal power line. */
        } period_limits;      /**< Period limits for available power line frequencies. */

        mains_meas_powerline_freq_t freq; /**< Frequency. */
    } powerline;                          /**< Power line information. */

    struct
    {
        uint16_t period[MAINS_MEAS_PERIOD_BUFFER_SIZE]; /**< Period. */
        uint8_t  n;                                     /**< Number of stored. */
    } buffer; /**< Buffer for storing captured power line signal periods. */

    struct
    {
        pfnc_zero_cross_cbk fncs[MAINS_MEAS_MAX_NEG_ZERO_CROSS_CBKS]; /**< Registered functions. */
        uint32_t            n; /**< Number of registered functions. */
    } zero_cross_cbk;          /**< Zero-cross detection callbacks. */

    uint32_t powerline_no_signal_cnt; /**< Counter when no signal is present. */

} mains_meas_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Mains power line measurement instance.
 */
// clang-format off
static mains_meas_t g_mains_meas = 
{
    .powerfail = eMAINS_MEAS_POWERFAIL_NOT_ACTIVE,
    .powerline = 
    {
        .period_limits =
        {
            .hz50 =
            {
                .min = MAINS_MEAS_MIN_TIM_PERIOD_AT_50HZ,
                .max = MAINS_MEAS_MAX_TIM_PERIOD_AT_50HZ,
            },
            .hz60 =
            {
                .min = MAINS_MEAS_MIN_TIM_PERIOD_AT_60HZ,
                .max = MAINS_MEAS_MAX_TIM_PERIOD_AT_60HZ,
            },
        }, 
        .freq = eMAINS_MEAS_POWERLINE_UNKNOWN
    },
    .buffer =
    {
        .period = {0},
        .n      = 0,
    },
    .zero_cross_cbk =
    {
        .fncs = {NULL},
        .n    = 0,
    },
    .powerline_no_signal_cnt = 0,
};
// clang-format on

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static inline void             mains_meas_init_gpio(void);
static inline mains_meas_err_t mains_meas_init_main_timer(void);
static inline mains_meas_err_t mains_meas_init_zero_cros_delay_timer(void);

static inline uint32_t mains_meas_get_and_clr_main_timer_flags(void);
static inline bool     mains_meas_is_pos_edge(const uint32_t flags);
static inline bool     mains_meas_is_neg_edge(const uint32_t flags);
static inline bool     mains_meas_is_overflow(const uint32_t flags);
static inline void     mains_meas_store_period(const uint32_t period);
static inline uint32_t mains_meas_read_period(void);

static inline void mains_meas_start_zero_cross_delay(void);
static inline void mains_meas_clr_zero_cross_delay_timer_flags(void);

static inline void mains_meas_set_powerfail(void);
static inline void mains_meas_dtrmn_powerline_freq(void);
static inline void mains_meas_proc_neg_zero_cross(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize GPIO used for measuring the mains power line signal.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_init_gpio(void)
{
    GPIO_InitTypeDef config_gpio = {0};

    config_gpio.Pin       = MAINS_MEAS_MAINS_TO_MCU_PIN;
    config_gpio.Mode      = GPIO_MODE_AF_OD;
    config_gpio.Pull      = GPIO_NOPULL;
    config_gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    config_gpio.Alternate = GPIO_AF10_TIM4;
    HAL_GPIO_Init(MAINS_MEAS_MAINS_TO_MCU_PORT, &config_gpio);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize timer used for measuring the mains power line signal frequency.
 *
 * @return       #mains_meas_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static inline mains_meas_err_t mains_meas_init_main_timer(void)
{
    mains_meas_err_t        err           = eMAINS_MEAS_ERR_OK;
    TIM_HandleTypeDef       tim_handle    = {0};
    TIM_IC_InitTypeDef      config_ic     = {0};
    TIM_SlaveConfigTypeDef  config_slave  = {0};
    TIM_MasterConfigTypeDef config_master = {0};

    // Initialize TIM4 with 20ms period
    __HAL_RCC_TIM4_CLK_ENABLE();
    tim_handle.Instance = TIM4;
    tim_handle.Init.Prescaler =
        MAINS_MEAS_TIM_PRESCALER -
        1; // -1 because if prescaler of value 0 is written actual prescaling is 1
    tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_handle.Init.Period =
        MAINS_MEAS_TIM_MAX_PERIOD - 1; // -1 because of same reason as stated above
    tim_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }
    if (HAL_TIM_IC_Init(&tim_handle) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }

    // Set so that only overflow generates update interrupt and not the slave mode reset
    __HAL_TIM_URS_ENABLE(&tim_handle);

    // Set channel 1 to capture rising edges
    // Set channel 2 to capture falling edges
    config_ic.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    config_ic.ICSelection = TIM_ICSELECTION_DIRECTTI;
    config_ic.ICPrescaler = TIM_ICPSC_DIV1;
    config_ic.ICFilter    = MAINS_MEAS_TIM_IC_FILTER;
    ;
    if (HAL_TIM_IC_ConfigChannel(&tim_handle, &config_ic, (uint32_t)TIM_CHANNEL_1) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }
    config_ic.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    config_ic.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if (HAL_TIM_IC_ConfigChannel(&tim_handle, &config_ic, (uint32_t)TIM_CHANNEL_2) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }

    // Positive edge resets the counter to 0
    config_slave.SlaveMode       = TIM_SLAVEMODE_RESET;
    config_slave.InputTrigger    = TIM_TS_TI1FP1;
    config_slave.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    config_slave.TriggerFilter   = 0;
    if (HAL_TIM_SlaveConfigSynchro(&tim_handle, &config_slave) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }
    config_master.MasterOutputTrigger = TIM_TRGO_RESET;
    config_master.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&tim_handle, &config_master) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_MAIN_TIMER;
    }

    // Enable overflow, input capture on channel 1 and channel 2 interrupts
    HAL_NVIC_SetPriority(TIM4_IRQn, TIM3_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(TIM4_IRQn);
    HAL_TIM_Base_Start_IT(&tim_handle);
    HAL_TIM_IC_Start_IT(&tim_handle, (uint32_t)TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&tim_handle, (uint32_t)TIM_CHANNEL_2);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize timer used for timing of signals negative zero-cross delay.
 *
 * @return       #mains_meas_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static inline mains_meas_err_t mains_meas_init_zero_cros_delay_timer(void)
{
    mains_meas_err_t  err        = eMAINS_MEAS_ERR_OK;
    TIM_HandleTypeDef tim_handle = {0};

    // Initialize TIM17 as one pulse delay
    __HAL_RCC_TIM17_CLK_ENABLE();
    tim_handle.Instance         = TIM17;
    tim_handle.Init.Prescaler   = 0;
    tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim_handle.Init.Period      = MAINS_MEAS_TIM_NEG_ZERO_CROSS_DELAY_PERIOD -
                             1; // -1 because if period of value 0 is written actual period is 1
    tim_handle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&tim_handle) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_DELAY_TIMER;
    }
    if (HAL_TIM_OnePulse_Init(&tim_handle, TIM_OPMODE_SINGLE) != HAL_OK)
    {
        err = eMAINS_MEAS_ERR_DELAY_TIMER;
    }

    // Enable overflow interrupt
    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn, TIM17_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);
    __HAL_TIM_ENABLE_IT(&tim_handle, TIM_IT_UPDATE);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Get and clear main timer status flags.
 *
 * @return       Flags.
 */
////////////////////////////////////////////////////////////////////////////////
static inline uint32_t mains_meas_get_and_clr_main_timer_flags(void)
{
    uint32_t flags = TIM4->SR;

    TIM4->SR &= ~flags;

    return flags;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Check if main timer status flags indicate a positive edge capture.
 *
 * @param[in]    Flags.
 * @return       True if positive edge.
 */
////////////////////////////////////////////////////////////////////////////////
static inline bool mains_meas_is_pos_edge(const uint32_t flags)
{
    return (bool)((flags & TIM_SR_CC1IF) > 0);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Check if main timer status flags indicate a negative edge capture.
 *
 * @param[in]    Flags.
 * @return       True if negative edge.
 */
////////////////////////////////////////////////////////////////////////////////
static inline bool mains_meas_is_neg_edge(const uint32_t flags)
{
    return (bool)((flags & TIM_SR_CC2IF) > 0);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Check if main timer status flags indicate an overflow event.
 *
 * @param[in]    Flags.
 * @return       True if overflow event.
 */
////////////////////////////////////////////////////////////////////////////////
static inline bool mains_meas_is_overflow(const uint32_t flags)
{
    return (bool)((flags & TIM_SR_UIF) > 0);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Store period to buffer.
 *
 * @param[in]    Period.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_store_period(const uint32_t period)
{
    g_mains_meas.buffer.period[g_mains_meas.buffer.n]  = period;
    g_mains_meas.buffer.n                             += 1;
    if (g_mains_meas.buffer.n >= MAINS_MEAS_PERIOD_BUFFER_SIZE)
    {
        g_mains_meas.buffer.n = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Read main timer captured signal period value.
 *
 * @return       Period.
 */
////////////////////////////////////////////////////////////////////////////////
static inline uint32_t mains_meas_read_period(void)
{
    return TIM4->CCR1;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Start zero-cross delay timer.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_start_zero_cross_delay(void)
{
    TIM17->CR1 |= TIM_CR1_CEN;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Clear zero-cross delay timer status flags.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_clr_zero_cross_delay_timer_flags(void)
{
    TIM17->SR &= ~TIM17->SR;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Determine if power fail occurred.
 *
 * Power fail state is set to active.
 * There is no relay that could turn off power, so no other checks are needed.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_set_powerfail(void)
{
    g_mains_meas.powerfail = eMAINS_MEAS_POWERFAIL_ACTIVE;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Determine power line frequency based on measured periods.
 *
 * For power line frequency to be set to either 50 Hz or 60 Hz more than
 * #MAINS_MEAS_MIN_N_VALID_POWERLINE_MEAS many measurements must be in that category.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_dtrmn_powerline_freq(void)
{
    uint16_t i;
    uint16_t freq_50_cnt = 0;
    uint16_t freq_60_cnt = 0;

    // Buffer filled and wrapped around
    if (g_mains_meas.buffer.n == 0)
    {
        for (i = 0; i < MAINS_MEAS_PERIOD_BUFFER_SIZE; i++)
        {
            if ((g_mains_meas.buffer.period[i] > g_mains_meas.powerline.period_limits.hz50.min) &&
                (g_mains_meas.buffer.period[i] < g_mains_meas.powerline.period_limits.hz50.max))
            {
                freq_50_cnt++;
            }

            if ((g_mains_meas.buffer.period[i] > g_mains_meas.powerline.period_limits.hz60.min) &&
                (g_mains_meas.buffer.period[i] < g_mains_meas.powerline.period_limits.hz60.max))
            {
                freq_60_cnt++;
            }
        }

        // Enough measurements must fall into appropriate category
        if (freq_50_cnt > MAINS_MEAS_MIN_N_VALID_POWERLINE_MEAS)
        {
            g_mains_meas.powerline.freq = eMAINS_MEAS_POWERLINE_50HZ;
        }
        else if (freq_60_cnt > MAINS_MEAS_MIN_N_VALID_POWERLINE_MEAS)
        {
            g_mains_meas.powerline.freq = eMAINS_MEAS_POWERLINE_60HZ;
        }
        else
        {
            g_mains_meas.powerline.freq = eMAINS_MEAS_POWERLINE_UNKNOWN;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Process negative zero-cross.
 *
 * Call all registered negative zero-cross callback functions.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void mains_meas_proc_neg_zero_cross(void)
{
    uint32_t i;

    for (i = 0; i < g_mains_meas.zero_cross_cbk.n; i++)
    {
        if (g_mains_meas.zero_cross_cbk.fncs[i] != NULL)
        {
            g_mains_meas.zero_cross_cbk.fncs[i]();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MAINS_MEAS_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * Process negative zero-cross.
 *
 * @note This function handles TIM17 global interrupt.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
    mains_meas_clr_zero_cross_delay_timer_flags();

#if (MAINS_MEAS_EN_ZERO_CROSS_DETECT_MEAS == 1)
    HAL_GPIO_WritePin(
        MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PORT, MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PIN,
        GPIO_PIN_RESET
    );
#endif

    mains_meas_proc_neg_zero_cross();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Process power line signal measurement.
 *
 * In case of positive edge event store the measured power line signals period. In
 * case of negative edge event start the zero-cross delay timer. And in case of
 * overflow event store period with value 0. Lastly determine the power line frequency
 * and power fail state.
 *
 * @note This function handles TIM4 global interrupt.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void TIM4_IRQHandler(void)
{
    uint32_t flags        = mains_meas_get_and_clr_main_timer_flags();
    uint16_t mains_period = mains_meas_read_period();

    if (mains_meas_is_pos_edge(flags))
    {
#if (MAINS_MEAS_EN_ZERO_CROSS_DETECT_MEAS == 1)
        HAL_GPIO_WritePin(
            MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PORT, MAINS_MEAS_ZERO_CROSS_DETECT_MEAS_PIN,
            GPIO_PIN_SET
        );
#endif

        mains_meas_store_period(mains_period);
        mains_meas_dtrmn_powerline_freq();
    }

    if (mains_meas_is_neg_edge(flags))
    {
        mains_meas_start_zero_cross_delay();
    }

    // If timer overflowed that means there is no signal on pin MAINS_TO_MCU
    // which means there is no mains line power signal present.
    // TODO: Should we clear powerfail error when signal returns, automatically,
    // or like in the case of WM wait until PU reads status packet so PU knows that MU
    // had powerfail error?
    if (mains_meas_is_overflow(flags))
    {
        g_mains_meas.powerline_no_signal_cnt++;
        if (g_mains_meas.powerline_no_signal_cnt >= MAINS_MEAS_POWERLINE_NO_SIGNAL_LIMIT)
        {
            mains_meas_set_powerfail();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize mains power line measurement module.
 *
 * Initialize GPIO and timer used for measuring the power line signal. And initialize
 * a separate timer used for negative zero-cross delay timing.
 *
 * @return       #mains_meas_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
mains_meas_err_t mains_meas_init(void)
{
    mains_meas_err_t err = eMAINS_MEAS_ERR_OK;

    mains_meas_init_gpio();
    err |= mains_meas_init_main_timer();
    err |= mains_meas_init_zero_cros_delay_timer();

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Register negative zero-cross callback function.
 *
 * @param[in]    fnc     Callback function.
 * @return       #mains_meas_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
mains_meas_err_t mains_meas_register_neg_zero_cross_cbk(pfnc_zero_cross_cbk fnc)
{
    mains_meas_err_t err = eMAINS_MEAS_ERR_OK;

    if (fnc == NULL)
    {
        err |= eMAINS_MEAS_ERR_CBK_NULL;
    }

    if (g_mains_meas.zero_cross_cbk.n >= MAINS_MEAS_MAX_NEG_ZERO_CROSS_CBKS)
    {
        err |= eMAINS_MEAS_ERR_MAX_CBKS_REGISTERED;
    }

    if (err == eMAINS_MEAS_ERR_OK)
    {
        g_mains_meas.zero_cross_cbk.fncs[g_mains_meas.zero_cross_cbk.n] = fnc;
        g_mains_meas.zero_cross_cbk.n++;
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Get power fail state.
 *
 * @return       #mains_meas_powerfail_t.
 */
////////////////////////////////////////////////////////////////////////////////
mains_meas_powerfail_t mains_meas_get_powerfail_state(void)
{
    return g_mains_meas.powerfail;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Clear power fail state.
 *
 * @return       #none.
 */
////////////////////////////////////////////////////////////////////////////////
void mains_meas_clear_powerfail(void)
{
    g_mains_meas.powerfail               = eMAINS_MEAS_POWERFAIL_NOT_ACTIVE;
    g_mains_meas.powerline_no_signal_cnt = 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Get measured power line frequency.
 *
 * @return       #mains_meas_powerline_freq_t.
 */
////////////////////////////////////////////////////////////////////////////////
mains_meas_powerline_freq_t mains_meas_get_powerline_freq(void)
{
    return g_mains_meas.powerline.freq;
}

uint16_t mains_meas_get_freq_hz_100(void)
{
    //frez_hz_100 = SYSTEM_CORE_CLOCK_HZ / MAINS_MEAS_TIM_PRESCALER / 
        // (float)g_mains_meas.buffer.period[0] * 100.0f;
    // This can be shortened to 170e6 / 425 * 100 / period = 4e7 / period
    // It divides nicely to integer so no need for float here.

    const uint16_t frez_hz_100 = 40000000 / g_mains_meas.buffer.period[0]; 
    return frez_hz_100;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
