/**
 * @file     tim.c
 * @brief    Timer initialization routines
 * @author   Erik Juvan
 * @date     6.12.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup TIM
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "tim.h"
#include "main.h"
#include "stm32xx_STLparam.h"
#include "trace.h"


////////////////////////////////////////////////////////////////////////////////
// Public variables
////////////////////////////////////////////////////////////////////////////////
TIM_HandleTypeDef htim3;   /**< Global TIM handle for EPS. */
TIM_HandleTypeDef htim15;  /**< Global TIM handle for LOCK. */


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Timer 3 initialization routine.
 *
 * @return Status.
 */
HAL_StatusTypeDef MX_TIM3_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig     = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig       = {0};
    TIM_IC_InitTypeDef sConfigIC              = {0};
    HAL_StatusTypeDef status = HAL_OK;

    htim3.Instance               = TIM3;
    htim3.Init.Prescaler         = 1;
    htim3.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim3.Init.Period            = 65535;
    htim3.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if ((status = HAL_TIM_Base_Init(&htim3)) != HAL_OK)
    {
        return status;
    }

    // Added by hand, so that only overflow generates update interrupt
    __HAL_TIM_URS_ENABLE(&htim3);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if ((status = HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig)) != HAL_OK)
    {
        return status;
    }

    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerFilter = 0;
    if ((status = HAL_TIM_SlaveConfigSynchro(&htim3, &sSlaveConfig)) != HAL_OK)
    {
        return status;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    if ((status = HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)) != HAL_OK)
    {
        return status;
    }

    if ((status = HAL_TIM_IC_Init(&htim3)) != HAL_OK)
    {
        return status;
    }

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter    = 0;
    if ((status = HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, EPS_PERIOD_TIM_IC_CHANNEL)) != HAL_OK)
    {
        return status;
    }

    sConfigIC.ICPolarity  = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    if ((status = HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, EPS_DUTY_TIM_IC_CHANNEL)) != HAL_OK)
    {
        return status;
    }

    return status;
}

/**
 * Timer 15 initialization routine.
 *
 * @return Status.
 */
HAL_StatusTypeDef MX_TIM15_Init(void)
{
    HAL_StatusTypeDef status = HAL_OK;

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_SlaveConfigTypeDef sSlaveConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_IC_InitTypeDef sConfigIC = {0};

    htim15.Instance = TIM15;
    htim15.Init.Prescaler = SystemCoreClock / 1000000 - 1; // set for 1us timer resolution
    htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim15.Init.Period = 25000; // 25ms
    htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim15.Init.RepetitionCounter = 0;
    htim15.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if ((status = HAL_TIM_Base_Init(&htim15)) != HAL_OK)
    {
        return status;
    }

    // Added by hand, so that only overflow generates update interrupt
    __HAL_TIM_URS_ENABLE(&htim15);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if ((status = HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig)) != HAL_OK)
    {
        return status;
    }

    if ((status = HAL_TIM_IC_Init(&htim15)) != HAL_OK)
    {
        return status;
    }

    sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
    sSlaveConfig.InputTrigger = TIM_TS_TI2FP2;
    sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sSlaveConfig.TriggerFilter = 0;
    if ((status = HAL_TIM_SlaveConfigSynchro(&htim15, &sSlaveConfig)) != HAL_OK)
    {
        return status;
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if ((status = HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig)) != HAL_OK)
    {
        return status;
    }

    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
    sConfigIC.ICFilter = 0;
    if ((status = HAL_TIM_IC_ConfigChannel(&htim15, &sConfigIC, LOCK_PERIOD_TIM_IC_CHANNEL)) != HAL_OK)
    {
        return status;
    }

    return status;
}

/**
 * HAL callback for TIM initialization (MSP - MCU Support Package).
 *
 * @param[in]    tim_baseHandle    Handle for the specific timer being initialized.
 * @return       None.
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (tim_baseHandle->Instance==TIM3)
    {
        /* TIM3 clock enable */
        __HAL_RCC_TIM3_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**TIM3 GPIO Configuration
        PA7     ------> TIM3_CH2
        */
        GPIO_InitStruct.Pin       = EPS_PIN;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
        HAL_GPIO_Init(EPS_PORT, &GPIO_InitStruct);

        /* TIM3 interrupt Init */
        HAL_NVIC_SetPriority(TIM3_TIM4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_TIM4_IRQn);
    }
    else if (tim_baseHandle->Instance==TIM15)
    {
        /** Initializes the peripherals clocks
        */
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_TIM15;
        PeriphClkInit.Tim15ClockSelection = RCC_TIM15CLKSOURCE_PCLK1;
        HAL_StatusTypeDef ret;
        ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
        if (ret != HAL_OK)
        {
            TRACE_FAT("SMN ::: HAL_RCCEx_PeriphCLKConfig error %d", ret);
            FailSafePOR();
        }

        /* Peripheral clock enable */
        __HAL_RCC_TIM15_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**TIM15 GPIO Configuration
        PB15     ------> TIM15_CH2
        */
        GPIO_InitStruct.Pin = LOCK_IND_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_TIM15;
        HAL_GPIO_Init(LOCK_IND_PORT, &GPIO_InitStruct);

        /* TIM15 interrupt Init */
        HAL_NVIC_SetPriority(TIM15_IRQn, 1, 0);
        HAL_NVIC_EnableIRQ(TIM15_IRQn);
    }
}

/**
 * HAL callback for TIM deinitialization.
 *
 * @param[in]    tim_baseHandle    Handle for the specific timer being deinitialized.
 * @return       None.
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
    if (tim_baseHandle->Instance==TIM3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /**TIM3 GPIO Configuration
        PA7     ------> TIM3_CH2
        */
        HAL_GPIO_DeInit(EPS_PORT, EPS_PIN);

        /* TIM3 interrupt Deinit */
        HAL_NVIC_DisableIRQ(TIM3_TIM4_IRQn);
    }
    else if (tim_baseHandle->Instance==TIM15)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM15_CLK_DISABLE();

        /**TIM15 GPIO Configuration
        PB15     ------> TIM15_CH2
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);

        /* TIM15 interrupt DeInit */
        HAL_NVIC_DisableIRQ(TIM15_IRQn);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
