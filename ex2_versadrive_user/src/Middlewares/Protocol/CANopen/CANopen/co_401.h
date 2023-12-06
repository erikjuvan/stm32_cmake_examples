////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_401.h
*@brief  contains defines for ciA 401 profile implementation
*@author Zdenko Mezgec
*@author EMTAS
*@date 03.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef CO_401_H
#define CO_401_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Profile offset, multiple of 0x800
 * Set by the CANopen Device Designer */
#if !defined(CO_401_PROFILE_OFFSET)
# define  CO_401_PROFILE_OFFSET 0u
#endif

/* Makes it sense to let the design tool specify the number of used
 * process IO, to reserve only needed resources?
 * */
#define CO_401_NDIGIN8	8		/* 8x8 = 64 bits */
#define CO_401_NDIGOUT8	8		/* 8x8 = 64 bits */
#define CO_401_NANIN16	8
#define CO_401_NANOUT16	8

/* Number of possible mappings in RPDOs
 * ! Objects can be mapped more than one time !*/
#define MAPPINGTABLE_MAX (		\
		(CO_401_NDIGOUT8 * 2)	\
		+ (CO_401_NANOUT16 * 1))

/* HW API functions */

/* digout */
typedef void (* CO_EVENT_401_DO_T)(UNSIGNED8, UNSIGNED16);
/* anout */
typedef void (* CO_EVENT_401_AO_T)(UNSIGNED8, INTEGER16);
/* digin */
typedef UNSIGNED8 (* CO_EVENT_401_DI_T)(UNSIGNED8, UNSIGNED8);
/* anin */
typedef INTEGER16 (* CO_EVENT_401_AI_T)(UNSIGNED8);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
RET_T coEventRegister_401(CO_EVENT_401_DI_T, CO_EVENT_401_DO_T,CO_EVENT_401_AI_T, CO_EVENT_401_AO_T);
RET_T co401Init(void);
void  co401Task(void);
void  co401DigOutDef(void);
void  co401DigOutErr(void);
void  co401AnOutDef(void);
void  co401AnOutErr(void);

#endif /*  CO_401_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
