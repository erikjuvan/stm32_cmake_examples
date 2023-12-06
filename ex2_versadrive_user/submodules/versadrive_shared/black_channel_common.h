////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     black_channel_common.h
* @brief    Black channel frame and payload definitions for MU and PU devices.
* @author   Dusan Slavinec
* @date     29.12.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup BLACK_CHANNEL_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __BLACK_CHANNEL_COMMON_H
#define __BLACK_CHANNEL_COMMON_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

#include "drumspin_common.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Slave and Master device IDs.
 */
typedef enum __attribute__ ((packed))
{
    eBLACKCH_DEVICE_ID_SLAVE  = 0xAA,   /**< MU device ID. */
    eBLACKCH_DEVICE_ID_MASTER = 0x55    /**< PU device ID. */
} blackch_device_id_t;


/**
 * Communication status.
 */
typedef enum
{
    eBLACKCH_OK               = 0x00, /**< Received valid frame. */
    eBLACKCH_INVALID_SENDER   = 0x01, /**< Received frame has invalid sender ID. */
    eBLACKCH_INVALID_RECEIVER = 0x02, /**< Received frame has invalid receiver ID. */
    eBLACKCH_MISMATCH_CRC     = 0x04, /**< Received frame CRC mismatches with expected. */
    eBLACKCH_INCORRECT_SEQ    = 0x08, /**< Incorrect frame sequence detected. */
    eBLACKCH_COMM_TIMEOUT     = 0x10, /**< Timeout reached while waiting to receive a frame. */
    eBLACKCH_RESPONSE_NULL    = 0x20, /**< Response destination is NULL (applies only to slave). */
} blackch_status_t;


/**
 * Generic payload structure common to both master and slave.
 */
typedef struct __attribute__ ((packed))
{
    uint8_t status;     /**< Status. */
    uint8_t req_resp;   /**< Request/response. */
} blackch_payload_t;

/**
 * Frame structure.
 */
typedef struct __attribute__ ((packed))
{
    blackch_device_id_t sender_id   ;   /**< Sender device ID. */
    blackch_device_id_t receiver_id ;   /**< Receiver device ID. */
    uint8_t             counter     ;   /**< Frame sequence number. */
    blackch_payload_t   payload     ;   /**< Payload. */
    uint16_t            crc16       ;   /**< CRC-16. */
} blackch_frame_t;

/* parasoft-begin-suppress MISRAC2012-RULE_6_1-a "Allow for this project as C90 is not used." */
/**
 * MU device status union.
 */
typedef union
{
    struct
    {
        drumspin_state_t drumspin_state : 2;    /**< Drum spinning state. */
        uint8_t reserved : 6;
    };
    uint8_t byte;                               /**< Status byte. */
} blackch_pyld_mu_status_t;

/**
 * PU device status union.
 */
typedef union
{
    struct
    {
        uint8_t reserved : 8; /**< Reserved (don't care). */
    };
    uint8_t byte;             /**< Status byte. */
} blackch_pyld_pu_status_t;
/* parasoft-end-suppress MISRAC2012-RULE_6_1-a "Allow for this project as C90 is not used." */

/**
 * MU device response byte.
 */
typedef enum
{
        eBLCH_REQ_NONE_RESP_OK_NONE = 0x00,  /**< Nothing. */
} blackch_pyld_response_t;

/**
 * PU device request byte.
 */
typedef enum
{
    eBLACKCH_REQUEST_NONE = 0,    /**< Nothing. */
} blackch_pyld_request_t;

/**
 * MU device payload structure.
 */
typedef struct __attribute__ ((packed))
{
    blackch_pyld_mu_status_t status;    /**< Status. */
    blackch_pyld_response_t  response;  /**< Response. */
} blackch_pyld_mu_t;

/**
 * PU device payload structure.
 */
typedef struct __attribute__ ((packed))
{
    blackch_pyld_pu_status_t status;    /**< Status. */
    blackch_pyld_request_t   request;   /**< Request. */
} blackch_pyld_pu_t;

#endif // __BLACK_CHANNEL_COMMON_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
