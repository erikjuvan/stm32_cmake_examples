//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        platform.c
// CREATION DATE:      2019/12/03
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Platform setup Hardware Abstraction Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------


#include "types.h"
#include "cfg/project_cfg.h"
#include "cfg/platform_cfg.h"
#include "Platform/proc.h"
#include "Platform/timer.h"

#ifndef RELOCATE_VECTORS_TO_RAM
#define RELOCATE_VECTORS_TO_RAM          1
#endif


/*
----------------------------------------------------------------------------
LOCAL FUNCTIONS.
----------------------------------------------------------------------------
*/

#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

void platform_HardFaultHandler_register(void)
{
    /* Nothing to do */
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

__attribute__ ((noreturn))
void platform_HardFaultHandler(void) PROC_RAM_FUNCTION;
void platform_HardFaultHandler(void)
{
    /***************************************************************************
    * We must not get here. If we do, a serious problem occurs, so go into
    * an infinite loop.
    ***************************************************************************/
    while (1)
    {
        CY_NOP;
    }
}

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

void platform_HardFaultHandler_register(void)
{
    CyRamVectors[CY_INT_HARD_FAULT_IRQN] = platform_HardFaultHandler;
}

#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

void platform_HardFaultHandler_register(void)
{
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

void platform_HardFaultHandler_register(void)
{
    /* Nothing to do */
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

void platform_HardFaultHandler_register(void)
{
    /* Nothing to do */
}

#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

#include "clock_config.h"

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
    BOARD_BootClockPLL180M();           /* LPC54607: maximum frequency is 180 MHz !!! */
}

void platform_CPUclock_deinit(void)
{
}

__attribute__ ((noreturn))
PROC_RAM_FUNCTION_INFRONT
void platform_HardFaultHandler(void) PROC_RAM_FUNCTION;
void platform_HardFaultHandler(void)
{
    /***************************************************************************
    * We must not get here. If we do, a serious problem occurs, so go into
    * an infinite loop.
    ***************************************************************************/

    /* If "A derived bus fault has occurred on exception entry (CFSR.STKERR, BFAR)", increase Stack */

    while (1)
    {
    }
}

void platform_HardFaultHandler_register(void)
{
    __NVIC_SetVector(HardFault_IRQn, (uint32_t)platform_HardFaultHandler);
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)

#include "cy_pdl.h"
#include "cybsp.h"

#define CY_ASSERT_FAILED (0u)

void platform_bsp_init(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
    /* 
    Everything already done in
    cybsp_init() --> init_cycfg_all() --> init_cycfg_system() and init_cycfg_clocks()
    */
}

void platform_CPUclock_deinit(void)
{
}

__attribute__ ((noreturn))
PROC_RAM_FUNCTION_INFRONT
void platform_HardFaultHandler(void) PROC_RAM_FUNCTION;
void platform_HardFaultHandler(void)
{
    /***************************************************************************
    * We must not get here. If we do, a serious problem occurs, so go into
    * an infinite loop.
    ***************************************************************************/

    /* 
    See Cy_SysLib_ProcessingFault() function, 
    but note that the platform_HardFaultHandler() function has to be executed from RAM !!!
    */

    while (1)
    {
        __NOP();
    }
}

void platform_HardFaultHandler_register(void)
{
    __NVIC_SetVector(HardFault_IRQn, (uint32_t)platform_HardFaultHandler);
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

void platform_bsp_init(void)
{
}

void platform_bsp_deinit(void)
{
}

void platform_CPUclock_setup(void)
{
}

void platform_CPUclock_deinit(void)
{
}

__attribute__ ((noreturn))
PROC_RAM_FUNCTION_INFRONT
void platform_HardFaultHandler(void) PROC_RAM_FUNCTION;
void platform_HardFaultHandler(void)
{
    /***************************************************************************
    * We must not get here. If we do, a serious problem occurs, so go into
    * an infinite loop.
    ***************************************************************************/

    /*
    Note that the platform_HardFaultHandler() function has to be executed from RAM !!!
    */

    while (1)
    {
        __NOP();
    }
}

void platform_HardFaultHandler_register(void)
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

    NVIC_SetVector(HardFault_IRQn, (uint32_t)platform_HardFaultHandler);
}
#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

void platform_bsp_init(void)
{
}

void platform_CPUclock_setup(void)
{
}

__attribute__ ((noreturn))
PROC_RAM_FUNCTION_INFRONT
void platform_HardFaultHandler(void) PROC_RAM_FUNCTION;
void platform_HardFaultHandler(void)
{
    /***************************************************************************
    * We must not get here. If we do, a serious problem occurs, so go into
    * an infinite loop.
    ***************************************************************************/

    /*
    Note that the platform_HardFaultHandler() function has to be executed from RAM !!!
    */

    while (1)
    {
        __NOP();
    }
}

void platform_HardFaultHandler_register(void)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    __NVIC_SetVector(UsageFault_IRQn, (uint32_t)platform_HardFaultHandler);
}

#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

void platform_CPUclock_setup(void)
{

}

void platform_HardFaultHandler_register(void)
{

}

#elif (PLATFORM_USED == PLATFORM_template)

#warning Support new platform:

void platform_bsp_init(void)
{
    /* TODO */
}

void platform_bsp_deinit(void)
{
    /* TODO */
}
void platform_CPUclock_setup(void)
{
    /* TODO */
}

void platform_CPUclock_deinit(void)
{
    /* TODO */
}

void platform_HardFaultHandler_register(void)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    /* TODO: register Hard Fault handler */
}

