//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_ApplicationL.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2021/11/25
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Application Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "project_cfg.h"
#include "hisense_ApplicationL.h"

#if HISENSE_PROTOCOL_USE

#include "hisense_Base.h"
#include "hisense_Mapping.h"

const uint8_t ExtendedHeaderMessageTypes[] = { 1, 14, 201 };
#define EXTENDED_HEADER_MESSAGE_TYPES_LENGTH    (sizeof(ExtendedHeaderMessageTypes) / sizeof(uint8_t))

/*
Public API functions:
*/

void hisense_ApplicationL_init(HISENSE_APPLICATIONL_LAYER* layer)
{
    layer->ALResponseOperationResult = AL_RESPONSE_ID_FAILED;
    layer->RequestALDataFrame = 0;
    layer->RequestALDataFrameNo = 0;
    layer->ResponseALDataFrame = 0;
    layer->ResponseALDataFrameNo = 0;

    netbuf_freem(layer->ResponseALDataBuffer);      /* Free, if left from previous usage */
    layer->ResponseALDataBuffer = NULL;

    #if (MASTER_FUNCTIONALITY_USE == 1)
    layer->MasterRequest_active = 0;
    #endif /* (MASTER_FUNCTIONALITY_USE == 1) */
}

void hisense_ApplicationLayerPacketClean(hisense_ApplicationLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->MessageType = 0;
    packet->MessageSubType = 0;
    packet->ResponseID = 0;
    packet->Frame = 0;
    packet->FrameNo = 0;
    packet->parent = NULL;
    hisense_Base_ClearLocalBuffer(packet->Data, MAX_DATA_MESSAGE_LENGTH);
    packet->DataLength = 0;
}

void hisense_ApplicationLayerHeadClean(hisense_ApplicationLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->MessageType = 0;
    packet->MessageSubType = 0;
    packet->ResponseID = 0;
    packet->Frame = 0;
    packet->FrameNo = 0;
    packet->parent = NULL;

    packet->DataLength = 0;
}

/* 
Return values: 
0  - No response!,
1  - OK 
*/
uint8_t hisense_ApplicationLayerPacketParse(HISENSE_APPLICATIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_ApplicationLayerPacket *destination, uint16_t messageLengthWithoutFrameTailAndChecksum)
{
    uint16_t counter = 0;

    if (layer == NULL)
    {
        return 0;
    }
    if (destination == NULL)
    {
        return 0;
    }
    if (data == NULL)
    {
        return 0;
    }
    if (dataLength == 0)
    {
        return 0;
    }
    if ((*currentArrayPointer) >= dataLength)
    {
        return 0;
    }

    /* hisense_ApplicationLayerPacketClean(destination); */

    layer->ALResponseOperationResult = AL_RESPONSE_ID_FAILED;
    layer->RequestALDataFrame = 0;
    layer->RequestALDataFrameNo = 0;

    destination->MessageType = data[(*currentArrayPointer)++];
    destination->MessageSubType = data[(*currentArrayPointer)++];
    destination->ResponseID = data[(*currentArrayPointer)++];
    for (counter = 0; counter < EXTENDED_HEADER_MESSAGE_TYPES_LENGTH; counter++)
    {
        if (ExtendedHeaderMessageTypes[counter] == destination->MessageType)
        {
            destination->Frame = data[(*currentArrayPointer)++];
            destination->FrameNo = data[(*currentArrayPointer)++];
            destination->FrameNo <<= 8;
            destination->FrameNo += data[(*currentArrayPointer)++];
            layer->RequestALDataFrame = destination->Frame;
            layer->RequestALDataFrameNo = destination->FrameNo;
            layer->ResponseALDataFrameNo = layer->RequestALDataFrameNo;
            break;
        }
    }

    for (counter = 0; counter < MAX_DATA_MESSAGE_LENGTH && (*currentArrayPointer) < messageLengthWithoutFrameTailAndChecksum; counter++)
    {
        destination->Data[counter] = data[(*currentArrayPointer)++];
    }
    destination->DataLength = counter;

    return 1;
}

