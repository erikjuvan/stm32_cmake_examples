////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     curr_meas_common.h
* @brief    Shared current measurements definitions.
* @author   Amadej Arnus
* @date     13.04.2022
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup CURR_MEAS_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CURR_MEAS_COMMON_H
#define __CURR_MEAS_COMMON_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * 32-bit floating point type.
 */
typedef float float32_t;

/**
 * Current measurement status.
 */
typedef enum
{
    eCURR_MEAS_OK          = 0x00,  /**< Measurement is OK. */
    eCURR_MEAS_INVALID     = 0x01,  /**< Invalid ADC sampling point. */
    eCURR_MEAS_INCORRECT   = 0x02,  /**< Incorrect ADC ISR trigger rate (FOC frequency). */
} curr_meas_status_t;

/**
 * Phase index used in #curr_meas_phases_t.
 */
typedef enum
{
    eCURR_MEAS_PHASE_U   = 0,   /**< Phase U selected. */
    eCURR_MEAS_PHASE_V   = 1,   /**< Phase V selected. */
    eCURR_MEAS_PHASE_W   = 2,   /**< Phase W selected. */

    eCURR_MEAS_PHASE_NUM = 3    /**< Number of phases. */
} curr_meas_phase_idx_t;

/**
 * Phases measurement.
 */
typedef struct
{
    float32_t phase[eCURR_MEAS_PHASE_NUM];  /**< Phase. */
} curr_meas_phases_t;

#endif /* CURR_MEAS_COMMON_H_ */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
