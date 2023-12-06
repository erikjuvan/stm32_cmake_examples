////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file   co_datatype.h
*@brief  contains defines for data types
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
#ifndef CO_DATATYPE_H
#define CO_DATATYPE_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#ifdef __STDC_VERSION__
#  if (__STDC_VERSION__ >= 199901L)
#    define C99
#  endif /* if */
#endif /* ifdef */

#ifdef C99
# include <stdint.h>
#endif /* C99 */

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifndef CO_INLINE
# define CO_INLINE
#endif

#ifndef CO_CONST
# define CO_CONST const
#endif

/**
* define for bool values 
*/
typedef enum {
	CO_FALSE,			/**< false */
	CO_TRUE				/**< true */
} BOOL_T;

#ifdef CO_CUSTOMER_DATATYPES
/* customer typedefs in gen_define.h */
#else /* CO_CUSTOMER_DATATYPES */

#ifdef C99
typedef uint8_t 			UNSIGNED8;
typedef uint16_t 			UNSIGNED16;
typedef uint32_t 			UNSIGNED32;
typedef int8_t				INTEGER8;
typedef int16_t				INTEGER16;
typedef int32_t				INTEGER32;
#else /* C99 */
typedef unsigned char		UNSIGNED8;
typedef unsigned short int	UNSIGNED16;
typedef unsigned long		UNSIGNED32;
typedef signed char			INTEGER8;
typedef signed short int	INTEGER16;
typedef signed long			INTEGER32;
#endif /* C99 */
typedef	char	 			*VIS_STRING;
typedef	char 				OCTET_STRING;
typedef	unsigned char 		*CO_DOMAIN_PTR;
typedef float				REAL32;

#endif /* CO_CUSTOMER_DATATYPES */

/**
* Defines for RET_T
*/
typedef enum {
	RET_OK,							/**< all ok */

	RET_INVALID_PARAMETER,			/**< error invalid parameter */
	RET_PARAMETER_INCOMPATIBLE,		/**< error incompatible parameter */

	RET_IDX_NOT_FOUND,				/**< error index not found */
	RET_SUBIDX_NOT_FOUND,			/**< error subindex not found */
	RET_OD_ACCESS_ERROR,			/**< error access at object dictionary */
	RET_NO_READ_PERM,				/**< error no read permission */
	RET_NO_WRITE_PERM,				/**< error no write permission */

	RET_SDO_UNKNOWN_CCS,			/**< error unknown command specifier */
	RET_SDO_DATA_TYPE_NOT_MATCH,	/**< error wrong data type */
	RET_SDO_INVALID_VALUE,			/**< error invalid value */
	RET_SDO_TRANSFER_NOT_SUPPORTED,	/**< error transfer not supported */
	RET_OUT_OF_MEMORY,				/**< error out of memory */
	RET_DATA_TYPE_MISMATCH,			/**< error data type mismatch */
	RET_TOGGLE_MISMATCH,			/**< error toogle bit not alternate */
	RET_SDO_CRC_ERROR,				/**< error CRC mismatch */
	RET_SDO_WRONG_BLOCKSIZE,		/**< error wrong blocksize */
	RET_SDO_WRONG_SEQ_NR,			/**< error wrong sequence number */
	RET_SDO_TIMEOUT,				/**< error sdo timeout */

	RET_NO_COB_AVAILABLE,			/**< error no cob available */
	RET_COB_DISABLED,				/**< error cob-id is disabled */

	RET_DRV_WRONG_BITRATE,			/**< error unknown bitrate */
	RET_DRV_ERROR,					/**< error driver */
	RET_DRV_TRANS_BUFFER_FULL,		/**< error transmit buffer full */
	RET_DRV_BUSY,					/**< error driver is busy */

	RET_EVENT_NO_RESSOURCE,			/**< error no ressource available */
	RET_INVALID_NMT_STATE,			/**< error invalid NMT state */

	RET_MAP_ERROR,					/**< error map entry incorrect */
	RET_MAP_LEN_ERROR,				/**< error mapping len incorrect */

	RET_SERVICE_BUSY,				/**< error service is busy */
	RET_INHIBIT_ACTIVE,				/**< error inhibit is active */

	RET_INTERNAL_ERROR,				/**< error internal */
	RET_HARDWARE_ERROR,				/**< error hardware access */
	RET_ERROR_PRESENT_DEVICE_STATE,	/**< error wrong device state */
	RET_VALUE_NOT_AVAILABLE,		/**< error value not available */
	RET_ERROR_STORE,				/**< error store data */

	RET_INVALID_NODEID,				/**< invalid node id */

	RET_SDO_FUNC_BLOCKED,   		/**< can't call SDO read write since other is using it */
	RET_SDO_IND_TIMEOUT,            /**< SDO timeout */
	RET_STRING_SET_TOO_LONG,        /**< internal required string size is too long         */
	RET_STRING_ADDRESS_INVALID,     /**< internal string setup address invalid             */
} RET_T;


/**
* transmit message flags:
* if the last message is not transmitted yet,
* overwrite the last data with the new data
*/
#define MSG_OVERWRITE		1u
/**
* return, if the inhibit time is not ellapsed yet
*/
#define MSG_RET_INHIBIT		2u

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#endif /* CO_DATATYPE_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
