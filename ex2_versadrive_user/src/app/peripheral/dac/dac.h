////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     dac.h
 * @brief    Functions for DAC
 * @author   Haris Kovacevic
 * @date     10.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup DAC_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __DAC_H
#define __DAC_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "stm32g4xx_hal.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef dac_init(void);
void              dac_protection_limit_update(void);

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
