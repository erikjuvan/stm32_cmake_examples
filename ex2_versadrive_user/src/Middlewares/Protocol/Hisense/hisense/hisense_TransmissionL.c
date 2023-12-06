//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_TransmissionL.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2021/11/25
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Transmission Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "cfg/project_cfg.h"
#include "hisense_TransmissionL.h"

#if HISENSE_PROTOCOL_USE

#include "hisense_Base.h"
#include "hisense_ApplicationL.h"


/*
Public API functions
*/

void hisense_TransmissionL_init(HISENSE_TRANSMISSIONL_LAYER* layer)
{
    layer->TLResponseOperationResult = 0;

    hisense_ApplicationL_init(layer->applicationLayer);
}

void hisense_TransmissionLayerPacketClean(hisense_TransmissionLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->ResponseID = 0;
    packet->Flag1 = 0;
    packet->parent = NULL;
    hisense_ApplicationLayerPacketClean(&(packet->ApplicationLayerContent));
}

void hisense_TransmissionLayerHeadClean(hisense_TransmissionLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->ResponseID = 0;
    packet->Flag1 = 0;
    packet->parent = NULL;

    hisense_ApplicationLayerHeadClean(&(packet->ApplicationLayerContent));
}

/* 
Return values: 
0  - No response!
1  - OK 
*/
uint8_t hisense_TransmissionLayerPacketParse(HISENSE_TRANSMISSIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_TransmissionLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum)
{
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

    /* hisense_TransmissionLayerPacketClean(destination); */

    layer->TLResponseOperationResult = 0;
    (destination->ApplicationLayerContent).parent = destination;

    destination->ResponseID = data[(*currentArrayPointer)++];
    destination->Flag1 = data[(*currentArrayPointer)++];
    if ((destination->Flag1 & 0x01) > 0)        /* 8.3 D0 bit (Flag1) tells if Flag2 is present     */
    {
        return 0;   // No response!
    }

    return hisense_ApplicationLayerPacketParse(layer->applicationLayer,
                                               dataLength, 
                                               data, 
                                               currentArrayPointer, 
                                               &(destination->ApplicationLayerContent), 
                                               messageLengthWithoutFrameTailAndChecksum);
}

/*
Return values:
0 - Fail.          MCU should not send reply to the packet.
1 - Validation OK. MCU should reply to the packet.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
*/
uint8_t hisense_TransmissionLayerPacketValidate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet)
{
    uint8_t returnVal;

    if (layer == NULL)
    {
        return 0;
    }

    if (packet == NULL)
    {
        return 0;
    }

    returnVal = 0;
    layer->TLResponseOperationResult = 1;
    if ((packet->Flag1 & 0x01) == 0x01)
    {
        layer->TLResponseOperationResult = 8;   /* Flag 2 must not be present                                           */
        returnVal = 1;
    }
    if (packet->ResponseID != 0 && (packet->Flag1 & 0xFE) != 0)
    {
        layer->TLResponseOperationResult = 8;   /* Encription specified on master side is not supported on slave        */
        returnVal = 1;
    }

    returnVal = hisense_ApplicationLayerPacketValidate(layer->applicationLayer,
                                                       &(packet->ApplicationLayerContent));

    return returnVal;
}

/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_TransmissionLayerResponseMessageBuild(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet, hisense_TransmissionLayerPacket *response)
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

    /* hisense_TransmissionLayerPacketClean(response); */
    
    response->ResponseID = layer->TLResponseOperationResult;    /* No encription on high half byte and response result on lower half */
    response->Flag1 = 0;    /* No encryption and no flag 2 */

    return hisense_ApplicationLayerResponseMessageBuild(layer->applicationLayer,
                                                        &(packet->ApplicationLayerContent), 
                                                        &(response->ApplicationLayerContent));
}

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_TransmissionLayerMasterRequestMessageBuild(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet, hisense_TransmissionLayerPacket *request)
{
    if ((layer == NULL) || (packet == NULL) || (request == NULL))
    {
        return 0;
    }

    /* hisense_TransmissionLayerPacketClean(request); */
    hisense_TransmissionLayerHeadClean(request);

    request->ResponseID = 0x00;
    request->Flag1 = 0;    /* No encryption and no flag 2 */

    if (hisense_ApplicationLayerMasterRequestMessageBuild(layer->applicationLayer,
                                                          &(packet->ApplicationLayerContent), 
                                                          &(request->ApplicationLayerContent)) != 1)
    {
        return 0;
    }

    return 1;
}
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

uint16_t hisense_TransmissionLayerLengthCalculate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet)
{
    uint16_t length = 0;
    uint16_t lengthApplicationLayer = 0;

    if (layer == NULL)
    {
        return 0;
    }
    if (packet == NULL)
    {
        return 0;
    }

    length++;   /* ResponseID */
    length++;   /* Flag1.     */
    
    lengthApplicationLayer = hisense_ApplicationLayerLengthCalculate(layer->applicationLayer,
                                                                     &packet->ApplicationLayerContent);

    return length + lengthApplicationLayer;
}

/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_TransmissionLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_TransmissionLayerPacket *packet)
{
    uint8_t returnVal = 0;

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

    *(buffer + *bufferLength) = packet->ResponseID;
    (*bufferLength)++;
    *(buffer + *bufferLength) = packet->Flag1;
    (*bufferLength)++;

    returnVal = hisense_ApplicationLayerContentGenerate(buffer, bufferLength, &packet->ApplicationLayerContent);

    return returnVal;
}



#else /* HISENSE_PROTOCOL_USE */

void hisense_TransmissionL_init(HISENSE_TRANSMISSIONL_LAYER* layer){}
void hisense_TransmissionLayerPacketClean(hisense_TransmissionLayerPacket *packet){}
void hisense_TransmissionLayerHeadClean(hisense_TransmissionLayerPacket *packet){}
uint8_t hisense_TransmissionLayerPacketParse(HISENSE_TRANSMISSIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_TransmissionLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum){return 0;}
uint8_t hisense_TransmissionLayerPacketValidate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet){return 0;}
uint8_t hisense_TransmissionLayerResponseMessageBuild(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet, hisense_TransmissionLayerPacket *response){return 0;}
uint16_t hisense_TransmissionLayerLengthCalculate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet){return 0;}
uint8_t hisense_TransmissionLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_TransmissionLayerPacket *packet){return 0;}

#endif /* HISENSE_PROTOCOL_USE */

/* [] END OF FILE */