#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error platform.c: Undefined or unsupported platform
#endif /* PLATFORM_USED */




/*
----------------------------------------------------------------------------
PUBLIC API FUNCTIONS.
----------------------------------------------------------------------------
*/

void platform_init(void)
{
    platform_CPUclock_setup();
    proc_init();
    
    platform_HardFaultHandler_register();
}

void platform_deinit(void)
{
}

uint8_t platform_platformUsed(void)
{
    return PLATFORM_USED;
}



#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)
static uint32_t platform_osc_pllout_frequency = PLATFORM_OSC_PLLOUT_FREQUENCY;

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return platform_osc_pllout_frequency;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* TODO if needed */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)
static uint32_t platform_osc_hfclk_frequency = PLATFORM_OSC_HFCLK_FREQUENCY;

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return platform_osc_hfclk_frequency;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* TODO if needed */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

#include "../STM8_STM8S207/STM8S/stm8s_clk.h"

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return (CLK_GetClockFreq());
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* TODO if needed */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return (PLATFORM_OSC_HCLK_FREQUENCY/(1ul << (APBC2_PSR_Val & 0x03u)));
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* Timer clock source frequency is the same as UART (APB2): */
    return (PLATFORM_OSC_HCLK_FREQUENCY/(1ul << (APBC2_PSR_Val & 0x03u)));
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)


uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return (PLATFORM_OSC_HCLK_FREQUENCY/(1ul << (APBC2_PSR_Val & 0x03u)));
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    return (PLATFORM_OSC_HCLK_FREQUENCY/(1ul << (APBC2_PSR_Val & 0x03u)));
}

#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

#include "fsl_clock.h"


uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return CLOCK_GetFlexCommClkFreq(uart_number);
}

#define CTIMER_CLK_FREQ CLOCK_GetFreq(kCLOCK_AsyncApbClk)

uint32_t platform_timer_clock_source_frequency_get(void)
{
    return CTIMER_CLK_FREQ;
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)
static uint32_t platform_osc_hfclk_frequency = PLATFORM_OSC_HFCLK_FREQUENCY;

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    /* UART clock enumeration (uart_number parameter) must be the same as 
       CY_SYSCLK_DIV_16_5_BIT enumeration (second parameter of the Cy_SysClk_PeriphSetFracDivider() function). */
    return platform_osc_hfclk_frequency;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* 
    Not needed for this platform.
    1 ms timer is implemented in Modus Toolbox. 
    Timer initialization is not needed in classical way.
    */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    /*
     * The UART clock frequency is needed only for baudrate change.
     * It can be retrieved by the HAL_RCC_GetPCLK1Freq() function
     * (HAL_UART_Init() -> HAL_UART_MspInit() -> UART_SetConfig() -> UART_GETCLOCKSOURCE() -> HAL_RCC_GetPCLK1Freq()...)
     * but because the baudrate is set with HAL functions the UART clock frequency is not needed by this way.
     */
    return 0;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /*
     * Not needed for this platform.
     * 1 ms timer is implemented in STM32 Cube IDE (.ioc file).
     * Timer initialization is not needed in classical way.
     */
    return 0;
}
#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    /*
     * The UART clock frequency is needed only for baudrate change.
     * It can be retrieved by the HAL_RCC_GetPCLK1Freq() function
     * (HAL_UART_Init() -> HAL_UART_MspInit() -> UART_SetConfig() -> UART_GETCLOCKSOURCE() -> HAL_RCC_GetPCLK1Freq()...)
     * but because the baudrate is set with HAL functions the UART clock frequency is not needed by this way.
     */
    return 0;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /*
     * Not needed for this platform.
     * 1 ms timer is implemented in STM32 Cube IDE (.ioc file).
     * Timer initialization is not needed in classical way.
     */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    return 0;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    return 0;
}


