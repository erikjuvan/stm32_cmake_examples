////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   ico_cobhandler.h
*@brief  contains internal defines for cob handling
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

#ifndef ICO_COB_HANDLER_H
#define ICO_COB_HANDLER_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum {
	CO_SERVICE_SDO_SERVER, CO_SERVICE_SDO_CLIENT,
	CO_SERVICE_NMT, CO_SERVICE_ERRCTRL,
	CO_SERVICE_PDO_TRANSMIT, CO_SERVICE_PDO_RECEIVE,
	CO_SERVICE_SYNC_TRANSMIT, CO_SERVICE_SYNC_RECEIVE,
	CO_SERVICE_EMCY_TRANSMIT, CO_SERVICE_EMCY_RECEIVE,
	CO_SERVICE_TIME_TRANSMIT, CO_SERVICE_TIME_RECEIVE,
	CO_SERVICE_LSS_TRANSMIT, CO_SERVICE_LSS_RECEIVE,
	CO_SERVICE_SLEEP,
	CO_SERVICE_CAN_DEBUG
} CO_SERVICE_T;

typedef UNSIGNED16	COB_REFERENZ_T;

typedef enum {
	CO_COB_TYPE_TRANSMIT, CO_COB_TYPE_RECEIVE
}	CO_COB_TYPE_T;


typedef enum {
	CO_COB_RTR_NONE, CO_COB_RTR
} CO_COB_RTR_T;


typedef struct {
	COB_REFERENZ_T	cobNr;			/* cob number */
	CO_COB_TYPE_T	type;			/* cob type */
	CO_CAN_COB_T	canCob;			/* can cob data */
	CO_SERVICE_T	service;		/* service */
	UNSIGNED16		serviceNr;		/* service number */
	UNSIGNED16		inhibit;		/* inhibit time */
	UNSIGNED8		len;			/* msg len */
	CO_TIMER_T		inhibitTimer;	/* inhibit timer */
} CO_COB_T;

#define CO_COB_INVALID		0x80000000UL
#define CO_COB_29BIT		0x20000000UL
#define CO_COB_VALID_MASK	0x80000000UL
#define CO_COB_29BIT_MASK	0x20000000UL
#define CO_COB_ID_MASK		0x1FFFFFFFUL


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
COB_REFERENZ_T	icoCobCreate(CO_COB_TYPE_T cobType, CO_SERVICE_T service,UNSIGNED16 serviceNr);
RET_T			icoCobSet(COB_REFERENZ_T cobRef, UNSIGNED32	 cobId,CO_COB_RTR_T rtr, UNSIGNED8	len);
UNSIGNED32		icoCobGet(COB_REFERENZ_T cobRef);
RET_T			icoCobEnable(COB_REFERENZ_T cobRef);
RET_T			icoCobDisable(COB_REFERENZ_T cobRef);
CO_COB_T		*icoCobCheck(const CO_CAN_COB_T *pCanCob);
CO_COB_T		*icoCobGetPointer(COB_REFERENZ_T cobRef);
RET_T			icoCobChangeType(COB_REFERENZ_T	cobRef, CO_COB_TYPE_T cobType);
RET_T			icoCobSetInhibit(COB_REFERENZ_T	cobRef, UNSIGNED16 inhibit);
RET_T			icoCobSetLen(COB_REFERENZ_T	cobRef,	UNSIGNED8 len);
void			icoCobDisableAll(void);
BOOL_T			icoCheckRestrictedCobs(UNSIGNED32 canId,UNSIGNED32	exceptFirst, UNSIGNED32	exceptLast);
UNSIGNED8		icoGetCanMsgLen(UNSIGNED8 dlc);
UNSIGNED8		icoGetCanTransferLen(UNSIGNED8 dlc);

#endif /* ICO_COB_HANDLER_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
