//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        proc.h
// CREATION DATE:      2019/09/25
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Processor management Hardware Abstraction Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PROC_HAL_H
#define PROC_HAL_H

#include "cfg/project_cfg.h"
#include "cfg/platform_cfg.h"
#include "types.h"

typedef enum proc_reset_cause_t
{
    PROC_RESET_CAUSE_NONE_NOT_CLEAR_FLAGS   = 0x00,     /* None, used for not clearing the Reset cause flag     */
    PROC_RESET_CAUSE_UNKNOWN                = 0x01,     /* Power up reset                                       */
    PROC_RESET_CAUSE_SOFTWARERESET          = 0x02,     /* Software reset                                       */
    PROC_RESET_CAUSE_WATCHDOGTIMEOUT        = 0x04      /* Watchdog timeout reset                               */
} PROC_RESET_CAUSE;

#ifndef EXECUTING_FROM_RAM
#define EXECUTING_FROM_RAM          0
#endif

#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)
#include <project.h>

/* NVIC vector table */
#define PROC_INT_VECT_TABLE         ((ISRaddress **) CYREG_NVIC_VECT_OFFSET)
#define PROC_INT_VECT_TABLE_WRITE   (*(uint32_t *) CYREG_NVIC_VECT_OFFSET)

/* Linker file: Section ".ram" is in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION           __attribute__ ((section(".ram")))
#define PROC_RAM_FUNCTION_INFRONT
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)
#include <project.h>
/* NVIC vector table */
#define PROC_INT_VECT_TABLE         ((ISRaddress **) CYREG_CM0P_VTOR)
#define PROC_INT_VECT_TABLE_WRITE   (*(uint32_t *) CYREG_CM0P_VTOR)

/* Linker file: Section ".ram" is in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION           __attribute__ ((section(".ram")))
#define PROC_RAM_FUNCTION_INFRONT
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT

#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

#include "../STM8S/stm8s_clk.h"
#include "SRC/reset.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE         ((ISRaddress **) 0xe0008000)
#define PROC_INT_VECT_TABLE_WRITE   (*(uint32_t *) 0xe0008000)

#warning Check proper executing from RAM:
/* Linker file: Section "FLASH_CODE" is in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION_START     section (FLASH_CODE)
#define PROC_RAM_FUNCTION_END       section ()
#define PROC_RAM_FUNCTION_INFRONT
#else
#define PROC_RAM_FUNCTION_START
#define PROC_RAM_FUNCTION_END
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS_START     section (FLASH_CODE)
#define PROC_RAM_FUNCTION_ALWAYS_END       section ()
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

#include "mcu.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress**) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: Define section in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __ramfunc
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __ramfunc


#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)

#include "mcu.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress**) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: Define section in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __ramfunc
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __ramfunc


#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

#include "fsl_device_registers.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress **) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: Define section in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __ramfunc
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __ramfunc


#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress **) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: ".ram_functions" section in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION                       __attribute__ ((section(".ram_functions")))
#define PROC_RAM_FUNCTION_INFRONT
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS                __attribute__ ((section(".ram_functions")))
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT


#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

#include "stm32g0xx.h"
#include "stm32g0xx_hal.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress **) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: Define section in RAM: (__RAM_FUNC  -->  __attribute__((section(".RamFunc"))))  -->  ".RamFunc" in linker file. */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __RAM_FUNC
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __RAM_FUNC

#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

#include "stm32f469xx.h"
#include "stm32f4xx_hal.h"

/* NVIC vector table */
#define PROC_INT_VECT_TABLE                     ((ISRaddress **) (SCB->VTOR))
#define PROC_INT_VECT_TABLE_WRITE               (SCB->VTOR)

/* Linker file: Define section in RAM: (__RAM_FUNC  -->  __attribute__((section(".RamFunc"))))  -->  ".RamFunc" in linker file. */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __RAM_FUNC
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __RAM_FUNC

#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

/* Linker file: Define section in RAM: */
#if (EXECUTING_FROM_RAM == 1)
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT               __attribute__ ((long_call, section (".RamFunc")))
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#define PROC_RAM_FUNCTION_ALWAYS
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        __attribute__ ((long_call, section (".RamFunc")))

