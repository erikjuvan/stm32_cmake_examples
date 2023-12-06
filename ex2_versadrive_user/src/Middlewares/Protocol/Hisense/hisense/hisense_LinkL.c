//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_LinkL.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/06/28
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Link Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include <string.h>
#include "cfg/platform_cfg.h"
#include "cfg/project_cfg.h"
#include "hisense_LinkL.h"
#include "Netbuf/netbuf.h"

#if HISENSE_PROTOCOL_USE

#include "Platform/serialport.h"
#include "Communication/wifi.h"
#include "hisense_Base.h"

/*
Private functions forward declarations:
*/
static uint8_t ValidateLinkLayerPacket(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet);
static uint8_t BuildLinkLayerResponseMessage(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet, hisense_LinkLayerPacket *response);
static uint8_t GenerateLinkLayerContent(HISENSE_LINKL_LAYER *layer, uint8_t *buffer, uint16_t bufferSize, uint16_t *bufferLength, hisense_LinkLayerPacket *packet);


/*
Private functions:
*/

void hisense_LinkL_RXinit(HISENSE_LINKL_LAYER *layer)
{
    layer->receivedByte = 0;
    layer->previousReceivedByte = 0;
}

void LinkLayerPacketClean(hisense_LinkLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->Header = 0;
    packet->ResponseID = 0;
    packet->Flag1 = 0;
    packet->FrameLength = 0;
    hisense_NetworkLayerPacketClean(&(packet->NetworkLayerContent));
    packet->Checksum = 0;
    packet->FrameTail = 0;
}

static void LinkLayerHeadTailClean(hisense_LinkLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    /* Head clean: */
    packet->Header = 0;
    packet->ResponseID = 0;
    packet->Flag1 = 0;
    packet->FrameLength = 0;

    hisense_NetworkLayerHeadClean(&(packet->NetworkLayerContent));

    /* Tail clean: */
    packet->Checksum = 0;
    packet->FrameTail = 0;
}

