//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        timer.c
// CREATION DATE:      2019/09/25
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Millisecond timer implementation definitions
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "cfg/platform_cfg.h"
#include "types.h"
#include "Platform/timer.h"
#include "Platform/proc.h"
#include "Platform/platform.h"

#ifndef RELOCATE_VECTORS_TO_RAM
#define RELOCATE_VECTORS_TO_RAM          1
#endif


uint32_t timer_freeRunningTimer;


// DESCRIPTION:
//   Registration of interrrupt service routine for timer functionality.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_register_ISR(void);


// DESCRIPTION:
//   Enabling of interrrupt for timer functionality.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_interrupt_enable(void);


// DESCRIPTION:
//   Disabling of interrrupt for timer functionality.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_interrupt_disable(void);



#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)
#include <project.h>

void timer_init(void)
{
    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */
    timer_1ms_Start();

    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    timer_1ms_STATUS;
    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    timer_1ms_ISR_StartEx(timer_ISR);
}

void timer_interrupt_enable(void)
{
    timer_1ms_ISR_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_5);
    timer_1ms_ISR_Enable();
}

void timer_interrupt_disable(void)
{
    timer_1ms_ISR_Disable();
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)
#include <project.h>

void timer_init(void)
{
    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */
    timer_1ms_Start();

    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    uint32_t status;

    /* Clear interrupt flag: */
    status = timer_1ms_INTERRUPT_REQ_REG; /* Instead of status = timer_1ms_GetInterruptSource(); - executing from RAM */
    timer_1ms_INTERRUPT_REQ_REG = status; /* Instead of timer_1ms_ClearInterrupt(status); - executing from RAM */

    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    timer_1ms_ISR_StartEx(timer_ISR);
}

void timer_interrupt_enable(void)
{
    timer_1ms_ISR_SetPriority(PLATFORM_INTERRUPT_PRIORITY_HIGH_1);
    timer_1ms_ISR_Enable();
}

void timer_interrupt_disable(void)
{
    timer_1ms_ISR_Disable();
}

#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

void timer_init(void)
{
    uint8_t Mask = 0;
    uint8_t NewPriority = 0;

    /* Define the mask corresponding to the bits position in the SPR register */
    /* The mask is reversed in order to clear the 2 bits after more easily */
    /* TIM4 = IRQ 23 */
    Mask = (uint8_t)(~(uint8_t)(0x03U << (((uint8_t)23 % 4U) * 2U)));
    
    /* Define the new priority to write */
    /* Priority level 1 */
    NewPriority = (uint8_t)((uint8_t)(1) << (((uint8_t)23 % 4U) * 2U));

    timer_freeRunningTimer = 0;

    /* DEINIT before INIT */
    TIM4->CR1 = TIM4_CR1_RESET_VALUE;
    TIM4->IER = TIM4_IER_RESET_VALUE;
    TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
    TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
    TIM4->ARR = TIM4_ARR_RESET_VALUE;
    TIM4->SR1 = TIM4_SR1_RESET_VALUE;

    /* Setting timer to overflow on 1 millisecond: */
    /* TIM4 prescaler can be: 2, 4, 8, 16, 32, 64, 128 */
    /* TIM4 autoreload value is calculated as: (desiredValue * cpuFreq) / prescalerValue = autoreload value */
    /* For 1ms: (0,001 * 16000000) / 128 = 125 */
    /* 125 = 0x7D */

    /* Set the Prescaler value */
    TIM4->PSCR = (uint8_t)(0x07);
    /* Set the Autoreload value */
    TIM4->ARR = (uint8_t)(0x7D);

    /* Setting new interrupt priority level */
    ITC->ISPR6 &= Mask;
    ITC->ISPR6 |= NewPriority;
}

void timer_deinit(void)
{
}

#warning Check proper executing from RAM:
#pragma PROC_RAM_FUNCTION_START
void timer_ISR(void)
{
    /* Clear the flags (rc_w0) clear this bit by writing 0. Writing 1 has no effect */
    TIM4->SR1 = (uint8_t)(~0x01);

    timer_freeRunningTimer++;
}
#pragma PROC_RAM_FUNCTION_END

void timer_register_ISR(void)
{
    /* Clear the flags (rc_w0) clear this bit by writing 0. Writing 1 has no effect */
    TIM4->SR1 = (uint8_t)(~0x01);
    /* Enable TIM4 */
    TIM4->CR1 |= TIM4_CR1_CEN;
}