#elif (PLATFORM_USED == PLATFORM_template)

#warning Proc: Implement new platform:

/* NVIC vector table */
#warning Define PROC_INT_VECT_TABLE and PROC_INT_VECT_TABLE_WRITE:
#define PROC_INT_VECT_TABLE                     ((ISRaddress **) 0xe000ed08u)  /* TODO: define for other platform(s). This is just an example. */
#define PROC_INT_VECT_TABLE_WRITE               (*(uint32_t*)    0xe000ed08u)  /* TODO: define for other platform(s). This is just an example. */

/* Linker file: Define section in RAM, for example ".ram_functions": */
#if (EXECUTING_FROM_RAM == 1)
#warning TODO: define RAM executing directive:
#define PROC_RAM_FUNCTION                       /* TODO define, for example ".ram_functions" */
#warning TODO: define RAM executing directive:
#define PROC_RAM_FUNCTION_INFRONT               /* TODO define, for example ".ram_functions" */
#else
#define PROC_RAM_FUNCTION
#define PROC_RAM_FUNCTION_INFRONT
#endif
/* Executing from RAM regardless of setting in EXECUTING_FROM_RAM */
#warning TODO: define RAM executing directive:
#define PROC_RAM_FUNCTION_ALWAYS                /* TODO define, for example ".ram_functions" */
#warning TODO: define RAM executing directive:
#define PROC_RAM_FUNCTION_ALWAYS_INFRONT        /* TODO define, for example ".ram_functions" */


#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
#error proc.h: Undefined or unsupported platform
#endif /* PLATFORM_USED */



// DESCRIPTION:
//   Initialization of MCU dependent hardware.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_init(void);


// DESCRIPTION:
//   Deinitialization of MCU dependent hardware.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_deinit(void);


// DESCRIPTION:
//   Enables global interrupt.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_GlobalIntEnable(void);


// DESCRIPTION:
//   Disables global interrupt.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_GlobalIntDisable(void);


/*******************************************************************************************************************//**
* @brief    Disables all MCU's interrupts. 
*           If MCU has some other interrupts separated from NVIC interrupts (for example SysTick interrupt), 
*           disable also these interrupts.
*           This function is typically used together with disabling global interrupt
*           (proc_GlobalIntDisable() function) before jumping to Application or Bootloader.
* 
* @details  Example:
* @code
*    proc_GlobalIntDisable();
*    proc_AllInterruptsDisable();
*    proc_jumpToStartupAddress(*(uint32_t*)BL_APPLICATION_INITIAL_STACK_POINTER, 
                               *(uint32_t*)BL_APPLICATION_ENTRY_POINT);
* @endcode
*
***********************************************************************************************************************/
void proc_AllInterruptsDisable(void);


/*******************************************************************************************************************//**
* @brief    Disables global interrupt and returns a value indicating whether global interrupt was enabled before calling this function.
* 
* @return   Value indicating whether global interrupt was enabled before calling this function (typically PRIMASK register):
*           0 - Global interrupt was enabled before calling this function.
*           1 - Global interrupt was not enabled before calling this function.
* 
* @details  Example:
* @code
*    uint8_t enableInterrupts;
*    enableInterrupts = proc_EnterCriticalSection();
*    ...
*    proc_ExitCriticalSection(enableInterrupts);
* @endcode
*
***********************************************************************************************************************/
uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;


/*******************************************************************************************************************//**
* @brief    Re-enables global interrupt if they were enabled before proc_EnterCriticalSection was called.
*           The argument should be the value returned from proc_EnterCriticalSection.
* 
* @param    Value returned from proc_EnterCriticalSection.
* 
* @details  Example:
* @code
*    uint8_t enableInterrupts;
*    enableInterrupts = proc_EnterCriticalSection();
*    ...
*    proc_ExitCriticalSection(enableInterrupts);
* @endcode
*
***********************************************************************************************************************/
void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;


