//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        wifi.c
// CREATION DATE:      2019/12/04
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Interface to WiFi communication port.
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include <string.h>
#include "cfg/project_cfg.h"
#include "Platform/proc.h"
#include "wifi.h"
#include "hisense/hisense_Base.h"
#include "serialprocess.h"


/* Time in one-byte-time-durations between two consequent frames sent on UART: */
#define WIFI_TX_INTERFRAME_PAUSE                    20

#define WIFI_SYNCHRO_TX_CHAR1                       HISENSE_LL_FRAME_HEADER_1
#define WIFI_SYNCHRO_TX_CHAR2                       HISENSE_LL_FRAME_HEADER_2


/*
Private functions:
*/

/*
Forward "frame_to_forward" to UARTs defined in "layer->forward.wifilayer_reference"
Return values:
0 - frame_to_forward not accepted. frame_to_forward must be freed in function that called the wifi_forward_inputData_packet() function.
1 - frame_to_forward accepted.     frame_to_forward will be freed when forward is finished.
*/
uint8_t wifi_forward_inputData_packet(WIFI_LAYER *layer, NETBUF* frame_to_forward)
{
    #if (FORWARDING_USE == 1)
    uint8_t i;
    NETBUF* frames_for_forwarding[MAX_NUMBER_OF_FORWARD_PORTS];
    WIFI_LAYER* wifilayer_dst;
    NETBUF* frame_to_forward_stuffed;

    if (layer == NULL)
    {
        return 0;
    }
    if (frame_to_forward == NULL)
    {
        return 0;
    }
    
    if (layer->forward.number_of_forwarding_ports == 0)
    {
        return 0;
    }

    /* Verify if there are valid forward wifi ports: */
    for (i = 0; i < (layer->forward.number_of_forwarding_ports); i++)
    {
        wifilayer_dst = (WIFI_LAYER*) layer->forward.wifilayer_reference[i];
        if (wifilayer_dst == NULL)
        {
            return 0;
        }
    }

    frame_to_forward_stuffed = hisense_Base_HandleStuffedBytes_nb(frame_to_forward);
    if (frame_to_forward_stuffed == NULL)
    {
        return 0;
    }

    /* Make copies of netbuffer for all forward ports: */
    frames_for_forwarding[0] = frame_to_forward_stuffed;
    for (i = 1; i < (layer->forward.number_of_forwarding_ports); i++)
    {
        frames_for_forwarding[i] = netbuf_copy_nb(frame_to_forward_stuffed);
        if (frames_for_forwarding[i] == NULL)
        {
            return 0;
        }
    }

    /* Queue to forward tx queues:*/
    for (i = 0; i < (layer->forward.number_of_forwarding_ports); i++)
    {
        wifilayer_dst = (WIFI_LAYER*) layer->forward.wifilayer_reference[i];

        if (NQ_QFULL(&wifilayer_dst->tx_queue))
        {
            /* If queue is full, discard frame that has to be sent out: */
            NQ_DROP(&wifilayer_dst->tx_queue);
            netbuf_freem(frames_for_forwarding[i]);
        }
        else
        {
            /* Add frame that has to be transmitted to queue: */
            NQ_ENQUEUE(&wifilayer_dst->tx_queue, frames_for_forwarding[i]);
        }
    }

    return 1;
    #else  /* (FORWARDING_USE == 1) */
    return 0;
    #endif /* (FORWARDING_USE == 0) */
}


uint8_t wifi_forward_inputData_byte_flash(WIFI_LAYER* layer, uint8_t data)
{
    #if (FORWARDING_USE == 1)
    uint8_t i;
    WIFI_LAYER* wifilayer_dst;
    uint8_t data_local;

    /* Allowed only if Direct Fast Flashing mode is active: */
    if (layer->mode_state != WIFI_MODE_STATE_DFF_FORWARDING_ONLY)
    {
        return 0;
    }

    /* Checking if all forward ports are in correct states: */
    for (i = 0; i < (layer->forward.number_of_forwarding_ports); i++)
    {
        wifilayer_dst = (WIFI_LAYER*) layer->forward.wifilayer_reference[i];
        if (wifilayer_dst->mode_state != WIFI_MODE_STATE_DFF_FORWARDING_ONLY)
        {
            /* Destination forward port is wrong state */
            return 0;
        }
    }

    /* Sending data on destination forward ports' TX queues: */
    for (i = 0; i < (layer->forward.number_of_forwarding_ports); i++)
    {
        wifilayer_dst = (WIFI_LAYER*) layer->forward.wifilayer_reference[i];
        timer_start(&wifilayer_dst->mode_state_timer);

        /* When data comes, data timeout is taken info account: */
        wifilayer_dst->dff_initial_timeout = layer->dff_initial_timeout;
        wifilayer_dst->dff_data_timeout    = layer->dff_data_timeout;

        /* Assure net buffer: */
        if (wifilayer_dst->dff_buffer[0] == NULL)
        {
            wifilayer_dst->dff_buffer[0] = netbuf_gethdr();
        }
        if (wifilayer_dst->dff_buffer[0] == NULL)
        {
            /* No net buffers available - discard data */
            return 0;
        }

        /* Add data to net buffer: */
        data_local = data; 
        if (netbuf_append(wifilayer_dst->dff_buffer[0], 1, (void*)&data_local) != 1)
        {
            return 0;
        }
    }

    return 1;

    #else /* (FORWARDING_USE == 1) */
    return 0;
    #endif 
}


#if (   (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)           \
     || (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)           \
     || (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)      \
     || (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)       \
     || (PLATFORM_USED == PLATFORM_NXP_LPC546XX)            \
     || (PLATFORM_USED == PLATFORM_STM_STM32G0xx)           \
     || (PLATFORM_USED == PLATFORM_STM32_STM32GXX)          \
     || (PLATFORM_USED == PLATFORM_STM_STM32F4xx))
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t wifi_forward_inputData_byte(WIFI_LAYER* layer, uint8_t data)
{
    /* Call the wifi_forward_inputData_byte_flash() function which is located in Flash.
       This is just warning suppresion reported by IAR compiler on MB9AF114MA, MB9BF121J and LPC546XX platforms.
       This function is not called when flash is writing.
   */
    /* wifi_forward_inputData_byte_flash(layer, data);  ---> */
    __asm volatile("    PUSH    {R7, LR}\n");
    __asm volatile("    UXTB    R1, R1\n");
    __asm volatile("    BL      wifi_forward_inputData_byte_flash\n");
    __asm volatile("    POP     {R1, PC}\n");
    return 0;           /* Dummy return */
}
#elif (   (PLATFORM_USED == PLATFORM_STM8_STM8S207)         \
       || (PLATFORM_USED == PLATFORM_STM_STM32G0xx)         \
       || (PLATFORM_USED == PLATFORM_STM32_STM32GXX)        \
      )
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t wifi_forward_inputData_byte(WIFI_LAYER* layer, uint8_t data)
{
    return wifi_forward_inputData_byte_flash(layer, data);
}
#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t wifi_forward_inputData_byte(WIFI_LAYER* layer, uint8_t data)
{
    #warning wifi.c: implement wifi_forward_inputData_byte() function for PLATFORM_CYPRESS_PSOC4100SMAX platform!
    return wifi_forward_inputData_byte_flash(layer, data);
}
#elif (PLATFORM_USED == PLATFORM_template)
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t wifi_forward_inputData_byte(WIFI_LAYER* layer, uint8_t data)
{
    #error wifi.c: implement wifi_forward_inputData_byte() function for new platform!
}
#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error wifi.c: Undefined or unsupported platform
#endif

