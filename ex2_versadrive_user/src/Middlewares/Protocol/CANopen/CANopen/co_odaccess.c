////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_odaccess.c
*@brief  contains routines for object dictionary access
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

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include "gen_define.h"
#include "co_datatype.h"
#include "co_odaccess.h"
#include "co_timer.h"
#include "co_drv.h"
#include "ico_cobhandler.h"
#include "ico_queue.h"
#include "ico_sdo.h"
#include "ico_pdo.h"
#include "ico_nmt.h"
#include "ico_sync.h"
#include "ico_emcy.h"
#include "ico_time.h"
#include "ico_odaccess.h"
#include "ico_store.h"
#include "co_canopen.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_EVENT_OBJECT_CHANGED
typedef struct {
    UNSIGNED16  index;          /* object index */
    UNSIGNED8   subIndex;       /* object subindex (if 255 - valid for all subindex) */
    CO_EVENT_OBJECT_CHANGED_FCT_T pFct;     /* function */
} CO_EVENT_OBJECT_CHANGED_T;
#endif /* CO_EVENT_OBJECT_CHANGED */

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_EVENT_OBJECT_CHANGED
static CO_EVENT_OBJECT_CHANGED_T    objectEventTable[CO_EVENT_OBJECT_CHANGED];
static UNSIGNED16                   objectEventTableCnt = 0u;
#endif /* CO_EVENT_OBJECT_CHANGED */

static CO_CONST CO_OD_ASSIGN_T  *pOdAssingTable;
static UNSIGNED16               odAssignCnt;
static CO_CONST CO_OBJECT_DESC_T*pObjDescTable;
/* static UNSIGNED16        objDescCnt; */
static CO_CONST UNSIGNED8       *pOdConst_u8 = NULL;
static CO_CONST UNSIGNED16      *pOdConst_u16 = NULL;
static CO_CONST UNSIGNED32      *pOdConst_u32 = NULL;
static CO_CONST REAL32          *pOdConst_r32 = NULL;
static UNSIGNED8                *pOdVar_u8 = NULL;
static UNSIGNED16               *pOdVar_u16 = NULL;
static UNSIGNED32               *pOdVar_u32 = NULL;
static REAL32                   *pOdVar_r32 = NULL;
static CO_CONST INTEGER8        *pOdConst_i8 = NULL;
static CO_CONST INTEGER16       *pOdConst_i16 = NULL;
static CO_CONST INTEGER32       *pOdConst_i32 = NULL;
static INTEGER8                 *pOdVar_i8 = NULL;
static INTEGER16                *pOdVar_i16 = NULL;
static INTEGER32                *pOdVar_i32 = NULL;
static UNSIGNED8 * CO_CONST     *pOdPtr_u8 = NULL;
static UNSIGNED16 * CO_CONST    *pOdPtr_u16 = NULL;
static UNSIGNED32 * CO_CONST    *pOdPtr_u32 = NULL;
static INTEGER8  * CO_CONST     *pOdPtr_i8 = NULL;
static INTEGER16 * CO_CONST     *pOdPtr_i16 = NULL;
static INTEGER32 * CO_CONST     *pOdPtr_i32 = NULL;
static REAL32    * CO_CONST     *pOdPtr_r32 = NULL;
static CO_CONST VIS_STRING      *pOdConst_VS = NULL;
static CO_CONST UNSIGNED32      *pOdConst_VS_Len = NULL;
static CO_CONST VIS_STRING      *pOdPtr_VS = NULL;
static CO_CONST UNSIGNED32      *pOdPtr_VS_Len = NULL;
static OCTET_STRING * CO_CONST  *pOdPtr_OS = NULL;
static UNSIGNED32     CO_CONST  *pOdLen_OS =  NULL;
static CO_DOMAIN_PTR * CO_CONST *pOdPtr_Domain = NULL; /*lint !e961 pointer to generic domains */
static UNSIGNED32               *pOdLen_Domain = NULL;
#ifdef CO_EVENT_STORE
static BOOL_T                   loadParameterState = CO_FALSE;
#endif /* CO_EVENT_STORE */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static RET_T                odGetObjDescIdx(UNSIGNED16 index, UNSIGNED8 subIndex,UNSIGNED16  *pDescIdx);
static UNSIGNED8            *odGetObjAddr(const CO_OBJECT_DESC_T   *pDesc);
static CO_CONST UNSIGNED8   *odGetObjAddr_R(const CO_OBJECT_DESC_T    *pDesc);
#ifdef CO_CPU_DSP
static void                 odCorrectObjOffs(CO_CONST CO_OBJECT_DESC_T *pDesc, UNSIGNED32 *pOffset,UNSIGNED8 *pPackOffset, UNSIGNED16 *pAttr);
#endif

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

/***************************************************************************/
/**
* \internal
*
* \brief coNumMemcpyPack - memcpy with numeric dependencies
*
* This function provide a memcpy from CAN-Controller to OD variable.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or packaging is done.
*
* \return none
*
*/
CO_INLINE BOOL_T coNumMemcpyPack(
		void		*pDest,
		CO_CONST	void *pSrc,
		UNSIGNED32	size,
		UNSIGNED16	numeric,
		UNSIGNED8	packOffset
	)
{
    BOOL_T	changed = CO_FALSE;
#ifdef CO_EVENT_OBJECT_CHANGED
    INTEGER32		cmp = 0;
#endif /* CO_EVENT_OBJECT_CHANGED */

#ifdef CO_CPU_DSP
#else /* CO_CPU_DSP */
    (void)packOffset;
#endif /* CO_CPU_DSP */

	if (numeric == 0u)  {
#ifdef CO_EVENT_OBJECT_CHANGED
		cmp = memcmp(pDest, pSrc, (size_t)size);
		if (cmp != 0)  {
			changed = CO_TRUE;
		}
#endif /* CO_EVENT_OBJECT_CHANGED */
		memcpy(pDest, pSrc, (size_t)size);
	} else {

#ifdef CO_BIG_ENDIAN
    UNSIGNED8	*pPtr;
    UNSIGNED8   *pDest_u8 = (UNSIGNED8 *) pDest;

		pPtr = (UNSIGNED8 *)pSrc + size - 1u;

		while (size > 0u)  {
#ifdef CO_EVENT_OBJECT_CHANGED
			if (*pDest_u8 != *pPtr)  {
				changed = CO_TRUE;
			}
#endif /* CO_EVENT_OBJECT_CHANGED */
			*pDest_u8 = *pPtr;
			pDest_u8 ++;
			pPtr --;
			size--;
		}

#else /* CO_BIG_ENDIAN */
# ifdef CO_CPU_DSP
unsigned char	*pDest_u32 = (unsigned char *) pDest;
    UNSIGNED8	*pDest_u8 = (UNSIGNED8 *) pDest;
    UNSIGNED8	*pSrc_u8 = (UNSIGNED8 *) pSrc;
    UNSIGNED32	val;

		while (size > 0u)  {
			val = *pSrc_u8;
#ifdef CO_EVENT_OBJECT_CHANGED
			if ((*pDest_u32 >> (packOffset * 8u)) != val)  {
				changed = CO_TRUE;
			}
#endif /* CO_EVENT_OBJECT_CHANGED */
			*pDest_u32 &= ~(0xfful << (packOffset * 8u));
			*pDest_u32 |= (val << (packOffset * 8u));

			/* incr packOffset */
			packOffset++;
			if ((packOffset % CO_CPU_DSP_BYTESIZE) == 0u) {
				packOffset = 0u;
				pDest_u8 ++;
				pDest_u32 = (unsigned char *) pDest_u8;
			}

			pSrc_u8 ++;
			size --;
		}

# else /* CO_CPU_DSP */

#ifdef CO_EVENT_OBJECT_CHANGED
		cmp = memcmp(pDest, pSrc, (size_t)size);
		if (cmp != 0)  {
			changed = CO_TRUE;
		}
#endif /* CO_EVENT_OBJECT_CHANGED */
		memcpy(pDest, pSrc, (size_t)size);
# endif /* CO_CPU_DSP */

#endif /* CO_BIG_ENDIAN */
	}

	return(changed);
}


