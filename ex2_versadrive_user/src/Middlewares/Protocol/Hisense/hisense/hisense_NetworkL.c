//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_NetworkL.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2021/12/13
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Network Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "cfg/project_cfg.h"
#include "hisense_NetworkL.h"

#if HISENSE_PROTOCOL_USE

#include "hisense_Base.h"


static const uint8_t ApplianceAddressCode[] =
{ 
    HISENSE_APPLIANCEADDRESSCODE_DEVICETYPE,        /* CPR2009 Appendix B device type */
    HISENSE_APPLIANCEADDRESSCODE_DEVICEMCUNUMBER 
};




/*
Public API functions
*/

void hisense_NetworkL_init(HISENSE_NETWORKL_LAYER* layer)
{
    layer->NLResponseOperationResult = 0;

    hisense_TransmissionL_init(layer->transmissionLayer);
}

void hisense_NetworkLayerPacketClean(hisense_NetworkLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->ResponseID = 0;
    packet->Flag1 = 0;
    hisense_Base_ClearLocalBuffer(packet->AddressCode1, ADDRESS_CODE_LENGTH);
    hisense_Base_ClearLocalBuffer(packet->AddressCode2, ADDRESS_CODE_LENGTH);
//    packet->parent = NULL;
    hisense_TransmissionLayerPacketClean(&(packet->TransmissionLayerContent));
}

void hisense_NetworkLayerHeadClean(hisense_NetworkLayerPacket *packet)
{
    if (packet == NULL)
    {
        return;
    }

    packet->ResponseID = 0;
    packet->Flag1 = 0;
    hisense_Base_ClearLocalBuffer(packet->AddressCode1, ADDRESS_CODE_LENGTH);
    hisense_Base_ClearLocalBuffer(packet->AddressCode2, ADDRESS_CODE_LENGTH);

    hisense_TransmissionLayerHeadClean(&(packet->TransmissionLayerContent));
}

/* 
Return values: 
0  - No response!,
1  - OK 
*/
uint8_t hisense_NetworkLayerPacketParse(HISENSE_NETWORKL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_NetworkLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum)
{
    uint8_t counter = 0;
    uint8_t lengthOfAddresses = 2;      /* 2 byte IP address length */
    #if 0
    Removed due to "set but not used" warning
    uint8_t typeOfAddresses = 1;        /* 1 = Hisense IP address code, 
                                           2 = IEEE IPv4 address, 
                                           3 = Hisense ID address code, 
                                           4 = Hisense ID address code + 2byte IP address code
                                        */
    #endif

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

    /* hisense_NetworkLayerPacketClean(destination); */

    layer->NLResponseOperationResult = 0;

    (destination->TransmissionLayerContent).parent = destination;

    destination->ResponseID = data[(*currentArrayPointer)++];
    destination->Flag1 = data[(*currentArrayPointer)++];
    if ((destination->Flag1 & 0x01) > 0)        /* 7.4 D0 bit (Flag1) tells if Flag2 is present     */
    {
        return 0;
    }

    /* address1 */
    counter = lengthOfAddresses;
    for (;counter > 0;counter--)
    {
        destination->AddressCode1[counter - 1] = data[(*currentArrayPointer)++];
    }
    /* address2 */
    counter = lengthOfAddresses;
    for (;counter > 0;counter--)
    {
        destination->AddressCode2[counter - 1] = data[(*currentArrayPointer)++];
    }

    return hisense_TransmissionLayerPacketParse(layer->transmissionLayer,
                                                dataLength, 
                                                data, 
                                                currentArrayPointer, 
                                                &(destination->TransmissionLayerContent), 
                                                networkLayerPacketLength, 
                                                messageLengthWithoutFrameTailAndChecksum);
}


/*
Return values:
0 - Fail.          MCU should not send reply to the packet.
1 - Validation OK. MCU should reply to the packet.
2 - Validation OK. MCU should forward packet to downstream communication port(s) (Destination address mismatch).
3 - Validation OK. This is Reply to Master request sent from this MCU. Reply should not be sent (prevention of loop). Master request callback function will be called.
*/
uint8_t hisense_NetworkLayerPacketValidate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet)
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
    layer->NLResponseOperationResult = 1;

    #if (FORWARD_ADDRESS_0_0_MESSAGES == 1)
    if ((packet->AddressCode1[0] == 0x00) && (packet->AddressCode1[1] == 0x00))
    {
        returnVal = 2;                          /* address 0x00 0x00 -> forward (if enabled)*/
        return returnVal;
    }
    #endif /* --> FORWARD_ADDRESS_0_0_MESSAGES */

    if (!((packet->AddressCode1[1] == ApplianceAddressCode[0] && packet->AddressCode1[0] == ApplianceAddressCode[1]) ||
          (packet->AddressCode1[0] == 0x00 && packet->AddressCode1[1] == 0x00)))
    {
        returnVal = 2;                          /* wrong destination address -> forward     */
        return returnVal;
    }

    if ((packet->Flag1 & 0x01) == 0x01)
    {
        layer->NLResponseOperationResult = 8;   /* Flag 2 must not be present               */
        returnVal = 1;
    }
    if ((packet->Flag1 & 0x20) != 0x00)
    {
        layer->NLResponseOperationResult = 8;   /* Only address code 1 and 2 are present.   */
        returnVal = 1;
    }
    if ((packet->Flag1 & 0xD8) != 0x00)
    {
        layer->NLResponseOperationResult = 8;   /* Flag 1 bits 7,6,4,3 must be 0.           */
        returnVal = 1;
    }

    returnVal = hisense_TransmissionLayerPacketValidate(layer->transmissionLayer,
                                                        &(packet->TransmissionLayerContent));

    return returnVal;
}