/*
Return values:
0 - Fail.          MCU should not send reply to the packet. "rx_frame" netbuffer must be released !!!
1 - Validation OK. MCU should reply to the packet. Reply is in "**tx_frame" netbuffer.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
Function also sets Application response ID and application Layer data position (index) and size inside the "rx_frame" received frame.
*/
static uint8_t ParseLinkLayerPacket(HISENSE_LINKL_LAYER *layer, 
                                    NETBUF* rx_frame, 
                                    NETBUF** tx_frame, 
                                    uint8_t* app_al_response_id,
                                    uint16_t* app_data_position, 
                                    uint16_t* app_data_size)
{
    uint16_t counter = 0;
    uint16_t currentArrayPointer = 0;
    uint16_t crcCalculationLength = 0;
    uint8_t crcLength = 1;
    uint8_t crcVersion = 0;
    uint16_t crcCalculationStartIndex = 0;
    uint16_t messageLengthWithoutFrameTailAndChecksum = 0;
    uint32_t crcCalculationResult = 0;
    uint16_t checksum2ByteResult = 0;
    uint8_t retValue;
    uint8_t data[RECEIVE_FRAME_BUFFER_SIZE];
    uint16_t dataLength, dataCopied;
    hisense_LinkLayerPacket requestStructure;
    hisense_LinkLayerPacket replyStructure;
    uint8_t OutputBuffer[SEND_FRAME_BUFFER_SIZE];
    uint16_t OutputBufferLength;

    if ((layer == NULL) ||
        (rx_frame == NULL) ||
        (tx_frame == NULL) ||
        (app_al_response_id == NULL) ||
        (app_data_position == NULL) ||
        (app_data_size == NULL))
    {
        return 0;
    }

    *app_al_response_id = 0;
    *app_data_position = 0;
    *app_data_size = 0;

    dataLength = rx_frame->nb_pktlen;
    dataLength = min(dataLength, sizeof(data));
    dataCopied = netbuf_copyfrom(rx_frame, 0, dataLength, data);
    if (dataCopied != dataLength)
    {
        return 0;
    }

    /* LinkLayerPacketClean(&requestStructure); */
    LinkLayerHeadTailClean(&requestStructure);

    layer->LLResponseOperationResult = 0;

    requestStructure.Header = data[currentArrayPointer++];
    requestStructure.Header = requestStructure.Header << 8;
    requestStructure.Header += data[currentArrayPointer++];
    crcCalculationStartIndex = currentArrayPointer;
    requestStructure.ResponseID = data[currentArrayPointer++];
    crcCalculationLength++;
    requestStructure.Flag1 = data[currentArrayPointer++];
    crcCalculationLength++;
    if ((requestStructure.Flag1 & 0x01) > 0)        /* 6.4 D0 bit (Flag1) tells if Flag2 is present */
    {
        return 0;
    }
    requestStructure.FrameLength = data[currentArrayPointer++];
    crcCalculationLength++;
    requestStructure.FrameLengthSize = 1;
    if ((requestStructure.Flag1 & 0x20) > 0)        /* 6.6 D5 bit (Flag1) specifies length is 1 or 2 byte */
    {
        requestStructure.FrameLength = requestStructure.FrameLength << 8;
        requestStructure.FrameLength += data[currentArrayPointer++];
        crcCalculationLength++;
        requestStructure.FrameLengthSize = 2;
    }

    /* Verifying maximum frame data length: */
    if (currentArrayPointer + requestStructure.FrameLength > sizeof(data) - 1)
    {
        return 0;
    }

    requestStructure.Checksum = data[currentArrayPointer + requestStructure.FrameLength];
    crcVersion = 1;
    if ((requestStructure.Flag1 & 0xC0) == 0x40)    /* 6.8 D7&D6 bit (Flag1) specifies Checksum is 2 byte  */
    {
        requestStructure.Checksum = requestStructure.Checksum << 8;
        requestStructure.Checksum += data[currentArrayPointer + requestStructure.FrameLength + 1];
        crcVersion = 2;
        crcLength = 2;
    }
    /* calculate checksum before further parsing */
    switch (crcVersion)
    {
        case 2:
        {
            messageLengthWithoutFrameTailAndChecksum = dataLength - 2 - 2;
            for (;counter < crcCalculationLength + requestStructure.FrameLength; counter++)
            {
                checksum2ByteResult += *(data + crcCalculationStartIndex + counter);
            }
            crcCalculationResult = checksum2ByteResult;
            break;
        }

        default:
            break;
    }
    if (crcCalculationResult == requestStructure.Checksum)
    {
        (layer->counters.crc_ok_cnt)++;
        retValue = hisense_NetworkLayerPacketParse(layer->networkLayer,
                                                   dataLength, 
                                                   data, 
                                                   &currentArrayPointer, 
                                                   &(requestStructure.NetworkLayerContent), 
                                                   requestStructure.FrameLengthSize, 
                                                   messageLengthWithoutFrameTailAndChecksum);
        if (retValue == 0)
        {
            return 0; /* No response! */
        }
        *app_al_response_id = requestStructure.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.ResponseID;
        *app_data_size = requestStructure.NetworkLayerContent.TransmissionLayerContent.ApplicationLayerContent.DataLength;
        *app_data_position = dataLength - 2 - 2 - (*app_data_size);
    }
    else
    {
        /* error...Checksum or CRC error... frame and data not valid */
        (layer->counters.crc_fail_cnt)++;
        return 0; /* No response! */
    }

    currentArrayPointer += crcLength;
    requestStructure.FrameTail += data[currentArrayPointer++];
    requestStructure.FrameTail = requestStructure.FrameTail << 8;
    requestStructure.FrameTail += data[currentArrayPointer];
    if (currentArrayPointer >= dataLength)
    {
        /* error...pointer runaway... frame and data not valid */
        return 0; /* fail */
    }

    retValue = ValidateLinkLayerPacket(layer,
                                       &requestStructure);
    if (retValue == 1)          /* MCU should reply to the packet */
    {
        if (BuildLinkLayerResponseMessage(layer,
                                          &requestStructure, 
                                          &replyStructure) != 1)
        {
            return 0;
        }
        OutputBufferLength = 0;
        if (GenerateLinkLayerContent(layer,
                                     OutputBuffer, 
                                     sizeof(OutputBuffer),
                                     &OutputBufferLength,
                                     &replyStructure) != 1)
        {
            return 0;
        }
        hisense_Base_HandleStuffedBytes(OutputBuffer, 
                                        &OutputBufferLength,
                                        SEND_FRAME_BUFFER_SIZE);

        *tx_frame = netbuf_gethdr();
        if (*tx_frame == NULL)
        {
            return 0;
        }
        if (netbuf_copyto(*tx_frame,
                          0,
                          (NETBUF_LEN) OutputBufferLength,
                          (void*) OutputBuffer) != (NETBUF_LEN) OutputBufferLength)
        {
            netbuf_freem(*tx_frame);
            return 0;
        }
        return 1; /* 1 - Validation OK. MCU should reply to the packet. Reply is in "**tx_frame" netbuffer. */
    }
    #if (FORWARDING_USE == 1)
    else if (retValue == 2)     /* MCU should forward the packet to downstream communication port(s) (Destination address mismatch). */
    {
        return 2; /* Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch). */
    }
    #endif  /* (FORWARDING_USE == 1) */
    #if (MASTER_FUNCTIONALITY_USE == 1)
    else if (retValue == 3)     /* This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called. */
    {
        return 3; /* Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called. */
    }
    #endif /* (MASTER_FUNCTIONALITY_USE == 1) */
    else
    {
        return 0;
    }
}

