/*!
 * @file application_Mapping.c
 * @author Uros Sadek (uros.sadek@emsiso.eu)
 * @date 31.05.2021
 *
 * @copyright Copyright (c) 2021 EMSISO d.o.o. All Rights Reserved
 *
 * @brief
 */

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "hisense/hisense_NetworkL.h"
#include "types.h"
#include "hisense/hisense_ApplicationL.h"
#include "hisense/hisense_Base.h"
#include <stddef.h>

#include "hisense/hisense_proto.h"

#include "communication/mat/mat.h"

////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 *   Mapping initialization function.
 *
 * @param[in]    None.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void hisense_app_mapping_init(void)
{
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Mapping function callback to process verification on SetParameters requests.
 *
 *
 * @param[in]    layer               Pointer to hisense APP layer instance.
 * @param[in]    contentLength       Received \p data length.
 * @param[in]    data                Received data pointer.
 * @param[in]    packet              Hisense APP layer packet pointer.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////

void hisense_mapping_verify_get_PU(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
)
{
    // TODO in WM project all PU MU comms were handled in the SET function
    // TODO trace here to see if we ever come here

    if (contentLength != 0)
    {
        hisense_ApplicationL_ALResponseOperationResult_set(
            layer, AL_RESPONSE_ID_CONFLICT_MISSING_CONTENT
        );
        return;
    }

    hisense_ApplicationL_ALResponseOperationResult_set(layer, AL_RESPONSE_ID_SUCCESSFUL);
}

void hisense_mapping_response_get_PU(HISENSE_APPLICATIONL_LAYER *layer)
{
    // empty
}

// NOTE: set_PU name can be confusing. Actual action is: PU_requests_parameter_set_on_MU.
// Or in other words PU_wants_to_set. This applies to all other set/get functions
// in this file.
void hisense_mapping_verify_set_PU(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
)
{
    // Entire communication, verify and response is handled here.
    // On the level of hisense CPR protocol everyhing will always be returned as success.
    // Errors will be implemented on the higher payload level.

    pu_parse_packet(data, contentLength);

    hisense_ApplicationL_ALResponseOperationResult_set(layer, AL_RESPONSE_ID_SUCCESSFUL);

    uint8_t *buffer = pu_get_response_buffer();
    uint16_t len    = pu_get_response_size();
    hisense_ApplicationL_SelectCurrentResponseBuffer(layer, buffer, len);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Mapping function callback to process reposonse on SetParameters requests.
 *
 * @param[in]    layer       Pointer to hisense APP layer instance.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void hisense_mapping_response_set_PU(HISENSE_APPLICATIONL_LAYER *layer)
{
    // Response is generated in verify function.
}

void hisense_mapping_handle_MAT(
    HISENSE_APPLICATIONL_LAYER *layer, uint16_t contentLength, uint8_t *data, void *packet
)
{
    bool ret = mat_handle_req(data, contentLength);
    if (ret)
    {
        hisense_ApplicationL_SelectCurrentResponseBuffer(
            layer, mat_reponse_buffer_get(), mat_reponse_buffer_length()
        );
        hisense_ApplicationL_ALResponseOperationResult_set(layer, AL_RESPONSE_ID_SUCCESSFUL);
    }
    else
    {
        hisense_ApplicationL_ALResponseOperationResult_set(layer, AL_RESPONSE_ID_FAILED);
    }

    hisense_ApplicationL_CommandProcessingFinshed(layer);
}

/* [] END OF FILE */
