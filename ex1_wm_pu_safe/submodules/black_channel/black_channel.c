////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     black_channel.c
* @brief    Black channel communication.
* @author   Dusan Slavinec
* @date     29.12.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup BLACK_CHANNEL
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "project_config.h"
#include "test_switch.h"
#include "black_channel.h"
#include "crc_16_arc.h"
#include "apptime.h"

////////////////////////////////////////////////////////////////////////////////
// Preprocessor check
////////////////////////////////////////////////////////////////////////////////
#if ((defined(BLACKCH_DEVICE_MASTER) && defined(BLACKCH_DEVICE_SLAVE)) || \
     (!defined(BLACKCH_DEVICE_MASTER) && !defined(BLACKCH_DEVICE_SLAVE)))
    #error "Exactly one either Master or Slave must be defined."
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#ifdef BLACKCH_DEVICE_MASTER
    //@{
    /**
     * Master transmit IDs.
     */
    #define BLACKCH_TX_SENDER_ID        ( eBLACKCH_DEVICE_ID_MASTER )
    #define BLACKCH_TX_RECEIVER_ID      ( eBLACKCH_DEVICE_ID_SLAVE )
    //@}
    //@{
    /**
     * Master receiver expected IDs.
     */
    #define BLACKCH_RX_SENDER_ID        ( eBLACKCH_DEVICE_ID_SLAVE )
    #define BLACKCH_RX_RECEIVER_ID      ( eBLACKCH_DEVICE_ID_MASTER )
    //@}
#else
    //@{
    /**
     * Slave transmit IDs.
     */
    #define BLACKCH_TX_SENDER_ID        ( eBLACKCH_DEVICE_ID_SLAVE )
    #define BLACKCH_TX_RECEIVER_ID      ( eBLACKCH_DEVICE_ID_MASTER )
    //@}
    //@{
    /**
     * Slave receiver expected IDs.
     */
    #define BLACKCH_RX_SENDER_ID        ( eBLACKCH_DEVICE_ID_MASTER )
    #define BLACKCH_RX_RECEIVER_ID      ( eBLACKCH_DEVICE_ID_SLAVE )
    //@}
#endif

/**
 * Communication period - maximum allowed time [ms] between two consecutive sent/received frames.
 */
#define BLACKCH_COMM_PERIOD_MS          ( 1000 )


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
 * Black channel communication structure.
 */
typedef struct
{
    blackch_device_id_t const   sender;                 /**< This devices ID. */
    blackch_device_id_t const   receiver;               /**< Expected sender ID in receiving frame. */
    blackch_status_t            status;                 /**< Communication status. */
    uint8_t                     frame_cnt;              /**< Internal frame counter. */
    apptime_t                   comm_to_start_time;     /**< Communication timeout start time. */
    blackch_rcvd_payload_cbk_t  pf_rcvd_payload_cbk;    /**< Pointer to function to consume received payload. */
#ifdef BLACKCH_DEVICE_MASTER
    blackch_send_frame_cbk_t    pf_send_frame_cbk;      /**< Pointer to function to send frame. */
#endif // BLACKCH_DEVICE_MASTER
} blackch_t;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static inline uint16_t blackch_calc_frame_crc16(const blackch_frame_t frame);
//------------------------------------------------------------------------------
static inline blackch_frame_t blackch_cstruct_frame(const blackch_payload_t payload);
//------------------------------------------------------------------------------
static inline blackch_status_t blackch_chk_rcvd_frame(const blackch_frame_t rx_frame);
//------------------------------------------------------------------------------
#ifdef BLACKCH_DEVICE_SLAVE
    static inline blackch_status_t blackch_chk_rcvd_n_resp_frame(const blackch_frame_t rx_frame, blackch_frame_t* const p_tx_frame);
//------------------------------------------------------------------------------
    static inline bool blackch_can_resync(void);
