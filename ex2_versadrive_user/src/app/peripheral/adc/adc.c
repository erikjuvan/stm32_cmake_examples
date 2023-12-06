////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     adc.c
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

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "adc.h"
#include "func_table.h"
#include "math.h"
#include "performance_meas.h"
#include "priorities.h"
#include "timer/timer.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// ADC channels
// ADC 1 channels
#define ADC_CHANNEL_SHUNT_A           ADC_CHANNEL_13 // OPAMP1 = PA1 
#define ADC_CHANNEL_VDC               ADC_CHANNEL_1  // PA0
#define ADC_CHANNEL_NTC_COMP          ADC_CHANNEL_12 // PB1
#define ADC_CHANNEL_NTC_IGBT          ADC_CHANNEL_5  // PB14
#define ADC_CHANNEL_TSO_IGBT          ADC_CHANNEL_3  // PA2
#define ADC_CHANNEL_MCU_TEMP          ADC_CHANNEL_TEMPSENSOR_ADC1
// ADC 2 channels
#define ADC_CHANNEL_SHUNT_B           ADC_CHANNEL_16 // OPAMP2 = PA7
#define ADC_CHANNEL_SHUNT_C           ADC_CHANNEL_18 // OPAMP3 = PB0

// ADC pinout
#define GPIO_PIN_ADC_VDC            GPIO_PIN_0
#define GPIO_PORT_ADC_VDC           GPIOA

#define GPIO_PIN_ADC_NTC_COMP       GPIO_PIN_1
#define GPIO_PORT_ADC_NTC_COMP      GPIOB

#define GPIO_PIN_ADC_NTC_IGBT       GPIO_PIN_14
#define GPIO_PORT_ADC_NTC_IGBT      GPIOB

#define GPIO_PIN_ADC_TSO_IGBT       GPIO_PIN_2
#define GPIO_PORT_ADC_TSO_IGBT      GPIOA

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
ADC_HandleTypeDef         g_hadc1;
ADC_HandleTypeDef         g_hadc2;
extern EXTI_HandleTypeDef g_hexti;

// Callback for queue channels
static adc_callback_fnc_t _adc_callback[eADC_NUMBER_OF_CALLBACKS] = {NULL};

static volatile uint16_t *_dma_buffer = NULL;

static volatile adc_data_t _adc_data = {0};
static volatile adc_data_raw_t _adc_data_raw = {0};

/* parasoft-begin-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */
/* parasoft-begin-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/*
@! SYMBOL       = motor_control__heatsink_temp
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Bridge heatsink temperature temperature [ °C ]"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "°C"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 8
@! READ_LEVEL   = 4
@! END
*/
volatile float motor_control__heatsink_temp = 0.0f;

/*
@! SYMBOL       = motor_control__internal_ntc_temp
@! A2L_TYPE     = MEASURE
@! DATA_TYPE    = FLOAT
@! DESCRIPTION  = "Bridge internal NTC temperature temperature [ C ]"
@! GROUP        = motor_control
@! DIMENSION    = 1 1
@! UNIT         = "C"
@! CAN_INDEX    = 0x5000
@! CAN_SUBINDEX = 9
@! READ_LEVEL   = 4
@! END
*/
volatile float motor_control__internal_ntc_temp = 0.0f;

