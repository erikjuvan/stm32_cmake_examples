//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_TransmissionL.h
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

#ifndef HISENSE_TRANSMISSIONL_H
#define HISENSE_TRANSMISSIONL_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "hisense_ApplicationL.h"
#include "Platform/serialport.h"

typedef struct hisense_TransmissionLayerPacket
{
    uint8_t ResponseID;
    uint8_t Flag1;
    hisense_ApplicationLayerPacket ApplicationLayerContent;
    void *parent;
} hisense_TransmissionLayerPacket;

typedef struct hisense_TransmissionL_layer_t
{
    uint8_t TLResponseOperationResult;

    /* Upper layer: */
    HISENSE_APPLICATIONL_LAYER* applicationLayer;

    /* Lower layer: */
    void* networkLayer;
} HISENSE_TRANSMISSIONL_LAYER;


void hisense_TransmissionL_init(HISENSE_TRANSMISSIONL_LAYER* layer);

void hisense_TransmissionLayerPacketClean(hisense_TransmissionLayerPacket *packet);
void hisense_TransmissionLayerHeadClean(hisense_TransmissionLayerPacket *packet);
uint8_t hisense_TransmissionLayerPacketParse(HISENSE_TRANSMISSIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_TransmissionLayerPacket *destination, uint8_t networkLayerPacketLength, uint16_t messageLengthWithoutFrameTailAndChecksum);
uint8_t hisense_TransmissionLayerPacketValidate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet);
uint8_t hisense_TransmissionLayerResponseMessageBuild(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet, hisense_TransmissionLayerPacket *response);
uint16_t hisense_TransmissionLayerLengthCalculate(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet);
uint8_t hisense_TransmissionLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_TransmissionLayerPacket *packet);

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_TransmissionLayerMasterRequestMessageBuild(HISENSE_TRANSMISSIONL_LAYER *layer, hisense_TransmissionLayerPacket *packet, hisense_TransmissionLayerPacket *request);
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

#endif /* HISENSE_TRANSMISSIONL_H */

/* [] END OF FILE */
