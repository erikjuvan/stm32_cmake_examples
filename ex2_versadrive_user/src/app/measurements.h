////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/*!
 * @file     measurements.h
 * @note
 * @author   Marko Rman
 * @date     27.12.2021
 * @brief    Header file for inertia and unbalance measurements
 */
////////////////////////////////////////////////////////////////////////////////
#ifndef __MEASUREMENTS_H
#define __MEASUREMENTS_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "app.h"
#include "hisense_proto.h"

////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////
/**
 * Enumeration for measurement status
 */
typedef enum
{
    eMEASUREMENT_IDLE        = 0, /**< no measurement command issued */
    eMEASUREMENT_IN_PROGRESS = 1, /**< measurement in progress */
} measurement_state_t;

typedef enum
{
    eMEAS_NONE = 0,   /**< No measurements */
    eMEAS_CONST_LOW,  /**< Measurement at constant 75 RPM */
    eMEAS_WAVE_LOW,   /**< Measurement at 75-100-75 RPM wave */
    eMEAS_CONST_HIGH, /**< Measurement at constant 100 RPM */
    eMEAS_WAVE_HIGH,  /**< Measurement at 100-125-100 RPM wave */
    eMEAS_CUSTOM,     /**< Custom measurement */
} measurement_type_t;

typedef struct
{
    // Last measurements
    float32_t raw_inertia;
    float32_t raw_unbalance;
    float32_t raw_friction;
    float32_t raw_au_ball;
    float32_t overload;

    float32_t inertia;
    float32_t unbalance;
    float32_t au_ball;
} measurements_t;

/**
 * Structure holding measurements module data
 */
typedef struct
{
    measurement_state_t state;   /**< Current state of Measurements */
    measurement_type_t  type;    /**< What type of measurement are we currently performing */
    measurements_t      values;  /**< Measurement values*/
    bool                success; /**< Flag if last measurement was successful or not */
    bool                request; /**< Pending request for start of measurement */
} measurements_data_t;

/**
 * Enumeration for machine types
 */
typedef enum
{
    eMACHINE_TYPE_NONE     = 0,
    eMACHINE_TYPE_1_838535 = 1, // 60/10-1600 BM-33
    eMACHINE_TYPE_2_825308 = 2, // 54/8-1400 BM-22
    eMACHINE_TYPE_3_825673 = 3, // 49/8-1400 BM-22
    eMACHINE_TYPE_4_828601 = 4,
    eMACHINE_TYPE_5_600235 = 5, // 60/9-1600 BM-33
    eMACHINE_TYPE_6_825228 = 6, // 60/9-1400 BM-33
    eMACHINE_TYPE_7_838609 = 7, // 54/8-1600 BM-33
    eMACHINE_TYPE_NUM      = 8,
} wm_type_t;

/**
 * Machine configuration
 */
typedef struct
{
    uint16_t           start_rpm;
    uint16_t           amplitude;
    uint16_t           turns;
    measurement_type_t type;
} measurement_config_t;

/**
 * Structure for inertia values
 */
typedef struct
{
    float32_t       zero_inertia;
    const float32_t p10;
} inertia_factors_t;

/**
 * Structure for unbalance values
 */
typedef struct
{
    float32_t       zero_unbalance;
    const float32_t p01;
    const float32_t p11;
    const float32_t p21;
} unbalance_factors_t;

/**
 * Structure for machine type
 */
typedef struct
{
    float32_t           half_load_raw_inertia;
    inertia_factors_t   inertia_low;
    inertia_factors_t   inertia_high;
    unbalance_factors_t unbalance_low_const;
    unbalance_factors_t unbalance_low_wave;
    unbalance_factors_t unbalance_high_const;
    unbalance_factors_t unbalance_high_wave;
} wm_factors_t;

/**
 * Machine settings
 */
typedef struct
{
    wm_type_t type;       /**< Selected type of machine */
    bool      configured; /**< Configured flag */
} wm_config_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
bool measurements_check_request(const command_parser_start_measure_t *const p_measure_req);
void measurements_handle(void);

measurement_state_t              measurements_get_state(void);
const measurements_data_t *const measurements_get_data(void);
void                             measurements_cancel(void);

bool measurements_set_machine(const command_parser_machine_set_t *const machine_set_packet);
const wm_config_t *const  measurements_get_machine(void);
const wm_factors_t *const measurements_get_factors(wm_type_t type);
bool                      measurements_is_machine_set(void);

#endif // __MEASUREMENTS_H
