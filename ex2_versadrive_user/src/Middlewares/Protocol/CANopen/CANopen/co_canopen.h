////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_canopen.h
*@brief  contains defines for all services
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

#ifndef CO_CANOPEN_H
#define CO_CANOPEN_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

#if !defined(DEBUG)
#include "gen_objdict_variables.h"
#include "gen_define.h"

#else // CO_FULL_MODE
#include "co_commtask.h"
#include "co_drv.h"
#include "co_emcy.h"
#include "co_led.h"
#include "co_lss.h"
#include "co_nmt.h"
#include "co_odaccess.h"
#include "co_pdo.h"
#include "co_sdo.h"
#include "co_store.h"
#include "co_sleep.h"
#include "co_sync.h"
#include "co_time.h"
#include "co_timer.h"
#include "canopen.h"
#include "gen_objdict_variables.h"

#endif // CO_MODE

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* actual CANopen stack software version */
#define CO_STACK_VERSION	0x10700ul

#if !defined(DEBUG)
#define CO_PDO_MAX_DATA_LEN (8u)
//#define CO_MAX_MAP_ENTRIES (8u)
#define MAX_SAVE_DATA (4u)

/**
* object attributes
*/
#define CO_ATTR_READ		0x0001u	/**< object readable */
#define CO_ATTR_WRITE		0x0002u	/**< object writeable */
#define CO_ATTR_NUM			0x0004u	/**< object is numeric */
#define CO_ATTR_MAP			0x0008u	/**< object can be mapped into a PDO */
#define CO_ATTR_MAP_TR		0x0010u	/**< object can be mapped into a transmit PDO */
#define CO_ATTR_MAP_REC		0x0020u	/**< object can be mapped into a receive PDO */
#define CO_ATTR_DEFVAL		0x0040u	/**< object has a default value */
#define CO_ATTR_LIMIT		0x0080u	/**< object has limits */



/**
* object datatypes
*/
typedef enum {
	CO_DTYPE_U8_VAR,	CO_DTYPE_U16_VAR,	CO_DTYPE_U32_VAR,
	CO_DTYPE_U8_CONST,	CO_DTYPE_U16_CONST,	CO_DTYPE_U32_CONST,
	CO_DTYPE_U8_PTR,	CO_DTYPE_U16_PTR,	CO_DTYPE_U32_PTR,
	CO_DTYPE_I8_VAR,	CO_DTYPE_I16_VAR,	CO_DTYPE_I32_VAR,
	CO_DTYPE_I8_CONST,	CO_DTYPE_I16_CONST,	CO_DTYPE_I32_CONST,
	CO_DTYPE_I8_PTR,	CO_DTYPE_I16_PTR,	CO_DTYPE_I32_PTR,
	CO_DTYPE_VS_PTR,	CO_DTYPE_VS_CONST,
	CO_DTYPE_OS_PTR,	CO_DTYPE_DOMAIN,
	CO_DTYPE_U8_SDO_SERVER,	CO_DTYPE_U32_SDO_SERVER,
	CO_DTYPE_U8_SDO_CLIENT,	CO_DTYPE_U32_SDO_CLIENT,
	CO_DTYPE_U8_TPDO,	CO_DTYPE_U16_TPDO,	CO_DTYPE_U32_TPDO,
	CO_DTYPE_U8_RPDO,	CO_DTYPE_U16_RPDO,	CO_DTYPE_U32_RPDO,
	CO_DTYPE_U8_TMAP,	CO_DTYPE_U32_TMAP,
	CO_DTYPE_U8_RMAP,	CO_DTYPE_U32_RMAP,
	CO_DTYPE_U8_ERRCTRL,CO_DTYPE_U16_ERRCTRL,	CO_DTYPE_U32_ERRCTRL,
	CO_DTYPE_U8_SYNC,	CO_DTYPE_U32_SYNC,
	CO_DTYPE_U8_EMCY,	CO_DTYPE_U16_EMCY,	CO_DTYPE_U32_EMCY,
	CO_DTYPE_U32_TIME,
	CO_DTYPE_U32_STORE,
	CO_DTYPE_R32_VAR,
	CO_DTYPE_R32_CONST,
	CO_DTYPE_R32_PTR
} CO_DATA_TYPE_T;

/**
* object description
*/
typedef struct {
	UNSIGNED8		subIndex;		/**< subindex */
	CO_DATA_TYPE_T	dType;			/**< datatype and table (var, const, ptr) */
	UNSIGNED16		tableIdx;		/**< index at var table (descibed by DATA_TYPE_T typ) */
	UNSIGNED16		attr;			/**< object attributes like e.g. acccess rights */
	UNSIGNED16		defValIdx;		/**< index at const table for default value */
	UNSIGNED16		limitMinIdx;	/**< index at const table for min limit val */
	UNSIGNED16		limitMaxIdx;	/**< index at const table for max limit val */
} CO_OBJECT_DESC_T;