void wifi_TxFrameProcess(WIFI_LAYER *layer)
{
    TIMER guardTimer;
    uint8_t *pdata;
    uint8_t data;
    uint16_t tosend;
    uint32_t maximum_tx_time;
    uint32_t one_byte_duration_us;
    uint8_t enableInterrupts;

    /* Sending of previous frame is still in progress: check timeouts: */
    if (layer->tx_frame != NULL)
    {
        /* Calculating maximum TX time in milliseconds: */
        if (layer->actualBaudrate == 0) layer->actualBaudrate = (SERIALPORT_BAUDRATE) 1;
        maximum_tx_time = 1000 * ((layer->tx_frame_len) * 10) / (layer->actualBaudrate)   *   11/10   +   3;
        if (timer_elapsed(&layer->packet_tx_timer) >= maximum_tx_time)
        {
            /* If the condition is met, check if there is a intercharacter pause in TX or if TX interrupt is not working. */
            (layer->counters.tx_timeout)++;
            layer->packet_sending = 0;
            netbuf_freem(layer->tx_frame);
            layer->tx_frame = NULL;
        }
    }   /* --> layer->tx_frame != NULL */
    else    /* layer->tx_frame == NULL: */
    {
        /* Check if there is TX frame waiting to be sent out: */
        if (layer->tx_frame_waiting != NULL)
        {
            if (hisense_LinkL_MessagePreparedForTransmission(layer->linkLayer) == 1)
            {
                if (NQ_QFULL(&layer->tx_queue))
                {
                    /* If queue is full, discard frame that has to be sent out: */
                    NQ_DROP(&layer->tx_queue);
                    netbuf_freem(layer->tx_frame_waiting);
                }
                else
                {
                    #if (SYNCHRONIZED_TX_USE == 1)
                    /* Wait for next synchronization: */
                    layer->synchronized_tx_ctrl.synchronized_tx_requested = 0;
                    #endif /* (SYNCHRONIZED_TX_USE == 1) */

                    /* Add frame that has to be transmitted to queue: */
                    NQ_ENQUEUE(&layer->tx_queue, layer->tx_frame_waiting);
                }
                layer->tx_frame_waiting = NULL; /* Added to queue or discarded */
            }
        }

        /* Wait that eventual character from forward is sent out, but maximum 3 ms: */
        if ((serialport_TxComplete(layer->serialPort) == 0))
        {
            timer_start(&guardTimer);
            while ((serialport_TxComplete(layer->serialPort) == 0) &&
                   (timer_elapsed(&guardTimer) < 3))                        /* 3 ms */
            {
                proc_clearWDT();
            }
            /* If it has to be waited for transmit is complete, wait additional 5 byte time */
            proc_delayus(min(1000, 5*1000000*10/(layer->actualBaudrate)));
        }

        /* Wait TX interframe delay: */
        one_byte_duration_us = 1000000 * 10 / (layer->actualBaudrate);
        if (timer_elapsed(&layer->packet_tx_timer) < ((one_byte_duration_us * WIFI_TX_INTERFRAME_PAUSE) / 1000 + 2))
        {
            return;
        }

        #if (SYNCHRONIZED_TX_USE == 1)
        if (timer_elapsed(&layer->synchronized_tx_ctrl.last_synchronization_time) < SYNCHRONIZED_TX_NOSYNCHRO_TIMEOUT)
        {
            if (layer->synchronized_tx_ctrl.synchronized_tx_requested != 1)
            {
                return;
            }
        }
        #endif /* (SYNCHRONIZED_TX_USE == 1) */

        /* Check if there is something to send: */
        NQ_DEQUEUE(&layer->tx_queue, layer->tx_frame);
        if (layer->tx_frame == NULL)
        {
            #if (SYNCHRONIZED_TX_USE == 1)
            if (layer->synchronized_tx_ctrl.synchronized_tx_requested == 1)
            {
                serialport_WriteTxData(layer->serialPort, WIFI_SYNCHRO_TX_CHAR1);
                serialport_WriteTxData(layer->serialPort, WIFI_SYNCHRO_TX_CHAR2);
                layer->synchronized_tx_ctrl.synchronized_tx_requested = 0;    /* --> sent */
            }
            #endif /* (SYNCHRONIZED_TX_USE == 1) */

            return;     /* Nothing to send */
        }
        #if (SYNCHRONIZED_TX_USE == 1)
        layer->synchronized_tx_ctrl.synchronized_tx_requested = 0;    /* --> sent */
        #endif /* (SYNCHRONIZED_TX_USE == 1) */
        
        layer->tx_frame_len = layer->tx_frame->nb_pktlen;
        while ((tosend = layer->tx_frame->nb_len) == 0)
        {
            if ((layer->tx_frame = netbuf_free(layer->tx_frame)) == NULL)
            {
                return; /* Nothing to send */
            }
        }
        tosend = layer->tx_frame->nb_len;
        if (tosend == 0)
        {
            return;     /* Nothing to send */
        }
        pdata = NBTOD(layer->tx_frame, uint8_t*);
        data = *pdata;
        if (netbuf_remove(layer->tx_frame, 1) == 1)
        {
            timer_start(&layer->packet_tx_timer);       /* Start of frame sending */
            enableInterrupts = proc_EnterCriticalSection();
            serialport_WriteTxData(layer->serialPort, data);
            layer->packet_sending = 1;
            proc_ExitCriticalSection(enableInterrupts);
        }
    }   /* --> layer->tx_frame == NULL */
}   /* --> wifi_TxFrameProcess() */

