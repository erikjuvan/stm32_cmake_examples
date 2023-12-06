////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      canopen_drv.c
*@brief     canopen driver fnc
*@author    Zdenko Mezgec
*@date      03.12.2020
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
#include "../canopen_cfg.h"
#include "co_canopen.h"
#include "canopen.h"
#include "peripheral/can/can/can.h"

#ifdef __CTC__
    #include "Cpu\Std\IfxCpu.h"
    #include "Scu\Std\IfxScuWdt.h"
#elif defined(__arm__)
		
#endif

#ifdef CO_FREERTOS
    #include "FreeRTOS.h"
    #include "semphr.h"
    #include "task.h"
    #include "timers.h"
    #include "stdbool.h"
#elif defined(CO_KEIL_RTX)
    #include "cmsis_os.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
void codrvCanReceiveInterrupt(const can_message_t *const rx_message);
void codrvCanTransmitInterrupt(void);

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
can_init_t g_can_init_canopen =
{
        .node_id = CANOPEN_CAN_NODE_INDEX,
        .extended_frame = false,
        .mode = eCAN_NORMAL_NODE,
        .baudrate = eCAN_BAUDRATE_500k,
        .can_id = CANOPEN_CAN_ID_MASTER,
        .can_id_mask = CANOPEN_CAN_ID_MASK,
        .p_tx_callback = &codrvCanTransmitInterrupt,
        .p_rx_callback = &codrvCanReceiveInterrupt,
};

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
p_can_channel_t can_channel = NULL;
can_init_t      *gp_can_init_canopen = &g_can_init_canopen;

BOOL_T                  can_enabled = CO_FALSE;
static BOOL_T           transmission_is_active = CO_FALSE;
CO_CONST CO_CAN_MSG_T   *pTxBuf = NULL;

////////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        init CAN controller
*
* This function initializes the CAN controller and configures the bitrate.
* At the end of the function, the CAN controller should be in state disabled.
*
* @param[in]    bitRate CAN bitrate
* @return       RET_T   Returns RET_OK if the CAN controller was initialised.
*/
////////////////////////////////////////////////////////////////////////////////
RET_T codrvCanInit(UNSIGNED16 bitRate)
{
    // First lets check if the can channel is already initialised.
    if( can_channel == NULL )
    {
        switch (bitRate)
        {
            case 250:
            {
                g_can_init_canopen.baudrate = eCAN_BAUDRATE_250k;
                break;
            }

            case 500:
            {
                g_can_init_canopen.baudrate = eCAN_BAUDRATE_500k;
                break;
            }

            case 1000:
            {
                g_can_init_canopen.baudrate = eCAN_BAUDRATE_1M;
                break;
            }

            default:
            {
                return RET_DRV_WRONG_BITRATE;
            }
        }
        if( eCAN_OK != can_init(gp_can_init_canopen, &can_channel) )
        {
            CO_ASSERT(0);
            return RET_DRV_ERROR;
        }
        else
        {
            can_enabled = CO_TRUE;
            return RET_OK;
        }
    }
    else
    {
        can_enabled = CO_TRUE;
        return RET_OK;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        Sends a message on canopen CAN channel.
*
*
* @param[in]    pBuf    pointer to a message buffer.
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T codrvCanTransmit( CO_CONST CO_CAN_MSG_T * pBuf)
{
    can_message_t can_msg;

    CO_ASSERT( g_can_init_canopen.extended_frame == (pBuf->canCob.extended ? 1:0));

    can_msg.id = (UNSIGNED32)pBuf->canCob.canId;
    can_msg.can_fd = false;
    can_msg.len = pBuf->len;

    memcpy(&can_msg.data.byte, &pBuf->data, pBuf->len);

    can_send(can_channel, &can_msg);
    return RET_OK;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        start can transmission if not active
*
* Transmission of CAN messages should be interrupt driven. If a message was sent, the
* Transmit Interrupt is called and the next message can be transmitted. To start the transmission
* of the first message, this function is called from the CANopen stack. The easiest way to implement
* this function is to trigger the transmit interrupt, but only of the transmission is not already active.
*
* @return       RET_T   Returns RET_OK if start of transmission was successful.
*/
////////////////////////////////////////////////////////////////////////////////
RET_T codrvCanStartTransmission(void)
{
    if ( can_enabled != CO_TRUE )
    {
        return RET_DRV_ERROR;
    }

    transmission_is_active = CO_TRUE;

    // get next message from transmit queue
    while ((pTxBuf = coQueueGetNextTransmitMessage()) != NULL)
    {
        // and transmit it - CAN message buffer is free
        codrvCanTransmit(pTxBuf);

        // if message is successfully sent inform CAN stack about it
        coQueueMsgTransmitted(pTxBuf);
        pTxBuf = NULL;
    }

    transmission_is_active = CO_FALSE;
    return(RET_OK);

}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        can driver transmit interrupt
*
* This function is called, after a message was transmitted. As first, inform stack
* about message transmission. Get the next message from the transmit buffer, write
* it to the CAN controller and transmit it.
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void codrvCanTransmitInterrupt(void)
{
    transmission_is_active = CO_FALSE;

    /* inform stack about transmitted message */
    if (pTxBuf != NULL)  {
        coQueueMsgTransmitted(pTxBuf);
        pTxBuf = NULL;
    }

    /* get next message from transmit queue */
    pTxBuf = coQueueGetNextTransmitMessage();
    if (pTxBuf != NULL)  {
        /* and transmit it - CAN message buffer is free */
        (void)codrvCanTransmit(pTxBuf);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        can driver receive interrupt
*
* This function is called, if a new message was received. As first get the pointer
* to the receive buffer and save the message there. Then set the buffer as filled and
* inform the lib about new data.
*
* @param[in]    rx_message      receive message
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void codrvCanReceiveInterrupt(const can_message_t *const rx_message)
{
    CO_CAN_MSG_T *pRecBuf;
    // get receiveBuffer
    pRecBuf = coQueueGetReceiveBuffer();
    if (pRecBuf == NULL)
    {
        /* error, no buffer available */
        return; /*lint !e904 no buffer possible */
    }

    /* save message at buffer */
    pRecBuf->canCob.canId = rx_message->id;

    pRecBuf->canCob.extended = CO_FALSE;        // TODO hadle extended addressing and remote frames
    pRecBuf->canCob.rtr = CO_FALSE;
    pRecBuf->len = rx_message->len;

    pRecBuf->data[0] = rx_message->data.byte[0];
    pRecBuf->data[1] = rx_message->data.byte[1];
    pRecBuf->data[2] = rx_message->data.byte[2];
    pRecBuf->data[3] = rx_message->data.byte[3];

    if (pRecBuf->len > 4u)
    {
        pRecBuf->data[4] = rx_message->data.byte[4];
        pRecBuf->data[5] = rx_message->data.byte[5];
        pRecBuf->data[6] = rx_message->data.byte[6];
        pRecBuf->data[7] = rx_message->data.byte[7];
    }

    // set buffer filled
    coQueueReceiveBufferIsFilled();

    // inform stack about new data
    coCommTaskSet(CO_COMMTASK_EVENT_MSG_AVAIL);

    // Process new message
    coStartCOMMTaskImmediatelly_FromISR();
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////


