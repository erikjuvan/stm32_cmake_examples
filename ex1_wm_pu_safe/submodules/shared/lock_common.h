#ifndef _LOCK_COMMON_H_
#define _LOCK_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif


////////////////////////////////////////////////////////////////////////////////
// Public definitions
////////////////////////////////////////////////////////////////////////////////
/**
 * Possible measured states of lock.
 */
typedef enum
{
    eLOCK_UNKNOWN  = 0, /**< Door state is not known. */
    eLOCK_UNLOCKED = 1, /**< Door is unlocked. */
    eLOCK_LOCKED   = 2  /**< Door is locked. */
} lock_state_t;

/**
 * Possible measured states of lock.
 */
typedef enum
{
    eLOCK_REQ_NONE_RESP_NONE                            = 0x00, /**< For description check black_channel_common.h */
    eLOCK_REQ_LOCK_RESP_LOCK                            = 0x01, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_ULCK                            = 0x02, /**< For description check black_channel_common.h */
    eLOCK_REQ_LOCK_RESP_IN_PROGRESS                     = 0x03, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_IN_PROGRESS                     = 0x04, /**< For description check black_channel_common.h */
    eLOCK_REQ_LOCK_RESP_LOCKED                          = 0x05, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_UNLOCKED                        = 0x06, /**< For description check black_channel_common.h */

    eLOCK_REQ_LOCK_RESP_DENIED_NOT_UNLOCKED             = 0x80, /**< For description check black_channel_common.h */
    eLOCK_REQ_LOCK_RESP_DENIED_IMPOSSIBLE               = 0x81, /**< For description check black_channel_common.h */
    eLOCK_REQ_LOCK_RESP_DENIED_NO_TRIAC_TRIG            = 0x82, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_DENIED_DOOR_NOT_LOCKED          = 0x83, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_DENIED_SPINNING                 = 0x84, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_DENIED_UNLOCK_IMPOSSIBLE        = 0x85, /**< For description check black_channel_common.h */
    eLOCK_REQ_ULCK_RESP_DENIED_EPS_HIGH                 = 0x86, /**< For description check black_channel_common.h */

    // Non black channel equivalents (PU specific)
    eLOCK_REQ_LOCK_RESP_DENIED_NO_UI_START              = 0x8F,  /**< Start button was not pressed before door locked was requested */
} lock_response_t;

/**
 * Possible TRIAC states.
 */
typedef enum
{
    eLOCK_TRIAC_OFF = 0, /**< TRIAC is turned off. */
    eLOCK_TRIAC_ON  = 1  /**< TRIAC is turned on. */
} lock_triac_t;

#ifdef __cplusplus
}
#endif

#endif
