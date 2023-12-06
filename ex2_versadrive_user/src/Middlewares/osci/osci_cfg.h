////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci_cfg.h
*@brief     Defines for customising OSCI
*@author    Zdenko Mezgec
*@date      10.12.2020
*@version   V1.2.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup OSCI_CONFIGURATION
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __OSCI_CFG_H
#define __OSCI_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
*  If defined it means we use the freertos RTOS.
*/
#undef OSCI_FREERTOS

/**
*  If defined it means the module uses the KEIL RTX RTOS.
*/
#undef OSCI_KEIL_RTX

/**
 *  Max number of channels,objects to use in osci
 */
#define OSCI_MAX_CHANNEL_COUNT              20

/**
 *  Support for osci canopen connection; currently only this option is allowed
 */
#define OSCI_CANOPEN_INTERFACE              1

/**
 * Support for registering osci_hndl as callback from FOC
 */
#define OSCI_ENABLE_MOTOR_CONTROL_CALLBACK  ( 1 )

/**
 *  reserved NR of samples in RAM. actual size in B is 4 x NR
 */
#define OSCI_MEMORY_SIZE                    10000

/**
 *  enable triggering of simulation osci signals; to demonstrate that everything works properly without additional objects
 */
#define OSCI_SIMULATE_SIGNALS               0

////////////////////////////////////////////////////////////////////////////////
// Debug settings
////////////////////////////////////////////////////////////////////////////////
/* parasoft-begin-suppress FORMAT-02 FORMAT-03 FORMAT-34 "Allow in define" */
#define CO_DEBUG_MODE  PROJECT_DEBUG_ENABLED
#if CO_DEBUG_MODE == 1
	#define CO_ASSERT(x) if (!(x)) 	{ while(1); }
#else
    #define CO_ASSERT(x)            {;}
#endif
/* parasoft-end-suppress FORMAT-02 FORMAT-03 FORMAT-34 "Allow in define" */

#endif // __OSCI_CFG_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

