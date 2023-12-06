//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_LinkL.h
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2021/12/14
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Link Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef HISENSE_LINKL_H
#define HISENSE_LINKL_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "hisense_NetworkL.h"
#include "Netbuf/netbuf.h"
#include "Platform/serialport.h"
#include "Platform/timer.h"
#include "src/hisense_app_public.h"



#define HISENSE_LL_FRAME_HEADER_1               0xF4u
#define HISENSE_LL_FRAME_HEADER_2               0xF5u
#define HISENSE_LL_FRAME_HEADER                 ((HISENSE_LL_FRAME_HEADER_1 << 8) + HISENSE_LL_FRAME_HEADER_2)
#define HISENSE_LL_FRAME_END_1                  0xF4u
#define HISENSE_LL_FRAME_END_2                  0xFBu
#define HISENSE_LL_FRAME_END                    ((HISENSE_LL_FRAME_END_1 << 8) + HISENSE_LL_FRAME_END_2)
#define HISENSE_LL_STUFFING_BYTE                0xF4u

#define SEND_FRAME_BUFFER_SIZE                  512
#define RECEIVE_FRAME_BUFFER_SIZE               APPLICATION_LL_MAX_FRAME_SIZE

#define HISENSE_LINKL_NUMBER_OF_RETRIES                         10
#define HISENSE_LINKL_NUMBER_OF_RETRIES_EXTENDED               (1 + \
                                                                max(HISENSE_LINKL_NUMBER_OF_RETRIES, \
                                                                    3*((application_app_bank_size_get() / (20*1024))+1)))
#define HISENSE_LINKL_REPLY_TIMEOUT                             2000

/* Setup check: */
#if (SYNCHRONIZED_TX_NOSYNCHRO_TIMEOUT > (HISENSE_LINKL_REPLY_TIMEOUT / 2))
#error TX no synchronization timeout too high. Should be less than 1 second to be able to send reply within timeout (2s->1s: safety factor) */
#endif

typedef enum 
{
    HISENSE_LINKL_LAYER_COUNTER_CRCOK,
    HISENSE_LINKL_LAYER_COUNTER_CRCFAIL,
    /*
    Add counters here, if needed.
    */
    HISENSE_LINKL_LAYER_COUNTER_MAX
} HISENSE_LINKL_LAYER_COUNTER;


typedef struct hisense_LinkLayerPacket
{
    uint16_t Header;
    uint8_t ResponseID;
    uint8_t Flag1;
    uint16_t FrameLength;
    uint8_t FrameLengthSize;        /* Flag for Transmission layer encription */
    hisense_NetworkLayerPacket NetworkLayerContent;
    uint32_t Checksum;
    uint16_t FrameTail;
} hisense_LinkLayerPacket;

typedef struct hisense_LinkLayerCounters_t
{
    uint32_t crc_fail_cnt;
    uint32_t crc_ok_cnt;
} hisense_LinkLayerCounters;

typedef struct hisense_LinkL_layer_t
{
    uint8_t receivedByte;
    uint8_t previousReceivedByte;
    uint8_t LLResponseOperationResult;

    hisense_LinkLayerCounters counters;

    TIMER replyTimer;                                       /* Time from receiving the frame for measuring maximum timeout when reply should be sent */

    /* Upper layer: */
    HISENSE_NETWORKL_LAYER* networkLayer;

    /* Lower layer: */
    void* wifiLayer;
} HISENSE_LINKL_LAYER;


void hisense_LinkL_init(HISENSE_LINKL_LAYER *layer);
uint8_t hisense_LinkL_MessagePreparedForTransmission(HISENSE_LINKL_LAYER *layer);   /* 0 no message waiting
                                                                                       1 message waiting to be sent */
uint32_t hisense_LinkL_counter_get(HISENSE_LINKL_LAYER *layer, HISENSE_LINKL_LAYER_COUNTER counter);

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_LinkL_BuildLinkLayerMasterRequestMessage(HISENSE_LINKL_LAYER *layer, hisense_LinkLayerPacket *packet, NETBUF** tx_frame);
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */

PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t hisense_LinkL_ReceiveByte(HISENSE_LINKL_LAYER *layer, NETBUF* pnb, uint8_t value);
uint8_t hisense_LinkL_RxFrameProcess(HISENSE_LINKL_LAYER *layer, NETBUF* rx_frame, NETBUF** tx_frame, uint8_t* app_al_response_id, uint16_t* app_data_position, uint16_t* app_data_size);
void hisense_LinkL_run(HISENSE_LINKL_LAYER *layer);

#endif  /* HISENSE_LINKL_H */

/* [] END OF FILE */
