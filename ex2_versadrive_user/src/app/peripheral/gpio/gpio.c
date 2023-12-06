////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     gpio.c
 * @brief    Functions for GPIOs
 * @author   Haris Kovacevic
 * @date     11.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup GPIO_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "gpio.h"
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes GPIOs
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef gpio_init(void)
{
    GPIO_InitTypeDef gpio_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /////////////
    // Outputs //
    /////////////

    gpio_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    gpio_InitStruct.Pin = GPIO_PIN_NTC_COMP_SUPPLY;
    HAL_GPIO_Init(GPIO_PORT_NTC_COMP_SUPPLY, &gpio_InitStruct);

    gpio_InitStruct.Pin = GPIO_PIN_PRECHARGE_RELAY;
    HAL_GPIO_Init(GPIO_PORT_PRECHARGE_RELAY, &gpio_InitStruct);

    gpio_InitStruct.Pin = GPIO_PIN_IGBT_ON;
    HAL_GPIO_Init(GPIO_PORT_IGBT_ON, &gpio_InitStruct);

    gpio_InitStruct.Pin = GPIO_PIN_FAN;
    HAL_GPIO_Init(GPIO_PORT_FAN, &gpio_InitStruct);

    gpio_InitStruct.Pin = GPIO_PIN_NTC_IGBT_SUPPLY;
    HAL_GPIO_Init(GPIO_PORT_NTC_IGBT_SUPPLY, &gpio_InitStruct);

    // Set outputs
    HAL_GPIO_WritePin(GPIO_PORT_NTC_COMP_SUPPLY, GPIO_PIN_NTC_COMP_SUPPLY,
        GPIO_PIN_RESET); // On, active LOW
    HAL_GPIO_WritePin(GPIO_PORT_PRECHARGE_RELAY, GPIO_PIN_PRECHARGE_RELAY,
        GPIO_PIN_RESET); // Off, so current flows through resistor
    HAL_GPIO_WritePin(GPIO_PORT_IGBT_ON, GPIO_PIN_IGBT_ON, GPIO_PIN_SET);                 // On
    HAL_GPIO_WritePin(GPIO_PORT_FAN, GPIO_PIN_FAN, GPIO_PIN_RESET);                       // Off
    HAL_GPIO_WritePin(GPIO_PORT_NTC_IGBT_SUPPLY, GPIO_PIN_NTC_IGBT_SUPPLY, GPIO_PIN_SET); // On

    ////////////
    // Inputs //
    ////////////

    // Production test input
    gpio_InitStruct.Mode  = GPIO_MODE_INPUT;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Pin   = GPIO_PIN_PROD_TEST_MODE;
    HAL_GPIO_Init(GPIO_PORT_PROD_TEST_MODE, &gpio_InitStruct);

    ///////////////
    // Alternate //
    ///////////////

    gpio_InitStruct.Mode      = GPIO_MODE_AF_OD;
    gpio_InitStruct.Pull      = GPIO_NOPULL;
    gpio_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio_InitStruct.Alternate = GPIO_AF12_TIM1_COMP1;
    gpio_InitStruct.Pin       = GPIO_PIN_BRIDGE_SD;
    HAL_GPIO_Init(GPIO_PORT_BRIDGE_SD, &gpio_InitStruct);

    return HAL_OK;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function for turn on/off IGBT. In case if 1 is written, IGBT is
 *                 turned on, otherwise is turned off.
 *
 * @param[in]    state        Turn on/off state
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void gpio_turn_on_off_IGBT(const uint8_t state)
{
    if (1U == state)
    {
        HAL_GPIO_WritePin(GPIO_PORT_IGBT_ON, GPIO_PIN_IGBT_ON, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIO_PORT_IGBT_ON, GPIO_PIN_IGBT_ON, GPIO_PIN_RESET);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Return state of TEST MODE pin
 *
 * @return   bool - true if pin is set
 */
////////////////////////////////////////////////////////////////////////////////
bool gpio_get_test_pin(void)
{
    return HAL_GPIO_ReadPin(GPIO_PORT_PROD_TEST_MODE, GPIO_PIN_PROD_TEST_MODE);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
