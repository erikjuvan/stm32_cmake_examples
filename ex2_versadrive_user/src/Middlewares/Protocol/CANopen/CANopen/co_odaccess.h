////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_odaccess.h
*@brief  contains defines for od access
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
#ifndef OD_ACCESS_H
#define OD_ACCESS_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
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
* Object type 
*/
typedef enum {
	CO_ODTYPE_VAR,		/**< variable */
	CO_ODTYPE_ARRAY,	/**< array */
	CO_ODTYPE_STRUCT	/**< structure */
} CO_ODTYPE_T;
 
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

/** \brief function pointer to object changed function 
 * \param index - object index
 * \param subindex - object subindex
 * 
 * \return RET_T
 */
typedef RET_T (* CO_EVENT_OBJECT_CHANGED_FCT_T)(UNSIGNED16, UNSIGNED8); /*lint !e960 customer specific parameter names */


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



////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void        coOdInitOdPtr(const CO_OD_ASSIGN_T	 *pOdAssing,UNSIGNED16		 odCnt,const CO_OBJECT_DESC_T *pObjdesc,UNSIGNED16		 descCnt,CO_EVENT_OBJECT_CHANGED_FCT_T	*pEventPtr,const CO_OD_DATA_VARIABLES_T *pOdVarPointers);
RET_T       coOdGetObj_u32(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED32 *pObj);
RET_T       coOdGetObj_u16(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED16 *pObj);
RET_T       coOdGetObj_u8(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED8 *pObj);
RET_T       coOdGetObj_i32(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER32 *pObj);
RET_T       coOdGetObj_i16(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER16 *pObj);
RET_T       coOdGetObj_i8(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER8 *pObj);
RET_T       coOdGetObj_r32(UNSIGNED16	index, UNSIGNED8 subIndex, REAL32 *pObj);
UNSIGNED16  coOdGetObjAttribute(CO_CONST CO_OBJECT_DESC_T *pObjDesc);
RET_T       coOdGetObjDescPtr(UNSIGNED16 index, UNSIGNED8 subIndex,CO_CONST CO_OBJECT_DESC_T ** pDescPtr);
UNSIGNED32  coOdGetObjSize(CO_CONST CO_OBJECT_DESC_T *pDesc);

RET_T coOdPutObj_u32(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED32 newVal);
RET_T coOdPutObj_u16(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED16 newVal);
RET_T coOdPutObj_u8(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED8 newVal);
RET_T coOdPutObj_i32(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER32 newVal);
RET_T coOdPutObj_i16(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER16 newVal);
RET_T coOdPutObj_i8(UNSIGNED16	index, UNSIGNED8 subIndex, INTEGER8 newVal);
RET_T coOdPutObj_r32(UNSIGNED16	index, UNSIGNED8 subIndex, REAL32 newVal);

void *coOdGetObjAddr(UNSIGNED16	index, UNSIGNED8 subIndex);

RET_T coOdGetDefaultVal_u32(UNSIGNED16	index, UNSIGNED8 subIndex, UNSIGNED32	*pDefVal);
RET_T coOdGetDefaultVal_u16(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED16 *pDefVal);
RET_T coOdGetDefaultVal_u8(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED8 *pDefVal);

RET_T coOdSetCobid(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED32	newCobId);
RET_T coOdDomainAddrSet(UNSIGNED16 index, UNSIGNED8 subIndex,CO_DOMAIN_PTR pAddr, UNSIGNED32 size);

RET_T coEventRegister_OBJECT_CHANGED(CO_EVENT_OBJECT_CHANGED_FCT_T, UNSIGNED16,UNSIGNED8);

CO_INLINE BOOL_T    coNumMemcpyPack(void *pDest, CO_CONST void *pSrc,UNSIGNED32 size, UNSIGNED16 numeric, UNSIGNED8 packOffset);
CO_INLINE void      coNumMemcpyUnpack(void *pDest, CO_CONST void *pSrc,UNSIGNED32 size, UNSIGNED16 numeric, UNSIGNED8 packOffset);
CO_INLINE BOOL_T    coNumMemcpy(void *pDest, CO_CONST void *pSrc,UNSIGNED32	size, UNSIGNED16 numeric);

#endif /* OD_ACCESS_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
