////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     can.h
 * @brief    Low level CAN driver for STM bxCAN.
 * @author   Tomaz Opresnik
 * @date     26.3.2021
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 *@addtogroup CAN_MODULE_API
 * @{ <!-- BEGIN GROUP -->
 *
 * Following module is part of API, which is available to the user of CAN Module
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __CAN_H_
#define __CAN_H_

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdint.h>

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
#include "stm32f7xx.h"
#else
#include "stm32g4xx.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Baud rate type.
 */
typedef enum
{
    eCAN_BAUDRATE_10k = 0, /**< CAN baud rate 10kbit/s */
    eCAN_BAUDRATE_20k,     /**< CAN baud rate 20kbit/s */
    eCAN_BAUDRATE_50k,     /**< CAN baud rate 50kbit/s */
    eCAN_BAUDRATE_100k,    /**< CAN baud rate 100kbit/s */
    eCAN_BAUDRATE_125k,    /**< CAN baud rate 125kbit/s */
    eCAN_BAUDRATE_250k,    /**< CAN baud rate 250kbit/s */
    eCAN_BAUDRATE_500k,    /**< CAN baud rate 500kbit/s */
    eCAN_BAUDRATE_1M,      /**< CAN baud rate 1Mbit/s */
} can_baudrate_t;

/**
 * The CAN type enumeration.
 */
typedef enum
{
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    eCAN_NORMAL_NODE = CAN_MODE_NORMAL, /**< CAN node in Normal mode */
#else
    eCAN_NORMAL_NODE = FDCAN_MODE_NORMAL, /**< CAN node in Normal mode */
#endif
} can_node_mode_t;

/**
 * Return type enumeration.
 */
typedef enum
{
    eCAN_OK = 0,                      /**< All OK */
    eCAN_ERROR,                       /**< General Error */
    eCAN_WRONG_BAUDRATE,              /**< Baud rate is not valid  */
    eCAN_NODE_NOT_EXISTING,           /**< Selected node is not existing */
    eCAN_NODE_MULTIPLE_CONFIGURATION, /**< Selected node is already configured with different
                                         settings */
    eCAN_CHANNEL_NO_MEMORY,           /**< No memory is available for channel FIFO buffer */
    eCAN_SEND_ERROR,                  /**< General error when trying to send CAN message */
    eCAN_NULL_PTR                     /**< Argumet to function null pointer */
} can_err_t;

/**
 * Structure for node definitions
 */
typedef enum
{
    eCAN_CAN1,         /**< CAN1 */
    eCAN_CAN2,         /**< CAN2 */
    eCAN_CAN3,         /**< CAN3 */
    eCAN_NUMBER_OF_CAN /**< Number of CAN channels*/
} can_node_index_t;

/**
 * Structure for CAN messages
 */
typedef struct
{
    uint32_t id;     /**< CAN ID of message */
    uint8_t  len;    /**< CAN message data length in bytes */
    bool     can_fd; /**< CAN FD flag, true if message is FD Frame */

    union
    {
        uint8_t byte[64]; /**< CAN message data with byte type */
        // dword not used by STM. Union structure must be left because of API compatibility with
        // Aurix
        // uint32_t    dword[8];   /**< CAN message data with double word type */ /*
        // parasoft-suppress MISRAC2012-RULE_3_1-c "ignore" */
    } data; /**< Data of message */
} can_message_t;

/**
 * The callback for message received.
 * This is a typedef for a function that gets called after receiving a message in interrupt. To the
 * receiving module it provides pointer to the received message structure
 */
typedef void (*rx_callback_t)(const can_message_t *const p_rx_message);

/**
 * The callback for message sent.
 * This is a typedef for a function that gets called after a message has been sent.
 */
typedef void (*tx_callback_t)(void);

/**
 * Logical channel type.
 * This type is used as a handle to the channel.
 */
typedef struct
{
    uint32_t tx_counter; /**< Number of transmitted CAN frames */
    uint32_t rx_counter; /**< Number of received CAN frames */
} can_channel_stats_t;

/**
 * Structure required for can initialisation
 */
typedef struct
{
    can_node_index_t node_id;        /**< Node ID which we want to use on this channel */
    can_baudrate_t   baudrate;       /**< Baud rate we want to use on this node */
    can_node_mode_t  mode;           /**< Config if we want to use CAN FD frames or Normal */
    uint32_t         can_id;         /**< CAN ID for RX filter */
    uint32_t         can_id_mask;    /**< CAN ID Mask for RX filter */
    bool             extended_frame; /**< True if we want to use 29-bit CAN ID frames */
    rx_callback_t
        p_rx_callback; /**< Function called when data has been received on CAN interface */
    tx_callback_t p_tx_callback; /**< Function called when transmit has been finished. */
} can_init_t;

/**
 * Abstract data type for channel instance
 */
typedef struct can_channel_s *p_can_channel_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

can_err_t can_init(const can_init_t *const p_can_init_struct, p_can_channel_t *const p_channel);
can_err_t can_send(const p_can_channel_t channel, can_message_t *const p_can_msg);

#endif // __CAN_H_
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
