/**
 * @file     eps.c
 * @brief    Electronic Pressure Sensor - EPS (water level sensor) module
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup EPS
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stm32g0xx_hal.h"
#include "eps.h"
#include "main.h"
#include "apptime.h"
#include "tim.h"
#include "trace.h"
#include "constants_common.h"
#include "emrgcy_drain.h"
#include "system_stm32g0xx.h"

/**
 * Used for qualification testing (standards compliance testing)
 */
#ifdef QUALIFICATION_TEST
#include "test_switch.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////

#if (CONF_EPS_LOG == 1)
#define EPS_LOG  /**< Debug macro for TRACE */
#endif

#define EPS_VERSION         "1.0.0"                              /**< Module version. */
#define EPS_NAME            "Safe - Electronic Pressure Sensor"  /**< Module name. */
#define EPS_NAME_SHORT      "SEP"                                /**< SAFE Module short name. */

/**
 * Duty cycle
 */
#define SIGNAL_MIN_DUTY_CYCLE             45     /**< Min Duty cycle (DC) for valid data. */
#define SIGNAL_MAX_DUTY_CYCLE             55     /**< Max DC for valid data. */

/**
 * Water levels
 */
#define SIGNAL_LEVEL_LOW_UM               93000  /**< Below this level it is safe to unlock door. */
#define SIGNAL_LEVEL_HIGH_UM              95000  /**< Above this level it is unsafe to unlock door. */
#define SIGNAL_WATER_LEVEL_EMPTY_UM       10000  /**< Water level empty. */

/**
 * Runtime plausibility
 */
#define RUNTIME_PLAUS_CHECK_PERIOD_MS     1000   /**< Runtime plausibility check period. */
#define ENOUGH_SIGNAL_DIFF_UM             30000  /**< Signal change above this level after valve or drain shuts OFF, we know hose is attached and is working. */
#define MAX_SIGNAL_DROP_TO_EMPTY_UM       50000  /**< Max signal drop, anything larger means hose came off. */
#define MIN_SIGNAL_DIFF_SENSOR_OK_UM      2000   /**< Min signal delta that lets us know that sensor is attached and working. */
#define MIN_DURATION_VALVE_ON_CHECK_S     20     /**< Minimum duration of time that valve must be ON for plausibility check related to valve being on, to be carried out. */
#define MIN_VALVE_DIFF_PER_45S_UM         2500   /**< Minimum signal difference while valve is on. PSA spec - min diff is 3mm/45s. */
#define MIN_DRAIN_DIFF_PER_45S_UM         3000   /**< Minimum signal difference while drain pump is on. */

/**
 * Errors
 */
#define CHECK_FOR_ERRORS_PERIOD_MS        1000   /**< Period of time to do error check. */
#define ERROR_CNT_THRESHOLD               10     /**< Maximum number of errors in CHECK_FOR_ERRORS_PERIOD_MS milliseconds. */

/**
 * Filter
 */
#define EPS_FILTER_LEVEL                  5      /**< Low pass filter "level". */


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * EPS error types
 */
typedef enum
{
    eEPS_ERR_ISR = 0,  /**< Error with processors interrupts (not triggering). */
    eEPS_ERR_SIG = 1,  /**< Signal error. Invalid signal levels or signal not present at all. */
    eEPS_ERR_NUM = 2   /**< Number of error types. */
} eps_err_t;

/**
 * EPS measurement struct with unit-less (timer ticks) members.
 */
typedef struct
{
    int32_t  duty;    /**< Duty cycle (falling edge) [timer ticks]. */
    uint32_t period;  /**< Period (rising edge) [timer ticks]. */
} eps_meas_tim_t;

/**
 * EPS measurement struct.
 */
typedef struct
{
    int32_t dc;  /**< Duty cycle [%]. */
    int32_t um;  /**< Water level [um]. */
} eps_meas_t;

/**
 * EPS module variables
 */
typedef struct
{
    lock_state_t  lock_state;                       /**< State of lock. */
    drain_state_t drain_state;                      /**< State of drain pump. */
    valve_state_t valve_state;                      /**< State of valves. */

    bool                    is_powerup;               /**< Power up flag, true - system just powered up. */
    bool                    emrgcy_drain_safe_window; /**< Safe window active flag. */
    eps_level_t             level;                    /**< Final EPS level state, LOW or HIGH. */
    volatile eps_meas_tim_t meas_tim;                 /**< Raw unit-less (timer ticks) measurement data. */
    eps_meas_t              meas;                     /**< Raw measurement data in human readable units. */
    uint32_t                timer_freq;               /**< Timer Frequency used by EPS (can be calibrated during FCT test). */
    eps_timer_freq_div_t    timer_freq_div;           /**< Division of MCU timer (standby gets /4). */
    struct
    {
        int32_t measured_eps_freq_offset_mHz;      /**< Measured EPS offset during production test. Includes EPS and timer frequency errors. */
    } calibration;                                  /**< Struct regarding calibration. */
    struct
    {
        struct
        {
            uint32_t curr;                          /**< Current ISR count. */
            uint32_t prev;                          /**< Previous ISR count. */
        } cnt;
        bool is_operate;                            /**< ISR is working properly flag. */
    } isr;                                          /**< Struct about ISR functionality. */
    struct
    {
        struct
        {
            int32_t raw;                            /**< Raw. */
            int32_t filtered;                       /**< Filtered. */
            int32_t filter_sum;                     /**< Filtered sum used by low pass filter. */
            bool    initialize_filter;              /**< Flag for low pass filter. */
        } um;                                       /**< Struct used to calculate water level in um */
        eps_level_t level;                          /**< Level based on calculations of period. */
        bool is_valid;                              /**< Signal is valid flag (within allowable limits) */
    } sig;                                          /**< Struct about EPS signal (period, level, validity) */
    struct
    {
        apptime_t runtime_plaus_ts;                 /**< Timestamp of last runtime plausibility check. */
        int32_t  um_prev;                           /**< Previous period value, used by runtime plaus check. */
        bool is_plaus;                              /**< EPS plausibility flag (true - trustworty, false - do not trust). */
    } sens;                                         /**< Struct about EPS plausibility (powerup and runtime) */
    struct
    {
        struct
        {
            uint32_t type[eEPS_ERR_NUM];            /**< Error type counters. */
            uint32_t total;                         /**< Sum of all error types. */
            uint32_t active;                        /**< Active error counter on basis of which we determine if we set the error flag. */
        } cnt;
        apptime_t chk_start_time;                   /**< Timestamp of last error check. */
        bool is_limit;                              /**< Error limit has been reach flag. */
    } error;                                        /**< Struct about EPS errors. */
} eps_t;


