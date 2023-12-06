////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_commtask.h
*@brief  contains defines for canopen services
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
#ifndef CO_COM_TASK_H
#define CO_COM_TASK_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
* CAN states
*/
typedef enum {
	CO_CAN_STATE_BUS_OFF,			/**< CAN bus state is bus off */
	CO_CAN_STATE_BUS_ON,			/**< CAN bus state is bus on */
	CO_CAN_STATE_PASSIVE,			/**< CAN bus state is passive */
	CO_CAN_STATE_UNCHANGED			/**< CAN bus state is unchanged */
} CO_CAN_STATE_T;

/**
* Communication state events
*/
typedef enum {
	CO_COMM_STATE_EVENT_NONE,				/**< no event */
	CO_COMM_STATE_EVENT_BUS_OFF,			/**< bus off */
	CO_COMM_STATE_EVENT_BUS_OFF_RECOVERY,	/**< recvovery from bus off */
	CO_COMM_STATE_EVENT_BUS_ON,				/**< bus on */
	CO_COMM_STATE_EVENT_PASSIVE,			/**< can passive */
	CO_COMM_STATE_EVENT_ACTIVE,				/**< can active */
	CO_COMM_STATE_EVENT_CAN_OVERRUN,		/**< can overrun */
	CO_COMM_STATE_EVENT_REC_QUEUE_FULL,		/**< receice queue full */
	CO_COMM_STATE_EVENT_REC_QUEUE_OVERFLOW,	/**< receive queue overflow */
	CO_COMM_STATE_EVENT_REC_QUEUE_EMPTY,	/**< receice queue empty */
	CO_COMM_STATE_EVENT_TR_QUEUE_FULL,		/**< transmit queue full */
	CO_COMM_STATE_EVENT_TR_QUEUE_OVERFLOW,	/**< transmit queue overflow */
	CO_COMM_STATE_EVENT_TR_QUEUE_EMPTY		/**< transmit queue emty */
} CO_COMM_STATE_EVENT_T;

/**
* Communication task events
*/
typedef enum {
	CO_COMMTASK_EVENT_TIMER,
	CO_COMMTASK_EVENT_MSG_AVAIL,
	CO_COMMTASK_EVENT_NEW_EVENT
} CO_COMMTASK_EVENT_T;


/** \brief function pointer to CAN state indication function 
 * \param canState - new CAN state
 * 
 * \return void
 */
typedef void (* CO_EVENT_CAN_STATE_T)(CO_CAN_STATE_T);	/*lint !e960 Function pointer without parameter names */


/** \brief function pointer to Communication state event indication function 
 * \param commState - new communication state
 * 
 * \return void
 */
typedef void (* CO_EVENT_COMM_T)(CO_COMM_STATE_EVENT_T);  /*lint !e960 Function pointer without parameter names */


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void coCommTask(void);
void coCommStateEvent(CO_COMM_STATE_EVENT_T	newEvent);
RET_T coEventRegister_COMM_EVENT(CO_EVENT_COMM_T pFunction);
RET_T coEventRegister_CAN_STATE(CO_EVENT_CAN_STATE_T pFunction);
CO_INLINE void  coCommTaskSet(CO_COMMTASK_EVENT_T event);
void	        coQueueInit(void);

#endif /* CO_COM_TASK_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
