////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     timer.c
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

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "timer.h"
#include "../adc/adc_cfg.h"
#include "priorities.h"
#include "stm32g4xx_hal_tim_ex.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Bridge signals
#define GPIO_PIN_PWM_A_HIGH  GPIO_PIN_8
#define GPIO_PORT_PWM_A_HIGH GPIOA

#define GPIO_PIN_PWM_B_HIGH  GPIO_PIN_9
#define GPIO_PORT_PWM_B_HIGH GPIOA

#define GPIO_PIN_PWM_C_HIGH  GPIO_PIN_10
#define GPIO_PORT_PWM_C_HIGH GPIOA

#define GPIO_PIN_PWM_A_LOW   GPIO_PIN_13
#define GPIO_PORT_PWM_A_LOW  GPIOC

#define GPIO_PIN_PWM_B_LOW   GPIO_PIN_12
#define GPIO_PORT_PWM_B_LOW  GPIOA

#define GPIO_PIN_PWM_C_LOW   GPIO_PIN_15
#define GPIO_PORT_PWM_C_LOW  GPIOB

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
TIM_HandleTypeDef g_htim1;

volatile bool    g_pwm_enabled             = false;
volatile uint8_t g_pwm_disable_first_entry = 0u;
volatile uint8_t g_pwm_enable_first_entry  = 0u;

static TIM_HandleTypeDef           g_htim7;
static timer_update_irq_callback_t g_tim7_update_irq_callback = NULL;

