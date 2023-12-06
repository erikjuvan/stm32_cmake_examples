//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        hisense_Mapping_app.c
// CREATION DATE:      2019/04/15
// AUTHOR:             Blaz Plaskan
// LAST CHANGED DATE:  2022/03/07
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Hisense protocol frame mapping to handler functions (application).
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#include "cfg/project_cfg.h"
#include "hisense_Mapping.h"

#if HISENSE_PROTOCOL_USE

#include "types.h"
#include "Communication/wifi.h"
#include "hisense/hisense_LinkL.h"
#include "hisense/hisense_NetworkL.h"
#include "hisense/hisense_TransmissionL.h"
#include "hisense/hisense_ApplicationL.h"
#include "src/hisense_app_mapping.h"


hisense_Mapping_MessageTypeList MessageTypeList[] =
{
/*   MessageType
      |   MessageSubType
      |    |   RequestRequiredResponseID
      |    |    |    VerifyFunction                                 ResponseFunction 
      |    |    |    |                                              |                       */
    APPLICATION_MAPPING_MESSAGE_TYPE_LIST_TABLE_EXTENSION

};
#define NUMBER_OF_SUPPORTED_MESSAGETYPES        (sizeof(MessageTypeList) / sizeof(hisense_Mapping_MessageTypeList))


/*
Public API functions:
*/


hisense_Mapping_MessageTypeList* hisense_Mapping_MessageTypeList_get(void)
{
    return &MessageTypeList[0];
}

uint8_t hisense_Mapping_NumberOfSupportedMessages_get(void)
{
    return NUMBER_OF_SUPPORTED_MESSAGETYPES;
}


#else /* HISENSE_PROTOCOL_USE */

hisense_Mapping_MessageTypeList* hisense_Mapping_MessageTypeList_get(void){return NULL;}
uint8_t hisense_Mapping_NumberOfSupportedMessages_get(void){return 0;}


#endif /* HISENSE_PROTOCOL_USE */

/* [] END OF FILE */
