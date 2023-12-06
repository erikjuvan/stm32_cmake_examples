////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     reset_status_log_common.h
* @brief    Common file for logging of reset status register.
* @author   Dusan Slavinec
* @date     15.06.2022
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup RESET_STATUS_LOG_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __RESET_STATUS_LOG_COMMON_H
#define __RESET_STATUS_LOG_COMMON_H


#include "mu_safe_common.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
* MCU reset logging checkpoints.
*/
typedef enum __attribute__((packed))
{
    eRESET_STATUS_LOG_CHKPT_UNKNOWN        = 0,  /**< Unknown. */
    eRESET_STATUS_LOG_CHKPT_STL_STARTUP    = 1,  /**< STL startup test. */
    eRESET_STATUS_LOG_CHKPT_STL_RUNTIME    = 2,  /**< STL runtime test. */
    eRESET_STATUS_LOG_CHKPT_SAFE_INIT      = 3,  /**< SAFE software initialization. */
    eRESET_STATUS_LOG_CHKPT_USER_INIT      = 4,  /**< USER software initialization. */
    eRESET_STATUS_LOG_CHKPT_SAFE_SUPERLOOP = 5,  /**< SAFE software superloop. */
    eRESET_STATUS_LOG_CHKPT_USER_SUPERLOOP = 6,  /**< USER software superloop. */
    eRESET_STATUS_LOG_CHKPT_HARDFAULT      = 7,  /**< Hard fault handler. */
    eRESET_STATUS_LOG_CHKPT_NMI            = 8,  /**< NMI handler. */
    eRESET_STATUS_LOG_CHKPT_MEMMANAGE      = 9,  /**< Memory manager handler. */
    eRESET_STATUS_LOG_CHKPT_BUSFAULT       = 10, /**< Bus fault handler. */
    eRESET_STATUS_LOG_CHKPT_USAGEFAULT     = 11, /**< Usage fault handler. */
    eRESET_STATUS_LOG_CHKPT_SVC            = 12, /**< SVC handler. */
    eRESET_STATUS_LOG_CHKPT_DEBUGMON       = 13, /**< Debug monitor handler. */
    eRESET_STATUS_LOG_CHKPT_PENDSV         = 14, /**< Pending SV handler. */
} reset_status_log_chkpt_t;

/**
* MCU reset cause flags.
*/
typedef enum __attribute__((packed))
{
    eRESET_STATUS_LOG_CAUSE_NONE     = 0x00,    /**< No reset flag is set. */
    eRESET_STATUS_LOG_CAUSE_OBLRSTF  = 0x01,    /**< Option byte loader reset flag. */
    eRESET_STATUS_LOG_CAUSE_PINRSTF  = 0x02,    /**< Pin reset flag. */
    eRESET_STATUS_LOG_CAUSE_BORRSTF  = 0x04,    /**< BOR reset flag. */
    eRESET_STATUS_LOG_CAUSE_SFTRSTF  = 0x08,    /**< Software reset flag. */
    eRESET_STATUS_LOG_CAUSE_IWDGRSTF = 0x10,    /**< Independent window watchdog reset flag. */
    eRESET_STATUS_LOG_CAUSE_WWDGRSTF = 0x20,    /**< Window watchdog reset flag. */
    eRESET_STATUS_LOG_CAUSE_LPWRRSTF = 0x40,    /**< Low-power reset flag. */
} reset_status_log_cause_t;

/**
 * Reset log entry structure.
 */
typedef struct
{
    union
    {
        struct
        {
            reset_status_log_chkpt_t chkpt;             /**< Last reached checkpoint. */
            reset_status_log_cause_t cause;             /**< Reset cause flags. */
            mu_safe_fsm_t            mu_safe_state;     /**< MU Safe FSM state. */
            uint8_t reserved;                           /**< Reserved; not used. */
        } info;
        uint32_t u_info;                                /**< Union of info struct. */
    };
    uint32_t time_chkpt;            /**< Time [ms] at which the checkpoint was reached. */
} reset_status_log_entry_t;

#endif // __RESET_STATUS_LOG_COMMON_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