void wifi_RxFrameProcess(WIFI_LAYER *layer)
{
    NETBUF* rx_frame;
    NETBUF* tx_frame;
    uint8_t action;
    uint8_t app_al_response_id;
    uint16_t app_data_position;
    uint16_t app_data_size;

    NQ_DEQUEUE(&layer->rx_queue, rx_frame);
    if (rx_frame == NULL)
    {
        return;     /* Nothing received */
    }
    tx_frame = NULL;
    action = hisense_LinkL_RxFrameProcess(layer->linkLayer, rx_frame, &tx_frame, &app_al_response_id, &app_data_position, &app_data_size);
    switch (action)
    {
        case 0: /* Fail.          MCU should not send reply to the packet. "rx_frame" netbuffer must be released !!! */
        {
            netbuf_freem(rx_frame);
            netbuf_freem(tx_frame); /* tx_frame should not be occupied, but just in case */
            break;
        }

        case 1: /* Validation OK. MCU should reply to the packet. Reply is in "tx_frame" netbuffer. */
        {
            timer_start(&layer->packet_rx_timer);
            netbuf_freem(rx_frame);
            if (tx_frame == NULL)
            {
                break;
            }
            
            if (hisense_LinkL_MessagePreparedForTransmission(layer->linkLayer) == 0)
            {
                /* Frame is not confirmed in the Application layer that it can be sent out. Save for later sending: */
                netbuf_freem(layer->tx_frame_waiting);      /* Discard eventual previous frame that was waiting */
                layer->tx_frame_waiting = tx_frame;
            }
            else
            {
                if (NQ_QFULL(&layer->tx_queue))
                {
                    /* If queue is full, discard frame that has to be sent out: */
                    NQ_DROP(&layer->tx_queue);
                    netbuf_freem(tx_frame);
                    (layer->counters.tx_queuefull)++;
                }
                else
                {
                    #if (SYNCHRONIZED_TX_USE == 1)
                    /* Wait for next synchronization: */
                    layer->synchronized_tx_ctrl.synchronized_tx_requested = 0;
                    #endif /* (SYNCHRONIZED_TX_USE == 1) */

                    /* Add frame that has to be transmitted to queue: */
                    NQ_ENQUEUE(&layer->tx_queue, tx_frame);
                }
            }
            break;
        }

        case 2: /* Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch). */
        {
            timer_start(&layer->packet_rx_timer);
            netbuf_freem(tx_frame); /* tx_frame should not be occupied, but just in case */

            if (wifi_forward_inputData_packet(layer, rx_frame) != 1)
            {
                /* Forwarding not successful: */
                netbuf_freem(rx_frame);
            }
            break;
        }

        case 3: /* 3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called. */
        {
            #if (MASTER_FUNCTIONALITY_USE == 1)
            uint8_t data[MAX_DATA_MESSAGE_LENGTH];
            uint16_t dataSize, dataCopied;

            /* Deactivate Master request callback verification in application layer: */
            hisense_ApplicationLayerMasterRequest_SetMessageType(layer->linkLayer->networkLayer->transmissionLayer->applicationLayer, 0, 0, 0);

            dataSize = min(app_data_size, sizeof(data));
            dataCopied = netbuf_copyfrom(rx_frame, app_data_position, dataSize, data);
            netbuf_freem(rx_frame);
            netbuf_freem(tx_frame); /* tx_frame should not be occupied, but just in case */

            if (dataCopied == dataSize)
            {
                /* Call the callback function: */
                (layer->master_ctrl.callback_func)((void*) &app_al_response_id, (void*) data, (void*) &dataSize);
            }
            #endif /* (MASTER_FUNCTIONALITY_USE == 1) */
            break;
        }

        default:
        {
            netbuf_freem(rx_frame);
            netbuf_freem(tx_frame); /* tx_frame should not be occupied, but just in case */
            break;
        }
    } /* --> switch (action) */
}


void wifi_RxTxFrameProcess(WIFI_LAYER *layer)
{
    wifi_RxFrameProcess(layer);
    wifi_TxFrameProcess(layer);
}

void wifi_TX_callback_flash(void* layer)
{
    WIFI_LAYER* wifiLayer;
    SERIALPORT_TXSTATUS status;
    uint16_t tosend;
    uint8_t enableInterrupts;
    uint8_t *pdata;
    uint8_t data;

    if (layer == NULL)
    {
        return;
    }
    wifiLayer = (WIFI_LAYER*) layer;

    status = serialport_ReadTxStatus(wifiLayer->serialPort);
    if (!(status & SERIALPORT_TXSTATUS_FIFO_EMPTY))
    {
        return;
    }
    serialport_ClearTxStatus(wifiLayer->serialPort, (SERIALPORT_TXSTATUS) (status & SERIALPORT_TXSTATUS_FIFO_EMPTY));

    switch (wifiLayer->mode_state)
    {
        case WIFI_MODE_STATE_NORMAL_DATARX:
        case WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME:
        case WIFI_MODE_STATE_DFF_ACTIVE:
        case WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST:
        {
            if (wifiLayer->tx_frame == NULL)
            {
                break;     /* Sending already finished (timeout...) */
            }

            do
            {
                enableInterrupts = proc_EnterCriticalSection();
                tosend = wifiLayer->tx_frame->nb_len;
                if (tosend == 0)
                {
                    wifiLayer->tx_frame = netbuf_free(wifiLayer->tx_frame);
                }
                proc_ExitCriticalSection(enableInterrupts);

                if (wifiLayer->tx_frame == NULL)
                {
                    /* All data sent in previous call of interrupt: */
                    timer_start(&wifiLayer->packet_tx_timer);       /* End of frame sending */
                    wifiLayer->packet_sending = 0;
                    (wifiLayer->counters.tx)++;

                    if (wifiLayer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST)
                    {
                        timer_start(&wifiLayer->mode_state_timer);
                        wifiLayer->mode_state = WIFI_MODE_STATE_DFF_FORWARDING_ONLY;
                    }
                    break;  /* Nothing left to send */
                }
                tosend = wifiLayer->tx_frame->nb_len;
            } while (tosend == 0);

            if (wifiLayer->tx_frame == NULL)
            {
                break;      /* Nothing left to send */
            }

            #if (PLATFORM_USED == PLATFORM_STM_STM32G0xx)
            /* STM32G0xx UART 3,4,5,6 shared interrupt fix: */
            if (wifiLayer->packet_sending == 0)
            {
                break;      /* Wait for first byte to be sent to FIFO, see wifi_TxFrameProcess() -> serialport_WriteTxData(). */
            }
            #endif /* --> PLATFORM_USED == PLATFORM_STM_STM32G0xx */

            tosend = wifiLayer->tx_frame->nb_len;
            if (tosend == 0)
            {
                /* All data sent in previous call of interrupt: */
                timer_start(&wifiLayer->packet_tx_timer);       /* End of frame sending */
                wifiLayer->packet_sending = 0;

                if (wifiLayer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST)
                {
                    timer_start(&wifiLayer->mode_state_timer);
                    wifiLayer->mode_state = WIFI_MODE_STATE_DFF_FORWARDING_ONLY;
                }
                break;
            }
            pdata = NBTOD(wifiLayer->tx_frame, uint8_t*);
            data = *pdata;
            if (netbuf_remove(wifiLayer->tx_frame, 1) == 1)
            {
                serialport_WriteTxData(wifiLayer->serialPort, data);
            }
            break;
        }   /* --> WIFI_MODE_STATE_NORMAL_DATARX, WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME, WIFI_MODE_STATE_DFF_ACTIVE or WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST */

        case WIFI_MODE_STATE_DFF_FORWARDING_ONLY:
        {
            if (wifiLayer->dff_buffer[0] == NULL)
            {
                break;
            }
            if (wifiLayer->dff_buffer[0]->nb_pktlen == 0)
            {
                /* Nothing to send: */
                break;
            }
            pdata = NBTOD(wifiLayer->dff_buffer[0], uint8_t*);
            data = *pdata;

            if (netbuf_remove(wifiLayer->dff_buffer[0], 1) == 1)
            {
                if (wifiLayer->dff_buffer[0]->nb_len == 0)
                {
                    wifiLayer->dff_buffer[0] = netbuf_free(wifiLayer->dff_buffer[0]);
                }

                serialport_WriteTxData(wifiLayer->serialPort, data);
            }
            break;
        }   /* --> WIFI_MODE_STATE_DFF_FORWARDING_ONLY */

        default:
        {
            break;
        }
    }   /* --> switch (wifiLayer->mode_state) */
}   /* --> wifi_TX_callback_flash() */

