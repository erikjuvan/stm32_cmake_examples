//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        wifi.h
// CREATION DATE:      2019/12/04
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/02/16
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Interface to WiFi communication port.
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.


#ifndef WIFI_H
#define WIFI_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "hisense/hisense_NetworkL.h"
#include "hisense/hisense_LinkL.h"
#include "Platform/timer.h"
#include "Platform/serialport.h"
#include "Netbuf/netbuf.h"

#ifndef DOUBLE_BUFFERING
#define DOUBLE_BUFFERING                                    0
#endif

#if (DOUBLE_BUFFERING == 1)
#define WIFI_NUMBER_OF_BUFFERS                              2
#else
#define WIFI_NUMBER_OF_BUFFERS                              1
#endif

/**
* @brief Direct Fast Flashing API commands
*/
typedef enum wifi_dff_mode_e
{
    WIFI_DFF_MODE_DISABLE                       = 0x00, /**< Go out of Direct Fast Flashing mode                                                            */
    WIFI_DFF_MODE_ENABLE                        = 0x01, /**< Go to Direct Fast Flashing mode for this device
                                                             (Rx data is passed directly to bootloader_bl).                                                 */
    WIFI_DFF_MODE_FORWARDING_ONLY               = 0x02, /**< Direct Fast Flashing is not active for this device,
                                                             but for devices connected to forwarding ports of this device.
                                                             Rx data is fordarded on byte-level to FORWARD layer.                                           */
} wifi_dff_mode_t;

#if (MASTER_FUNCTIONALITY_USE == 1)
/**
* @brief Results of Master request API function (wifi_master_request and wifi_master_request_cancel functions)
*/
typedef enum wifi_master_request_tesult_e
{
    WIFI_MASTER_REQUEST_RESULT_OK               = 0x00, /**< Master request accepted. Reply from slave has to be waited, callback will be called.           
                                                             or Master Request Cancel successfully ended.                                                   */
    WIFI_MASTER_REQUEST_RESULT_INVALID_PORT     = 0x01, /**< Invalid port number specified in paramater                                                     */
    WIFI_MASTER_REQUEST_RESULT_NOT_READY        = 0x02, /**< Serial port is not ready. Try again next time.                                                 */
    WIFI_MASTER_REQUEST_RESULT_INVALID_DATA     = 0x03, /**< Invalid data in Master request API call.                                                       */

    WIFI_MASTER_REQUEST_RESULT_FAIL             = 0xFF  /**< Unspecified end of function call.                                                              */
} WIFI_MASTER_REQUEST_RESULT;
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */


/**
* @brief Modes and states for the non-DFF (slave, master) and DFF operation mode
*/
typedef enum wifi_mode_state_t
{
    WIFI_MODE_STATE_NORMAL_DATARX,                      /**< Normal mode (Hisense protocol frame parse), frame is receiving.                                */
    WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME,         /**< Direct Fast Flashing is requested for this device. 
                                                             Wait for guard time before entering ACTIVE state.                                              */
    WIFI_MODE_STATE_DFF_ACTIVE,                         /**< Direct Fast Flashing is active for this device.
                                                             Rx data is passed directly to bootloader_bl                                                    */
    WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST,        /**< Direct Fast Flashing forwarding is requested, but answer to the Request has to be sent first 
                                                             before entering the WIFI_MODE_STATE_DFF_FORWARDING_ONLY state .                                  */
    WIFI_MODE_STATE_DFF_FORWARDING_ONLY,                /**< Direct Fast Flashing is not active for this device, but for devices connected to forwarding 
                                                             ports of this device. Rx data is fordarded on byte-level to forward ports, 
                                                             on wifi layers defined in wifilayer_reference.                                                   */
} WIFI_MODE_STATE;

/**
* @brief WiFi (UART) counter enumeration
*/
typedef enum wifi_layer_counter_t
{
    WIFI_LAYER_COUNTER_RX,
    WIFI_LAYER_COUNTER_RX_QUEUEFULL,
    WIFI_LAYER_COUNTER_TX,
    WIFI_LAYER_COUNTER_TX_QUEUEFULL,
    WIFI_LAYER_COUNTER_TX_TIMEOUT,
    /*
    Add counters here, if needed.
    */
    WIFI_LAYER_COUNTER_MAX
} WIFI_LAYER_COUNTER;

