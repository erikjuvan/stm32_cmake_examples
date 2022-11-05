////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     black_channel.h
* @brief    Black channel communication.
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

#ifndef __BLACK_CHANNEL_H
#define __BLACK_CHANNEL_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>

#include "black_channel_common.h"

#ifdef UNIT_TESTING
#include "project_config.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifdef BLACKCH_DEVICE_MASTER
    /**
     * Function type called when frame shall be sent by master.
     */
    typedef void (* blackch_send_frame_cbk_t)( const blackch_frame_t tx_frame );

    /**
     * Function type called when received payload is ready to be consumed by master.
     */
    typedef void (* blackch_rcvd_payload_cbk_t)(const blackch_payload_t rx_payload);
#else
    /**
     * Function type called when received payload is ready to be consumed and response sent by slave.
     */
    typedef blackch_payload_t (* blackch_rcvd_payload_cbk_t)(const blackch_payload_t rx_payload);
#endif


/**
 * Module initialization status.
 */
typedef enum
{
    eBLACKCH_INIT_OK       = 0x00,  /**< Initialization successful. */
    eBLACKCH_INIT_CBK_FAIL = 0x01,  /**< Callback function is NULL pointer. */
} blackch_init_status_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
#ifdef BLACKCH_DEVICE_MASTER
    blackch_init_status_t blackch_init(blackch_rcvd_payload_cbk_t pf_rcvd_payload_cbk ,blackch_send_frame_cbk_t pf_send_frame_cbk);
    void blackch_send_payload(const blackch_payload_t tx_payload);
    void blackch_rcv_frame(const blackch_frame_t rx_frame);
#else
    blackch_init_status_t blackch_init(blackch_rcvd_payload_cbk_t pf_rcvd_payload_cbk);
    bool blackch_rcv_n_resp_frame(const blackch_frame_t rx_frame, blackch_frame_t* p_tx_frame);
#endif

void blackch_proc(void);
bool blackch_is_timeout(void);

#endif // __BLACK_CHANNEL_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
