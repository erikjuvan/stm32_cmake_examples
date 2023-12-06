////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_drv.h
*@brief  contains defines for driver
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
#ifndef CO_DRV_H
#define CO_DRV_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#ifdef CO_CANFD_SUPPORTED
# define CO_CAN_MAX_DATA_LEN	64u
# define CO_CAN_FD_MAX_DLC		15u
#else /* CO_CANFD_SUPPORTED*/
# define CO_CAN_MAX_DATA_LEN	8u
#endif /* CO_CANFD_SUPPORTED*/

typedef	void *		LIBDRV_HANDLE_T;

/** CAN cob structure */
typedef struct {
	UNSIGNED32			canId;			/**< can identifier */
	UNSIGNED16			canChan;		/**< reserved for driver */
	BOOL_T				extended;		/**< extended id  */
	BOOL_T				rtr;			/**< rtr */
	BOOL_T				transmit;		/**< transmit/receive */
	BOOL_T				enabled;		/**< cob enabled/disabled */
} CO_CAN_COB_T;


/** CAN message structure */
typedef struct {
	LIBDRV_HANDLE_T		handle;			/**< library internal handle */
	CO_CAN_COB_T		canCob;			/**< cob infos */
	UNSIGNED8			len;			/**< msg len */
	UNSIGNED8			data[CO_CAN_MAX_DATA_LEN];	/**< data */
} CO_CAN_MSG_T;



/** bittiming table entries */
typedef struct {
	UNSIGNED16 bitRate; 	/**< bitrate in kbit/s */
	UNSIGNED16 pre; 		/**< prescaler */
	UNSIGNED8 prop;			/**< propagation segment */
	UNSIGNED8 seg1; 		/**< segment 1 */
	UNSIGNED8 seg2; 		/**< segment 2 */
} CODRV_BTR_T;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void codrvWaitForEvent(UNSIGNED32	msecTimeout);
void codrvWaitForCanEvent(UNSIGNED32	waitTimeOut);

RET_T codrvCanInit(UNSIGNED16 bitRate);
RET_T codrvCanStartTransmission(void);
void  codrvHardwareInit(void);

CO_CAN_MSG_T *coQueueGetReceiveBuffer(void);
void	coQueueReceiveBufferIsFilled(void);
void	coQueueMsgTransmitted(const CO_CAN_MSG_T *pBuf);
CO_CAN_MSG_T *coQueueGetNextTransmitMessage(void);

void codrvCanEnableInterrupt(void);
void codrvCanDisableInterrupt(void);

RET_T codrvCanSetFilter(CO_CAN_COB_T *pCanCob);

RET_T codrvTimerSetup(UNSIGNED32 timerInterval);

#endif /* CO_DRV_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

