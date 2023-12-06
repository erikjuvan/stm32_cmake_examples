#ifndef _MU_SAFE_TYPES_H
#define _MU_SAFE_TYPES_H

typedef enum
{
    eDOOR_OPENED = 0,
    eDOOR_CLOSED = 1,
} door_state_t;

typedef enum
{
    eRELAY_LBL_DOOR  = 0, // door lock relay
    eRELAY_LBL_POWER = 1, // main power supply relay
    eRELAY_LBL_NUM   = 2, // number of labels
} relay_lbl_t; // lbl = label

typedef enum
{
    eRELAY_OFF = 0,
    eRELAY_ON  = 1,
} relay_state_t;

/**
* Status of door locking or unlocking.
*/
typedef enum
{
    eDOOR_LOCK_POWERUP      = 0, /**< Powerup procedure ongoing. */
    eDOOR_LOCK_LOCKING      = 1, /**< Locking procedure ongoing. */
    eDOOR_LOCK_TRIAC_LOCK   = 2, /**< Requesting to trigger TRIAC and lock the door. */
    eDOOR_LOCK_LOCKED       = 3, /**< Door is locked. */
    eDOOR_LOCK_UNLOCKING    = 4, /**< Unlocking procedure ongoing. */
    eDOOR_LOCK_TRIAC_UNLOCK = 5, /**< Requesting to trigger TRIAC and unlock the door. */
    eDOOR_LOCK_UNLOCKED     = 6, /**< Door is unlocked. */
    eDOOR_LOCK_ERROR        = 7, /**< Error occurred and further locking and unlocking is prevented. */
} door_lock_state_t;

/**
* Drum spinning detection state.
*/
typedef enum
{
    eDRUMSPIN_STOPPED   = 0, /**< Drum is stopped. */
    eDRUMSPIN_MEASURING = 1, /**< Measurement is ongoing. */
    eDRUMSPIN_ROTATING  = 2, /**< Drum is rotating. */
} drumspin_state_t;

#endif
