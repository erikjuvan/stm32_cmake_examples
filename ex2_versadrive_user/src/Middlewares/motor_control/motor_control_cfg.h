////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
* @file     motor_control_cfg.h
* @brief    Motor Control configuration file
* @author   Marko Rman
* @date     05.01.2021
* @version  V0.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/*!
* @addtogroup MOTOR_CONTROL_CONFIGURATION
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MOTOR_CONTROL_CFG_H
#define __MOTOR_CONTROL_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Maximum number of possible callbacks that can be registered
 */
#define MOTOR_CONTROL_MAX_CALLBACKS             ( 3 )

/**
 * Number of samples for current offset calibration
 */
#define MOTOR_CONTORL_CURRENT_OFFSET_SAMPLES    ( 2000 )

////////////////////////////////////////////////////////////////////////////////
// Protections
////////////////////////////////////////////////////////////////////////////////

/**
 * Maximum Current offset measured at power up
 */
#define MOTOR_CONTROL_MAX_CURRENT_OFFSET      (float) ( 0.75f )


////////////////////////////////////////////////////////////////////////////////
// Debug settings
////////////////////////////////////////////////////////////////////////////////

/**
 * Defines if ASSERTs are enabled or not
 */
#define MOTOR_CONTROL_ASSERT_ENABLE         ( 0 )

/**
 * Define if PRINTF is enabled or not.
 * Shell module is required for use of this.
 */
#define MOTOR_CONTROL_DEBUG_SHELL_ENABLE    ( 0 )

/* parasoft-begin-suppress FORMAT-34 FORMAT-02 FORMAT-03 "Allow in define" */
#if (MOTOR_CONTROL_DEBUG_SHELL_ENABLE == 1)
    #define MOTOR_CONTROL_PRINTF(...)               sprintf(can_buff_msg_out, (const char*) __VA_ARGS__); shell_write(can_buff_msg_out)
#else
    #define MOTOR_CONTROL_PRINTF(...)               {;}
#endif

#if (MOTOR_CONTROL_ASSERT_ENABLE == 1)
    #define MOTOR_CONTROL_ASSERT(x)                 if (!(x))   { MOTOR_CONTROL_PRINTF("MOTOR_CONTROL ERROR: Assertion failed in file: %s, line: %d\n", __FILE__, __LINE__); while(1){} }
#else
    #define MOTOR_CONTROL_ASSERT(x)                 {;}
#endif
/* parasoft-end-suppress FORMAT-34 FORMAT-02 FORMAT-03 "Allow in define" */

#endif // __MOTOR_CONTROL_CFG_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
