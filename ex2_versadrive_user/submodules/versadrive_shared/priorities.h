////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     priorities.h
* @brief    Interrupt Request and DMA priorities.
* @author   Matej Otic
* @date     25.08.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup PRIORITIES
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PRIORITIES_H
#define __PRIORITIES_H

////////////////////////////////////////////////////////////////////////////////
// Priorities
////////////////////////////////////////////////////////////////////////////////

//@{
/**
 * Interrupt Request priorities.
 */
#define PENDSV_IRQ_PRIORITY                 ( 15UL )
#define SYSTICK_IRQ_PRIORITY                ( 15UL )
#define EXTI2_IRQ_PRIORITY                  ( 10UL )
#define EXTI3_IRQ_PRIORITY                  ( 2UL )
#define DMA1_CHANNEL1_IRQ_PRIORITY          ( 3UL )
#define ADC1_2_IRQ_PRIORITY                 ( 0UL )
#define FDCAN1_IT0_IRQ_PRIORITY             ( 5UL )
#define FDCAN1_IT1_IRQ_PRIORITY             ( 5UL )
#define TIM1_UP_TIM16_IRQ_PRIORITY          ( 1UL )
#define TIM3_IRQ_PRIORITY                   ( 4UL )
#define TIM17_IRQ_PRIORITY                  ( 4UL )
#define TIM4_IRQ_PRIORITY                   ( 4UL )
#define USART1_IRQ_PRIORITY                 ( 6UL )
#define USART2_IRQ_PRIORITY                 ( 6UL )
#define USART3_IRQ_PRIORITY                 ( 6UL )
#define EXTI5_10_IRQ_PRIORITY               ( 7UL )
#define TIM7_IRQ_PRIORITY                   ( 12UL )
//@}

//@{
/**
 * DMA Channel priorities.
 */
#define DMA1_CHANNEL1_CH_PRIORITY           ( DMA_PRIORITY_LOW )
//@}

#endif // __PRIORITIES_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