#endif // BLACKCH_DEVICE_SLAVE
//------------------------------------------------------------------------------
static inline void blackch_reset_comm_to(void);
static inline void blackch_chk_comm_to(void);
//------------------------------------------------------------------------------
static blackch_status_t blackch_get_status(void);
static inline void blackch_status_clear_flag(const blackch_status_t flag);
static inline void blackch_status_set_flag(const blackch_status_t flag);
static inline bool blackch_is_status_flag_set(const blackch_status_t flag);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Black channel communication instance.
 */
static blackch_t g_blackch =
{
#ifdef BLACKCH_DEVICE_MASTER
    .sender   = eBLACKCH_DEVICE_ID_MASTER,
    .receiver = eBLACKCH_DEVICE_ID_SLAVE ,
    .pf_send_frame_cbk = NULL,
#else
    .sender   = eBLACKCH_DEVICE_ID_SLAVE ,
    .receiver = eBLACKCH_DEVICE_ID_MASTER,
#endif
    .status = eBLACKCH_OK,
    .frame_cnt          = 0,
    .comm_to_start_time = 0,
    .pf_rcvd_payload_cbk = NULL,
};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
* Calculate CRC-16 over the whole frame excluding only the CRC-16 field.
*
* @param[in]    frame   Frame.
* @return       Calculated frame CRC-16.
*/
////////////////////////////////////////////////////////////////////////////////
static inline uint16_t blackch_calc_frame_crc16(const blackch_frame_t frame)
{
    return crc_16_arc_calculate( (uint8_t*)(&frame), (uint16_t)(sizeof(blackch_frame_t) - sizeof(uint16_t)) );
}

////////////////////////////////////////////////////////////////////////////////
/**
* Construct frame from given payload by appending correct sender/receiver IDs and
* CRC-16.
*
* @param[in]    payload     Payload to include in frame.
* @return       Frame.
*/
////////////////////////////////////////////////////////////////////////////////
static inline blackch_frame_t blackch_cstruct_frame(const blackch_payload_t payload)
{
    blackch_frame_t tx_frame;

    tx_frame.sender_id   = BLACKCH_TX_SENDER_ID  ;
    tx_frame.receiver_id = BLACKCH_TX_RECEIVER_ID;
    tx_frame.counter     = g_blackch.frame_cnt;
    tx_frame.payload     = payload;
    tx_frame.crc16       = blackch_calc_frame_crc16( tx_frame );

    return tx_frame;
}

////////////////////////////////////////////////////////////////////////////////
/**
* Check received frame validity and correctness and set/clear appropriate status flags.
*
* @param[in]    rx_frame    Frame.
* @return       Status flags.
*/
////////////////////////////////////////////////////////////////////////////////
static inline blackch_status_t blackch_chk_rcvd_frame( const blackch_frame_t rx_frame )
{
    uint8_t counter = g_blackch.frame_cnt;

#ifdef BLACKCH_DEVICE_MASTER
    // Decrement frame counter because it was incremented after frame was sent
    counter -= 1;
#endif

    if( rx_frame.crc16 != blackch_calc_frame_crc16(rx_frame) )
    {
        blackch_status_set_flag(eBLACKCH_MISMATCH_CRC);
    }
    else
    {
        blackch_status_clear_flag(eBLACKCH_MISMATCH_CRC);
    }

    if( rx_frame.sender_id != BLACKCH_RX_SENDER_ID )
    {
        blackch_status_set_flag(eBLACKCH_INVALID_SENDER);
    }
    else
    {
        blackch_status_clear_flag(eBLACKCH_INVALID_SENDER);
    }

    if( rx_frame.receiver_id != BLACKCH_RX_RECEIVER_ID )
    {
        blackch_status_set_flag(eBLACKCH_INVALID_RECEIVER);
    }
    else
    {
        blackch_status_clear_flag(eBLACKCH_INVALID_RECEIVER);
    }

    if( counter != rx_frame.counter )
    {
        blackch_status_set_flag(eBLACKCH_INCORRECT_SEQ);
    }
    else
    {
        blackch_status_clear_flag(eBLACKCH_INCORRECT_SEQ);
    }

    return blackch_get_status();
}

