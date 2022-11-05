/**
 * @file     safe_info.c
 * @brief    Information provider about some internal SAFE variables needed by USER code.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup SAFE_INFO
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "safe_info.h"
#include "project_config.h"

////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern const uint32_t _Safe_Crc;  /**< Location of SAFE CRC. Linker script variable. */


////////////////////////////////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////////////////////////////////
/**
 * Instance of safe_info_t struct.
 */
safe_info_t g_safe_info =
{
    .power_relay_closed = false,
    .door_relay_closed = false,

    .emrgcy_drain_active = false,

    .version =
    {
        .major = CONF_VERSION_MAJOR,
        .minor = CONF_VERSION_MINOR,
        .patch = CONF_VERSION_PATCH
    },

    .crc32 = CONF_VERSION_CRC32,
};


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialize safe info members that can only be initialized at runtime, and are not known at compile time.
 *
 * @return       None.
 */
void safe_info_init(void)
{
    g_safe_info.crc32 = *(uint32_t *)(&_Safe_Crc);
}

/**
 * Get safe info struct.
 *
 * @return     Constant pointer to the safe info struct, so USER code can read, but not modify it.
 */
const safe_info_t* safe_info_get(void)
{
    return &g_safe_info;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