/*
Return values:
0 - Fail
1 - OK
*/
static uint8_t BuildLinkLayerResponseMessage(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet, hisense_LinkLayerPacket *response)
{
    if (layer == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }
    if (response == NULL)
    {
        return 0;
    }

    /* LinkLayerPacketClean(response); */

    response->ResponseID = layer->LLResponseOperationResult;
    response->Flag1 = packet->Flag1;    /* transfer sequence number from master */
    //response->Flag1 &= 0x7E;            /* Checksum is 2 bytes in length, NL length is calculated later, remove Flag2 */
    response->Header = (uint16_t)HISENSE_LL_FRAME_HEADER;
    response->FrameTail = HISENSE_LL_FRAME_END;

    return hisense_NetworkLayerResponseMessageBuild(layer->networkLayer,
                                                    &(packet->NetworkLayerContent), 
                                                    &(response->NetworkLayerContent));
}

/*
Return values:
0 - Fail.          MCU should not send reply to the packet.
1 - Validation OK. MCU should reply to the packet.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
*/
static uint8_t ValidateLinkLayerPacket(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet)
{
    uint8_t returnVal;

    if (packet == NULL)
    {
        return 0;
    }
    if ((packet->Header != HISENSE_LL_FRAME_HEADER) || 
        (packet->FrameTail != HISENSE_LL_FRAME_END))
    {
        return 0;
    }

    returnVal = 0;
    layer->LLResponseOperationResult = 1;

    returnVal = hisense_NetworkLayerPacketValidate(layer->networkLayer,
                                                   &(packet->NetworkLayerContent));
    if (returnVal == 2)
    {
        layer->LLResponseOperationResult = 0;   /* destination address mismatch */
        #if (FORWARDING_USE == 0)
        #if (REPLY_ADDRESSMISMATCH_MESSAGES == 1)
        returnVal = 1;                  /* build message anyway */
        #endif  /* (REPLY_ADDRESSMISMATCH_MESSAGES == 1) */
        #endif  /* (FORWARDING_USE == 0) */
    }
    else
    {
        if ((packet->Flag1 & 0x01) == 0x01)
        {
            layer->LLResponseOperationResult = 8;   /* Flag 2 must not be present */
            returnVal = 1;
        }
        #if 0
        else if ((packet->Flag1 & 0xC0) != 0x40)
        {
            layer->LLResponseOperationResult = 8;   /* Checksum is not supported (2byte checksum) */
            returnVal = 1;
        }
        #endif
    }
    return returnVal;
}