void timer_interrupt_enable(void)
{
    /* Enable the Interrupt sources */
    TIM4->IER |= (uint8_t)0x01;
}

void timer_interrupt_disable(void)
{
    /* Disable the Interrupt sources */
    TIM4->IER &= (uint8_t)(~0x01);
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

#include "mcu.h"

#define stc_mftn_frt_t  FM3_MFT_FRT_TypeDef
#define MFT0_FRT        (*((volatile stc_mftn_frt_t *) FM3_MFT0_FRT_BASE))

void timer_init(void)
{
    uint32_t frequency;
    uint8_t clk;

    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */

    /* Free running timer UP counting: */
    bFM3_MFT0_FRT_TCSA0_MODE = 0u;

    /* Don't set ADC trigger: */
    bFM3_MFT0_FRT_TCSB0_AD0E = 0u;
    bFM3_MFT0_FRT_TCSB0_AD1E = 0u;
    bFM3_MFT0_FRT_TCSB0_AD2E = 0u;

    frequency = platform_timer_clock_source_frequency_get();
    clk = 0;
    while (frequency > 65535 * 1000)
    {
        frequency /= 2;
        clk++;
    }
    /* Set count clock cycle of FRT counter */
    bFM3_MFT0_FRT_TCSA0_CLK0 = ((clk & 0x01u) >> 0);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK1 = ((clk & 0x02u) >> 1);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK2 = ((clk & 0x04u) >> 2);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK3 = ((clk & 0x08u) >> 3);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */

    /* Don't set external clock enable bit */
    bFM3_MFT0_FRT_TCSA0_ECKE = 0u;

    /* Set buffer enable bit */
    bFM3_MFT0_FRT_TCSA0_BFE = 1u;

    /* Set interrupt enable for peak match irq */
    bFM3_MFT0_FRT_TCSA0_ICRE = 1u;

    timer_register_ISR();
    timer_interrupt_enable();

    /* Set FRT cycle value for 1ms */
    MFT0_FRT.TCCP0 = frequency / 1000;

    /* Start FRT */
    bFM3_MFT0_FRT_TCSA0_STOP = 0u;
    bFM3_MFT0_FRT_TCSA0_SCLR = 0u;
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void timer_ISR(void)
{
    /* Clear interrupt flag: */
    bFM3_MFT0_FRT_TCSA0_ICLR = 0;
    
    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!! 
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }
    __NVIC_SetVector(FRTIM_IRQn, (uint32_t)timer_ISR);
}

void timer_interrupt_enable(void)
{
    NVIC_ClearPendingIRQ(FRTIM_IRQn);
    NVIC_SetPriority(FRTIM_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_13);
    NVIC_EnableIRQ(FRTIM_IRQn);
}

void timer_interrupt_disable(void)
{
    NVIC_DisableIRQ(FRTIM_IRQn);
    NVIC_ClearPendingIRQ(FRTIM_IRQn);
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)

#include "mcu.h"

#define stc_mftn_frt_t  FM3_MFT_FRT_TypeDef
#define MFT0_FRT        (*((volatile stc_mftn_frt_t *) FM3_MFT0_FRT_BASE))

void timer_init(void)
{
    uint32_t frequency;
    uint8_t clk;

    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */

    /* Free running timer UP counting: */
    bFM3_MFT0_FRT_TCSA0_MODE = 0u;

    /* Don't set ADC trigger: */
    bFM3_MFT0_FRT_TCSB0_AD0E = 0u;

    frequency = platform_timer_clock_source_frequency_get();
    clk = 0;
    while (frequency > 65535 * 1000)
    {
        frequency /= 2;
        clk++;
    }
    /* Set count clock cycle of FRT counter */
    bFM3_MFT0_FRT_TCSA0_CLK0 = ((clk & 0x01u) >> 0);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK1 = ((clk & 0x02u) >> 1);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK2 = ((clk & 0x04u) >> 2);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */
    bFM3_MFT0_FRT_TCSA0_CLK3 = ((clk & 0x08u) >> 3);        /*  clk=0: FRT clock: PCLK/(2^0) divider  */

    /* Don't set external clock enable bit */
    bFM3_MFT0_FRT_TCSA0_ECKE = 0u;

    /* Set buffer enable bit */
    bFM3_MFT0_FRT_TCSA0_BFE = 1u;

    /* Set interrupt enable for peak match irq */
    bFM3_MFT0_FRT_TCSA0_ICRE = 1u;

    timer_register_ISR();
    timer_interrupt_enable();

    /* Set FRT cycle value for 1ms */
    MFT0_FRT.TCCP0 = frequency / 1000;

    /* Start FRT */
    bFM3_MFT0_FRT_TCSA0_STOP = 0u;
    bFM3_MFT0_FRT_TCSA0_SCLR = 0u;
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void timer_ISR(void)
{
    /* Clear interrupt flag: */
    bFM3_MFT0_FRT_TCSA0_ICLR = 0;
    
    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!! 
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }
    __NVIC_SetVector(FRTIM_IRQn, (uint32_t)timer_ISR);
}

void timer_interrupt_enable(void)
{
    NVIC_ClearPendingIRQ(FRTIM_IRQn);
    NVIC_SetPriority(FRTIM_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_13);
    NVIC_EnableIRQ(FRTIM_IRQn);
}

void timer_interrupt_disable(void)
{
    NVIC_DisableIRQ(FRTIM_IRQn);
    NVIC_ClearPendingIRQ(FRTIM_IRQn);
}

#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

#include "fsl_clock.h"
#include "fsl_ctimer.h"

#define CTIMER                  CTIMER3                 /* Timer 3 */
#define CTIMER_MAT0_OUT         kCTIMER_Match_0         /* Match output 0 */

/* Match Configuration for Channel 0 */
static ctimer_match_config_t matchConfig0;

void timer_init(void)
{
    ctimer_config_t config;

    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */

    /* Enable the asynchronous bridge */
    SYSCON->ASYNCAPBCTRL = 1;

    /* Use 12 MHz clock for some of the Ctimers */
    CLOCK_AttachClk(kFRO12M_to_ASYNC_APB);

    CTIMER_GetDefaultConfig(&config);
    CTIMER_Init(CTIMER, &config);

    /* Configuration 0 */
    matchConfig0.enableCounterReset = true;
    matchConfig0.enableCounterStop  = false;
    matchConfig0.matchValue         = platform_timer_clock_source_frequency_get() / 1000 + 1;   /* 1000 - 1 millisecond */
    matchConfig0.outControl         = kCTIMER_Output_NoAction;
    matchConfig0.outPinInitState    = false;
    matchConfig0.enableInterrupt    = true;

    timer_register_ISR();

    CTIMER_SetupMatch(CTIMER, CTIMER_MAT0_OUT, &matchConfig0);

    timer_interrupt_enable();

    CTIMER_StartTimer(CTIMER);
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    /* Clear interrupt flag: */
    /* Implement for other platform(s) */

    timer_freeRunningTimer++;
}

/* Array of function pointers for callback for each channel */
ctimer_callback_t ctimer_callback_table[] = 
{
    (ctimer_callback_t)timer_ISR, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

void timer_register_ISR(void)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!! 
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    CTIMER_RegisterCallBack(CTIMER, &ctimer_callback_table[0], kCTIMER_MultipleCallback);
}

void timer_interrupt_enable(void)
{
    NVIC_ClearPendingIRQ(CTIMER3_IRQn);
    NVIC_SetPriority(CTIMER3_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_13);
    NVIC_EnableIRQ(CTIMER3_IRQn);
}

void timer_interrupt_disable(void)
{
    NVIC_DisableIRQ(CTIMER3_IRQn);
    NVIC_ClearPendingIRQ(CTIMER3_IRQn);
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)

#include "cy_pdl.h"
#include "cybsp.h"

#define TIMER_1MS_TCPWM_CNT_NUM         (0UL)
#define TIMER_1MS_TCPWM_CNT_MASK        (1UL << TIMER_1MS_TCPWM_CNT_NUM)

void timer_init(void)
{
    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */
    Cy_TCPWM_Counter_Init(TIMER_1MS_HW, TIMER_1MS_TCPWM_CNT_NUM, &TIMER_1MS_config);    /* 1 ms timer initialization */
    Cy_TCPWM_Counter_Enable(TIMER_1MS_HW, TIMER_1MS_TCPWM_CNT_NUM);                     /* Enable the initialized counter */
    Cy_TCPWM_TriggerStart(TIMER_1MS_HW, TIMER_1MS_TCPWM_CNT_MASK);                      /* Then start the counter */

    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_deinit(void)
{
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    uint32_t interrupts;

    interrupts = Cy_TCPWM_GetInterruptStatusMasked(TIMER_1MS_HW, TIMER_1MS_TCPWM_CNT_NUM);
    if (0UL != (CY_TCPWM_INT_ON_TC & interrupts))
    {
        /* Handle the Terminal Count event */
        timer_freeRunningTimer++;
    }
    if (0UL != (CY_TCPWM_INT_ON_CC & interrupts))
    {
        /* Handle the Compare/Capture event */
    }
    /* Clear the interrupt */
    Cy_TCPWM_ClearInterrupt(TIMER_1MS_HW, TIMER_1MS_TCPWM_CNT_NUM, interrupts);
}

void timer_register_ISR(void)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!! 
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    __NVIC_SetVector(tcpwm_interrupts_0_IRQn, (uint32_t) timer_ISR);
}

void timer_interrupt_enable(void)
{
    NVIC_ClearPendingIRQ(tcpwm_interrupts_0_IRQn);
    NVIC_SetPriority(tcpwm_interrupts_0_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_13);
    NVIC_EnableIRQ(tcpwm_interrupts_0_IRQn);
}

void timer_interrupt_disable(void)
{
    NVIC_DisableIRQ(tcpwm_interrupts_0_IRQn);
    NVIC_ClearPendingIRQ(tcpwm_interrupts_0_IRQn);
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

/* Selects which hardware timer is used in this module. Value 4 means TIM4 ... */
#define TIMER_USED          HISENSE_PROTOCOL_TIMER_USED

#include "tim.h"
#include "hal_callbacks.h"
#if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
    #if (PROJECT_TYPE == PROJECT_TYPE_APPLICATION)
        #include "priorities.h"
    #elif (PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)
        #define TIM17_IRQ_PRIORITY                          ( 2u )
    #endif /* --> PROJECT_TYPE */
#else
    #define TIM17_IRQ_PRIORITY                          ( 2u )
#endif /* --> PROJECT_NAME */

TIM_HandleTypeDef* htim;

void timer_init(void)
{
    timer_freeRunningTimer = 0;
    htim = NULL;

    /* Setting timer to overflow on 1 millisecond: */
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 4)
        case 4:
            MX_TIM4_Init();
            htim = timer4_init();
            break;
        #endif /* (TIMER_USED == 4) */
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 17)
        case 17:
            MX_TIM17_Init();
            htim = timer17_init();
            break;
        #endif /* (TIMER_USED == 17) */
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */

    #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_MAIN)
        #if (PROJECT_TYPE == PROJECT_TYPE_APPLICATION)
            if ((htim == NULL) ||
                (htim->Init.Prescaler != 0) ||
                (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
                (htim->Init.Period != 15999) ||
                (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
        #elif (PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)
            if ((htim == NULL) ||
                (htim->Init.Prescaler != 0) ||
                (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
                (htim->Init.Period != 63999) ||
                (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
        #else
            /* Wrong project type setting. */
            proc_halt(0);
        #endif /* --> PROJECT_TYPE */
    #elif ((PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC))
        if ((htim == NULL) ||
            (htim->Init.Prescaler != 0) ||
            (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
            (htim->Init.Period != 63999) ||
            (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
    #elif ((PROJECT_NAME == PROJECT_NAME_DW50_GCUL))
        if ((htim == NULL) ||
            (htim->Init.Prescaler != 0) ||
            (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
            (htim->Init.Period != 15999) ||
            (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
    #elif ((PROJECT_NAME == PROJECT_NAME_WM22_PCU))
        if ((htim == NULL) ||
            (htim->Init.Prescaler != 31) ||
            (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
            (htim->Init.Period != 999) ||
            (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
    #else
        /* Add settings check for added project !!! */
        proc_halt(0);
    #endif /* --> PROJECT_NAME */
    {
        /* Wrong timer settings. Timer must overflow on 1 millisecond!!! */
        proc_halt(0);
    }

    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_deinit(void)
{
    if (htim == NULL)
    {
        return;
    }

    /* For TIM3 / TIM4 shared interrupt: only the timer that is used is disabled (not all together TIM3 and TIM4 shared interrupt): */
    timer_interrupt_disable();

    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 4)
        case 4:
            __HAL_RCC_TIM4_CLK_DISABLE();
            break;
        #endif /* (TIMER_USED == 4) */
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 17)
        case 17:
            __HAL_RCC_TIM17_CLK_DISABLE();
            break;
        #endif /* (TIMER_USED == 17) */
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */

    htim = NULL;
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    htim->Instance->SR &= ~TIM_SR_UIF; /* Clear interrupt flag */
    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    #if (RELOCATE_VECTORS_TO_RAM == 1)
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }
    #endif /* --> (RELOCATE_VECTORS_TO_RAM == 1) */

    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 4)
        case 4:
            hal_callback_tim4_register_callback((void*) timer_ISR);
            break;
        #endif /* (TIMER_USED == 4) */
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        #if (TIMER_USED == 17)
        case 17:
            hal_callback_tim17_register_callback((void*) timer_ISR);
            break;
        #endif /* (TIMER_USED == 17) */
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

void timer_interrupt_enable(void)
{
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
        case 4:
            /* Global TIM3_TIM4 shared interrupt: */
            HAL_NVIC_ClearPendingIRQ(TIM3_TIM4_IRQn);
            /* Timer interrupt priority must be higher than Tx interrupt of the all UARTs used for WiFi ports !!! */
            HAL_NVIC_SetPriority(TIM3_TIM4_IRQn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_1, 0);
            HAL_NVIC_EnableIRQ(TIM3_TIM4_IRQn);
            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);   /* Enable the TIM Update interrupt */
            break;
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 17:
            /* Global TIM17 interrupt: */

            #if defined(STM32G0B0xx)
                HAL_NVIC_ClearPendingIRQ(TIM17_IRQn);
                /* Timer interrupt priority must be higher than Tx interrupt of the all UARTs used for WiFi ports !!! */
                #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
                    HAL_NVIC_SetPriority(TIM17_IRQn, TIM17_IRQ_PRIORITY, 0);
                #else
                    HAL_NVIC_SetPriority(TIM17_IRQn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_1, 0);
                #endif
                HAL_NVIC_EnableIRQ(TIM17_IRQn);
            #elif defined(STM32G0B1xx)
                HAL_NVIC_ClearPendingIRQ(TIM17_FDCAN_IT1_IRQn);
                /* Timer interrupt priority must be higher than Tx interrupt of the all UARTs used for WiFi ports !!! */
                #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
                    HAL_NVIC_SetPriority(TIM17_FDCAN_IT1_IRQn, TIM17_IRQ_PRIORITY, 0);
                #else
                    HAL_NVIC_SetPriority(TIM17_FDCAN_IT1_IRQn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_1, 0);
                #endif
                HAL_NVIC_EnableIRQ(TIM17_FDCAN_IT1_IRQn);
            #else
                #error timer.c: MCU is not supported.
            #endif

            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);   /* Enable the TIM Update interrupt */
            break;
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

void timer_interrupt_disable(void)
{
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
        case 4:
            /* Global TIM3_TIM4 shared interrupt MUST NOT be disabled as it is shared between TIM3 and TIM4 timers !!! */

            __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);  /* Disable the TIM Update interrupt */
            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            break;
        case 6:
        case 7:
        case 14:
        case 15:
        case 16:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 17:
            /* Global TIM17 interrupt: */
            #if defined(STM32G0B0xx)
                HAL_NVIC_ClearPendingIRQ(TIM17_IRQn);
                HAL_NVIC_DisableIRQ(TIM17_IRQn);
            #elif defined(STM32G0B1xx)
                HAL_NVIC_ClearPendingIRQ(TIM17_FDCAN_IT1_IRQn);
                HAL_NVIC_DisableIRQ(TIM17_FDCAN_IT1_IRQn);
            #else
                #error timer.c: MCU is not supported.
            #endif
            __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);  /* Disable the TIM Update interrupt */
            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            break;
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

/* Selects which hardware timer is used in this module. Value 4 means TIM4 ... */
#define TIMER_USED          HISENSE_PROTOCOL_TIMER_USED

#include "tim.h"
#include "hal_callbacks.h"

TIM_HandleTypeDef* htim;

void timer_init(void)
{
    timer_freeRunningTimer = 0;
    htim = NULL;

    /* Setting timer to overflow on 1 millisecond: */
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 4:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 6:
        case 7:
        	break;
        case 11:
            MX_TIM11_Init();
            htim = timer11_init();
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */

#if (PROJECT_TYPE == PROJECT_TYPE_APPLICATION)
    if ((htim == NULL) ||
        (htim->Init.Prescaler != 999) ||
        (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
        (htim->Init.Period != 179) ||
        (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
    {
        /* Wrong timer settings. Timer must overflow on 1 millisecond!!! */
        proc_halt(0);
    }
#else
    if ((htim == NULL) ||
            (htim->Init.Prescaler != 1) ||
            (htim->Init.CounterMode != TIM_COUNTERMODE_UP) ||
            (htim->Init.Period != 7999) ||
            (htim->Init.ClockDivision != TIM_CLOCKDIVISION_DIV1))
        {
            /* Wrong timer settings. Timer must overflow on 1 millisecond!!! */
            proc_halt(0);
        }
#endif

    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    htim->Instance->SR &= ~TIM_SR_UIF; /* Clear interrupt flag */
    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 4:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 6:
        case 7:
            break;
        case 11:
            hal_callback_tim11_register_callback((void*) timer_ISR);
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

void timer_interrupt_enable(void)
{
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
        case 4:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 6:
        case 7:
        	break;
        case 11:
            /* Global TIM3_TIM4 shared interrupt: */
            HAL_NVIC_ClearPendingIRQ(TIM1_TRG_COM_TIM11_IRQn);
            /* Timer interrupt priority must be higher than Tx interrupt of the all UARTs used for WiFi ports !!! */
            HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_1, 0);
            HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);   /* Enable the TIM Update interrupt */
        	break;
        case 14:
        case 15:
        case 16:
        case 17:
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

void timer_interrupt_disable(void)
{
    switch (TIMER_USED)
    {
        case 1:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 3:
        case 4:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
        case 6:
        case 7:
            break;
        case 11:
            __HAL_TIM_DISABLE_IT(htim, TIM_IT_UPDATE);  /* Disable the TIM Update interrupt */
            htim->Instance->SR &= ~TIM_SR_UIF;          /* Clear interrupt flag */
            break;
        case 14:
        case 15:
        case 16:
        case 17:
        default:
            /* Unsupported timer usage */
            proc_halt(0);
            break;
    } /* -> switch (TIMER_USED) */
}

#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

#include <timer/timer.h>

/*
void timer_init(void)
{
    timer_freeRunningTimer = 0;

    // Setting timer to overflow on 1 millisecond:
    // Implement for other platform(s)
    timer_init();

    timer_register_ISR();
    timer_interrupt_enable();
}
*/


void timer7_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void timer7_ISR(void)
{
    /* Clear interrupt flag: */
    /* Implement for other platform(s) */

    timer_freeRunningTimer++;
}

/*
void timer_register_ISR(void)
{
    timer7_update_irq_set(timer_ISR);
    // Implement for other platform(s)
}

void timer_interrupt_enable(void)
{
    // Implement for other platform(s)
    timer7_update_irq_enable();
}

void timer_interrupt_disable(void)
{
    // Implement for other platform(s)
    timer7_update_irq_disable();
}
*/

#elif (PLATFORM_USED == PLATFORM_template)

#warning Timer: Implement new platform:

void timer_init(void)
{
    timer_freeRunningTimer = 0;

    /* Setting timer to overflow on 1 millisecond: */
    /* Implement for other platform(s) */


    timer_register_ISR();
    timer_interrupt_enable();
}

void timer_deinit(void)
{
}

#error timer_ISR() function MUST be run from RAM !!! Check and remove error.
void timer_ISR(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void timer_ISR(void)
{
    /* Clear interrupt flag: */
    /* Implement for other platform(s) */

    timer_freeRunningTimer++;
}

void timer_register_ISR(void)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!! 
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    /* Implement for other platform(s) */
}

void timer_interrupt_enable(void)
{
    /* Implement for other platform(s) */
    
    #warning Timer interrupt priority must be higher than Tx interrupt of the all UARTs used for WiFi ports !!!
}

void timer_interrupt_disable(void)
{
    /* Implement for other platform(s) */
}

#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error timer.c: Undefined or unsupported platform
#endif /* PLATFORM_USED */



/* Common part (platform independent): */

#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_ALWAYS_START
#else
void timer_start(TIMER *timer) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void timer_start(TIMER *timer)
{
    timer->started = 1;
    timer->value = timer_freeRunningTimer;
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif

void timer_stop(TIMER *timer)
{
    timer->started = 0;
}

void timer_clear(TIMER *timer)
{
    timer->started = 0;
    timer->value = 0;
}

uint32_t timer_elapsed(TIMER *timer)
{
    uint8_t enableInterrupts;
    uint32_t elapsed;

    enableInterrupts = proc_EnterCriticalSection();
    if (timer->started == 1)
    {
        elapsed = - (timer->value);         /* "timer->value" must be loaded before "timer_freeRunningTimer" !!!      */
        elapsed += timer_freeRunningTimer;  /* and in separate lines. DO NOT join together in one line !!!!!          */
    }
    else
    {
        elapsed = 0;
    }
    proc_ExitCriticalSection(enableInterrupts);

    return elapsed;
}

/* [] END OF FILE */
