//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_ApplicationL.h
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/01/25
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol Application Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef HISENSE_APPLICATIONL_H
#define HISENSE_APPLICATIONL_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "Platform/serialport.h"
#include "Netbuf/netbuf.h"

#define MAX_DATA_MESSAGE_LENGTH                 APPLICATION_AL_MAX_DATA_SIZE

typedef enum al_response_id_t
{
    AL_RESPONSE_ID_FAILED                   = 0x00,
    AL_RESPONSE_ID_SUCCESSFUL               = 0x01,
    AL_RESPONSE_ID_RESERVED2                = 0x02,
    AL_RESPONSE_ID_MCU_NOT_EXSIST           = 0x03,
    AL_RESPONSE_ID_NEED_TIME                = 0x04,
    AL_RESPONSE_ID_NO_SUCH_VALUE            = 0x05,
    AL_RESPONSE_ID_CANNOT_EXECUTE           = 0x06,
    AL_RESPONSE_ID_REMOTE_CONTROL_OFF       = 0x07,
    AL_RESPONSE_ID_NOT_SUPPORTED            = 0x08,
    AL_RESPONSE_ID_CONFLICT_MISSING_CONTENT = 0x09,
    AL_RESPONSE_ID_ADDRESS_OVER_RANGE       = 0x0A
} AL_RESPONSE_ID;

typedef enum big_data_byte1_t
{
    BIG_DATA_BYTE1_EEPROM                   = 0xA1,
    BIG_DATA_BYTE1_PROGRAM                  = 0xA2,
    BIG_DATA_BYTE1_CONTROL_KEY              = 0xA3,
    BIG_DATA_BYTE1_EEPROM_KEY               = 0xA4,
    BIG_DATA_BYTE1_PROGRAM_KEY              = 0xA5,
    BIG_DATA_BYTE1_PASSWORD_KEY             = 0xA6,
    BIG_DATA_BYTE1_PICTURE                  = 0xB1,
    BIG_DATA_BYTE1_MUSIC                    = 0xB2,
    BIG_DATA_BYTE1_VIDEO                    = 0xB3
} BIG_DATA_BYTE1;


typedef struct hisense_ApplicationLayerPacket
{
    uint8_t MessageType;
    uint8_t MessageSubType;
    uint8_t ResponseID;
    uint8_t Frame;
    uint16_t FrameNo;
    uint8_t *pData;
    uint8_t Data[MAX_DATA_MESSAGE_LENGTH]; /* Must be aligned on 4 address !!! STM32 specific, otherwise causes Hard Fault. */
    uint16_t DataLength;
    uint8_t master_request;
    void *parent;
} __attribute__((aligned(4))) hisense_ApplicationLayerPacket;

typedef struct hisense_ApplicationL_layer_t
{
    uint8_t ALResponseOperationResult;
    uint8_t WaitProcessing;                 /* 0 - Reply should be sent immediately;   1 - Reply should wait */
    uint8_t RequestALDataFrame;
    uint8_t ResponseALDataFrame;
    uint16_t ResponseALDataFrameNo;
    uint16_t RequestALDataFrameNo;

    NETBUF* ResponseALDataBuffer;
    uint16_t ResponseALDataBufferLength;

    #if (MASTER_FUNCTIONALITY_USE == 1)
    uint8_t MasterRequest_active;
    uint8_t MasterRequest_MessageType;
    uint8_t MasterRequest_MessageSubtype;
    #endif /* (MASTER_FUNCTIONALITY_USE == 1) */

    /* Upper layer: */
    /*      ---     */

    /* Lower layer: */
    void* transmissionLayer;
} HISENSE_APPLICATIONL_LAYER;


/* Hisense communication stack internal functions: */
void hisense_ApplicationL_init(HISENSE_APPLICATIONL_LAYER* layer);
void hisense_ApplicationLayerPacketClean(hisense_ApplicationLayerPacket *packet);
void hisense_ApplicationLayerHeadClean(hisense_ApplicationLayerPacket *packet);
uint8_t hisense_ApplicationLayerPacketParse(HISENSE_APPLICATIONL_LAYER *layer, uint16_t dataLength, uint8_t *data, uint16_t *currentArrayPointer, hisense_ApplicationLayerPacket *destination, uint16_t messageLengthWithoutFrameTailAndChecksum);
uint8_t hisense_ApplicationLayerPacketValidate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet);
uint8_t hisense_ApplicationLayerResponseMessageBuild(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet, hisense_ApplicationLayerPacket *response);
uint16_t hisense_ApplicationLayerLengthCalculate(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet);
uint8_t hisense_ApplicationLayerContentGenerate(uint8_t *buffer, uint16_t *bufferLength, hisense_ApplicationLayerPacket *packet);
uint8_t hisense_ApplicationL_IsCommandProcessingFinshed(HISENSE_APPLICATIONL_LAYER *layer);

#if (MASTER_FUNCTIONALITY_USE == 1)
uint8_t hisense_ApplicationLayerMasterRequestMessageBuild(HISENSE_APPLICATIONL_LAYER *layer, hisense_ApplicationLayerPacket *packet, hisense_ApplicationLayerPacket *request);
void hisense_ApplicationLayerMasterRequest_SetMessageType(HISENSE_APPLICATIONL_LAYER *layer, uint8_t active, uint8_t MessageType, uint8_t MessageSubtype);
#endif /* (MASTER_FUNCTIONALITY_USE == 1) */