/*
Return values:
0 - Fail
1 - OK
*/
static uint8_t GenerateLinkLayerContent(HISENSE_LINKL_LAYER *layer, uint8_t *buffer, uint16_t bufferSize, uint16_t *bufferLength, hisense_LinkLayerPacket *packet)
{
    uint16_t counter = 0;
    uint16_t expectedNetworkLayerLength = 0;
    uint16_t calculatedLinkLayerLength;
    uint8_t returnVal = 0;
    /* uint8_t crcLength = 1;       Removed due to "set but not used" warning */
    uint8_t crcVersion = 0;
    uint16_t checksum2ByteResult = 0;

    if (layer == NULL)
    {
        return 0;
    }
    if (buffer == NULL)
    {
        return 0;
    }
    if (bufferLength == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }

    *(buffer + *bufferLength) = (uint8_t)(packet->Header >> 8);
    (*bufferLength)++;
    *(buffer + *bufferLength) = (uint8_t)packet->Header;
    (*bufferLength)++;
    counter = *bufferLength;
    *(buffer + *bufferLength) = packet->ResponseID;
    (*bufferLength)++;
    expectedNetworkLayerLength = hisense_NetworkLayerLengthCalculate(layer->networkLayer,
                                                                     &packet->NetworkLayerContent);
    if (expectedNetworkLayerLength <= 0x00FF)
    {
        packet->Flag1 &= 0xDF;          /* clear D5, length is 1 byte */
    }
    else
    {
        packet->Flag1 |= 0x20;          /* clear D5, length is 2 byte */
    }
    *(buffer + *bufferLength) = packet->Flag1;
    (*bufferLength)++;
    if (expectedNetworkLayerLength <= 0x00FF)
    {
        *(buffer + *bufferLength) = (uint8_t)expectedNetworkLayerLength;
        (*bufferLength)++;
    }
    else
    {
        *(buffer + *bufferLength) = (uint8_t)(expectedNetworkLayerLength >> 8);
        (*bufferLength)++;
        *(buffer + *bufferLength) = (uint8_t)expectedNetworkLayerLength;
        (*bufferLength)++;
    }

    /* Check if buffer size in long enough: */
    calculatedLinkLayerLength = (*bufferLength) + expectedNetworkLayerLength + 4 + 2;   /* 4...max CRC length; 2...tail size */
    if (calculatedLinkLayerLength > bufferSize)
    {
        return 0;
    }

    returnVal = hisense_NetworkLayerContentGenerate(buffer, bufferLength, &packet->NetworkLayerContent);

    /* Adding Checksum or CRC: */
    crcVersion = 1;
    if ((packet->Flag1 & 0xC0) == 0x40)     /* 6.8 D7&D6 bit (Flag1) specifies Checksum is 2 byte  */
    {
        crcVersion = 2;
        /* crcLength = 2;   Removed due to "set but not used" warning */
    }
    switch (crcVersion)
    {
        case 2:
        {
            for (;counter < *bufferLength; counter++)
            {
                checksum2ByteResult += *(buffer + counter);
            }
            *(buffer + *bufferLength) = (uint8_t)(checksum2ByteResult >> 8);
            (*bufferLength)++;
            *(buffer + *bufferLength) = (uint8_t)checksum2ByteResult;
            (*bufferLength)++;
            break;
        }

        default:
            break;
    }

    /* Adding frame tail: */
    *(buffer + *bufferLength) = (uint8_t)(packet->FrameTail >> 8);
    (*bufferLength)++;
    *(buffer + *bufferLength) = (uint8_t)packet->FrameTail;
    (*bufferLength)++;

    /* Calculated buffer length should be less than actual buffer length. 
       Otherwise possibility of buffer overflow occured !!! */
    if ((*bufferLength) > calculatedLinkLayerLength)
    {
        return 0;
    }

    return returnVal;
}