////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern TIM_HandleTypeDef htim3;   /**< Timer handle used by EPS. */


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
/**
 * EPS module variables instance.
 */
static eps_t g_eps =
{
    .lock_state = eLOCK_UNLOCKED,
    .drain_state = eDRAIN_OFF,
    .valve_state = eVALVE_OFF,

    .is_powerup = true,
    .emrgcy_drain_safe_window = false,
    .level = eEPS_HIGH,
    .meas_tim =
    {
        .period = 0,
        .duty = 0,
    },
    .meas =
    {
        .dc = 0,
        .um = 0,
    },
    .timer_freq = EPS_TIMER_DEFAULT_FREQ,
    .timer_freq_div = EPS_TIMER_FREQ_NODIV,
    .calibration =
    {
        .measured_eps_freq_offset_mHz = 0,
    },
    .isr =
    {
        .cnt =
        {
            .curr = 0,
            .prev = 0,
        },
        .is_operate = false,
    },
    .sig =
    {
        .um =
        {
            .raw = 0,
            .filtered = 0,
            .filter_sum = 0,
            .initialize_filter = true,
        },
        .level = eEPS_HIGH,
        .is_valid = false,
    },
    .sens =
    {
        .runtime_plaus_ts = 0,
        .um_prev = 0,
        .is_plaus = false,
    },
    .error =
    {
        .cnt =
        {
            .type = {0},
            .total = 0,
        },
        .chk_start_time = 0,
        .is_limit = false,
    },
};


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static eps_init_status_t eps_timer_init(void);
static bool eps_timer_deinit(void);
static eps_init_status_t eps_gpio_init(void);
static void eps_timer_isr_incr(void);
static void eps_chk_isr(void);
static void eps_calc_level(void);
static void eps_lpf_init(int32_t *sum, int32_t start_value);
static int32_t eps_lpf(int32_t *sum, int32_t x);
static eps_level_t eps_calc_hyst(eps_level_t level, int32_t um);
static eps_level_t eps_set_level(void);
static int32_t eps_diehl_freq_to_um(uint32_t period);
static eps_meas_t eps_convert_meas(eps_meas_tim_t et);
static void eps_chk_meas(void);
static bool eps_is_meas_valid(eps_meas_t meas);
static void eps_chk_sensor(void);
static bool eps_is_powerup_plaus(void);
static bool eps_is_runtime_plaus(void);
static void eps_overwrite_plaus_after_forced_drain_and_unlocked_door();
static void eps_chk_error(void);
static void eps_report_error(eps_err_t err);
static void eps_timer_ic_isr(void);
static void eps_timer_ovf_isr(void);
static bool eps_timer_chk_and_clr_ic(void);
static bool eps_timer_chk_and_clr_ovf(void);
static bool eps_calibration_timer_freq_is_valid(uint32_t freq);


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * EPS initialization routine.
 *
 * @return       None.
 */
eps_init_status_t eps_init(void)
{
    eps_init_status_t status = eEPS_INIT_OK;

    // initialize timer
    status |= eps_timer_init();

    // initialize gpio
    status |= eps_gpio_init();

    // Just to make doubly sure that the values are correct!
    eps_calibration_eps_freq_offset_reset();
    eps_calibration_timer_freq_reset();

#ifdef EPS_LOG
    if (status == eEPS_INIT_OK)
        TRACE_INF("%s ::: eps_init success", EPS_NAME_SHORT);
    else
        TRACE_FAT("%s ::: eps_init failed (status: %d)", EPS_NAME_SHORT, status);
#endif

    return status;
}

/**
 * Main run/proc function for EPS module.
 *
 * @param[in]    lock    Door locked state.
 * @param[in]    drain   Drain pump state.
 * @param[in]    valve   Valve state.
 * @return       None.
 */