#if ((PLATFORM_USED == PLATFORM_NXP_LPC546XX)           || \
     (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)     || \
     (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J))
    /* Call the wifi_TX_callback_flash() function which is located in Flash.
       This is just warning suppresion reported by IAR compiler on LPC546XX platform.
       Prevention that this function is not called when flash is writing is done on different way: flash_is_writing().
       For MB9AF114MA and MB9BF121J platforms this is also a warning suppresion because 
       the wifi_TX_callback() function is not even called from RAM function.
   */
    /* wifi_TX_callback_flash(layer);  ---> */
PROC_RAM_FUNCTION_ALWAYS_INFRONT void wifi_TX_callback(void* layer)
{
    __asm volatile("    PUSH    {R7, LR}\n");
    __asm volatile("    UXTB    R1, R1\n");
    __asm volatile("    BL      wifi_TX_callback_flash\n");
    __asm volatile("    POP     {R0, PC}\n");
}
#else
PROC_RAM_FUNCTION_ALWAYS_INFRONT void wifi_TX_callback(void* layer)
{
    wifi_TX_callback_flash(layer);
}
#endif

#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_START
#else
void wifi_RX_callback(void* layer) PROC_RAM_FUNCTION;
#endif
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void wifi_RX_callback(void* layer)
{
    SERIALPORT_RXSTATUS status;
    uint8_t bytesRead;
    uint8_t ch;
    uint8_t retValue;
    uint8_t buffNo;
    WIFI_LAYER* wifiLayer;
    #if ((PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)              || \
         (PLATFORM_USED == PLATFORM_NXP_LPC546XX)               || \
         (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)       || \
         (PLATFORM_USED == PLATFORM_STM_STM32G0xx)              || \
         (PLATFORM_USED == PLATFORM_STM_STM32F4xx))
    SERIALPORT_TXSTATUS txStatus;
    #endif

    if (layer == NULL)
    {
        return;
    }
    wifiLayer = (WIFI_LAYER*) layer;

    bytesRead = 0;
    do 
    {
        status = serialport_ReadRxStatus(wifiLayer->serialPort);
        if (status & SERIALPORT_RXSTATUS_FIFO_NOTEMPTY)
        {
            ch = serialport_ReadRxData(wifiLayer->serialPort);    /* Returns the next byte of received data. */
            serialport_ClearRxStatus(wifiLayer->serialPort, SERIALPORT_RXSTATUS_FIFO_NOTEMPTY);

            switch (wifiLayer->mode_state)
            {
                case WIFI_MODE_STATE_NORMAL_DATARX:
                {
                    /* Assure net buffer: */
                    if (wifiLayer->rx_frame == NULL)
                    {
                        wifiLayer->rx_frame = netbuf_gethdr();
                    }
                    if (wifiLayer->rx_frame == NULL)
                    {
                        /* No net buffers available - discard data */
                        break;
                    }
                    retValue = hisense_LinkL_ReceiveByte(wifiLayer->linkLayer,
                                                         wifiLayer->rx_frame,
                                                         ch);
                    switch (retValue)
                    {
                        /* Frame not complete yet: */
                        case 0:
                        {
                            /* Wait to complete frame or error */
                            break;
                        }

                        /* Frame complete: Adding to queue, if not full, otherwise discard frame: */
                        case 1:
                        {
                            if (NQ_QFULL(&wifiLayer->rx_queue))
                            {
                                /* If queue is full, discard received frame: */
                                NQ_DROP(&wifiLayer->rx_queue);
                                netbuf_freem(wifiLayer->rx_frame);
                                wifiLayer->rx_frame = NULL;
                                (wifiLayer->counters.rx_queuefull)++;
                            }
                            else
                            {
                                /* Add received frame to queue: */
                                NQ_ENQUEUE(&wifiLayer->rx_queue, wifiLayer->rx_frame);
                                wifiLayer->rx_frame = NULL;
                                (wifiLayer->counters.rx)++;
                            }
                            break;
                        }

                        case 2:     /* Frame error (cannot add to net buffer, or too many data came): */
                        case 3:     /* Invalid data: */
                        case 4:     /* Invalid parameters: */
                        {
                            netbuf_freem(wifiLayer->rx_frame);
                            wifiLayer->rx_frame = NULL;
                            break;
                        }

                        default:
                        {
                            netbuf_freem(wifiLayer->rx_frame);
                            wifiLayer->rx_frame = NULL;
                            break;
                        }
                    }   /* --> switch (retValue) */
                    break;
                }           /* --> case WIFI_MODE_STATE_NORMAL_DATARX */

                case WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME:
                {
                    /* Do nothing. Discard received data. */
                    break;
                }

                case WIFI_MODE_STATE_DFF_ACTIVE:
                {
                    if (wifiLayer->dff_rx_enabled != 1)
                    {
                        break;
                    }
                    buffNo = wifiLayer->currentRxBuffer;

                    /* Add data to buffer: */
                    if (wifiLayer->dff_buffer[buffNo] == NULL)
                    {
                        break;
                    }
                    if (netbuf_append(wifiLayer->dff_buffer[buffNo], 1, (void*) &ch) != 1)
                    {
                        break;
                    }

                    wifiLayer->dff_bytesToReceive--;
                    timer_start(&wifiLayer->mode_state_timer);
                    /* When data comes, data_timeout is taken info account: */
                    wifiLayer->dff_initial_timeout = wifiLayer->dff_data_timeout;
                    break;
                }

                #if (FORWARDING_USE == 1)
                case WIFI_MODE_STATE_DFF_FORWARDING_ONLY:
                {
                    if (flash_is_writing() == 0)
                    {
                        if (wifi_forward_inputData_byte(wifiLayer, ch) != 1)
                        {
                            /* ERROR */
                        }

                        timer_start(&wifiLayer->mode_state_timer);
                        /* When data comes, data timeout is taken info account: */
                        wifiLayer->dff_initial_timeout = wifiLayer->dff_data_timeout;
                    }   /* --> (flash_is_writing() == 0) */
                    break;
                }
                #endif /* (FORWARDING_USE == 1) */

                default:
                {
                    break;
                }
            }   /* --> switch (wifiLayer->mode_state) */
            
            bytesRead++;
        }
    }
    while ((status & SERIALPORT_RXSTATUS_FIFO_NOTEMPTY) && (bytesRead < 32));
    
    #if ((PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)              || \
         (PLATFORM_USED == PLATFORM_NXP_LPC546XX)               || \
         (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)       || \
         (PLATFORM_USED == PLATFORM_STM_STM32G0xx)              || \
         (PLATFORM_USED == PLATFORM_STM_STM32F4xx))
    /* PSoC4, LPC546xx, PSOC4100SMAX and STM32G0xx platforms specific: the same ISR for RX and TX. */
    /* Disable Hisense frame transmitting while flash write is in progress: */
    if (flash_is_writing() == 0)
    {
        wifi_TX_callback(layer);
    }
    else
    {
        /* Clear status flag that interrupt will not occur again: */
        txStatus = serialport_ReadTxStatus(wifiLayer->serialPort);
        serialport_ClearTxStatus(wifiLayer->serialPort, txStatus);
    }
    #endif /* (PLATFORM_USED: PLATFORM_CYPRESS_PSOC4, PLATFORM_NXP_LPC546XX, PLATFORM_CYPRESS_PSOC4100SMAX or PLATFORM_STM_STM32G0xx or PLATFORM_STM_STM32F4xx) */
}
#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
#pragma PROC_RAM_FUNCTION_END
#endif