/* 
Public API functions: 
*/

void hisense_LinkL_init(HISENSE_LINKL_LAYER *layer)
{
    hisense_LinkL_RXinit(layer);

    layer->LLResponseOperationResult = 0;
    memset(&layer->counters, 0x00, sizeof(layer->counters));

    hisense_NetworkL_init(layer->networkLayer);
}

uint8_t hisense_LinkL_MessagePreparedForTransmission(HISENSE_LINKL_LAYER *layer)
{
    /* Message can be sent out by wifi layer, if also application layer is set so (processing on application layer can be longer). */
    if (hisense_ApplicationL_IsCommandProcessingFinshed(layer->networkLayer->transmissionLayer->applicationLayer) == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint32_t hisense_LinkL_counter_get(HISENSE_LINKL_LAYER *layer, HISENSE_LINKL_LAYER_COUNTER counter)
{
    uint32_t cnt;

    if (layer == NULL)
    {
        return 0;
    }
    if (counter >= HISENSE_LINKL_LAYER_COUNTER_MAX)
    {
        return 0;
    }

    cnt = 0;
    switch (counter)
    {
        case HISENSE_LINKL_LAYER_COUNTER_CRCOK:
        {
            cnt = layer->counters.crc_ok_cnt;
            break;
        }

        case HISENSE_LINKL_LAYER_COUNTER_CRCFAIL:
        {
            cnt = layer->counters.crc_fail_cnt;
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

/*
Returned values:
0 - Data successfully added to buffer, but Hisense frame is not complete yet.
1 - Hisense link layer frame is complete. The frame can be parsed in upper Hisense protocol stack layers.
2 - Data cannot be added to buffer, or too many data came. Frame should be discarded.
3 - Invalid data.
4 - Invalid parameters.
*/
uint8_t hisense_LinkL_ReceiveByte_flash(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value)
{
    if ((layer == NULL) || (pnb == NULL))
    {
        return 4;
    }

    layer->previousReceivedByte = layer->receivedByte;
    layer->receivedByte = value;

    /* Stuffed byte (0xF4) detection: */
    if ((layer->previousReceivedByte == HISENSE_LL_STUFFING_BYTE) && 
        (layer->receivedByte         == HISENSE_LL_STUFFING_BYTE))
    {
        layer->receivedByte = 0; /* If next 0xF4 pair comes in this enables read! */
        return 0;
    }

    /* First byte should be 0xF4: */
    if ((pnb->nb_pktlen == 0) && (layer->receivedByte != HISENSE_LL_FRAME_HEADER_1))
    {
        hisense_LinkL_RXinit(layer);
        return 3;
    }
    /* Second byte should be 0xF5: */
    if ((pnb->nb_pktlen == 1) && (layer->receivedByte != HISENSE_LL_FRAME_HEADER_2))
    {
        hisense_LinkL_RXinit(layer);
        return 3;
    }

    /* Adding received byte to net buffer: */
    if (netbuf_append(pnb, 1, (VOID*)&(layer->receivedByte)) != 1)
    {
        hisense_LinkL_RXinit(layer);    /* Cannot add to net buffer */
        return 2;                       /* Error - discard frame */
    }

    /* Buffer overflow detection: */
    if (pnb->nb_pktlen > RECEIVE_FRAME_BUFFER_SIZE) 
    {
        hisense_LinkL_RXinit(layer);
        return 2;                       /* Error - discard frame */
    }

    /* START of new frame detection: */
    if ((layer->previousReceivedByte == HISENSE_LL_FRAME_HEADER_1) && 
        (layer->receivedByte         == HISENSE_LL_FRAME_HEADER_2))
    {
        #if (RX_SYNCHRONIZATION_USE == 1)
        wifi_rx_synchronization_callback((WIFI_LAYER*) layer->wifiLayer);
        #endif /* (RX_SYNCHRONIZATION_USE == 1) */

        netbuf_empty(pnb);
        if (netbuf_append(pnb, 1, (VOID*)&(layer->previousReceivedByte)) != 1)
        {
            hisense_LinkL_RXinit(layer);    /* Cannot add to net buffer */
            return 2;                       /* Error - discard frame */
        }
        if (netbuf_append(pnb, 1, (VOID*)&(layer->receivedByte)) != 1)
        {
            hisense_LinkL_RXinit(layer);    /* Cannot add to net buffer */
            return 2;                       /* Error - discard frame */
        }
    }

    /* END of frame detection: */
    if ((layer->previousReceivedByte == HISENSE_LL_FRAME_END_1) && 
        (layer->receivedByte         == HISENSE_LL_FRAME_END_2))
    {
        timer_start(&layer->replyTimer);
        return 1;                           /* Parse data and start new frame */
    }   /* else no action required! */

    return 0; /* Link Layer frame is not complete yet */
}

#if (   (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)               \
     || (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)               \
     || (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)          \
     || (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)           \
     || (PLATFORM_USED == PLATFORM_NXP_LPC546XX)                \
 /*  ||  (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX) */    \
     || (PLATFORM_USED == PLATFORM_STM_STM32G0xx)               \
     || (PLATFORM_USED == PLATFORM_STM32_STM32GXX)              \
     || (PLATFORM_USED == PLATFORM_STM_STM32F4xx))
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value)
{
    /* Call the "return hisense_LinkL_ReceiveByte_flash()" function which is located in Flash.
       This is just warning suppresion reported by IAR compiler on MB9AF114MA, MB9BF121J and LPC546XX platforms.
       Prevention that this function is not called when flash is writing is done on different way: flash_is_writing().
   */
    /* return hisense_LinkL_ReceiveByte_flash(layer, pnb, value);  ---> */
    __asm volatile("    PUSH    {R3, LR}\n");
    __asm volatile("    BL      hisense_LinkL_ReceiveByte_flash\n");
    __asm volatile("    POP     {R3, PC}\n");

    return 0;   /* Actually not reached, because POP command will return. This is just warning suppression. */
}
#elif (   (PLATFORM_USED == PLATFORM_STM8_STM8S207)             \
       || (PLATFORM_USED == PLATFORM_STM_STM32G0xx)             \
       || (PLATFORM_USED == PLATFORM_STM32_STM32GXX)            \
      )
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value)
{
    return hisense_LinkL_ReceiveByte_flash(layer, pnb, value);
}
#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value)
{
    #warning hisense_LinkL.c: implement hisense_LinkL_ReceiveByte() function for PLATFORM_CYPRESS_PSOC4100SMAX platform!
    return hisense_LinkL_ReceiveByte_flash(layer, pnb, value);
}
#elif (PLATFORM_USED == PLATFORM_template)
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value)
{
    #error hisense_LinkL.c: implement hisense_LinkL_ReceiveByte() function for new platform!
}
#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error hisense_LinkL.c: Undefined or unsupported platform
#endif


