//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        proc.c
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

#include "cfg/platform_cfg.h"
#include "cfg/project_cfg.h"
#include "Platform/proc.h"

#ifndef RELOCATE_VECTORS_TO_RAM
#define RELOCATE_VECTORS_TO_RAM          1
#endif


#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)
#include <project.h>

void proc_init(void)
{
    /* Nothing to do. Cypress PSoC Creator's function initialize_psoc() ---> cyfitter_cfg() does the inicialization */
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    CyGlobalIntEnable;
}

void proc_GlobalIntDisable(void)
{
    CyGlobalIntDisable;
}

void proc_AllInterruptsDisable(void)
{
    /* Disables the SysTick interrupt: */
    CySysTickDisableInterrupt();

    /* Disable all interrupts: */
    *(reg32*) CYREG_NVIC_CLRENA0 = 0xFFFFFFFF;

    /* Clear pending register of all interrupts: */
    *(reg32*) CYREG_NVIC_CLRPEND0 = 0xFFFFFFFF;
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
uint8_t proc_EnterCriticalSection(void)
{
    /* Copy of CyEnterCriticalSection() function from CyBootAsmGnu.s to be executed from RAM: */
    __asm volatile("    MRS r0, PRIMASK\n");        /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                /* Disable interrupts */
    __asm volatile("    BX lr\n");
    /* warning return value: see __attribute__((always_inline)) __STATIC_INLINE uint32_t __get_IPSR(void) */
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    /* Copy of CyExitCriticalSection() function from CyBootAsmGnu.s to be executed from RAM: */
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts = 0xFFFFFFFF;
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    enabledInterrupts = *(reg32*) CYREG_NVIC_SETENA0;
    *(reg32*) CYREG_NVIC_CLRENA0 = 0xFFFFFFFF - UART_0_RXInterrupt__INTC_MASK - timer_1ms_ISR__INTC_MASK;
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    *(reg32*) CYREG_NVIC_CLRPEND0 = 0xFFFFFFFF - UART_0_RXInterrupt__INTC_MASK - timer_1ms_ISR__INTC_MASK;
    *(reg32*) CYREG_NVIC_SETENA0 = enabledInterrupts;
}

void proc_halt(uint8_t reason)
{
    CyHalt(reason);
}

void proc_softwareReset()
{
    CySoftwareReset();
}

void proc_delayms(uint32_t milliseconds)
{
    CyDelay(milliseconds);
}

void proc_delayus(uint32_t microseconds)
{
    CyDelayUs(microseconds);
}

void proc_startWDT(void)
{
    /* Set 256 - 384 ms WD timeout value: */
    CyWdtStart(CYWDT_128_TICKS, CYWDT_LPMODE_MAXINTER);
}

void proc_clearWDT(void)
{
    CyWdtClear();
}

uint32_t proc_get_IPSR(void)
{
    return __get_IPSR();
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    PROC_RESET_CAUSE cause;
    uint8_t resetStatus;

    cause = PROC_RESET_CAUSE_UNKNOWN;
    resetStatus = CyResetStatus;
    
    if (resetStatus & CY_RESET_WD)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (resetStatus & CY_RESET_SW)
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (resetStatus & CY_RESET_LVID)
    {
        /* TODO, if needed */
    }
    else if (resetStatus & CY_RESET_LVIA)
    {
        /* TODO, if needed */
    }
    else if (resetStatus & CY_RESET_HVIA)
    {
        /* TODO, if needed */
    }
    else
    {
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    /* Clear Reset cause flags, if requested: */
    if (reset_cause_clear != PROC_RESET_CAUSE_NONE_NOT_CLEAR_FLAGS)
    {
        /* Cypress CY8C5888 specific: all Reset Cause flags are cleared on reading register: */
        
        /* Reset Status Register has Read-to-clear SW access mode.
        * Preserve current RESET_SR0 state to make it available for next reading.
        */
        *(reg32 *)(CYREG_PHUB_CFGMEM23_CFG1) = *(reg32 *)(CYREG_RESET_SR0);
        *(reg32 *)(CYREG_PHUB_CFGMEM23_CFG1) = 0;
        CyResetStatus = CY_GET_REG8(CYREG_PHUB_CFGMEM23_CFG1);
    }

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)

#include "../Common/Platform/flash.h"

extern cyisraddress CyRamVectors[CYINT_IRQ_BASE + CY_NUM_INTERRUPTS];

void proc_init(void)
{
    /* Nothing to do. Cypress PSoC Creator's function initialize_psoc() ---> cyfitter_cfg() does the inicialization */
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    /* Override VTOR settings fixed set to 0x20000000 instead of address of RAM vectors: */
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&CyRamVectors;
    
    CyGlobalIntEnable;
}

void proc_GlobalIntDisable(void)
{
    CyGlobalIntDisable;
}

void proc_AllInterruptsDisable(void)
{
    /* Disables the SysTick interrupt: */
    CySysTickDisableInterrupt();

    /* Disable all interrupts: */
    *(reg32*) CYREG_CM0P_ICER = 0xFFFFFFFF;

    /* Clear pending register of all interrupts: */
    *(reg32*) CYREG_CM0P_ICPR = 0xFFFFFFFF;
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
uint8_t proc_EnterCriticalSection(void)
{
    /* Copy of CyEnterCriticalSection() function from CyBootAsmGnu.s to be executed from RAM: */
    __asm volatile("    MRS r0, PRIMASK\n");        /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                /* Disable interrupts */
    __asm volatile("    BX lr\n");
    /* warning return value: see __attribute__((always_inline)) __STATIC_INLINE uint32_t __get_IPSR(void) */
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    /* Copy of CyExitCriticalSection() function from CyBootAsmGnu.s to be executed from RAM: */
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts = 0xFFFFFFFF;
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART0 RX, 1ms timer interrupt and SPC, used for flash writing: */
    enabledInterrupts = *(reg32*) CYREG_CM0P_ISER;
    *(reg32*) CYREG_CM0P_ICER = 0xFFFFFFFF - UART_0_RXInterrupt__INTC_MASK 
                                           - timer_1ms_ISR__INTC_MASK
                                           - SPC_ISR__INTC_MASK;
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    *(reg32*) CYREG_CM0P_ICPR = 0xFFFFFFFF - UART_0_RXInterrupt__INTC_MASK 
                                           - timer_1ms_ISR__INTC_MASK
                                           - SPC_ISR__INTC_MASK;
    *(reg32*) CYREG_CM0P_ISER = enabledInterrupts;
}

void proc_halt(uint8_t reason)
{
    CyHalt(reason);
}

void proc_softwareReset()
{
    CySoftwareReset();
}

void proc_delayms(uint32_t milliseconds)
{
    CyDelay(milliseconds);
}

void proc_delayus(uint32_t microseconds)
{
    CyDelayUs(microseconds);
}

void proc_startWDT(void)
{
    #if (WATCHDOG_USE == 1)
    CySysWdtEnable();
    CySysWdtSetIgnoreBits(5);   /* Maximum WD reset timeout 153.6ms typically (40 kHz clock is +50/+100% !!!) */
    #endif /* WATCHDOG_USE */
}

void proc_clearWDT(void) PROC_RAM_FUNCTION;
void proc_clearWDT(void)
{
    #if (WATCHDOG_USE == 1)
    /* Substutition for CySysWdtClearInterrupt() - execution from RAM: */
    CY_SYS_SRSS_INTR_REG |= CY_SYS_WDT_LOWER_BIT_MASK;
    #endif /* WATCHDOG_USE */
}

uint32_t proc_get_IPSR(void)
{
    return __get_IPSR();
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    PROC_RESET_CAUSE cause;
    uint32_t reasonIn, reasonOut;

    reasonIn = CY_SYS_RESET_PROTFAULT;
    if (reset_cause_clear & PROC_RESET_CAUSE_SOFTWARERESET)
    {
        reasonIn |= CY_SYS_RESET_SW;
    }
    if (reset_cause_clear & PROC_RESET_CAUSE_WATCHDOGTIMEOUT)
    {
        reasonIn |= CY_SYS_RESET_WDT;
    }
    reasonOut = CySysGetResetReason(reasonIn);

    cause = PROC_RESET_CAUSE_UNKNOWN;
    if (reasonOut & CY_SYS_RESET_WDT)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (reasonOut & CY_SYS_RESET_SW)
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (reasonOut & CY_SYS_RESET_PROTFAULT)
    {
        /* TODO, if needed */
    }
    else
    {
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

#include "../STM8_STM8S207/STM8S/stm8s_clk.h"
#include "../STM8_STM8S207/STM8S/stm8s_tim4.h"

#define CPU_SOFT_INT_DISABLED               ((uint8_t)0x28) /* Mask for I1 and I0 bits in CPU_CC register */
#define IS_ITC_INTERRUPTS_DISABLED          (ITC_GetSoftIntStatus() == CPU_SOFT_INT_DISABLED)
uint8_t ITC_GetCPUCC(void);
uint8_t ITC_GetSoftIntStatus(void);

uint8_t ITC_GetCPUCC(void)
{
  _asm("push cc");
  _asm("pop a");
  return; /* Ignore compiler warning, the returned value is in A register */
}

uint8_t ITC_GetSoftIntStatus(void)
{
  return (uint8_t)(ITC_GetCPUCC() & CPU_CC_I1I0);
}

void proc_init(void)
{
    /* DEINIT before INIT */
    CLK->ICKR = CLK_ICKR_RESET_VALUE;
    CLK->ECKR = CLK_ECKR_RESET_VALUE;
    CLK->SWR  = CLK_SWR_RESET_VALUE;
    CLK->SWCR = CLK_SWCR_RESET_VALUE;
    CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
    CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
    CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
    CLK->CSSR = CLK_CSSR_RESET_VALUE;
    CLK->CCOR = CLK_CCOR_RESET_VALUE;
    while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0)
    {}
    CLK->CCOR = CLK_CCOR_RESET_VALUE;
    CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
    CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;

    /* INIT */
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
    /* 128 because bit7 = 1 means CPU divider */
    CLK->CKDIVR |= (uint8_t)((uint8_t)128 & (uint8_t)CLK_CKDIVR_CPUDIV);

    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
    /* Bit7 = 0 means HSI divider */
    CLK->CKDIVR |= (uint8_t)((uint8_t)0 & (uint8_t)CLK_CKDIVR_HSIDIV);

    /* Clears of the CCO type bits part */
    CLK->CCOR &= (uint8_t)(~CLK_CCOR_CCOSEL);
    
    /* Selects the source provided on cco_ck output */
    /* 0x08 = Clock Output CPU */
    CLK->CCOR |= (uint8_t)0x08;
    
    /* Enable the clock output */
    CLK->CCOR |= CLK_CCOR_CCOEN;

    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    enableInterrupts();
}

void proc_GlobalIntDisable(void)
{
    disableInterrupts();
}

void proc_AllInterruptsDisable(void)
{
    #warning TODO: Implement proc_AllInterruptsDisable() function (disabling all interrupts):
    /* Disable all interrupts: */
    /* TODO */

    /* Clear pending register of all interrupts: */
    /* TODO */
}

#pragma PROC_RAM_FUNCTION_START
uint8_t proc_EnterCriticalSection(void)
{
    /* Returns 1 if interrupts were previously disabled and 0 if they were enabled */
    /* Return values correspond to those from the "uint8_t CyEnterCriticalSection(void)" function from the Cypress API */
    if (IS_ITC_INTERRUPTS_DISABLED)
    {
        #warning Execute sim() function from RAM
        sim();
        return 1;
    }
    #warning Execute sim() function from RAM
    sim();
    return 0;
}
#pragma PROC_RAM_FUNCTION_END

#pragma PROC_RAM_FUNCTION_START
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    /* 1 if interrupts were previously disabled and 0 if they were enabled */
    /* Values correspond to those from the "uint8_t CyEnterCriticalSection(void)" function from the Cypress API */
    if (enableInterrupts == 0)
    {
        #warning Execute rim() function from RAM
        rim();
    }
    /* else: do nothing, interrupts have to stay disabled */
}
#pragma PROC_RAM_FUNCTION_END

void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    #warning TODO implement proc_disableNonRAMinterrupts() function: Disables interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    #warning TODO implement proc_enableNonRAMinterrupts() function: Re-enable interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_halt(uint8_t reason)
{
    halt();
}

void proc_softwareReset(void)
{
    /* Reset SW by clearing T6 */
    WWDG->CR = WWDG_CR_T6;
}

void proc_delayms(uint32_t milliseconds)
{
    DelayMilliseconds(milliseconds);
}

void proc_delayus(uint32_t microseconds)
{
    DelayMicroseconds(microseconds);
}

void proc_startWDT(void)
{
    #warning proc_startWDT(): set watchdog timeout to value between 50 and 300 ms.

    /* WWDG is enabled by setting the WDGA bit in the WWDG_CR register. 
       It cannot be disabled again except by a reset. */
    /* Control register */
    WWDG->CR = WWDG_CR_WDGA;
    WWDG->CR = (uint8_t)0x7F;
    /* Window register */
    WWDG->WR = (uint8_t)0x60;
}

void proc_clearWDT(void)
{
    /* Write to T[6:0] bits to configure the counter value, no need to do
    a read-modify-write; writing a 0 to WDGA bit does nothing */
    WWDG->CR = (uint8_t)0x7F;
}

uint32_t proc_get_IPSR(void)
{
    /* Implement for other platform(s) */
    #warning Return interrupt number of current process.
    return 0;
}

/* STM8 has 9 possible reset sources:
    - External reset through the NRST pin
    - Power-on reset (POR)
    - Brown-out Reset (BOR)
    - Independent watchdog reset (IWDG)
    - Window watchdog reset (WWDG)
    - Software reset
    - SWIM reset
    - Illegal opcode reset
    - EMC reset: generated if critical registers are corrupted or badly loaded 
   POR and BOR have no reset flag.
*/
PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    PROC_RESET_CAUSE cause;
    RESET_FLAG resetFlag = RESET_FLAG_ILLOPF; /* undocumented instruction reset code */

    resetFlag = RESET_GetResetFlag();
    cause = PROC_RESET_CAUSE_UNKNOWN;

    if (resetFlag == RESET_FLAG_WWDGF)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (resetFlag == RESET_FLAG_IWDGF)
    {
        /* TODO, if needed */
    }
    else if (resetFlag == RESET_FLAG_SWIMF)
    {
        /* TODO, if needed */
    }
    else if (resetFlag == RESET_FLAG_EMCF)
    {
        /* TODO, if needed */
    }
    else if (resetFlag == RESET_FLAG_ILLOPF)
    {
        /* TODO, if needed */
    }
    else
    {
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    #warning proc_reset_cause() TODO on ST STM8S207: Clear Reset cause flag(s) regarding the "reset_cause_clear" parameter!

    return cause;
}

void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    /* 
    LDW:  Load the destination word (16-bit value) with the source word. The dst and
            src can be a 16-bit register (X, Y or SP) or a memory/data 16-bit value.
    LD:   Load the destination byte with the source byte. The dst and src can be a
          register, a byte (low/high) of an index register or a memory/data byte. When
          half of an index register is loaded, the other half remains unchanged.
    JPF:  The unconditional jump simply replaces the content of the PC by a
          destination with an extended address. Control then passes to the
          statement addressed by the program counter. For safe memory usage, this
          instruction must be used, when the operation crosses a memory section.
    XREF: To access value of variable in assembly. The compiler declares any global 
          variable in C with an underscore (_) prefix.
    */
    _asm("LDW   X, SP\n");
    _asm("LD    A, $FF\n");
    _asm("LD    XL, A\n");
    _asm("LDW   SP, X\n");
    _asm("jpf   c_lreg\n", userStartup);

    enableInterrupts();
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

#include "mcu.h"

volatile static ISRaddress *romVectors;
__attribute__ ((aligned(256)))
volatile static ISRaddress ramVectors[16+48];

void proc_init(void)
{
    uint16_t i;
    uint16_t numberOfIRQs;
    uint8_t enabledInterrupts;

    if ((((uint32_t) &ramVectors[0]) % 256) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 256 bytes !!! */
        proc_halt(0);
    }
    
    romVectors = (ISRaddress *) PROC_INT_VECT_TABLE;
    numberOfIRQs = sizeof(ramVectors) / sizeof(ISRaddress);
    
    enabledInterrupts = proc_EnterCriticalSection();
    for (i = 0; i < numberOfIRQs; i++)
    {
        ramVectors[i] = *(romVectors + i);
    }
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&ramVectors;
    proc_ExitCriticalSection(enabledInterrupts);
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    __asm("CPSIE   i");
}

void proc_GlobalIntDisable(void)
{
    __asm("CPSID   i");
}

#define NUMBER_OF_IRQS          256
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;             /* Clear pending register       */
    }
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts[NUMBER_OF_IRQS / 32];
void proc_disableNonRAMinterrupts(void)
{
    uint8_t i;

    /* Save enabled interrupts and disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        enabledInterrupts[i] = NVIC->ISER[i];   /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Enable timer interrupt: */
    i = FRTIM_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)FRTIM_IRQn) % 32 ));
    __COMPILER_BARRIER();
    
    /* Enable WiFi UART Rx interrupt: */
    i = MFS0RX_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)MFS0RX_IRQn) % 32 ));
    __COMPILER_BARRIER();
}