/**
* @brief Forward ports data and states derived from this wifi instance
*/
#if (FORWARDING_USE == 1)
/**
* @brief Forward functionality control structure
*/
typedef struct wifi_forward_s
{
    uint8_t number_of_forwarding_ports;                         /**< Number of forwarding ports used for this instance of layer */
    void*   wifilayer_reference[MAX_NUMBER_OF_FORWARD_PORTS];   /**< Destination wifi layer reference                           */
} WIFI_FORWARD;
#endif /* (FORWARDING_USE == 1) */

typedef struct wifi_counters_s
{
    uint32_t rx;                    /**< Whole LL packet came on RX                                                             */
    uint32_t rx_queuefull;          /**< Whole LL packet came on RX, but RX queue was full to process the packet                */
    uint32_t tx;                    /**< Whole LL packet sent out successfully within time anticipated by current baudrate      */
    uint32_t tx_queuefull;          /**< Reply was prepared to sent out, but TX queue was full                                  */
    uint32_t tx_timeout;            /**< Whole LL packet NOT sent out successfully within time anticipated by current baudrate  */
} WIFI_COUNTERS;

#if (MASTER_FUNCTIONALITY_USE == 1)
/**
* @brief Master request control structure
*/
typedef struct wifi_master_ctrl_s
{
    callbackFunc_3args          callback_func;          /**< Callback func. that is called when reply from slave is received  */
    callbackFunc                calling_func_addr;      /**< Address of calling function that called the Master Request       */
} WIFI_MASTER_CTRL;

/**
* @brief Data for Master request
*/
typedef struct wifi_master_req_dst_data_s
{
    uint8_t     nl_AddressCode1[ADDRESS_CODE_LENGTH];       /**< Network layer Address Code 1 (destination address)   */
    uint8_t     nl_AddressCode1_length;                     /**< Network layer Address Code 1 length                  */
    uint8_t     al_message_type;                            /**< Application layer Message type                       */
    uint8_t     al_message_subtype;                         /**< Application layer Message subtype                    */
    uint8_t*    al_data;                                    /**< Application layer data                               */
    uint16_t    al_data_length;                             /**< Application layer data length                        */
} WIFI_MASTER_REQUEST_DST_DATA;
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

#if (SYNCHRONIZED_TX_USE == 1)
typedef struct wifi_synchronized_tx_ctrl_s
{
    uint8_t synchronized_tx_enabled;
    uint8_t synchronized_tx_requested;
    TIMER last_synchronization_time;
} WIFI_SYNCHRONOZED_TX_CTRL;
#endif /* (SYNCHRONIZED_TX_USE == 1) */

#if (RX_SYNCHRONIZATION_USE == 1)
typedef struct wifi_rx_synchronization_ctrl_s
{
    uint8_t rx_synchronization_enabled;
    callbackFunc rx_synchronization_callback;
} WIFI_RX_SYNCHRONIZED_CTRL;
#endif /* (RX_SYNCHRONIZATION_USE == 1) */