/*
Return values:
0 - Fail.          MCU should not send reply to the packet. "rx_frame" netbuffer must be released !!!
1 - Validation OK. MCU should reply to the packet. Reply is in "**tx_frame" netbuffer.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
Function also sets Application response ID and application Layer data position (index) and size inside the "rx_frame" received frame.
*/
uint8_t hisense_LinkL_RxFrameProcess(HISENSE_LINKL_LAYER *layer, NETBUF* rx_frame, NETBUF** tx_frame, uint8_t* app_al_response_id, uint16_t* app_data_position, uint16_t* app_data_size)
{
    uint8_t retValue;

    if (layer == NULL)
    {
        return 0;
    }
    if (rx_frame == NULL)
    {
        return 0;
    }
    if (tx_frame == NULL)
    {
        return 0;
    }
    if (app_al_response_id == NULL)
    {
        return 0;
    }
    if (app_data_position == NULL)
    {
        return 0;
    }
    if (app_data_size == NULL)
    {
        return 0;
    }

    *app_al_response_id = 0;
    *app_data_position = 0;
    *app_data_size = 0;

    retValue = ParseLinkLayerPacket(layer,
                                    rx_frame,
                                    tx_frame,
                                    app_al_response_id,
                                    app_data_position,
                                    app_data_size);
    switch (retValue)
    {
        case 0:
        {
            return 0;
            break;
        }
        case 1:
        {
            return 1;
            break;
        }
        case 2:
        {
            return 2;
            break;
        }
        case 3:
        {
            return 3;
            break;
        }
        default:
        {
            return 0;
            break;
        }
    }   /* --> switch (retValue) */
}