/*
Return values:
0 - Fail.          MCU should not send reply to the packet.
1 - Validation OK. MCU should reply to the packet.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
*/
uint8_t hisense_ApplicationLayerPacketValidate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet)
{
    hisense_Mapping_MessageTypeList* MessageTypeList;
    uint8_t counter = 0;
    uint8_t numberOfSupportedMessages;

    if (layer == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }

    MessageTypeList = hisense_Mapping_MessageTypeList_get();
    layer->ALResponseOperationResult = AL_RESPONSE_ID_NOT_SUPPORTED;
    layer->ResponseALDataFrame = 0;
    //layer->ResponseALDataFrameNo = 0;

    netbuf_freem(layer->ResponseALDataBuffer);      /* Free, if left from previous usage */
    layer->ResponseALDataBuffer = NULL;

    numberOfSupportedMessages = hisense_Mapping_NumberOfSupportedMessages_get();
    for (;counter < numberOfSupportedMessages; counter++)
    {
        if (packet->MessageType == MessageTypeList[counter].MessageType && 
            packet->MessageSubType == MessageTypeList[counter].MessageSubType &&
            packet->ResponseID == MessageTypeList[counter].RequestRequiredResponseID)
        {
            if (MessageTypeList[counter].VerifyFunction == NULL)
            {
                /* Nothing to verify */
                layer->ALResponseOperationResult = AL_RESPONSE_ID_SUCCESSFUL;
            }
            else
            {
                MessageTypeList[counter].VerifyFunction(layer, packet->DataLength, packet->Data, (void*)packet);
            }
            if (layer->ALResponseOperationResult != AL_RESPONSE_ID_FAILED)
            {
                if (MessageTypeList[counter].ResponseFunction != NULL)
                {
                    MessageTypeList[counter].ResponseFunction(layer);
                }
            }
            return 1; /* Always OK since we came this far, layer->ALResponseOperationResult defines actual errors if any. */
        }
    }

    #if (MASTER_FUNCTIONALITY_USE == 1)
    /* Check if reply matches Master request Message Type and Message Subtype: */
    if (layer->MasterRequest_active == 1)
    {
        if ((packet->MessageType == layer->MasterRequest_MessageType) && 
            (packet->MessageSubType == layer->MasterRequest_MessageSubtype))
        {
            return 3; /* This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called. */
        }
    }
    #endif /* (MASTER_FUNCTIONALITY_USE == 1) */

    return 0;
}

/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_ApplicationLayerResponseMessageBuild(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet, hisense_ApplicationLayerPacket *response)
{
    uint16_t datalen, dataCopied;
    uint8_t counter;

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

    /* hisense_ApplicationLayerPacketClean(response); */
    hisense_ApplicationLayerHeadClean(response);

    response->MessageType = packet->MessageType;
    response->MessageSubType = packet->MessageSubType;
    response->ResponseID = layer->ALResponseOperationResult;
    for (counter = 0; counter < EXTENDED_HEADER_MESSAGE_TYPES_LENGTH; counter++)
    {
        if (ExtendedHeaderMessageTypes[counter] == packet->MessageType)
        {
            response->Frame = layer->ResponseALDataFrame;
            response->FrameNo = layer->ResponseALDataFrameNo;
            break;
        }
    }
    
    if (layer->ResponseALDataBuffer != NULL)
    {
        datalen = layer->ResponseALDataBuffer->nb_pktlen;
        if (datalen > 0)
        {
            datalen = min(datalen, sizeof(response->Data));
            response->DataLength = datalen;
            dataCopied = netbuf_copyfrom(layer->ResponseALDataBuffer, 0, datalen, (void*) response->Data);
            if (dataCopied != datalen)
            {
                return 0;
            }
            layer->ResponseALDataBufferLength = datalen;
            netbuf_freem(layer->ResponseALDataBuffer);
            layer->ResponseALDataBuffer = NULL;
        }
        else
        {
            layer->ResponseALDataBufferLength = 0;
        }
    }
    else
    {
        layer->ResponseALDataBufferLength = 0;
    }

    response->master_request = 0;
    return 1;
}

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_ApplicationLayerMasterRequestMessageBuild(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet, hisense_ApplicationLayerPacket *request)
{
    if ((layer == NULL) || (packet == NULL) || (request == NULL))
    {
        return 0;
    }

    if (packet->DataLength > sizeof(request->Data))
    {
        return 0;
    }

    /* hisense_ApplicationLayerPacketClean(request); */
    hisense_ApplicationLayerHeadClean(request);

    request->MessageType = packet->MessageType;
    request->MessageSubType = packet->MessageSubType;
    request->ResponseID = 0x00;
    request->Frame = 0x00;
    request->FrameNo = 0x00;

    if (packet->DataLength > 0)
    {
        request->DataLength = packet->DataLength;
        hisense_Base_CopyData(packet->pData, 
                              request->Data, 
                              packet->DataLength);
    }
    request->master_request = 1;
    return 1;
}

