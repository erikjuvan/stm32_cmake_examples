////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     timer.h
 * @brief    Functions for timer
 * @author   Haris Kovacevic
 * @date     08.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup TIMER_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __TIMER_H
#define __TIMER_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "stm32g4xx_hal.h"
#include "timer_cfg.h"
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Definitions of possible break which can be generated
 */
typedef enum
{
    BREAK1 = 0, /**< Break 1 used for over voltage protection */
    BREAK2 = 1  /**< Break 2 used for over current protection */
} break_type_t;

/**
 * Definitions of possible protections that can be triggered
 */
typedef enum
{
    BREAK_SW_TRIGG = 0, /**< Break is trigger by application */
    BREAK_HW_TRIGG = 1  /**< Break is triggered by hardware */
} break_trigg_type_t;

typedef struct
{
    break_type_t       type;       /**< Break type */
    break_trigg_type_t trigg_type; /**< Break trigger type */
    bool               status;     /**< Break status */
} break_t;

typedef void (*timer_update_irq_callback_t)(void);
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef timer_init(void);
void              timer1_enable_pwms(void);
void              timer1_disable_pwms(void);
/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
void timer1_set_pwms(float a, float b, float c);
void timer1_get_pwms(float *a, float *b, float *c);
/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
bool timer1_is_hw_break_active(void);
bool timer1_is_break2_active(void);
void timer1_clear_break_flag(void);
void timer1_clear_break2_flag(void);
void timer1_break_event_generate(void);
void timer1_break_event_clear(void);
bool timer1_is_break_active_raw(void);

void timer1_get_cmp_val(uint16_t *a, uint16_t *b, uint16_t *c);

HAL_StatusTypeDef timer7_init(uint8_t normal_power);
void              timer7_update_irq_enable(void);
void              timer7_update_irq_disable(void);
void              timer7_update_irq_set(timer_update_irq_callback_t callback);

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
