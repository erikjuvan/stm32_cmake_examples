/**
 * @file     usart.c
 * @brief    UART driver module
 * @author   Erik Juvan
 * @date     6.12.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup USART
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "usart.h"
#include "stm32xx_STLlib.h"
#include "trace.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////
UART_HandleTypeDef UartHandle;  /**< Global UART handle for TRACE. Name is such since it is used by STL and I don't want to edit STL if not necessary. */


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize USART5.
 *
 * @return       None.
 */
void MX_USART5_UART_Init(void)
{
  UartHandle.Instance                    = USART5;
  UartHandle.Init.BaudRate               = 1000000;
  UartHandle.Init.WordLength             = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits               = UART_STOPBITS_1;
  UartHandle.Init.Parity                 = UART_PARITY_NONE;
  UartHandle.Init.Mode                   = UART_MODE_TX;
  UartHandle.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling           = UART_OVERSAMPLING_8;
  UartHandle.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
  UartHandle.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_StatusTypeDef ret;
  ret = HAL_UART_Init(&UartHandle);
  if (ret != HAL_OK) {
      TRACE_FAT("SMN ::: HAL_UART_Init error %d", ret);
    FailSafePOR();
  }
}

/**
 * HAL callback for UART initialization (MSP - MCU Support Package).
 *
 * @param[in]    uartHandle    Handle for the specific UART being initialized.
 * @return       None.
 */
void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if (uartHandle->Instance == USART5) {
    __HAL_RCC_USART5_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART5 GPIO Configuration
    PB0     ------> USART5_TX
    PB1     ------> USART5_RX
    */
    GPIO_InitStruct.Pin       = UART_EXT_TX_PIN | UART_EXT_RX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART5;
    HAL_GPIO_Init(UART_EXT_PORT, &GPIO_InitStruct);
  }
}

/**
 * HAL callback for UART deinitialization..
 *
 * @param[in]    uartHandle    Handle for the specific UART being deinitialized.
 * @return       None.
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if (uartHandle->Instance == USART5) {
    __HAL_RCC_USART5_CLK_DISABLE();

    /**USART5 GPIO Configuration
    PB0     ------> USART5_TX
    PB1     ------> USART5_RX
    */
    HAL_GPIO_DeInit(GPIOB, UART_EXT_TX_PIN | UART_EXT_RX_PIN);
  }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