/*
API functions: 
*/

void wifi_init(WIFI_LAYER *layer)
{
    uint8_t i;

    if (layer == NULL)
    {
        return;
    }

    /* Freeing netbuffers used for DFF mode and setting layer->mode_state to  WIFI_MODE_STATE_NORMAL_DATARX: */
    wifi_dff_mode_set(layer, WIFI_DFF_MODE_DISABLE, 0, 0);
    layer->dff_rx_enabled = 1;

    timer_stop(&layer->packet_rx_timer);        /* No frame received yet. */
    timer_start(&layer->packet_tx_timer);       /* First frame can be sent after WIFI_TX_INTERFRAME_PAUSE milliseconds. */
    layer->packet_sending = 0;                  /* Packet is currently not sending. */
    memset(&layer->counters, 0x00, sizeof(layer->counters));

    /* Free netbuffers and queues if the  wifi_init() function was run from MCU running state (not after power-on reset): */
    netbuf_freem(layer->rx_frame);
    netbuf_freem(layer->tx_frame);
    netbuf_freem(layer->tx_frame_waiting);
    netbuf_freeq(&layer->rx_queue);
    netbuf_freeq(&layer->tx_queue);
    layer->rx_frame         = NULL;
    layer->tx_frame         = NULL;
    layer->tx_frame_waiting = NULL;
    layer->rx_queue.maxlen  = NUMBER_OF_MASTERS;
    layer->tx_queue.maxlen  = NUMBER_OF_MASTERS;
    for (i = 0; i < WIFI_NUMBER_OF_BUFFERS; i++)
    {
        netbuf_freem(layer->dff_buffer[i]);
        layer->dff_buffer[i] = NULL;
    }

    serialport_init(layer->serialPort, layer->rx_alt_pin, layer->tx_alt_pin);
    layer->actualBaudrate = (SERIALPORT_BAUDRATE) serialport_baudrate_set(layer->serialPort, layer->baudrate);

    hisense_LinkL_init(layer->linkLayer);
}

void wifi_reinit_baudrate(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }

    layer->actualBaudrate = (SERIALPORT_BAUDRATE) serialport_baudrate_set(layer->serialPort, layer->baudrate);
}

void wifi_initDone(WIFI_LAYER *layer)
{
    /* Obsolete */
}

uint32_t wifi_counter_get(WIFI_LAYER *layer, WIFI_LAYER_COUNTER counter)
{
    uint32_t cnt;

    if (layer == NULL)
    {
        return 0;
    }
    if (counter >= WIFI_LAYER_COUNTER_MAX)
    {
        return 0;
    }

    cnt = 0;
    switch (counter)
    {
        case WIFI_LAYER_COUNTER_RX:
        {
            cnt = layer->counters.rx;
            break;
        }

        case WIFI_LAYER_COUNTER_RX_QUEUEFULL:
        {
            cnt = layer->counters.rx_queuefull;
            break;
        }

        case WIFI_LAYER_COUNTER_TX:
        {
            cnt = layer->counters.tx;
            break;
        }
        
        case WIFI_LAYER_COUNTER_TX_QUEUEFULL:
        {
            cnt = layer->counters.tx_queuefull;
            break;
        }

        case WIFI_LAYER_COUNTER_TX_TIMEOUT:
        {
            cnt = layer->counters.tx_timeout;
            break;
        }

        default:
        {
            cnt = 0;
            break;
        }
    }
    return cnt;
}

void wifi_interrupts_enable(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }

    serialport_RX_data_callback_register(layer->serialPort, (void*) wifi_RX_callback, (void*) layer);
    serialport_TX_data_callback_register(layer->serialPort, (void*) wifi_TX_callback, (void*) layer);

    serialport_ClearRxBuffer(layer->serialPort);         /* Clear received characters left from Application / Bootloader. */

    serialport_RXinterruptEnable(layer->serialPort);
    serialport_TXinterruptEnable(layer->serialPort);

    serialport_ClearRxBuffer(layer->serialPort);         /* Clear received characters left from Application / Bootloader.
                                                            The function is intentionally called again after enabling interrupts. */
}

