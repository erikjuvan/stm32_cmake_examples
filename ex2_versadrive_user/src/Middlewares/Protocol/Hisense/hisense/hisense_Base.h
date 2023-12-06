//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_Base.h
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol base data management and calculation.
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2020 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef HISENSE_BASE_H
#define HISENSE_BASE_H

#include "types.h"
#include "Netbuf/netbuf.h"

uint32_t hisense_Base_CalculateWholeArrayCRC32(uint32_t dataLength, uint8_t *dataBytes);
void hisense_Base_ClearLocalBuffer(uint8_t *buffer, uint16_t length);
void hisense_Base_ShiftBytesToRight(uint8_t *buffer, uint16_t index, uint16_t bufferLength, uint16_t maxBufferLength);
void hisense_Base_HandleStuffedBytes(uint8_t *buffer, uint16_t *length, uint16_t maxBufferLength);
NETBUF* hisense_Base_HandleStuffedBytes_nb(NETBUF* pnb);
void hisense_Base_CopyData(uint8_t *source, uint8_t *destination, uint16_t length);
uint8_t hisense_Base_ConvertIntToBCD(uint8_t value);
uint8_t hisense_Base_ConvertBCDToInt(uint8_t bcdByte);
void hisense_Base_InitCRC32(uint32_t *result_CRC32);
void hisense_Base_CalculateCRC32(uint32_t *result_CRC32, uint32_t dataLength, uint8_t *dataBytes);
void hisense_Base_FinishCalculateCRC32(uint32_t *result_CRC32);

#endif  /* HISENSE_BASE_H */

/* [] END OF FILE */
