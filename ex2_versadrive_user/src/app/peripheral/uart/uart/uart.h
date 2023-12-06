////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     uart.h
 * @brief    UART peripheral driver.
 * @author   Ziga Miklosic
 * @author   Matej Otic
 * @author   Uros Sadek
 * @date     31.05.2021
 * @version  V1.0.1
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup UART_API
 * @{ <!-- BEGIN GROUP -->
 *
 * Following module is part of API which is available to the user of UART Module.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __UART_H
#define __UART_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>

#include "stm32g4xx.h"

#include "../uart_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * UART peripheral pin mapping.
 */
struct uart_pins_s
{
    struct
    {
        uint16_t      pin;
        GPIO_TypeDef *p_port;
    } rx;

    struct
    {
        uint16_t      pin;
        GPIO_TypeDef *p_port;
    } tx;
};

/**
 * Return status enumeration.
 */
typedef enum
{
    eUART_OK,                      /**< Successful completion. */
    eUART_NOT_INITIALIZED,         /**< \p module has not yet been initialized. */
    eUART_MULTIPLE_CONFIGURATIONS, /**< \p module has already been initialized with different
                                      settings. */
    eUART_NULL_POINTER,            /**< Pointer has NULL value. */
    eUART_BUFFER_SIZE_ZERO,        /**< \p tx_buffer_size has 0 value. */
    eUART_SIZE_ZERO,               /**< \p size has 0 value. */
    eUART_INVALID_BAUDRATE,        /**< \p baudrate has less or equal to 0 value. */
    eUART_INVALID_MODULE,          /**< \p module_num is not valid value. */
    eUART_INVALID_PARITY,          /**< \p parity is not valid value. */
    eUART_INVALID_STOP_BIT,        /**< \p stop_bits is not valid value. */
    eUART_INVALID_SHIFTDIR,        /**< \p shiftdir is not valid value. */
    eUART_INVALID_PINS,          /**< Pins set in uart_cfg.c are NULL or not mapped to correct UART
                                    peripheral. */
    eUART_FIFO_CREATE_FAIL,      /**< Tx FIFO buffer could not be created. */
    eUART_FIFO_ADD_FAIL,         /**< Timeout reached when trying to add data to Tx FIFO. */
    eUART_MUTEX_ACQUIRE_TIMEOUT, /**< Timeout reached when trying to acquire Mutex for access to
                                    UART. */
    eUART_MUTEX_CREATE_FAIL,     /**< Mutex for access to UART could not be created. */
} uart_err_t;

/**
 * Available UART modules.
 * @note First must start with 0!!
 */
typedef enum
{
    eUART_MODULE_0 = 0, /**< UART peripheral module 0. */
#if (UART_MAX_MODULES > 1)
    eUART_MODULE_1, /**< UART peripheral module 1. */
#endif
#if (UART_MAX_MODULES > 2)
    eUART_MODULE_2, /**< UART peripheral module 2. */
#endif

    // Leave below unchanged!!
    eUART_MODULE_NUM /**< Number of available UART peripheral modules. */
} uart_module_num_t;

/**
 * Stop bit settings.
 */
typedef enum
{
    eUART_STOP_BIT_1, /**< Add 1 stop bit. */
    eUART_STOP_BIT_2  /**< Add 2 stop bits. */
} uart_stop_bits_t;

/**
 * Parity settings.
 */
typedef enum
{
    eUART_PARITY_NONE, /**< Do not add parity bit. */
    eUART_PARITY_ODD,  /**< Add odd parity bit. */
    eUART_PARITY_EVEN  /**< Add even parity bit. */
} uart_parity_t;

/**
 * Shift direction settings.
 */
typedef enum
{
    eUART_SHIFTDIR_LSB_FIRST, /**< Shift out Least Significant Bit first. */
    eUART_SHIFTDIR_MSB_FIRST  /**< Shift out Most Significant Bit first. */
} uart_shiftdir_t;

/**
 * Abstract structure for handling UART module instance.
 */
typedef struct uart_module_s *p_uart_module_t;

/**
 * Typedef for UART byte received callback function.
 */
typedef void (*uart_rx_callback_t)(const uint8_t byte);

/**
 * Typedef for UART byte send callback function.
 */
typedef void (*uart_tx_callback_t)(void);

/**
 * UART peripheral initialization structure.
 */
typedef struct
{
    uart_module_num_t  module_num;     /**< UART peripheral module number to use. */
    uint32_t           baudrate;       /**< Baudrate. */
    uint16_t           tx_buffer_size; /**< Tx FIFO buffer size in bytes. */
    uart_parity_t      parity;         /**< Parity bit. */
    uart_stop_bits_t   stop_bits;      /**< Stop bit. */
    uart_shiftdir_t    shiftdir;       /**< Shift direction. */
    uart_rx_callback_t p_rx_callback;  /**< Byte received callback. */
    uart_tx_callback_t p_tx_callback;  /**< Byte sent callback. */
} uart_init_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
uart_err_t uart_init(const uart_init_t *const p_init, p_uart_module_t *const p_module);
uart_err_t uart_is_module_init(const p_uart_module_t module);
void       uart_deinit(const p_uart_module_t module);
uart_err_t uart_send(const p_uart_module_t module, const uint8_t *pu8_data, uint16_t size);
bool       uart_is_tx_not_full(const p_uart_module_t module);
bool       uart_is_tx_in_progress(const p_uart_module_t module);

#endif // __UART_H
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