/* Public API functions: */

/*******************************************************************************************************************//**
* @brief    Returns Application layer's frame type.
* 
* @param    layer           Hisense Application layer.
* @return   0x00 - This frame is a new frame and no frames are following it.
*           0x10 - This frame is a new frame and another new frame will follow it.
*           0x01 - This frame is re-transmitted, and no frames are following it.
*           0x11 - This frame is re-transmitted, and another new frame will follow it.
* 
* @details  
*     This property is used mainly for large data transfer. It depends on message type if this
*     property is present in communication frame.
*     Example:
* @code
*    uint8_t frame_type;
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*
*    frame_type = hisense_ApplicationL_RequestALDataFrame_get(layer);
* @endcode
*
***********************************************************************************************************************/
uint8_t hisense_ApplicationL_RequestALDataFrame_get(HISENSE_APPLICATIONL_LAYER *layer);


/*******************************************************************************************************************//**
* @brief    Returns Application layer's frame number.
* 
* @param    layer           Hisense Application layer.
* @return   Frame number, valid value for this property is from 1 to 65535. NOTE: 0 value is reserved!
* 
* @details  
*     This property is used mainly for large data transfer. It depends on message type if this
*     property is present in communication frame.
     Example:
* @code
*    uint16_t frame_no;
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*
*    frame_no = hisense_ApplicationL_RequestALDataFrameNo_get(layer);
* @endcode
*
***********************************************************************************************************************/
uint16_t hisense_ApplicationL_RequestALDataFrameNo_get(HISENSE_APPLICATIONL_LAYER *layer);


/*******************************************************************************************************************//**
* @brief    Selects buffer for response to the command.
* 
* @param    layer           Hisense Application layer.
* @param    currentBuffer   Pointer to the buffer of the response to the command.
* @param    length          Length of the buffer of the response to the command.
* @return   1 - buffer successfully accepted; 0 - buffer is not successfully accepted.

* 
* @details Example:
* @code
*    static uint8_t response[2];
*
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*    response[0] = 0x56;
*    response[1] = 0x57;
*
*    hisense_ApplicationL_SelectCurrentResponseBuffer(layer, &response[0], sizeof(response));
* @endcode
*
***********************************************************************************************************************/
uint8_t hisense_ApplicationL_SelectCurrentResponseBuffer(HISENSE_APPLICATIONL_LAYER *layer, uint8_t *currentBuffer, uint16_t length);


/*******************************************************************************************************************//**
* @brief    Selects frame type and frame number for response to the command.
* 
* @param    layer           Hisense Application layer.
* @param    frame           Frame type of the response to the command.
* @param    frameNo         Frame number of the response to the command.
* 
* @details Example:
* @code
*    uint8_t frame_type;
*    unit16_t frame_no;
*
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*    frame_type = 0x11; // This frame is re-transmitted, and another new frame will follow it.
*    frame_no = 42789;
*
*    hisense_ApplicationL_SelectCurrentResponseFrameAndFrameNo(layer, frame_type, frame_no);
* @endcode
*
***********************************************************************************************************************/
void hisense_ApplicationL_SelectCurrentResponseFrameAndFrameNo(HISENSE_APPLICATIONL_LAYER *layer, uint8_t frame, uint16_t frameNo);


/*******************************************************************************************************************//**
* @brief    Selects Application layer's response ID to the command.
* 
* @param    layer           Hisense Application layer.
* @param    response        AL response ID, see CPR2009 document and AL_RESPONSE_ID enum.
* 
* @details Example:
* @code
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*
*    hisense_ApplicationL_ALResponseOperationResult_set(layer, AL_RESPONSE_ID_SUCCESSFUL);
* @endcode
*
***********************************************************************************************************************/
void hisense_ApplicationL_ALResponseOperationResult_set(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response);


/*******************************************************************************************************************//**
* @brief    Selects Application layer's response ID to the command, but not send response immediately.
*           Response will be sent after hisense_ApplicationL_CommandProcessingFinshed() function call.
* 
* @param    layer           Hisense Application layer.
* @param    response        AL response ID, see CPR2009 document and AL_RESPONSE_ID enum.
* 
* @details Example:
* @code
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*
*    hisense_ApplicationL_ALResponseOperationResult_prepare(layer, AL_RESPONSE_ID_SUCCESSFUL);
* @endcode
*
***********************************************************************************************************************/
void hisense_ApplicationL_ALResponseOperationResult_prepare(HISENSE_APPLICATIONL_LAYER *layer, AL_RESPONSE_ID response);


/*******************************************************************************************************************//**
* @brief    Notifies Application layer that last command processing is finished and response can be sent.
* 
* @param    layer           Hisense Application layer.
* 
* @details Example:
* @code
*    HISENSE_APPLICATIONL_LAYER *layer;
*    layer = ...
*
*    hisense_ApplicationL_CommandProcessingFinshed(layer);
* @endcode
*
***********************************************************************************************************************/
void hisense_ApplicationL_CommandProcessingFinshed(HISENSE_APPLICATIONL_LAYER *layer);


#endif /* HISENSE_APPLICATIONL_H */

/* [] END OF FILE */
