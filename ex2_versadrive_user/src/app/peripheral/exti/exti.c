////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     exti.c
 * @brief    Functions for EXTI
 * @author   Haris Kovacevic
 * @date     19.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup EXTI_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "exti.h"
#include "gpio/gpio.h"
#include "priorities.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
EXTI_HandleTypeDef g_hexti3;
EXTI_HandleTypeDef g_hexti2;
EXTI_HandleTypeDef g_hexti10;

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = motor_control__hw_bridge_flag
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Hardware error flag detected on bridge (OC or UV)"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 10
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile uint8_t motor_control__hw_bridge_flag = 0U;

/*
@! SYMBOL       = motor_control__hw_bridge_cnt
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE
@! DESCRIPTION  = "Hardware error flag detected on bridge counter"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 11
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile uint8_t motor_control__hw_bridge_cnt = 0U;
/* parasoft-END-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef exti_backgound_init(void);
static HAL_StatusTypeDef exti_bridge_fault_init(void);
static void (*pg_slow_loop_callback_fnc)(void);
static void (*pg_background_loop_callback_fnc)(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes EXTI
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef exti_init(void)
{
    HAL_StatusTypeDef  init_status = HAL_OK;
    EXTI_ConfigTypeDef extiConfig;

    g_hexti3.Line = 3;

    extiConfig.GPIOSel = EXTI_GPIOB;
    extiConfig.Line    = EXTI_LINE_3;
    extiConfig.Mode    = EXTI_MODE_INTERRUPT;
    extiConfig.Trigger = EXTI_TRIGGER_RISING;

    if (HAL_EXTI_SetConfigLine(&g_hexti3, &extiConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // EXTI interrupt init - SW interrupt for 1 ms loop
    HAL_NVIC_SetPriority(EXTI3_IRQn, EXTI3_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    if (exti_backgound_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    if (exti_bridge_fault_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes EXTI which will be used for background
 *                 loop calculation
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef exti_backgound_init(void)
{
    HAL_StatusTypeDef  init_status = HAL_OK;
    EXTI_ConfigTypeDef extiConfig;

    g_hexti2.Line = 2;

    extiConfig.GPIOSel = EXTI_GPIOB;
    extiConfig.Line    = EXTI_LINE_2;
    extiConfig.Mode    = EXTI_MODE_INTERRUPT;
    extiConfig.Trigger = EXTI_TRIGGER_RISING;

    if (HAL_EXTI_SetConfigLine(&g_hexti2, &extiConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // EXTI interrupt init - SW interrupt for 1 s loop
    HAL_NVIC_SetPriority(EXTI2_IRQn, EXTI2_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes EXTI which will be used for background
 *                 loop calculation
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef exti_bridge_fault_init(void)
{
    HAL_StatusTypeDef  init_status = HAL_OK;
    EXTI_ConfigTypeDef extiConfig;

    g_hexti10.Line = 10;

    extiConfig.GPIOSel = EXTI_GPIOB;
    extiConfig.Line    = EXTI_LINE_10;
    extiConfig.Mode    = EXTI_MODE_INTERRUPT;
    extiConfig.Trigger = EXTI_TRIGGER_FALLING;

    if (HAL_EXTI_SetConfigLine(&g_hexti10, &extiConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // EXTI interrupt init - Bridge error flag (HW OC or UV)
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, EXTI5_10_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function sets callback to interrupt.
 *
 * @param[in]        func            - callback for the slow loop (1 ms task)
 * @return           none
 */
////////////////////////////////////////////////////////////////////////////////
void exti_set_slow_loop_callback(void func(void))
{
    pg_slow_loop_callback_fnc = func;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function sets callback for background loop interrupt.
 *
 * @param[in]        func            - callback for the slow loop (1 s task)
 * @return           none
 */
////////////////////////////////////////////////////////////////////////////////
void exti_set_background_loop_callback(void func(void))
{
    pg_background_loop_callback_fnc = func;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function triggers SW interrupt for 1 ms task.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void exti_trigger_sw_isr(void)
{
    HAL_EXTI_GenerateSWI(&g_hexti3);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function triggers SW interrupt for 1 s task.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void exti_trigger_sw_background_isr(void)
{
    HAL_EXTI_GenerateSWI(&g_hexti2);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function represents ISR for velocity loop. This SW interrupt is
 *                 triggered from FOC loop with defined ratio between FOC and
 *                 velocity loop.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void EXTI15_10_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function callback, cannot be changed." */
{
    HAL_EXTI_IRQHandler(&g_hexti10);

    // Set flag to 1 in case of HW error flag
    motor_control__hw_bridge_flag = 1U;
    uint8_t temp                  = motor_control__hw_bridge_cnt;
    temp++;
    motor_control__hw_bridge_cnt = temp;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function represents ISR for velocity loop. This SW interrupt is
 *                 triggered from FOC loop with defined ratio between FOC and
 *                 velocity loop.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void EXTI2_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function callback, cannot be changed." */
{
    HAL_EXTI_IRQHandler(&g_hexti2);

    // Execute 1 s task
    pg_background_loop_callback_fnc();
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function represents ISR for velocity loop. This SW interrupt is
 *                 triggered from FOC loop with defined ratio between FOC and
 *                 velocity loop.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void EXTI3_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function callback, cannot be changed." */
{
    HAL_EXTI_IRQHandler(&g_hexti3);

    // Execute 1 ms task
    pg_slow_loop_callback_fnc();
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
