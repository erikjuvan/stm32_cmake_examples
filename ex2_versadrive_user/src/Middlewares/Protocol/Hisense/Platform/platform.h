//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        platform.h
// CREATION DATE:      2019/12/03
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/03/04
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Platform setup Hardware Abstraction Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PLATFORM_HAL_H
#define PLATFORM_HAL_H

#include "types.h"
#include "cfg/project_cfg.h"
#include "cfg/platform_cfg.h"


// DESCRIPTION:
//   Initializes platform (GPIO's, Timer, ...).
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void platform_init(void);


// DESCRIPTION:
//   Deinitializes platform (GPIO's, Timer, ...).
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void platform_deinit(void);


// DESCRIPTION:
//   Returns which platform is in use.
//   This is needed because another "#if" cannot be used inside "#define".
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   Platform that is in use.
// ----------------------------------------------------------------------------
uint8_t platform_platformUsed(void);


/*******************************************************************************************************************//**
* @brief    Returns UART clock source frequency in Hz.
* 
* @param    uart_number  -  UART number to get the frequency for.
* @return   UART clock source frequency in Hz.
*
* @details  Example:
* @code
*    uint32_t freq;
*    freq = platform_uart_clock_source_frequency_get(3);
* @endcode
*
***********************************************************************************************************************/
uint32_t platform_uart_clock_source_frequency_get(uint8_t uart_number);


/*******************************************************************************************************************//**
* @brief    Returns 1ms timer clock source frequency in Hz.
* 
* @return   1ms timer clock source frequency in Hz.
*
* @details  Example:
* @code
*    uint32_t freq;
*    freq = platform_timer_clock_source_frequency_get();
* @endcode
*
***********************************************************************************************************************/
uint32_t platform_timer_clock_source_frequency_get(void);


#if ((PLATFORM_USED == PLATFORM_CYPRESS_PSOC4) && (EXECUTING_FROM_RAM == 1))
/*******************************************************************************************************************//**
* @brief    Sets IMO clock (only for Cypress PSoC4)
* 
* @param    frequency       Frequency in Hz to set the IMO clock.
*
* @details  Example:
* @code
*    platform_imo_clock_set(48000000);
* @endcode
*
***********************************************************************************************************************/
void platform_imo_clock_set(uint32_t frequency);


/*******************************************************************************************************************//**
* @brief    Corrects the 1ms timer prescaler regarding HCLK passed in parameter (only for Cypress PSoC4)
* 
* @param    frequency       Frequency of the HCLK in Hz.
*
* @details  Example:
* @code
*    platform_hclk_clock_set(48000000);
* @endcode
*
***********************************************************************************************************************/
void platform_hclk_clock_set(uint32_t frequency);


#endif

#endif /* PLATFORM_HAL_H */
/* [] END OF FILE */