#ifdef BLACKCH_DEVICE_SLAVE
////////////////////////////////////////////////////////////////////////////////
/**
* Check received frame validity and correctness and response frame destination
* and set/clear appropriate status flags.
*
* @param[in]    rx_frame    Received frame.
* @param[ou]    p_tx_frame  Pointer to response frame destination.
* @return       Status flags.
*/
////////////////////////////////////////////////////////////////////////////////
static inline blackch_status_t blackch_chk_rcvd_n_resp_frame(const blackch_frame_t rx_frame, blackch_frame_t* const p_tx_frame)
{
    (void)blackch_chk_rcvd_frame(rx_frame);

    if( p_tx_frame == NULL )
    {
        blackch_status_set_flag(eBLACKCH_RESPONSE_NULL);
    }
    else
    {
        blackch_status_clear_flag(eBLACKCH_RESPONSE_NULL);
    }

    return blackch_get_status();
}

////////////////////////////////////////////////////////////////////////////////
/**
* Check if slave to master frame sequence resynchronization is possible.
*
* @return   True if its possible.
*/
////////////////////////////////////////////////////////////////////////////////
static inline bool blackch_can_resync(void)
{
    return (bool)((blackch_is_status_flag_set(eBLACKCH_INCORRECT_SEQ) || blackch_is_status_flag_set(eBLACKCH_COMM_TIMEOUT)) &&
                 ((blackch_is_status_flag_set(eBLACKCH_INVALID_SENDER) != true) &&
                  (blackch_is_status_flag_set(eBLACKCH_INVALID_RECEIVER) != true) &&
                  (blackch_is_status_flag_set(eBLACKCH_MISMATCH_CRC) != true)));
}
#endif // BLACKCH_DEVICE_SLAVE

////////////////////////////////////////////////////////////////////////////////
/**
* Reset communication timeout.
*
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
static inline void blackch_reset_comm_to(void)
{
    g_blackch.comm_to_start_time = apptime_get_ms();
    blackch_status_clear_flag( eBLACKCH_COMM_TIMEOUT );
}

////////////////////////////////////////////////////////////////////////////////
/**
* Check if communication timeout has been reached and set the status flag.
*
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
static inline void blackch_chk_comm_to(void)
{
    if ( apptime_is_elapsed( g_blackch.comm_to_start_time, (uint32_t)BLACKCH_COMM_PERIOD_MS ) )
    {
        blackch_status_set_flag( eBLACKCH_COMM_TIMEOUT );
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
* Get communication status flags.
*
* @return       Status flags.
*/
////////////////////////////////////////////////////////////////////////////////
static inline blackch_status_t blackch_get_status(void)
{
    return g_blackch.status;
}