void wifi_interrupts_disable(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }

    serialport_RXinterruptDisable(layer->serialPort);
    serialport_TXinterruptDisable(layer->serialPort);
}

void wifi_enableReceiver(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }

    serialport_RXinterruptEnable(layer->serialPort);
}

void wifi_disableReceiver(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }

    serialport_RXinterruptDisable(layer->serialPort);
}

uint8_t wifi_isSending(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return 0;
    }

    if (serialport_TxComplete(layer->serialPort) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void wifi_dff_mode_set(WIFI_LAYER *layer, wifi_dff_mode_t dff_mode, uint32_t initial_timeout, uint32_t data_timeout)
{
    if (layer == NULL)
    {
        return;
    }

    #if (PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)
    #if (FORWARDING_USE == 1)
    WIFI_LAYER *fwdlayer;
    #endif /* (FORWARDING_USE == 1) */
    uint8_t i;
    BOOTLOADER_APP_ADDRESS_START_END *app_address_space_start_end;
    
    /* Allowed parameter values: */
    if (!((dff_mode == WIFI_DFF_MODE_DISABLE) ||
          (dff_mode == WIFI_DFF_MODE_FORWARDING_ONLY) ||
          (dff_mode == WIFI_DFF_MODE_ENABLE)))
    {
        /* Not valid dff_mode parameter! */
        return;
    }
    
    layer->dff_initial_timeout = initial_timeout;
    layer->dff_data_timeout = data_timeout;
    timer_start(&layer->mode_state_timer);

    if (dff_mode == WIFI_DFF_MODE_DISABLE)
    {
        /* Prevent loops: */
        if (layer->mode_state != WIFI_MODE_STATE_NORMAL_DATARX)
        {
            /* Exiting DFF mode for this interface:*/
            layer->dff_initial_timeout = 0;
            layer->dff_data_timeout = 0;
            for (i = 0; i < WIFI_NUMBER_OF_BUFFERS; i++)
            {
                netbuf_freem(layer->dff_buffer[i]);
                layer->dff_buffer[i] = NULL;
            }
            layer->mode_state = WIFI_MODE_STATE_NORMAL_DATARX;

            #if (FORWARDING_USE == 1)
            /* Exiting DFF mode for all forward interfaces:*/
            for (i = 0; i < layer->forward.number_of_forwarding_ports; i++)
            {
                fwdlayer = layer->forward.wifilayer_reference[i];
                wifi_dff_mode_set(fwdlayer, WIFI_DFF_MODE_DISABLE, 0, 0);
            }
            #endif /* (FORWARDING_USE == 1) */
        }
    }

    if (dff_mode == WIFI_DFF_MODE_FORWARDING_ONLY)
    {
        #if (FORWARDING_USE == 1)
        /* Prevent loops: */
        if ((layer->mode_state != WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST) &&
            (layer->mode_state != WIFI_MODE_STATE_DFF_FORWARDING_ONLY))
        {
            /* Set DFF forwarding mode for this interface:*/
            netbuf_freem(layer->dff_buffer[0]);
            layer->dff_buffer[0] = netbuf_gethdr();

            /* First answer to the Request has to be sent: */
            layer->mode_state = WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST;

            /* Set DFF forwarding mode for all forward interfaces: */
            for (i = 0; i < layer->forward.number_of_forwarding_ports; i++)
            {
                fwdlayer = layer->forward.wifilayer_reference[i];
                wifi_dff_mode_set(fwdlayer, WIFI_DFF_MODE_FORWARDING_ONLY, initial_timeout, data_timeout);
            }
        }
        #endif /* (FORWARDING_USE == 1) */
    }

    if (dff_mode == WIFI_DFF_MODE_ENABLE)
    {
        /* Go to guard time state first: */
        layer->mode_state = WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME;

        for (i = 0; i < WIFI_NUMBER_OF_BUFFERS; i++)
        {
            netbuf_freem(layer->dff_buffer[i]);
            layer->dff_buffer[i] = netbuf_gethdr();
        }
        layer->currentRxBuffer = 0;
        layer->currentWriteBuffer = 0;
        app_address_space_start_end = bootloader_getApplicationAddressSpace();
        layer->dff_bytesToReceive = SWAP_ENDIAN32(app_address_space_start_end->bootloader_app_address_end) - 
                                    SWAP_ENDIAN32(app_address_space_start_end->bootloader_app_address_start) + 1;
    }
    #endif  /*(PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)*/
}

#if (MASTER_FUNCTIONALITY_USE == 1)
WIFI_MASTER_REQUEST_RESULT wifi_master_request(SERIALPORT_NUMBER serialport,
                                               WIFI_MASTER_REQUEST_DST_DATA destination_data,
                                               callbackFunc_3args callback_address,
                                               void* caller_address)
{
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;
    hisense_LinkLayerPacket packet;
    NETBUF* tx_frame;

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_PORT;
    }
    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_PORT;
    }

    if (wifilayer->mode_state != WIFI_MODE_STATE_NORMAL_DATARX)
    {
        return WIFI_MASTER_REQUEST_RESULT_NOT_READY;
    }

    if ((callback_address == NULL) || (caller_address == NULL))
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_DATA;
    }

    if (destination_data.nl_AddressCode1_length != 2)
    {
        /* Only two bytes Networl Layer address code is supported */
        return WIFI_MASTER_REQUEST_RESULT_INVALID_DATA;
    }

    if ((destination_data.al_data_length != 0) &&
        (destination_data.al_data == NULL))
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_DATA;
    }

    packet.NetworkLayerContent.AddressCode1[0] = destination_data.nl_AddressCode1[0];
    packet.NetworkLayerContent.AddressCode1[1] = destination_data.nl_AddressCode1[1];
    packet.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.MessageType = destination_data.al_message_type;
    packet.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.MessageSubType = destination_data.al_message_subtype;
    packet.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.DataLength = destination_data.al_data_length;
    packet.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.pData = destination_data.al_data;

    tx_frame = NULL;
    if (hisense_LinkL_BuildLinkLayerMasterRequestMessage(wifilayer->linkLayer, &packet, &tx_frame) != 1)
    {
        netbuf_freem(tx_frame); /* tx_frame should not be occupied, but just in case */
        return WIFI_MASTER_REQUEST_RESULT_FAIL;
    }

    if (NQ_QFULL(&wifilayer->tx_queue))
    {
        /* If queue is full, discard frame that has to be sent out: */
        NQ_DROP(&wifilayer->tx_queue);
        netbuf_freem(tx_frame);
        return WIFI_MASTER_REQUEST_RESULT_FAIL;
    }
    else
    {
        wifilayer->master_ctrl.callback_func = callback_address;
        wifilayer->master_ctrl.calling_func_addr = (callbackFunc) caller_address;

        hisense_ApplicationLayerMasterRequest_SetMessageType(wifilayer->linkLayer->networkLayer->transmissionLayer->applicationLayer,
                                                             1,
                                                             destination_data.al_message_type,
                                                             destination_data.al_message_subtype);

        #if (SYNCHRONIZED_TX_USE == 1)
        /* Wait for next synchronization: */
        wifilayer->synchronized_tx_ctrl.synchronized_tx_requested = 0;
        #endif /* (SYNCHRONIZED_TX_USE == 1) */

        /* Add frame that has to be transmitted to queue: */
        NQ_ENQUEUE(&wifilayer->tx_queue, tx_frame);
    }

    return WIFI_MASTER_REQUEST_RESULT_OK;
}

