////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     opamp.c
 * @brief    Functions for internal operational amplifiers
 * @author   Haris Kovacevic
 * @date     08.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup OPAMP_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "opamp.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// 0 - OPAMP1 is not used, 1 - OPAMP1 is used
#define OPAMP1_ENABLE          (1)
// 0 - OPAMP2 is not used, 1 - OPAMP2 is used
#define OPAMP2_ENABLE          (1)
// 0 - OPAMP3 is not used, 1 - OPAMP3 is used
#define OPAMP3_ENABLE          (1)

// 0 - OPAMP1 output is internal, 1 - OPAMP1 output is available on external pin
#define OPAMP1_OUT_EXTERNAL_EN (0)
// 0 - OPAMP2 output is internal, 1 - OPAMP2 output is available on external pin
#define OPAMP2_OUT_EXTERNAL_EN (0)
// 0 - OPAMP3 output is internal, 1 - OPAMP3 output is available on external pin
#define OPAMP3_OUT_EXTERNAL_EN (0)

// 0 - OPAMP1 calibration disabled, 1 - OPAMP1 calibration enabled
#define OPAMP1_OFFSET_CALIB_EN (0)
// 0 - OPAMP2 calibration disabled, 1 - OPAMP2 calibration enabled
#define OPAMP2_OFFSET_CALIB_EN (0)
// 0 - OPAMP3 calibration disabled, 1 - OPAMP3 calibration enabled
#define OPAMP3_OFFSET_CALIB_EN (0)

#define GPIO_PIN_OPAMP1_VINP   GPIO_PIN_1
#define GPIO_PORT_OPAMP1_VINP  GPIOA

#define GPIO_PIN_OPAMP2_VINP   GPIO_PIN_7
#define GPIO_PORT_OPAMP2_VINP  GPIOA

#define GPIO_PIN_OPAMP3_VINP   GPIO_PIN_0
#define GPIO_PORT_OPAMP3_VINP  GPIOB

#if (OPAMP1_OUT_EXTERNAL_EN == 1U)
#define GPIO_PIN_OPAMP1_OUT  GPIO_PIN_2
#define GPIO_PORT_OPAMP1_OUT GPIOA
#endif

#if (OPAMP2_OUT_EXTERNAL_EN == 1U)
#define GPIO_PIN_OPAMP2_OUT  GPIO_PIN_6
#define GPIO_PORT_OPAMP2_OUT GPIOA
#endif

#if (OPAMP3_OUT_EXTERNAL_EN == 1U)
#define GPIO_PIN_OPAMP3_OUT  GPIO_PIN_1
#define GPIO_PORT_OPAMP3_OUT GPIOB
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#if (OPAMP1_ENABLE == 1)
OPAMP_HandleTypeDef g_hopamp1;
#endif
#if (OPAMP2_ENABLE == 1)
OPAMP_HandleTypeDef g_hopamp2;
#endif
#if (OPAMP3_ENABLE == 1)
OPAMP_HandleTypeDef g_hopamp3;
#endif

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
#if (OPAMP1_ENABLE == 1)
static HAL_StatusTypeDef opamp1_init(void);
#endif
#if (OPAMP2_ENABLE == 1)
static HAL_StatusTypeDef opamp2_init(void);
#endif
#if (OPAMP3_ENABLE == 1)
static HAL_StatusTypeDef opamp3_init(void);
#endif

