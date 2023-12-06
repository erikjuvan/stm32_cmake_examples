/*
 * co_direct_read.c
 *
 *  Created on: 28 Mar 2022
 *      Author: amadeja
 */

#include "co_direct_read.h"
#include "ico_odaccess.h"


/* number of objects */
#define CO_OD_ASSIGN_CNT 123u
#define CO_OBJ_DESC_CNT 560u


static CO_CONST CO_OD_ASSIGN_T* pOdAssingTable;
static CO_CONST CO_OBJECT_DESC_T* pObjDescTable;
static UNSIGNED16 odAssignCnt;

static CO_CONST UNSIGNED8       *pOdConst_u8 = NULL;
static CO_CONST UNSIGNED16      *pOdConst_u16 = NULL;
static CO_CONST UNSIGNED32      *pOdConst_u32 = NULL;
static CO_CONST REAL32          *pOdConst_r32 = NULL;
static CO_CONST INTEGER8        *pOdConst_i8 = NULL;
static CO_CONST INTEGER16       *pOdConst_i16 = NULL;
static CO_CONST INTEGER32       *pOdConst_i32 = NULL;
static UNSIGNED8                *pOdVar_u8 = NULL;
static UNSIGNED16               *pOdVar_u16 = NULL;
static UNSIGNED32               *pOdVar_u32 = NULL;
static REAL32                   *pOdVar_r32 = NULL;
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


RET_T co_direct_coOdGetObjDescPtr(UNSIGNED16 index, UNSIGNED8	subIndex, CO_CONST CO_OBJECT_DESC_T **pDescPtr);
static CO_INLINE RET_T co_direct_odGetObjDescIdx(UNSIGNED16 index, UNSIGNED8 subIndex, UNSIGNED16 *pDescIdx);
RET_T co_direct_icoOdGetObj(CO_CONST CO_OBJECT_DESC_T *pDesc, void *pData, UNSIGNED32 offset, UNSIGNED32 maxData,	BOOL_T internal);
RET_T co_direct_icoOdPutObj(CO_CONST CO_OBJECT_DESC_T *pDesc, CO_CONST void *pData, UNSIGNED32 offset, UNSIGNED32 maxData, BOOL_T internal, BOOL_T *pChanged);
static CO_CONST UNSIGNED8 *co_direct_odGetObjAddr_R(CO_CONST CO_OBJECT_DESC_T *pDesc);
static UNSIGNED8 *co_direct_odGetObjAddr(CO_CONST CO_OBJECT_DESC_T *pDesc);
UNSIGNED32 co_direct_coOdGetObjSize(CO_CONST CO_OBJECT_DESC_T *pDesc);
CO_INLINE BOOL_T co_direct_coNumMemcpy(void *pDest, CO_CONST void *pSrc, UNSIGNED32 size, UNSIGNED16 numeric);
CO_INLINE void co_direct_coNumMemcpyUnpack(void *pDest, CO_CONST void *pSrc, UNSIGNED32 size, UNSIGNED16 numeric, UNSIGNED8 packOffset);
CO_INLINE BOOL_T co_direct_coNumMemcpyPack(void *pDest, CO_CONST void *pSrc, UNSIGNED32 size, UNSIGNED16 numeric, UNSIGNED8 packOffset);
void co_direct_icoOdReset(UNSIGNED16 startIdx, UNSIGNED16 lastIdx);


RET_T co_get_obj_data(
		UNSIGNED16		index,			/**< index */
		UNSIGNED8		subIndex,		/**< sub index */
		CO_DATA_TYPE_T 	*data_type,		/**< pointer to data type */
		void			*pData			/**< pointer to data */
	)
{
	CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;

#if defined(DEBUG)
    retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
#else
	retVal = co_direct_coOdGetObjDescPtr(index, subIndex,	&pDesc);
#endif
	if (retVal != RET_OK)
	{
		return(retVal);
	}

	*data_type = pDesc->dType;

#if defined(DEBUG)
	retVal = icoOdGetObj(pDesc, pData, 0u, 0u, CO_TRUE);
#else
	retVal = co_direct_icoOdGetObj(pDesc, pData, 0u, 0u, CO_TRUE);
#endif

	return retVal;
}

RET_T co_set_obj_data(
		UNSIGNED16	index,		/**< index of object */
		UNSIGNED8	subIndex,	/**< subindex of object */
		void 		*newVal		/**< new value */
	)
{
    CO_CONST CO_OBJECT_DESC_T *pDesc;
    RET_T	retVal;
    BOOL_T	changed = CO_FALSE;

#if defined(DEBUG)
    retVal = coOdGetObjDescPtr(index, subIndex,	&pDesc);
#else
    retVal = co_direct_coOdGetObjDescPtr(index, subIndex,	&pDesc);
#endif
	if (retVal != RET_OK)
	{
		return(retVal);
	}

#if defined(DEBUG)
	retVal = icoOdPutObj(pDesc, newVal, 0u, 0u, CO_TRUE, &changed);
#else
	retVal = co_direct_icoOdPutObj(pDesc, newVal, 0u, 0u, CO_TRUE, &changed);
#endif

	return(retVal);
}

//#if !defined(DEBUG)

