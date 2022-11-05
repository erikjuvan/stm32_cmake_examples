/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lock.h"
#include "stm32g0xx_it.h"
#include "main.h"
#include "eps.h"
#include "func_table.h"
#include "stm32xx_STLparam.h"
#include "stm32xx_STLlib.h"
#include "stm32xx_STLapp.h"


/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef void ( *pUser_IrqHandler_t )(void);

/* Private define ------------------------------------------------------------*/
#define    OFFSET__estack                                       0x00
#define    OFFSET_Reset_Handler                                 0x04
#define    OFFSET_NMI_Handler                                   0x08
#define    OFFSET_HardFault_Handler                             0x0C
#define    OFFSET_SVC_Handler                                   0x2C
#define    OFFSET_PendSV_Handler                                0x38
#define    OFFSET_SysTick_Handler                               0x3C
#define    OFFSET_WWDG_IRQHandler                               0x40
#define    OFFSET_PVD_VDDIO2_IRQHandler                         0x44
#define    OFFSET_RTC_TAMP_IRQHandler                           0x48
#define    OFFSET_FLASH_IRQHandler                              0x4C
#define    OFFSET_RCC_CRS_IRQHandler                            0x50
#define    OFFSET_EXTI0_1_IRQHandler                            0x54
#define    OFFSET_EXTI2_3_IRQHandler                            0x58
#define    OFFSET_EXTI4_15_IRQHandler                           0x5C
#define    OFFSET_USB_UCPD1_2_IRQHandler                        0x60
#define    OFFSET_DMA1_Channel1_IRQHandler                      0x64
#define    OFFSET_DMA1_Channel2_3_IRQHandler                    0x68
#define    OFFSET_DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler  0x6C
#define    OFFSET_ADC1_COMP_IRQHandler                          0x70
#define    OFFSET_TIM1_BRK_UP_TRG_COM_IRQHandler                0x74
#define    OFFSET_TIM1_CC_IRQHandler                            0x78
#define    OFFSET_TIM2_IRQHandler                               0x7C
#define    OFFSET_TIM3_TIM4_IRQHandler                          0x80
#define    OFFSET_TIM6_DAC_LPTIM1_IRQHandler                    0x84
#define    OFFSET_TIM7_LPTIM2_IRQHandler                        0x88
#define    OFFSET_TIM14_IRQHandler                              0x8C
#define    OFFSET_TIM15_IRQHandler                              0x90
#define    OFFSET_TIM16_FDCAN_IT0_IRQHandler                    0x94
#define    OFFSET_TIM17_FDCAN_IT1_IRQHandler                    0x98
#define    OFFSET_I2C1_IRQHandler                               0x9C
#define    OFFSET_I2C2_3_IRQHandler                             0xA0
#define    OFFSET_SPI1_IRQHandler                               0xA4
#define    OFFSET_SPI2_3_IRQHandler                             0xA8
#define    OFFSET_USART1_IRQHandler                             0xAC
#define    OFFSET_USART2_LPUART2_IRQHandler                     0xB0
#define    OFFSET_USART3_4_5_6_LPUART1_IRQHandler               0xB4
#define    OFFSET_CEC_IRQHandler                                0xB8

/* External variables --------------------------------------------------------*/
extern const uint32_t _MEMPART_FLASH_USER_APP_ADDR; // address from linker script

extern WWDG_HandleTypeDef WwdgHandle;

/* Private macro -------------------------------------------------------------*/

/**
* Call USER IRQ handler at offset \p offset in USER isr_vector section.
* 1. Get USER ISR vector table entry address for ISR selected with \p offset
* 2. Convert address to 4 byte pointer type - which is size of an ISR vector table entry
* 3. Get address pointed to by the vector table entry - which is USER ISR handler function
* 4. Convert address to void function pointer type - which is type of ISR handler functions
* 5. Call USER ISR handler function
*/
#define CALL_USER_IRQ_HANDLER(offset)                                                                       \
/*4.*/     ( (pUser_IrqHandler_t) (                                                                         \
/*3.*/                                *(                                                                    \
/*2.*/                                    (uint32_t*)(                                                      \
/*1.*/                                                (uint32_t)(&_MEMPART_FLASH_USER_APP_ADDR) + offset    \
/*2.*/                                               )                                                      \
/*3.*/                                 )                                                                    \
/*4.*/                             )                                                                        \
/*5.*/     )()