/**
* @brief WiFi Layer data structure
*/
typedef struct wifi_layer_s
{
    SERIALPORT_NUMBER serialPort;               /**< Serial port number                                               */
    SERIALPORT_ALTERNATE_RX_PIN rx_alt_pin;     /**< Serial port alternate pin used for RX                            */
    SERIALPORT_ALTERNATE_TX_PIN tx_alt_pin;     /**< Serial port alternate pin used for TX                            */
    SERIALPORT_BAUDRATE baudrate;               /**< Baudrate that is to be set (required baudrate)                   */
    SERIALPORT_BAUDRATE actualBaudrate;         /**< Baudrate that is actually set due to integer dividers            */

    NETBUF* rx_frame;                           /**< Frame (not complete yet) that is currently receiving             */
    NETBUF* tx_frame;                           /**< Frame that is currently transmitting (transmit not complete yet) */
    NETBUF* tx_frame_waiting;                   /**< Frame that is to be sent out but not confirmed on app. layer     */
    NETBUF_QUEUE rx_queue;                      /**< Received frames queue                                            */
    NETBUF_QUEUE tx_queue;                      /**< Queue of frames that has to be transmitted                       */
    NETBUF_LEN tx_frame_len;                    /**< Length of the frame (tx_frame) that is currently transmitting    */

    WIFI_MODE_STATE mode_state;                 /**< Mode of operation and state of the state-machine                 */
    TIMER mode_state_timer;                     /**< Timer to handle mode_state transitions                           */
    TIMER packet_rx_timer;                      /**< Timer when last frame with correct CRC was received(also for FWD)*/
    TIMER packet_tx_timer;                      /**< Timer to control TX time of response packet                      */
    WIFI_COUNTERS counters;                     /**< Counters for statistics...                                       */
    #if (FORWARDING_USE == 1)
    WIFI_FORWARD forward;                       /**< Forward ports data and states derived from this wifi instance    */
    #endif /* (FORWARDING_USE == 1) */
    #if (MASTER_FUNCTIONALITY_USE == 1)
    WIFI_MASTER_CTRL master_ctrl;               /**< Master functionality control structure                           */
    #endif /* (MASTER_FUNCTIONALITY_USE == 1) */
    #if (SYNCHRONIZED_TX_USE == 1)
    WIFI_SYNCHRONOZED_TX_CTRL synchronized_tx_ctrl;
    #endif /* (SYNCHRONIZED_TX_USE == 1) */
    #if (RX_SYNCHRONIZATION_USE == 1)
    WIFI_RX_SYNCHRONIZED_CTRL rx_synchronization_ctrl;
    #endif /* (RX_SYNCHRONIZATION_USE == 1) */


    uint8_t dff_rx_enabled;                     /**< Receiving in Direct Fast FW update mode is enabled               */
    uint32_t dff_initial_timeout;               /**< Timeout value to handle DFF state timeouts (initial_timeout)     */
    uint32_t dff_data_timeout;                  /**< Timeout value to handle DFF state timeouts (data_timeout)        */
    NETBUF* dff_buffer[WIFI_NUMBER_OF_BUFFERS]; /**< Net buffer for Direct Fact Flashing mode receiving               */
    uint32_t dff_bytesToReceive;                /**< How many data is expected to receive in DFF mode                 */
    uint8_t currentRxBuffer;                    /**< Which "dff_buffer" is currently receiving (DOUBLE_BUFFERING mode)*/
    uint8_t currentWriteBuffer;                 /**< Which "dff_buffer" is currently writing transferring to BL       */
    uint8_t packet_sending;                     /**< If packet is currently sending out.                              */

    /* Upper layer: */
    HISENSE_LINKL_LAYER* linkLayer;

    /* Lower layer: */
    /*    ----      */
} WIFI_LAYER;


// DESCRIPTION:
//   Initializes WiFi communication port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void wifi_init(WIFI_LAYER *layer);


/*******************************************************************************************************************//**
* @brief    Reinitialize baudrate (because UART clock source change).
* 
* @param    layer           WiFi layer.
* 
* @details  Example:
* @code
*    WIFI_LAYER *layer;
*    layer = ...
*
*    wifi_reinit_baudrate(layer);
* @endcode
*
***********************************************************************************************************************/
void wifi_reinit_baudrate(WIFI_LAYER *layer);


// DESCRIPTION:
//   Sets internal variable to mark the initialization of other HW is complete.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_initDone(WIFI_LAYER *layer);


// DESCRIPTION:
//   Returns WiFi (UART) counters.
// INPUTS:
//   *layer  - WiFi Layer structure
//   counter - Which counter to get
// OUTPUT: 
//   --
// RETURNS:
//   Counter value passed in parameter.
// ---------------------------------------------------------------------------- 
uint32_t wifi_counter_get(WIFI_LAYER *layer, WIFI_LAYER_COUNTER counter);


// DESCRIPTION:
//   Enables interrupts for WiFi communication port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_interrupts_enable(WIFI_LAYER *layer);


// DESCRIPTION:
//   Disables interrupts for WiFi communication port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_interrupts_disable(WIFI_LAYER *layer);


