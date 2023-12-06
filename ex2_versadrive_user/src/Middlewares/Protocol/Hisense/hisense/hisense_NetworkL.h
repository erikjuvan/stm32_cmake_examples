//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_NetworkL.h
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2021/11/25
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Network Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef HISENSE_NETWORKL_H
#define HISENSE_NETWORKL_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "hisense_TransmissionL.h"
#include "Platform/serialport.h"

#define ADDRESS_CODE_LENGTH 8

typedef struct hisense_NetworkLayerPacket
{
    uint8_t ResponseID;
    uint8_t Flag1;
    uint8_t AddressCode1[ADDRESS_CODE_LENGTH];
    uint8_t AddressCode2[ADDRESS_CODE_LENGTH];
    hisense_TransmissionLayerPacket TransmissionLayerContent;
//    void *parent;
} hisense_NetworkLayerPacket;


typedef struct hisense_NetworkL_layer_t
{
    uint8_t NLResponseOperationResult;

    /* Upper layer: */
    HISENSE_TRANSMISSIONL_LAYER* transmissionLayer;

    /* Lower layer: */
    void* linkLayer;
} HISENSE_NETWORKL_LAYER;


void hisense_NetworkL_init(HISENSE_NETWORKL_LAYER* layer);

void hisense_NetworkLayerPacketClean(hisense_NetworkLayerPacket *packet);
void hisense_NetworkLayerHeadClean(hisense_NetworkLayerPacket *packet);
uint8_t hisense_NetworkLayerPacketParse(HISENSE_NETWORKL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_NetworkLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum);
uint8_t hisense_NetworkLayerPacketValidate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet);
uint8_t hisense_NetworkLayerResponseMessageBuild(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet, hisense_NetworkLayerPacket *response);
uint16_t hisense_NetworkLayerLengthCalculate(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet);
uint8_t hisense_NetworkLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_NetworkLayerPacket *packet);
const uint8_t* hisense_NetworkLayer_ApplianceAddressCode_get(void);

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_NetworkLayerMasterRequestMessageBuild(HISENSE_NETWORKL_LAYER *layer, hisense_NetworkLayerPacket *packet, hisense_NetworkLayerPacket *request);
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

#endif /* HISENSE_NETWORKL_H */

/* [] END OF FILE */
