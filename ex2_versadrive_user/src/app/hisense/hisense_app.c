/*!
 * @file hisense_app.c
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
#include "Communication/serialprocess.h"
#include "Platform/platform.h"
#include "hisense_app_mapping.h"
#include "project_cfg.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
extern const uint32_t _MEMPART_FLASH_USER_APP_SIZE; // Address from linker script

////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////

void hisense_app_init(void)
{
    // Initialize platform for hisense
    platform_init();
    // Initialize communication blocks mapping
    hisense_app_mapping_init();
    // Initialize serial communication
    serialprocess_init();
}

uint32_t application_app_bank_size_get(void)
{
    return (uint32_t)(&_MEMPART_FLASH_USER_APP_SIZE);
}

void hisense_app_run(void)
{
    // Process serial communication
    serialprocess_run();
}
