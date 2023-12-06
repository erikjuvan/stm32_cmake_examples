////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     can_cfg.c
 * @brief    Header file for CAN driver
 * @author   Tomaz Opresnik
 * @date     26.3.2021
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup CAN_MODULE_CONFIGURATION
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __CAN_CFG_H
#define __CAN_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "priorities.h"
#include "project_config.h"

#include "can/can.h"

////////////////////////////////////////////////////////////////////////////////
// General settings
////////////////////////////////////////////////////////////////////////////////

#define CAN1_TX_PIN           GPIO_PIN_9
#define CAN1_TX_PORT          GPIOB
#define CAN1_TX_CLK_EN()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define CAN1_RX_PIN           GPIO_PIN_8
#define CAN1_RX_PORT          GPIOB
#define CAN1_RX_CLK_EN()      __HAL_RCC_GPIOB_CLK_ENABLE()
/*#define CAN1_RX_PIN                  GPIO_PIN_11
#define CAN1_RX_PORT                 GPIOA
#define CAN1_RX_CLK_EN()             __HAL_RCC_GPIOA_CLK_ENABLE()*/

#define CAN2_TX_PIN           GPIO_PIN_13
#define CAN2_TX_PORT          GPIOB
#define CAN2_TX_CLK_EN()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define CAN2_RX_PIN           GPIO_PIN_5
#define CAN2_RX_PORT          GPIOB
#define CAN2_RX_CLK_EN()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define CAN3_TX_PIN           GPIO_PIN_15
#define CAN3_TX_PORT          GPIOA
#define CAN3_TX_CLK_EN()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define CAN3_RX_PIN           GPIO_PIN_8
#define CAN3_RX_PORT          GPIOA
#define CAN3_RX_CLK_EN()      __HAL_RCC_GPIOA_CLK_ENABLE()

/**
 * Size of transmit fifo buffer used in channel initialization. Number defines number of messagess
 * in fifobuffer Where each message is of can_message_t type. Each message has the size of 70bytes.
 * Required memory is then 70 * this define (in bytes)
 */
#define CAN_TX_BUFFER_SIZE    (8)

/**
 * @{
 */
/** ISR priority
 * Each channel has two interrupts. Interrupts only exist if the channel exists. BUT the defines
 * should be provided for all the channels regardless of how many channels exist.
 */
#define CAN1_RX0_PRIORITY     (PROJECT_ISR_PRIO_CAN1_RX)
#define CAN1_RX0_SUB_PRIORITY (PROJECT_ISR_SUB_PRIO_CAN1_RX)
#define CAN1_TX_PRIORITY      (PROJECT_ISR_PRIO_CAN1_TX)
#define CAN1_TX_SUB_PRIORITY  (PROJECT_ISR_SUBPRIO_CAN1_TX)

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
#define CAN2_RX0_PRIORITY     (PROJECT_ISR_PRIO_CAN2_RX)
#define CAN2_RX0_SUB_PRIORITY (PROJECT_ISR_SUB_PRIO_CAN2_RX)
#define CAN2_TX_PRIORITY      (PROJECT_ISR_PRIO_CAN2_TX)
#define CAN2_TX_SUB_PRIORITY  (PROJECT_ISR_SUBPRIO_CAN2_TX)

#define CAN3_RX0_PRIORITY     (PROJECT_ISR_PRIO_CAN3_RX)
#define CAN3_RX0_SUB_PRIORITY (PROJECT_ISR_SUB_PRIO_CAN3_RX)
#define CAN3_TX_PRIORITY      (PROJECT_ISR_PRIO_CAN3_TX)
#define CAN3_TX_SUB_PRIORITY  (PROJECT_ISR_SUBPRIO_CAN3_TX)
#endif
/**
 * @}
 */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Debug settings
////////////////////////////////////////////////////////////////////////////////
/**
 * CAN asert mode disable/enable (0/1)
 */
#define CAN_ASSERT_ENABLE (0)

/**
 * CAN debug print disable/enable (0/1)
 */
#define CAN_DEBUG_ENABLE  (0)

// parasoft-begin-suppress FORMAT-02 FORMAT-03 FORMAT-34 "Used only for debugging purposes"
#if (CAN_DEBUG_ENABLE == 1)
#define CAN_PRINTF(...)                                   \
    sprintf(can_buff_msg_out, (const char *)__VA_ARGS__); \
    shell_write(can_buff_msg_out)
#else
#define CAN_PRINTF(...) \
    {                   \
        ;               \
    }
#endif

#if (CAN_ASSERT_ENABLE == 1)
#define CAN_ASSERT(x)                                                                          \
    if (!(x))                                                                                  \
    {                                                                                          \
        CAN_PRINTF("CAN ERROR: Assertion failed in file: %s, line: %d\n", __FILE__, __LINE__); \
        while (1)                                                                              \
        {                                                                                      \
        }                                                                                      \
    }
#else
#define CAN_ASSERT(x) \
    {                 \
        ;             \
    }
#endif
// parasoft-end-suppress FORMAT-02 FORMAT-03 FORMAT-34

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

#endif // __CAN_CFG_H