void eps_proc(lock_state_t lock, drain_state_t drain, valve_state_t valve)
{
    g_eps.lock_state = lock;
    g_eps.drain_state = drain;
    g_eps.valve_state = valve;

    // Check ISR (uC) is working
    eps_chk_isr();
    if (g_eps.isr.is_operate == false)
    {
        eps_report_error(eEPS_ERR_ISR);
    }

    // Check signal - are duty and period within limits? This includes if there is no signal present since duty and period are then set to 0
    eps_chk_meas();
    if (g_eps.sig.is_valid == false)
    {
        eps_report_error(eEPS_ERR_SIG);
    }
    else
    {
        eps_calc_level();
    }

    // Check for errors
    eps_chk_error();

    // Check sensor measurement is plausible (power-up, runtime plausibility)
    eps_chk_sensor();

    // Check if emergency drain safe window is active
    g_eps.emrgcy_drain_safe_window = emrgcy_drain_is_safe_window();

    // Check if forced drain is complete and door is unlocked, then overwrite plaus to TRUE
    eps_overwrite_plaus_after_forced_drain_and_unlocked_door();

    // Set level
    g_eps.level = eps_set_level();

#ifdef EPS_LOG
    static eps_level_t lvl = eEPS_HIGH;
    static const char* level_to_string[] = {"HIGH", "LOW"};
    if (g_eps.level != lvl)
    {
        TRACE_DEB("%s ::: EPS level %s -> %s (%ldum rtp:%d)", EPS_NAME_SHORT, level_to_string[lvl],
            level_to_string[g_eps.level], g_eps.meas.um, g_eps.sens.is_plaus);
        lvl = g_eps.level;
    }
#endif
}

/**
 * EPS Timer periphery ISR callback.
 *
 * @return       None.
 */
void eps_timer_isr(void)
{
#ifdef QUALIFICATION_TEST
    if (g_test_switch.eps.isr_fail == true)
    {
        eps_timer_chk_and_clr_ic();
        eps_timer_chk_and_clr_ovf();
        return;
    }
#endif

    if (eps_timer_chk_and_clr_ic() == true)
    {
        eps_timer_ic_isr();
        eps_timer_isr_incr();
    }

    if (eps_timer_chk_and_clr_ovf() == true)
    {
        eps_timer_ovf_isr();
        eps_timer_isr_incr();
    }
}

/**
 * Power up EPS module
 *
 * @return    None.
 */
void eps_power_up(void)
{
    HAL_GPIO_WritePin(EPS_POWER_EN_PORT, EPS_POWER_EN_PIN, GPIO_PIN_SET);
    eps_timer_init();
    // Set flag to reinitialize filter to not have to wait too long to reach latest level
    g_eps.sig.um.initialize_filter = true;
}

/**
 * Power down EPS module.
 * NOTE: I think there was a bug, where TIM clock was disabled, and TIM IRQ flag (UIF) was set just after
 * disabling clock so after that IRQ handler couldn't reset that flag and it was constantly executing TIM3 IRQ handler
 * thereby triggering watchdog.
 *
 * @return    None.
 */
void eps_power_down(void)
{
    eps_timer_deinit();
    HAL_GPIO_WritePin(EPS_POWER_EN_PORT, EPS_POWER_EN_PIN, GPIO_PIN_RESET);
}

/**
 * Get EPS level.
 *
 * @return     EPS level (LOW/HIGH).
 */
eps_level_t eps_get_level(void)
{
    return g_eps.level;
}

/**
 * Get EPS timer period value.
 *
 * @return    Timer period.
 */
uint32_t eps_get_period(void)
{
    // return raw/direct value, straight from isr
    return g_eps.meas_tim.period;
}

/**
 * Get water level in um.
 *
 * @return    Water level in um.
 */
int32_t eps_get_sig_um(void)
{
    return g_eps.sig.um.filtered;
}

/**
 * Get EPS timer periphery ISR count.
 *
 * @return     EPS timer ISR count.
 */
uint32_t eps_get_isr_cnt(void)
{
    return g_eps.isr.cnt.curr;
}

/**
 * Check if EPS is in error.
 *
 * @return     True if EPS is in error.
 */
bool eps_is_error(void)
{
    return g_eps.error.is_limit;
}

/**
 * Check if EPS level is plausible.
 *
 * @return     True if EPS is plausible.
 */
bool eps_is_plausible(void)
{
    return g_eps.sens.is_plaus;
}

/**
 * Check if EPS level is "safe". Safe meaning there are no errors and level is LOW.
 *
 * @return     True if EPS is safe.
 */
bool eps_is_safe(void)
{
    return eps_is_error() == false && g_eps.sig.level == eEPS_LOW;
}

/**
 * Check if EPS level is valid and "empty".
 *
 * @return     True if EPS signal is valid and water level is empty.
 */
bool eps_is_level_empty(void)
{
    if (eps_is_safe() == true &&
        eps_get_sig_um() <= SIGNAL_WATER_LEVEL_EMPTY_UM)
    {
        return true;
    }

    return false;
}

/**
 * @brief Sets the division factor of EPS timer (/4 in standby or /1 in active running)
 *
 * @param div of type @eps_timer_freq_div_t
 */
void eps_set_timer_mode(eps_timer_freq_div_t div)
{
    /* if/else is to make sure timer_freq_div is valid */
    if (div == EPS_TIMER_FREQ_DIV4)
        g_eps.timer_freq_div = div;
    else
        g_eps.timer_freq_div = EPS_TIMER_FREQ_NODIV;
}

