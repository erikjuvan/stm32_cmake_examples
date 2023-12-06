////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     comparator.c
 * @brief    Functions for comparators
 * @author   Haris Kovacevic
 * @date     10.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup COMPARATOR_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "comparator.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
COMP_HandleTypeDef g_hcomp1;
COMP_HandleTypeDef g_hcomp2;
COMP_HandleTypeDef g_hcomp3;
COMP_HandleTypeDef g_hcomp4;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef comparator1_init(void);
static HAL_StatusTypeDef comparator2_init(void);
static HAL_StatusTypeDef comparator3_init(void);
static HAL_StatusTypeDef comparator4_init(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes comparators
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef comparator_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    // @Notes: Only comparator 3 is used for HW protection because
    //           DAC1 is used for current offset setup

    if ((comparator1_init() != HAL_OK) || (comparator2_init() != HAL_OK) ||
        (comparator3_init() != HAL_OK) || (comparator4_init() != HAL_OK))
    {
        init_status = HAL_ERROR;
    }

    // Start Comparators
    HAL_COMP_Start(&g_hcomp1);
    HAL_COMP_Start(&g_hcomp2);
    HAL_COMP_Start(&g_hcomp3);
    HAL_COMP_Start(&g_hcomp4);

    return init_status;
}

HAL_StatusTypeDef comparator_stop(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    // Stop Comparators
    HAL_COMP_Stop(&g_hcomp1);
    HAL_COMP_Stop(&g_hcomp2);
    HAL_COMP_Stop(&g_hcomp3);
    HAL_COMP_Stop(&g_hcomp4);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes comparator 1
 *
 * @details      Comparator 1 is used for overcurrent protection for phase U
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef comparator1_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hcomp1.Instance          = COMP1;
    g_hcomp1.Init.InputPlus    = COMP_INPUT_PLUS_IO1;
    g_hcomp1.Init.InputMinus   = COMP_INPUT_MINUS_DAC1_CH1;
    g_hcomp1.Init.OutputPol    = COMP_OUTPUTPOL_NONINVERTED;
    g_hcomp1.Init.Hysteresis   = COMP_HYSTERESIS_10MV;
    g_hcomp1.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
    g_hcomp1.Init.TriggerMode  = COMP_TRIGGERMODE_NONE;

    if (HAL_COMP_Init(&g_hcomp1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes comparator 2
 *
 * @details      Comparator 2 is used for overcurrent protection for phase V
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef comparator2_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hcomp2.Instance          = COMP2;
    g_hcomp2.Init.InputPlus    = COMP_INPUT_PLUS_IO1;
    g_hcomp2.Init.InputMinus   = COMP_INPUT_MINUS_DAC1_CH2;
    g_hcomp2.Init.OutputPol    = COMP_OUTPUTPOL_NONINVERTED;
    g_hcomp2.Init.Hysteresis   = COMP_HYSTERESIS_10MV;
    g_hcomp2.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
    g_hcomp2.Init.TriggerMode  = COMP_TRIGGERMODE_NONE;

    if (HAL_COMP_Init(&g_hcomp2) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes comparator 3
 *
 * @details      Comparator 3 is used for overvoltage protection for DC link
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef comparator3_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hcomp3.Instance          = COMP3;
    g_hcomp3.Init.InputPlus    = COMP_INPUT_PLUS_IO1;
    g_hcomp3.Init.InputMinus   = COMP_INPUT_MINUS_DAC3_CH1;
    g_hcomp3.Init.OutputPol    = COMP_OUTPUTPOL_NONINVERTED;
    g_hcomp3.Init.Hysteresis   = COMP_HYSTERESIS_10MV;
    g_hcomp3.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
    g_hcomp3.Init.TriggerMode  = COMP_TRIGGERMODE_NONE;

    if (HAL_COMP_Init(&g_hcomp3) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function initializes comparator 4
 *
 * @details      Comparator 1 is used for overcurrent protection for phase W
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
static HAL_StatusTypeDef comparator4_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    g_hcomp4.Instance          = COMP4;
    g_hcomp4.Init.InputPlus    = COMP_INPUT_PLUS_IO1;
    g_hcomp4.Init.InputMinus   = COMP_INPUT_MINUS_DAC3_CH2;
    g_hcomp4.Init.OutputPol    = COMP_OUTPUTPOL_NONINVERTED;
    g_hcomp4.Init.Hysteresis   = COMP_HYSTERESIS_10MV;
    g_hcomp4.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
    g_hcomp4.Init.TriggerMode  = COMP_TRIGGERMODE_NONE;

    if (HAL_COMP_Init(&g_hcomp4) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
