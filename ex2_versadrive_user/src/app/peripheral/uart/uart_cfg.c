////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     uart_cfg.c
 * @brief    UART peripheral driver configuration file.
 * @author   Matej Otic
 * @author   Uros Sadek
 * @date     22.05.2021
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup UART_CONFIGURATION
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "uart_cfg.h"
#include "uart/uart.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Table containing pin definition for all available UART modules.
 * Number of entries in this table must match #UART_MAX_MODULES.
 */
static const uart_pins_t g_uart_pins[UART_MAX_MODULES] = {
  // Pins for UART 1 peripheral
    {.rx = {.pin = GPIO_PIN_7, .p_port = GPIOB},  .tx = {.pin = GPIO_PIN_6, .p_port = GPIOB} },

#if (UART_MAX_MODULES > 1)
  // Pins for UART 2 peripheral
    {.rx = {.pin = GPIO_PIN_4, .p_port = GPIOB},  .tx = {.pin = GPIO_PIN_3, .p_port = GPIOB} },
#endif

#if (UART_MAX_MODULES > 2)
  // Pins for UART 3 peripheral
    {.rx = {.pin = GPIO_PIN_11, .p_port = GPIOB}, .tx = {.pin = GPIO_PIN_10, .p_port = GPIOB}},
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *   This function is called after first UART peripheral is initialized and only
 *   once.
 *
 *   Function is used to initialize additional hardware parts that are needed
 *   for proper UART communication i.e. transceivers.\n
 *   If no additional initialization is required then the function should be
 *   left empty.
 *
 * @return       void
 */
////////////////////////////////////////////////////////////////////////////////
void uart_post_init(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup UART
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

//
// Leave below code unchanged!!
//

////////////////////////////////////////////////////////////////////////////////
/**
 *   Get pointer to UART pin definition table
 *
 * @return       Pointer to UART pin definition table.
 */
////////////////////////////////////////////////////////////////////////////////
const uart_pins_t *const uart_get_pin_table_ptr(void)
{
    return g_uart_pins;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