/*******************************************************************************************************************//**
* @brief    Disables interrupts whose Interrupt Service Rutines are not executed from RAM: all other interrupts except UART0 Rx interrupt and 1ms timer interrupt.
* 
* @details  Example:
* @code
*    proc_disableNonRAMinterrupts();
     ... code ...
*    proc_enableNonRAMinterrupts();
*    enableInterrupts = proc_EnterCriticalSection();
* @endcode
*
***********************************************************************************************************************/
void proc_disableNonRAMinterrupts(void);


/*******************************************************************************************************************//**
* @brief    Re-enable interrupts whose Interrupt Service Rutines are not executed from RAM: all other interrupts except UART0 Rx interrupt and 1ms timer interrupt.
* 
* @details  Example:
* @code
*    proc_disableNonRAMinterrupts();
     ... code ...
*    proc_enableNonRAMinterrupts();
*    enableInterrupts = proc_EnterCriticalSection();
* @endcode
*
***********************************************************************************************************************/
void proc_enableNonRAMinterrupts(void);


// DESCRIPTION:
//   Halts the CPU.
// INPUTS:
//   reason     - Reason value to be used during debugging.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_halt(uint8_t reason);


// DESCRIPTION:
//   Forces a device software reset.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_softwareReset(void);


// DESCRIPTION:
//   Blocks for milliseconds.
// INPUTS:
//   milliseconds   - milliseconds to block.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_delayms(uint32_t milliseconds);


// DESCRIPTION:
//   Blocks for microseconds.
// INPUTS:
//   microseconds   - microseconds to block.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_delayus(uint32_t microseconds);


// DESCRIPTION:
//   Initializes and starts watchdog timer.
//   Set Watchdog timeout to value between 50 and 300 ms (on some MCUs the value cannot be set to exact value).
//   From now on the watchdog should be cleared on at least this timeout using the proc_clearWDT() function.
//   If Window Watchdog is used, window functionality should be disabled (set to 0) !!!
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void proc_startWDT(void);


// DESCRIPTION:
//   Clears (resets) watchdog timer.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT void proc_clearWDT(void) PROC_RAM_FUNCTION_ALWAYS;


// DESCRIPTION:
//   Get IPSR (Interrupt Program Status Register) Register - IRQ number of current process running.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   Returns the content of the IPSR Register.
// ----------------------------------------------------------------------------
uint32_t proc_get_IPSR(void);


// DESCRIPTION:
//   Get reset cause (Software reset, watchdog timeout, ...). 
// INPUTS:
//   reset_cause_clear - Which Reset cause flag will be cleared. If Reset cause flag is cleared, 
//                       another call of this function would not return the same Reset cause again.
// OUTPUT:
//   --
// RETURNS:
//   Reset cause: software reset, watchdog timeout reset or none (power-on-reset).
// ----------------------------------------------------------------------------
PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear);


/*******************************************************************************************************************//**
* @brief    Set stack pointer to specified value (usually reset to the end of RAM address) 
*           and continue executing code from specified startup address.
*           Code on startupAddress should never return and therefore also this function should never return.
* 
* @param    stackPointerValue       Value the Stack Pointer is set prior jumping starting execuding from address.
* @param    startupAddress          Address for the code the executing continues from.
*
* @details  Example:
* @code
*    #define APP_BANK_APPLICATION_START                      0x00000000
*    #define BL_BANK_APPLICATION_START                       APP_BANK_APPLICATION_START
*    #define BL_APPLICATION_ENTRY_POINT                      (BL_BANK_APPLICATION_START + 4)
*    #define BL_APPLICATION_INITIAL_STACK_POINTER            (BL_BANK_APPLICATION_START + 0)
*    proc_GlobalIntDisable();
*    proc_AllInterruptsDisable();
*    *PROC_INT_VECT_TABLE = (ISRaddress*)(BL_APPLICATION_INITIAL_STACK_POINTER);
*    proc_clearWDT();
*    proc_jumpToStartupAddress(*(uint32_t*)BL_APPLICATION_INITIAL_STACK_POINTER, 
*                              *(uint32_t*)BL_APPLICATION_ENTRY_POINT);
*    while (1)
*    {
*        // If main returns, make sure we don't return.
*    };
* @endcode
*
***********************************************************************************************************************/
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress);


#endif /* PROC_HAL_H */
/* [] END OF FILE */
