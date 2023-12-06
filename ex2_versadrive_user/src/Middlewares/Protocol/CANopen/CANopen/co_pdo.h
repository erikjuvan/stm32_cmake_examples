////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_pdo.h
*@brief  contains defines for pdo services
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

#ifndef CO_PDO_H
#define CO_PDO_H 1

# if defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT)

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
* PDO transmit mapping entry (one mapping entry)
*/
typedef struct {
	CO_CONST void	*pVar;	/**< pointer to variable */
	UNSIGNED8	len;		/**< number of bytes for variable */
	BOOL_T		numeric;	/**< numeric flag for byte swapping */
	UNSIGNED32	val;		/**< OD value */
} PDO_TR_MAP_ENTRY_T;

/**
* PDO receive mapping entry (one mapping entry)
*/
typedef struct {
	void		*pVar;		/**< pointer to variable */
	UNSIGNED8	len;		/**< number of bytes for variable */
	BOOL_T		numeric;	/**< numeric flag for byte swapping */
	UNSIGNED32	val;		/**< OD value */
} PDO_REC_MAP_ENTRY_T;

/**
* PDO mapping table (mapping entries for one transmit PDO) */
typedef struct {
	UNSIGNED8		    mapCnt;		                    /**< number of mapping entries */
	PDO_TR_MAP_ENTRY_T	mapEntry[CO_MAX_MAP_ENTRIES];   /**< Mapping entries */
} PDO_TR_MAP_TABLE_T;

/**
* PDO mapping table (mapping entries for one receive PDO) */
typedef struct {
	UNSIGNED8		    mapCnt;		                    /**< number of mapping entries */
	PDO_REC_MAP_ENTRY_T	mapEntry[CO_MAX_MAP_ENTRIES];   /**< Mapping entries */
} PDO_REC_MAP_TABLE_T;


/** \brief function pointer to PDO indication
 * \param pdoNr - PDO number
 * 
 * \return void
 */
typedef void (* CO_EVENT_PDO_T)(UNSIGNED16); /*lint !e960 customer specific parameter names */


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coPdoTransmitInit(UNSIGNED16 pdoNr, UNSIGNED8	transType,UNSIGNED16 inhibit, UNSIGNED16	eventTime, UNSIGNED8 syncStartVal,const PDO_TR_MAP_TABLE_T *mapTable);
RET_T coPdoReceiveInit(UNSIGNED16 pdoNr, UNSIGNED8 transType,UNSIGNED16 inhibit, UNSIGNED16 eventTime,const PDO_REC_MAP_TABLE_T *mapTable);

RET_T coPdoReqNr(UNSIGNED16 pdoNr, UNSIGNED8 flags);
RET_T coPdoReqObj(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED8 flags);

BOOL_T coPdoObjIsMapped(UNSIGNED16 pdoNr, UNSIGNED16 index, UNSIGNED8 subIndex);

RET_T coEventRegister_PDO(CO_EVENT_PDO_T pFunction);
RET_T coEventRegister_PDO_SYNC(CO_EVENT_PDO_T pFunction);
RET_T coEventRegister_PDO_REC_EVENT(CO_EVENT_PDO_T pFunction);

# endif /* defined(CO_PDO_TRANSMIT_CNT) || defined(CO_PDO_RECEIVE_CNT) */
#endif /* CO_PDO_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