// DESCRIPTION:
//   Enables RX interrupt for WiFi communication port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_enableReceiver(WIFI_LAYER *layer);


// DESCRIPTION:
//   Disables RX interrupt for WiFi communication port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_disableReceiver(WIFI_LAYER *layer);


// DESCRIPTION:
//   If data is being sent to WiFi port.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   0 - WiFi Tx is Idle
//   1 - Data sending to WiFi port is in progress
// ---------------------------------------------------------------------------- 
uint8_t wifi_isSending(WIFI_LAYER *layer);


/*******************************************************************************************************************//**
* @brief    Set WiFi layer to Direct FastFlashing passed in parameter.
*           Regarding mode, also inform Forward layer about DFF mode.
* 
* @param    layer           WiFi layer.
* @param    dff_mode        Mode to set the WiFi layer to:
*                           WIFI_DFF_MODE_DISABLE, WIFI_DFF_MODE_ENABLE or WIFI_DFF_MODE_FORWARDING_ONLY.
* @param    initial_timeout Timeout to stay in requested state before returning to normal mode, before first data come.
* @param    data_timeout    Timeout to stay in requested state before returning to normal mode, after first data came.
* 
* @details  Example:
* @code
*    WIFI_LAYER *layer;
*    layer = ...
*    wifi_dff_mode_t wifi_dff_mode;
*    wifi_dff_mode = WIFI_DFF_MODE_ENABLE;
*
*    wifi_dff_mode_set(layer, wifi_dff_mode, 30000, 200);
* @endcode
*
***********************************************************************************************************************/
void wifi_dff_mode_set(WIFI_LAYER *layer, wifi_dff_mode_t dff_mode, uint32_t initial_timeout, uint32_t data_timeout);


#if (MASTER_FUNCTIONALITY_USE == 1)
/*******************************************************************************************************************//**
* @brief    Trigger the Master request on the specified UART interface.
* 
* @param    serialport          Serial port number to where the Master request is to be sent.
* @param    destination_data    Destination data (network and application data) for the Master request that is to be sent.
* @param    callback_address    Address for callback function that will be called when reply from slave is received.
* @param    caller_address      Address of the function from where the wifi_master_request() was called.
* 
* @details  Example:
* @code
* void master_request_callback(void* application_response_id, void* application_data, void* application_data_length)
* {
*    AL_RESPONSE_ID al_response_id;
*    uint8_t* al_data;
*    uint16_t al_data_length;
*    uint16_t i;
*
*    if ((application_response_id == NULL ) ||
*        (application_data == NULL) ||
*        (application_data_length == NULL))
*    {
*        return;
*    }
*
*    al_response_id = *(AL_RESPONSE_ID*) application_response_id;
*    if (al_response_id != AL_RESPONSE_ID_SUCCESSFUL)
*    {
*        // Failed, not supported, remote control off ... 
*        return;
*    }
*
*    al_data = (uint8_t*) application_data;
*    al_data_length = *(uint16_t*) (application_data_length);
*
*    for (i = 0; i < al_data_length; i++)
*    {
*        // Do something with data in "al_data[i]" ... 
*        al_data[i] = al_data[i];    // Warning suppression 
*    }
* }
*
* void calling_function_address(void)
* {
*     WIFI_MASTER_REQUEST_DST_DATA destination_data;
*     // Fill destination_data
*     result = wifi_master_request(SERIALPORT_NUMBER_2, destination_data , master_request_callback, (void*) calling_function_address);
*     if (result != WIFI_MASTER_REQUEST_RESULT_OK)
*     {
*         // NOT OK, handle error ...
*     }
* @endcode
*
***********************************************************************************************************************/
WIFI_MASTER_REQUEST_RESULT wifi_master_request(SERIALPORT_NUMBER serialport,
                                               WIFI_MASTER_REQUEST_DST_DATA destination_data,
                                               callbackFunc_3args callback_address,
                                               void* caller_address);