void proc_enableNonRAMinterrupts(void)
{
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        
    }
}

void proc_softwareReset()
{
    SCB->AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
}

void proc_delayms(uint32_t milliseconds)
{
    uint32_t i;

    for (i = 0; i < 100; i++)
    {
        proc_delayus(milliseconds * 10);
    }
}

void proc_delayus(uint32_t microseconds)
{
    uint32_t imax;
    
    if (microseconds < 10000)
    {
        #if DEBUG
        imax = microseconds*855/129;
        #else
        imax = microseconds*1040/129;
        #endif
    }
    else
    {
        #if DEBUG
        imax = microseconds/129*855;
        #else
        imax = microseconds/129*1040;
        #endif
    }
    
    while (imax--)
    {
        __asm("NOP");
    }
}

void proc_startWDT(void)
{
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;    /* HW Watchdog Unlock */;
    /* Do not set Watchdog timeout value lower than 1.1 seconds (Sector erase time, Small Sector takes max 1.1s) ! */
    FM3_HWWDT->WDG_LDR = 110000;         /* Interval:1.1s (@CLKLC:100kHz) */

    /* HW Watchdog Control Register unlock sequence */
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;
    FM3_HWWDT->WDG_LCK = 0xE5331AAEu;
    /* Setup HW-Watchdog and start */
    FM3_HWWDT->WDG_CTL_f.RESEN = 1;

    /* HW Watchdog Control Register unlock sequence */
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;
    FM3_HWWDT->WDG_LCK = 0xE5331AAEu;
    /* Enable interrupt and count */
    FM3_HWWDT->WDG_CTL_f.INTEN = 1;
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;   /* Release Lock */
    FM3_HWWDT->WDG_ICL = 0xAA;          /* Clear possible interrupt and reload value, step 1 */
    FM3_HWWDT->WDG_ICL = 0x55;          /* Clear possible interrupt and reload value, step 2 */
}