/**
 * @brief This function is called by class A to set the actual measured frequency of the timer
 *
 * @param measured_timer_freq
 * @return true if succeeds
 * @return false if timer_freq is outside valid limits
 */
bool eps_calibration_timer_freq_set(uint32_t measured_timer_freq)
{
    if (eps_calibration_timer_freq_is_valid(measured_timer_freq))
    {
        g_eps.timer_freq = measured_timer_freq;
        return true;
    }

    return false;
}

/**
 * @brief Reset measured timer frequency back to default.
 *
 */
void eps_calibration_timer_freq_reset(void)
{
    g_eps.timer_freq = EPS_TIMER_DEFAULT_FREQ;
}

/**
 * @brief This function is called by class A to set the EPS calibration offset.
 *
 * @param measured_eps_freq_offset_mHz Value in milliHz
 * @return true if succeeds
 * @return false if parameter is outside valid limits
 */
bool eps_calibration_eps_freq_offset_set(int32_t measured_eps_freq_offset_mHz)
{
    if (measured_eps_freq_offset_mHz >= EPS_MIN_CALIBRATION_OFFSET_FREQ_MILLIHZ &&
        measured_eps_freq_offset_mHz <= EPS_MAX_CALIBRATION_OFFSET_FREQ_MILLIHZ)
    {
        g_eps.calibration.measured_eps_freq_offset_mHz = measured_eps_freq_offset_mHz;
        return true;
    }

    return false;
}

/**
 * @brief Reset EPS calibration offset back to 0.
 *
 */
void eps_calibration_eps_freq_offset_reset(void)
{
    g_eps.calibration.measured_eps_freq_offset_mHz = 0;
}



////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * EPS timer periphery initialization.
 *
 * @return     EPS init status.
 */
static eps_init_status_t eps_timer_init(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    // initialize HAL timer
    status |= MX_TIM3_Init();

    // start timer
    status |= HAL_TIM_Base_Start_IT(&htim3); // enable update irq

    // start DC input capture (no need for irq)
    status |= HAL_TIM_IC_Start(&htim3, EPS_DUTY_TIM_IC_CHANNEL);

    // start period input capture (enable ic irq)
    status |= HAL_TIM_IC_Start_IT(&htim3, EPS_PERIOD_TIM_IC_CHANNEL);

    if (status != HAL_OK)
    {
        return eEPS_INIT_TIMER_FAIL;
    }

    return eEPS_INIT_OK;
}

/**
 * EPS timer periphery deinitialization.
 *
 * @return     True if deinit was successful.
 */
bool eps_timer_deinit(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    // start timer
    status |= HAL_TIM_Base_Stop_IT(&htim3); // enable update irq

    // start DC input capture (no need for irq)
    status |= HAL_TIM_IC_Stop(&htim3, EPS_DUTY_TIM_IC_CHANNEL);

    // start period input capture (enable ic irq)
    status |= HAL_TIM_IC_Stop_IT(&htim3, EPS_PERIOD_TIM_IC_CHANNEL);

    // initialize HAL timer
    status |= HAL_TIM_Base_DeInit(&htim3);

    if (status != HAL_OK)
    {
        return false;
    }

    return true;
}

/**
 * EPS GPIO periphery initialization.
 *
 * @return     EPS init status.
 */
static eps_init_status_t eps_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Initialize transistor to supply EPS with 5V

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin = EPS_POWER_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EPS_POWER_EN_PORT, &GPIO_InitStruct);

    eps_power_up();

    // Timer GPIO is initialized via timer in HAL_TIM_Base_MspInit
    return eEPS_INIT_OK;
}

/**
 * Increment EPS timer IRQ counter.
 *
 * @return     None.
 */
static void eps_timer_isr_incr(void)
{
    g_eps.isr.cnt.curr++;
}

/**
 * Check if EPS timer periphery IRQ is working properly.
 *
 * @return     None.
 */
static void eps_chk_isr(void)
{
    __disable_irq();
    uint32_t cnt_curr = g_eps.isr.cnt.curr;
    __enable_irq();

    if (g_eps.isr.cnt.prev == cnt_curr)
    {
        g_eps.isr.is_operate = false;
    }
    else
    {
        g_eps.isr.is_operate = true;
    }

    g_eps.isr.cnt.prev = cnt_curr;
}

/**
 * Calculate EPS level based on filtered timer values.
 *
 * @return       None.
 */
static void eps_calc_level(void)
{
    // initialize low pass filter with raw value
    if (g_eps.sig.um.initialize_filter == true)
    {
        eps_lpf_init(&g_eps.sig.um.filter_sum, g_eps.sig.um.raw);
        g_eps.sig.um.initialize_filter = false;
    }

    g_eps.sig.um.filtered = eps_lpf(&g_eps.sig.um.filter_sum, g_eps.sig.um.raw);
    g_eps.sig.level = eps_calc_hyst(g_eps.sig.level, g_eps.sig.um.filtered);
}

/**
 * Initialize low-pass filter.
 *
 * @param[in]    sum          Accumulator used in this type of filter implementation.
 * @param[in]    start_value  Starting value, so as to not need to ramp up to initial value.
 * @return       None.
 */
static void eps_lpf_init(int32_t *sum, int32_t start_value)
{
    *sum = start_value << EPS_FILTER_LEVEL;
}

