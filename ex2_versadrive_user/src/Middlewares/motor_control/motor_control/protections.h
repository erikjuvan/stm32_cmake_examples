////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      protections.h
*@brief     Module with protections of drive
*@author    Marko Rman
*@date      07.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup MOTOR_CONTROL_PROTECTIONS
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __PROTECTIONS_H
#define __PROTECTIONS_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "project_config.h"
#include "motor_control.h"
#include "../motor_control_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
 * Definitions of possible protections that can be triggered
 */
typedef enum
{
    ePROTECTIONS_NONE               = 0x0000, /**< None of protections is triggered */
    ePROTECTIONS_OVERCURRENT        = 0x0001, /**< Phase Over current */
    ePROTECTIONS_OVERVOLTAGE        = 0x0002, /**< DC link over voltage */
    ePROTECTIONS_UNDERVOLTAGE       = 0x0004, /**< DC link under voltage */
    ePROTECTIONS_OVERTEMP_BRIDGE    = 0x0008, /**< Over temperature of bridge */
    ePROTECTIONS_CURRENT_OFFSET     = 0x0010, /**< Current offset to big */
	ePROTECTIONS_CURRENT_MEAS_ERROR = 0x0020, /**< Current measurement error */
	ePROTECTIONS_MOTOR_CONTROL_ERROR= 0x0040, /**< Motor control not returning enabled signal */
	ePROTECTIONS_MOTOR_ERROR        = 0x0080, /**< No motor connected */
	ePROTECTIONS_MOTOR_OVERTEMP		= 0x0100, /**< MOtor Overtemperature */
    ePROTECTIONS_ALL                = 0xFFFF, /**< Mask for all possible protection */
}protections_t;

/**
 * Definitions of possible warning from motor control
 */
typedef enum
{
    eMC_WARNING_NONE                = 0x0000,   /**< No active warning */
    eMC_WARNING_POWER_LIMIT         = 0x0001,   /**< Performance limited due to power limit reached */
    eMC_WARNING_BRIDGE_TEMP_LIMIT   = 0x0002,   /**< Performance limited due to Bridge temperature de-rating */
    eMC_WARNING_MOTOR_TEMP_LIMIT    = 0x0004,   /**< Performance limited due to Motor temperature de-rating */
}warnings_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
protections_t protections_update_fast(const motor_control_data_t *const data);
protections_t protections_update(const motor_control_temperature_t *const temperatures);
protections_t protections_check_current_offset(const motor_control_current_t *const currents);
protections_t protections_get_active(void);
protections_t protections_clear(void);

warnings_t warnings_update_fast(const motor_control_data_t *const data);
warnings_t warings_get_active(void);

void protections_set_fault(const protections_t protection);
void protections_clear_fault(const protections_t protection);

#endif // __PROTECTIONS_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