uint32_t proc_get_IPSR(void)
{
    uint32_t result;

    __asm volatile ("MRS %0, IPSR" : "=r" (result) );
    return(result);
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    stc_crg_rst_str_field_t resetCause;
    PROC_RESET_CAUSE cause;
    
    resetCause = FM3_CRG->RST_STR_f;
    cause = PROC_RESET_CAUSE_UNKNOWN;
    
    if (resetCause.HWDT || resetCause.SWDT)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (resetCause.SRST &&  ~resetCause.PONR)
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (resetCause.PONR)
    {
        /* Power-on-reset: */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    /* All Reset cause flags are cleared regardless the "reset_cause_clear" parameter!
       See RST_STR: ReSeT STatus Register: "Reading the register clears all bits".
    */

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)

#include "mcu.h"

volatile static ISRaddress *romVectors;
__attribute__ ((aligned(256)))
volatile static ISRaddress ramVectors[16+48];

void proc_init(void)
{
    uint16_t i;
    uint16_t numberOfIRQs;
    uint8_t enabledInterrupts;

    if ((((uint32_t) &ramVectors[0]) % 256) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 256 bytes !!! */
        proc_halt(0);
    }
    
    romVectors = (ISRaddress *) PROC_INT_VECT_TABLE;
    numberOfIRQs = sizeof(ramVectors) / sizeof(ISRaddress);
    
    enabledInterrupts = proc_EnterCriticalSection();
    for (i = 0; i < numberOfIRQs; i++)
    {
        ramVectors[i] = *(romVectors + i);
    }
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&ramVectors;
    proc_ExitCriticalSection(enabledInterrupts);
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    __asm("CPSIE   i");
}

void proc_GlobalIntDisable(void)
{
    __asm("CPSID   i");
}

#define NUMBER_OF_IRQS          256
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;             /* Clear pending register       */
    }
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts[NUMBER_OF_IRQS / 32];
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    uint8_t i;

    /* Save enabled interrupts and disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        enabledInterrupts[i] = NVIC->ISER[i];   /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Enable timer interrupt: */
    i = FRTIM_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)FRTIM_IRQn) % 32 ));
    __COMPILER_BARRIER();
    
    /* Enable WiFi UART Rx interrupt: */
    i = MFS0RX_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)MFS0RX_IRQn) % 32 ));
    __COMPILER_BARRIER();
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        
    }
}

void proc_softwareReset()
{
    SCB->AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
}

void proc_delayms(uint32_t milliseconds)
{
    uint32_t i;

    for (i = 0; i < 100; i++)
    {
        proc_delayus(milliseconds * 10);
    }
}

void proc_delayus(uint32_t microseconds)
{
    uint32_t imax;
    
    if (microseconds < 10000)
    {
        #if DEBUG
        imax = microseconds*941/129;
        #else
        imax = microseconds*1175/129;
        #endif
    }
    else
    {
        #if DEBUG
        imax = microseconds/129*941;
        #else
        imax = microseconds/129*1175;
        #endif
    }
    
    while (imax--)
    {
        __asm("NOP");
    }
}