/* Private variables ---------------------------------------------------------*/
static struct {
	uint32_t nmi_ram_parity;
} safe_irq_flags_count = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
#ifdef LOG_VIA_UART
    // Print why it happened
    printf("\nNMI IRQ: SRAM parity: %d  FLASH ECCD1: %d  FLASH ECCD2: %d  CSS: %d\n",
        (SYSCFG->CFGR2 & SYSCFG_CFGR2_SPF) == SYSCFG_CFGR2_SPF,
        (FLASH->ECCR & FLASH_ECCR_ECCD) == FLASH_ECCR_ECCD,
        (FLASH->ECC2R & FLASH_ECC2R_ECCD) == FLASH_ECC2R_ECCD,
        __HAL_RCC_GET_IT(RCC_IT_CSS));

    print_stackdump();
#endif

	// Check for SRAM parity error flag
	if (SYSCFG->CFGR2 & SYSCFG_CFGR2_SPF)
	{
		// Clear SRAM parity error flag
		SYSCFG->CFGR2 |= SYSCFG_CFGR2_SPF;

		// increment local counter (used for debug)
		safe_irq_flags_count.nmi_ram_parity++;
	}

    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_NMI_Handler);
    }

    STL_NMI_Handler();
    FailSafePOR();
}

/**
  * @brief This function handles Hard Fault.
  */
void HardFault_Handler(void)
{
    print_stackdump();
    STL_HardFault_Handler();
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
    STL_SVC_Handler();
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
    STL_PendSV_Handler();
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
    static uint8_t  systick_counter = 0;
    static uint32_t count_per_ms    = 0;
    static bool     is_count_rtv    = false;

    //
    // HAL_IncTick() is called in STL_SysTick_Handler()
    //

    if (STL_GetIsRunTimeInitialized() != true)  // STL Startup check
    {
        // Nothing to do here
    }
    else                                        // STL Runtime check
    {
        //////////////////////////////////////////////////////////
        //                      SAFE                            //
        //////////////////////////////////////////////////////////

        // Retrieve SysTick frequency set by USER app to check
        // if STL_SysTick_Handler should be called. It is foreseen that
        // STL_SysTick_Handler is called with 1000Hz, but USER app may need
        // faster SysTick frequency due to motor control algorithms.
        if (is_count_rtv != true)
        {
            count_per_ms = /*pg_user_func_table->fp_Get_SYS_TICK_FREQUENCY()*/ 1000 / 1000ul;
            is_count_rtv = true;
        }

        if (systick_counter >= count_per_ms)
        {
            systick_counter = 0;

            STL_SysTick_Handler();
        }
        systick_counter++;

        //////////////////////////////////////////////////////////
        //                      USER                            //
        //////////////////////////////////////////////////////////
        if(gu8_enable_user_irq != 0)
        {
            CALL_USER_IRQ_HANDLER(OFFSET_SysTick_Handler);
        }
    }
}

/**
  * @brief This function handles WWDG_IR interrupt.
  */
void WWDG_IRQHandler(void)
{
    // Reset WWDG counter to give user enough time to prepare for reset (IWDG - WWDG)
    HAL_WWDG_Refresh(&WwdgHandle);

    // Clear WWDG Early Wakeup flag to prevent multiple IRQ triggers
    HAL_WWDG_IRQHandler(&WwdgHandle);

    print_stackdump();

    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_WWDG_IRQHandler);
    }
}


/**
  * @brief This function handles PVD_VDDIO2_IR interrupt.
  */
void PVD_VDDIO2_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_PVD_VDDIO2_IRQHandler);
    }
}


/**
  * @brief This function handles RTC_TAMP_IR interrupt.
  */
void RTC_TAMP_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_RTC_TAMP_IRQHandler);
    }
}


/**
  * @brief This function handles FLASH_IR interrupt.
  */
void FLASH_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_FLASH_IRQHandler);
    }
}


/**
  * @brief This function handles RCC_CRS_IR interrupt.
  */
void RCC_CRS_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_RCC_CRS_IRQHandler);
    }
}


/**
  * @brief This function handles EXTI0_1_IR interrupt.
  */
void EXTI0_1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_EXTI0_1_IRQHandler);
    }
}