/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_NetworkLayerResponseMessageBuild(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet, hisense_NetworkLayerPacket *response)
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

    /* hisense_NetworkLayerPacketClean(response); */

    response->AddressCode1[1] = packet->AddressCode2[1];
    response->AddressCode1[0] = packet->AddressCode2[0];
    response->AddressCode2[0] = ApplianceAddressCode[1];
    response->AddressCode2[1] = ApplianceAddressCode[0];
    response->ResponseID = layer->NLResponseOperationResult;
    response->Flag1 = 0x00; /* Defaults for NL Identification 1, no flag2, IP codes of length of 2 bytes, only destination and source address code */

    return hisense_TransmissionLayerResponseMessageBuild(layer->transmissionLayer,
                                                         &(packet->TransmissionLayerContent), 
                                                         &(response->TransmissionLayerContent));
}

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_NetworkLayerMasterRequestMessageBuild(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet, hisense_NetworkLayerPacket *request)
{
    if ((layer == NULL) || (packet == NULL) || (request == NULL))
    {
        return 0;
    }

    /* hisense_NetworkLayerPacketClean(request); */
    hisense_NetworkLayerHeadClean(request);

    request->AddressCode1[0] = packet->AddressCode1[1];
    request->AddressCode1[1] = packet->AddressCode1[0];
    request->AddressCode2[0] = ApplianceAddressCode[1];
    request->AddressCode2[1] = ApplianceAddressCode[0];
    request->ResponseID = 0x00;
    request->Flag1 = 0x00;

    if (hisense_TransmissionLayerMasterRequestMessageBuild(layer->transmissionLayer,
                                                           &(packet->TransmissionLayerContent), 
                                                           &(request->TransmissionLayerContent)) != 1)
    {
        return 0;
    }

    return 1;
}
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

uint16_t hisense_NetworkLayerLengthCalculate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet)
{
    uint16_t length = 0;
    uint8_t lengthOfAddresses = 2;      /* 2 byte IP address length */
    uint8_t numberOffAddressCodes = 2;  /* 2 = only destination and source addresses, 
                                           5 = all 5 address codes defined 
                                        */

    if (layer == NULL)
    {
        return 0;
    }

    if (packet == NULL)
    {
        return 0;
    }

    length++; /* ResponseID */
    length++; /* Flag1      */
    
    length += (numberOffAddressCodes * lengthOfAddresses);  /* AddressCodes                 */
    length += hisense_TransmissionLayerLengthCalculate(layer->transmissionLayer,
                                                       &packet->TransmissionLayerContent);

    return length;
}


/*
Return values:
0 - Fail
1 - OK
*/
uint8_t hisense_NetworkLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_NetworkLayerPacket *packet)
{
    uint8_t returnVal = 0;
    uint8_t counter = 0;
    uint8_t lengthOfAddresses = 2;      /* 2 byte IP address length     */

    if (buffer == NULL)
    {
        return 0;
    }
    if (bufferLength == NULL)
    {
        return 0;
    }
    if  (packet == NULL)
    {
        return 0;
    }

    *(buffer + *bufferLength) = packet->ResponseID;
    (*bufferLength)++;
    *(buffer + *bufferLength) = packet->Flag1;
    (*bufferLength)++;
    
    /* address1 */
    counter = lengthOfAddresses;
    for (;counter > 0;counter--)
    {
        *(buffer + *bufferLength) = packet->AddressCode1[counter - 1];
        (*bufferLength)++;
    }
    /* address2 */
    counter = lengthOfAddresses;
    for (;counter > 0;counter--)
    {
        *(buffer + *bufferLength) = packet->AddressCode2[counter - 1];
        (*bufferLength)++;
    }
    
    returnVal = hisense_TransmissionLayerContentGenerate(buffer, bufferLength, &packet->TransmissionLayerContent);

    return returnVal;
}

const uint8_t* hisense_NetworkLayer_ApplianceAddressCode_get(void)
{
    return &ApplianceAddressCode[0];
}

#else /* HISENSE_PROTOCOL_USE */

void hisense_NetworkL_init(HISENSE_NETWORKL_LAYER* layer){}
void hisense_NetworkLayerPacketClean(hisense_NetworkLayerPacket *packet){}
void hisense_NetworkLayerHeadClean(hisense_NetworkLayerPacket *packet){}
uint8_t hisense_NetworkLayerPacketParse(HISENSE_NETWORKL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_NetworkLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum){return 0;}
uint8_t hisense_NetworkLayerPacketValidate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet){return 0;}
uint8_t hisense_NetworkLayerResponseMessageBuild(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet, hisense_NetworkLayerPacket *response){return 0;}
uint16_t hisense_NetworkLayerLengthCalculate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet){return 0;}
uint8_t hisense_NetworkLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_NetworkLayerPacket *packet){return 0;}
const uint8_t* hisense_NetworkLayer_ApplianceAddressCode_get(void){return 0;}

#endif /* HISENSE_PROTOCOL_USE */

/* [] END OF FILE */
