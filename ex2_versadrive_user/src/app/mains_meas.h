////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     mains_meas.h
 * @brief    Mains power supply measurement.
 * @author   Haris Kovacevic
 * @date     10.03.2022
 * @version  V1.0.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MAINS_MEAS_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAINS_MEAS_H
#define __MAINS_MEAS_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Power-line frequency.
 */
typedef enum
{
    eMAINS_MEAS_POWERLINE_50HZ    = 0, /**< 50 Hz nominal frequency. */
    eMAINS_MEAS_POWERLINE_60HZ    = 1, /**< 60 Hz nominal frequency. */
    eMAINS_MEAS_POWERLINE_UNKNOWN = 2  /**< Unknown frequency. */
} mains_meas_powerline_freq_t;

/**
 * Module status.
 */
typedef enum
{
    eMAINS_MEAS_ERR_OK          = 0x00, /**< Action successfully completed. */
    eMAINS_MEAS_ERR_MAIN_TIMER  = 0x01, /**< Failed to initialize main timer. */
    eMAINS_MEAS_ERR_DELAY_TIMER = 0x02, /**< Failed to initialize zero-cross delay timer. */
    eMAINS_MEAS_ERR_CBK_NULL    = 0x04, /**< Callback function is null. */
    eMAINS_MEAS_ERR_MAX_CBKS_REGISTERED =
        0x08, /**< Maximum number of callback functions already registered. */
} mains_meas_err_t;

/**
 * Power fail state.
 */
typedef enum
{
    eMAINS_MEAS_POWERFAIL_NOT_ACTIVE = 0, /**< Power fail is not active. */
    eMAINS_MEAS_POWERFAIL_ACTIVE /**< Power fail is active when 50/60 Hz power line is not present.
                                  */
} mains_meas_powerfail_t;

/**
 * Typedef for zero-cross callback function.
 */
typedef void (*pfnc_zero_cross_cbk)(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
mains_meas_err_t mains_meas_init(void);
mains_meas_err_t mains_meas_register_neg_zero_cross_cbk(pfnc_zero_cross_cbk fnc);

mains_meas_powerfail_t mains_meas_get_powerfail_state(void);
void                   mains_meas_clear_powerfail(void);

mains_meas_powerline_freq_t mains_meas_get_powerline_freq(void);
uint16_t mains_meas_get_freq_hz_100(void);

#endif // __MAINS_MEAS_H
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
