////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
// parasoft-begin-suppress ALL
/**
 * @file     uart.c
 * @brief    UART peripheral driver.
 * @author   Ziga Miklosic
 * @author   Matej Otic
 * @author   Uros Sadek
 * @date     22.05.2021
 * @version  V1.0.0
 *
 * @section Description
 *   This driver is built upon STM32 HAL library. It is only intended for use
 *   with STM MCUs.
 *
 * @note
 *   Since this driver was built for STM32G431xx variant with 3 UART peripherals,
 *   when ported to other MCUs #uart_module_num_t might need to be accommodated.
 *
 * @pre
 *   Before this module can be used following steps need to be completed:
 *   - Type __float32_t__ needs to be defined as 32-bit floating point in project_config.h.
 *   - Project has STM32 HAL libraries included.
 *   - Project has __fifobuffer__ module included and ALLOW_OVERFLOW disabled.
 *   - Configuration file set appropriately.
 *
 *   \n
 *   To use this driver first uart_init() function shall be called with #uart_init_t structure
 * filled with desired configuration settings. This function will create a new instance of UART
 * module.
 *
 * @code
 *   // Create UART module instance pointer
 *   p_uart_module_t uart_instance = NULL;
 *
 *   // Create configuration structure
 *   uart_init_t uart_init_config;
 *
 *   // Fill structure with desired values
 *   uart_init_config.module_num = eUART_MODULE_0;
 *   uart_init_config.baudrate = 115200;
 *   uart_init_config.tx_buffer_size = 256;
 *   uart_init_config.parity = eUART_PARITY_NONE;
 *   uart_init_config.stop_bits = eUART_STOP_BIT_1;
 *   uart_init_config.shiftdir = eUART_SHIFTDIR_LSB_FIRST;
 *   uart_init_config.p_rx_callback = rx_function;
 *   uart_init_config.p_tx_callback = tx_function;
 *
 *   // Call initialization function which returns pointer to UART module instance
 *   uart_init(&uart_init_config, &uart_instance);
 * @endcode
 *
 *   From this point on user can use this instance pointer to use other functionalities.
 *   To send date over UART use uart_send() function.
 *
 * @code
 *   // Some data buffer
 *   uint8_t buffer[5];
 *
 *   // Transmit data over UART (uart_instance is pointer to UART instance over which we want to
 * send) uart_send(uart_instance, buffer, 5);
 * @endcode
 *
 *   \n
 *   If RTOS mutex is used (#UART_USE_RTOS) then uart_send() function will be mutex protected. In
 * this case all of used UART instances will have its own RTOS mutex making it possible to send data
 * across same UART instance from multiple threads.\n Irregardless if RTOS mutex is enabled or not
 * uart_send() function is safe to call from multiple threads but in this case only for different
 * UART instances.
 *
 *   \n
 *   uart_send() implements timeout of maximum #UART_TX_TIMEOUT_TICKS number of CPU ticks. Timeout
 * is reset every time a byte is successfully added to Tx buffer. If #UART_TX_TIMEOUT_TICKS number
 * of CPU ticks passed without data added to buffer function will exit and error will be returned.
 *
 *   \n
 *   This driver allows configuration of the following settings per any UART module instance:
 *   - Baudrate,
 *   - Software Tx FIFO buffer size,
 *   - Parity bit,
 *   - Stop bit,
 *   - Shift direction.
 *
 *   Additionally every UART instance can have 2 callback's registered.
 *   One is for TX interrupt and another is for RX interrupt.
 *
 *   \n
 *   The following settings are not configurable and are hard-coded:
 *   - 8-bit data,
 *   - UART peripheral pre-divider set to 1,
 *   - Oversampling set to 16x per bit,
 *   - Used bit samples are 7th, 8th and 9th,
 *   - Rx pin has internal pull-up enabled,
 *   - Tx pin as push-pull,
 *   - HW flow control disabled
 *   - UART mode is RX/TX
 *
 *   Not implemented:
 *   - Error interrupt,
 *   - RTS/CTS handshake.
 */