WIFI_MASTER_REQUEST_RESULT wifi_master_request_cancel(SERIALPORT_NUMBER serialport, void* caller_address)
{
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_PORT;
    }

    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_PORT;
    }

    if (caller_address == NULL)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_DATA;
    }

    if (wifilayer->master_ctrl.calling_func_addr != caller_address)
    {
        return WIFI_MASTER_REQUEST_RESULT_INVALID_DATA;
    }

    /* Deactivate Master request callback verification in application layer: */
    hisense_ApplicationLayerMasterRequest_SetMessageType(wifilayer->linkLayer->networkLayer->transmissionLayer->applicationLayer, 0, 0, 0);

    return WIFI_MASTER_REQUEST_RESULT_OK;
}
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

#if (SYNCHRONIZED_TX_USE == 1)
void wifi_synchronized_tx_request(SERIALPORT_NUMBER serialport)
{
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return;
    }
    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return;
    }
    if (wifilayer->synchronized_tx_ctrl.synchronized_tx_enabled != 1)
    {
        return;
    }

    wifilayer->synchronized_tx_ctrl.synchronized_tx_requested = 1;
    timer_start(&wifilayer->synchronized_tx_ctrl.last_synchronization_time);
    wifi_TxFrameProcess(wifilayer);
}
#endif /* (SYNCHRONIZED_TX_USE == 1) */


#if (RX_SYNCHRONIZATION_USE == 1)
void wifi_rx_synchronization_callback_register(SERIALPORT_NUMBER serialport, void* callback_func_address)
{
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;

    if (callback_func_address == NULL)
    {
        return;
    }
    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return;
    }
    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return;
    }
    if (wifilayer->rx_synchronization_ctrl.rx_synchronization_enabled != 1)
    {
        return;
    }

    wifilayer->rx_synchronization_ctrl.rx_synchronization_callback = (callbackFunc) callback_func_address;
}

void wifi_rx_synchronization_callback(WIFI_LAYER *layer)
{
    if (layer == NULL)
    {
        return;
    }
    if (layer->rx_synchronization_ctrl.rx_synchronization_callback == NULL)
    {
        return;
    }

    /* Call the callback function: */
    layer->rx_synchronization_ctrl.rx_synchronization_callback((void*) &layer->serialPort);
}
#endif /* (RX_SYNCHRONIZATION_USE == 1) */

uint32_t wifi_rx_packet_receive_time(WIFI_LAYER *layer)
{
    uint32_t elapsed;

    if (layer->packet_rx_timer.started == 0)
    {
        elapsed = 0xFFFFFFFF;
    }
    else
    {
        elapsed = timer_elapsed(&layer->packet_rx_timer);
    }

    return elapsed;
}

