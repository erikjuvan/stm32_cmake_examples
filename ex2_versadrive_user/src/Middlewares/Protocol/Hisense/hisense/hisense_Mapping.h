//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_Mapping.h
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/03/07
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol frame mapping to handler functions (common for BL and APP).
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------
    
#ifndef HISENSE_MAPPING_H
#define HISENSE_MAPPING_H

#include "types.h"
#include "hisense_ApplicationL.h"

typedef struct hisense_Mapping_MessageTypeList
{
    uint8_t MessageType;
    uint8_t MessageSubType;
    uint8_t RequestRequiredResponseID;
    void   (*VerifyFunction)(HISENSE_APPLICATIONL_LAYER *, uint16_t, uint8_t *, void *);     /* return 0 = ERROR, return 1 = OK */
    void   (*ResponseFunction)(HISENSE_APPLICATIONL_LAYER *);
} hisense_Mapping_MessageTypeList;


hisense_Mapping_MessageTypeList* hisense_Mapping_MessageTypeList_get(void);
uint8_t hisense_Mapping_NumberOfSupportedMessages_get(void);
void hisense_Mapping_aFotaAllowed_setState(uint8_t afota_allowed);
uint8_t hisense_Mapping_aFotaAllowed_getState(void);


#endif  /* HISENSE_MAPPING_H */

/* [] END OF FILE */
