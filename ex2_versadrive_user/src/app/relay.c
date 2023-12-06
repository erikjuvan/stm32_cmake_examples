////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     relay.c
 * @brief    Relay control.
 * @author   Matej Otic
 * @date     01.07.2022
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup RELAY
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "relay.h"
#include "gpio/gpio.h"
#include "stm32g4xx.h"
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Relay control structure.
 */
typedef struct
{
    volatile relay_state_t req_state; /**< Requested state. */
    volatile bool          is_req;    /**< Is request active? */

    struct
    {
        GPIO_TypeDef *port;
        uint16_t      pin;
    };

} relay_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Relay control instance
 */
static relay_t g_relay[NUM_OF_RELAYS] = {
    [RELAY_PRECHARGE] =
        {.req_state = RELAY_OFF,
                           .is_req    = false,
                           .port      = GPIO_PORT_PRECHARGE_RELAY,
                           .pin       = GPIO_PIN_PRECHARGE_RELAY},
};

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * Set actual GPIO that controls switching the relay.
 *
 * @param[in]    relay   Relay.
 * @param[in]    state   State the relay should be set to.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void relay_set(relay_id_t relay, relay_state_t state)
{
    HAL_GPIO_WritePin(
        g_relay[relay].port, g_relay[relay].pin, state == RELAY_ON ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Process relay request to switch to state.
 *
 * @param[in]    relay   Relay.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
static inline void relay_proc_req(relay_id_t relay)
{
    if (g_relay[relay].is_req)
    {
        g_relay[relay].is_req = false;
        relay_set(relay, g_relay[relay].req_state);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup RELAY_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * Request relay to switch to state.
 * If relay is requested to turn ON software shall wait for zero-cross.
 * If relay is requested to turn OFF the relay shall be switched immediately.
 *
 * @param[in]    relay   Relay.
 * @param[in]    state   New state.
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void relay_req_set(relay_id_t relay, relay_state_t state)
{
    // Set state as the first thing in this function due to the following case:
    // 1. Relay ON request is ongoing
    // 2. This function is called and OFF is requested
    // 3. State is set to OFF
    // 3. Zero-cross IRQ is triggered
    // 4. Relay is switched to OFF through zero-cross
    // 5. Relay is then again switched to OFF through this function directly
    // In this case we get rid of turning the relay ON first (zero-cross) and then OFF (this
    // function) by turning it OFF two times
    g_relay[relay].req_state = state;

    if (state == RELAY_ON)
    {
        g_relay[relay].is_req = true;
    }
    else
    {
        g_relay[relay].is_req = false; // Cancel ongoing ON request
        relay_set(relay, state);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Request SAFE to switch relay state to previously provided.
 *
 * @note This function shall be called when zero-cross is detected.
 *
 * @return       None.
 */
////////////////////////////////////////////////////////////////////////////////
void relay_proc_zero_cross(void)
{
    relay_proc_req(RELAY_PRECHARGE);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Get state of the relay via actual GPIO value.
 *
 * @param[in]    relay   Relay.
 * @return       Relay state.
 */
////////////////////////////////////////////////////////////////////////////////
relay_state_t relay_get_state(relay_id_t relay)
{
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(g_relay[relay].port, g_relay[relay].pin);

    return pin_state == GPIO_PIN_SET ? RELAY_ON : RELAY_OFF;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