void proc_startWDT(void)
{
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;    /* HW Watchdog Unlock */;
    /* Do not set Watchdog timeout value lower than 0.7 seconds (Sector erase time, Small Sector takes max 0.7s) ! */
    FM3_HWWDT->WDG_LDR = 70000;         /* Interval:0.7s (@CLKLC:100kHz) */

    /* HW Watchdog Control Register unlock sequence */
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;
    FM3_HWWDT->WDG_LCK = 0xE5331AAEu;
    /* Setup HW-Watchdog and start */
    FM3_HWWDT->WDG_CTL_f.RESEN = 1;

    /* HW Watchdog Control Register unlock sequence */
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;
    FM3_HWWDT->WDG_LCK = 0xE5331AAEu;
    /* Enable interrupt and count */
    FM3_HWWDT->WDG_CTL_f.INTEN = 1;
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    FM3_HWWDT->WDG_LCK = 0x1ACCE551u;   /* Release Lock */
    FM3_HWWDT->WDG_ICL = 0xAA;          /* Clear possible interrupt and reload value, step 1 */
    FM3_HWWDT->WDG_ICL = 0x55;          /* Clear possible interrupt and reload value, step 2 */
}

uint32_t proc_get_IPSR(void)
{
    uint32_t result;

    __asm volatile ("MRS %0, IPSR" : "=r" (result) );
    return(result);
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    stc_crg_rst_str_field_t resetCause;
    PROC_RESET_CAUSE cause;
    
    resetCause = FM3_CRG->RST_STR_f;
    cause = PROC_RESET_CAUSE_UNKNOWN;
    
    if (resetCause.HWDT || resetCause.SWDT)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (resetCause.SRST &&  ~resetCause.PONR)
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (resetCause.PONR)
    {
        /* Power-on-reset: */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    /* All Reset cause flags are cleared regardless the "reset_cause_clear" parameter!
       See RST_STR: ReSeT STatus Register: "Reading the register clears all bits".
    */

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

volatile static ISRaddress *romVectors;
__attribute__ ((aligned(256)))
volatile static ISRaddress ramVectors[16+57];

void proc_init(void)
{
    uint16_t i;
    uint16_t numberOfIRQs;
    uint8_t enabledInterrupts;

    if ((((uint32_t) &ramVectors[0]) % 256) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 256 bytes !!! */
        proc_halt(0);
    }
    
    romVectors = (ISRaddress *) PROC_INT_VECT_TABLE;
    numberOfIRQs = sizeof(ramVectors) / sizeof(ISRaddress);
    
    enabledInterrupts = proc_EnterCriticalSection();
    for (i = 0; i < numberOfIRQs; i++)
    {
        ramVectors[i] = *(romVectors + i);
    }
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&ramVectors;
    proc_ExitCriticalSection(enabledInterrupts);
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    __asm("CPSIE   i");
}

void proc_GlobalIntDisable(void)
{
    __asm("CPSID   i");
}

#define NUMBER_OF_IRQS          64
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;             /* Clear pending register       */
    }
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts[NUMBER_OF_IRQS / 32];
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    uint8_t i;

    /* Save enabled interrupts and disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        enabledInterrupts[i] = NVIC->ISER[i];   /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Enable timer interrupt: */
    i = CTIMER3_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)CTIMER3_IRQn) % 32 ));
    __COMPILER_BARRIER();
    
    /* Enable WiFi UART Rx interrupt  (and also Tx, but not needed): */
    i = FLEXCOMM8_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)FLEXCOMM8_IRQn) % 32 ));
    __COMPILER_BARRIER();
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        
    }
}

void proc_softwareReset()
{
    SCB->AIRCR = (0x5FA<<SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
}

void proc_delayms(uint32_t milliseconds)
{
    uint32_t i;

    for (i = 0; i < 100; i++)
    {
        proc_delayus(milliseconds * 10);
    }
}

void proc_delayus(uint32_t microseconds)
{
    uint32_t imax;
    
    if (microseconds < 10000)
    {
        #if DEBUG
        imax = microseconds*927/31;
        #else
        imax = microseconds*1116/31;
        #endif
    }
    else
    {
        #if DEBUG
        imax = microseconds/31*927;
        #else
        imax = microseconds/31*1116;
        #endif
    }
    
    while (imax--)
    {
        __asm("NOP");
    }
}

#include "fsl_wwdt.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_device_registers.h"
#define WDT_CLK_FREQ        CLOCK_GetFreq(kCLOCK_WdtOsc)

void proc_startWDT(void)
{
    wwdt_config_t config;
    uint32_t wdtFreq;

    POWER_DisablePD(kPDRUNCFG_PD_WDT_OSC);
    
    /* The WDT divides the input frequency into it by 4 */
    wdtFreq = WDT_CLK_FREQ / 4;

    WWDT_GetDefaultConfig(&config);

    /*
     * Set watchdog feed time constant to approximately 128 ms (1/8)
     * Set watchdog warning time to 512 ticks after feed time constant.
     * Set watchdog window time to 0 ms (disabled).
     */
    config.timeoutValue = wdtFreq / 8;          /* WDT timeout: 128 ms (1/8)            */
    config.warningValue = 512;
    config.windowValue  = 0x00FFFFFF;           /* Window functionality disabled        */
    /* Configure WWDT to reset on timeout */
    config.enableWatchdogReset = true;
    /* Setup watchdog clock frequency(Hz). */
    config.clockFreq_Hz = WDT_CLK_FREQ;
    WWDT_Init(WWDT, &config);
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    /* Instead of 
    WWDT_Refresh(((WWDT_Type *)WWDT_BASE));
    function (executing from RAM): */
    uint32_t primaskValue = 0U;

    /* Disable the global interrupt to protect refresh sequence */
    primaskValue = __get_PRIMASK();     /* Instead of DisableGlobalIRQ();   (executing from RAM) */
    __disable_irq();                    /* Instead of DisableGlobalIRQ();   (executing from RAM) */

    ((WWDT_Type *)WWDT_BASE)->FEED   = WWDT_FIRST_WORD_OF_REFRESH;
    ((WWDT_Type *)WWDT_BASE)->FEED   = WWDT_SECOND_WORD_OF_REFRESH;

    __set_PRIMASK(primaskValue);        /* Instead of EnableGlobalIRQ(primaskValue);    (executing from RAM) */
}

uint32_t proc_get_IPSR(void)
{
    uint32_t result;

    __asm volatile ("MRS %0, IPSR" : "=r" (result) );
    return(result);
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    uint32_t resetCause;
    PROC_RESET_CAUSE cause;

    resetCause = SYSCON->SYSRSTSTAT;
    cause = PROC_RESET_CAUSE_UNKNOWN;
    
    if ((resetCause & SYSCON_SYSRSTSTAT_WDT_MASK) == SYSCON_SYSRSTSTAT_WDT_MASK)
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
        SYSCON->SYSRSTSTAT |= SYSCON_SYSRSTSTAT_WDT_MASK;
    }
    else if ((resetCause & SYSCON_SYSRSTSTAT_SYSRST_MASK) == SYSCON_SYSRSTSTAT_SYSRST_MASK)
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
        SYSCON->SYSRSTSTAT |= SYSCON_SYSRSTSTAT_SYSRST_MASK;
    }
    else if ((resetCause & SYSCON_SYSRSTSTAT_POR_MASK) == SYSCON_SYSRSTSTAT_POR_MASK)
    {
        /* Power-on-reset: */
        cause = PROC_RESET_CAUSE_UNKNOWN;
        SYSCON->SYSRSTSTAT |= SYSCON_SYSRSTSTAT_POR_MASK;
    }
    else if ((resetCause & SYSCON_SYSRSTSTAT_EXTRST_MASK) == SYSCON_SYSRSTSTAT_EXTRST_MASK)
    {
        /* External reset: */
        cause = PROC_RESET_CAUSE_UNKNOWN;
        SYSCON->SYSRSTSTAT |= SYSCON_SYSRSTSTAT_EXTRST_MASK;
    }
    else if ((resetCause & SYSCON_SYSRSTSTAT_BOD_MASK) == SYSCON_SYSRSTSTAT_BOD_MASK)
    {
        /* Brown-out detect reset: */
        cause = PROC_RESET_CAUSE_UNKNOWN;
        SYSCON->SYSRSTSTAT |= SYSCON_SYSRSTSTAT_BOD_MASK;
    }

    #warning proc_reset_cause() TODO on NXP LPC546XX: Clear Reset cause flag(s) regarding the "reset_cause_clear" parameter!

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)