/***************************************************************************/
/**
* \internal
*
* \brief coNumMemcpyUnpack - memcpy with numeric dependencies
*
* This function provide a memcpy from OD variable to CAN-Controller.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or unpackaging is done.
*
* \return none
*
*/
CO_INLINE void coNumMemcpyUnpack(
		void		*pDest,			/* destination address */
		CO_CONST void	*pSrc,		/* source address */
		UNSIGNED32	size,			/* size in bytes */
		UNSIGNED16	numeric,		/* numeric copy */
		UNSIGNED8	packOffset		/* pack offset */
	)
{
#ifdef CO_CPU_DSP
#else /* CO_CPU_DSP */
    (void)packOffset;
#endif /* CO_CPU_DSP */

	/* no numeric copy, copy address by address */
	if (numeric == 0u)  {
		memcpy(pDest, pSrc, (size_t)size);
	} else {

#ifdef CO_BIG_ENDIAN
    UNSIGNED8	*pPtr;
    UNSIGNED8   *pDest_u8 = (UNSIGNED8 *) pDest;

		pPtr = (UNSIGNED8 *)pSrc + size - 1;

		while (size > 0u)  {
			*pDest_u8 = *pPtr;
			pDest_u8 ++;
			pPtr --;
			size--;
		}
#else /* CO_BIG_ENDIAN */
# ifdef CO_CPU_DSP
    typedef unsigned char U_DSP_T;
    UNSIGNED8	*pDest_u8 = (UNSIGNED8 *) pDest;
    UNSIGNED8	*pSrc_u8 = (UNSIGNED8 *) pSrc;
    U_DSP_T	*pSrc_u32 = (U_DSP_T *) pSrc;

		while (size > 0u)  {
			*pDest_u8 = (*pSrc_u32 >> (packOffset * 8u)) & 0xffu;

			/* incr packOffset */
			packOffset++;
			if ((packOffset % CO_CPU_DSP_BYTESIZE) == 0u) {
				packOffset = 0u;
				pSrc_u8 ++;
				pSrc_u32 = (U_DSP_T *) pSrc_u8;
			}

			pDest_u8 ++;
			size --;
		}

# else /* CO_CPU_DSP */
		memcpy(pDest, pSrc, (size_t)size);
# endif /* CO_CPU_DSP */
#endif /* CO_BIG_ENDIAN */
	}
}


/***************************************************************************/
/**
* \internal
*
* \brief coNumMemcpy - memcpy with numeric dependencies
*
* This function provide an internal memcpy.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or unpackaging is done.
*
* \return none
*
*/
CO_INLINE BOOL_T coNumMemcpy(
		void		*pDest,			/* destination address */
		CO_CONST void	*pSrc,		/* source address */
		UNSIGNED32	size,			/* size in bytes */
		UNSIGNED16	numeric			/* numeric copy */
	)
{
#ifdef CO_EVENT_OBJECT_CHANGED
    INTEGER32		cmp;
#endif /* CO_EVENT_OBJECT_CHANGED */
    UNSIGNED32	corrSize = size;
    BOOL_T	changed = CO_FALSE;
    (void)numeric;

#ifdef CO_CPU_DSP
	if (numeric != 0u)  {
		corrSize = size / CO_CPU_DSP_BYTESIZE;
		if (corrSize == 0u)  {
			corrSize = 1u;
		}
	}
#endif /* CO_CPU_DSP */

#ifdef CO_EVENT_OBJECT_CHANGED
	cmp = memcmp(pDest, pSrc, (size_t)corrSize);
	if (cmp != 0)  {
		changed = CO_TRUE;
	}
#endif /* CO_EVENT_OBJECT_CHANGED */

	memcpy(pDest, pSrc, (size_t)corrSize);

	return(changed);
}


/***************************************************************************/
/**
* \brief coOdSetCobid - set cob id
*
* This function set the COB-Id for a service indicated by index and subindex.
*
* According to the standard, the COB-ID is disabled first by this function,
* and then the new COB-ID is set.
*
* \return RET_T
*
*/
RET_T coOdSetCobid(
		UNSIGNED16	index,			/**< index for the cob */
		UNSIGNED8	subIndex,		/**< subIndex for the cob */
		UNSIGNED32	newCobId		/**< new cob-id */
	)
{
    RET_T	retVal;

	/* disable cob-id */
	retVal = coOdPutObj_u32(index, subIndex, CO_COB_INVALID);
	if (retVal == RET_OK)  {
		/* write new cobid */
		retVal = coOdPutObj_u32(index, subIndex, newCobId);
	}

	return(retVal);
}


