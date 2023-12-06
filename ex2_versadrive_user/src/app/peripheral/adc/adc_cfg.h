////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     adc_cfg.h
 * @brief    Functions for adc configuration module
 * @author   Haris Kovacevic
 * @date     12.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup ADC_CFG_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __ADC_CFG_H
#define __ADC_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* parasoft-begin-suppress FORMAT-02 FORMAT-03 FORMAT-34 "Allow for defines." */
// ADC voltage reference in [ V ]
#define ADC_VOLTAGE_REFERENCE ((float)3.3)
// ADC voltage offset for current measurement in ADC quants due to external circuit
// and opamp offset
#define ADC_VOLT_OFFS_QUANT   ((int16_t)2012)

// Shunt resistance for current measurement in [ ohm ]
#define ADC_SHUNT_RESISTANCE  (0.05f)
// ADC resolution in [ bits ]
#define ADC_RESOLUTION        (12U)
// ADC maximum output value
#define ADC_MAX_VAL           (((uint16_t)1U << ADC_RESOLUTION) - 1U)
// Voltage to current gain ratio
#define ADC_CURRENT_GAIN_AV   (3.508772f)

#if 1 // ST IGBT bridge module

// Temperature offset
#define ADC_TEMP_OFFSET      (0.7f)
#define ADC_TEMP_OFFS_QUANTS ((int16_t)868)
// Temperature gain
#define ADC_TEMP_GAIN        (0.0184f)

#elif 0 // BM64374S-VA IGBT module

// Temperature offset
#define ADC_TEMP_OFFSET      (0.495)
#define ADC_TEMP_OFFS_QUANTS ((int16_t)614)
// Temperature gain
#define ADC_TEMP_GAIN        (0.0252f)

#endif

/// Factor for calculating raw ADC value to voltage
#define ADC_RAW_TO_U ((float)(3.3f / 4095.0f))

// Voltage gain for DC link voltage
#define ADC_VOLTAGE_GAIN \
    ((float)((360000.0f + 360000.0f + 360000.0f + 360000.0f + 10000.0f) / 10000.0f))
#define ADC_CURR_CONV_FACT  ((float)(ADC_RAW_TO_U / (ADC_CURRENT_GAIN_AV * ADC_SHUNT_RESISTANCE)))
#define ADC_VOLTS_CONV_FACT ((float)(ADC_RAW_TO_U * ADC_VOLTAGE_GAIN))
#define ADC_TEMP_CONV_FACT  ((float)(ADC_RAW_TO_U / ADC_TEMP_GAIN))

/* parasoft-end-suppress FORMAT-02 FORMAT-03 FORMAT-34 "Allow for defines." */

#endif
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
