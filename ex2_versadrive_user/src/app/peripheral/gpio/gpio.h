////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     gpio.h
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

#ifndef __GPIO_H
#define __GPIO_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>

#include "stm32g4xx_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// NTC (the one mounted on compressor) supply
#define GPIO_PIN_NTC_COMP_SUPPLY  GPIO_PIN_15
#define GPIO_PORT_NTC_COMP_SUPPLY GPIOA

// Enable relay NOT USED (but is present on schematic)!!!
#define GPIO_PIN_ENABLE_RELAY     GPIO_PIN_2
#define GPIO_PORT_ENABLE_RELAY    GPIOB

// Precharge relay
#define GPIO_PIN_PRECHARGE_RELAY  GPIO_PIN_5
#define GPIO_PORT_PRECHARGE_RELAY GPIOB

// Bridge error indication pin
#define GPIO_PIN_BRIDGE_SD        GPIO_PIN_10
#define GPIO_PORT_BRIDGE_SD       GPIOB

// Bridge enable
#define GPIO_PIN_IGBT_ON          GPIO_PIN_11
#define GPIO_PORT_IGBT_ON         GPIOB

// Fan
#define GPIO_PIN_FAN              GPIO_PIN_12
#define GPIO_PORT_FAN             GPIOB

// NTC (in IGBT chip) supply
#define GPIO_PIN_NTC_IGBT_SUPPLY  GPIO_PIN_13
#define GPIO_PORT_NTC_IGBT_SUPPLY GPIOB

// Production test mode
#define GPIO_PIN_PROD_TEST_MODE   GPIO_PIN_14
#define GPIO_PORT_PROD_TEST_MODE  GPIOC

// DBG_GPIO2
#define GPIO_PIN_DBG_GPIO2        GPIO_PIN_15
#define GPIO_PORT_DBG_GPIO2       GPIOC

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef gpio_init(void);
void              gpio_turn_on_off_IGBT(const uint8_t state);
bool              gpio_get_test_pin(void);

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