/**
 * Low pass filter.
 *
 * @param[in]    sum   Filter accumulator.
 * @param[in]    x     Input value.
 * @return       Filtered value.
 */
static int32_t eps_lpf(int32_t *sum, int32_t x)
{
    *sum = (*sum - (*sum >> EPS_FILTER_LEVEL)) + x;

    return *sum >> EPS_FILTER_LEVEL;
}

/**
 * Determine EPS level based on period, using a hysteresis to help with noise.
 *
 * @param[in]    level    Current EPS level.
 * @param[in]    um       EPS level in um.
 * @return       EPS level.
 */
static eps_level_t eps_calc_hyst(eps_level_t level, int32_t um)
{
    if (um >= SIGNAL_LEVEL_HIGH_UM)
    {
        level = eEPS_HIGH;
    }
    else if (um <= SIGNAL_LEVEL_LOW_UM)
    {
        level = eEPS_LOW;
    }
    else
    { // do nothing <- this implements hysteresis
    }

    return level;
}

/**
 * Determine EPS level taking into account everything that would overwrite calculated level.
 *
 * @return     EPS level.
 */
static eps_level_t eps_set_level(void)
{
    eps_level_t level;

    if (g_eps.error.is_limit == false && g_eps.sens.is_plaus == true) // if sensor is OK
    {
        level = g_eps.sig.level;
    }
    else if (g_eps.emrgcy_drain_safe_window == true)
    {
        level = eEPS_LOW;
    }
    else if (g_eps.error.is_limit == true)
    {
        level = eEPS_HIGH;
    }
    else if (g_eps.sens.is_plaus == false)
    {
        level = eEPS_HIGH;
    }
    else // SHOULD NEVER REACH HERE
    {
#ifdef EPS_LOG
        TRACE_WAR("%s ::: eps_set_level ELSE! (plaus:%d err:%d ED:%d)",
            g_eps.sens.is_plaus, g_eps.error.is_limit, g_eps.emrgcy_drain_safe_window);
#endif
        level = eEPS_HIGH;
    }

    return level;
}

/**
 * DIEHL equation to calculate water level height in micrometers from frequency: "h[um] = (621000 miliHz - f[miliHz]) * 2,5"
 *
 * @param[in]    period    Timer period in timer tick units.
 * @return       Water level in um.
 */
static int32_t eps_diehl_freq_to_um(uint32_t period)
{
    if (period == 0)
        return 0;

    uint32_t freq_mHz = (((uint32_t)(g_eps.timer_freq * 100U)) / period) * 10; // * 100 since *1000 would overflow uint32_t since 16e9 > 2**32

    if (g_eps.timer_freq_div == EPS_TIMER_FREQ_DIV4)
    {
        freq_mHz /= 4;
    }

    freq_mHz = freq_mHz + g_eps.calibration.measured_eps_freq_offset_mHz;
    int32_t partial_result = (621 * 1000) - freq_mHz;
    int32_t um = (partial_result << 1) + (partial_result >> 1); // x * 2.5 without using floating point number -> (x * 2) + (x / 2)

    return um;
}

/**
 * Convert eps_meas_tim_t struct to eps_meas_t. Convert struct in timer tick units to more human friendly units (mm, %)
 *
 * @param[in]    et    eps_meas_tim_t struct.
 * @return       eps_meas_t struct.
 */
static eps_meas_t eps_convert_meas(eps_meas_tim_t et)
{
    eps_meas_t e;

    e.dc = et.period > 0 ? (100 * et.duty) / et.period : 0;
    e.um = eps_diehl_freq_to_um(et.period);

    return e;
}

/**
 * Check measurement and save it if valid.
 *
 * @return       None.
 */
static void eps_chk_meas(void)
{
    // Convert timer tick measurement to more human friendly units (DC [%], EPS level [mm])
    __disable_irq();
    eps_meas_tim_t et = g_eps.meas_tim;
    __enable_irq();
    g_eps.meas = eps_convert_meas(et);

    g_eps.sig.is_valid = eps_is_meas_valid(g_eps.meas);

    if (g_eps.sig.is_valid == true)
    {
        g_eps.sig.um.raw = g_eps.meas.um;
    }
}

/**
 * Check if measurement is within limits.
 *
 * @param[in]    meas    Measurement to check if valid.
 * @return       True if valid.
 */
static bool eps_is_meas_valid(eps_meas_t meas)
{
    bool valid = true;

    // Check if duty cycle and frequency are out of range
    if (meas.dc < SIGNAL_MIN_DUTY_CYCLE      || meas.dc > SIGNAL_MAX_DUTY_CYCLE ||
        meas.um < (EPS_SIGNAL_MIN_MM * 1000) || meas.um > (EPS_SIGNAL_MAX_MM * 1000))
    {
        valid = false;
#ifdef EPS_LOG
        static uint32_t timestamp = 0;
        if (apptime_is_elapsed(timestamp, 1000))
        {
            timestamp = apptime_get_ms();
            TRACE_DEB("%s ::: signal error - duty:%ld [%d-%d] %ldum [%d, %d]", EPS_NAME_SHORT, meas.dc,
                SIGNAL_MIN_DUTY_CYCLE, SIGNAL_MAX_DUTY_CYCLE, meas.um, EPS_SIGNAL_MIN_MM * 1000, EPS_SIGNAL_MAX_MM * 1000);
        }
#endif
    }

    return valid;
}

