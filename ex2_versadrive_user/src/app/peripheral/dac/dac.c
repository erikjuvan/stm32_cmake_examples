////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     dac.c
 * @brief    Functions for DAC
 * @author   Haris Kovacevic
 * @date     10.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup DAC_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "dac.h"
#include "adc/adc_cfg.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// DAC voltage reference in [ V ]
#define DAC_VREF               (3.3f)
// DAC maximum value
#define DAC_MAX_VAL            (4095.0f)

// Current protection for phase currents in [ A ]
#define DAC_CURRENT_LIMIT_AMPS (1.5)
// Voltage offset on the (+) pin of OPAMP while current is equal to zero ( 0.405 V approx. 502 DAC
// quants )
#define DAC_CURRENT_OFFSET     (4095.0f * (1400.0f / (1400.0f + 10000.0f)))
// Shunt resistance ( 50 mOhm )
#define DAC_SHUNT_RESISTANCE   (0.05f)

// Current protection for phase currents in [ ADC quants ] with input parameter
#define DAC_CURRENT_LIMIT(LIMIT)                                                                  \
    ((uint16_t)(DAC_CURRENT_OFFSET + (((float)LIMIT * DAC_SHUNT_RESISTANCE) * (4095.0f / 3.3f)) + \
                12U))

// DC link voltage protection in [ ADC quants ]
#define DAC_VOLTAGE_LIMIT(LIMIT) ((LIMIT / ADC_VOLTAGE_GAIN) * (DAC_MAX_VAL / DAC_VREF))

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
DAC_HandleTypeDef g_hdac1;
DAC_HandleTypeDef g_hdac3;

/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */
/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = protections__phase_overcurrent_hw
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 10]
@! DESCRIPTION  = "Over current protection limit using internal comparators (HW protection)"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 5
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 8.0
@! END
*/
volatile float protections__phase_overcurrent_hw = 8.0f;

/*
@! SYMBOL       = protections__phase_overvoltage_hw
@! A2L_TYPE     = PARAMETER
@! DATA_TYPE    = FLOAT [0 ... 450]
@! DESCRIPTION  = "Over voltage protection limit using internal comparators (HW protection)"
@! GROUP        = protections
@! DIMENSION    = 1 1
@! UNIT         = "\"
@! CAN_INDEX    = 0x5003
@! CAN_SUBINDEX = 6
@! READ_LEVEL   = 5
@! WRITE_LEVEL  = 5
@! PERSIST      = 1
@! DEFAULT      = 410.0
@! END
*/
volatile float protections__phase_overvoltage_hw = 410.0f;

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef dac1_init(void);
static HAL_StatusTypeDef dac3_init(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes DAC
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef dac_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    if ((dac1_init() != HAL_OK) || (dac3_init() != HAL_OK))
    {
        init_status = HAL_ERROR;
    }

    // Set over current and over voltage thresholds
    HAL_DAC_SetValue(
        &g_hdac1, (uint32_t)DAC_CHANNEL_1, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );
    HAL_DAC_SetValue(
        &g_hdac1, (uint32_t)DAC_CHANNEL_2, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );

    HAL_DAC_SetValue(
        &g_hdac3, (uint32_t)DAC_CHANNEL_1, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_VOLTAGE_LIMIT(protections__phase_overvoltage_hw)
    );
    HAL_DAC_SetValue(
        &g_hdac3, (uint32_t)DAC_CHANNEL_2, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );

    // Start DAC1
    HAL_DAC_Start(&g_hdac1, (uint32_t)DAC_CHANNEL_1);
    HAL_DAC_Start(&g_hdac1, (uint32_t)DAC_CHANNEL_2);
    // Start DAC3
    HAL_DAC_Start(&g_hdac3, (uint32_t)DAC_CHANNEL_1);
    HAL_DAC_Start(&g_hdac3, (uint32_t)DAC_CHANNEL_2);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes DAC1
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef dac1_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    DAC_ChannelConfTypeDef sConfig = {0};

    // Enable clock for DAC1
    __HAL_RCC_DAC1_CLK_ENABLE();

    g_hdac1.Instance = DAC1;

    if (HAL_DAC_Init(&g_hdac1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfig.DAC_HighFrequency           = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
    sConfig.DAC_DMADoubleDataMode       = DISABLE;
    sConfig.DAC_SignedFormat            = DISABLE;
    sConfig.DAC_SampleAndHold           = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger                 = DAC_TRIGGER_NONE;
    sConfig.DAC_Trigger2                = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer            = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
    sConfig.DAC_UserTrimming            = DAC_TRIMMING_FACTORY;

    if (HAL_DAC_ConfigChannel(&g_hdac1, &sConfig, (uint32_t)DAC_CHANNEL_1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfig.DAC_OutputBuffer            = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;

    if (HAL_DAC_ConfigChannel(&g_hdac1, &sConfig, (uint32_t)DAC_CHANNEL_2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes DAC3
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef dac3_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    DAC_ChannelConfTypeDef sConfig = {0};

    // Enable clock for DAC3
    __HAL_RCC_DAC3_CLK_ENABLE();

    g_hdac3.Instance = DAC3;

    if (HAL_DAC_Init(&g_hdac3) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfig.DAC_HighFrequency           = DAC_HIGH_FREQUENCY_INTERFACE_MODE_AUTOMATIC;
    sConfig.DAC_DMADoubleDataMode       = DISABLE;
    sConfig.DAC_SignedFormat            = DISABLE;
    sConfig.DAC_SampleAndHold           = DAC_SAMPLEANDHOLD_DISABLE;
    sConfig.DAC_Trigger                 = DAC_TRIGGER_NONE;
    sConfig.DAC_Trigger2                = DAC_TRIGGER_NONE;
    sConfig.DAC_OutputBuffer            = DAC_OUTPUTBUFFER_DISABLE;
    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;
    sConfig.DAC_UserTrimming            = DAC_TRIMMING_FACTORY;

    if (HAL_DAC_ConfigChannel(&g_hdac3, &sConfig, (uint32_t)DAC_CHANNEL_1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_INTERNAL;

    if (HAL_DAC_ConfigChannel(&g_hdac3, &sConfig, (uint32_t)DAC_CHANNEL_2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function updates limit for over current protection
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void dac_protection_limit_update(void)
{
    // Set over current and over voltage thresholds
    HAL_DAC_SetValue(
        &g_hdac1, (uint32_t)DAC_CHANNEL_1, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );
    HAL_DAC_SetValue(
        &g_hdac1, (uint32_t)DAC_CHANNEL_2, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );

    HAL_DAC_SetValue(
        &g_hdac3, (uint32_t)DAC_CHANNEL_2, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_CURRENT_LIMIT(protections__phase_overcurrent_hw)
    );
    HAL_DAC_SetValue(
        &g_hdac3, (uint32_t)DAC_CHANNEL_1, (uint32_t)DAC_ALIGN_12B_R,
        (uint32_t)DAC_VOLTAGE_LIMIT(protections__phase_overvoltage_hw)
    );
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