#include "cy_pdl.h"
#include "cybsp.h"
#include "cyhal_system_impl.h"
#include "cyhal_wdt.h"

extern cy_israddress __RAM_VECTOR_TABLE[CY_VECTOR_TABLE_SIZE];

void proc_init(void)
{
    /* Typically (if not already done in pre main() phase):
       - check if RAM vector table is 256-bytes aligned
       - copy ROM to RAM vectors
       - change NVIC to point to RAM vectors
    */

    if ((((uint32_t) &__RAM_VECTOR_TABLE[0]) % 256) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 256 bytes !!! */
        proc_halt(0);
    }
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    /* __asm("CPSIE   i");  */
    __enable_irq();     /* --> __ASM volatile ("cpsie i" : : : "memory"); */
}

void proc_GlobalIntDisable(void)
{
    /* __asm("CPSID   i"); */
    __disable_irq();    /* --> __ASM volatile ("cpsid i" : : : "memory"); */
}
#define NUMBER_OF_IRQS          (CY_VECTOR_TABLE_SIZE - 16)     /* --> 32 ... only one ICER[] and ICPR[] register. */
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disables the SysTick interrupt: */
    Cy_SysTick_DisableInterrupt();

    /* Disable all interrupts: */
    for (i = 0; i < (((NUMBER_OF_IRQS - 1) / 32) + 1); i++) /* -1 and +1: round up          */
    {
        NVIC->ICER[i] = 0xFFFFFFFF;                         /* Disable all interrupts       */
        __DSB();
        __ISB();
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < (((NUMBER_OF_IRQS - 1) / 32) + 1); i++) /* -1 and +1: round up          */
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;                         /* Clear pending register       */
    }
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    /* Copy of Cy_SysLib_EnterCriticalSection() function. This has to be executed from RAM !!! */
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    /* Copy of Cy_SysLib_ExitCriticalSection() function. This has to be executed from RAM !!! */
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

static uint32_t enabledInterrupts[((NUMBER_OF_IRQS - 1) / 32) + 1]; /* -1 and +1: round up          */
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    uint8_t i;

    /* Disable all interrupts: */
    for (i = 0; i < (((NUMBER_OF_IRQS - 1) / 32) + 1); i++)         /* -1 and +1: round up          */
    {
        enabledInterrupts[i] = NVIC->ISER[i];                       /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;                                 /* Disable all interrupts       */
        __DSB();
        __ISB();
    }

    /* Enable 1 ms timer interrupt: */
    i = tcpwm_interrupts_0_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)tcpwm_interrupts_0_IRQn) % 32 ));
    __COMPILER_BARRIER();

    /* Enable WiFi UART Rx interrupt  (and also Tx, but not needed): */
    i = scb_3_interrupt_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)scb_3_interrupt_IRQn) % 32 ));
    __COMPILER_BARRIER();

    #if (EXECUTING_FROM_RAM == 1)
    /* Enable SPC interrupt used for flash writing: */
    i = cpuss_interrupt_spcif_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)cpuss_interrupt_spcif_IRQn) % 32 ));
    __COMPILER_BARRIER();
    #endif /* (EXECUTING_FROM_RAM == 1) */
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    CY_HALT();
    while (1)
    {
        __NOP();
    }
}

void proc_softwareReset()
{
    __NVIC_SystemReset();
}

void proc_delayms(uint32_t milliseconds)
{
    cyhal_system_delay_ms(milliseconds);
}

void proc_delayus(uint32_t microseconds)
{
    for (uint32_t i = 0; i < (microseconds / 60000); i++)
    {
        cyhal_system_delay_ms(60);
    }
    cyhal_system_delay_us((uint16_t) (microseconds % 60000));
}

cyhal_wdt_t wdt_obj;
void proc_startWDT(void)
{
    /* Set Watchdog timeout to 200 ms */
    cyhal_wdt_init(&wdt_obj, 200);
}

void proc_clearWDT(void) PROC_RAM_FUNCTION_ALWAYS;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    /* Instead of 
    cyhal_wdt_kick() --> Cy_WDT_ClearWatchdog() --> Cy_WDT_ClearInterrupt()
    function, because the proc_clearWDT() function has to be executed from RAM:
    */
    SRSSLT_SRSS_INTR = SRSSLT_SRSS_INTR_WDT_MATCH_Msk;

    /* Read the interrupt register to ensure that the initial clearing write has
    * been flushed out to the hardware.
    */
    (void) SRSSLT_SRSS_INTR;
}

uint32_t proc_get_IPSR(void)
{
    return __get_IPSR();
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    cyhal_reset_reason_t resetCause;
    PROC_RESET_CAUSE cause;

    resetCause = cyhal_system_get_reset_reason();
    cause = PROC_RESET_CAUSE_UNKNOWN;
    
    if (resetCause == CYHAL_SYSTEM_RESET_WDT)
    {
        /* A watchdog timer (WDT) reset has occurred */
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_SOFT)
    {
        /* The CPU requested a system reset through it's SYSRESETREQ. */
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_ACTIVE_FAULT)
    {
        /* The fault logging system requested a reset from its Active logic. */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_DEEPSLEEP_FAULT)
    {
        /* The fault logging system requested a reset from its Deep-Sleep logic. */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_HIB_WAKEUP)
    {
        /* A reset has occurred due to a a wakeup from hibernate power mode. */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_WCO_ERR)
    {
        /* A reset has occurred due to a watch-crystal clock error */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_SYS_CLK_ERR)
    {
        /* A reset has occurred due to a system clock error */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }
    else if (resetCause == CYHAL_SYSTEM_RESET_PROTECTION)
    {
        /* A reset has occurred due to a protection violation */
        cause = PROC_RESET_CAUSE_UNKNOWN;
    }

    /* Clear the reset reason */
    if (reset_cause_clear != PROC_RESET_CAUSE_NONE_NOT_CLEAR_FLAGS)
    {
        cyhal_system_clear_reset_reason();
    }

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

#if (WATCHDOG_USE == 1)
#include "iwdg.h"
#include "wwdg.h"
#endif /* (WATCHDOG_USE == 1) */
#include "../Common/Platform/serialport.h"

#if (RELOCATE_VECTORS_TO_RAM == 1)
volatile static ISRaddress *romVectors;
__attribute__ ((aligned(256)))
volatile static ISRaddress ramVectors[16+32];
#endif /* --> (RELOCATE_VECTORS_TO_RAM == 1) */

void proc_init(void)
{
    #if (RELOCATE_VECTORS_TO_RAM == 1)
    uint16_t i;
    uint16_t numberOfIRQs;
    uint8_t enabledInterrupts;
    /* Check if RAM vector table is 256-bytes aligned: */
    if ((((uint32_t) &ramVectors[0]) % 256) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 256 bytes !!! */
        proc_halt(0);
    }
    /* Copy ROM to RAM vectors: */
    romVectors = (ISRaddress *) PROC_INT_VECT_TABLE;
    numberOfIRQs = sizeof(ramVectors) / sizeof(ISRaddress);
    enabledInterrupts = proc_EnterCriticalSection();
    for (i = 0; i < numberOfIRQs; i++)
    {
        ramVectors[i] = *(romVectors + i);
    }

    /* Change NVIC to point to RAM vectors: */
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&ramVectors;
    proc_ExitCriticalSection(enabledInterrupts);
    #endif /* --> (RELOCATE_VECTORS_TO_RAM == 1) */
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    /* __asm("CPSIE   i");  */
    __enable_irq();     /* --> __ASM volatile ("cpsie i" : : : "memory"); */
}

void proc_GlobalIntDisable(void)
{
    /* __asm("CPSID   i"); */
    __disable_irq();    /* --> __ASM volatile ("cpsid i" : : : "memory"); */
}

#define NUMBER_OF_IRQS          32
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disable all interrupts, including system tick interrupt (if available on MCU) or any other interrupts. */

    proc_clearWDT();
    /* Disable Systick interrupt: */
    SysTick->CTRL = 0;

    /* Disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;             /* Clear pending register       */
    }
    proc_clearWDT();
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