////////////////////////////////////////////////////////////////////////////////
/**
* Clear communication status flag.
*
* @param[in]    flag    Flag to clear.
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
static inline void blackch_status_clear_flag(const blackch_status_t flag)
{
    ASSERT((flag == eBLACKCH_INVALID_SENDER) ||
           (flag == eBLACKCH_INVALID_RECEIVER) ||
           (flag == eBLACKCH_MISMATCH_CRC) ||
           (flag == eBLACKCH_INCORRECT_SEQ) ||
           (flag == eBLACKCH_COMM_TIMEOUT) ||
           (flag == eBLACKCH_RESPONSE_NULL));

    g_blackch.status &= ~(flag);
}

////////////////////////////////////////////////////////////////////////////////
/**
* Set communication status flag.
*
* @param[in]    flag    Flag to set.
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
static inline void blackch_status_set_flag(const blackch_status_t flag)
{
    ASSERT((flag == eBLACKCH_INVALID_SENDER) ||
           (flag == eBLACKCH_INVALID_RECEIVER) ||
           (flag == eBLACKCH_MISMATCH_CRC) ||
           (flag == eBLACKCH_INCORRECT_SEQ) ||
           (flag == eBLACKCH_COMM_TIMEOUT) ||
           (flag == eBLACKCH_RESPONSE_NULL));

    g_blackch.status |= flag;

#ifdef QUALIFICATION_TESTS
    switch (flag)
    {
    case eBLACKCH_MISMATCH_CRC:
        g_test_qualif.blackch.mismatch_crc_cnt++;
        break;
    case eBLACKCH_INVALID_SENDER:
        g_test_qualif.blackch.invalid_sender_cnt++;
        break;
    case eBLACKCH_INVALID_RECEIVER:
        g_test_qualif.blackch.invalid_receiver_cnt++;
        break;
    case eBLACKCH_INCORRECT_SEQ:
        g_test_qualif.blackch.incorrect_seq_cnt++;
        break;
    case eBLACKCH_COMM_TIMEOUT:
        g_test_qualif.blackch.comm_timeout_cnt++;
        break;
    case eBLACKCH_RESPONSE_NULL:
        g_test_qualif.blackch.response_null_cnt++;
        break;
    default:
        break;
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
* Check if communication status flag is set.
*
* @param[in]    flag    Flag to check.
* @return       True if set.
*/
////////////////////////////////////////////////////////////////////////////////
static inline bool blackch_is_status_flag_set(const blackch_status_t flag)
{
    ASSERT((flag == eBLACKCH_INVALID_SENDER) ||
           (flag == eBLACKCH_INVALID_RECEIVER) ||
           (flag == eBLACKCH_MISMATCH_CRC) ||
           (flag == eBLACKCH_INCORRECT_SEQ) ||
           (flag == eBLACKCH_COMM_TIMEOUT) ||
           (flag == eBLACKCH_RESPONSE_NULL));

    return (bool)((g_blackch.status & flag) != 0);
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup BLACK_CHANNEL_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
* Initialize module used for black channel communication.
*
* @param[in]    pf_rcvd_payload_cbk     Pointer to function to consume received payload.
* @param[in]    pf_send_frame_cbk       Pointer to function to send frame.
* @return       Status.
*/
////////////////////////////////////////////////////////////////////////////////
#ifdef BLACKCH_DEVICE_MASTER
blackch_init_status_t blackch_init( blackch_rcvd_payload_cbk_t pf_rcvd_payload_cbk, blackch_send_frame_cbk_t pf_send_frame_cbk )
#else
////////////////////////////////////////////////////////////////////////////////
/**
* Initialize module used for black channel communication.
*
* @param[in]    pf_rcvd_payload_cbk     Pointer to function to consume received payload.
* @return       Status.
*/
////////////////////////////////////////////////////////////////////////////////
blackch_init_status_t blackch_init( blackch_rcvd_payload_cbk_t pf_rcvd_payload_cbk )
#endif
{
    STATIC_ASSERT(sizeof(blackch_device_id_t) == 1);
    STATIC_ASSERT(sizeof(blackch_payload_t  ) == 2);
    STATIC_ASSERT(sizeof(blackch_frame_t    ) == 7);

    blackch_init_status_t init_status;

    g_blackch.pf_rcvd_payload_cbk = pf_rcvd_payload_cbk;
    g_blackch.comm_to_start_time = apptime_get_ms();
    g_blackch.frame_cnt = 0;
    blackch_status_set_flag(eBLACKCH_COMM_TIMEOUT);
#ifdef BLACKCH_DEVICE_MASTER
    g_blackch.pf_send_frame_cbk = pf_send_frame_cbk;
#endif

#ifdef BLACKCH_DEVICE_MASTER
    if(    ( pf_rcvd_payload_cbk != NULL )
        && ( pf_send_frame_cbk   != NULL )
      )
#else
    if( pf_rcvd_payload_cbk != NULL )
#endif
    {
        init_status = eBLACKCH_INIT_OK;
    }
    else
    {
        init_status = eBLACKCH_INIT_CBK_FAIL;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/**
* Process black channel and check for communication timeout.
*
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
void blackch_proc(void)
{
    blackch_chk_comm_to();
}

////////////////////////////////////////////////////////////////////////////////
/**
* Check if communication timeout has been reached.
*
* @return       True if reached.
*/
////////////////////////////////////////////////////////////////////////////////
bool blackch_is_timeout( void )
{
    return blackch_is_status_flag_set( eBLACKCH_COMM_TIMEOUT );
}

#ifdef BLACKCH_DEVICE_MASTER
////////////////////////////////////////////////////////////////////////////////
/**
* Construct and send frame from given payload then increment internal frame counter.
*
* @param[in]    tx_payload  Payload.
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
void blackch_send_payload( const blackch_payload_t tx_payload )
{
    g_blackch.pf_send_frame_cbk( blackch_cstruct_frame( tx_payload ) );
    g_blackch.frame_cnt += 1;
}

////////////////////////////////////////////////////////////////////////////////
/**
* Consume received frame under condition that its valid and correct and reset
* communication timeout.
*
* @param[in]    rx_frame    Received frame.
* @return       None.
*/
////////////////////////////////////////////////////////////////////////////////
void blackch_rcv_frame( const blackch_frame_t rx_frame )
{
    if( blackch_chk_rcvd_frame( rx_frame ) == eBLACKCH_OK )
    {
        g_blackch.pf_rcvd_payload_cbk( rx_frame.payload );
    }

    blackch_reset_comm_to();
}
#else
////////////////////////////////////////////////////////////////////////////////
/**
* Consume received frame under condition that its valid and correct then try to
* construct and write a response frame to the requested destination or resynchronize
* and reset communication timeout.
*
* @param[in]    rx_frame    Received frame.
* @param[out]   p_tx_frame  Pointer to where response frame shall be written.
* @return       True if response frame was written.
*/
////////////////////////////////////////////////////////////////////////////////
bool blackch_rcv_n_resp_frame( const blackch_frame_t rx_frame, blackch_frame_t* p_tx_frame )
{
    bool do_respond = false;

#ifdef QUALIFICATION_TESTS
    if (g_test_qualif.blackch.dis_comm_timeout_reset)
    {
        // Skip all checks and force communication timeout
        return false;
    }

    if (g_test_qualif.blackch.set_response_null)
    {
        p_tx_frame = NULL;
    }
#endif

    if( blackch_chk_rcvd_n_resp_frame( rx_frame, p_tx_frame ) == eBLACKCH_OK)
    {
        *p_tx_frame = blackch_cstruct_frame( g_blackch.pf_rcvd_payload_cbk(rx_frame.payload) );
        do_respond = true;
        g_blackch.frame_cnt += 1;
#ifdef QUALIFICATION_TESTS
        g_test_qualif.blackch.ok_cnt++;
#endif
    }
    else if (blackch_can_resync())
    {
        g_blackch.frame_cnt = rx_frame.counter + 1;
    }
    else
    {
        // Invalid frame; do nothing
    }

    blackch_reset_comm_to();

    return do_respond;
}
#endif // BLACKCH_DEVICE_MASTER


// Placeholders for unit test function callbacks
#ifdef UNIT_TESTING
	#ifdef BLACKCH_DEVICE_MASTER
    	static void ut_rcvd_payload_placeholder(const blackch_payload_t rx_payload)
    	{
    		// Do nothing
    	}

    	static void ut_send_frame_placeholder(blackch_frame_t tx_frame)
    	{
    		// Do nothing
    	}
    #endif

    #ifdef BLACKCH_DEVICE_SLAVE
    	static blackch_payload_t ut_rcvd_payload_placeholder(const blackch_payload_t rx_payload)
    	{
    	    // Return don't care
    		blackch_payload_t tx_payload = { 0 };

    		return tx_payload;
    	}
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