/**
 * Check if EPS measurements are trustworthy/plausible.
 *
 * @return       None.
 */
static void eps_chk_sensor(void)
{
    if (g_eps.is_powerup == true)
    {
        g_eps.sens.is_plaus = eps_is_powerup_plaus();
    }
    else if (apptime_is_elapsed(g_eps.sens.runtime_plaus_ts, RUNTIME_PLAUS_CHECK_PERIOD_MS))
    {
        g_eps.sens.runtime_plaus_ts = apptime_get_ms();
        g_eps.sens.is_plaus = eps_is_runtime_plaus();
        g_eps.sens.um_prev = g_eps.sig.um.filtered;
    }
}

/**
 * Determine EPS plausibility at power up.
 *
 * @return       True if plausible.
 */
static bool eps_is_powerup_plaus(void)
{
    bool is_plaus = false;

    if (g_eps.lock_state == eLOCK_UNKNOWN)
    {
        is_plaus = false;
#ifdef EPS_LOG
        static int onetime = 1;
        if (onetime)
        {
            onetime = 0;
            TRACE_DEB("%s ::: Power Up Plausibility FALSE - LOCK UNKNOWN waiting to get valid door lock result", EPS_NAME_SHORT);
        }
#endif
    }
    else
    {
        g_eps.is_powerup = false;

        if (g_eps.lock_state == eLOCK_LOCKED && g_eps.sig.level == eEPS_LOW)
        {
            is_plaus = false;
        }
        else
        {
            is_plaus = true;
        }

#ifdef EPS_LOG
        static const char* txt[] = {"UNKNOWN", "UNLOCKED", "LOCKED"};
        TRACE_DEB("%s ::: Power Up Plausibility %s (LOCK: %s, EPS: %s)", EPS_NAME_SHORT, is_plaus ? "TRUE" : "FALSE",
            txt[g_eps.lock_state],
            g_eps.sig.level == eEPS_LOW ? "LOW" : "HIGH");
#endif
    }

    return is_plaus;
}

/**
 * Determine EPS plausibility at run time.
 *
 * @return       True if plausible.
 */