static void opamp_gpio_init(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes internal operational amplifiers
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef opamp_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

#if (OPAMP1_ENABLE == 1)
    if (opamp1_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif

#if (OPAMP2_ENABLE == 1)
    if (opamp2_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif

#if (OPAMP3_ENABLE == 1)
    if (opamp3_init() != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif

    // Initialize GPIOs for OPAMPs
    opamp_gpio_init();

    // Start OPAMPs
#if (OPAMP1_ENABLE == 1)
    HAL_OPAMP_Start(&g_hopamp1);
#endif
#if (OPAMP2_ENABLE == 1)
    HAL_OPAMP_Start(&g_hopamp2);
#endif
#if (OPAMP3_ENABLE == 1)
    HAL_OPAMP_Start(&g_hopamp3);
#endif

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Stop and disable all OPAMPs.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void opamp_stop(void)
{
    HAL_OPAMP_Stop(&g_hopamp1);
    HAL_OPAMP_Stop(&g_hopamp2);
    HAL_OPAMP_Stop(&g_hopamp3);
}

#if (OPAMP1_ENABLE == 1)
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes OPAMP1
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef opamp1_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hopamp1.Instance               = OPAMP1;
    g_hopamp1.Init.PowerMode         = OPAMP_POWERMODE_NORMAL;
    g_hopamp1.Init.Mode              = OPAMP_PGA_MODE;
    g_hopamp1.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
#if (OPAMP1_OUT_EXTERNAL_EN == 1U)
    g_hopamp1.Init.InternalOutput = DISABLE;
#else
    g_hopamp1.Init.InternalOutput = ENABLE;
#endif
    g_hopamp1.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
    g_hopamp1.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
    g_hopamp1.Init.PgaGain                = OPAMP_PGA_GAIN_4_OR_MINUS_3;
    g_hopamp1.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;

    if (HAL_OPAMP_Init(&g_hopamp1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

#if (OPAMP1_OFFSET_CALIB_EN == 1U)
    if (HAL_OPAMP_SelfCalibrate(&g_hopamp1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif

    return init_status;
}
#endif

#if (OPAMP2_ENABLE == 1)
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes OPAMP2
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef opamp2_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hopamp2.Instance               = OPAMP2;
    g_hopamp2.Init.PowerMode         = OPAMP_POWERMODE_NORMAL;
    g_hopamp2.Init.Mode              = OPAMP_PGA_MODE;
    g_hopamp2.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
#if (OPAMP2_OUT_EXTERNAL_EN == 1U)
    g_hopamp2.Init.InternalOutput = DISABLE;
#else
    g_hopamp2.Init.InternalOutput = ENABLE;
#endif
    g_hopamp2.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
    g_hopamp2.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
    g_hopamp2.Init.PgaGain                = OPAMP_PGA_GAIN_4_OR_MINUS_3;
    g_hopamp2.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;

    if (HAL_OPAMP_Init(&g_hopamp2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

#if (OPAMP2_OFFSET_CALIB_EN == 1U)
    if (HAL_OPAMP_SelfCalibrate(&g_hopamp2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif

    return init_status;
}
#endif

#if (OPAMP3_ENABLE == 1)
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes OPAMP3
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef opamp3_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hopamp3.Instance               = OPAMP3;
    g_hopamp3.Init.PowerMode         = OPAMP_POWERMODE_NORMAL;
    g_hopamp3.Init.Mode              = OPAMP_PGA_MODE;
    g_hopamp3.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
#if (OPAMP3_OUT_EXTERNAL_EN == 1U)
    g_hopamp3.Init.InternalOutput = DISABLE;
#else
    g_hopamp3.Init.InternalOutput = ENABLE;
#endif
    g_hopamp3.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
    g_hopamp3.Init.PgaConnect             = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
    g_hopamp3.Init.PgaGain                = OPAMP_PGA_GAIN_4_OR_MINUS_3;
    g_hopamp3.Init.UserTrimming           = OPAMP_TRIMMING_FACTORY;

    if (HAL_OPAMP_Init(&g_hopamp3) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

#if (OPAMP3_OFFSET_CALIB_EN == 1U)
    if (HAL_OPAMP_SelfCalibrate(&g_hopamp3) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }
#endif
    return init_status;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes GPIOs for internal operational amplifiers
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static void opamp_gpio_init(void)
{
    // OPAMP1_VINP              PA1         VINP0
    // OPAMP1_VINM              NOT CONNECTED
    // OPAMP2_VINP              PA7         VINP0
    // OPAMP2_VINM              NOT CONNECTED
    // OPAMP3_VINP              PB0         VINP0
    // OPAMP3_VINM              NOT CONNECTED
    // OPAMP4_VINP              PB11        VINP2
    // OPAMP4_VINM              NOT CONNECTED

    GPIO_InitTypeDef gpio_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

#if (OPAMP1_ENABLE == 1)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP1_VINP;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP1_VINP, &gpio_InitStruct);

#if (OPAMP1_OUT_EXTERNAL_EN == 1U)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP1_OUT;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP1_OUT, &gpio_InitStruct);
#endif
#endif

#if (OPAMP2_ENABLE == 1)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP2_VINP;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP2_VINP, &gpio_InitStruct);

#if (OPAMP2_OUT_EXTERNAL_EN == 1U)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP2_OUT;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP2_OUT, &gpio_InitStruct);
#endif
#endif

#if (OPAMP3_ENABLE == 1)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP3_VINP;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP3_VINP, &gpio_InitStruct);

#if (OPAMP3_OUT_EXTERNAL_EN == 1U)
    gpio_InitStruct.Pin   = GPIO_PIN_OPAMP3_OUT;
    gpio_InitStruct.Mode  = GPIO_MODE_ANALOG;
    gpio_InitStruct.Pull  = GPIO_NOPULL;
    gpio_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIO_PORT_OPAMP3_OUT, &gpio_InitStruct);
#endif
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
