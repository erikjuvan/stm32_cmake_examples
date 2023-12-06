////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/**
 * @file     dma.c
 * @brief    Functions for DMA
 * @author   Haris Kovacevic
 * @date     15.03.2021
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup DMA_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "dma.h"
#include "priorities.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
DMA_HandleTypeDef        g_hdma1;
extern ADC_HandleTypeDef g_hadc1;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        Function configures DMA
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
HAL_StatusTypeDef dma_init(void)
{
    HAL_StatusTypeDef init_status = HAL_OK;

    // DMA controller clock enable
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    g_hdma1.Instance                 = DMA1_Channel1;
    g_hdma1.Init.Request             = DMA_REQUEST_ADC1;
    g_hdma1.Init.Direction           = DMA_PERIPH_TO_MEMORY;
    g_hdma1.Init.PeriphInc           = DMA_PINC_DISABLE;
    g_hdma1.Init.MemInc              = DMA_MINC_ENABLE;
    g_hdma1.Init.Mode                = DMA_CIRCULAR;
    g_hdma1.Init.Priority            = DMA1_CHANNEL1_CH_PRIORITY;
    g_hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    g_hdma1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;

    if (HAL_DMA_Init(&g_hdma1) != HAL_OK)
    {
        init_status = HAL_ERROR;
    }

    // Link ADC1 and DMA
    __HAL_LINKDMA(&g_hadc1, DMA_Handle, g_hdma1);

    // DMA1_Channel1_IRQn interrupt configuration
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, DMA1_CHANNEL1_IRQ_PRIORITY, 0UL);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    return init_status;
}

////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief        This function handles DMA1 channel 1 global interrupt.
 *
 * @param[in]    None
 * @return       None
 */
////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel1_IRQHandler(void
) /* parasoft-suppress NAMING-34 "External function callback, cannot be changed." */
{
    HAL_DMA_IRQHandler(&g_hdma1);
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////