void wifi_run(WIFI_LAYER *layer)
{
    #if (PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)
    #if (DOUBLE_BUFFERING == 0)
    BOOTLOADER_RESULT ret;
    #endif
    uint8_t buffNo;
    uint16_t pktlen;
    NETBUF* pnb;
    uint8_t *pdata;
    uint8_t data;
    uint32_t dataLen;
    TIMER guardTimer;
    #endif

    if (layer == NULL)
    {
        return;
    }

    wifi_RxTxFrameProcess(layer);
    hisense_LinkL_run(layer->linkLayer);

    #if PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER
    if (layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME)
    {
        /* Time in milliseconds after the response to the "Enter Direct Fast Flashing mode" command (8#-16#) in sent 
           and before data received is transferred to bootloader. In that time between, eventual data received is discarded. */
        if (timer_elapsed(&layer->mode_state_timer) > ((layer->dff_data_timeout) / 2))
        {
            layer->mode_state = WIFI_MODE_STATE_DFF_ACTIVE;
            timer_start(&layer->mode_state_timer);
        }
    }   /* layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME */

    /* Direct Fast Flashing processing: */
    if (layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE)
    {
        buffNo = layer->currentRxBuffer;
        if (layer->dff_buffer[buffNo] != NULL)
        {
            pktlen = layer->dff_buffer[buffNo]->nb_pktlen;
            if (pktlen > 0)
            {
                /* Enough data came, or timeout at the end of data: */
                if ((layer->dff_bytesToReceive == 0) ||
                    (pktlen >= (FW_UPDATE_BIGDATA_MAX_DATA_SIZE / WIFI_NUMBER_OF_BUFFERS)) ||
                    (timer_elapsed(&layer->mode_state_timer) > layer->dff_data_timeout))
                {
                    #if (DOUBLE_BUFFERING == 0)
                    timer_start(&layer->mode_state_timer);

                    /* Pass data to bootloader: */
                    pnb = layer->dff_buffer[buffNo];
                    ret = BOOTLOADER_RESULT_FAIL;
                    while ((pktlen > 0) && (pnb != NULL))
                    {
                        pdata = NBTOD(pnb, uint8_t*);
                        dataLen = pnb->nb_len;
                        ret = bootloader_dataInput_dff(pdata, dataLen);
                        if (ret != BOOTLOADER_RESULT_OK)
                        {
                            break;
                        }
                        pktlen -= dataLen;
                        pnb = pnb->nb_next;
                    }
                    /* Prepare for new data: */
                    netbuf_empty(layer->dff_buffer[buffNo]);

                    /* Disable DFF RX (LIN echo): */
                    layer->dff_rx_enabled = 0;
                    switch (ret)
                    {
                        case BOOTLOADER_RESULT_OK:
                        {
                            /* When data is written in flash, send Acknowledge Char: */
                            serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_ACK_CHAR);
                            break;
                        }
                        case BOOTLOADER_RESULT_DATA_SIZE_TOO_BIG:
                        {
                            /* Too much data to write: */
                            serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_NACK_CHAR_DATA_SIZE);
                            break;
                        }
                        case BOOTLOADER_RESULT_INVALID_ADDRESS:
                        {
                            /* Invalid address: */
                            serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_NACK_CHAR_INVALID_ADDRESS);
                            break;
                        }
                        case BOOTLOADER_RESULT_FAIL:
                        {
                            /* Failed to write to flash: */
                            serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_NACK_CHAR_FAIL_WRITE);
                            break;
                        }
                        default:
                        {
                            /* Unhandled cases: */
                            serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_NACK_CHAR_FAIL_OTHER);
                            break;
                        }
                    }   /* --> switch (ret) */
                    /* Wait for 1 bit time or maximum 1000 us that character that was sent to FIFO 
                       is transferred to shift register (start transmitting): */
                    proc_delayus(min(1000, 1000000 / (layer->actualBaudrate)) / 20);
                    
                    /* Wait that character is sent out, but maximum 100 ms: */
                    timer_start(&guardTimer);
                    while ((serialport_TxComplete(layer->serialPort) == 0) &&
                           (timer_elapsed(&guardTimer) < 100))
                    {
                        proc_clearWDT();
                    }
                    /* Wait for 1 byte time or maximum 1000 us before enabling DFF RX: */
                    proc_delayus(min(1000, 1 * 10 * 1000000 / (layer->actualBaudrate)));
                    
                    /* Reenable DFF RX: */
                    layer->dff_rx_enabled = 1;

                    timer_start(&layer->mode_state_timer);

                    #else   /* --> DOUBLE_BUFFERING == 0 */

                    /* Writing of previous buffer finished: */
                    if (layer->currentRxBuffer == layer->currentWriteBuffer)
                    {
                        timer_start(&layer->mode_state_timer);

                        /* Prepare to receive new data in new buffer: */
                        layer->currentRxBuffer = (layer->currentRxBuffer + 1) % WIFI_NUMBER_OF_BUFFERS;
                        netbuf_empty(layer->dff_buffer[layer->currentRxBuffer]);

                        /* Wait for 3 bytes time or maximum 1000 us before sending acknowledgement: */
                        proc_delayus(min(1000, 3 * 10 * 1000000 / (layer->actualBaudrate)));

                        /* Disable DFF RX (LIN echo): */
                        layer->dff_rx_enabled = 0;
                        /* Inform that new data can be sent: */
                        serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_ACK_CHAR);

                        /* Wait for 1 bit time or maximum 1000 us that character that was sent to FIFO 
                           is transferred to shift register (start transmitting): */
                        proc_delayus(min(1000, 1000000 / (layer->actualBaudrate)) / 20);

                        /* Wait that character is sent out, but maximum 100 ms: */
                        timer_start(&guardTimer);
                        while ((serialport_TxComplete(layer->serialPort) == 0) &&
                               (timer_elapsed(&guardTimer) < 100))
                        {
                            proc_clearWDT();
                        }
                        /* Wait for 1 byte time or maximum 1000 us before enabling DFF RX: */
                        proc_delayus(min(1000, 1 * 10 * 1000000 / (layer->actualBaudrate)));
                        
                        /* Reenable DFF RX: */
                        layer->dff_rx_enabled = 1;

                        /* Pass data to bootloader: */
                        buffNo = layer->currentWriteBuffer;
                        pnb = layer->dff_buffer[buffNo];
                        while ((pktlen > 0) && (pnb != NULL))
                        {
                            pdata = NBTOD(pnb, uint8_t*);
                            dataLen = pnb->nb_len;
                            bootloader_dataInput_dff(pdata, dataLen);
                            pktlen -= dataLen;
                            pnb = pnb->nb_next;
                        }
                        /* Prepare for new data: */
                        netbuf_empty(layer->dff_buffer[buffNo]);

                        layer->currentWriteBuffer = (layer->currentWriteBuffer + 1) % WIFI_NUMBER_OF_BUFFERS;

                        timer_start(&layer->mode_state_timer);
                    }
                    #endif  /* --> DOUBLE_BUFFERING != 0 */
                } /* --> Enough data came, or timeout at the end of data  */
            }     /* --> pktlen > 0                                       */
        }         /* --> layer->dff_buffer[buffNo] != NULL                */
    }             /* --> layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE  */

    if (layer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST)
    {
        if (layer->tx_frame == NULL)
        {
            timer_start(&layer->mode_state_timer);
            layer->mode_state = WIFI_MODE_STATE_DFF_FORWARDING_ONLY;
        }
    }   /* --> (layer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY_REQUEST) */

    if (layer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY)
    {
        if (layer->dff_buffer[0] != NULL)
        {
            /* If anything to send: */
            if (layer->dff_buffer[0]->nb_pktlen > 0)
            {
                if (serialport_TxComplete(layer->serialPort) == 1)
                {
                    pdata = NBTOD(layer->dff_buffer[0], uint8_t*);
                    data = *pdata;

                    if (netbuf_remove(layer->dff_buffer[0], 1) == 1)
                    {
                        if (layer->dff_buffer[0]->nb_len == 0)
                        {
                            layer->dff_buffer[0] = netbuf_free(layer->dff_buffer[0]);
                        }

                        serialport_WriteTxData(layer->serialPort, data);
                    }
                }
            }   /* --> (layer->dff_buffer[0]->nb_pktlen > 0)                      */
        }       /* --> (layer->dff_buffer[0] != NULL)                             */
    }           /* --> (layer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY) */


    /* Check timeout for DFF mode of operation (Active and Forwarding mode): */
    if ((layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME) ||
        (layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE) ||
        (layer->mode_state == WIFI_MODE_STATE_DFF_FORWARDING_ONLY))
    {
        /* Timeout if no data came at all: */
        if (timer_elapsed(&layer->mode_state_timer) > layer->dff_initial_timeout + 10) /* 10 ms guard time to allow bootloader_dataInput_dff() happens first. */
        {
            if (layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE)
            {
                serialport_WriteTxData(layer->serialPort, BOOTLOADER_DFF_MODE_NACK_CHAR_TIMEOUT);
            }
            /* Return to normal mode after timeout (exit DFF): */
            wifi_dff_mode_set(layer, WIFI_DFF_MODE_DISABLE, 0, 0);
        }
    }   /* layer->mode_state == WIFI_MODE_STATE_DFF_ACTIVE_WAIT_GUARD_TIME, WIFI_MODE_STATE_DFF_ACTIVE or WIFI_MODE_STATE_DFF_FORWARDING_ONLY */
    #endif /* PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER */
}

/* [] END OF FILE */