/*******************************************************************************************************************//**
* @brief    Cancel master request pending on UART interface specified in parameter.
* 
* @param    serialport          Serial port number on which Master request is pending and has to be canceled.
* @param    caller_address      Address of the function from where the wifi_master_request() was called.
* 
* @details  Example:
* @code
* WIFI_MASTER_REQUEST_RESULT result;
* result = wifi_master_request_cancel(SERIALPORT_NUMBER_2, &calling_function_address);
* if (result != WIFI_MASTER_REQUEST_RESULT_OK)
* {
*    // NOT OK, handle error ...
* }
* @endcode
*
***********************************************************************************************************************/
WIFI_MASTER_REQUEST_RESULT wifi_master_request_cancel(SERIALPORT_NUMBER serialport, void* caller_address);
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */


#if (SYNCHRONIZED_TX_USE == 1)
/*******************************************************************************************************************//**
* @brief    Trigger sending of reply which is waiting in TX queue at actual time (time of calling this function). 
*           If the TX queue is empty, synchronization characters 0xF4 and 0xF5 are sent at the time of calling this function.
* 
* @param    serialport          Serial port number on which to send the reply or synchronization characters, if the queue is empty. 
* 
* @details  Example:
* @code
*    // In initialization phase:
*    iocontrol_register_ISR(IOCONTROL_PIN_KEY1, zero_cross_detection_ISR);
*    // ...
*    void zero_cross_detection_ISR(void)
*    {
*        iocontrol_clearInterrupt(IOCONTROL_PIN_KEY1);
*        // Because of asymmetry: only transitions from 1 to 0:
*        if (iocontrol_read(IOCONTROL_PIN_KEY1) == 0)
*        {
*            wifi_synchronized_tx_request(SERIALPORT_NUMBER_0);
*        }
*    }
* @endcode
*
***********************************************************************************************************************/
void wifi_synchronized_tx_request(SERIALPORT_NUMBER serialport);
#endif /* (SYNCHRONIZED_TX_USE == 1) */


#if (RX_SYNCHRONIZATION_USE == 1)
/*******************************************************************************************************************//**
* @brief    Register the callback function which will be called when synchronization characters come on specified by the "serialport".
* 
* @param    serialport            Serial port number for which to register callback. 
* @param    callback_func_address Callback function which is called when RX synchronization is received.
* 
* @details  Example:
* @code
*    // In initialization phase:
*    wifi_rx_synchronization_callback_register(SERIALPORT_NUMBER_0, zero_cross_callback);
*    // ...
*    void zero_cross_callback(void* serialport)
*    if (serialport == NULL)
*    {
*        return;
*    }
*    if (*(SERIALPORT_NUMBER*)serialport != SERIALPORT_NUMBER_0)
*    {
*        return;
*    }
*
*    iocontrol_write(IOCONTROL_PIN_LED1, 1);
*    proc_delayus(10);
*    iocontrol_write(IOCONTROL_PIN_LED1, 0);
* @endcode
*
***********************************************************************************************************************/
void wifi_rx_synchronization_callback_register(SERIALPORT_NUMBER serialport, void* callback_func_address);

/*******************************************************************************************************************//**
* @brief    Function that has to be called when RX synchronization occured. 
*           The function is called from Hisense Link layer.
*           This is just internal function to propagate the informnation towards WiFi module.
*           Actual RX synchronization callback function is called from this function.
* 
* @param    layer             WiFi layer reference on which the RX synchronozation occured.
*
***********************************************************************************************************************/
void wifi_rx_synchronization_callback(WIFI_LAYER *layer);

#endif /* (RX_SYNCHRONIZATION_USE == 1) */


/*******************************************************************************************************************//**
* @brief    Function returns time from now in milliseconds when the last valid (CRC OK) frame was received,
*           for this MCU or for MCU(s) on forwarding port(s).
*           If there was no packet received yet, the function returns 0xFFFFFFFF.
* 
* @param    layer             WiFi layer reference for which to get the time of last received frame.
* @return   Time in milliseconds elapsed from last correct frame received.
*
***********************************************************************************************************************/
uint32_t wifi_rx_packet_receive_time(WIFI_LAYER *layer);


// DESCRIPTION:
//   WiFi process function.
// INPUTS:
//   *layer - WiFi Layer structure
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void wifi_run(WIFI_LAYER *layer);


#endif /* WIFI_H */

/* [] END OF FILE */