/**
  * @brief This function handles EXTI2_3_IR interrupt.
  */
void EXTI2_3_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_EXTI2_3_IRQHandler);
    }
}


/**
  * @brief This function handles EXTI4_15_IR interrupt.
  */
void EXTI4_15_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_EXTI4_15_IRQHandler);
    }
}


/**
  * @brief This function handles USB_UCPD1_2_IR interrupt.
  */
void USB_UCPD1_2_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_USB_UCPD1_2_IRQHandler);
    }
}


/**
  * @brief This function handles DMA1_Channel1_IR interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_DMA1_Channel1_IRQHandler);
    }
}


/**
  * @brief This function handles DMA1_Channel2_3_IR interrupt.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_DMA1_Channel2_3_IRQHandler);
    }
}


/**
  * @brief This function handles DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IR interrupt.
  */
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler);
    }
}


/**
  * @brief This function handles ADC1_COMP_IR interrupt.
  */
void ADC1_COMP_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_ADC1_COMP_IRQHandler);
    }
}


/**
  * @brief This function handles TIM1_BRK_UP_TRG_COM_IR interrupt.
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM1_BRK_UP_TRG_COM_IRQHandler);
    }
}


/**
  * @brief This function handles TIM1_CC_IR interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM1_CC_IRQHandler);
    }
}


/**
  * @brief This function handles TIM2_IR interrupt.
  */
void TIM2_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM2_IRQHandler);
    }
}


/**
  * @brief This function handles TIM3_TIM4_IR interrupt.
  */
void TIM3_TIM4_IRQHandler(void)
{
    eps_timer_isr();

    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM3_TIM4_IRQHandler);
    }
}


/**
  * @brief This function handles TIM6_DAC_LPTIM1_IR interrupt.
  */
void TIM6_DAC_LPTIM1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM6_DAC_LPTIM1_IRQHandler);
    }
}


/**
  * @brief This function handles TIM7_LPTIM2_IR interrupt.
  */
void TIM7_LPTIM2_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM7_LPTIM2_IRQHandler);
    }
}


/**
 * @brief This function handles TIM14 global interrupt.
 */
void TIM14_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM14_IRQHandler);
    }
}

/**
 * @brief This function handles TIM15 global interrupt.
 */
void TIM15_IRQHandler(void)
{
    lock_timer_isr();

    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM15_IRQHandler);
    }
}

/**
 * @brief This function handles TIM16, FDCAN1_IT0, FDCAN2_IT0 global interrupts.
 */
void TIM16_FDCAN_IT0_IRQHandler(void)
{
    STL_TIM16_IRQHandler();

    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM16_FDCAN_IT0_IRQHandler);
    }
}

/**
  * @brief This function handles TIM17_FDCAN_IT1_IR interrupt.
  */
void TIM17_FDCAN_IT1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_TIM17_FDCAN_IT1_IRQHandler);
    }
}


/**
  * @brief This function handles I2C1_IR interrupt.
  */
void I2C1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_I2C1_IRQHandler);
    }
}


/**
  * @brief This function handles I2C2_3_IR interrupt.
  */
void I2C2_3_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_I2C2_3_IRQHandler);
    }
}


/**
  * @brief This function handles SPI1_IR interrupt.
  */
void SPI1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_SPI1_IRQHandler);
    }
}


/**
  * @brief This function handles SPI2_3_IR interrupt.
  */
void SPI2_3_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_SPI2_3_IRQHandler);
    }
}


/**
  * @brief This function handles USART1_IR interrupt.
  */
void USART1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_USART1_IRQHandler);
    }
}


/**
  * @brief This function handles USART2_LPUART2_IR interrupt.
  */
void USART2_LPUART2_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_USART2_LPUART2_IRQHandler);
    }
}


/**
  * @brief This function handles USART3_4_5_6_LPUART1_IR interrupt.
  */
void USART3_4_5_6_LPUART1_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_USART3_4_5_6_LPUART1_IRQHandler);
    }
}


/**
  * @brief This function handles CEC_IR interrupt.
  */
void CEC_IRQHandler(void)
{
    if(gu8_enable_user_irq != 0)
    {
        CALL_USER_IRQ_HANDLER(OFFSET_CEC_IRQHandler);
    }
}

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