/***************************************************************************/
/**
*
* internal
*
* \brief coOdGetObjAddr - get address of an object
*
* Get the address of an object from the object dictionary.
*
* \return pointer to object address, NULL if object not found
*
*/
void *coOdGetObjAddr(
		UNSIGNED16	index,		/* index of object */
		UNSIGNED8	subIndex	/* subindex of object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    UNSIGNED8	*pAddr;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(NULL);
	}
	pAddr = odGetObjAddr(pDesc);

	return(pAddr);
}


/***************************************************************************/
/**
* \brief coOdGetObj_u8 - get UNSIGNED8 object
*
* Get an object from the object dictionary from type UNSIGNED8.
*
* \return RET_T
*
*/
RET_T coOdGetObj_u8(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED8	*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_CONST:
		case CO_DTYPE_U8_VAR:
		case CO_DTYPE_U8_PTR:
		case CO_DTYPE_U8_SDO_SERVER:
		case CO_DTYPE_U8_SDO_CLIENT:
		case CO_DTYPE_U8_TPDO:
		case CO_DTYPE_U8_RPDO:
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U8_SYNC:
		case CO_DTYPE_U8_EMCY:
		case CO_DTYPE_U8_ERRCTRL:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
			break;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_u16 - get UNSIGNED16 object
*
* Get an object from the object dictionary from type UNSIGNED16.
*
* \return RET_T
*
*/
RET_T coOdGetObj_u16(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED16	*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U16_CONST:
		case CO_DTYPE_U16_VAR:
		case CO_DTYPE_U16_PTR:
		case CO_DTYPE_U16_TPDO:
		case CO_DTYPE_U16_RPDO:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U16_EMCY:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_u32 - get UNSIGNED32 object
*
* Get an object from the object dictionary from type UNSIGNED32.
*
* \return RET_T
*
*/
RET_T coOdGetObj_u32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED32	*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U32_CONST:
		case CO_DTYPE_U32_VAR:
		case CO_DTYPE_U32_PTR:
		case CO_DTYPE_U32_SDO_SERVER:
		case CO_DTYPE_U32_SDO_CLIENT:
		case CO_DTYPE_U32_TPDO:
		case CO_DTYPE_U32_RPDO:
		case CO_DTYPE_U32_TMAP:
		case CO_DTYPE_U32_RMAP:
		case CO_DTYPE_U32_ERRCTRL:
		case CO_DTYPE_U32_EMCY:
		case CO_DTYPE_U32_SYNC:
		case CO_DTYPE_U32_STORE:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}


	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_i8 - get INTEGER8 object
*
* Get an object from the object dictionary from type INTEGER8.
*
* \return RET_T
*
*/
RET_T coOdGetObj_i8(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER8	*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I8_VAR:
		case CO_DTYPE_I8_CONST:
		case CO_DTYPE_I8_PTR:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}


	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_i16 - get INTEGER16 object
*
* Get an object from the object dictionary from type INTEGER16.
*
* \return RET_T
*
*/
RET_T coOdGetObj_i16(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER16	*pObj		/**< pointer to object */
	)
{
    RET_T	retVal;
    CO_CONST CO_OBJECT_DESC_T *pDesc;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I16_VAR:
		case CO_DTYPE_I16_PTR:
		case CO_DTYPE_I16_CONST:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_i32 - get INTEGER32 object
*
* Get an object from the object dictionary from type INTEGER32.
*
* \return RET_T
*
*/
RET_T coOdGetObj_i32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER32	*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I32_VAR:
		case CO_DTYPE_I32_PTR:
		case CO_DTYPE_I32_CONST:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObj_r32 - get REAL32 object
*
* Get an object from the object dictionary from type REAL32.
*
* \return RET_T
*
*/
RET_T coOdGetObj_r32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		REAL32		*pObj		/**< pointer to object */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_R32_VAR:
		case CO_DTYPE_R32_PTR:
		case CO_DTYPE_R32_CONST:
			retVal = icoOdGetObj(pDesc, pObj, 0u, 0u, CO_TRUE);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_u8 - put UNSIGNED8 value to object
*
* Put value from type UNSIGNED8 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_u8(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED8	newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_VAR:
		case CO_DTYPE_U8_PTR:
		case CO_DTYPE_U8_SDO_SERVER:
		case CO_DTYPE_U8_SDO_CLIENT:
		case CO_DTYPE_U8_TPDO:
		case CO_DTYPE_U8_RPDO:
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U8_SYNC:
		case CO_DTYPE_U8_EMCY:
		case CO_DTYPE_U8_ERRCTRL:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_u16 - put UNSIGNED16 value to object
*
* Put value from type UNSIGNED16 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_u16(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED16	newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U16_VAR:
		case CO_DTYPE_U16_PTR:
		case CO_DTYPE_U16_TPDO:
		case CO_DTYPE_U16_RPDO:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U16_EMCY:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_u32 - put UNSIGNED32 value to object
*
* Put value from type UNSIGNED32 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_u32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		UNSIGNED32	newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U32_VAR:
		case CO_DTYPE_U32_PTR:
		case CO_DTYPE_U32_SDO_SERVER:
		case CO_DTYPE_U32_SDO_CLIENT:
		case CO_DTYPE_U32_TPDO:
		case CO_DTYPE_U32_RPDO:
		case CO_DTYPE_U32_TMAP:
		case CO_DTYPE_U32_RMAP:
		case CO_DTYPE_U32_ERRCTRL:
		case CO_DTYPE_U32_EMCY:
		case CO_DTYPE_U32_SYNC:
		case CO_DTYPE_U32_TIME:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_i8 - Put INTEGER8 object
*
* Put value from type INTEGER8 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_i8(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER8	newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I8_VAR:
		case CO_DTYPE_I8_PTR:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_i16 - Put INTEGER16 object
*
* Put value from type INTEGER16 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_i16(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER16	newVal		/**< new value */
	)
{
    RET_T	retVal;
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I16_VAR:
		case CO_DTYPE_I16_PTR:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_i32 - Put INTEGER32 object
*
* Put value from type INTEGER32 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_i32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		INTEGER32	newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_I32_VAR:
		case CO_DTYPE_I32_PTR:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
			break;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdPutObj_r32 - Put REAL32 object
*
* Put value from type REAL32 to the object dictionary
*
* \return RET_T
*
*/
RET_T coOdPutObj_r32(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		REAL32		newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_R32_VAR:
		case CO_DTYPE_R32_PTR:
			retVal = icoOdPutObj(pDesc, &newVal, 0u, 0u, CO_TRUE, &changed);
			break;

		default:
			retVal = RET_OD_ACCESS_ERROR;
			break;
	}

	if (retVal == RET_OK)  {
		retVal = icoEventObjectChanged(pDesc, index, changed);
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoOdGetObj - get data from object
*
* copy data from OD to given pointer
*
*
* \return RET_T
*
*/
RET_T icoOdGetObj(
		CO_CONST CO_OBJECT_DESC_T	*pDesc,	/* pointer for description index */
		void				*pData,		/* pointer to data */
		UNSIGNED32			offset,		/* offset at object data in bytes */
		UNSIGNED32			maxData,	/* buffer len of pData in bytes */
		BOOL_T				internal	/* data are used internal */
	)
{
    CO_CONST UNSIGNED8	*pObjAddr;
    UNSIGNED32	size = 0u;
    RET_T	    retVal = RET_VALUE_NOT_AVAILABLE;
    UNSIGNED8	packOffset = 0u;
    UNSIGNED16	attr = pDesc->attr;

	pObjAddr = odGetObjAddr_R(pDesc);
	if (pObjAddr != NULL)  {
		/* get object size */
		size = coOdGetObjSize(pDesc);

		/* request data outsize of object return error */
		if (offset >= size)  {
			return(RET_OUT_OF_MEMORY);
		}
		/* return only rest data */
		size -= offset;
		if (maxData != 0u)  {
			if (size > maxData)  {
				size = maxData;
			}
		}

#ifdef CO_CPU_DSP
		odCorrectObjOffs(pDesc, &offset, &packOffset, &attr);
#endif /* CO_CPU_DSP */

		pObjAddr += offset; /*lint !e960 Note: pointer arithmetic other than array indexing used */

		/* lock OD */
		co_odaccess_os_lock();

		if (internal == CO_TRUE)  {
			(void)coNumMemcpy(pData, pObjAddr, size, (attr & CO_ATTR_NUM));
		} else {
			coNumMemcpyUnpack(pData, pObjAddr, size,
				(attr & CO_ATTR_NUM), packOffset);
		}
        /* unlock OD */
		co_odaccess_os_unlock();

		retVal = RET_OK;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoOdPutObj - put data to object
*
* copy data to OD from given pointer
*
*
* \return RET_T
*
*/
RET_T icoOdPutObj(
		CO_CONST CO_OBJECT_DESC_T	*pDesc,	/* pointer for description index */
		CO_CONST void		*pData,		/* pointer to data */
		UNSIGNED32			offset,		/* offset at object */
		UNSIGNED32			maxData,	/* buffer len of pData */
		BOOL_T				internal,	/* data are used internal */
		BOOL_T				*pChanged	/* object got new value */
	)
{
    UNSIGNED8	*pObjAddr = NULL;
    UNSIGNED32	size = 0u;
    RET_T		retVal = RET_INVALID_PARAMETER;
    UNSIGNED8	packOffset = 0u;
    UNSIGNED16	attr = pDesc->attr;

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_CONST:
		case CO_DTYPE_U16_CONST:
		case CO_DTYPE_U32_CONST:
		case CO_DTYPE_I8_CONST:
		case CO_DTYPE_I16_CONST:
		case CO_DTYPE_I32_CONST:
		case CO_DTYPE_R32_CONST:
		case CO_DTYPE_VS_CONST:
			pObjAddr = NULL;
			break;
		default:
			pObjAddr = odGetObjAddr(pDesc);
			break;
	}

	if (pObjAddr != NULL)  {
		size = coOdGetObjSize(pDesc);

		if (offset >= size)  {
			return(RET_OUT_OF_MEMORY);
		}
		size -= offset;
		if (maxData != 0u)  {
			if (size > maxData)  {
				size = maxData;
			}
		}

#ifdef CO_CPU_DSP
		odCorrectObjOffs(pDesc, &offset, &packOffset, &attr);
#endif /* CO_CPU_DSP */

		pObjAddr += offset; /*lint !e960 Note: pointer arithmetic other than array indexing used */

        /* lock OD */
		co_odaccess_os_lock();

		/* check for new data */
		if (internal == CO_TRUE)   {
			*pChanged = coNumMemcpy((void *)pObjAddr, pData, size,
				(attr & CO_ATTR_NUM));
		} else {
			*pChanged = coNumMemcpyPack((void *)pObjAddr, pData,size,
				(attr & CO_ATTR_NUM), packOffset);
		}

        /* unlock OD */
		co_odaccess_os_unlock();

		retVal = RET_OK;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief odGetObjAddr_R - get object address for read objects
*
* \return RET_T
*
*/
static CO_CONST UNSIGNED8 *odGetObjAddr_R(
		CO_CONST CO_OBJECT_DESC_T	*pDesc	/* pointer for description index */
	)
{
    CO_CONST void *pObjAddr = NULL;

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_CONST:
			if (pOdConst_u8 != NULL)  {
				pObjAddr = &pOdConst_u8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U16_CONST:
			if (pOdConst_u16 != NULL)  {
				pObjAddr = &pOdConst_u16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U32_CONST:
			/* table initialized ? */
			if (pOdConst_u32 != NULL)  {
				pObjAddr = &pOdConst_u32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I8_CONST:
			/* table initialized ? */
			if (pOdConst_i8 != NULL)  {
				pObjAddr = &pOdConst_i8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I16_CONST:
			/* table initialized ? */
			if (pOdConst_i16 != NULL)  {
				pObjAddr = &pOdConst_i16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I32_CONST:
			/* table initialized ? */
			if (pOdConst_i32 != NULL)  {
				pObjAddr = &pOdConst_i32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_R32_CONST:
			/* table initialized ? */
			if (pOdConst_r32 != NULL)  {
				pObjAddr = &pOdConst_r32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;

#ifdef CO_PDO_RECEIVE_CNT
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U32_RMAP:
			pObjAddr = icoRecMapGetObjectAddr_R(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_PDO_RECEIVE_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U32_TMAP:
			pObjAddr = icoTrMapGetObjectAddr_R(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_PDO_TRANSMIT_CNT */

		default:
			pObjAddr = odGetObjAddr(pDesc);
			break;
	}

	return(pObjAddr);
}


/***************************************************************************/
/**
* \internal
*
* \brief odGetObjAddr - get object address (read/write)
*
* \return RET_T
*
*/
static UNSIGNED8 *odGetObjAddr(
		CO_CONST CO_OBJECT_DESC_T	*pDesc		/* pointer for description index */
	)
{
    void *pObjAddr = NULL;

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_VAR:
			/* table initialized ? */
			if (pOdVar_u8 != NULL)  {
				pObjAddr = &pOdVar_u8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U16_VAR:
			/* table initialized ? */
			if (pOdVar_u16 != NULL)  {
				pObjAddr = &pOdVar_u16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U32_VAR:
			/* table initialized ? */
			if (pOdVar_u32 != NULL)  {
				pObjAddr = &pOdVar_u32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_R32_VAR:
			/* table initialized ? */
			if (pOdVar_r32 != NULL)  {
				pObjAddr = &pOdVar_r32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U8_PTR:
			if (pOdPtr_u8 != NULL)  {
				pObjAddr = pOdPtr_u8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U16_PTR:
			if (pOdPtr_u16 != NULL)  {
				pObjAddr = pOdPtr_u16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U32_PTR:
			/* table initialized ? */
			if (pOdPtr_u32 != NULL)  {
				pObjAddr = pOdPtr_u32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I8_VAR:
			/* table initialized ? */
			if (pOdVar_i8 != NULL)  {
				pObjAddr = &pOdVar_i8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I16_VAR:
			/* table initialized ? */
			if (pOdVar_i16 != NULL)  {
				pObjAddr = &pOdVar_i16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I32_VAR:
			/* table initialized ? */
			if (pOdVar_i32 != NULL)  {
				pObjAddr = &pOdVar_i32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I8_PTR:
			if (pOdPtr_i8 != NULL)  {
				pObjAddr = pOdPtr_i8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I16_PTR:
			if (pOdPtr_i16 != NULL)  {
				pObjAddr = pOdPtr_i16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_I32_PTR:
			/* table initialized ? */
			if (pOdPtr_i32 != NULL)  {
				pObjAddr = pOdPtr_i32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_R32_PTR:
			/* table initialized ? */
			if (pOdPtr_r32 != NULL)  {
				pObjAddr = pOdPtr_r32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_VS_CONST:
			if (pOdConst_VS != NULL)  {
				pObjAddr = pOdConst_VS[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_VS_PTR:
			if (pOdPtr_VS != NULL)  {
				pObjAddr = pOdPtr_VS[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_OS_PTR:
			if (pOdPtr_OS != NULL)  {
				pObjAddr = pOdPtr_OS[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_DOMAIN:
			if (pOdPtr_Domain != NULL)  {
				pObjAddr = *pOdPtr_Domain[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U8_SDO_SERVER:
			pObjAddr = icoSdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_SDO_SERVER:
			pObjAddr = icoSdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#ifdef CO_SDO_CLIENT_CNT
		case CO_DTYPE_U8_SDO_CLIENT:
			pObjAddr = icoSdoClientGetObjectAddr((UNSIGNED8)pDesc->tableIdx,
					(UNSIGNED8)pDesc->subIndex);
			break;
		case CO_DTYPE_U32_SDO_CLIENT:
			pObjAddr = icoSdoClientGetObjectAddr((UNSIGNED8)pDesc->tableIdx,
					(UNSIGNED8)pDesc->subIndex);
			break;
#endif /* CO_SDO_CLIENT_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
		case CO_DTYPE_U8_TPDO:
			pObjAddr = icoTrPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U16_TPDO:
			pObjAddr = icoTrPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_TPDO:
			pObjAddr = icoTrPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U32_TMAP:
			pObjAddr = icoTrMapGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_PDO_TRANSMIT_CNT */

#ifdef CO_PDO_RECEIVE_CNT
		case CO_DTYPE_U8_RPDO:
			pObjAddr = icoRecPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U16_RPDO:
			pObjAddr = icoRecPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_RPDO:
			pObjAddr = icoRecPdoGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U32_RMAP:
			pObjAddr = icoRecMapGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_PDO_RECEIVE_CNT */

		case CO_DTYPE_U8_ERRCTRL:
			pObjAddr = icoErrCtrlGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U16_ERRCTRL:
			pObjAddr = icoErrCtrlGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_ERRCTRL:
			pObjAddr = icoErrCtrlGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;

#ifdef CO_SYNC_SUPPORTED
		case CO_DTYPE_U8_SYNC:
			pObjAddr = icoSyncGetObjectAddr(pDesc->tableIdx);
			break;
		case CO_DTYPE_U32_SYNC:
			pObjAddr = icoSyncGetObjectAddr(pDesc->tableIdx);
			break;
#endif /* CO_SYNC_SUPPORTED */

		case CO_DTYPE_U8_EMCY:
			pObjAddr = icoEmcyGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#ifdef CO_EMCY_PRODUCER
		case CO_DTYPE_U16_EMCY:
			pObjAddr = icoEmcyGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_EMCY_PRODUCER */
#if defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT)
		case CO_DTYPE_U32_EMCY:
			pObjAddr = icoEmcyGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT) */

#ifdef CO_TIME_SUPPORTED
		case CO_DTYPE_U32_TIME:
			pObjAddr = icoTimeGetObjectAddr();
			break;
#endif /* CO_TIME_SUPPORTED */

#ifdef CO_STORE_SUPPORTED
		case CO_DTYPE_U32_STORE:
			pObjAddr = icoStoreGetObjectAddr(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_STORE_SUPPORTED */
		default:
			break;
	}

	return(pObjAddr);
}


#ifdef CO_CPU_DSP
/***************************************************************************/
/**
* \internal
*
* \brief odCorrectObjOffs - correct object address offset for DSP domains
*
* \return RET_T
*
*/
void odCorrectObjOffs(
		CO_CONST CO_OBJECT_DESC_T	*pDesc,		/* pointer for description index */
		UNSIGNED32			*pOffset,
		UNSIGNED8			*pPackOffset,
		UNSIGNED16			*pAttr
	)
{
	switch (pDesc->dType)  {
//		case CO_DTYPE_U64:
		case CO_DTYPE_DOMAIN:
		case CO_DTYPE_OS_PTR:
			*pPackOffset = *pOffset % CO_CPU_DSP_BYTESIZE;
			*pOffset = *pOffset / CO_CPU_DSP_BYTESIZE;
			*pAttr |= CO_ATTR_NUM;
			break;

		default:
			break;
	}
}
#endif /* CO_CPU_DSP */


/***************************************************************************/
/**
* \brief coOdGetObjAttribute - get object attribute
*
* This function returns the attribute of the object from the given
* object description.
*
* \return attribute
*
*/
UNSIGNED16 coOdGetObjAttribute(
		CO_CONST CO_OBJECT_DESC_T	*pObjDesc	/**< pointer to object description */
	)
{
	return(pObjDesc->attr);
}


/***************************************************************************/
/**
* \brief coOdDomainAddrSet - set domain address
*
* This function sets the adress and the size of a domain.<br>
* At the initialization time,
* domains are not initialized at the object dictionary.
* This has to be done by this function.
* 
* \return RET_T
*
*/
RET_T coOdDomainAddrSet(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		CO_DOMAIN_PTR	pAddr,	/**< pointer to domain */
		UNSIGNED32	size		/**< domain length */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	if (pDesc->dType != CO_DTYPE_DOMAIN)  {
		return(RET_SDO_DATA_TYPE_NOT_MATCH);
	}

	*pOdPtr_Domain[pDesc->tableIdx] = pAddr; /*lint !e960 Note: pointer arithmetic other than array indexing used */
	pOdLen_Domain[pDesc->tableIdx] = size; /*lint !e960 Note: pointer arithmetic other than array indexing used */

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoOdCheckObjLimits - check object limits
*
*
* \return RET_T
*
*/
RET_T icoOdCheckObjLimits(
		CO_CONST CO_OBJECT_DESC_T *pDesc,	/* pointer to object description */
		CO_CONST UNSIGNED8	*pData			/* pointer to receive data */
	)
{
    RET_T	retVal = RET_INVALID_PARAMETER;
    UNSIGNED32	u32;
#ifdef CO_CONFIG_LIMIT_CHECK
    UNSIGNED8	u8 = 0;
    UNSIGNED32	u16 = 0;
    INTEGER8	i8 = 0;
    INTEGER16	i16 = 0;
    INTEGER32	i32 = 0;
    REAL32		r32 = 0;
#endif /* CO_CONFIG_LIMIT_CHECK */

	/* check only objects with limit check attribut */
	if ((pDesc->attr & CO_ATTR_LIMIT) == 0u)  {
		return(RET_OK);
	}

	(void)coNumMemcpyPack(&u32, pData, sizeof(u32), 1u, 0u);

	switch (pDesc->dType)  {

#ifdef CO_CONFIG_LIMIT_CHECK
		case CO_DTYPE_U8_VAR:
		case CO_DTYPE_U8_PTR:
			if (pOdConst_u8 != NULL)  {
				(void)coNumMemcpyPack(&u8, pData, sizeof(u8), 1u, 0u);
				if ((u8 >= pOdConst_u8[pDesc->limitMinIdx]) 
				 && (u8 <= pOdConst_u8[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_U16_VAR:
		case CO_DTYPE_U16_PTR:
			if (pOdConst_u16 != NULL)  {
				(void)coNumMemcpyPack(&u16, pData, sizeof(u16), 1u, 0u);
				if ((u16 >= pOdConst_u16[pDesc->limitMinIdx]) 
				 && (u16 <= pOdConst_u16[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_U32_VAR:
		case CO_DTYPE_U32_PTR:
			if (pOdConst_u32 != NULL)  {
				if ((u32 >= pOdConst_u32[pDesc->limitMinIdx]) 
				 && (u32 <= pOdConst_u32[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_I8_VAR:
		case CO_DTYPE_I8_PTR:
			if (pOdConst_i8 != NULL)  {
				(void)coNumMemcpyPack(&i8, pData, sizeof(i8), 1u, 0u);
				if ((i8 >= pOdConst_i8[pDesc->limitMinIdx]) 
				 && (i8 <= pOdConst_i8[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_I16_VAR:
		case CO_DTYPE_I16_PTR:
			if (pOdConst_i16 != NULL)  {
				(void)coNumMemcpyPack(&i16, pData, sizeof(i16), 1u, 0u);
				if ((i16 >= pOdConst_i16[pDesc->limitMinIdx]) 
				 && (i16 <= pOdConst_i16[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_I32_VAR:
		case CO_DTYPE_I32_PTR:
			if (pOdConst_i32 != NULL)  {
				(void)coNumMemcpyPack(&i32, pData, sizeof(i32), 1u, 0u);
				if ((i32 >= pOdConst_i32[pDesc->limitMinIdx]) 
				 && (i32 <= pOdConst_i32[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;
		case CO_DTYPE_R32_VAR:
		case CO_DTYPE_R32_PTR:
			if (pOdConst_r32 != NULL)  {
				(void)coNumMemcpyPack(&r32, pData, sizeof(r32), 1u, 0u);
				if ((r32 >= pOdConst_r32[pDesc->limitMinIdx]) 
				 && (r32 <= pOdConst_r32[pDesc->limitMaxIdx]))  {
					retVal = RET_OK;
				}
			}
			break;

#else /* CO_CONFIG_LIMIT_CHECK */
		case CO_DTYPE_U8_VAR:
		case CO_DTYPE_U16_VAR:
		case CO_DTYPE_U32_VAR:
		case CO_DTYPE_R32_VAR:
		case CO_DTYPE_U8_PTR:
		case CO_DTYPE_U16_PTR:
		case CO_DTYPE_U32_PTR:
		case CO_DTYPE_R32_PTR:
		case CO_DTYPE_I8_CONST:
		case CO_DTYPE_I16_CONST:
		case CO_DTYPE_I32_CONST:
		case CO_DTYPE_I8_VAR:
		case CO_DTYPE_I16_VAR:
		case CO_DTYPE_I32_VAR:
		case CO_DTYPE_I8_PTR:
		case CO_DTYPE_I16_PTR:
		case CO_DTYPE_I32_PTR:
			retVal = RET_OK;
			break;
#endif /* CO_CONFIG_LIMIT_CHECK */

		case CO_DTYPE_U8_CONST:
		case CO_DTYPE_U16_CONST:
		case CO_DTYPE_U32_CONST:
		case CO_DTYPE_R32_CONST:
		case CO_DTYPE_VS_CONST:
		case CO_DTYPE_VS_PTR:
		case CO_DTYPE_OS_PTR:
		case CO_DTYPE_DOMAIN:
			retVal = RET_OK;
			break;

		case CO_DTYPE_U8_SDO_SERVER:
			retVal = icoSdoCheckObjLimitNode(pDesc->tableIdx);
			break;
		case CO_DTYPE_U32_SDO_SERVER:
			retVal = icoSdoCheckObjLimitCobId(pDesc->tableIdx, pDesc->subIndex,
				u32);
			break;
#ifdef CO_SDO_CLIENT_CNT
		case CO_DTYPE_U8_SDO_CLIENT:
			retVal = icoSdoClientCheckObjLimitNode(pDesc->tableIdx);
			break;
		case CO_DTYPE_U32_SDO_CLIENT:
			retVal = icoSdoClientCheckObjLimitCobId(pDesc->tableIdx,
				pDesc->subIndex, u32);
			break;
#endif /* CO_SDO_CLIENT_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
		case CO_DTYPE_U8_TPDO:
			/* transtype/sync start value */
			retVal = icoTrPdoCheckObjLimit_u8(pDesc->tableIdx, pDesc->subIndex,
				*pData);
			break;
		case CO_DTYPE_U16_TPDO:
			/* inhibit/event timer */
			retVal = icoTrPdoCheckObjLimit_u16(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_TPDO:
			retVal = icoTrPdoCheckObjLimitCobid(pDesc->tableIdx, u32);
			break;
		case CO_DTYPE_U8_TMAP:
			retVal = icoTrMapCheckObjLimitMapTable(pDesc->tableIdx,
				pDesc->subIndex, *pData);
			break;
		case CO_DTYPE_U32_TMAP:
			retVal = icoTrMapCheckObjLimitMapEntry(pDesc->tableIdx, u32);
			break;
#endif /* CO_PDO_TRANSMIT_CNT */

#ifdef CO_PDO_RECEIVE_CNT
		case CO_DTYPE_U8_RPDO:
			/* transtype/sync start value */
			retVal = icoRecPdoCheckObjLimit_u8(pDesc->tableIdx, pDesc->subIndex,				*pData);
			break;
		case CO_DTYPE_U16_RPDO:
			/* inhibit/event timer */
			retVal = icoRecPdoCheckObjLimit_u16(pDesc->tableIdx, pDesc->subIndex);
			break;
		case CO_DTYPE_U32_RPDO:
			retVal = icoRecPdoCheckObjLimitCobid(pDesc->tableIdx, u32);
			break;
		case CO_DTYPE_U8_RMAP:
			retVal = icoRecMapCheckObjLimitMapTable(pDesc->tableIdx,
				pDesc->subIndex, *pData);
			break;
		case CO_DTYPE_U32_RMAP:
			retVal = icoRecMapCheckObjLimitMapEntry(pDesc->tableIdx, u32);
			break;
#endif /* CO_PDO_RECEIVE_CNT */

#ifdef CO_HB_CONSUMER_CNT
		case CO_DTYPE_U32_ERRCTRL:
			retVal = icoErrCtrlCheckObjLimit(pDesc->subIndex, u32);
			break;
#endif /* CO_HB_CONSUMER_CNT */

#ifdef CO_EMCY_PRODUCER
		case CO_DTYPE_U8_EMCY:
			retVal = icoEmcyCheckObjLimitHist(pDesc->subIndex, *pData);
			break;
#endif /* CO_EMCY_PRODUCER */
#if defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT)
		case CO_DTYPE_U32_EMCY:
			retVal = icoEmcyCheckObjLimitCobid(pDesc->tableIdx, pDesc->subIndex, u32);
			break;
#endif /* defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT) */

#ifdef CO_SYNC_SUPPORTED
		case CO_DTYPE_U8_SYNC:
			retVal = icoSyncCheckObjLimit_u8(pDesc->tableIdx, *pData);
			break;
		case CO_DTYPE_U32_SYNC:
			retVal = icoSyncCheckObjLimit_u32(pDesc->tableIdx, u32);
			break;
#endif /* CO_SYNC_SUPPORTED */

#ifdef CO_TIME_SUPPORTED
		case CO_DTYPE_U32_TIME:
			retVal = icoTimeCheckObjLimit_u32(u32);
			break;
#endif /* CO_TIME_SUPPORTED */

#ifdef CO_STORE_SUPPORTED
		case CO_DTYPE_U32_STORE:
			retVal = icoStoreCheckObjLimit_u32(pDesc->tableIdx, u32);
			break;
#endif /* CO_STORE_SUPPORTED */

		case CO_DTYPE_U8_ERRCTRL:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U16_EMCY:
			retVal = RET_OK;
			break;
		default:
			retVal = RET_OD_ACCESS_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObjSize - get object size
*
* This function returns the size of an object (in bytes),
* given by the object description.
*
*
* \return object size
*
*/
UNSIGNED32 coOdGetObjSize(
		CO_CONST CO_OBJECT_DESC_T	*pDesc	/**< pointer for description index */
	)
{
    UNSIGNED32	size = 0u;

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_CONST:
			/* table initialized ? */
			if (pOdConst_u8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_U16_CONST:
			/* table initialized ? */
			if (pOdConst_u16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_U32_CONST:
			/* table initialized ? */
			if (pOdConst_u32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_R32_CONST:
			/* table initialized ? */
			if (pOdConst_r32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_U8_VAR:
			/* table initialized ? */
			if (pOdVar_u8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_U16_VAR:
			/* table initialized ? */
			if (pOdVar_u16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_U32_VAR:
			/* table initialized ? */
			if (pOdVar_u32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_R32_VAR:
			/* table initialized ? */
			if (pOdVar_r32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_U8_PTR:
			/* table initialized ? */
			if (pOdPtr_u8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_U16_PTR:
			/* table initialized ? */
			if (pOdPtr_u16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_U32_PTR:
			/* table initialized ? */
			if (pOdPtr_u32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_I8_CONST:
			/* table initialized ? */
			if (pOdConst_i8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_I16_CONST:
			/* table initialized ? */
			if (pOdConst_i16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_I32_CONST:
			/* table initialized ? */
			if (pOdConst_i32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_I8_VAR:
			/* table initialized ? */
			if (pOdVar_i8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_I16_VAR:
			/* table initialized ? */
			if (pOdVar_i16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_I32_VAR:
			/* table initialized ? */
			if (pOdVar_i32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_I8_PTR:
			/* table initialized ? */
			if (pOdPtr_i8 != NULL)  {
				size = 1u;
			}
			break;
		case CO_DTYPE_I16_PTR:
			/* table initialized ? */
			if (pOdPtr_i16 != NULL)  {
				size = 2u;
			}
			break;
		case CO_DTYPE_I32_PTR:
			/* table initialized ? */
			if (pOdPtr_i32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_R32_PTR:
			/* table initialized ? */
			if (pOdPtr_r32 != NULL)  {
				size = 4u;
			}
			break;
		case CO_DTYPE_VS_PTR:
			if (pOdPtr_VS_Len != NULL)  {
				size = pOdPtr_VS_Len[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_VS_CONST:
			if (pOdConst_VS_Len != NULL)  {
				size = pOdConst_VS_Len[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_OS_PTR:
			if (pOdLen_OS != NULL)  {
				size = pOdLen_OS[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_DOMAIN:
			if (pOdPtr_Domain != NULL)  {
				size = pOdLen_Domain[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			}
			break;
		case CO_DTYPE_U8_SDO_SERVER:
		case CO_DTYPE_U8_SDO_CLIENT:
		case CO_DTYPE_U8_TPDO:
		case CO_DTYPE_U8_RPDO:
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U8_SYNC:
		case CO_DTYPE_U8_EMCY:
		case CO_DTYPE_U8_ERRCTRL:
			size = 1u;
			break;
		case CO_DTYPE_U16_TPDO:
		case CO_DTYPE_U16_RPDO:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U16_EMCY:
			size = 2u;
			break;
		case CO_DTYPE_U32_SDO_SERVER:
		case CO_DTYPE_U32_SDO_CLIENT:
		case CO_DTYPE_U32_TPDO:
		case CO_DTYPE_U32_TMAP:
		case CO_DTYPE_U32_RPDO:
		case CO_DTYPE_U32_RMAP:
		case CO_DTYPE_U32_ERRCTRL:
		case CO_DTYPE_U32_SYNC:
		case CO_DTYPE_U32_EMCY:
		case CO_DTYPE_U32_TIME:
		case CO_DTYPE_U32_STORE:
			size = 4u;
			break;
		default:
			size = 0u;
	}

	return(size);
}


/***************************************************************************/
/**
* internal
*
* \brief icoOdGetObjRecMapData - get data of receive mapping entry
*
* This function returns data of mapping entry
* If index is not mapable, returns error
*
* \return RET_T
*
*/
RET_T icoOdGetObjRecMapData(
		UNSIGNED16	index,		/* index of mapped variable */
		UNSIGNED8	subIndex,	/* subindex of mapped variable */
		void		**pVar,		/* pointer to mapped variable */
		UNSIGNED8	*pLen,		/* len of mapped variable */
		BOOL_T		*pNumeric	/* numeric sign for mapped variable */
	)
{
    CO_CONST CO_OBJECT_DESC_T	*pDesc;
    RET_T	retVal;
    UNSIGNED16	attr;

	/* check for dummy mapping */
	if (index < 0x20u) {
		*pVar = NULL;
		*pNumeric = CO_TRUE;
		retVal = RET_OK;

		switch (index)  {
			case 1u:				/* bool */
				*pLen = 0x01u;
				break;
			case 2u:				/* 8 bit */
			case 5u:				/* 8 bit */
				*pLen = 0x01u;
				break;
			case 3u:				/* 16 bit */
			case 6u:				/* 16 bit */
				*pLen = 0x02u;
				break;
			case 4u:				/* 32 bit */
			case 7u:				/* 32 bit */
				*pLen = 0x04u;
				break;
			default:
				retVal = RET_MAP_ERROR;
				break;
		}

		return(retVal);
	}

	/* get description stucture */
	retVal = coOdGetObjDescPtr(index, subIndex, &pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	attr = coOdGetObjAttribute(pDesc);

	/* check for mapable */
	if ((attr & (CO_ATTR_MAP | CO_ATTR_MAP_REC)) == 0u)  {
		return(RET_MAP_ERROR);
	}

	/* check for writable */
	if ((attr & CO_ATTR_WRITE) == 0u)  {
		return(RET_NO_WRITE_PERM);
	}

	if ((attr & CO_ATTR_NUM) == 0u)  {
		*pNumeric = CO_FALSE;
	} else {
		*pNumeric = CO_TRUE;
	}

	*pVar = odGetObjAddr(pDesc);
	*pLen = (UNSIGNED8)coOdGetObjSize(pDesc);

	return(RET_OK);
}

/***************************************************************************/
/**
* internal
*
* \brief icoOdGetObjTrMapData - get data of transmit mapping entry
*
* This function returns data of mapping entry
* If index is not mapable, returns error
*
* \return RET_T
*
*/
RET_T icoOdGetObjTrMapData(
		UNSIGNED16	index,		/* index of mapped variable */
		UNSIGNED8	subIndex,	/* subindex of mapped variable */
		CO_CONST void	**pVar,		/* pointer to mapped variable */
		UNSIGNED8	*pLen,		/* len of mapped variable */
		BOOL_T		*pNumeric	/* numeric sign for mapped variable */
	)
{
    CO_CONST CO_OBJECT_DESC_T	*pDesc;
    RET_T	retVal;
    UNSIGNED16	attr;

	/* check for dummy mapping */
	if (index < 0x20u) {
		return(RET_MAP_ERROR);
	}

	/* get description stucture */
	retVal = coOdGetObjDescPtr(index, subIndex, &pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	attr = coOdGetObjAttribute(pDesc);

	/* check for mapable */
	if ((attr & (CO_ATTR_MAP | CO_ATTR_MAP_TR)) == 0u)  {
		return(RET_MAP_ERROR);
	}
	/* check for redable */
	if ((attr & CO_ATTR_READ) == 0u)  {
		return(RET_NO_READ_PERM);
	}

	if ((attr & CO_ATTR_NUM) == 0u)  {
		*pNumeric = CO_FALSE;
	} else {
		*pNumeric = CO_TRUE;
	}

	*pVar = odGetObjAddr_R(pDesc);
	*pLen = (UNSIGNED8)coOdGetObjSize(pDesc);

	return(RET_OK);
}


/***************************************************************************/
/**
* internal
*
* \brief icoOdCheckObjAttr - check object for given attributes
*
* This function checks an object for the given attributes
*
* \return RET_T
*
*/
RET_T icoOdCheckObjAttr(
		UNSIGNED16	index,		/* index */
		UNSIGNED8	subIndex,	/* subindex */
		UNSIGNED16	checkAttr	/* attribute to check */
	)
{
    CO_CONST CO_OBJECT_DESC_T	*pDesc;
    RET_T	retVal;
    UNSIGNED16	attr;

	/* generic data types are ok */
	if (index < 0x20u) {
		return(RET_OK);
	}

	/* get description stucture */
	retVal = coOdGetObjDescPtr(index, subIndex, &pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	attr = coOdGetObjAttribute(pDesc);

	/* mask only check attributes */
	attr &= checkAttr;

	/* if not all attr are set, return error */
	if ((attr ^ checkAttr) != 0u)  {
		retVal = RET_MAP_ERROR;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetDefaultVal_u8 - get default value for specific object 
*
* This function returns the default value of an UNSIGNED8 object.
*
* \return RET_T
*
*/
RET_T coOdGetDefaultVal_u8(
		UNSIGNED16	index,			/**< index */
		UNSIGNED8	subIndex,		/**< sub index */
		UNSIGNED8	*pDefVal		/**< pointer to default val */
	)
{
    RET_T	retVal;
    CO_CONST CO_OBJECT_DESC_T *pDesc;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U8_VAR:
		case CO_DTYPE_U8_PTR:
		case CO_DTYPE_U8_CONST:
		case CO_DTYPE_U8_SYNC:
		case CO_DTYPE_U8_TPDO:
		case CO_DTYPE_U8_RPDO:
		case CO_DTYPE_U8_TMAP:
		case CO_DTYPE_U8_RMAP:
		case CO_DTYPE_U8_EMCY:
		case CO_DTYPE_U8_SDO_CLIENT:
		case CO_DTYPE_U8_ERRCTRL:
			*pDefVal = pOdConst_u8[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			break;
		default:
			retVal = RET_SDO_DATA_TYPE_NOT_MATCH;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetDefaultVal_u16 - get default value for specific object 
*
* This function returns the default value of an UNSIGNED16 object.
*
* \return RET_T
*
*/
RET_T coOdGetDefaultVal_u16(
		UNSIGNED16	index,			/**< index */
		UNSIGNED8	subIndex,		/**< sub index */
		UNSIGNED16	*pDefVal		/**< pointer to default val */
	)
{
    RET_T	retVal;
    CO_CONST CO_OBJECT_DESC_T *pDesc;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U16_VAR:
		case CO_DTYPE_U16_PTR:
		case CO_DTYPE_U16_CONST:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U16_TPDO:
		case CO_DTYPE_U16_RPDO:
		case CO_DTYPE_U16_EMCY:
			*pDefVal = pOdConst_u16[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			break;
		default:
			retVal = RET_SDO_DATA_TYPE_NOT_MATCH;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetDefaultVal_u32 - get default value for specific object 
*
* This function returns the default value of an UNSIGNED32 object.
*
* \return RET_T
*
*/
RET_T coOdGetDefaultVal_u32(
		UNSIGNED16	index,			/**< index */
		UNSIGNED8	subIndex,		/**< sub index */
		UNSIGNED32	*pDefVal		/**< pointer to default val */
	)
{
    RET_T	retVal;
    CO_CONST CO_OBJECT_DESC_T *pDesc;

	retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	switch (pDesc->dType)  {
		case CO_DTYPE_U32_VAR:
		case CO_DTYPE_U32_PTR:
		case CO_DTYPE_U32_CONST:
		case CO_DTYPE_U32_ERRCTRL:
		case CO_DTYPE_U32_SYNC:
		case CO_DTYPE_U32_EMCY:
		case CO_DTYPE_U32_TMAP:
		case CO_DTYPE_U32_RMAP:
		case CO_DTYPE_U32_TIME:
		case CO_DTYPE_U32_RPDO:
		case CO_DTYPE_U32_TPDO:
			*pDefVal = pOdConst_u32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
			break;
		default:
			retVal = RET_SDO_DATA_TYPE_NOT_MATCH;
	}

	return(retVal);
}


/***************************************************************************/
/**
* \brief coOdGetObjDescPtr - get object description pointer
*
* This function returns a pointer to the object description
* of an object of the object dictionary.
*
* \return RET_T
*
*/
RET_T coOdGetObjDescPtr(
		UNSIGNED16	index,			/**< index */
		UNSIGNED8	subIndex,		/**< sub index */
		CO_CONST CO_OBJECT_DESC_T **pDescPtr	/**< pointer for description index */
	)
{
    UNSIGNED16	idx = 0u;
    RET_T	retVal;

	retVal =  odGetObjDescIdx(index, subIndex, &idx);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	*pDescPtr = &pObjDescTable[idx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief coOdGetObjDescIdx - get object description index
*
*
* \return RET_T
*
*/
static CO_INLINE RET_T odGetObjDescIdx(
		UNSIGNED16	index,		/* index */
		UNSIGNED8	subIndex,	/* sub index */
		UNSIGNED16	*pDescIdx	/* pointer for description index */
	)
{
    UNSIGNED16	left = 0u;
    UNSIGNED16	right = odAssignCnt - 1u;
    UNSIGNED16	middle = 0u;
    BOOL_T		found = CO_FALSE;
    BOOL_T		breakLoop = CO_FALSE;
    RET_T		retVal;

	/* binary search index */
	while (left <= right)  {
		/* l + r / 2 */
		middle = (left + right) >> 1;
		if (pOdAssingTable[middle].index == index)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			found = CO_TRUE;
			breakLoop = CO_TRUE;	/* break; */
		}
		if (pOdAssingTable[middle].index > index)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			if (middle == 0u)  {
				breakLoop = CO_TRUE;	/* break; */
			}
			right = middle - 1u;
		} else {
			left = middle + 1u;
		}

		/* finish loop ? */
		if (breakLoop == CO_TRUE)  {
			break;
		}
	}

	if (found != CO_TRUE)  {
		return(RET_IDX_NOT_FOUND);
	}

	/* binary search subindex */
	left = pOdAssingTable[middle].odDescIdx; /*lint !e960 Note: pointer arithmetic other than array indexing used */
	right = (left + pOdAssingTable[middle].numberOfSubs) - 1u; /*lint !e960 Note: pointer arithmetic other than array indexing used */
	found = CO_FALSE;

	breakLoop = CO_FALSE;
	while (left <= right)  {
		/* l + r / 2 */
		middle = (left + right) >> 1;
		if (pObjDescTable[middle].subIndex == subIndex)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			found = CO_TRUE;
			breakLoop = CO_TRUE;	/* break; */
		}
		if (pObjDescTable[middle].subIndex > subIndex)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			if (middle == 0u)  {
				breakLoop = CO_TRUE;	/* break; */
			}
			right = middle - 1u;
		} else {
			left = middle + 1u;
		}

		/* finish loop ? */
		if (breakLoop == CO_TRUE)  {
			break;
		}
	}

    if (found == CO_FALSE) {
		retVal = RET_SUBIDX_NOT_FOUND;
	} else {
		*pDescIdx = middle;
		retVal = RET_OK;
	}

	return(retVal);
}


#ifdef CO_EVENT_OBJECT_CHANGED
/***************************************************************************/
/**
* \brief coEventRegister_OBJECT_CHANGED - register object changed function
*
* This function registered a indication function for a given object.
* Each time, this object is changed by PDO, SDO or by coOdPutObj_xx()
* the given function is called.
*
* If the subindex == 255, then the indication is called for each subindex.
*
* \return RET_T
*
*/

RET_T coEventRegister_OBJECT_CHANGED(
		CO_EVENT_OBJECT_CHANGED_FCT_T	pFunction,	/**< pointer to function */
		UNSIGNED16		index,					/**< index */
		UNSIGNED8		subIndex				/**< subIndex */
    )
{
    UNSIGNED16	idx = 255u;
    RET_T	retVal;
    CO_EVENT_OBJECT_CHANGED_T tObj;
    CO_EVENT_OBJECT_CHANGED_FCT_T pFct;
    CO_CONST UNSIGNED32	l = sizeof(CO_EVENT_OBJECT_CHANGED_T);

	/* check if object exist */
	if (subIndex == 255u)  {
		retVal = odGetObjDescIdx(index, 0u, &idx);
	} else {
		retVal = odGetObjDescIdx(index, subIndex, &idx);
	}
	if (retVal != RET_OK)  {
		return(RET_OD_ACCESS_ERROR);
	}

	/* index/sub already at list, return error */
	pFct = icoEventObjectChangedFunction(index, subIndex);
	if (pFct != NULL)  {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* list full, abort */
	if (objectEventTableCnt == CO_EVENT_OBJECT_CHANGED)  {
		return(RET_EVENT_NO_RESSOURCE);
	}

	/* sort into last position from list */
	idx = objectEventTableCnt;
	objectEventTableCnt++;
	objectEventTable[idx].index = index;
	objectEventTable[idx].subIndex = subIndex;
	objectEventTable[idx].pFct = pFunction;
	while (idx > 0u)  {
		/* if value < predecessor, exchange with him */
		if (objectEventTable[idx].index < objectEventTable[idx - 1u].index)  {
			memcpy(&tObj, &objectEventTable[idx], l);
			memcpy(&objectEventTable[idx], &objectEventTable[idx - 1u], l);
			memcpy(&objectEventTable[idx - 1u], &tObj, l);
		} else {
			if (objectEventTable[idx].index == objectEventTable[idx - 1u].index)  {
				/* subindex = 255 is valid for each subindex */
				if (objectEventTable[idx].subIndex != 255u)  {
					/* check the same with subindex */
					if (objectEventTable[idx].subIndex < objectEventTable[idx - 1u].subIndex)  {
						memcpy(&tObj, &objectEventTable[idx], l);
						memcpy(&objectEventTable[idx], &objectEventTable[idx - 1u], l);
						memcpy(&objectEventTable[idx - 1u], &tObj, l);
					}
				}
			}
		}
		idx--;
	}

	return(RET_OK);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoEventObjectGetIndex - get object event function
*
* \return RET_T
*
*/
CO_EVENT_OBJECT_CHANGED_FCT_T icoEventObjectChangedFunction(
		UNSIGNED16	index,			/* index */
		UNSIGNED8	subIndex		/* subIndex */
	)
{
    UNSIGNED16	idx = 0u;

	while (idx < objectEventTableCnt)  {
		if (objectEventTable[idx].index == index)  {
			if ((objectEventTable[idx].subIndex == subIndex)
		 	 || (objectEventTable[idx].subIndex == 255u))  {
				return(objectEventTable[idx].pFct);
			}
		}
		idx++;
	}

	return(NULL);
}

#endif /* CO_EVENT_OBJECT_CHANGED */


/***************************************************************************/
/**
* \internal
*
* \brief coEventObjectChanged - event object changed
*
* \return RET_T
*
*/
RET_T icoEventObjectChanged(
		CO_CONST CO_OBJECT_DESC_T	*pDesc,	/* pointer for description index */
		UNSIGNED16					index,	/* object index */
		BOOL_T						changed	/* object has new value */
	)
{
    RET_T		retVal = RET_OK;
#ifdef CO_EVENT_OBJECT_CHANGED
#else /* CO_EVENT_OBJECT_CHANGED */
    (void)index;
    (void)changed;
#endif /* CO_EVENT_OBJECT_CHANGED */

#ifdef CO_EVENT_STORE
	/* don't execute function at load parameter state */
	if (loadParameterState == CO_TRUE)  {
		return(retVal);
	}
#endif /* CO_EVENT_STORE */

	/* first, call objects at 0x1000..0x1fff */
	switch (pDesc->dType)  {
		case CO_DTYPE_U32_SDO_SERVER:
			retVal = icoSdoObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;
#ifdef CO_SDO_CLIENT_CNT
		case CO_DTYPE_U32_SDO_CLIENT:
			retVal = icoSdoClientObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_SDO_CLIENT_CNT */

#ifdef CO_PDO_TRANSMIT_CNT
		case CO_DTYPE_U8_TPDO:
		case CO_DTYPE_U16_TPDO:
		case CO_DTYPE_U32_TPDO:
			retVal = icoTrPdoObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;

# ifdef CO_PDO_DYN_MAP_ENTRIES
		case CO_DTYPE_U8_TMAP:
			retVal = icoTrPdoMapChanged(pDesc->tableIdx);
			break;
# endif /* CO_PDO_DYN_MAP_ENTRIES */
#endif /* CO_PDO_TRANSMIT_CNT */

#ifdef CO_PDO_RECEIVE_CNT
		case CO_DTYPE_U8_RPDO:
		case CO_DTYPE_U16_RPDO:
		case CO_DTYPE_U32_RPDO:
			retVal = icoRecPdoObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;

# ifdef CO_REC_PDO_DYN_MAP_ENTRIES
		case CO_DTYPE_U8_RMAP:
			retVal = icoRecPdoMapChanged(pDesc->tableIdx);
			break;
# endif /* CO_PDO_DYN_MAP_ENTRIES */
#endif /* CO_PDO_RECEIVE_CNT */

		case CO_DTYPE_U8_ERRCTRL:
		case CO_DTYPE_U16_ERRCTRL:
		case CO_DTYPE_U32_ERRCTRL:
			retVal = icoErrCtrlObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;

#ifdef CO_SYNC_SUPPORTED
		case CO_DTYPE_U8_SYNC:
		case CO_DTYPE_U32_SYNC:
			retVal = icoSyncObjChanged(pDesc->tableIdx);
			break;
#endif /* CO_SYNC_SUPPORTED */

		case CO_DTYPE_U8_EMCY:
		case CO_DTYPE_U16_EMCY:
#ifdef CO_EMCY_PRODUCER
			retVal = icoEmcyObjChanged(pDesc->tableIdx, pDesc->subIndex);
#endif /* CO_EMCY_PRODUCER */
			break;
		case CO_DTYPE_U32_EMCY:
#if defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT)
			retVal = icoEmcyObjChanged(pDesc->tableIdx, pDesc->subIndex);
#endif /* defined(CO_EMCY_PRODUCER) || defined (CO_EMCY_CONSUMER_CNT) */
			break;

#ifdef CO_TIME_SUPPORTED
		case CO_DTYPE_U32_TIME:
			retVal = icoTimeObjChanged();
			break;
#endif /* CO_TIME_SUPPORTED */

#ifdef CO_STORE_SUPPORTED
		case CO_DTYPE_U32_STORE:
			retVal = icoStoreObjChanged(pDesc->tableIdx, pDesc->subIndex);
			break;
#endif /* CO_STORE_SUPPORTED */

		default:
			break;
	}

	if (retVal != RET_OK)  {
		return(retVal);
	}

#ifdef CO_EVENT_OBJECT_CHANGED
	if (changed == CO_TRUE)  {
		CO_EVENT_OBJECT_CHANGED_FCT_T pFct;

		pFct = icoEventObjectChangedFunction(index, pDesc->subIndex);
		if (pFct != NULL)  {
			retVal = pFct(index, pDesc->subIndex);
		}
	}
#endif /* CO_EVENT_OBJECT_CHANGED */

	return(retVal);
}


/***************************************************************************/
/**
* \internal
*
* \brief icoOdGetNumberOfSubs - get number of subindexe
*
*
* \return number of subs
*
*/
UNSIGNED8	icoOdGetNumberOfSubs(
		UNSIGNED16 index
	)
{
    UNSIGNED16	i;

	for (i = 0u; i < odAssignCnt; i++)  {
		if (pOdAssingTable[i].index == index)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			return(pOdAssingTable[i].numberOfSubs); /*lint !e960 Note: pointer arithmetic other than array indexing used */
		}
	}

	return(0u);
}


#ifdef CO_EVENT_STORE
/***************************************************************************/
/**
* \internal
*
* \brief icoOdReset - reset obect dictionary variables
*
*
* \return none
*
*/
void icoOdSetLoadParaState(
		BOOL_T	newState
	)
{
	loadParameterState = newState;
}
#endif /* CO_EVENT_STORE */


/***************************************************************************/
/**
* \internal
*
* \brief icoOdReset - reset obect dictionary variables
*
*
* \return none
*
*/
void icoOdReset(
		UNSIGNED16	startIdx,		/* start index */
		UNSIGNED16	lastIdx			/* last index */
	)
{
    UNSIGNED16			i;
    UNSIGNED16			sub = 0u;
    const CO_OBJECT_DESC_T	*pDesc;
    UNSIGNED8	*p_u8;
    UNSIGNED16	*p_u16;
    UNSIGNED32	*p_u32;
    INTEGER8	*p_i8;
    INTEGER16	*p_i16;
    INTEGER32	*p_i32;
    REAL32		*p_r32;

	/* start at index */
	for (i = 0u; i < odAssignCnt; i++)  {
		if ((pOdAssingTable[i].index >= startIdx) /*lint !e960 Note: pointer arithmetic other than array indexing used */
		 && (pOdAssingTable[i].index <= lastIdx))  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
			/* reset all subs */
			for (sub = 0u; sub < pOdAssingTable[i].numberOfSubs; sub++)  { /*lint !e960 Note: pointer arithmetic other than array indexing used */
				pDesc = &pObjDescTable[pOdAssingTable[i].odDescIdx + sub]; /*lint !e960 Note: pointer arithmetic other than array indexing used */

				/* reset only objects with default value */
				if ((pDesc->attr & CO_ATTR_DEFVAL) != 0u)  {
					switch (pDesc->dType)  {
						case CO_DTYPE_U8_VAR:
							pOdVar_u8[pDesc->tableIdx] = pOdConst_u8[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I8_VAR:
							pOdVar_i8[pDesc->tableIdx] = pOdConst_i8[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_U16_VAR:
							pOdVar_u16[pDesc->tableIdx] = pOdConst_u16[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I16_VAR:
							pOdVar_i16[pDesc->tableIdx] = pOdConst_i16[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_U32_VAR:
							pOdVar_u32[pDesc->tableIdx] = pOdConst_u32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_R32_VAR:
							pOdVar_r32[pDesc->tableIdx] = pOdConst_r32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I32_VAR:
							pOdVar_i32[pDesc->tableIdx] = pOdConst_i32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_U8_PTR:
							p_u8 = pOdPtr_u8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_u8 = pOdConst_u8[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I8_PTR:
							p_i8 = pOdPtr_i8[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_i8 = pOdConst_i8[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_U16_PTR:
							p_u16 = pOdPtr_u16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_u16 = pOdConst_u16[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I16_PTR:
							p_i16 = pOdPtr_i16[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_i16 = pOdConst_i16[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_U32_PTR:
							p_u32 = pOdPtr_u32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_u32 = pOdConst_u32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_I32_PTR:
							p_i32 = pOdPtr_i32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_i32 = pOdConst_i32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						case CO_DTYPE_R32_PTR:
							p_r32 = pOdPtr_r32[pDesc->tableIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							*p_r32 = pOdConst_r32[pDesc->defValIdx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */
							break;
						default:
							break;
					}
				}
			}
		}
	}
}


/***************************************************************************/
/**
* \brief coOdInitOdPtr - init all object dictionary and variable pointers
*
* This function initializes the object dictionary with all variables.
*
* \return none
*
*/
void coOdInitOdPtr(
		const CO_OD_ASSIGN_T	 *pOdAssing,		/**< pointer to OD assign */
		UNSIGNED16		 odCnt,						/**< number of objects */
		const CO_OBJECT_DESC_T *pObjdesc,			/**< pointer to obj descr */
		UNSIGNED16		 descCnt,					/**< number of obj desc */
		CO_EVENT_OBJECT_CHANGED_FCT_T *pEventPtr,	/**< pointer to obj events */
		const CO_OD_DATA_VARIABLES_T *pOdVarPointers	/**< pointer to variable types */
	)
{
    (void)descCnt;
    (void)pEventPtr;

	pOdAssingTable = pOdAssing;
	odAssignCnt = odCnt;
	pObjDescTable = pObjdesc;
	/* pObjEventPtr = pEventPtr; */

	pOdConst_u8 = pOdVarPointers->odConst_u8;
	pOdConst_u16 = pOdVarPointers->odConst_u16;
	pOdConst_u32 = pOdVarPointers->odConst_u32;
	pOdConst_r32 = pOdVarPointers->odConst_r32;
	pOdVar_u8 = pOdVarPointers->odVar_u8;
	pOdVar_u16 = pOdVarPointers->odVar_u16;
	pOdVar_u32 = pOdVarPointers->odVar_u32;
	pOdVar_r32 = pOdVarPointers->odVar_r32;
	pOdPtr_u8 =  pOdVarPointers->odPtr_u8;
	pOdPtr_u16 =  pOdVarPointers->odPtr_u16;
	pOdPtr_u32 =  pOdVarPointers->odPtr_u32;
	pOdPtr_r32 =  pOdVarPointers->odPtr_r32;

	pOdConst_i8 = pOdVarPointers->odConst_i8;
	pOdConst_i16 = pOdVarPointers->odConst_i16;
	pOdConst_i32 = pOdVarPointers->odConst_i32;
	pOdVar_i8 = pOdVarPointers->odVar_i8;
	pOdVar_i16 = pOdVarPointers->odVar_i16;
	pOdVar_i32 = pOdVarPointers->odVar_i32;
	pOdPtr_i8 =  pOdVarPointers->odPtr_i8;
	pOdPtr_i16 =  pOdVarPointers->odPtr_i16;
	pOdPtr_i32 =  pOdVarPointers->odPtr_i32;

	pOdConst_VS = pOdVarPointers->odConstVisString;
	pOdConst_VS_Len = pOdVarPointers->odConstVisStringLen;
	pOdPtr_VS = pOdVarPointers->odVisString;
	pOdPtr_VS_Len = pOdVarPointers->odVisStringLen;

	pOdPtr_OS = pOdVarPointers->odOctetString;
	pOdLen_OS = pOdVarPointers->odOctetStringLen;
	pOdPtr_Domain = pOdVarPointers->domainPtr;
	pOdLen_Domain = pOdVarPointers->domainLen;

	/* reset application variables */
	icoOdReset(0x1000u, 0x9fffu);
}
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
