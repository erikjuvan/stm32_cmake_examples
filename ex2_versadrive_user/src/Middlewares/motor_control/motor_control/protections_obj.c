////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      protections_obj.c
*@brief     CANopen objects for protections file.
*@author    Marko Rman
*@date      07.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup MOTOR_CONTROL_PROTECTIONS
* @{ <!-- BEGIN GROUP -->
*
* Following group is part of motor control protections
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////////////////
#include "protections_obj.h"
#include "protections.h"

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable" */
/*
@! SYMBOL       = motor_control__protections
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = UBYTE [0 ... 255]
@! DESCRIPTION  = "Active motor control protections (bit 0 - Overcurrent, bit 1 - Overvoltage, bit 2 - Undervoltage, bit 3 - Bridge Overtemperature, bit 4 - Capacitor overtemperature, bit 5 - Motor overtemperature"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 3
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 0
@! DEFAULT      = 0
@! END
*/
volatile protections_t motor_control__protections = ePROTECTIONS_NONE;

/*
@! SYMBOL       = motor_control__warnigns
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = UBYTE [0 ... 255]
@! DESCRIPTION  = "Active motor control warnings (bit 0 - power limit, bit 1 - temperature limit"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 5
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 0
@! DEFAULT      = 0
@! END
*/
volatile warnings_t motor_control__warnigns = eMC_WARNING_NONE;

/*
@! SYMBOL       = protections__phase_overcurrent
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 1100]
@! DESCRIPTION  = "Maximum phase current for single measurement"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 8.0
@! END
*/
volatile float32_t protections__phase_overcurrent = 8.0f;

/*
@! SYMBOL       = protections__overvoltage
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 450]
@! DESCRIPTION  = "Maximum DC link voltage"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 410
@! END
*/
volatile float32_t protections__overvoltage = 410.0f;

/*
@! SYMBOL       = protections__undervoltage
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [15 ... 350]
@! DESCRIPTION  = "Minimum DC link voltage"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 3
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 100.0
@! END
*/
volatile float32_t protections__undervoltage = 100.0f;

/*
@! SYMBOL       = protections__bridge_overtemperature
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 150]
@! DESCRIPTION  = "Maximum temperature for bridge in �C"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 4
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 105
@! END
*/
volatile float32_t protections__bridge_overtemperature = 105.0f;

/*
@! SYMBOL       = protections__motor_overtemperature
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 200]
@! DESCRIPTION  = "Maximum temperature for motor in �C"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 7
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 150.0
@! END
*/
volatile float32_t protections__motor_overtemperature = 150.0f;

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable" */

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
