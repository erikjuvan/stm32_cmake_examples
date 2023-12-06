////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_sleep.h
*@brief  contains defines for sleep services
*@author Zdenko Mezgec
*@author EMTAS
*@date   03.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef CO_SLEEP_H
#define CO_SLEEP_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
* SLEEP states 
*/
typedef enum {
	CO_SLEEP_MODE_CHECK,		/**< check if sleep mode is possible */
	CO_SLEEP_MODE_OBJECTION,	/**< slave has send an objection */
	CO_SLEEP_MODE_PREPARE,		/**< automatic start sleep mode */
	CO_SLEEP_MODE_SILENT,		/**< sleep mode silent */
	CO_SLEEP_MODE_DOZE,			/**< sleep mode doze */
	CO_SLEEP_MODE_REQUEST_SLEEP	/**< sleep mode reuqest sleep */
} CO_SLEEP_MODE_T;

/** \brief function pointer to sleep event function 
 * \param sleep mode - node Id
 * 
 * \retval 0 - ok
 * \retval !=0 - error
 */
typedef UNSIGNED8 (* CO_EVENT_SLEEP_T)(CO_SLEEP_MODE_T); /*lint !e960 customer specific parameter names */

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	coSleepModeStart(UNSIGNED16 waitTime);
RET_T	coEventRegister_SLEEP(CO_EVENT_SLEEP_T pFunction);
BOOL_T	coSleepModeActive(void);
void	coSleepModeCheck(UNSIGNED16	waitTime);
void	coSleepAwake(BOOL_T master,	UNSIGNED8 status, UNSIGNED8	reason,UNSIGNED16 repeatTime);
void	coSleepRequestSleep(void);

#endif /* CO_SLEEP_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////