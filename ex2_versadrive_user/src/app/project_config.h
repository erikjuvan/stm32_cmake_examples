////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/*!
 * @file     project_config.h
 * @author   Jure Vreca
 * @date     23.04.2020
 * @brief    Configuration of the weber project.
 */

#ifndef __PROJECT_CONFIG_H
#define __PROJECT_CONFIG_H

typedef float float32_t;

////////////////////////////////////////////////////////////////////////////////
// Project Debug settings
////////////////////////////////////////////////////////////////////////////////

// DEBUG
#ifdef DEBUG_MODE
#define PROJECT_DEBUG_ENABLED (0)
// RELEASE
#else
#define PROJECT_DEBUG_ENABLED (0)
#endif

// Assert behavior in release
#if (0 == PROJECT_DEBUG_ENABLED)
#define PROJECT_RELEASE_ASSERT_BEHAVIOUR                                                     \
    {                                                                                        \
        ;                                                                                    \
    } /* parasoft-suppress FORMAT-03 "Ignore for defines." */ /* parasoft-suppress FORMAT-02 \
                                                                 "Ignore for defines." */
#endif

#define MOTOR_BRIDGE_USE_GTM         (0)
#define MOTOR_BRIDGE_CHARGE_PUMP_PWM (0)
#define MOTOR_BRIDGE_USE_TIM1        (1)

#define HISENSE_CPR

#define SYSTEM_CORE_CLOCK_HZ (170000000ULL)

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#endif // __PROJECT_CONFIG_H
