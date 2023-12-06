////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      current_rec.c
*@brief     Current reconstruction
*@author    Haris Kovacevic
*@date      29.06.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup MOTOR_CONTROL_CURRENT_REC
* @{ <!-- BEGIN GROUP -->
*
* Following group is part of motor control current reconstruction
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "current_rec.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Define section to link FAST loop functions to RAM.
 * Add "select "foc_fast_code";" to linker file in coresponding RAM
 */
#define __FOC_FAST_CODE__   __attribute__((section(".ccmram")))

/**
 * Define section to link SLOW loop functions to RAM.
 * Add "select "foc_slow_code";";" to linker file in coresponding RAM
 */
#define __FOC_SLOW_CODE__   __attribute__((section ("foc_slow_code")))

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = current_rec__sector
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE [0 ... 255]
@! DESCRIPTION  = "Voltage vector sector"
@! GROUP        = current_rec
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5004
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 0
@! END
*/
volatile uint8_t current_rec__sector = 0U;

/*
@! SYMBOL       = current_rec__sector_old
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = UBYTE [0 ... 255]
@! DESCRIPTION  = "Voltage vector previous sector"
@! GROUP        = current_rec
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5004
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 0
@! END
*/
volatile uint8_t current_rec__sector_old = 0U;

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static uint8_t current_rec_determine_voltage_sector(const uint16_t a, const uint16_t b, const uint16_t c);
static uint8_t current_rec_get_sector(const uint16_t a, const uint16_t b, const uint16_t c);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    	Function calculates third current component
*
* @return       None
*/
////////////////////////////////////////////////////////////////////////////////
void current_rec_execute(motor_control_current_t * const phaseCurrents,
						  const uint16_t a, const uint16_t b, const uint16_t c)
{
	uint8_t sector;

	sector = current_rec_get_sector(a, b, c);

#if 1
	switch(sector)
	{
		// A > B & A > C
		case 0:
			phaseCurrents->phase_u = -(phaseCurrents->phase_v + phaseCurrents->phase_w);
			break;
		// B > A & B > C
		case 1:
			phaseCurrents->phase_v = -(phaseCurrents->phase_u + phaseCurrents->phase_w);
			break;
		// C > A & C > B
		case 2:
			phaseCurrents->phase_w = -(phaseCurrents->phase_u + phaseCurrents->phase_v);
			break;
		default:
			// Do nothing
			break;
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    	Function determines sector based on calculated duty cycles
*
* @return       returns sector value
*/
////////////////////////////////////////////////////////////////////////////////
static uint8_t current_rec_determine_voltage_sector(const uint16_t a, const uint16_t b, const uint16_t c)
{
	uint8_t sector;

	// A > B & A > C
	if((a < b) && (a <= c))
	{
		sector = 0U;
	}
	// B > A & B > C
	else if((b < a) && (b <= c))
	{
		sector = 1U;
	}
	// C > A & C > B
	else if((c < a) && (c <= b))
	{
		sector = 2U;
	}
	else
	{
		sector = 3U;
		// No actions required according to MISRA
	}

	return sector;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    	Function returns sector value
*
* @return       returns sector value
*/
////////////////////////////////////////////////////////////////////////////////
static uint8_t current_rec_get_sector(const uint16_t a, const uint16_t b, const uint16_t c)
{
	current_rec__sector_old = current_rec__sector;
	current_rec__sector = current_rec_determine_voltage_sector(a, b, c);

	return current_rec__sector_old;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
