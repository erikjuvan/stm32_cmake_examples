////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 *@file      performance_meas.c
 *@brief     API for performance measurement
 *@author    Martin Pec
 *@author    Haris Kovacevic
 *@date      25.05.2021
 *@version   V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup PERFORMANCE_MEAS
 * @{ <!-- BEGIN GROUP -->
 *
 * Following module is part of performance module
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "project_config.h"
#include <performance_meas.h>
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#define ELAPSED_TIME_MAX_SECTIONS 2

typedef struct elapsed_time
{
    uint32_t start;
    uint32_t current;
    uint32_t max;
    uint32_t min;
} elapsed_time_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
volatile uint32_t *pg_DWT_CYCCNT  = (volatile uint32_t *)0xE0001004U;
volatile uint32_t *pg_DWT_CONTROL = (volatile uint32_t *)0xE0001000U;
volatile uint32_t *pg_SCB_DEMCR   = (volatile uint32_t *)0xE000EDFCU;

static volatile elapsed_time_t g_elapsed_time_tbl[ELAPSED_TIME_MAX_SECTIONS];

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Clear the measurements stats
 *
 * @param[in]    i            Index
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void performance_meas_clr(const uint32_t i)
{
    volatile elapsed_time_t *p_tbl;

    p_tbl          = &g_elapsed_time_tbl[i];
    p_tbl->start   = 0;
    p_tbl->current = 0;
    p_tbl->min     = 0xFFFFFFFFU;
    p_tbl->max     = 0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Initialization of performance measurement module
 * @notes        Must be called before any of the other functions in this module
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void performance_meas_init(void)
{
    uint32_t i;

    // See if DWT is available
    if (*pg_DWT_CONTROL != 0)
    {
        *pg_SCB_DEMCR   |= 1UL << 24; // Set bit 24
        *pg_DWT_CYCCNT   = 0;
        *pg_DWT_CONTROL |= 1 << 0; // Set bit 0
    }

    for (i = 0; i < ELAPSED_TIME_MAX_SECTIONS; i++)
    {
        performance_meas_clr(i);
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Start the measurement of a code section
 *
 * @param[in]    i        Index
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void performance_meas_start(const uint32_t i)
{
    g_elapsed_time_tbl[i].start = *pg_DWT_CYCCNT;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Stop the measurement of a code section and compute stats
 *
 * @param[in]    i        Index
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void performance_meas_stop(const uint32_t i)
{
    uint32_t                 stop;
    volatile elapsed_time_t *p_tbl;

    stop           = *pg_DWT_CYCCNT;
    p_tbl          = &g_elapsed_time_tbl[i];
    p_tbl->current = stop - p_tbl->start;

    if (p_tbl->max < p_tbl->current)
    {
        p_tbl->max = p_tbl->current;
    }

    if (p_tbl->min > p_tbl->current)
    {
        p_tbl->min = p_tbl->current;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function calculates time duration in [us] for specified measurement.
 *
 * @param[in]        None
 * @return           None
 */
////////////////////////////////////////////////////////////////////////////////
float performance_meas_calc_time(const uint32_t i
) /* parasoft-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */
{
    return (g_elapsed_time_tbl[i].current / 170.0f);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