/**
* \brief co_direct_coOdGetObjDescPtr - get object description pointer
*
* This function returns a pointer to the object description
* of an object of the object dictionary.
*
* \return RET_T
*
*/
RET_T co_direct_coOdGetObjDescPtr(
		UNSIGNED16	index,			/**< index */
		UNSIGNED8	subIndex,		/**< sub index */
		CO_CONST CO_OBJECT_DESC_T **pDescPtr	/**< pointer for description index */
	)
{
    UNSIGNED16	idx = 0u;
    RET_T	retVal;

	retVal =  co_direct_odGetObjDescIdx(index, subIndex, &idx);
	if (retVal != RET_OK)  {
		return(retVal);
	}

	*pDescPtr = &pObjDescTable[idx]; /*lint !e960 Note: pointer arithmetic other than array indexing used */

	return(RET_OK);
}

/**
* \internal
*
* \brief coco_direct_odGetObjDescIdx - get object description index
*
*
* \return RET_T
*
*/
static CO_INLINE RET_T co_direct_odGetObjDescIdx(
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

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_icoOdGetObj - get data from object
*
* copy data from OD to given pointer
*
*
* \return RET_T
*
*/
RET_T co_direct_icoOdGetObj(
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

	pObjAddr = co_direct_odGetObjAddr_R(pDesc);
	if (pObjAddr != NULL)  {
		/* get object size */
		size = co_direct_coOdGetObjSize(pDesc);

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

		if (internal == CO_TRUE)  {
			(void)co_direct_coNumMemcpy(pData, pObjAddr, size, (attr & CO_ATTR_NUM));
		} else {
			co_direct_coNumMemcpyUnpack(pData, pObjAddr, size,
				(attr & CO_ATTR_NUM), packOffset);
		}

		retVal = RET_OK;
	}

	return(retVal);
}

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_odGetObjAddr_R - get object address for read objects
*
* \return RET_T
*
*/
static CO_CONST UNSIGNED8 *co_direct_odGetObjAddr_R(
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

		default:
			pObjAddr = co_direct_odGetObjAddr(pDesc);
			break;
	}

	return(pObjAddr);
}

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_odGetObjAddr - get object address (read/write)
*
* \return RET_T
*
*/
static UNSIGNED8 *co_direct_odGetObjAddr(
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

		default:
			break;
	}

	return(pObjAddr);
}

/***************************************************************************/
/**
* \brief co_direct_coOdGetObjSize - get object size
*
* This function returns the size of an object (in bytes),
* given by the object description.
*
*
* \return object size
*
*/
UNSIGNED32 co_direct_coOdGetObjSize(
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

		default:
			size = 0u;
	}

	return(size);
}

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_coNumMemcpy - memcpy with numeric dependencies
*
* This function provide an internal memcpy.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or unpackaging is done.
*
* \return none
*
*/
CO_INLINE BOOL_T co_direct_coNumMemcpy(
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
* \internal
*
* \brief co_direct_coNumMemcpyUnpack - memcpy with numeric dependencies
*
* This function provide a memcpy from OD variable to CAN-Controller.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or unpackaging is done.
*
* \return none
*
*/
CO_INLINE void co_direct_coNumMemcpyUnpack(
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
* \brief coOdInitOdPtr - init all object dictionary and variable pointers
*
* This function initializes the object dictionary with all variables.
*
* \return none
*
*/
void co_direct_coOdInitOdPtr(
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

	/*
	pOdConst_VS = pOdVarPointers->odConstVisString;
	pOdConst_VS_Len = pOdVarPointers->odConstVisStringLen;
	pOdPtr_VS = pOdVarPointers->odVisString;
	pOdPtr_VS_Len = pOdVarPointers->odVisStringLen;

	pOdPtr_OS = pOdVarPointers->odOctetString;
	pOdLen_OS = pOdVarPointers->odOctetStringLen;
	pOdPtr_Domain = pOdVarPointers->domainPtr;
	pOdLen_Domain = pOdVarPointers->domainLen;
	*/

	/* reset application variables */
	co_direct_icoOdReset(0x1000u, 0x9fffu);
}

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_icoOdReset - reset obect dictionary variables
*
*
* \return none
*
*/
void co_direct_icoOdReset(
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
* \internal
*
* \brief co_direct_icoOdPutObj - put data to object
*
* copy data to OD from given pointer
*
*
* \return RET_T
*
*/
RET_T co_direct_icoOdPutObj(
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
			pObjAddr = co_direct_odGetObjAddr(pDesc);
			break;
	}

	if (pObjAddr != NULL)  {
		size = co_direct_coOdGetObjSize(pDesc);

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

		/* check for new data */
		if (internal == CO_TRUE)   {
			*pChanged = co_direct_coNumMemcpy((void *)pObjAddr, pData, size,
				(attr & CO_ATTR_NUM));
		} else {
			*pChanged = co_direct_coNumMemcpyPack((void *)pObjAddr, pData,size,
				(attr & CO_ATTR_NUM), packOffset);
		}

		retVal = RET_OK;
	}

	return(retVal);
}

/***************************************************************************/
/**
* \internal
*
* \brief co_direct_coNumMemcpyPack - memcpy with numeric dependencies
*
* This function provide a memcpy from CAN-Controller to OD variable.
* If the parameter numeric is unequal 0,
* then the data are copied as numeric value,
* so byte swapping or packaging is done.
*
* \return none
*
*/
CO_INLINE BOOL_T co_direct_coNumMemcpyPack(
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

//#endif
