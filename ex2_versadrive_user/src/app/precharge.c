/**
 * @file     precharge.c
 * @brief    Precharge procedure
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup PRECHARGE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "precharge.h"
#include "apptime.h"
#include "relay.h"

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
/**
 * Duration of how long the precharge procedure should last in milliseconds
 */
static const apptime_t PRECHARGE_DURATION_MS = 300;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static apptime_t precharge_start_time; //!< Timestamp of when the precharge
                                       //!< procedure starts.

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Start precharge timer
 */
////////////////////////////////////////////////////////////////////////////////
void precharge_start(void)
{
    precharge_start_time = apptime_get_ms();
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    Check if precharge timeout has elapsed, aka precharge is done
 *
 * @return   true if precharge is done, false otherwise
 */
////////////////////////////////////////////////////////////////////////////////
bool precharge_is_done(void)
{
    return apptime_is_elapsed_ms(precharge_start_time, PRECHARGE_DURATION_MS);
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief    Finish the precharge procedure. Note that after the precharge
            is finished relay turns off and bypasses the precharge PTC resistor.
*/
////////////////////////////////////////////////////////////////////////////////
void precharge_finish(void)
{
    relay_req_set(RELAY_PRECHARGE, RELAY_ON);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