#if (MASTER_FUNCTIONALITY_USE == 1)
/* Return values:
0 - Fail. Master request is not generated.
1 - OK.   Master request is generated and is in the "**tx_frame" netbuffer.
*/
uint8_t hisense_LinkL_BuildLinkLayerMasterRequestMessage(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet, NETBUF** tx_frame)
{
    hisense_LinkLayerPacket request;
    uint8_t OutputBuffer[SEND_FRAME_BUFFER_SIZE];
    uint16_t OutputBufferLength;

    if (layer == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }
    if (tx_frame == NULL)
    {
        return 0;
    }

    /* LinkLayerPacketClean(&request); */
    LinkLayerHeadTailClean(&request);

    request.ResponseID = 0x00;
    request.Flag1 = 0x40;
    request.Header = (uint16_t)HISENSE_LL_FRAME_HEADER;
    request.FrameTail = HISENSE_LL_FRAME_END;

    if (hisense_NetworkLayerMasterRequestMessageBuild(layer->networkLayer,
                                                      &(packet->NetworkLayerContent), 
                                                      &(request.NetworkLayerContent)) != 1)
    {
        return 0;
    }

    OutputBufferLength = 0;
    if (GenerateLinkLayerContent(layer,
                                 OutputBuffer, 
                                 sizeof(OutputBuffer),
                                 &OutputBufferLength,
                                 &request) != 1)
    {
        return 0;
    }

    hisense_Base_HandleStuffedBytes(OutputBuffer, 
                                    &OutputBufferLength,
                                    SEND_FRAME_BUFFER_SIZE);

    *tx_frame = netbuf_gethdr();
    if (*tx_frame == NULL)
    {
        return 0;
    }

    if (netbuf_copyto(*tx_frame,
                      0,
                      (NETBUF_LEN) OutputBufferLength,
                      (void*) OutputBuffer) != (NETBUF_LEN) OutputBufferLength)
    {
        netbuf_freem(*tx_frame);
        return 0;
    }
    return 1; /* Master Request is in "**tx_frame" netbuffer. */
}
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

void hisense_LinkL_run(HISENSE_LINKL_LAYER *layer)
{
    /* Check timeouts */
    if (hisense_ApplicationL_IsCommandProcessingFinshed(
            layer->networkLayer->transmissionLayer->applicationLayer) != 1)
    {
        if (timer_elapsed(&layer->replyTimer) > HISENSE_LINKL_REPLY_TIMEOUT * HISENSE_LINKL_NUMBER_OF_RETRIES_EXTENDED)
        {
            timer_stop(&layer->replyTimer);
            hisense_ApplicationL_CommandProcessingFinshed(layer->networkLayer->transmissionLayer->applicationLayer);
        }
    }
}


#else /* HISENSE_PROTOCOL_USE */

void hisense_LinkL_init(HISENSE_LINKL_LAYER *layer){};
uint8_t hisense_LinkL_MessagePreparedForTransmission(HISENSE_LINKL_LAYER *layer){return 0;}

uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value){}
uint8_t hisense_LinkL_RxFrameProcess(HISENSE_LINKL_LAYER *layer, NETBUF* rx_frame, NETBUF** tx_frame, uint8_t* app_al_response_id, uint16_t* app_data_position, uint16_t* app_data_size){}
void hisense_LinkL_run(HISENSE_LINKL_LAYER *layer){}

#endif /* HISENSE_PROTOCOL_USE */

/* [] END OF FILE */