static bool eps_is_runtime_plaus(void)
{
    // Save all global variables to local ones
    valve_state_t valve_state = g_eps.valve_state;
    drain_state_t drain_state = g_eps.drain_state;
    int32_t sig_um = g_eps.sig.um.filtered;
    bool is_plaus = g_eps.sens.is_plaus;

    ///////////////////////////////////////////////////////////////
    // VALVE
    ///////////////////////////////////////////////////////////////
    static valve_state_t valve_state_prev = eVALVE_OFF;
    static int32_t valve_start_sig = 0;
    static apptime_t valve_start_time = 0;

    int valve_diff = valve_state - valve_state_prev;
    valve_state_prev = valve_state;

    if (valve_diff > 0) // valve turned on
    {
        valve_start_sig = sig_um;
        valve_start_time = apptime_get_ms();
#ifdef EPS_LOG
        TRACE_DEB("%s ::: RTP - valve ON (%ldum)", EPS_NAME_SHORT, valve_start_sig);
#endif
    }
    else if (valve_diff < 0) // valve turned off
    {
        int32_t valve_stop_sig = sig_um;
        apptime_t duration_s = (apptime_get_ms() - valve_start_time) / 1000;
        int min_calculated_diff = (MIN_VALVE_DIFF_PER_45S_UM * duration_s) / 45;
        int valve_sig_diff_um = valve_stop_sig - valve_start_sig;

        if (valve_sig_diff_um > ENOUGH_SIGNAL_DIFF_UM || valve_sig_diff_um > min_calculated_diff) // using min_calculated_diff for potential short valve bursts
        {
            is_plaus = true;
#ifdef EPS_LOG
            TRACE_DEB("%s ::: RTP (TRUE) - Valve OFF (was ON %ds), enough signal diff detected (new:%ldum prev:%ldum diff:%d > %d or %d)", EPS_NAME_SHORT,
                duration_s, valve_stop_sig, valve_start_sig, valve_sig_diff_um, min_calculated_diff, ENOUGH_SIGNAL_DIFF_UM);
#endif
        }
        else if (duration_s >= MIN_DURATION_VALVE_ON_CHECK_S)
        {
            is_plaus = false;
#ifdef EPS_LOG
            TRACE_DEB("%s ::: RTP (FALSE) - Valve OFF (was ON %ds), not enough signal diff detected (new:%ldum prev:%ldum diff:%d < %d or %d)", EPS_NAME_SHORT,
                duration_s, valve_stop_sig, valve_start_sig, valve_sig_diff_um, min_calculated_diff, ENOUGH_SIGNAL_DIFF_UM);
#endif
        }
        else
        {
#ifdef EPS_LOG
            TRACE_DEB("%s ::: RTP (no change (%s)) - Valve OFF (was ON %ds), too short valve ON time and not enough sig diff detected (new:%ldum prev:%ldum diff:%d < %d or %d)", EPS_NAME_SHORT,
                is_plaus ? "TRUE" : "FALSE", duration_s, valve_stop_sig, valve_start_sig, valve_sig_diff_um, min_calculated_diff, ENOUGH_SIGNAL_DIFF_UM);
#endif
        }
    }

    ///////////////////////////////////////////////////////////////
    // DRAIN
    ///////////////////////////////////////////////////////////////
    static bool ignore_drain_check = false;     // ignore drain check
    static drain_state_t drain_state_prev = eDRAIN_OFF;
    static int32_t drain_start_sig = 0;
    static apptime_t drain_start_time = 0;

    int drain_diff = drain_state - drain_state_prev;
    drain_state_prev = drain_state;

    if (drain_diff > 0) // drain turned on
    {
        drain_start_sig = sig_um;
        drain_start_time = apptime_get_ms();
#ifdef EPS_LOG
        TRACE_DEB("%s ::: RTP - drain ON (%ldum)", EPS_NAME_SHORT, drain_start_sig);
#endif
    }
    else if (drain_diff < 0) // drain turned off
    {
    	int32_t drain_stop_sig = sig_um;
        apptime_t duration_s = (apptime_get_ms() - drain_start_time) / 1000;
        int min_calculated_diff = (MIN_DRAIN_DIFF_PER_45S_UM * duration_s) / 45;
        int drain_sig_diff_um = drain_stop_sig - drain_start_sig;

        if (drain_sig_diff_um < -ENOUGH_SIGNAL_DIFF_UM || drain_sig_diff_um < -min_calculated_diff) // using ENOUGH_SIGNAL_DIFF if e.g. we are doing a long drain procedure with little water.
        {
            if (ignore_drain_check == true)
            {
                // After large signal drop
                ignore_drain_check = false;
            }
            else
            {
                is_plaus = true;
#ifdef EPS_LOG
                TRACE_DEB("%s ::: RTP (TRUE) - Drain OFF (was ON %ds), enough signal diff detected (new:%ldum prev:%ldum diff:%d < %d or %d)", EPS_NAME_SHORT,
                    duration_s, drain_stop_sig, drain_start_sig, drain_sig_diff_um, -min_calculated_diff, -ENOUGH_SIGNAL_DIFF_UM);
#endif
            }
        }
        else
        {
            // do not set is_plaus = false, since you can run drain pump without water in drum
#ifdef EPS_LOG
            TRACE_DEB("%s ::: RTP (no change (%s)) - Drain OFF (was ON %ds), not enough signal diff (new:%ldum prev:%ldum diff:%d !< %d or %d)", EPS_NAME_SHORT,
                is_plaus ? "TRUE" : "FALSE", duration_s, drain_stop_sig, drain_start_sig, drain_sig_diff_um, -min_calculated_diff, -ENOUGH_SIGNAL_DIFF_UM);
#endif
        }
    }

    ///////////////////////////////////////////////////////////////
    // SIGNAL DIFF HISTORY CALCULATION
    ///////////////////////////////////////////////////////////////
    static int32_t sig_history[3] = {0};
    static int     sig_history_idx = 0;
    static const int diff_calc_idx_arr[][3] = {{0, 2, 1}, {1, 0, 2}, {2, 1, 0}};

    sig_history[sig_history_idx] = sig_um;

    const int *parr = &diff_calc_idx_arr[sig_history_idx][0];
    int diff1 = sig_history[parr[0]] - sig_history[parr[1]];
    int diff2 = sig_history[parr[1]] - sig_history[parr[2]];
    int diff3 = sig_history[parr[0]] - sig_history[parr[2]];

    int maxdiff = abs(diff1) > abs(diff2) ? diff1 : diff2;
    maxdiff = abs(maxdiff) > abs(diff3) ? maxdiff : diff3;

    if (++sig_history_idx >= 3)
        sig_history_idx = 0;

    ///////////////////////////////////////////////////////////////
    // SOME SIGNAL CHANGE
    ///////////////////////////////////////////////////////////////
    static int ignore_n_sig_change_measurements = 3; // ignore first 3 measurements

    bool some_sig_change = abs(maxdiff) > MIN_SIGNAL_DIFF_SENSOR_OK_UM;
    if (ignore_n_sig_change_measurements > 0)
    {
        ignore_n_sig_change_measurements--;
    }
    else if (some_sig_change)
    {
#ifdef EPS_LOG
        int maxval = abs(diff1) > abs(diff2) ? abs(diff1) : abs(diff2);
        maxval = maxval > abs(diff3) ? maxval : abs(diff3);
        if (is_plaus == false)
            TRACE_DEB("%s ::: RTP (TRUE) - change detected on signal (%ld->%ld->%ldum (diff: abs(%d) > %d))", EPS_NAME_SHORT,
                sig_history[parr[2]], sig_history[parr[1]], sig_history[parr[0]], maxval, MIN_SIGNAL_DIFF_SENSOR_OK_UM);
#endif
        is_plaus = true;
    }

    ///////////////////////////////////////////////////////////////
    // LARGE SIGNAL DROP TO ZERO
    ///////////////////////////////////////////////////////////////
    // WARNING: if this happens while drain pump is on, then after the pump turns off there will be
    // large enough signal difference and plausibility will be set back to true.
    bool large_sig_drop = maxdiff < -MAX_SIGNAL_DROP_TO_EMPTY_UM;
    if (large_sig_drop && sig_um < SIGNAL_WATER_LEVEL_EMPTY_UM)
    {
        is_plaus = false;
        if (g_eps.drain_state == eDRAIN_ON)
            ignore_drain_check = true;
        ignore_n_sig_change_measurements = 2; // to calm signal down to not get SOME SIGNAL CHANGE in the next call
#ifdef EPS_LOG
        int dropdiff = diff1 < diff2 ? diff1 : diff2;
        dropdiff = dropdiff < diff3 ? dropdiff : diff3;
        TRACE_DEB("%s ::: RTP (FALSE) - sudden drop in signal (%ld->%ld->%ldum (diff: %d < -%d))", EPS_NAME_SHORT,
            sig_history[parr[2]], sig_history[parr[1]], sig_history[parr[0]], dropdiff, MAX_SIGNAL_DROP_TO_EMPTY_UM);
#endif
    }

    return is_plaus;
}

