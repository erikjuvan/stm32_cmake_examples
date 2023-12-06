////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      protections.c
*@brief     Module with protections of drive
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
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "peripheral/adc/adc.h"
#include "peripheral/timer/timer.h"
#include "protections.h"
#include "protections_obj.h"
#include "simulink_model/emDriveFOC_control_global.h"
#include "simulink_model/emDriveFOC_control.h"

#include "motor_control_fsm.h"
#include "func_table.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Definition of ms how long the motor temperature error is present,
 * before it is cleared
 */
#define PROTECTIONS_MOTOR_OVERTEMPERATURE_ERROR_CLEAR_TIME 	(60 * 3 * 1000) // 3 minutes

/**
 * Definition of how much samples of mootor temperature must be over
 * defined temperature to trigger error
 */
#define PROTETIONS_MOTOR_OVERTEMPERATURE_ERROR_SAMPLES	( 2 )

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

/**
 * Protections data structure
 */
typedef struct
{
	struct
	{
		uint32_t motor_overtemperature;
	}timestamp;
}protectiuons_data_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
// CANopen objects are defined in protections_obj.c file for normal release build
// and protections_obj_125.c file for testing release build

static protectiuons_data_t protections = {0};

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
//static __FOC_FAST_CODE__ void protections_clear_fault(const protections_t protection);
static float protections_fabs(float variable); /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats" */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets flag with given protection
*
*@param[in] protection - protection to be cleared
*@return    void
 */
