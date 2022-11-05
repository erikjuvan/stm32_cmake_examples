#ifndef _DRAIN_COMMON_H_
#define _DRAIN_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Public definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Possible states of drain pump.
 */
typedef enum
{
    eDRAIN_OFF = 0, /**< Drain pump is turned off. */
    eDRAIN_ON  = 1  /**< Drain pump is turned on. */
} drain_state_t;


#ifdef __cplusplus
}
#endif

#endif