/**
 * Overwrite plausibility to TRUE if conditions are met.
 *
 * @return       None.
 */
void eps_overwrite_plaus_after_forced_drain_and_unlocked_door(void)
{
    if (g_eps.sens.is_plaus == false && g_eps.emrgcy_drain_safe_window == true && g_eps.lock_state == eLOCK_UNLOCKED)
    {
        g_eps.sens.is_plaus = true;
#ifdef EPS_LOG
        TRACE_INF("%s ::: Overwriting plausibility from FALSE to TRUE (safe window + unlocked).", EPS_NAME_SHORT);
#endif
    }
}

/**
 * Check if enough errors have accumulated and set flag.
 *
 * @return       None.
 */
static void eps_chk_error(void)
{
    uint32_t err_cnt = g_eps.error.cnt.type[eEPS_ERR_ISR] + g_eps.error.cnt.type[eEPS_ERR_SIG];

    if (err_cnt > g_eps.error.cnt.total) // error is present
    {
        if (g_eps.error.cnt.active < ERROR_CNT_THRESHOLD + 5)
        {
            g_eps.error.cnt.active++;
#ifdef EPS_LOG
            TRACE_DEB("%s ::: error counter +: %d", EPS_NAME_SHORT, g_eps.error.cnt.active);
#endif
        }

    }
    else if (g_eps.error.cnt.active > 0)
    {
        g_eps.error.cnt.active--;
#ifdef EPS_LOG
        TRACE_DEB("%s ::: error counter -: %d", EPS_NAME_SHORT, g_eps.error.cnt.active);
#endif
    }

    g_eps.error.cnt.total = err_cnt;

    if (g_eps.error.cnt.active >= ERROR_CNT_THRESHOLD)
    {
        g_eps.error.is_limit = true;
    }
    else
    {
        g_eps.error.is_limit = false;
    }

#ifdef EPS_LOG
    static bool prev_is_limit = false;
    if (prev_is_limit != g_eps.error.is_limit && g_eps.error.is_limit == true)
    {
        prev_is_limit = g_eps.error.is_limit;
        TRACE_DEB("%s ::: error limit reached: total:%lu isr:%lu sig:%lu", EPS_NAME_SHORT,
            err_cnt - g_eps.error.cnt.total, g_eps.error.cnt.type[eEPS_ERR_ISR],
            g_eps.error.cnt.type[eEPS_ERR_SIG]);
    }
#endif
}

/**
 * Increment error type counter.
 *
 * @param[in]    err    Type of error.
 * @return       None.
 */
static void eps_report_error(eps_err_t err)
{
    g_eps.error.cnt.type[err]++;
}

/**
 * Timer input capture IRQ callback.
 *
 * @return       None.
 */
static void eps_timer_ic_isr(void)
{
    g_eps.meas_tim.period = HAL_TIM_ReadCapturedValue(&htim3, EPS_PERIOD_TIM_IC_CHANNEL);
    g_eps.meas_tim.duty   = HAL_TIM_ReadCapturedValue(&htim3, EPS_DUTY_TIM_IC_CHANNEL);
}

/**
 * Timer overflow IRQ callback. Sets period and duty to 0.
 *
 * @return       None.
 */
static void eps_timer_ovf_isr(void)
{
    g_eps.meas_tim.period = 0;
    g_eps.meas_tim.duty = 0;
}

/**
 * Check for and clear EPS timer input capture interrupt.
 *
 * @return     True if the source of timer IRQ was EPS signal (input capture).
 */
static bool eps_timer_chk_and_clr_ic(void)
{
    bool ret = false;

    if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC2) != RESET &&
        __HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_CC2) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_CC2);

        if ((htim3.Instance->CCMR1 & TIM_CCMR1_CC2S) != 0x00U)
        {
            ret = true;
        }
    }

    return ret;
}

/**
 * Check for and clear EPS timer overflow interrupt.
 *
 * @return     True if the source of timer IRQ was an overflow of timer counter (no EPS signal present).
 */
static bool eps_timer_chk_and_clr_ovf(void)
{
    bool ret = false;

    if (__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_UPDATE) != RESET &&
        __HAL_TIM_GET_IT_SOURCE(&htim3, TIM_IT_UPDATE) != RESET
        )
    {
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);

        ret = true;
    }

    return ret;
}

/**
 * @brief Check if timer freq is within limits
 *
 * @param freq measured frequency of EPS timer
 * @return true if valid
 * @return false if out of bounds
 */
static bool eps_calibration_timer_freq_is_valid(uint32_t freq)
{
    return (freq >= EPS_TIMER_MIN_FREQ && freq <= EPS_TIMER_MAX_FREQ);
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