void hisense_ApplicationLayerMasterRequest_SetMessageType(HISENSE_APPLICATIONL_LAYER *layer, uint8_t active, uint8_t MessageType, uint8_t MessageSubtype)
{
    layer->MasterRequest_active = active;
    layer->MasterRequest_MessageType = MessageType;
    layer->MasterRequest_MessageSubtype = MessageSubtype;
}
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

uint16_t hisense_ApplicationLayerLengthCalculate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet)
{
    uint16_t length = 0;
    uint8_t counter = 0;

    if (layer == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }

    length++;               /* MessageType     */
    length++;               /* MessageSubType  */
    length++;               /* ResponseID      */
    for (counter = 0; counter < EXTENDED_HEADER_MESSAGE_TYPES_LENGTH; counter++)
    {
        if (ExtendedHeaderMessageTypes[counter] == packet->MessageType)
        {
            length++;       /* Frame   */
            length += 2;    /* FrameNo */
            break;
        }
    }

    if (packet->master_request == 0)
    {
        if (layer->ResponseALDataBufferLength > 0)
        {
            length += layer->ResponseALDataBufferLength;
        }
    }
    else
    {
        if (packet->DataLength > 0)
        {
            length += packet->DataLength;
        }
    }

    return length;
}

/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_ApplicationLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_ApplicationLayerPacket *packet)
{
    uint16_t counter = 0;

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

    *(buffer + *bufferLength) = packet->MessageType;
    (*bufferLength)++;
    *(buffer + *bufferLength) = packet->MessageSubType;
    (*bufferLength)++;
    *(buffer + *bufferLength) = packet->ResponseID;
    (*bufferLength)++;
    for (counter = 0; counter < EXTENDED_HEADER_MESSAGE_TYPES_LENGTH; counter++)
    {
        if (ExtendedHeaderMessageTypes[counter] == packet->MessageType)
        {
            *(buffer + *bufferLength) = packet->Frame;
            (*bufferLength)++;
            *(buffer + *bufferLength) = (uint8_t)(packet->FrameNo >> 8);
            (*bufferLength)++;
            *(buffer + *bufferLength) = (uint8_t)packet->FrameNo;
            (*bufferLength)++;
            break;
        }
    }
    for (counter = 0; counter < packet->DataLength;counter++)
    {
        *(buffer + *bufferLength) = packet->Data[counter];
        (*bufferLength)++;
    }

    return 1;
}