#if ((HISENSE_PROTOCOL_TIMER_USED == 3) || \
     (HISENSE_PROTOCOL_TIMER_USED == 4))
    #define TIMER_IRQn              TIM3_TIM4_IRQn
#elif (HISENSE_PROTOCOL_TIMER_USED == 17)
    #if defined(STM32G0B0xx)
        #define TIMER_IRQn              TIM17_IRQn
    #elif defined(STM32G0B1xx)
        #define TIMER_IRQn              TIM17_FDCAN_IT1_IRQn
    #else
        #error proc.c: MCU is not supported.
    #endif
#else
    #error Proc: Selected timer not supported !!!
#endif

static uint32_t enabledInterrupts[NUMBER_OF_IRQS / 32];
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    uint8_t i;
    uint32_t wifi_uart_irqn;
    uint8_t wifi_primary;

    /* Save enabled interrupts and disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        enabledInterrupts[i] = NVIC->ISER[i];   /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Enable timer interrupt: */
    i = TIMER_IRQn / 32;
    __COMPILER_BARRIER();
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)TIMER_IRQn) % 32 ));
    __COMPILER_BARRIER();

    wifi_primary = WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER;
    switch (wifi_primary)
    {
        #if (SERIALPORT0_IN_USE)
        case SERIALPORT_NUMBER_0:
            wifi_uart_irqn = USART1_IRQn;
            break;
        #endif
        #if (SERIALPORT1_IN_USE)
        case SERIALPORT_NUMBER_1:
            #if defined(STM32G0B0xx)
                wifi_uart_irqn = USART2_IRQn;
            #elif defined(STM32G0B1xx)
                wifi_uart_irqn = USART2_LPUART2_IRQn;
            #else
                #error proc.c: MCU is not supported.
            #endif
            break;
        #if (SERIALPORT2_IN_USE)
        case SERIALPORT_NUMBER_2:
        #endif
        #if (SERIALPORT3_IN_USE)
        case SERIALPORT_NUMBER_3:
        #endif
        #if (SERIALPORT4_IN_USE)
        case SERIALPORT_NUMBER_4:
        #endif
        #if (SERIALPORT5_IN_USE)
        case SERIALPORT_NUMBER_5:
        #endif
            #if defined(STM32G0B0xx)
                wifi_uart_irqn = USART3_4_5_6_IRQn;
            #elif defined(STM32G0B1xx)
                wifi_uart_irqn = USART3_4_5_6_LPUART1_IRQn;
            #else
                #error proc.c: MCU is not supported.
            #endif
            break;
        #endif /* --> SERIALPORT2_IN_USE */

        default:
            wifi_uart_irqn = 0xFFFFFFFF;
            break;
    }

    if (wifi_uart_irqn != 0xFFFFFFFF)
    {
        /* Enable WiFi UART Rx interrupt  (and also Tx, but not needed): */
        i = wifi_uart_irqn / 32;
        __COMPILER_BARRIER();
        NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)wifi_uart_irqn) % 32 ));
        __COMPILER_BARRIER();
    }
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = enabledInterrupts[i];
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        __NOP();
    }
}

void proc_softwareReset()
{
    __NVIC_SystemReset();
}

void proc_delayms(uint32_t milliseconds)
{
    /* HAL_Delay(milliseconds); does not work always */
    uint32_t i;

    for (i = 0; i < 100; i++)
    {
        proc_delayus(milliseconds * 10);
    }
}

void proc_delayus(uint32_t microseconds)
{
    uint32_t imax;

    if (microseconds < 10000)
    {
        #if DEBUG
        imax = microseconds*276/129;
        #else
        imax = microseconds*347/129;
        #endif
    }
    else
    {
        #if DEBUG
        imax = microseconds/129*276;
        #else
        imax = microseconds/129*347;
        #endif
    }

    while (imax--)
    {
        __asm("NOP");
    }
}

void proc_startWDT(void)
{
    #if (WATCHDOG_USE == 1)
    MX_IWDG_Init();                         /* IWDG_PRESCALER_32   &   Window = 127   -->   ~125 ms */
    MX_WWDG_Init();                         /* WWDG_PRESCALER_8, Window = 0x7a, Counter = 0x7a, EWIMode = WWDG_EWI_ENABLE and  NVIC interrupt enabled */
    __HAL_RCC_DBGMCU_CLK_ENABLE();          /* Enable register write                                */
    __HAL_DBGMCU_FREEZE_IWDG();             /* Stop watchdog in debug mode                          */
    __HAL_DBGMCU_FREEZE_WWDG();
    #endif /* (WATCHDOG_USE == 1) */
}

void proc_clearWDT(void) PROC_RAM_FUNCTION_ALWAYS;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    #if (WATCHDOG_USE == 1)
    /* Copy of
     * HAL_IWDG_Refresh(&hiwdg);  -->  __HAL_IWDG_RELOAD_COUNTER(hiwdg);
     * because proc_clearWDT() has to be executed from RAM !!!
     */
    WRITE_REG((&hiwdg)->Instance->KR, IWDG_KEY_RELOAD);

    /* Copy of
     * HAL_WWDG_Refresh(&hwwdg);  -->  WRITE_REG(hwwdg->Instance->CR, (hwwdg->Init.Counter));
     * because proc_clearWDT() has to be executed from RAM !!!
     */
    WRITE_REG((&hwwdg)->Instance->CR, ((&hwwdg)->Init.Counter));
    #endif /* (WATCHDOG_USE == 1) */
}