/**
* Object type
*/
typedef enum {
	CO_ODTYPE_VAR,		/**< variable */
	CO_ODTYPE_ARRAY,	/**< array */
	CO_ODTYPE_STRUCT	/**< structure */
} CO_ODTYPE_T;

/**
* object dictionary
*/
typedef struct {
	UNSIGNED16	index;				/**< object index */
	UNSIGNED8	numberOfSubs;		/**< number of subindex */
	UNSIGNED8	highestSub;			/**< highest used subindex */
	CO_ODTYPE_T	odType;				/**< variable, array, struct */
	UNSIGNED16	odDescIdx;			/**< index in object_description table */
} CO_OD_ASSIGN_T;

/* data variables from OD */
typedef struct {
	CO_CONST UNSIGNED8	 *odConst_u8;
	CO_CONST UNSIGNED16 *odConst_u16;
	CO_CONST UNSIGNED32 *odConst_u32;
	CO_CONST REAL32	*odConst_r32;
	UNSIGNED8	*odVar_u8;
	UNSIGNED16	*odVar_u16;
	UNSIGNED32	*odVar_u32;
	REAL32		*odVar_r32;
	CO_CONST INTEGER8	*odConst_i8;
	CO_CONST INTEGER16	*odConst_i16;
	CO_CONST INTEGER32	*odConst_i32;
	INTEGER8	*odVar_i8;
	INTEGER16	*odVar_i16;
	INTEGER32	*odVar_i32;
	UNSIGNED8	* CO_CONST *odPtr_u8;
	UNSIGNED16	* CO_CONST *odPtr_u16;
	UNSIGNED32	* CO_CONST *odPtr_u32;
	INTEGER8	* CO_CONST *odPtr_i8;
	INTEGER16	* CO_CONST *odPtr_i16;
	INTEGER32	* CO_CONST *odPtr_i32;
	REAL32	        * CO_CONST *odPtr_r32;
	CO_CONST VIS_STRING	*odConstVisString;
	CO_CONST UNSIGNED32	*odConstVisStringLen;
	VIS_STRING			*odVisString;
	UNSIGNED32			*odVisStringLen;
	OCTET_STRING		**odOctetString;
	UNSIGNED32			*odOctetStringLen;
	CO_DOMAIN_PTR 		**domainPtr;
	UNSIGNED32			*domainLen;
} CO_OD_DATA_VARIABLES_T;

/** sdowrite_callback return type.
 * Return type enumeration for sdo write callback register
*/
typedef enum
{
    eCANOPEN_SDO_REG_OK             = 0x00, /**< successfully registered */
    eCANOPEN_SDO_REG_ERR            = 0x01, /**< can't register desired fnc; out of memory */
} canopen_sdo_reg_t;

#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
extern CO_DOMAIN_PTR   *od_domain[];
extern UNSIGNED32      od_domain_len[];
extern VIS_STRING      od_vis_string[];
extern UNSIGNED32      od_vis_string_len[];

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

UNSIGNED8 co_od_domain_table_len (void);
UNSIGNED8 co_vis_string_table_len (void);

RET_T co_get_obj_data(UNSIGNED16 index,UNSIGNED8 subIndex, CO_DATA_TYPE_T *data_type, void *pData);
RET_T co_set_obj_data(UNSIGNED16 index, UNSIGNED8 subIndex, void *newVal);


#if !defined(DEBUG)
void coSimpleInit(void);

/** \brief function pointer to save/load/clear function
 *
 * \param subIndex - subindex parameter to point parameter area
 *
 * \return none
 */
typedef RET_T (* CO_EVENT_STORE_T)(UNSIGNED8 subIndex);


#else // CO_FULL_MODE
RET_T coCanOpenStackInit(CO_EVENT_STORE_T pLoadFunction);

void                canopen_sdowrite_call_callback(UNSIGNED16  index,UNSIGNED8   subIndex);
void                canopen_sdowrite_init_callback (void);
canopen_sdo_reg_t   icanopen_sdowrite_register_callback(UNSIGNED16  index,UNSIGNED8   subIndex,void * func_point);

void     coStartCOMMTaskImmediatelly_FromISR (void);
RET_T    co_odgetobjdomain(void *pData,UNSIGNED32 size,UNSIGNED16 index,UNSIGNED8 subIndex,UNSIGNED8 * p_regular_read_segment);
RET_T    co_sdoservercheckwriteind(UNSIGNED16 index, UNSIGNED8 subIndex, const UNSIGNED8 *pData);
void     co_ledgreenind(BOOL_T);
void     co_ledredind  (BOOL_T);
RET_T    co_saveind (UNSIGNED8 sub);
RET_T    co_clearind(UNSIGNED8 sub);
RET_T    co_loadind (UNSIGNED8 sub);
void     co_setup_string_object_data(void);
RET_T    co_setup_vis_string (char * CO_vis_string_Drive_manufacturer, char * new_string);
void     co_odaccess_os_lock(void);
void     co_odaccess_os_unlock(void);


#endif // CO_MODE

#endif /* CO_CANOPEN_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