#elif (PLATFORM_USED == PLATFORM_template)

#warning Support new platform:

uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number)
{
    /* TODO */
    return 0;
}

uint32_t platform_timer_clock_source_frequency_get(void)
{
    /* TODO if needed */
    return 0;
}

#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error platform.c: Undefined or unsupported platform
#endif /* PLATFORM_USED */


#if ((PLATFORM_USED == PLATFORM_CYPRESS_PSOC4) && (EXECUTING_FROM_RAM == 1))

#include "../Common/Platform/flash.h"

void platform_imo_clock_set(uint32_t frequency)
{
    if (frequency == 48000000)
    {
        flash_CySysFlashClockConfig();                  /* IMO clock from 24 MHz to 48 MHz                            */
        flash_stop_switch_imo();                        /* Flash module will not call flash_CySysFlashClockConfig()   */
        CyDelayFreq(frequency);                         /* Correct constants that delay ms and us function will work  */
        Clock_2M4_SetFractionalDividerRegister(19, 0);  /* Correct 1ms timer prescaler that the timer will work       */
        platform_osc_hfclk_frequency = frequency;       /* Update HFCLK frequency which is clock source for UARTs     */
    }
}

/* Copy of Clock_2M4_SetFractionalDividerRegister() function - execution from RAM: */
void platform_Clock_2M4_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional) PROC_RAM_FUNCTION;
void platform_Clock_2M4_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional)
{
    uint32 maskVal;
    uint32 regVal;
    
    #if defined (Clock_2M4__FRAC_MASK) || defined (CYREG_PERI_DIV_CMD)
    
    /* get all but divider bits */
    maskVal = Clock_2M4_DIV_REG & 
                    (uint32)(~(uint32)(Clock_2M4_DIV_INT_MASK | Clock_2M4_DIV_FRAC_MASK)); 
    /* combine mask and new divider vals into 32-bit value */
    regVal = maskVal |
        ((uint32)((uint32)clkDivider <<  Clock_2M4_DIV_INT_SHIFT) & Clock_2M4_DIV_INT_MASK) |
        ((uint32)((uint32)clkFractional << Clock_2M4_DIV_FRAC_SHIFT) & Clock_2M4_DIV_FRAC_MASK);
    
    #else
    /* get all but integer divider bits */
    maskVal = Clock_2M4_DIV_REG & (uint32)(~(uint32)Clock_2M4__DIVIDER_MASK);
    /* combine mask and new divider val into 32-bit value */
    regVal = clkDivider | maskVal;
    
    #endif /* Clock_2M4__FRAC_MASK || CYREG_PERI_DIV_CMD */

    Clock_2M4_DIV_REG = regVal;
}

void platform_hclk_clock_set(uint32_t frequency) PROC_RAM_FUNCTION;
void platform_hclk_clock_set(uint32_t frequency)
{
    uint16_t clkDivider;
    uint8 clkFractional;
    
    clkDivider = frequency / 2400000;       /* 1ms timer clock source frequency should be 2.4 MHz */
    clkFractional = 32 * (frequency % 2400000) / 2400000;
    
    platform_Clock_2M4_SetFractionalDividerRegister(clkDivider - 1, clkFractional);
}

#endif

/* [] END OF FILE */
