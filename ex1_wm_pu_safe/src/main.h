/**
 * @file     main.h
 * @brief    Main header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MAIN_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "stm32g0xx_hal.h"
#include "error.h"


////////////////////////////////////////////////////////////////////////////////
// Exported variables
////////////////////////////////////////////////////////////////////////////////
extern uint8_t gu8_enable_user_irq;


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////

//@{
/**
 * Self-Test Library defines.
 */
#define USARTx                     USART5

#define MAX_FLASH_LATENCY           FLASH_LATENCY_1
#define __HAL_RCC_CLEAR_FLAG        __HAL_RCC_CLEAR_RESET_FLAGS

#define __HAL_RCC_CLEAR_FLAG        __HAL_RCC_CLEAR_RESET_FLAGS
#define RCC_FLAG_PORRST             RCC_FLAG_PWRRST
#define RAM_BCKUP                   0

#define __DBGMCU_FREEZE_IWDG        __HAL_DBGMCU_FREEZE_IWDG
#define __DBGMCU_FREEZE_WWDG        __HAL_DBGMCU_FREEZE_WWDG
//@}

#define EPS_POWER_EN_PIN       GPIO_PIN_2   // SAFE_STD_BY_PIN
#define EPS_POWER_EN_PORT      GPIOC        // SAFE_STD_BY_GPIO_PORT
#define EPS_PIN                GPIO_PIN_7
#define EPS_PORT               GPIOA
#define LOCK_TRIAC_PIN         GPIO_PIN_12
#define LOCK_TRIAC_PORT        GPIOC
#define LOCK_IND_PIN           GPIO_PIN_15
#define LOCK_IND_PORT          GPIOB
#define DRAIN_PIN              GPIO_PIN_11
#define DRAIN_PORT             GPIOC
#define UART_EXT_TX_PIN        GPIO_PIN_0
#define UART_EXT_RX_PIN        GPIO_PIN_1
#define UART_EXT_PORT          GPIOB

#define EPS_DUTY_TIM_IC_CHANNEL     TIM_CHANNEL_1
#define EPS_PERIOD_TIM_IC_CHANNEL   TIM_CHANNEL_2

#define LOCK_PERIOD_TIM_IC_CHANNEL  TIM_CHANNEL_2

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void Jump_To_Main       (void);
void Safe_Main          (void);
void SystemInit         (void);
void StartUpClock_Config(void);
void StandbyClock_Config(void);
void print_stackdump    (void);

#ifdef __cplusplus
}
#endif

#endif // _MAIN_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
