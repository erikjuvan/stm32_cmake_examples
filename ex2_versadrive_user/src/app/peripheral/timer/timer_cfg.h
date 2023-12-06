////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     timer_cfg.h
 * @brief    Functions for timer configuration module
 * @author   Haris Kovacevic
 * @date     08.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup TIMER_CFG_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __TIMER_CFG_H
#define __TIMER_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// PWM counter types
#define PWM_CNT_UP              (0U)
#define PWM_CNT_DOWN            (1U)
#define PWM_CNT_CENTER_ALIGNED1 (2U)
#define PWM_CNT_CENTER_ALIGNED2 (3U)
#define PWM_CNT_CENTER_ALIGNED3 (4U)

// PWM clock source frequency in [ Hz ]
#define PWM_CLK_FREQ            (170000000)
// PWM frequency in [ Hz ]
#define PWM_FREQ                (16000.0)
// PWM resolution in [ ns ]
#define PWM_RESOLUTION          (23.52941176)
// PWM deadtime in [ us ]
#define PWM_DEADTIME            (1.2)

// Deadtime range types
// @Notes: Range depends on clock frequency and it prepared for 170 MHz input frequency
#define DEADTIME_0US_0_8US      (0U) // Resolution 5.88 ns
#define DEADTIME_0_8US_1_5US    (1U) // Resolution 11.76 ns
#define DEADTIME_1_5US_3US      (2U) // Resolution 47.06 ns
#define DEADTIME_3US_6US        (3U) // Resolution 94.12 ns

// Selected PWM counter type
#define PWM_CNT_TYPE            (PWM_CNT_CENTER_ALIGNED1)
// Selected PWM deadtime range type
#define PWM_DEADTIME_RANGE      (DEADTIME_0_8US_1_5US)

// PWM period in [ s ]
#define PWM_PERIOD              (1.0 / PWM_FREQ)
#if ((PWM_CNT_TYPE == PWM_CNT_UP) || (PWM_CNT_TYPE == PWM_CNT_DOWN))
// PWM period in [ CLK TICKS ]
#define PWM_PERIOD_TICKS ((uint16_t)(PWM_PERIOD / (PWM_RESOLUTION * 10E-9)))
#else
// PWM period in [ CLK TICKS ]
#define PWM_PERIOD_TICKS ((uint16_t)((PWM_PERIOD / (PWM_RESOLUTION * 0.001 * 0.001 * 0.001)) / 2.0))
#endif

#if ((PWM_CNT_TYPE == PWM_CNT_UP) || (PWM_CNT_TYPE == PWM_CNT_DOWN))
// PWM clock prescaler
#define PWM_CLK_PRESCALER ((uint16_t)((PWM_PERIOD / PWM_PERIOD_TICKS) * PWM_CLK_FREQ) - 1U)
#else
// PWM clock prescaler
#define PWM_CLK_PRESCALER \
    (((uint16_t)(((PWM_PERIOD / PWM_PERIOD_TICKS) * PWM_CLK_FREQ) / 2.0) - 1U))
#endif

#if (PWM_CLK_FREQ == 170000000)
#if (PWM_DEADTIME_RANGE == DEADTIME_0US_0_8US)
// Formula works for ranges 0.0 us - 1.5 us with resolution 5.88 ns
#define PWM_DEADTIME_TICKS (((uint16_t)(PWM_DEADTIME / (1.0 / (PWM_CLK_FREQ / 1000.0 / 1000.0)))))
#elif (PWM_DEADTIME_RANGE == DEADTIME_0_8US_1_5US)
// Formula works for ranges 75.3 ns - 1.5 us with resolution 11.76 ns
#define PWM_DEADTIME_TICKS \
    (0x0080U +             \
     (((uint16_t)((PWM_DEADTIME / (1.0 / (PWM_CLK_FREQ / 1000.0 / 1000.0))) / 2.0)) - 64U))
#elif (PWM_DEADTIME_RANGE == DEADTIME_1_5US_3US)
// Formula works for ranges 1.5 us - 3.0 us with resolution 47.06 ns
#define PWM_DEADTIME_TICKS \
    (0x00C0U +             \
     (((uint16_t)((PWM_DEADTIME / (1.0 / (PWM_CLK_FREQ / 1000.0 / 1000.0))) / 8.0)) - 32U))
#elif (PWM_DEADTIME_RANGE == DEADTIME_3US_6US)
// Formula works for ranges 3.0 us - 6.0 us with resolution 94.12 ns
#define PWM_DEADTIME_TICKS \
    (0x00E0U +             \
     (((uint16_t)((PWM_DEADTIME / (1.0 / (PWM_CLK_FREQ / 1000.0 / 1000.0))) / 16.0)) - 32U))
#endif
#endif

// TIMER7 definitions
// Update frequency calculation
// UPDATE_FREQ = TIMER7_TIM_FREQ / (TIMER7_TIM_PRESCALER * TIMER7_TIM_PERIOD)
#define TIMER7_TIM_FREQ      170000000
// Settings for update frequency of 1000 Hz
#define TIMER7_TIM_PRESCALER 17
#define TIMER7_TIM_PERIOD    10000

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
