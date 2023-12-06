////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2022
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     otdet_common.h
* @brief    Shared over-temperature detection definitions.
* @author   Matej Otic
* @date     14.03.2022
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup OTDET_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __OTDET_COMMON_H
#define __OTDET_COMMON_H

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Number of temperature samples per channel #otdet_temp_ch_t.
 */
#define OTDET_SAMPLE_PER_CH            ( 32 )


/**
 * 32-bit floating point type.
 */
typedef float float32_t;

/**
 * Temperature measurement channels.
 */
typedef enum
{
    eOTDET_TEMP_CH_NTC_COMP = 0, /**< NTC on compressor */
    eOTDET_TEMP_CH_NTC_IGBT = 1, /**< NTC in IGBT */
    eOTDET_TEMP_CH_TSO_IGBT = 2, /**< TSO in IGBT */
    eOTDET_TEMP_CH_MCU_TEMP = 3, /**< MCU internal */

    eOTDET_TEMP_CH_NUM       /**< Number of measurement channels. */
} otdet_temp_ch_t;

/**
 * Over-temperature detection status.
 */
typedef enum
{
    eOTDET_OK              = 0x00,  /**< Measurement is OK. */
    eOTDET_PWRUP_IMPLAUS   = 0x01,  /**< Powerup temperature difference is implausible. */
    eOTDET_RUNTIME_IMPLAUS = 0x02,  /**< IGBT temperature during runtime is implausible. */
    eOTDET_TEMP_IMPLAUS    = 0x04,  /**< Temperature is out of plausible range. */
    eOTDET_ISR_INOPER      = 0x08,  /**< DMA ISR is inoperational. */
} otdet_status_t;

/**
 * Measured temperature on channels #otdet_temp_ch_t.
 */
typedef struct
{
    float32_t sensor[eOTDET_TEMP_CH_NUM];   /**< Channel (sensor). */
} otdet_temps_t;

#endif // __OTDET_COMMON_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
