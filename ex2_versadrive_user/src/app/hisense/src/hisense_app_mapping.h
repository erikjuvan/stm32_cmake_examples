/*!
 * @file application_Mapping.h
 * @author Uros Sadek (uros.sadek@emsiso.eu)
 * @date 31.05.2021
 *
 * @copyright Copyright (c) 2021 EMSISO d.o.o. All Rights Reserved
 *
 * @brief
 */

#ifndef HISENSE_APP_MAPPING_H
#define HISENSE_APP_MAPPING_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "hisense/hisense_ApplicationL.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
// clang-format off
/*   MessageType
     |    MessageSubType
     |    |  RequestRequiredResponseID
     |    |  |  VerifyFunction                 ResponseFunction
     |    |  |  |                              | */
#define APPLICATION_MAPPING_MESSAGE_TYPE_LIST_TABLE_EXTENSION                    \
    {102, 1, 0, hisense_mapping_verify_get_PU, hisense_mapping_response_get_PU}, \
    {101, 1, 0, hisense_mapping_verify_set_PU, hisense_mapping_response_set_PU}, \
    {102, 2, 0, hisense_mapping_handle_MAT,    NULL                           }, \
    {101, 2, 0, hisense_mapping_handle_MAT,    NULL                           }
// clang-format on

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
// Hisense communication callbacks
void hisense_app_mapping_init(void);
void hisense_mapping_verify_get_PU(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
);
void hisense_mapping_response_get_PU(HISENSE_APPLICATIONL_LAYER *layer);
void hisense_mapping_verify_set_PU(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
);
void hisense_mapping_response_set_PU(HISENSE_APPLICATIONL_LAYER *layer);
void hisense_mapping_handle_MAT(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
);

#endif /* HISENSE_APP_MAPPING_H */
/* [] END OF FILE */
