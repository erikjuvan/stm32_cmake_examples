////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_timer.h
*@brief  contains defines for timer
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
#ifndef CO_TIMER_H
#define CO_TIMER_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/**
* timer attributes
*/
typedef enum {
	CO_TIMER_ATTR_ROUNDUP,			/**< round up given timer value */
	CO_TIMER_ATTR_ROUNDUP_CYCLIC,	/**< round up and start timer again */
	CO_TIMER_ATTR_ROUNDDOWN,		/**< round down given timer value */
	CO_TIMER_ATTR_ROUNDDOWN_CYCLIC	/**< round down and start timer again */
} CO_TIMER_ATTR_T;

/** \brief function pointer to Timer indication
 * \param pFct - pointer to timer up function
 * 
 * \return void
 */
typedef void (* CO_TIMER_FCT_T)(void *); /*lint !e960 customer specific parameter names */

/**
* timer structure 
*/
typedef struct co_timer {
	struct co_timer	*pNext;			/**< pointer to next timer */
	UNSIGNED32		actTicks;		/**< actual timer ticks */
	UNSIGNED32		ticks;			/**< calculated timer ticks */
	CO_TIMER_FCT_T	pFct;			/**< pointer to own function */
	void			*pData;			/**< pointer for own data */
	CO_TIMER_ATTR_T	attr;			/**< timer attributes */
} xTimer;
typedef struct co_timer	CO_TIMER_T;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void	coTimerInit(UNSIGNED32	timerVal);
RET_T	coTimerStart(CO_TIMER_T *pTimer, UNSIGNED32 timerTime,CO_TIMER_FCT_T pFct, void *pData, CO_TIMER_ATTR_T timerAttributes);
RET_T	coTimerStop(CO_CONST CO_TIMER_T *pTimer);
BOOL_T	coTimerIsActive(CO_CONST CO_TIMER_T *pTimer);
void	coTimerTick(void);
void	coTimerAttrChange(CO_TIMER_T *pTimer, CO_TIMER_ATTR_T timerAttributes);

#endif /* CO_TIMER_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