/* parasoft-end-suppress EMSISO-GLOBAL "Flow variable, not applicable." */
/* parasoft-end-suppress MISRAC2012-DIR_4_6-b "Ignore rule." */

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef adc1_init(void);
static HAL_StatusTypeDef adc2_init(void);
static void              adc_gpio_init(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes ADC
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef adc_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    _dma_buffer = (uint16_t *)pg_safe_func_table->fp_Get_ADC1_DMA1Ch1_Buffer_Addr();

    // Enable ADC clock
    __HAL_RCC_ADC12_CLK_ENABLE();

    // Initialize GPIOs as analog
    adc_gpio_init();

    init_status |= adc1_init();
    init_status |= adc2_init();
    init_status |= HAL_ADCEx_Calibration_Start(&g_hadc2, ADC_SINGLE_ENDED);
    init_status |= HAL_ADCEx_Calibration_Start(&g_hadc1, ADC_SINGLE_ENDED);

    // ADC12 enable interrupt
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC1_2_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes ADC1
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef adc1_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    ADC_MultiModeTypeDef     multimode       = {0};
    ADC_ChannelConfTypeDef   sConfig         = {0};
    ADC_InjectionConfTypeDef sConfigInjected = {0};

    g_hadc1.Instance                   = ADC1;
    g_hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    g_hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    g_hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    g_hadc1.Init.GainCompensation      = 0;
    g_hadc1.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    g_hadc1.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    g_hadc1.Init.LowPowerAutoWait      = DISABLE;
    g_hadc1.Init.ContinuousConvMode    = ENABLE;
    g_hadc1.Init.NbrOfConversion       = eOTDET_TEMP_CH_NUM;
    g_hadc1.Init.DiscontinuousConvMode = DISABLE;
    g_hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    g_hadc1.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
    g_hadc1.Init.DMAContinuousRequests = ENABLE;
    g_hadc1.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    g_hadc1.Init.OversamplingMode      = DISABLE;

    init_status |= HAL_ADC_Init(&g_hadc1);

    multimode.Mode             = ADC_DUALMODE_INJECSIMULT;
    multimode.DMAAccessMode    = ADC_DMAACCESSMODE_DISABLED;
    multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_1CYCLE;

    init_status |= HAL_ADCEx_MultiModeConfigChannel(&g_hadc1, &multimode);

    ////////////////////////////////////////////////////////////////////////
    // ADC1 Regular channels
    ////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////
    // ADC_CHANNEL_TSO_IGBT_TEMP and ADC_CHANNEL_INTERNAL_TEMP_SENSOR are used by SAFE!
    // Do not change settings otherwise SAFE ADC and DMA configuration
    // check will fail!!!
    // If more regular channels are needed add them with ADC_REGULAR_RANK_3
    // or higher.
    ////////////////////////////////////////////////////////////////////////

    // Settings for all regular channels
    sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
    sConfig.SingleDiff   = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset       = 0;

    // initialize ADC channel for external (compressor) NTC
    sConfig.Channel      = ADC_CHANNEL_NTC_COMP;
    sConfig.Rank         = ADC_REGULAR_RANK_1;
    init_status |= HAL_ADC_ConfigChannel(&g_hadc1, &sConfig);

    // initialize ADC channel for NTC in IGBT
    sConfig.Channel      = ADC_CHANNEL_NTC_IGBT;
    sConfig.Rank         = ADC_REGULAR_RANK_2;
    init_status |= HAL_ADC_ConfigChannel(&g_hadc1, &sConfig);

    // initialize IGBT Internal Temperature sensor channel
    sConfig.Channel      = ADC_CHANNEL_TSO_IGBT;
    sConfig.Rank         = ADC_REGULAR_RANK_3;
    init_status |= HAL_ADC_ConfigChannel(&g_hadc1, &sConfig);

    // initialize MCU's Internal Temperature sensor channel
    sConfig.Channel      = ADC_CHANNEL_MCU_TEMP;
    sConfig.Rank         = ADC_REGULAR_RANK_4;
    init_status |= HAL_ADC_ConfigChannel(&g_hadc1, &sConfig);

    ////////////////////////////////////////////////////////////////////////
    // Injected channels
    ////////////////////////////////////////////////////////////////////////
    sConfigInjected.InjectedSingleDiff            = ADC_SINGLE_ENDED;
    sConfigInjected.InjectedOffsetNumber          = ADC_OFFSET_NONE;
    sConfigInjected.InjectedOffset                = 0;
    sConfigInjected.InjectedNbrOfConversion       = 3;
    sConfigInjected.AutoInjectedConv              = DISABLE;
    sConfigInjected.QueueInjectedContext          = DISABLE;
    sConfigInjected.InjecOversamplingMode         = DISABLE;
    sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
    sConfigInjected.ExternalTrigInjecConvEdge     = ADC_EXTERNALTRIGINJECCONV_EDGE_FALLING;
    sConfigInjected.ExternalTrigInjecConv         = ADC_EXTERNALTRIGINJEC_T1_TRGO;
    sConfigInjected.InjectedSamplingTime          = ADC_SAMPLETIME_6CYCLES_5;

    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_2; // DUMMY conversion
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_1;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc1, &sConfigInjected); 

    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_SHUNT_A;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_2;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc1, &sConfigInjected); 

    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_VDC;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_3;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc1, &sConfigInjected); 

    // dummy conversion of the same channel to prevent MUX switch too early to idle channel
    // see ES0431 STM32G431xx/441xx device errata, ver 7.0, 20.Apr 2021, section 2.5.6
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_VDC;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_4;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc1, &sConfigInjected); 

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes ADC2
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef adc2_init(void)
{
    HAL_StatusTypeDef        init_status     = HAL_OK;
    ADC_InjectionConfTypeDef sConfigInjected = {0};

    g_hadc2.Instance                   = ADC2;
    g_hadc2.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    g_hadc2.Init.Resolution            = ADC_RESOLUTION_12B;
    g_hadc2.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    g_hadc2.Init.GainCompensation      = 0;
    g_hadc2.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    g_hadc2.Init.EOCSelection          = ADC_EOC_SEQ_CONV;
    g_hadc2.Init.LowPowerAutoWait      = DISABLE;
    g_hadc2.Init.ContinuousConvMode    = DISABLE;
    g_hadc2.Init.NbrOfConversion       = 0;
    g_hadc2.Init.DiscontinuousConvMode = DISABLE;
    g_hadc2.Init.DMAContinuousRequests = DISABLE;
    g_hadc2.Init.Overrun               = ADC_OVR_DATA_PRESERVED;
    g_hadc2.Init.OversamplingMode      = DISABLE;

    init_status |= HAL_ADC_Init(&g_hadc2);

    // Configures ADC injected channel common settings
    sConfigInjected.InjectedOffsetNumber     = ADC_OFFSET_NONE;
    sConfigInjected.InjectedOffset           = 0;
    sConfigInjected.InjectedOffsetSign       = ADC_OFFSET_SIGN_NEGATIVE;
    sConfigInjected.InjectedOffsetSaturation = DISABLE;

    sConfigInjected.InjectedSingleDiff            = ADC_SINGLE_ENDED;
    sConfigInjected.InjectedNbrOfConversion       = 3;
    sConfigInjected.AutoInjectedConv              = DISABLE;
    sConfigInjected.QueueInjectedContext          = DISABLE;
    sConfigInjected.InjecOversamplingMode         = DISABLE;
    sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
    sConfigInjected.InjectedSamplingTime          = ADC_SAMPLETIME_6CYCLES_5;

    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_4; // DUMMY conversion
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_1;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc2, &sConfigInjected);


    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_SHUNT_B;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_2;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc2, &sConfigInjected);

    // Configures for the selected ADC injected channel its corresponding rank
    // in the sequencer and its sample time
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_SHUNT_C;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_3;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc2, &sConfigInjected);

    // dummy conversion of the same channel to prevent MUX switch too early to idle channel
    // see ES0431 STM32G431xx/441xx device errata, ver 7.0, 20.Apr 2021, section 2.5.6
    sConfigInjected.InjectedChannel      = ADC_CHANNEL_SHUNT_C;
    sConfigInjected.InjectedRank         = ADC_INJECTED_RANK_4;
    init_status |= HAL_ADCEx_InjectedConfigChannel(&g_hadc2, &sConfigInjected);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes GPIOs for analog measurement
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static void adc_gpio_init(void)
{
    GPIO_InitTypeDef gpio_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_InitStruct.Mode = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull = GPIO_NOPULL;

    gpio_InitStruct.Pin  = GPIO_PIN_ADC_VDC;    
    HAL_GPIO_Init(GPIO_PORT_ADC_VDC, &gpio_InitStruct);

    gpio_InitStruct.Pin  = GPIO_PIN_ADC_NTC_COMP;
    HAL_GPIO_Init(GPIO_PORT_ADC_NTC_COMP, &gpio_InitStruct);

    gpio_InitStruct.Pin  = GPIO_PIN_ADC_NTC_IGBT;
    HAL_GPIO_Init(GPIO_PORT_ADC_NTC_IGBT, &gpio_InitStruct);

    gpio_InitStruct.Pin  = GPIO_PIN_ADC_TSO_IGBT;
    HAL_GPIO_Init(GPIO_PORT_ADC_TSO_IGBT, &gpio_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function starts the ADC module
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
adc_status_t adc_start(void)
{
    adc_status_t status = eADC_STATUS_OK;

    if (HAL_ADC_Start_DMA(
            &g_hadc1, (uint32_t *)_dma_buffer, eOTDET_TEMP_CH_NUM * OTDET_SAMPLE_PER_CH
        ) != HAL_OK)
    {
        status = eADC_STATUS_DMA_NOT_STARTED;
    }
    else
    {
        // Start Injected conversion for ADC1 and ADC2
        HAL_ADCEx_InjectedStart(&g_hadc2);
        HAL_ADCEx_InjectedStart_IT(&g_hadc1);
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function stops the ADC module
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
adc_status_t adc_stop(void)
{
    adc_status_t status = eADC_STATUS_OK;

    if (HAL_ADC_Stop(&g_hadc1) != HAL_OK)
    {
        status = eADC_STATUS_NOT_STARTED;
    }

    if (HAL_ADC_Stop(&g_hadc2) != HAL_OK)
    {
        status = eADC_STATUS_NOT_STARTED;
    }

    if (HAL_ADC_Stop_DMA(&g_hadc1) != HAL_OK)
    {
        status = eADC_STATUS_NOT_STARTED;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function return analog values.
 *
 * @param[in]    None
 * @return       adc_data        Analog values
 */
////////////////////////////////////////////////////////////////////////////////
volatile adc_data_t *adc_analog_get(void)
{
    return &_adc_data;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 *   Function sets callback to interrupt.
 *
 * @param[in]        callback        - callback for the required group
 * @param[in]        callback_fnc    - pointer to the callback function
 * @return           none
 */
////////////////////////////////////////////////////////////////////////////////
void adc_set_callback(const adc_callback_t callback, const adc_callback_fnc_t callback_fnc)
{
    _adc_callback[callback] = callback_fnc;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function handles ADC DMA read
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc
) /* parasoft-suppress MISRAC2012-RULE_8_13-a NAMING-34 "External function callback, cannot be
     changed." */
{
    otdet_temp_ch_t ch;
    uint32_t        i;
    uint32_t        sum[eOTDET_TEMP_CH_NUM] = {0};

    for (ch = 0; ch < eOTDET_TEMP_CH_NUM; ch++)
    {
        for (i = 0; i < OTDET_SAMPLE_PER_CH; i++)
        {
            sum[ch] += _dma_buffer[(eOTDET_TEMP_CH_NUM * i) + ch];
        }
    }

    // Save raw values
    _adc_data_raw.ntc_comp = sum[0] / OTDET_SAMPLE_PER_CH;
    _adc_data_raw.ntc_igbt = sum[1] / OTDET_SAMPLE_PER_CH;
    _adc_data_raw.tso_igbt = sum[2] / OTDET_SAMPLE_PER_CH;
    _adc_data_raw.mcu_temp = sum[3] / OTDET_SAMPLE_PER_CH;

    // Don't convert raw values since it is done in SAFE.
    // Instead just read the values from SAFE. I am wondering how the values are
    // synchronized here since SAFE seems to be reading dma buffer asynchronously.
    otdet_temps_t temps = pg_safe_func_table->fp_Get_Otdet();
    _adc_data.ntc_comp = temps.sensor[eOTDET_TEMP_CH_NTC_COMP];
    _adc_data.ntc_igbt = temps.sensor[eOTDET_TEMP_CH_NTC_IGBT];
    _adc_data.tso_igbt = temps.sensor[eOTDET_TEMP_CH_TSO_IGBT];
    _adc_data.mcu_temp = temps.sensor[eOTDET_TEMP_CH_MCU_TEMP];
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function represents Injected ADC ISR which is generated after
 *                                all injected conversions are done
 *
 * @param[in]    *hadc                Pointer to ADC_HandleTypeDef structure
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc
) /* parasoft-suppress MISRAC2012-RULE_8_13-a NAMING-34 "External function callback, cannot be
     changed." */
{
#if (ADC_DBG_LED_ISR == 1U)
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
#endif

#if (PERFORMANCE_MEAS_EN == 1U)
    performance_meas_start((uint32_t)FAST_LOOP_EXECUTION);
#endif

    // Save raw values
    _adc_data_raw.current_u = HAL_ADCEx_InjectedGetValue(&g_hadc1, ADC_INJECTED_RANK_2);
    _adc_data_raw.current_v = HAL_ADCEx_InjectedGetValue(&g_hadc2, ADC_INJECTED_RANK_2);
    _adc_data_raw.current_w = HAL_ADCEx_InjectedGetValue(&g_hadc2, ADC_INJECTED_RANK_3);
    _adc_data_raw.vdc       = HAL_ADCEx_InjectedGetValue(&g_hadc1, ADC_INJECTED_RANK_3);

    // Convert raw values
    _adc_data.current_u = -((int16_t)_adc_data_raw.current_u - ADC_VOLT_OFFS_QUANT) * ADC_CURR_CONV_FACT;
    _adc_data.current_v = -((int16_t)_adc_data_raw.current_v - ADC_VOLT_OFFS_QUANT) * ADC_CURR_CONV_FACT;
    _adc_data.current_w = -((int16_t)_adc_data_raw.current_w - ADC_VOLT_OFFS_QUANT) * ADC_CURR_CONV_FACT;
    _adc_data.vdc       = _adc_data_raw.vdc * ADC_VOLTS_CONV_FACT;

    // Call motor_control_hndl or app_discharge_dc_link_hndl when going to low power 
    if (_adc_callback[eADC_CALLBACK_INJECTED_CHANNELS_DONE] != NULL)
    {
        _adc_callback[eADC_CALLBACK_INJECTED_CHANNELS_DONE]();
    }

#if (PERFORMANCE_MEAS_EN == 1U)
    performance_meas_stop((uint32_t)FAST_LOOP_EXECUTION);
#endif

#if (ADC_DBG_LED_ISR == 1U)
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function handles ADC1 and ADC2 global interrupts.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void ADC1_2_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function callback, cannot be changed." */
{
    /* Conversion complete callback */
    HAL_ADCEx_InjectedConvCpltCallback(&g_hadc1);

    /* Clear injected group conversion flag */
    __HAL_ADC_CLEAR_FLAG(&g_hadc1, ADC_FLAG_JEOC);
    __HAL_ADC_CLEAR_FLAG(&g_hadc1, ADC_FLAG_JEOS);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
