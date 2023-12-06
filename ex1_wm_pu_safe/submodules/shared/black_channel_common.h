////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     blackch_common.h
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

#include "mu_safe_types.h"
#include "eps_common.h"
#include "drain_common.h"
#include "lock_common.h"

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
        door_state_t  door_state        : 1;    /**< Door state. */
        relay_state_t relay_door_state  : 1;    /**< Door relay state. */
        relay_state_t relay_power_state : 1;    /**< Power supply relay state. */
        door_lock_state_t door_lock_state : 3;  /**< Door lock state. */
        drumspin_state_t drumspin_state : 2;    /**< Drum spinning state. */
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
        drain_state_t drain_state  : 1; /**< Drain pump state. */
        eps_level_t   eps_level    : 1; /**< EPS level. */
        lock_state_t  lock_state   : 2; /**< Lock state. */
        uint8_t       reserved     : 4; /**< Reserved (don't care). */
    };
    uint8_t byte;                       /**< Status byte. */
} blackch_pyld_pu_status_t;
/* parasoft-end-suppress MISRAC2012-RULE_6_1-a "Allow for this project as C90 is not used." */

/**
 * MU device response byte.
 */
typedef enum
{
    eBLCH_REQ_NONE_RESP_NONE                            = 0x00,  /**< Nothing. */
    eBLCH_REQ_LOCK_RESP_LOCK                            = 0x01,  /**< Lock request approved, PU can lock door. */
    eBLCH_REQ_ULCK_RESP_ULCK                            = 0x02,  /**< Unlock request approved, PU can unlock door. */
    eBLCH_REQ_LOCK_RESP_IN_PROGRESS                     = 0x03,  /**< Lock request in progress (MU checking if request can be approved). */
    eBLCH_REQ_ULCK_RESP_IN_PROGRESS                     = 0x04,  /**< Unlock request in progress (MU checking if request can be approved). */
    eBLCH_REQ_LOCK_RESP_LOCKED                          = 0x05,  /**< Device already in state where door is expected to be locked (request has been ignored). */
    eBLCH_REQ_ULCK_RESP_UNLOCKED                        = 0x06,  /**< Device already in state where door is expected to be unlocked (request has been ignored). */

    eBLCH_REQ_LOCK_RESP_DENIED_NOT_UNLOCKED             = 0x80,  /**< Denied lock request, door is locked. */
    eBLCH_REQ_LOCK_RESP_DENIED_IMPOSSIBLE               = 0x81,  /**< Denied lock request, not possible in current SAFE FSM state. */
    eBLCH_REQ_LOCK_RESP_DENIED_NO_TRIAC_TRIG            = 0x82,  /**< Denied lock request, triac not triggered. */
    eBLCH_REQ_ULCK_RESP_DENIED_DOOR_NOT_LOCKED          = 0x83,  /**< Denied unlock request, door not locked. */
    eBLCH_REQ_ULCK_RESP_DENIED_SPINNING                 = 0x84,  /**< Denied unlock request, drum not stopped. */
    eBLCH_REQ_ULCK_RESP_DENIED_UNLOCK_IMPOSSIBLE        = 0x85,  /**< Denied unlock request, not possible in current SAFE FSM state. */
    eBLCH_REQ_ULCK_RESP_DENIED_EPS_HIGH                 = 0x86,  /**< Denied unlock request, water level high. */
} blackch_pyld_response_t;

/**
 * PU device request byte.
 */
typedef enum
{
    eBLCH_REQ_NONE   = 0,    /**< Nothing. */
    eBLCH_REQ_LOCK   = 1,    /**< Request MU to start locking procedure. */
    eBLCH_REQ_ULCK   = 2,    /**< Request MU to start unlocking procedure. */
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