static break_t g_break1 = {BREAK1, BREAK_HW_TRIGG, false};

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef timer1_init(void);
static void              timer1_gpio_init(void);
void                     timer7_ISR(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes TIM1 for PWM operation and for ADC
 *                trigger
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef timer_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    // Initialize TIM1
    if (timer1_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    g_pwm_disable_first_entry = 0u;
    g_pwm_enable_first_entry  = 0u;

    // Initialize TIM7
    if (timer7_init(1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
    timer7_update_irq_set(timer7_ISR);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes TIM1 for PWM operation and for ADC
 *                trigger
 *
 * @param[in]    None
 * @return       init_status     TIM1 initialization status
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef timer1_init(void)
{
    HAL_StatusTypeDef              init_status          = HAL_OK;
    TIM_ClockConfigTypeDef         sClockSourceConfig   = {0};
    TIM_MasterConfigTypeDef        sMasterConfig        = {0};
    TIMEx_BreakInputConfigTypeDef  sBreakInputConfig    = {0};
    TIM_OC_InitTypeDef             sConfigOC            = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    // TIM1 clock enable
    __HAL_RCC_TIM1_CLK_ENABLE();

    g_htim1.Instance               = TIM1;
    g_htim1.Init.Prescaler         = PWM_CLK_PRESCALER;
    g_htim1.Init.CounterMode       = TIM_COUNTERMODE_CENTERALIGNED1;
    g_htim1.Init.Period            = PWM_PERIOD_TICKS;
    g_htim1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    g_htim1.Init.RepetitionCounter = 1;
    g_htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    if (HAL_TIM_Base_Init(&g_htim1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

    if (HAL_TIM_ConfigClockSource(&g_htim1, &sClockSourceConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    if (HAL_TIM_PWM_Init(&g_htim1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sMasterConfig.MasterOutputTrigger  = TIM_TRGO_OC4REF;
    sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    sMasterConfig.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;

    if (HAL_TIMEx_MasterConfigSynchronization(&g_htim1, &sMasterConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakInputConfig.Source   = TIM_BREAKINPUTSOURCE_COMP1;
    sBreakInputConfig.Enable   = TIM_BREAKINPUTSOURCE_ENABLE;
    sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;

    if (HAL_TIMEx_ConfigBreakInput(&g_htim1, (uint32_t)TIM_BREAKINPUT_BRK2, &sBreakInputConfig) !=
        HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakInputConfig.Source   = TIM_BREAKINPUTSOURCE_COMP2;
    sBreakInputConfig.Enable   = TIM_BREAKINPUTSOURCE_ENABLE;
    sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;

    if (HAL_TIMEx_ConfigBreakInput(&g_htim1, (uint32_t)TIM_BREAKINPUT_BRK2, &sBreakInputConfig) !=
        HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakInputConfig.Source   = TIM_BREAKINPUTSOURCE_COMP4;
    sBreakInputConfig.Enable   = TIM_BREAKINPUTSOURCE_ENABLE;
    sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;

    if (HAL_TIMEx_ConfigBreakInput(&g_htim1, (uint32_t)TIM_BREAKINPUT_BRK2, &sBreakInputConfig) !=
        HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakInputConfig.Source   = TIM_BREAKINPUTSOURCE_COMP3;
    sBreakInputConfig.Enable   = TIM_BREAKINPUTSOURCE_ENABLE;
    sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_HIGH;

    if (HAL_TIMEx_ConfigBreakInput(&g_htim1, (uint32_t)TIM_BREAKINPUT_BRK, &sBreakInputConfig) !=
        HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakInputConfig.Source   = TIM_BREAKINPUTSOURCE_BKIN;
    sBreakInputConfig.Enable   = TIM_BREAKINPUTSOURCE_ENABLE;
    sBreakInputConfig.Polarity = TIM_BREAKINPUTSOURCE_POLARITY_LOW;

    if (HAL_TIMEx_ConfigBreakInput(&g_htim1, TIM_BREAKINPUT_BRK, &sBreakInputConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfigOC.OCMode       = TIM_OCMODE_PWM2;
    sConfigOC.Pulse        = 0;
    sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_SET;

    if (HAL_TIM_PWM_ConfigChannel(&g_htim1, &sConfigOC, (uint32_t)TIM_CHANNEL_1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfigOC.Pulse = 0;

    if (HAL_TIM_PWM_ConfigChannel(&g_htim1, &sConfigOC, (uint32_t)TIM_CHANNEL_2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfigOC.Pulse = 0;

    if (HAL_TIM_PWM_ConfigChannel(&g_htim1, &sConfigOC, (uint32_t)TIM_CHANNEL_3) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfigOC.Pulse  = 1;
    sConfigOC.OCMode = TIM_OCMODE_PWM2;

    if (HAL_TIM_PWM_ConfigChannel(&g_htim1, &sConfigOC, (uint32_t)TIM_CHANNEL_4) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
    sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime         = PWM_DEADTIME_TICKS;
    sBreakDeadTimeConfig.BreakState       = TIM_BREAK_ENABLE;
    sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter =
        0xF; // TODO: filter because BKIN is now additionally connected to external GPO_SD??
    sBreakDeadTimeConfig.BreakAFMode     = TIM_BREAK_AFMODE_INPUT;
    sBreakDeadTimeConfig.Break2State     = TIM_BREAK2_ENABLE /*TIM_BREAK2_DISABLE*/;
    sBreakDeadTimeConfig.Break2Polarity  = TIM_BREAK2POLARITY_HIGH;
    sBreakDeadTimeConfig.Break2Filter    = 0xF;
    sBreakDeadTimeConfig.Break2AFMode    = TIM_BREAK2_AFMODE_INPUT;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;

    if (HAL_TIMEx_ConfigBreakDeadTime(&g_htim1, &sBreakDeadTimeConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // Configure GPIO pins for PWM
    timer1_gpio_init();

    HAL_TIM_OC_Start(&g_htim1, (uint32_t)TIM_CHANNEL_4);
    HAL_TIM_Base_Start(&g_htim1);

    // Disable PWMs
    timer1_disable_pwms();
    timer1_clear_break_flag();

    // Connect PWM pins to TIM1
    // Enable TIM1_CH1 and TIM1_CH1N outputs
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->CCER |= TIM_CCER_CC1NE;
    // Enable TIM1_CH2 and TIM1_CH2N outputs
    TIM1->CCER |= TIM_CCER_CC2E;
    TIM1->CCER |= TIM_CCER_CC2NE;
    // Enable TIM1_CH3 and TIM1_CH3N outputs
    TIM1->CCER |= TIM_CCER_CC3E;
    TIM1->CCER |= TIM_CCER_CC3NE;

    // Enable bridge
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes GPIO pins for PWM signals
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static void timer1_gpio_init(void)
{
    GPIO_InitTypeDef gpio_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /** TIM1 GPIO Configuration
    PC13    ------> TIM1_CH1N
    PA12    ------> TIM1_CH2N
    PB15    ------> TIM1_CH3N
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10    ------> TIM1_CH3 */

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_A_HIGH;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF6_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_A_HIGH, &gpio_InitStruct);

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_B_HIGH;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF6_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_B_HIGH, &gpio_InitStruct);

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_C_HIGH;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF6_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_C_HIGH, &gpio_InitStruct);

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_A_LOW;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF4_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_A_LOW, &gpio_InitStruct);

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_B_LOW;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF6_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_B_LOW, &gpio_InitStruct);

    gpio_InitStruct.Pin       = GPIO_PIN_PWM_C_LOW;
    gpio_InitStruct.Mode      = GPIO_MODE_AF_PP;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF4_TIM1;

    HAL_GPIO_Init(GPIO_PORT_PWM_C_LOW, &gpio_InitStruct);

    // Safety expects PWMs are disabled on init
    GPIO_PORT_PWM_A_HIGH->MODER &= ~GPIO_MODER_MODE8;
    GPIO_PORT_PWM_B_HIGH->MODER &= ~GPIO_MODER_MODE9;
    GPIO_PORT_PWM_C_HIGH->MODER &= ~GPIO_MODER_MODE10;
    GPIO_PORT_PWM_A_LOW->MODER  &= ~GPIO_MODER_MODE13;
    GPIO_PORT_PWM_B_LOW->MODER  &= ~GPIO_MODER_MODE12;
    GPIO_PORT_PWM_C_LOW->MODER  &= ~GPIO_MODER_MODE15;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function enables PWM outputs
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_enable_pwms(void)
{
    if (g_pwm_enable_first_entry == 0u)
    {
        // Clear break2 event
        TIM1->SR   &= ~TIM_SR_B2IF_Msk;
        // Turn on Master output
        TIM1->BDTR |= TIM_BDTR_MOE;

        // Indication that IGBTs are turned on
        g_pwm_enabled = true;

        g_pwm_enable_first_entry  = 1u;
        g_pwm_disable_first_entry = 0u;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function disables PWM outputs
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_disable_pwms(void)
{
    if (g_pwm_disable_first_entry == 0u)
    {
        // Generate break2 event to put all IGBTs to off state but still controlled
        // by the timer
        TIM1->EGR |= TIM_EGR_B2G;

        // Indication that IGBTs are turned off
        g_pwm_enabled = false;

        timer1_set_pwms(0.0f, 0.0f, 0.0f);

        g_pwm_disable_first_entry = 1u;
        g_pwm_enable_first_entry  = 0u;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function sets PWM duty cycle
 *
 * @param[in]    a                    Duty cycle for phase A
 * @param[in]    b                    Duty cycle for phase B
 * @param[in]    c                    Duty cycle for phase C
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_set_pwms(
    float a, float b, float c
) /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
{
    // Optimise
    if (a > 1.0f)
    {
        a = 1.0f;
    }
    if (a < 0.0f)
    {
        a = 0.0f;
    }

    if (b > 1.0f)
    {
        b = 1.0f;
    }
    if (b < 0.0f)
    {
        b = 0.0f;
    }

    if (c > 1.0f)
    {
        c = 1.0f;
    }
    if (c < 0.0f)
    {
        c = 0.0f;
    }

    __HAL_TIM_SET_COMPARE(
        &g_htim1, TIM_CHANNEL_1,
        (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
            (uint16_t)(a * (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1))
    );
    __HAL_TIM_SET_COMPARE(
        &g_htim1, TIM_CHANNEL_2,
        (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
            (uint16_t)(b * (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1))
    );
    __HAL_TIM_SET_COMPARE(
        &g_htim1, TIM_CHANNEL_3,
        (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
            (uint16_t)(c * (uint16_t)__HAL_TIM_GET_AUTORELOAD(&g_htim1))
    );
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function gets PWM duty cycle
 *
 * @param[in]    *a                    Duty cycle for phase A
 * @param[in]    *b                    Duty cycle for phase B
 * @param[in]    *c                    Duty cycle for phase C
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
void timer1_get_pwms(float *a, float *b, float *c)
{
    *a = ((float)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
          (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_1)) /
         (float)__HAL_TIM_GET_AUTORELOAD(&g_htim1);
    *b = ((float)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
          (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_2)) /
         (float)__HAL_TIM_GET_AUTORELOAD(&g_htim1);
    *c = ((float)__HAL_TIM_GET_AUTORELOAD(&g_htim1) -
          (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_3)) /
         (float)__HAL_TIM_GET_AUTORELOAD(&g_htim1);
}

/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
void timer1_get_cmp_val(uint16_t *a, uint16_t *b, uint16_t *c)
{
    *a = (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_1);
    *b = (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_2);
    *c = (uint16_t)__HAL_TIM_GET_COMPARE(&g_htim1, TIM_CHANNEL_3);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function checks if break is active. In case if break is
 *                 active this means that over voltage occurred.
 *
 * @return       true in case if break is active, otherwise false
 */
////////////////////////////////////////////////////////////////////////////////
bool timer1_is_hw_break_active(void)
{
    bool ret_status = false;

    if (g_break1.trigg_type == BREAK_HW_TRIGG)
    {
        if ((TIM1->SR & TIM_SR_BIF_Msk) != 0)
        {
            g_break1.status = true;
        }
        else
        {
            g_break1.status = false;
        }
    }

    ret_status = (g_break1.status & (g_break1.trigg_type == BREAK_HW_TRIGG));

    return ret_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function clears break flag.
 *
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_clear_break_flag(void)
{
    TIM1->SR &= ~TIM_SR_BIF;

    // TODO check why we had this MOE enable until version 4.21
    // This could be the root cause of failed IGBT modules
    // Enable Master output
    // TIM1->BDTR |= TIM_BDTR_MOE;

    // Clear break1 status
    g_break1.trigg_type = BREAK_HW_TRIGG;
    g_break1.status     = false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * This functions generates brake event for timer 1
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_break_event_generate(void)
{
    g_break1.trigg_type  = BREAK_SW_TRIGG;
    g_break1.status      = true;
    TIM1->EGR           |= TIM_EGR_BG;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * This functions clears generated brake event for timer 1
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_break_event_clear(void)
{
    TIM1->SR            &= ~TIM_SR_BIF;
    // Clear break1 status
    g_break1.trigg_type  = BREAK_HW_TRIGG;
    g_break1.status      = false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function checks if break2 is active. In case if break2 is
 *               active this means that over current occurred.
 *
 * @return       true in case if break2 is active, otherwise false
 */
////////////////////////////////////////////////////////////////////////////////
bool timer1_is_break2_active(void)
{
    bool status_active = false;

    if (((TIM1->SR & TIM_SR_B2IF_Msk) != 0) && (g_pwm_enabled == true))
    {
        status_active = true;
    }

    return status_active;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function directly return brake 2 flag from timer 1
 * @return       true in case if break2 is active, otherwise false
 */
////////////////////////////////////////////////////////////////////////////////
bool timer1_is_break_active_raw(void)
{
    if (TIM1->SR & TIM_SR_BIF)
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function clears break2 flag.
 *
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer1_clear_break2_flag(void)
{
    TIM1->SR &= ~TIM_SR_B2IF;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes TIM7 for periodic update interrupt
 *
 * @param[in]    normal_power    1 - 170 MHz cock configuration,
 *                               0 - 16 MHz clock configuration
 * @return       init_status     TIM7 initialization status
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef timer7_init(uint8_t normal_power)
{
    HAL_StatusTypeDef       init_status   = HAL_OK;
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    // Enable timer clock
    __HAL_RCC_TIM7_CLK_ENABLE();

    // Initialize timer
    g_htim7.Instance = TIM7;
    if (normal_power)
    {
        // 1ms period
        g_htim7.Init.Prescaler = TIMER7_TIM_PRESCALER - 1;
        g_htim7.Init.Period    = TIMER7_TIM_PERIOD - 1;
    }
    else
    {
        // 1ms period
        g_htim7.Init.Prescaler = 1 - 1;
        g_htim7.Init.Period    = 16000 - 1;
    }
    g_htim7.Init.CounterMode = TIM_COUNTERMODE_UP;

    g_htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&g_htim7) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&g_htim7, &sMasterConfig) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // Enable NVIC interrupts
    HAL_NVIC_SetPriority(TIM7_IRQn, TIM7_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    // Start timer
    HAL_TIM_Base_Start(&g_htim7);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function enables TIM7 update interrupt
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer7_update_irq_enable(void)
{
    __HAL_TIM_ENABLE_IT(&g_htim7, TIM_IT_UPDATE);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function disables TIM7 update interrupt
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer7_update_irq_disable(void)
{
    __HAL_TIM_DISABLE_IT(&g_htim7, TIM_IT_UPDATE);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function configures TIM7 update interrupt callback
 *
 * @param[in]    callback                Timer update interrupt callback
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void timer7_update_irq_set(timer_update_irq_callback_t callback)
{
    g_tim7_update_irq_callback = callback;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function handles TIM7 global interrupt.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void TIM7_IRQHandler(void) /* parasoft-suppress NAMING-34 "External function, cannot be changed." */
{
    /* USER CODE BEGIN TIM7_IRQn 0 */

    /* USER CODE END TIM7_IRQn 0 */
    HAL_TIM_IRQHandler(&g_htim7);
    /* USER CODE BEGIN TIM7_IRQn 1 */

    /* USER CODE END TIM7_IRQn 1 */
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Period elapsed callback in non-blocking mode
 *
 * @param[in]    htim TIM handle
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim
) /* parasoft-suppress NAMING-34 MISRAC2012-RULE_8_13-a "External function, cannot be changed." */
{
    if (htim->Instance == TIM7)
    {
        // TIM7 period elapsed event
        if (g_tim7_update_irq_callback)
        {
            g_tim7_update_irq_callback();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
