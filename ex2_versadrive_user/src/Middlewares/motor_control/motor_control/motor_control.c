////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      motor_control.c
*@brief     API to generated motor control model
*@author    Marko Rman
*@date      05.01.2021
*@version   V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup MOTOR_CONTROL
* @{ <!-- BEGIN GROUP -->
*
* Following module is part of motor control module
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>

#include "simulink_model/emDriveFOC_control.h"
#include "simulink_model/rtmodel.h"
#include "motor_control.h"
#include "motor_control_fsm.h"
#include "../motor_control_cfg.h"
#include "current_rec.h"

#include "peripheral/adc/adc.h"
#include "peripheral/gpio/gpio.h"  // just used for peripheral purposes
#include "peripheral/timer/timer.h"
#include "peripheral/exti/exti.h"
#include "peripheral/dac/dac.h"
#include "peripheral/opamp/opamp.h"
#include "peripheral/comparator/comparator.h"
#include "peripheral/dma/dma.h"

#include "peripheral/watchdog/watchdog.h"
#include "project_config.h"
#include "performance_meas.h"

#include "motor_control/motor_control/protections.h"

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

/**
 * Variable that hold information of initialization
 */
static bool g_motor_control_initialized = false;

/**
 * Holder of motor control data
 */
static motor_control_data_t g_motor_control_data = {{0}};

/**
 * Number of registered callbacks
 */
static uint8_t g_motor_control_callbacks_cnt = 0;

/**
 * Array of functions for registered callbacks
 */
static motor_control_callback_t g_motor_control_callbacks[MOTOR_CONTROL_MAX_CALLBACKS] = {NULL};

/* parasoft-begin-suppress EMSISO-GLOBAL NAMING-06 "Flow variable, not applicable." */
/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */
// TODO remove this flow variable once dc link measurement is realized in HW

/*
@! SYMBOL       = motor_control__dc_link
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [12.0 ... 120.0]
@! DESCRIPTION  = "Simulated DC link voltage"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 4
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile float32_t motor_control__dc_link = 50.0f;

/*
   @@ SYMBOL = Ctrl_CurrentReg__CurrentOffset_Iu
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-3.0 ... 3.0]
   @@ DESCRIPTION = "U phase current offset. [A]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 8
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__CurrentOffset_Iu = 0.0f;

/*
   @@ SYMBOL = Ctrl_CurrentReg__CurrentOffset_Iv
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-3.0 ... 3.0]
   @@ DESCRIPTION = "V phase current offset. [A]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 9
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = -0.25
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__CurrentOffset_Iv = 0.0f;

/*
   @@ SYMBOL = Ctrl_CurrentReg__CurrentOffset_Iw
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-3.0 ... 3.0]
   @@ DESCRIPTION = "W phase current offset. [A]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 10
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = -0.3
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__CurrentOffset_Iw = 0.0f;

/*
   @@ SYMBOL = motor_control__DCLinkVoltage
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "DC Link voltage [V]"
   @@ GROUP = motor_control
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x5000
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T motor_control__DCLinkVoltage = 0.0f;

/*
   @@ SYMBOL = test__DCLinkVoltageMin
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 350.0]
   @@ DESCRIPTION = "DC Link voltage Min [V]"
   @@ GROUP = test
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x5006
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @@ END
 */
volatile real32_T test__DCLinkVoltageMin = 410.0f;

/*
   @@ SYMBOL = test__DCLinkVoltageMax
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 350.0]
   @@ DESCRIPTION = "DC Link voltage Max [V]"
   @@ GROUP = test
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x5006
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @@ END
 */
volatile real32_T test__DCLinkVoltageMax = 0.0f;

/*
   @@ SYMBOL = test__DCLinkVoltageAvg
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "DC Link voltage Avg [V]"
   @@ GROUP = test
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x5006
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 5
   @! WRITE_LEVEL = 5
   @@ END
 */
volatile real32_T test__DCLinkVoltageAvg = 0.0f;

/*
@! SYMBOL       = motor_control__bridge_temp
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Bridge temperature [ °C ]"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "°C"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 7
@! READ_LEVEL   = 4
@! END
*/
volatile float motor_control__bridge_temp = 0.0f;

/*
@! SYMBOL       = performance__foc_loop
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0.0 ... 125.0]
@! DESCRIPTION  = "FOC loop time duration [us]"
@! GROUP        = performance
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5001
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile float32_t performance__foc_loop = 0.0f;

/*
@! SYMBOL       = performance__velocity_loop
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0.0 ... 5000.0]
@! DESCRIPTION  = "Velocity loop time duration [us]"
@! GROUP        = performance
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5001
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! END
*/
volatile float32_t performance__velocity_loop = 0.0f;

/* parasoft-end-suppress EMSISO-GLOBAL NAMING-06 "Flow variable, not applicable." */
/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule for floats." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_step_current(void);
static __FOC_FAST_CODE__ void motor_control_hndl(void);
static __FOC_FAST_CODE__ void motor_control_trigger_velocity_loop(void);
static __FOC_FAST_CODE__ void motor_control_trigger_background_loop(void);
static __FOC_FAST_CODE__ void motor_control_set_phase_currents(motor_control_current_t const *currents);
static __FOC_FAST_CODE__ void motor_control_set_Udc(float32_t udc);
static __FOC_FAST_CODE__ void motor_control_get_stator_voltages(motor_control_duty_t *duty);
static __FOC_FAST_CODE__ void motor_control_set_runstop(bool run_stop);
static __FOC_FAST_CODE__ bool motor_control_get_runstop(void);

static __FOC_SLOW_CODE__ void motor_control_set_temperatures(motor_control_temperature_t const *temperature);
static __FOC_SLOW_CODE__ void motor_control_step_velocity(void);

__FOC_SLOW_CODE__ void motor_control_velocity_hndl(void);

static void motor_control_step_background(void);
static void motor_control_init_model(void);
void motor_control_callback_hndl(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   Execute fast FOC loop of motor control
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_step_current(void)
{
    emDriveFOC_control_step(0);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Execute slow velocity loop of motor control
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_SLOW_CODE__ void motor_control_step_velocity(void)
{
    emDriveFOC_control_step(1);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Execute slow background loop of motor control
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static void motor_control_step_background(void)
{
    emDriveFOC_control_step(2);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Functions triggers SW interrupt that handles velocity loop
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_trigger_velocity_loop(void)
{
    // Trigger SW interrupt for velocity loop execution
	exti_trigger_sw_isr();
}

// Disabled for now
#if 1
////////////////////////////////////////////////////////////////////////////////
/**
*   Functions triggers SW interrupt that handles background loop
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_trigger_background_loop(void)
{
    // Trigger SW interrupt for background loop execution
	exti_trigger_sw_background_isr();
}
#endif

__FOC_SLOW_CODE__ void motor_control_velocity_hndl(void)
{
    MOTOR_CONTROL_ASSERT(g_motor_control_initialized)

	performance_meas_start((uint32_t)VELOCITY_LOOP);

    motor_control_step_velocity();

    performance_meas_stop((uint32_t)VELOCITY_LOOP);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets measured phase currents for motor control
*
*@param[in] currents - pointer to structure containing phase currents
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_set_phase_currents(motor_control_current_t const *currents)
{
    emDriveFOC_control_U.SensorsTsc.Is_abc[0] = currents->phase_u;
    emDriveFOC_control_U.SensorsTsc.Is_abc[1] = currents->phase_v;
    emDriveFOC_control_U.SensorsTsc.Is_abc[2] = currents->phase_w;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets measured temperatures for motor control
*
*@param[in] temperature - pointer to structure containng temperatures of motor,
*           capacitor and bridge (ims).
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_SLOW_CODE__ void motor_control_set_temperatures(motor_control_temperature_t const *temperature)
{
	emDriveFOC_control_U.SensorsTsc.BridgeT   = temperature->bridge;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets DC link voltage to input for motor control
*
*@param[in] udc     DC link voltage in V
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_set_Udc(float32_t udc)
{
	static uint16_t i = 0;
	static float32_t DCLinkVoltageSum = 0.0f;

    emDriveFOC_control_U.SensorsTsc.Udc = udc;

    // update Udcmin and Udcmax
    if ( test__DCLinkVoltageMax < udc)
    {
    	test__DCLinkVoltageMax = udc;
    }

    if ( test__DCLinkVoltageMin > udc)
    {
    	test__DCLinkVoltageMin = udc;
    }

    if ( i >= 15999 )
    {
        i = 0;
        test__DCLinkVoltageAvg = DCLinkVoltageSum / 16000.0;
        DCLinkVoltageSum = 0.0;
    }
    else
    {
        i += 1;
        DCLinkVoltageSum += udc;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets number of drum turns for measurement
*
*@param[in] turns   Number of drum turns for measurement
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_measurement_turns(float32_t turns)
{
	emDriveFOC_control_U.Control.LoadNoOfTurns = turns;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets number of drum turns for measurement
*
*@param[in] turns   Number of drum turns for measurement
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_measurement_start(bool measure)
{
    emDriveFOC_control_U.Control.LoadMeasStart = measure;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets load speed amplitude for measurement
*
*@param[in] loadRPM_amplitude   Load speed amplitude for measurement
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_measurement_loadRPM_amplitude(float loadRPM_amplitude)
{
    emDriveFOC_control_U.Control.LoadRPM_amplitude = loadRPM_amplitude;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Enables or disables motor control model
*
*@param[in] run_stop    true to enable, false to disable
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_set_runstop(bool run_stop)
{
    emDriveFOC_control_U.Control.RunStop = run_stop;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Gets state of motor control model output runstop
*
*   This value indicated if we shall enable or disable PWm
*
*@return    true if bridge shall be enabled, false if not
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ bool motor_control_get_runstop(void)
{
    return (bool)emDriveFOC_control_Y.ActuatorsTsc.RunStop;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns current PWM values for all phases
*
*@param[in] duty - pointer to structure where phase PWM values are stored <0-1>
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_get_stator_voltages(motor_control_duty_t *duty)
{
    if( duty != NULL )
    {
        duty->phase_u = emDriveFOC_control_Y.ActuatorsTsc.PWM[0];
        duty->phase_v = emDriveFOC_control_Y.ActuatorsTsc.PWM[1];
        duty->phase_w = emDriveFOC_control_Y.ActuatorsTsc.PWM[2];
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Initialization of motor control model
*
*   This functions will initialize motor control model and its input
*   array with proper values.
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static void motor_control_init_model(void)
{
    // Initialize core motor control model
    emDriveFOC_control_initialize();

    // initial setup of input model values
    // Control inputs
    emDriveFOC_control_U.Control.RPM_ref = 0.0f;
    emDriveFOC_control_U.Control.RunStop = false;
    //emDriveFOC_control_U.Control.RTref    = 0.0f;

    // phase currents
    emDriveFOC_control_U.SensorsTsc.Is_abc[0] = 0.0f;
    emDriveFOC_control_U.SensorsTsc.Is_abc[1] = 0.0f;
    emDriveFOC_control_U.SensorsTsc.Is_abc[2] = 0.0f;

    // Position and velocity
    emDriveFOC_control_U.SensorsTsc.Pos.omega_r = 0.0f;
    emDriveFOC_control_U.SensorsTsc.Pos.theta_r = 0.0f;

    // DC link voltage
    emDriveFOC_control_U.SensorsTsc.Udc = 0.0f;

    emDriveFOC_control_U.Control.AscActive = false;
    emDriveFOC_control_U.SensorsTsc.BridgeT = 20.0f;
    emDriveFOC_control_U.Control.RPM_ramp = 1000.0f;

}

void motor_control_callback_hndl(void)
{
    MOTOR_CONTROL_ASSERT(g_motor_control_initialized)

    // Loop and call all callbacks
	const uint8_t cnt = g_motor_control_callbacks_cnt;
    for(uint8_t i = 0; i < cnt; i++)
    {
        if( g_motor_control_callbacks[i] != NULL )
        {
            g_motor_control_callbacks[i]();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Main function of motor control
*
*   This function shall be called every PWM cycle in ADC end of conversion interrupt.
*   Functions prepares the input values for both fast(FOC) and slow(Velocity) loop.
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
static __FOC_FAST_CODE__ void motor_control_hndl(void)
{
    static uint32_t velocity_loop_cnt = 0;
    static uint32_t background_loop_cnt = 0;
    uint16_t a, b, c;

    MOTOR_CONTROL_ASSERT(g_motor_control_initialized)

	// Prepare models input with new measured currents
	g_motor_control_data.current.phase_u = adc_analog_get()->current_u - g_motor_control_data.current_offset.phase_u;
	g_motor_control_data.current.phase_v = adc_analog_get()->current_v - g_motor_control_data.current_offset.phase_v;
	g_motor_control_data.current.phase_w = adc_analog_get()->current_w - g_motor_control_data.current_offset.phase_w;

	// Current reconstruction based on modulation type
	// In case if DPWM is used, current reconstruction is executed, otherwise all three
	// phase currents are measured
	if(motor_control_get_modulation_factor() > 0.5f)
	{
		timer1_get_cmp_val(&a, &b, &c);
		current_rec_execute(&g_motor_control_data.current, a, b, c);
	}

	motor_control_set_phase_currents(&g_motor_control_data.current);
	g_motor_control_data.dc_link = adc_analog_get()->vdc;
	motor_control__bridge_temp = adc_analog_get()->tso_igbt;

    motor_control_set_Udc(g_motor_control_data.dc_link);

    // Check protections
    protections_update_fast(&g_motor_control_data);
    // Update warnings
    warnings_update_fast(&g_motor_control_data);

    // Handle FSM and enable model if needed
    if( (motor_control_fsm_hndl(&g_motor_control_data) == eMOTOR_CONTROL_FSM_STATE_RUN)
     || (emDriveFOC_control_U.Control.AscActive == true) )
    {
       motor_control_set_runstop(true);
    }
    else
    {
       motor_control_set_runstop(false);
    }

    // Execute fast loop
    motor_control_step_current();

    // Enable/disable bridge depending on motor control model output
    if( true == motor_control_get_runstop() )
    {
        // Update bridge with newly calculated values
        motor_control_duty_t duty;
        motor_control_get_stator_voltages(&duty);

        timer1_set_pwms(duty.phase_u, duty.phase_v, duty.phase_w);

        timer1_enable_pwms();
    }
    else
    {
    	timer1_disable_pwms();

    	if( (motor_control_fsm_get_state() == eMOTOR_CONTROL_FSM_STATE_RUN)
    	 && (motor_control_is_break_enable() == false) )
    	{
    	    protections_set_fault(ePROTECTIONS_MOTOR_CONTROL_ERROR);
    	}
    }

    // Run also velocity loop if needed
    velocity_loop_cnt++;
    if( velocity_loop_cnt >= motor_control_get_velocity_downsample() )
    {
        // Prepare data for velocity loop
        background_loop_cnt++;

        g_motor_control_data.temperature.bridge = adc_analog_get()->tso_igbt;
        g_motor_control_data.temperature.motor  = motor_control_get_motor_temperature();

        motor_control_set_temperatures(&g_motor_control_data.temperature);

        // Check protections
        protections_update(&g_motor_control_data.temperature);

        // Trigger slow loop
        motor_control_trigger_velocity_loop();

        velocity_loop_cnt = 0;
        
        if(background_loop_cnt >= 1000)
        {
            // Trigger background loop
            motor_control_trigger_background_loop();
          
            background_loop_cnt = 0;
        }
    }

    // Handle registered callbacks
    motor_control_callback_hndl();
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup MOTOR_CONTROL_API
* @{ <!-- BEGIN GROUP -->
*
* Following module is part of API, which is available to the user of Motor Control Module
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   Initializes Motor Control module
*
* @return   Returns status of initialization of type motor_control_status_t
*/
////////////////////////////////////////////////////////////////////////////////
motor_control_status_t motor_control_init(void)
{
    motor_control_status_t status = eMOTOR_CONTROL_OK;

    if( g_motor_control_initialized == false )
    {
    	// GPIO initialization
    	if(gpio_init() != HAL_OK)
    	{
    		status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
    	}
        // Perform initialization of required peripherals
		else if( dac_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( adc_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( opamp_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( timer_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( dma_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( comparator_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		else if( exti_init() != HAL_OK )
		{
			status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
		}
		/*else if( watchdog_init() != HAL_OK )
    	{
    		status = eMOTOR_CONTROL_PERIPH_INIT_ERROR;
    	}*/
		else
		{
			// Do nothing
		}

        if(status != eMOTOR_CONTROL_OK )
        {
            MOTOR_CONTROL_PRINTF("MOTOR_CONTROL: Peripheral initialization failed!\n")
            MOTOR_CONTROL_ASSERT(0);
        }

        // Initialize callback function for 1 ms task
        exti_set_slow_loop_callback(motor_control_velocity_hndl);
        // Initialize callback function for 1 s task
        exti_set_background_loop_callback(motor_control_background_hndl);

        // Initialize motor control model
        motor_control_init_model();

        // Initialize motor control state machine
        motor_control_fsm_init();

        g_motor_control_initialized = true;
    }

    // If initializations is success
    if( status == eMOTOR_CONTROL_OK )
    {
        // Call background loop first time here to make sure that gains are calculated before
        // model is executed for the first time
        motor_control_background_hndl();

        // Register callback which will call FOC loop
		adc_set_callback(eADC_CALLBACK_INJECTED_CHANNELS_DONE, &motor_control_hndl);

		// Test
		adc_start();
    }

	return status;
}


////////////////////////////////////////////////////////////////////////////////
/**
*   Registers FOC callback
*
*   Registers funtion to be called each FOC cycle. Maximum number of callback*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_register_callback(motor_control_callback_t callback)
{
    if( g_motor_control_callbacks_cnt < MOTOR_CONTROL_MAX_CALLBACKS )
    {
        g_motor_control_callbacks[g_motor_control_callbacks_cnt] = callback;
        g_motor_control_callbacks_cnt++;
    }
    else
    {
        MOTOR_CONTROL_ASSERT(0);
    }
}

void motor_control_background_hndl(void)
{
    MOTOR_CONTROL_ASSERT(g_motor_control_initialized)

    motor_control_step_background();

    // Update HW over current and over voltage  protection - shall be removed in final application
    dac_protection_limit_update();

#if (PERFORMANCE_MEAS_EN == 1U)
    // Calculate function time duration for debug purpose
    performance__foc_loop = performance_meas_calc_time((uint32_t)FAST_LOOP_EXECUTION);
    performance__velocity_loop = performance_meas_calc_time((uint32_t)VELOCITY_LOOP);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Enable motor control
*
*   This function will enable both motor control model and bridge
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_enable(void)
{
    motor_control_fsm_set_state(eMOTOR_CONTROL_FSM_STATE_RUN);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Disable motor control
*
*   This function will disable both motor control model and bridge
*
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_disable(void)
{
    motor_control_fsm_set_state(eMOTOR_CONTROL_FSM_STATE_OFF);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets requested speed for motor control when in speed control mode
*
*@param[in] speed requested speed in RPM
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_rpm(float32_t speed)
{
	emDriveFOC_control_U.Control.RPM_ref = speed;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets requested ramp for motor control when in speed control mode
*
*@param[in] ramp requested ramp in RPM
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_ramp(float32_t ramp)
{
	emDriveFOC_control_U.Control.RPM_ramp = ramp;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Get requested ramp for motor control when in speed control mode
*
*@return    Ramp
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_ramp(void)
{
	return emDriveFOC_control_U.Control.RPM_ramp;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Sets the offsets of phase measurements
*
*   This functions shall be called in motor control INIT state to calibrate
*   current sensors offsets
*
*@param[in] current_offsets - current offsets
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_set_current_offset(const motor_control_current_t *const current_offsets)
{
    g_motor_control_data.current_offset.phase_u = current_offsets->phase_u;
    g_motor_control_data.current_offset.phase_v = current_offsets->phase_v;
    g_motor_control_data.current_offset.phase_w = current_offsets->phase_w;

    // update CAN objects
    Ctrl_CurrentReg__CurrentOffset_Iu = current_offsets->phase_u;
    Ctrl_CurrentReg__CurrentOffset_Iv = current_offsets->phase_v;
    Ctrl_CurrentReg__CurrentOffset_Iw = current_offsets->phase_w;
}


////////////////////////////////////////////////////////////////////////////////
/**
*   Get the offsets of phase measurements
*
*   This functions is called during discharge
*
*@param[in] current_offsets - current offsets
*@return    void
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_get_current_offset(motor_control_current_t *const current_offsets)
{
    current_offsets->phase_u = g_motor_control_data.current_offset.phase_u;
    current_offsets->phase_v = g_motor_control_data.current_offset.phase_v;
    current_offsets->phase_w = g_motor_control_data.current_offset.phase_w;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns number of motor pole pairs
*
*@return    number of motor pole pairs
*/
////////////////////////////////////////////////////////////////////////////////
int32_t motor_control_get_pole_pairs(void)
{
	return (int32_t)Ctrl_Motor__Np;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns ratio between current and velocity loops
*
*@return    ratio
*/
////////////////////////////////////////////////////////////////////////////////
int32_t motor_control_get_velocity_downsample(void)
{
	return (int32_t)Ctrl_Gen__CurToVelRatio;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns FOC frequency
*
*@return    float32_t returns foc frequency in Hz.
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_frequency(void)
{
    return Ctrl_Gen__Fcur;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns Phase RMS Voltage
*
*@return    float32_t returns phase RMS voltage in V
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_phase_voltage(void)
{
	return emDriveFOC_control_Y.DataTsc.Uph;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns Phase RMS Current
*
*@return    float32_t returns phase RMS current in A
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_phase_current(void)
{
	return emDriveFOC_control_Y.DataTsc.Iph;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns Motor Power
*
*@return    float32_t return motor power in W
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_motor_power(void)
{
	return emDriveFOC_control_Y.DataTsc.Pel;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns motor RPM
*
*@return    float32_t returns motor speed in RPM
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_motor_RPM(void)
{
	return emDriveFOC_control_Y.DataTsc.RPM;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns run-stop state of FOC
*
*@return    int32_t - 0 if stopped, 1 if enabled
*/
////////////////////////////////////////////////////////////////////////////////
int32_t motor_control_get_run_state(void)
{
	return (int32_t)emDriveFOC_control_Y.ActuatorsTsc.RunStop;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns DC torque
*
*@return    float32_t returns DC component of torque in Nm
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_dc_torque(void)
{
	// return emDriveFOC_control_Y.DataTsc.LoadDC_Torque;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns 1st harmonic torque
*
*@return    float32_t returns 1st harmonic component of torque in Nm
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_harmonic_torque(void)
{
	// return emDriveFOC_control_Y.DataTsc.LoadFirstHarmTorque;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns overload value in measurement
*
*@return    float32_t return value 0-1 of overload percentage
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_overload(void)
{
    // return emDriveFOC_control_Y.DataTsc.OverLoad_Precentage;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns friction value in measurement
*
*@return    float32_t returns friction value in Nm
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_friction()
{
    // return emDriveFOC_control_Y.DataTsc.LoadDC_TorquePositive;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns AU ball in measurement
*
*@return    float32_t return of AU ball measurement in Nm
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_auball()
{
    // return emDriveFOC_control_Y.DataTsc.AU_ball_HPF;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Returns flag of measurement finished
*
*@return    bool returns true if measurement is done
*/
////////////////////////////////////////////////////////////////////////////////
bool motor_control_is_measurement_done(void)
{
	// return (bool)emDriveFOC_control_Y.DataTsc.LoadMeasFinished;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    	Function returns modulation factor
*
* @param[in]    None
* @return       Ctrl_CurrentReg_StatusModFactor
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_modulation_factor(void)
{
	return Ctrl_CurrentReg_StatusModFactor;
}

////////////////////////////////////////////////////////////////////////////////
/*!
*   Function returns state of power limit
*
* @return       motor_control_power_limit_state
*/
////////////////////////////////////////////////////////////////////////////////
motor_control_power_limit_state motor_control_get_power_limit_state(void)
{
    return (motor_control_power_limit_state)Ctrl_PowerLimit__State;
}

////////////////////////////////////////////////////////////////////////////////
/*!
*   Returns estimated motor temperature
*
* @return float32_t - Estimated motor temperature in °C
*/
////////////////////////////////////////////////////////////////////////////////
float32_t motor_control_get_motor_temperature(void)
{
    return Ctrl_Gen_Stat__MotorWireTemp;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    	Function enables or disables break functionality (motor SC).
* 				If true, motor break is enabled, otherwise it is disabled.
*
* @param[in]    enable Enable or disable motor break
* @return       none
*/
////////////////////////////////////////////////////////////////////////////////
void motor_control_break_enable(const bool enable)
{
	emDriveFOC_control_U.Control.AscActive = enable;
}

////////////////////////////////////////////////////////////////////////////////
/*!
*   Function return state of MC ASC mode
*
* @return bool - true of active
*/
////////////////////////////////////////////////////////////////////////////////
bool motor_control_is_break_enable(void)
{
    return emDriveFOC_control_U.Control.AscActive;
}

float32_t motor_control_get_udc(void)
{
    return g_motor_control_data.dc_link;
}




////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
