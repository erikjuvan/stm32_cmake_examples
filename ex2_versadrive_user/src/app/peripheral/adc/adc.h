////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     adc.h
 * @brief    Functions for ADC
 * @author   Haris Kovacevic
 * @date     11.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup ADC_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __ADC_H
#define __ADC_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "adc_cfg.h"
#include "stm32g4xx_hal.h"
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Status of ADC initialization
typedef enum
{
    eADC_STATUS_OK = 0,                     /**< No errors */
    eADC_STATUS_ALREADY_INITIALIZED,        /**< ADC module already initialized */
    eADC_STATUS_MODULE_INITIALIZATON_FAIL,  /**< ADC module failed to initialize */
    eADC_STATUS_GROUP_INITIALIZATON_FAIL,   /**< ADC group failed to initialize */
    eADC_STATUS_CHANNEL_INITIALIZATON_FAIL, /**< ADC channel failed to initialize */
    eADC_STATUS_NOT_INITIALIZED,            /**< ADC is not initialized */
    eADC_STATUS_NOT_STARTED,                /**< ADC is not started */
    eADC_STATUS_DMA_NOT_STARTED             /**< DMA is not started */
} adc_status_t;

/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */
typedef struct
{
    float current_u; // injected
    float current_v; // injected
    float current_w; // injected
    float vdc;       // injected
    float ntc_comp;  // regular, DMA
    float ntc_igbt;  // regular, DMA
    float tso_igbt;  // regular, DMA
    float mcu_temp;  // regular, DMA
} adc_data_t;

typedef struct
{
    uint16_t current_u; // injected
    uint16_t current_v; // injected
    uint16_t current_w; // injected
    uint16_t vdc;       // injected
    uint16_t ntc_comp;  // regular, DMA
    uint16_t ntc_igbt;  // regular, DMA
    uint16_t tso_igbt;  // regular, DMA
    uint16_t mcu_temp;  // regular, DMA
} adc_data_raw_t;

/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */

// Enum for queued measurement callback by group
typedef enum
{
    eADC_CALLBACK_INJECTED_CHANNELS_DONE = 0U, /**< Callback for VADC group 0 queue measurement */
    eADC_NUMBER_OF_CALLBACKS
} adc_callback_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// Callback function prototype
typedef void (*adc_callback_fnc_t)(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef    adc_init(void);
adc_status_t         adc_start(void);
adc_status_t         adc_stop(void);
volatile adc_data_t *adc_analog_get(void);
void adc_set_callback(const adc_callback_t callback, const adc_callback_fnc_t callback_fnc);

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
