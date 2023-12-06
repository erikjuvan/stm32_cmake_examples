////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file     version.c
 * @note     Version source files
 * @author   Marko Rman
 * @date     01.02.2021
 * @brief    Revision functions
 */
////////////////////////////////////////////////////////////////////////////////
/*!
 * @addtogroup REVISION
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "version.h"
#include "func_table.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 1
#define FIRMWARE_VERSION_PATCH 0

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@@ SYMBOL       = identity_object__vendor_id
@@ A2L_TYPE     = MEASURE
@@ DATA_TYPE    = ULONG
@@ DESCRIPTION  = ""
@@ GROUP        = identity_object
@@ DIMENSION    = 1 1
@@ UNIT         = ""
@! CAN_INDEX    = 0x1018
@! CAN_SUBINDEX = 1
@! READ_LEVEL   = 6
@! WRITE_LEVEL  = 1
@! PERSIST      = 0
@! DEFAULT      = 793
@@ END
*/
volatile uint32_t identity_object__vendor_id = 793;

/*
@@ SYMBOL       = identity_object__product_code
@@ A2L_TYPE     = MEASURE
@@ DATA_TYPE    = ULONG
@@ DESCRIPTION  = ""
@@ GROUP        = identity_object
@@ DIMENSION    = 1 1
@@ UNIT         = ""
@! CAN_INDEX    = 0x1018
@! CAN_SUBINDEX = 2
@! READ_LEVEL   = 6
@! WRITE_LEVEL  = 1
@! PERSIST      = 0
@@ END
*/
volatile uint32_t identity_object__product_code = 0x20250120; // emDRIVE V2 250 120

/*
@@ SYMBOL       = identity_object__revision_number
@@ A2L_TYPE     = MEASURE
@@ DATA_TYPE    = ULONG
@@ DESCRIPTION  = ""
@@ GROUP        = identity_object
@@ DIMENSION    = 1 1
@@ UNIT         = ""
@! CAN_INDEX    = 0x1018
@! CAN_SUBINDEX = 3
@! READ_LEVEL   = 6
@! WRITE_LEVEL  = 1
@! PERSIST      = 0
@@ END
*/
volatile uint32_t identity_object__revision_number =
    (FIRMWARE_VERSION_MAJOR << 16) | (FIRMWARE_VERSION_MINOR << 8) | (FIRMWARE_VERSION_PATCH);
/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */

version_t version = {
    .major = FIRMWARE_VERSION_MAJOR,
    .minor = FIRMWARE_VERSION_MINOR,
    .patch = FIRMWARE_VERSION_PATCH
};

// Version which is included in image
volatile uint8_t version_image[4] __attribute__((section(".user_ver"))
) = {SW_MODEL_ID, FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_PATCH};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Functions gets system SW version
 *
 * @return       version_t version structure
 */
////////////////////////////////////////////////////////////////////////////////
const version_t *const get_system_version(void)
{
    return &version;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