// parasoft-end-suppress ALL
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup UART
 * @{ <!-- BEGIN GROUP -->
 *
 * All functions that are used inside module and not relevant to user.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "uart.h"
#if (UART_USE_RTOS == 1)
#include "uart_os.h"
#endif // (UART_USE_RTOS == 1)

#include "stm32g4xx_hal_uart.h"

#include "fifobuffer/fifobuffer/fifobuffer.h"

////////////////////////////////////////////////////////////////////////////////
// Check preprocessor configuration
////////////////////////////////////////////////////////////////////////////////

#if defined(UART3) || defined(USART3)
#define UART_NUM_MODULES 3
#elif defined(UART2) || defined(USART2)
#define UART_NUM_MODULES 2
#elif defined(UART1) || defined(USART1)
#define UART_NUM_MODULES 1
#else
#define UART_NUM_MODULES 0
#endif

#if (UART_MAX_MODULES < 1)
#error "URAT_MAX_MODULES must be at least 1."
#endif

#if (UART_MAX_MODULES > UART_NUM_MODULES)
#error "Not enough UART peripherals on device."
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define IS_GPIO_PORT(port)                                                       \
    ((port == GPIOA) || (port == GPIOB) || (port == GPIOC) || (port == GPIOD) || \
     (port == GPIOE) || (port == GPIOF) || (port == GPIOG))
#define __HAL_RCC_GPIOx_CLK_ENABLE(GPIOx) \
    switch ((uint32_t)GPIOx)              \
    {                                     \
    case (uint32_t)GPIOA:                 \
        __HAL_RCC_GPIOA_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOB:                 \
        __HAL_RCC_GPIOB_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOC:                 \
        __HAL_RCC_GPIOC_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOD:                 \
        __HAL_RCC_GPIOD_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOE:                 \
        __HAL_RCC_GPIOE_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOF:                 \
        __HAL_RCC_GPIOF_CLK_ENABLE();     \
        break;                            \
    case (uint32_t)GPIOG:                 \
        __HAL_RCC_GPIOG_CLK_ENABLE();     \
        break;                            \
    default:                              \
        break;                            \
    }

/**
 * Number of available ASCLIN peripherals on device.
 */
#define UART_NUM_UART_MODULES (UART_NUM_MODULES)

/**
 * Internal UART instance structure.
 */
typedef struct uart_module_s
{
    USART_TypeDef     *p_uart;        /**< Pointer to U(S)ART peripheral registers. */
    uart_module_num_t  module_num;    /**< Used module number. */
    fifo_t             tx_fifo;       /**< Tx FIFO buffer. */
    uint32_t           baudrate;      /**< Baudrate. */
    uart_parity_t      parity;        /**< Parity bit. */
    uart_stop_bits_t   stop_bits;     /**< Stop bit. */
    uart_shiftdir_t    shiftdir;      /**< Shift direction. */
    uart_tx_callback_t p_tx_callback; /**< Tx callback. */
    uart_rx_callback_t p_rx_callback; /**< Rx callback. */
    bool               is_init;       /**< Is module initialized. */
    bool               is_tx_in_prog; /**< Is Tx in progress. */
} uart_module_t;

/**
 * Default UART module settings.
 */
typedef struct
{
    USART_TypeDef *p_uart;       /**< Pointer to U(S)ART peripheral registers. */
    uint8_t        irq_priority; /**< Tx interrupt priority. */
    IRQn_Type      irq;          // IRQ type
    uint8_t        gpio_af;      // GPIO AF
} uart_default_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Array containing settings of all selected UART modules.
 */
static uart_module_t g_uart_modules[UART_MAX_MODULES] = {0};

/**
 * Set to true after uart_post_init() is called.
 */
static bool gb_is_post_init = false;

/**
 * Pointer to UART pin definition table.
 */
static const uart_pins_t *pg_uart_pins = NULL;

/**
 * Array containing default settings for all selected UART modules.
 */
static uart_default_t g_uart_defaults[UART_MAX_MODULES] = {
    {.p_uart       = USART1,
     .irq_priority = USART1_IRQ_PRIORITY,
     .irq          = USART1_IRQn,
     .gpio_af      = GPIO_AF7_USART1},
#if (UART_MAX_MODULES > 1)
    {.p_uart       = USART2,
     .irq_priority = USART2_IRQ_PRIORITY,
     .irq          = USART2_IRQn,
     .gpio_af      = GPIO_AF7_USART2},
#endif
#if (UART_MAX_MODULES > 2)
    {.p_uart       = USART3,
     .irq_priority = USART3_IRQ_PRIORITY,
     .irq          = USART3_IRQn,
     .gpio_af      = GPIO_AF7_USART3},
#endif
};

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static uart_err_t uart_is_config_valid(const uart_init_t *const p_init);
static uart_err_t uart_is_same_config(const uart_init_t *const p_init);
static uart_err_t
uart_module_init(const uart_init_t *const p_init, p_uart_module_t *const p_module);

static inline void uart_tx_byte(uart_module_t *const p_module);
static void        uart_tx_isr_template(uart_module_t *const p_module);
static void        uart_rx_isr_template(const uart_module_t *const p_module);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *   Check if configuration defined with \p p_init has valid values.
 *
 *   Check:
 *   - \p p_init is not NULL pointer,
 *   - \p p_init->module_num is smaller then #eUART_MODULE_NUM and smaller then
 *   number of peripherals on device,
 *   - \p p_init->baudrate positive value,
 *   - \p p_init->tx_buffer_size at least 1 byte,
 *   - \p p_init->parity one of #uart_parity_t values,
 *   - \p p_init->stop_bits one of #uart_stop_bits_t values,
 *   - \p p_init->shiftdir one of #uart_shiftdir_t values.
 *
 * @param[in]    p_init  Pointer to UART initialization structure.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static uart_err_t uart_is_config_valid(const uart_init_t *const p_init)
{
    uart_err_t err = eUART_OK;

    if (p_init == NULL)
    {
        err = eUART_NULL_POINTER;
    }

    if (err == eUART_OK)
    {
        if ((p_init->module_num >= eUART_MODULE_NUM) ||
            (p_init->module_num >= UART_NUM_UART_MODULES))
        {
            err = eUART_INVALID_MODULE;
        }
    }

    if (err == eUART_OK)
    {
        if (p_init->baudrate <= 0)
        {
            err = eUART_INVALID_BAUDRATE;
        }
    }

    if (err == eUART_OK)
    {
        if (p_init->tx_buffer_size == 0)
        {
            err = eUART_BUFFER_SIZE_ZERO;
        }
    }

    if (err == eUART_OK)
    {
        if ((p_init->parity != eUART_PARITY_ODD) && (p_init->parity != eUART_PARITY_EVEN) &&
            (p_init->parity != eUART_PARITY_NONE))
        {
            err = eUART_INVALID_PARITY;
        }
    }

    if (err == eUART_OK)
    {
        if ((p_init->stop_bits != eUART_STOP_BIT_1) && (p_init->stop_bits != eUART_STOP_BIT_2))
        {
            err = eUART_INVALID_STOP_BIT;
        }
    }

    if (err == eUART_OK)
    {
        if ((p_init->shiftdir != eUART_SHIFTDIR_LSB_FIRST) &&
            (p_init->shiftdir != eUART_SHIFTDIR_MSB_FIRST))
        {
            err = eUART_INVALID_SHIFTDIR;
        }
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Check if module number selected with \p p_init has same configuration as
 *   module number stored in #g_uart_modules table.
 *
 *   Check that values match:
 *   - baudrate,
 *   - parity,
 *   - stop bits,
 *   - shift direction,
 *   - Tx callback,
 *   - Rx callback.
 *
 * @note
 *   This function does not check for validity of \p p_init.
 *
 * @param[in]    p_init  Pointer to UART initialization structure.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static uart_err_t uart_is_same_config(const uart_init_t *const p_init)
{
    uart_err_t err;

    if ((g_uart_modules[p_init->module_num].baudrate == p_init->baudrate) &&
        (g_uart_modules[p_init->module_num].parity == p_init->parity) &&
        (g_uart_modules[p_init->module_num].stop_bits == p_init->stop_bits) &&
        (g_uart_modules[p_init->module_num].shiftdir == p_init->shiftdir) &&
        (g_uart_modules[p_init->module_num].p_tx_callback == p_init->p_tx_callback) &&
        (g_uart_modules[p_init->module_num].p_rx_callback == p_init->p_rx_callback))
    {
        err = eUART_OK;
    }
    else
    {
        err = eUART_MULTIPLE_CONFIGURATIONS;
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Initialize UART peripheral with settings defined with \p p_init and return
 *   pointer to initialized UART instance.
 *
 *   Additionally register Tx and Rx interrupts and create Tx FIFO buffer.
 *
 * @note
 *   This function does not check for validity of \p p_init.
 *
 * @param[in]    p_init      Pointer to UART initialization structure.
 * @param[out]   p_module    Pointer to initialized UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static uart_err_t uart_module_init(
    const uart_init_t *const p_init, p_uart_module_t *const p_module
) /* parasoft-suppress METRIC.CC "Intended functionality" */
{
    uart_err_t         err = eUART_OK;
    USART_TypeDef     *p_uart;
    UART_HandleTypeDef huart;

    // Get UART peripheral instance
    p_uart = g_uart_defaults[p_init->module_num].p_uart;

    // Enable clock
#if defined(UART4) || defined(USART4)
    if (p_uart == (USART_TypeDef *)UART4)
    {
        __HAL_RCC_UART4_CONFIG(RCC_UART4CLKSOURCE_PCLK1);
        __HAL_RCC_UART4_CLK_ENABLE();
    }
#endif
#if defined(UART3) || defined(USART3)
    if (p_uart == (USART_TypeDef *)USART3)
    {
        __HAL_RCC_USART3_CONFIG(RCC_USART3CLKSOURCE_PCLK1);
        __HAL_RCC_USART3_CLK_ENABLE();
    }
#endif
#if defined(UART2) || defined(USART2)
    if (p_uart == (USART_TypeDef *)USART2)
    {
        __HAL_RCC_USART2_CONFIG(RCC_USART2CLKSOURCE_PCLK1);
        __HAL_RCC_USART2_CLK_ENABLE();
    }
#endif
#if defined(UART1) || defined(USART1)
    if (p_uart == (USART_TypeDef *)USART1)
    {
        __HAL_RCC_USART1_CONFIG(RCC_USART1CLKSOURCE_HSI);
        __HAL_RCC_USART1_CLK_ENABLE();
    }
#endif

    huart.Instance                    = p_uart;
    huart.Init.BaudRate               = p_init->baudrate;
    huart.Init.WordLength             = UART_WORDLENGTH_8B;
    huart.Init.Mode                   = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    huart.Init.OverSampling           = UART_OVERSAMPLING_16;
    huart.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    huart.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
    huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    // Parity
    if (p_init->parity == eUART_PARITY_ODD)
    {
        huart.Init.Parity = UART_PARITY_ODD;
    }
    else if (p_init->parity == eUART_PARITY_EVEN)
    {
        huart.Init.Parity = UART_PARITY_EVEN;
    }
    else
    {
        huart.Init.Parity = UART_PARITY_NONE;
    }

    // Stop bit
    if (p_init->stop_bits == eUART_STOP_BIT_1)
    {
        huart.Init.StopBits = UART_STOPBITS_1;
    }
    else // eUART_STOP_BIT_2
    {
        huart.Init.StopBits = UART_STOPBITS_2;
    }

    // Shift direction
    huart.AdvancedInit.AdvFeatureInit |= UART_ADVFEATURE_MSBFIRST_INIT;
    if (p_init->shiftdir == eUART_SHIFTDIR_LSB_FIRST)
    {
        huart.AdvancedInit.MSBFirst = UART_ADVFEATURE_MSBFIRST_DISABLE;
    }
    else // eUART_SHIFTDIR_MSB_FIRST
    {
        huart.AdvancedInit.MSBFirst = UART_ADVFEATURE_MSBFIRST_ENABLE;
    }

    // Configure RX pin
    if (IS_GPIO_PIN(pg_uart_pins[p_init->module_num].rx.pin) &&
        IS_GPIO_PORT(pg_uart_pins[p_init->module_num].rx.p_port))
    {
        __HAL_RCC_GPIOx_CLK_ENABLE(pg_uart_pins[p_init->module_num].rx.p_port);
        /* parasoft-suppress MISRAC2012-RULE_16_4-b "Cannot add coment to macro expansion" */
        /* parasoft-suppress CODSTA-83 "External function" */
        GPIO_InitTypeDef gpio_InitStruct = {
            .Pin       = pg_uart_pins[p_init->module_num].rx.pin,
            .Mode      = GPIO_MODE_AF_PP,
            .Pull      = GPIO_PULLUP,
            .Speed     = GPIO_SPEED_FREQ_LOW,
            .Alternate = g_uart_defaults[p_init->module_num].gpio_af
        };
        HAL_GPIO_Init(pg_uart_pins[p_init->module_num].rx.p_port, &gpio_InitStruct);
    }
    else
    {
        err = eUART_INVALID_PINS;
    }

    // Configure TX pin
    if (IS_GPIO_PIN(pg_uart_pins[p_init->module_num].tx.pin) &&
        IS_GPIO_PORT(pg_uart_pins[p_init->module_num].tx.p_port))
    {
        __HAL_RCC_GPIOx_CLK_ENABLE(pg_uart_pins[p_init->module_num].tx.p_port);
        /* parasoft-suppress MISRAC2012-RULE_16_4-b "Cannot add coment to macro expansion" */
        /* parasoft-suppress CODSTA-83 "External function" */
        GPIO_InitTypeDef gpio_InitStruct = {
            .Pin       = pg_uart_pins[p_init->module_num].tx.pin,
            .Mode      = GPIO_MODE_AF_PP,
            .Pull      = GPIO_NOPULL,
            .Speed     = GPIO_SPEED_FREQ_LOW,
            .Alternate = g_uart_defaults[p_init->module_num].gpio_af
        };
        HAL_GPIO_Init(pg_uart_pins[p_init->module_num].tx.p_port, &gpio_InitStruct);
    }
    else
    {
        err = eUART_INVALID_PINS;
    }

    if (err == eUART_OK)
    {
        // Initialize UART peripheral
        if (HAL_UART_Init(&huart) != HAL_OK)
        {
            err = eUART_NOT_INITIALIZED;
        }
        if (HAL_UARTEx_SetTxFifoThreshold(&huart, (uint32_t)UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            err = eUART_NOT_INITIALIZED;
        }
        if (HAL_UARTEx_SetRxFifoThreshold(&huart, (uint32_t)UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            err = eUART_NOT_INITIALIZED;
        }
        if (HAL_UARTEx_EnableFifoMode(&huart) != HAL_OK)
        {
            err = eUART_NOT_INITIALIZED;
        }
    }

    if (err == eUART_OK)
    {
        // S/W Tx FIFO
        if (fifo_create(
                &g_uart_modules[p_init->module_num].tx_fifo, p_init->tx_buffer_size,
                (uint16_t)sizeof(uint8_t)
            ) != eFIFO_OK)
        {
            err = eUART_FIFO_CREATE_FAIL;
        }
    }

#if (UART_USE_RTOS == 1)
    if (err == eUART_OK)
    {
        err = uart_os_mutex_init(p_init->module_num);
    }
#endif // (UART_USE_RTOS == 1)

    if (err == eUART_OK)
    {
        // Module successfully initialized
        g_uart_modules[p_init->module_num].p_rx_callback = p_init->p_rx_callback;
        g_uart_modules[p_init->module_num].p_tx_callback = p_init->p_tx_callback;
        g_uart_modules[p_init->module_num].parity        = p_init->parity;
        g_uart_modules[p_init->module_num].shiftdir      = p_init->shiftdir;
        g_uart_modules[p_init->module_num].stop_bits     = p_init->stop_bits;
        g_uart_modules[p_init->module_num].module_num    = p_init->module_num;
        g_uart_modules[p_init->module_num].baudrate      = p_init->baudrate;
        g_uart_modules[p_init->module_num].p_uart = g_uart_defaults[p_init->module_num].p_uart;
        g_uart_modules[p_init->module_num].is_tx_in_prog = false;
        g_uart_modules[p_init->module_num].is_init       = true;
        *p_module                                        = &g_uart_modules[p_init->module_num];

        // Configure interrupt
        HAL_NVIC_SetPriority(
            g_uart_defaults[p_init->module_num].irq,
            (uint32_t)g_uart_defaults[p_init->module_num].irq_priority, 0UL
        );
        HAL_NVIC_EnableIRQ(g_uart_defaults[p_init->module_num].irq);

        // Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        SET_BIT(huart.Instance->CR3, USART_CR3_EIE);

        // Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
        // SET_BIT(huart.Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE_RXFNEIE);
        SET_BIT(huart.Instance->CR1, USART_CR1_PEIE);
        SET_BIT(huart.Instance->CR3, USART_CR3_RXFTIE);
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Retrieve byte from \p p_module UART module instance Tx FIFO buffer and
 *   send it over UART.
 *
 *   If byte could not be retrieved from FIFO set Tx in progress flag for
 *   \p p_module instance to false.
 *
 * @param[in]    p_module    Pointer to UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void uart_tx_byte(uart_module_t *const p_module)
{
    uint8_t byte;
    if (fifo_get(&p_module->tx_fifo, (void *)&byte) == eFIFO_OK)
    {
        p_module->p_uart->TDR = (uint8_t)(byte & (uint8_t)0xFF);
    }
    else
    {
        p_module->is_tx_in_prog = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   If defined Call Tx callback for \p p_module UART instance and retrieve if
 *   possible next byte from FIFO then send it.
 *
 * @param[in]    p_module    Pointer to UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void uart_tx_isr_template(uart_module_t *const p_module)
{
    if (p_module->is_init)
    {
        if (p_module->p_tx_callback != NULL)
        {
            p_module->p_tx_callback();
        }

        uart_tx_byte(p_module);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Read byte from \p p_module UART instance and if defined call Rx callback
 *   function for \p p_module with retrieved byte passed as argument.
 *
 * @param[in]    p_module    Pointer to UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void uart_rx_isr_template(const uart_module_t *const p_module)
{
    if (p_module->is_init)
    {
        // 8 byte FIFO enabled (less chance for overflow if TX IRQ is sending data constantly)
        // Read all RX FIFO values
        uint32_t isrflags = READ_REG(p_module->p_uart->ISR);
        while ((isrflags & USART_ISR_RXNE_RXFNE) != 0U
        ) /* parasoft-suppress CODSTA-83 "Intended functionality" */
        {
            uint8_t uhdata = (uint8_t)p_module->p_uart->RDR;
            if (p_module->p_rx_callback != NULL)
            {
                p_module->p_rx_callback(uhdata);
            }

            isrflags = READ_REG(p_module->p_uart->ISR);
            if ((isrflags & (USART_ISR_PE | USART_ISR_FE | USART_ISR_NE)) != 0U)
            {
                // Clear all errors
                p_module->p_uart->ICR = isrflags;
            }
        }

        // Read RX bytes one at a time
        /*uint8_t uhdata = (uint8_t)p_module->p_uart->RDR;
        if (p_module->p_rx_callback != NULL)
        {
            p_module->p_rx_callback(uhdata);
        }*/
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Read byte from \p p_module UART instance and if defined call Rx callback
 *   function for \p p_module with retrieved byte passed as argument.
 *
 * @param[in]    p_module    Pointer to UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
static void uart_isr_template(const uart_module_t *const p_module)
{
    if (p_module->is_init)
    {
        uint32_t isrflags = READ_REG(p_module->p_uart->ISR);
        uint32_t cr1its   = READ_REG(p_module->p_uart->CR1);
        uint32_t cr3its   = READ_REG(p_module->p_uart->CR3);

        uint32_t errorflags;

        /* If no error occurs */
        errorflags =
            (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE |
                                   USART_ISR_RTOF));
        if (errorflags == 0U)
        {
            /* UART in mode Receiver ---------------------------------------------------*/
            if (((isrflags & USART_ISR_RXNE_RXFNE) != 0U) &&
                (((cr1its & USART_CR1_RXNEIE_RXFNEIE) != 0U) || ((cr3its & USART_CR3_RXFTIE) != 0U)
                ))
            {
                uart_rx_isr_template(p_module);
                return;
            }
        }

        // If some errors occur */
        if ((errorflags != 0U) &&
            ((((cr3its & (USART_CR3_RXFTIE | USART_CR3_EIE)) != 0U) ||
              ((cr1its & (USART_CR1_RXNEIE_RXFNEIE | USART_CR1_PEIE | USART_CR1_RTOIE)) != 0U))))
        {
            // Clear all errors
            p_module->p_uart->ICR = errorflags;
        }

        // UART in mode Transmitter
        if (((isrflags & USART_ISR_TXE_TXFNF) != 0U) &&
            (((cr1its & USART_CR1_TXEIE_TXFNFIE) != 0U) || ((cr3its & USART_CR3_TXFTIE) != 0U)))
        {
            uart_tx_isr_template((uart_module_t *const)p_module);
            if (!p_module->is_tx_in_prog)
            {
                CLEAR_BIT(p_module->p_uart->CR1, USART_CR1_TXEIE_TXFNFIE);
                SET_BIT(p_module->p_uart->CR1, USART_CR1_TCIE);
            }

            return;
        }

        /* UART in mode Transmitter (transmission end) -----------------------------*/
        if (((isrflags & USART_ISR_TC) != 0U) && ((cr1its & USART_CR1_TCIE) != 0U))
        {
            CLEAR_BIT(p_module->p_uart->CR1, USART_CR1_TCIE);
            return;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Call UART interrupt handler uart_isr_template() with UART 1 instance as
 *   passed argument
 *
 * @return       void.
 */
////////////////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function, cannot be changed." */
{
    uart_isr_template(&g_uart_modules[eUART_MODULE_0]);
}

#if (UART_MAX_MODULES > 1)
////////////////////////////////////////////////////////////////////////////////
/**
 *   Call UART interrupt handler uart_isr_template() with UART 2 instance as
 *   passed argument
 *
 * @return       void.
 */
////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function, cannot be changed." */
{
    uart_isr_template(&g_uart_modules[eUART_MODULE_1]);
}

#endif // UART_MAX_MODULES > 1

#if (UART_MAX_MODULES > 2)
////////////////////////////////////////////////////////////////////////////////
/**
 *   Call UART interrupt handler uart_isr_template() with UART 3 instance as
 *   passed argument
 *
 * @return       void.
 */
////////////////////////////////////////////////////////////////////////////////
void USART3_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function, cannot be changed." */
{
    uart_isr_template(&g_uart_modules[eUART_MODULE_2]);
}

#endif // UART_MAX_MODULES > 2

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup UART_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *   Initialize UART peripheral with settings as defined with \p p_init and
 *   return initialized UART module instance.
 *
 *   First check if \p p_init has valid values set then check if UART module
 *   instance with used \p p_init->module_num has already been initialized.
 *   If it has make sure that they share the same settings, reutrn #eUART_OK
 *   otherwise #eUART_MULTIPLE_CONFIGURATIONS.\n
 *   This function will initialize UART peripheral with with pin mapping
 *   as set in uart_cfg.c file at selected \p p_init->module_num table
 *   index. It will also create Tx and Rx interrupts that get triggered
 *   on every byte received or send.
 *
 * @param[in]    p_init      Pointer to UART initialization structure.
 * @param[out]   p_module    Pointer to initialized UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
uart_err_t uart_init(const uart_init_t *const p_init, p_uart_module_t *const p_module)
{
    uart_err_t err = eUART_OK;

    if (pg_uart_pins == NULL)
    {
        pg_uart_pins = uart_get_pin_table_ptr();
        if (pg_uart_pins == NULL)
        {
            err = eUART_NULL_POINTER;
        }
    }

    if (err == eUART_OK)
    {
        err = uart_is_config_valid(p_init);
    }

    if (err == eUART_OK)
    {
        err = uart_is_module_init(&g_uart_modules[p_init->module_num]);
        if (err == eUART_OK)
        {
            // Module already initialized
            err = uart_is_same_config(p_init);
        }
        else if (err == eUART_NOT_INITIALIZED)
        {
            // Module not initialized
            err = uart_module_init(p_init, p_module);
        }
        else
        {
            // Invalid module
        }
    }

    if ((err == eUART_OK) && (gb_is_post_init == false))
    {
        uart_post_init();
        gb_is_post_init = true;
    }

    UART_ASSERT(err == eUART_OK);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Check if \p module UART module instance has already been initialized.
 *
 * @param[in]    module  UART module instance.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
uart_err_t uart_is_module_init(const p_uart_module_t module
) /* parasoft-suppress MISRAC2012-RULE_8_13-b "Same typedef used among all functions -- readability"
   */
{
    uart_err_t err;

    if (module == NULL)
    {
        err = eUART_NULL_POINTER;

        UART_ASSERT(0);
    }
    else
    {
        if (module->is_init)
        {
            err = eUART_OK;
        }
        else
        {
            err = eUART_NOT_INITIALIZED;
        }
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   De-initialize \p module UART module instance.
 *
 *   De-initialize interrupts, reset peripheral registers, flush H/W FIFOs and
 *   de-initialize used pins.
 *
 * @param[in]    module  UART module instance.
 * @return       void.
 */
////////////////////////////////////////////////////////////////////////////////
void uart_deinit(const p_uart_module_t module)
{
    if (uart_is_module_init(module) == eUART_OK)
    {
        // Disable IRQs
        HAL_NVIC_DisableIRQ(g_uart_defaults[module->module_num].irq);

        // DeInit UART
        UART_HandleTypeDef huart = {.Instance = module->p_uart};
        HAL_UART_DeInit(&huart);

        // DeInit GPIOs
        HAL_GPIO_DeInit(
            pg_uart_pins[module->module_num].rx.p_port,
            (uint32_t)pg_uart_pins[module->module_num].rx.pin
        );
        HAL_GPIO_DeInit(
            pg_uart_pins[module->module_num].tx.p_port,
            (uint32_t)pg_uart_pins[module->module_num].tx.pin
        );

        // De-init S/W Tx buffer
        fifo_destroy(&module->tx_fifo);

// Disable clock
#if defined(UART4) || defined(USART4)
        if (module->p_uart == (USART_TypeDef *)UART4)
        {
            __HAL_RCC_UART4_CLK_DISABLE();
        }
#endif
#if defined(UART3) || defined(USART3)
        if (module->p_uart == (USART_TypeDef *)USART3)
        {
            __HAL_RCC_USART3_CLK_DISABLE();
        }
#endif
#if defined(UART2) || defined(USART2)
        if (module->p_uart == (USART_TypeDef *)USART2)
        {
            __HAL_RCC_USART2_CLK_DISABLE();
        }
#endif
#if defined(UART1) || defined(USART1)
        if (module->p_uart == (USART_TypeDef *)USART1)
        {
            __HAL_RCC_USART1_CLK_DISABLE();
        }
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Send \p size number of bytes from \p pu8_data data buffer with \p module
 *   UART module instance.
 *
 *   Fill the Tx FIFO buffer then if Tx is not yet in progress for selected
 *   instance set Tx in progress flag to true and initiate transmission.
 *
 * @param[in]    module      UART module instance.
 * @param[in]    pu8_data    Pointer to data to send.
 * @param[in]    size        Number of bytes to send.
 * @return       #uart_err_t.
 */
////////////////////////////////////////////////////////////////////////////////
uart_err_t uart_send(const p_uart_module_t module, const uint8_t *pu8_data, uint16_t size)
{
    uart_err_t err     = uart_is_module_init(module);
    uint32_t   timeout = 0;

    if (err == eUART_OK)
    {
        if (pu8_data == NULL)
        {
            err = eUART_NULL_POINTER;
        }
    }

    if (err == eUART_OK)
    {
        if (size == 0)
        {
            err = eUART_SIZE_ZERO;
        }
    }

#if (UART_USE_RTOS == 1)
    if (err == eUART_OK)
    {
        err = uart_os_mutex_lock(module->module_num);
    }
#endif // (UART_USE_RTOS == 1)

    if (err == eUART_OK)
    {
        while (size > 0)
        {
            if (fifo_add(&module->tx_fifo, (void *)pu8_data) == eFIFO_OK)
            {
                timeout = 0;

                pu8_data++;
                size--;
            }

            // Continuously increment for timeout and when byte is added reset timeout value
            timeout++;
            if (timeout >= UART_TX_TIMEOUT_TICKS)
            {
                err = eUART_FIFO_ADD_FAIL;
                break;
            }
        }

#if (UART_USE_RTOS == 1)
        (void)uart_os_mutex_unlock(module->module_num);
#endif // (UART_USE_RTOS == 1)

        if (module->is_tx_in_prog == false)
        {
            module->is_tx_in_prog = true;

            // Just enable TX empty interrupt and transmission will start until there are bytes in
            // TX fifo
            SET_BIT(module->p_uart->CR1, USART_CR1_TXEIE_TXFNFIE);
        }
    }

    UART_ASSERT(err == eUART_OK);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Checking function if there is some space available in TX fifo buffer.
 *
 *    Function returns true if there is still space available in TX fifo, else it returns false.
 *
 * @param[in]    module      UART module instance.
 * @return       true         FIFO has some space available
 *                 false        FIFO is full
 */
////////////////////////////////////////////////////////////////////////////////
bool uart_is_tx_not_full(const p_uart_module_t module)
{
    uart_err_t err  = uart_is_module_init(module);
    bool       bRet = false;

    if (err == eUART_OK)
    {
        uint16_t space_left;
        fifo_space(&module->tx_fifo, &space_left);

        if (space_left > 0)
        {
            bRet = true;
        }
        else
        {
            bRet = false;
        }
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Checking function if TX is in progress.
 *
 *    Function returns true if transmitter is in progress, else it returns false.
 *
 * @param[in]    module      UART module instance.
 * @return       true         TX is in progress
 *                 false        TX in idle
 */
////////////////////////////////////////////////////////////////////////////////
bool uart_is_tx_in_progress(const p_uart_module_t module)
{
    uart_err_t err  = uart_is_module_init(module);
    bool       bRet = false;

    if (err == eUART_OK)
    {
        return module->is_tx_in_prog;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