////////////////////////////////////////////////////////////////////////////////
void protections_set_fault(const protections_t protection)
{
    if( (motor_control__protections & protection) != protection )
    {
        MOTOR_CONTROL_PRINTF("PROTECTIONS: Triggered ID:%d\n", protection)
        motor_control__protections |= protection;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Clears flag with given protection
*
*@param[in] protection - protection to be cleared
*@return    void
 */
////////////////////////////////////////////////////////////////////////////////
//static __FOC_FAST_CODE__ void protections_clear_fault(const protections_t protection)
__FOC_FAST_CODE__ void protections_clear_fault(const protections_t protection)
{
    if( (motor_control__protections & protection) == protection )
    {
        motor_control__protections &= ~protection;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Fast protections check function.
 *
 *  This function shall be called periodically from FOC fast loop and will
 *  check for all protections that need to be checked in every FOC loop.
 *  Check done are over current, over and under voltage.
 *
 *@return returns bit mask of protections
 */
////////////////////////////////////////////////////////////////////////////////
protections_t __FOC_FAST_CODE__ protections_update_fast(const motor_control_data_t *const data)
{

    // Check DC link voltage
    if( data->dc_link > protections__overvoltage )
    {
        protections_set_fault(ePROTECTIONS_OVERVOLTAGE);
        // Generate break event to put motor in short circuit
        timer1_break_event_generate();
    }
    if( data->dc_link < protections__undervoltage)
    {
        protections_set_fault(ePROTECTIONS_UNDERVOLTAGE);
    }

    // CHeck this only in running state, since SAFE is manipulating this flag
    if( motor_control_fsm_get_state() == eMOTOR_CONTROL_FSM_STATE_RUN )
    {
        // Over current shall only be checked if there is no ASC condition present
        if( (false == timer1_is_break_active_raw()) && (false == motor_control_is_break_enable()) )
        {
            // Check for over currents
            if(  (protections_fabs(data->current.phase_u) > protections__phase_overcurrent)
              || (protections_fabs(data->current.phase_v) > protections__phase_overcurrent)
              || (protections_fabs(data->current.phase_w) > protections__phase_overcurrent) )
            {
                protections_set_fault(ePROTECTIONS_OVERCURRENT);
            }

            // Check if hardware over current occurred
            if(timer1_is_break2_active() == true)
            {
                protections_set_fault(ePROTECTIONS_OVERCURRENT);
            }
        }

        // Check if hardware over voltage occurred
        if(timer1_is_hw_break_active() == true)
        {
            // Check if maybe Safety disabled motor?
            if( pg_safe_func_table->fp_Can_Drive_Motor() == true )
            {
                protections_set_fault(ePROTECTIONS_OVERVOLTAGE);
            }
        }
    }

#if (ALT_POWER_TEST != 1)
    if((Ctrl_CurrentReg_StatusModFactor > 0.6f) &&
       ((Ctrl_Sensorless_Status__State == enumSenzorlessState_OpenLoop) ||
        (Ctrl_Sensorless_Status__State == enumSenzorlessState_Align)))
    {
        protections_set_fault(ePROTECTIONS_MOTOR_ERROR);
    }
#endif

    // If we are not changing rotation direction then we shall never fall back to open loop
    if( signbit(emDriveFOC_control_U.Control.RPM_ref) == signbit(emDriveFOC_control_Y.DataTsc.RPM) )
    {
        if( Ctrl_Sensorless_Status__State == enumSenzorlessState_TransitionToOpenLoop )
        {
            protections_set_fault(ePROTECTIONS_MOTOR_CONTROL_ERROR);
        }
    }

    return motor_control__protections;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Slow protections check function.
 *
 *  This function shall be called periodically from FOC slow loop and will
 *  check for all protections that need to be checked rearly.
 *  Check done are temperatures of bridge, capacitor and motor
 *
 *@return returns bit mask of protections
 */
////////////////////////////////////////////////////////////////////////////////
protections_t __FOC_SLOW_CODE__ protections_update(const motor_control_temperature_t *const temperatures)
{
	static uint8_t motor_temp_cnt = 0;

    // Check temperatures
    if( temperatures->bridge > protections__bridge_overtemperature )
    {
        protections_set_fault(ePROTECTIONS_OVERTEMP_BRIDGE);
    }

    // Only check when new measurements is done, which is at end of align sensorless state
	if( Ctrl_Sensorless_Status__State == enumSenzorlessState_OpenLoop )
	{
		// Check motor over temperature error
		if( temperatures->motor > protections__motor_overtemperature)
		{
			// Don't trigger on first sample since, model updated temperature later,
			// so make sure that temperature is actually updated
			if( motor_temp_cnt >= PROTETIONS_MOTOR_OVERTEMPERATURE_ERROR_SAMPLES)
			{
				// Store time stamp if error just happens
				if( protections.timestamp.motor_overtemperature == 0 )
				{
					protections.timestamp.motor_overtemperature = HAL_GetTick();
				}

				protections_set_fault(ePROTECTIONS_MOTOR_OVERTEMP);
			}
			else
			{
				motor_temp_cnt++;
			}
		}
		else
		{
			motor_temp_cnt = 0;
		}
	}
	else
	{
		motor_temp_cnt = 0;
	}

    return motor_control__protections;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Protections check for current offset
 *
 *  This function is called in initilization of motor control, when current
 *  offsets are measured.
 *
 *@return returns bit mask of protections
 */
////////////////////////////////////////////////////////////////////////////////
protections_t /*__FOC_FAST_CODE__*/ protections_check_current_offset(const motor_control_current_t *const currents)
{
    // Check for over currents
    if(  (protections_fabs(currents->phase_u) > MOTOR_CONTROL_MAX_CURRENT_OFFSET)
      || (protections_fabs(currents->phase_v) > MOTOR_CONTROL_MAX_CURRENT_OFFSET)
      || (protections_fabs(currents->phase_w) > MOTOR_CONTROL_MAX_CURRENT_OFFSET) )
    {
        protections_set_fault(ePROTECTIONS_CURRENT_OFFSET);
    }

    return motor_control__protections;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Gets the status of all currently triggered protections
 *
 *@return returns bit mask of active protections
 */
////////////////////////////////////////////////////////////////////////////////
protections_t __FOC_FAST_CODE__ protections_get_active(void)
{
    return motor_control__protections;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Updated status of warnings
 *
 *@return returns bit mask of active warnings
 */
////////////////////////////////////////////////////////////////////////////////
warnings_t warnings_update_fast(const motor_control_data_t *const data)
{
    // Power limit active ?
    if( Ctrl_PowerLimit__StatusActive < 1.0f )
    {
        motor_control__warnigns |= eMC_WARNING_POWER_LIMIT;
    }
    else
    {
        motor_control__warnigns &= ~eMC_WARNING_POWER_LIMIT;
    }

    // Bridge Temperature limit
    if( Ctrl_Limits_StatusLim[2] < 1.0f )
    {
        motor_control__warnigns |= eMC_WARNING_BRIDGE_TEMP_LIMIT;
    }
    else
    {
        motor_control__warnigns &= ~eMC_WARNING_BRIDGE_TEMP_LIMIT;
    }

    // Update motor temperature warning
    if( data->temperature.motor >= Ctrl_Limits__MotorTemperature)
    {
		motor_control__warnigns |= eMC_WARNING_MOTOR_TEMP_LIMIT;
	}
	else
	{
		motor_control__warnigns &= ~eMC_WARNING_MOTOR_TEMP_LIMIT;
	}

    return motor_control__warnigns;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Gets the status of currently active warnings
 *
 *@return returns bit mask of active warnings
 */
////////////////////////////////////////////////////////////////////////////////
warnings_t warings_get_active(void)
{
    return motor_control__warnigns;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Clear active protections.
 *
 *  Functions will try to clear all currently active protections. If the fault
 *  is still preset protections will not be cleared as it is still present.
 *
 *@return returns bit mask of active protections
 */
////////////////////////////////////////////////////////////////////////////////
protections_t protections_clear(void)
{

	// Clear error due to motor control model
	protections_clear_fault(ePROTECTIONS_MOTOR_CONTROL_ERROR);
    protections_clear_fault(ePROTECTIONS_MOTOR_ERROR);

    // Clear overcurrent
    if(  (protections_fabs(adc_analog_get()->current_u) < protections__phase_overcurrent)
      && (protections_fabs(adc_analog_get()->current_v) < protections__phase_overcurrent)
      && (protections_fabs(adc_analog_get()->current_w) < protections__phase_overcurrent) )
    {
        // Only clear flag if we actualy have error set before
        // and SAFE allows clearing of the break flag meaning it does not have control over it
        if(( protections_get_active() & ePROTECTIONS_OVERCURRENT ) &&
           ( pg_safe_func_table->fp_Can_Clear_Break2() ))
        {
            // Clear flag for over current (hardware over current)
            timer1_clear_break2_flag();
        }

        protections_clear_fault(ePROTECTIONS_OVERCURRENT);
    }

	float dc_link = adc_analog_get()->vdc; /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats" */
		
    // Clear over voltage
    if( dc_link < protections__overvoltage )
    {
        // Only clear flag if we actualy have error set before
        // and SAFE allows clearing of the break flag meaning it does not have control over it
        if(( protections_get_active() & ePROTECTIONS_OVERVOLTAGE ) &&
           ( pg_safe_func_table->fp_Can_Clear_Break() ))
        {
            // Clear flag for over voltage (hardware over voltage)
            timer1_clear_break_flag();
        }

        protections_clear_fault(ePROTECTIONS_OVERVOLTAGE);
    }
    // Clear under voltage
    if( dc_link > protections__undervoltage)
    {
        protections_clear_fault(ePROTECTIONS_UNDERVOLTAGE);
    }

    // Temperatures
    if( adc_analog_get()->tso_igbt < protections__bridge_overtemperature )
    {
       protections_clear_fault(ePROTECTIONS_OVERTEMP_BRIDGE);
    }

    // Motor temperature error is cleared after 3 minutes
    if( protections_get_active() & ePROTECTIONS_MOTOR_OVERTEMP )
    {
		if( (HAL_GetTick() - protections.timestamp.motor_overtemperature) > PROTECTIONS_MOTOR_OVERTEMPERATURE_ERROR_CLEAR_TIME )
		{
			protections_clear_fault(ePROTECTIONS_MOTOR_OVERTEMP);

			protections.timestamp.motor_overtemperature = 0;
		}
    }

    return motor_control__protections;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        This function calculates absolute value.
*
* @param[in]    variable		Input variable
* @return       abs_value		Absolute value of input variable
*/
////////////////////////////////////////////////////////////////////////////////
static float protections_fabs(float variable) /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats" */
{
	float abs_value; /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats" */

	if(variable < 0.0f)
	{
		abs_value = -variable;
	}
	else
	{
		abs_value = variable;
	}

	return abs_value;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
