////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     relay.h
 * @brief    Relay control.
 * @author   Matej Otic
 * @date     01.07.2022
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup RELAY_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __RELAY_H
#define __RELAY_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    RELAY_PRECHARGE = 0,

    NUM_OF_RELAYS
} relay_id_t;

typedef enum
{
    RELAY_OFF = 0,
    RELAY_ON  = 1,
} relay_state_t;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void          relay_req_set(relay_id_t relay, relay_state_t state);
void          relay_proc_zero_cross(void);
relay_state_t relay_get_state(relay_id_t relay);

#endif // __RELAY_H
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