uint32_t proc_get_IPSR(void)
{
    uint32_t result;

    __asm volatile ("MRS %0, IPSR" : "=r" (result) );
    return(result);
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    /* RCC_FLAG_IWDGRST - Independent Watchdog reset flag */
    /* RCC_FLAG_WWDGRST - Window watchdog reset flag */

    /* RCC_FLAG_SFTRST  - Software Reset flag */

    /* RCC_FLAG_PWRRST  - BOR or POR/PDR reset flag */

	/* RCC_FLAG_OBLRST  - Option Byte Loader reset flag */
    /* RCC_FLAG_PINRST  - PIN reset flag */
    /* RCC_FLAG_LPWRRST - Low-Power reset flag */

    PROC_RESET_CAUSE cause;

    cause = PROC_RESET_CAUSE_UNKNOWN;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) ||
        __HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST) ||
             __HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        cause = PROC_RESET_CAUSE_UNKNOWN; /* Power-on-reset or pin-reset is reported as "Unknown" */
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_OBLRST) ||
             __HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
    {
        cause = PROC_RESET_CAUSE_UNKNOWN; /* Other reset causes */
    }

    if (reset_cause_clear != PROC_RESET_CAUSE_NONE_NOT_CLEAR_FLAGS)
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

#include "iwdg.h"
#include "../Common/Platform/serialport.h"

volatile static ISRaddress *romVectors;
__attribute__ ((aligned(512)))
volatile static ISRaddress ramVectors[16+96];

void proc_init(void)
{
    uint16_t i;
    uint16_t numberOfIRQs;
    uint8_t enabledInterrupts;
    /* Check if RAM vector table is 512-bytes aligned: */
    if ((((uint32_t) &ramVectors[0]) % 512) != 0)
    {
        /* Variable ramVectors must be aligned in RAM to 512 bytes !!! */
        proc_halt(0);
    }
    /* Copy ROM to RAM vectors: */
    romVectors = (ISRaddress *) PROC_INT_VECT_TABLE;
    numberOfIRQs = sizeof(ramVectors) / sizeof(ISRaddress);
    enabledInterrupts = proc_EnterCriticalSection();
    for (i = 0; i < numberOfIRQs; i++)
    {
        ramVectors[i] = *(romVectors + i);
    }

    /* Change NVIC to point to RAM vectors: */
    PROC_INT_VECT_TABLE_WRITE = (uint32_t)&ramVectors;
    proc_ExitCriticalSection(enabledInterrupts);
}

void proc_GlobalIntEnable(void)
{
    /* __asm("CPSIE   i");  */
    __enable_irq();     /* --> __ASM volatile ("cpsie i" : : : "memory"); */
}

void proc_GlobalIntDisable(void)
{
    /* __asm("CPSID   i"); */
    __disable_irq();    /* --> __ASM volatile ("cpsid i" : : : "memory"); */
}

#define NUMBER_OF_IRQS          (32 * 8)
void proc_AllInterruptsDisable(void)
{
    uint8_t i;

    /* Disable all interrupts, including system tick interrupt (if available on MCU) or any other interrupts. */

    proc_clearWDT();
    /* Disable Systick interrupt: */
    SysTick->CTRL = 0;

    /* Disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Clear pending register of all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;             /* Clear pending register       */
    }
    proc_clearWDT();
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile("    CPSID I\n");                            /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile("    MSR PRIMASK, r0\n");        /* Restore interrupt state */
    __asm volatile("    BX lr\n");
}

#if ((HISENSE_PROTOCOL_TIMER_USED == 11))
#define TIMER_IRQn              TIM1_TRG_COM_TIM11_IRQn
#else
#error Proc: Selected timer not supported !!!
#endif

static uint32_t enabledInterrupts[NUMBER_OF_IRQS / 32];
void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    uint8_t i;
    uint32_t wifi_uart_irqn;
    uint8_t wifi_primary;

    wifi_uart_irqn = 0xFFFFFFFF;

    /* Save enabled interrupts and disable all interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        enabledInterrupts[i] = NVIC->ISER[i];   /* Save enabled interrupts      */
        NVIC->ICER[i] = 0xFFFFFFFF;             /* Disable all interrupts       */
    }

    /* Enable timer interrupt: */
    i = TIMER_IRQn / 32;
    NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)TIMER_IRQn) % 32 ));

    wifi_primary = WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER;
    switch (wifi_primary)
    {
        #if (SERIALPORT1_IN_USE)
        case SERIALPORT_NUMBER_1:
            wifi_uart_irqn = USART2_IRQn;
            break;
        #endif
        #if (SERIALPORT2_IN_USE)
        case SERIALPORT_NUMBER_2:
            wifi_uart_irqn = USART2_IRQn;
            break;
        #if (SERIALPORT3_IN_USE)
        case SERIALPORT_NUMBER_3:
            wifi_uart_irqn = UART4_IRQn;
            break;
        #endif
        #if (SERIALPORT4_IN_USE)
        case SERIALPORT_NUMBER_4:
        #endif
        #if (SERIALPORT5_IN_USE)
        case SERIALPORT_NUMBER_5:
        #endif
        #if (SERIALPORT6_IN_USE)
        case SERIALPORT_NUMBER_6:
        #endif
            wifi_uart_irqn = USART2_IRQn;
            break;
        #endif /* --> SERIALPORT2_IN_USE */
    }

    if (wifi_uart_irqn != 0xFFFFFFFF)
    {
        /* Enable WiFi UART Rx interrupt  (and also Tx, but not needed): */
        i = wifi_uart_irqn / 32;
        NVIC->ISER[i] = (uint32_t)(1 << (((uint32_t)wifi_uart_irqn) % 32 ));
    }
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    uint8_t i;

    /* Re-enabled disabled interrupts: */
    for (i = 0; i < NUMBER_OF_IRQS / 32; i++)
    {
        NVIC->ICPR[i] = enabledInterrupts[i];
        NVIC->ISER[i] = enabledInterrupts[i];
        __DSB();
        __ISB();
    }
}

void proc_halt(uint8_t reason)
{
    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        __NOP();
    }
}

void proc_softwareReset()
{
    __NVIC_SystemReset();
}

void proc_delayms(uint32_t milliseconds)
{
    /* HAL_Delay(milliseconds); does not work always */
    uint32_t i;

    for (i = 0; i < 100; i++)
    {
        proc_delayus(milliseconds * 10);
    }
}

void proc_delayus(uint32_t microseconds)
{
    uint32_t imax;

    if (microseconds < 10000)
    {
        imax = microseconds*347/129;
    }
    else
    {
        imax = microseconds/129*347;
    }

    while (imax--)
    {
        __asm("NOP");
    }
}

void proc_startWDT(void)
{
    MX_IWDG_Init();                         /* IWDG_PRESCALER_32   &   Window = 127   -->   ~125 ms */
    //__DBGMCU_CLK_ENABLE();
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_IWDG_STOP;/* Stop watchdog in debug mode                          */
}

void proc_clearWDT(void) PROC_RAM_FUNCTION_ALWAYS;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    /* Copy of
     * HAL_IWDG_Refresh(&hiwdg);  -->  __HAL_IWDG_RELOAD_COUNTER(hiwdg);
     * because proc_clearWDT() has to be executed from RAM !!!
     */
    (&hiwdg)->Instance->KR  = IWDG_KEY_RELOAD;
}