uint8_t hisense_ApplicationL_IsCommandProcessingFinshed(HISENSE_APPLICATIONL_LAYER *layer)
{
    if (layer->WaitProcessing == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



uint8_t hisense_ApplicationL_RequestALDataFrame_get(HISENSE_APPLICATIONL_LAYER *layer)
{
    return (layer->RequestALDataFrame);
}

uint16_t hisense_ApplicationL_RequestALDataFrameNo_get(HISENSE_APPLICATIONL_LAYER *layer)
{
    return (layer->RequestALDataFrameNo);
}

uint8_t hisense_ApplicationL_SelectCurrentResponseBuffer(HISENSE_APPLICATIONL_LAYER *layer, uint8_t *currentBuffer, uint16_t length)
{
    netbuf_freem(layer->ResponseALDataBuffer);      /* Free, if left from previous usage */

    layer->ResponseALDataBuffer = netbuf_gethdr();
    if (layer->ResponseALDataBuffer == NULL)
    {
        return 0;
    }

    if (netbuf_append(layer->ResponseALDataBuffer, length, (void*) currentBuffer) != length)
    {
        netbuf_freem(layer->ResponseALDataBuffer);
        layer->ResponseALDataBuffer = NULL;
        return 0;
    }

    return 1;
}

void hisense_ApplicationL_SelectCurrentResponseFrameAndFrameNo(HISENSE_APPLICATIONL_LAYER *layer, uint8_t frame, uint16_t frameNo)
{
    layer->ResponseALDataFrame = frame;
    layer->ResponseALDataFrameNo = frameNo;
}

void hisense_ApplicationL_ALResponseOperationResult_set(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response)
{
    layer->ALResponseOperationResult = response;
    layer->WaitProcessing = 0;
}

void hisense_ApplicationL_ALResponseOperationResult_prepare(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response)
{
    layer->ALResponseOperationResult = response;
    layer->WaitProcessing = 1;
}

void hisense_ApplicationL_CommandProcessingFinshed(HISENSE_APPLICATIONL_LAYER *layer)
{
    layer->WaitProcessing = 0;
}

#else /* HISENSE_PROTOCOL_USE */

void hisense_ApplicationL_init(HISENSE_APPLICATIONL_LAYER* layer){}
void hisense_ApplicationLayerPacketClean(hisense_ApplicationLayerPacket *packet){}
void hisense_ApplicationLayerHeadClean(hisense_ApplicationLayerPacket *packet){}
uint8_t hisense_ApplicationLayerPacketParse(HISENSE_APPLICATIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_ApplicationLayerPacket *destination, uint16_t messageLengthWithoutFrameTailAndChecksum){return 0;}
uint8_t hisense_ApplicationLayerPacketValidate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet){return 0;}
uint8_t hisense_ApplicationLayerResponseMessageBuild(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet, hisense_ApplicationLayerPacket *response){return 0;}
uint16_t hisense_ApplicationLayerLengthCalculate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet){return 0;}
uint8_t hisense_ApplicationLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_ApplicationLayerPacket *packet){return 0;}
uint8_t hisense_ApplicationL_IsCommandProcessingFinshed(HISENSE_APPLICATIONL_LAYER *layer){return 0;}
uint8_t hisense_ApplicationL_RequestALDataFrame_get(HISENSE_APPLICATIONL_LAYER *layer){return 0;}
uint16_t hisense_ApplicationL_RequestALDataFrameNo_get(HISENSE_APPLICATIONL_LAYER *layer){return 0;}
uint8_t hisense_ApplicationL_SelectCurrentResponseBuffer(HISENSE_APPLICATIONL_LAYER *layer, uint8_t *currentBuffer, uint16_t length){}
void hisense_ApplicationL_SelectCurrentResponseFrameAndFrameNo(HISENSE_APPLICATIONL_LAYER *layer, uint8_t frame, uint16_t frameNo){}
void hisense_ApplicationL_ALResponseOperationResult_set(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response){}
void hisense_ApplicationL_ALResponseOperationResult_prepare(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response){}
void hisense_ApplicationL_ALResponseOperationResult_prepare(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response){}

#endif /* HISENSE_PROTOCOL_USE */

/* [] END OF FILE */