uint32_t proc_get_IPSR(void)
{
    uint32_t result;

    __asm volatile ("MRS %0, IPSR" : "=r" (result) );
    return(result);
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    /* RCC_FLAG_IWDGRST - Independent Watchdog reset flag */
    /* RCC_FLAG_WWDGRST - Window watchdog reset flag */

    /* RCC_FLAG_SFTRST  - Software Reset flag */

    /* RCC_FLAG_PWRRST  - BOR or POR/PDR reset flag */

    /* RCC_FLAG_OBLRST  - Option Byte Loader reset flag */
    /* RCC_FLAG_PINRST  - PIN reset flag */
    /* RCC_FLAG_LPWRRST - Low-Power reset flag */

    PROC_RESET_CAUSE cause;

    cause = PROC_RESET_CAUSE_UNKNOWN;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) ||
        __HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
    {
        cause = PROC_RESET_CAUSE_WATCHDOGTIMEOUT;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        cause = PROC_RESET_CAUSE_SOFTWARERESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) ||
             __HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        cause = PROC_RESET_CAUSE_UNKNOWN; /* Power-on-reset or pin-reset is reported as "Unknown" */
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST) ||
            __HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST) ||
            __HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
    {
        cause = PROC_RESET_CAUSE_UNKNOWN; /* Other reset causes */
    }

    if (reset_cause_clear != PROC_RESET_CAUSE_NONE_NOT_CLEAR_FLAGS)
    {
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }

    return cause;
}

__attribute__((noinline)) /* Workaround for GCC toolchain bug with inlining */
__attribute__((naked))
void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    __asm volatile("    msr msp, r0\n");
    __asm volatile("    msr psp, r0\n");
    __asm volatile("    bx r1\n");
}

#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

void proc_init(void)
{
    /* Implement for other platform(s) */
}

void proc_GlobalIntEnable(void)
{
    __asm volatile ("cpsie i" : : : "memory");
}

void proc_GlobalIntDisable(void)
{
    __asm volatile ("cpsid i" : : : "memory");
}

void proc_AllInterruptsDisable(void)
{

}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    uint32_t result;

    __asm volatile ("   MRS %0, PRIMASK" : "=r" (result) );     /* Save and return interrupt state */
    __asm volatile ("   cpsid i" : : : "memory");               /* Disable interrupts */

    return ((uint8_t) result);
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    __asm volatile ("MSR primask, %0" : : "r" (enableInterrupts) : "memory");
}

void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except UART RX for WiFi and 1ms timer interrupt: */
    //#warning TODO implement proc_disableNonRAMinterrupts() function: Disables interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    //#warning TODO implement proc_enableNonRAMinterrupts() function: Re-enable interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_halt(uint8_t reason)
{
    while (1) {}
    /* Implement for other platform(s) */
}

void proc_softwareReset()
{
    /* Implement for other platform(s) */
}

void proc_delayms(uint32_t milliseconds)
{
    /* Implement for other platform(s) */
}

void proc_delayus(uint32_t microseconds)
{
    /* Implement for other platform(s) */
}

void proc_startWDT(void)
{
    /* Implement for other platform(s) */

    /* Set Watchdog timeout to value between 50 and 300 ms (on some MCUs the value cannot be set to exact value). */

    /* Warning: If Window Watchdog is used, window functionality should be disabled (set to 0) !!! */
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    /* Implement for other platform(s) */
}

uint32_t proc_get_IPSR(void)
{
    /* Implement for other platform(s) */
    return 0;
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    /* Implement for other platform(s) */
    return PROC_RESET_CAUSE_UNKNOWN;
}

#elif (PLATFORM_USED == PLATFORM_template)

#warning Proc: Implement new platform:

void proc_init(void)
{
    /* Typically (if not already done in pre main() phase): 
       - check if RAM vector table is 256-bytes aligned
       - copy ROM to RAM vectors
       - change NVIC to point to RAM vectors
    */

    /* Implement for other platform(s) */
}

void proc_deinit(void)
{
}

void proc_GlobalIntEnable(void)
{
    /* Implement for other platform(s) */
}

void proc_GlobalIntDisable(void)
{
    /* Implement for other platform(s) */
}

void proc_AllInterruptsDisable(void)
{
    /* Disable all interrupts, including system tick interrupt (if available on MCU) or any other interrupts. */


    #warning TODO: Implement proc_AllInterruptsDisable() function (disabling all interrupts):
    /* Disable all interrupts: */
    /* TODO */

    /* Clear pending register of all interrupts: */
    /* TODO */
}

uint8_t proc_EnterCriticalSection(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
uint8_t proc_EnterCriticalSection(void)
{
    #warning Execute from RAM and substitute with assembly (see at PSoC5)
    #error TODO: Implement proc_EnterCriticalSection() function !!!
}

void proc_ExitCriticalSection(uint8_t enableInterrupts) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_INFRONT
void proc_ExitCriticalSection(uint8_t enableInterrupts)
{
    #warning Execute from RAM and substitute with assembly (see at PSoC5)
    #error TODO: Implement proc_ExitCriticalSection() function !!!
}

void proc_disableNonRAMinterrupts(void)
{
    /* Disable all interrupts except:
        - UART RX for WiFi,
        - 1ms timer interrupt,
        - interrupt for flash writing (if it is used for the platform and if it is executing from RAM).
    */
    #warning TODO implement proc_disableNonRAMinterrupts() function: Disables interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_enableNonRAMinterrupts(void)
{
    /* Clear pending register and re-enable interrupts: */
    #warning TODO implement proc_enableNonRAMinterrupts() function: Re-enable interrupts whose Interrupt Service Rutines are not executed from RAM.
}

void proc_halt(uint8_t reason)
{
    /* Implement for other platform(s) */

    #if 0 EXAMPLE:

    if (0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }
    __asm("    bkpt    1");
    while (1)
    {
        __NOP();
    }
    #endif
}

void proc_softwareReset()
{
    /* Implement for other platform(s) */

    #if 0 EXAMPLE:
    __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                        buffered write are completed before reset */
    SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                    SCB_AIRCR_SYSRESETREQ_Msk);
    __DSB();                                                          /* Ensure completion of memory access */

    for(;;)                                                           /* wait until reset */
    {
        __NOP();
    }
    #endif
}

void proc_delayms(uint32_t milliseconds)
{
    /* Implement for other platform(s) */
}

void proc_delayus(uint32_t microseconds)
{
    /* Implement for other platform(s) */
}

void proc_startWDT(void)
{
    /* Implement for other platform(s) */

    /* Set Watchdog timeout to value between 50 and 300 ms (on some MCUs the value cannot be set to exact value). */

    /* Warning: If Window Watchdog is used, window functionality should be disabled (set to 0) !!! */
}

void proc_clearWDT(void) PROC_RAM_FUNCTION_ALWAYS;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void proc_clearWDT(void)
{
    /* Implement for other platform(s) */
}

uint32_t proc_get_IPSR(void)
{
    /* Implement for other platform(s) */
    return 0;
}

PROC_RESET_CAUSE proc_reset_cause(PROC_RESET_CAUSE reset_cause_clear)
{
    /* Implement determination of reset cause in the following sequence:
     * 1. Watchdog timeout (also Window watchdog, if used) -> return PROC_RESET_CAUSE_WATCHDOGTIMEOUT
     * 2. Software reset                                   -> return PROC_RESET_CAUSE_SOFTWARERESET
     * 3. Power-on-reset or HW-pin-reset                   -> return PROC_RESET_CAUSE_UNKNOWN
     * 4. Any other reset causes                           -> return PROC_RESET_CAUSE_UNKNOWN
     */

    return PROC_RESET_CAUSE_UNKNOWN;
}

void proc_jumpToStartupAddress(uint32_t stackPointerValue, uint32_t startupAddress)
{
    /* Implement for other platform(s) */
    #error TODO implement proc_jumpToStartupAddress() function !!!
}

#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error proc.c: Undefined or unsupported platform
#endif /* PLATFORM_USED */

/* [] END OF FILE */
