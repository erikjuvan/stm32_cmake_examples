////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     can.c
 * @brief    CAN module
 * @author   Tomaz Opresnik
 * @date     26.3.2021
 * @version  V1.0.0
 *
 * @mainpage
 * @section  Description
 *
 *   This driver is built upon the STM hal library. It is only intended for
 *   use with STM32 MCUs. Aurix MCUs have a peripheral called BXCAN, which this
 *   driver uses.
 *
 *   The user must first call the can_init() function for every CAN and provide
 *   it with the init structure. For sending the data the user can use can_send()
 *   function. The sending and receiving messages is handled via interrupts.
 *
 *   The user can configure the p_rx_callback and p_tx_callback for callback at
 *   the end of for received and/or send messages.
 *
 *   @note Only the CAN1 was tested.
 *   @note this driver supports only APB1 frequency 54MHz
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup CAN_MODULE
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include <string.h>

#include "../can_cfg.h"
#include "can.h"

#include "fifobuffer/fifobuffer.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Internal structure with configuration of channel
 */
typedef struct can_channel_s
{
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    CAN_HandleTypeDef h_can; /**< CAN handler */
#else
    FDCAN_HandleTypeDef h_can; /**< CAN handler */
#endif
    bool             initialized; /**< Initialised flag */
    can_node_index_t node_index;  /**< Node index to which the channel is linked */
    uint32_t         can_id;      /**< Receive CAN ID filter */
    uint32_t         can_id_mask; /**< Received CAN Mask filter */
    bool extended_frame; /**< false for 11-bit frames and true for the extended 27-bit frame. */
    rx_callback_t
        p_rx_callback; /**< Function called when data has been received on CAN interface. */
    tx_callback_t       p_tx_callback; /**< Function called when transmit has been finished. */
    fifo_t              tx_fifo;       /**< Fifobuffer for transmit */
    can_channel_stats_t stats;         /**< Can channel statistics */
} can_channel_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/**
 * Global handler for CAN
 */
can_channel_t gh_can_channel[eCAN_NUMBER_OF_CAN];

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////

static void can_irq_rx_template(can_channel_t *p_channel);
static void can_irq_tx_template(can_channel_t *p_channel, bool start);
static void can_gpio_init(can_node_index_t can_node);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN1_RX0_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN1].h_can);
    can_irq_rx_template(&gh_can_channel[eCAN_CAN1]);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN1_TX_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN1].h_can);
    can_irq_tx_template(&gh_can_channel[eCAN_CAN1], false);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN2_RX0_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN2].h_can);
    can_irq_rx_template(&gh_can_channel[eCAN_CAN2]);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN2_TX_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN2].h_can);
    can_irq_tx_template(&gh_can_channel[eCAN_CAN2], false);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN3_RX0_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN3].h_can);
    can_irq_rx_template(&gh_can_channel[eCAN_CAN3]);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void CAN3_TX_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_CAN_IRQHandler(&gh_can_channel[eCAN_CAN3].h_can);
    can_irq_tx_template(&gh_can_channel[eCAN_CAN3], false);
}
#else

void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan
) /* parasoft-suppress MISRAC2012-RULE_8_13-a NAMING-34 "External function callback, cannot be
     changed." */
// void HAL_FDCAN_TxEventFifoCallback(FDCAN_HandleTypeDef *hfdcan, uint32_t TxEventFifoITs)
{
    can_irq_tx_template(&gh_can_channel[eCAN_CAN1], false);
}

void HAL_FDCAN_RxFifo0Callback(
    FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs
) /* parasoft-suppress MISRAC2012-RULE_8_13-a NAMING-34 "External function callback, cannot be
     changed." */
{
    can_irq_rx_template(&gh_can_channel[eCAN_CAN1]);
}

void HAL_FDCAN_RxFifo1Callback(
    FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs
) /* parasoft-suppress MISRAC2012-RULE_8_13-a NAMING-34 "External function callback, cannot be
     changed." */
{
    can_irq_rx_template(&gh_can_channel[eCAN_CAN1]);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   CAN IRQ function
 *
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
#if defined(DEBUG)
void FDCAN1_IT0_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_FDCAN_IRQHandler(&gh_can_channel[eCAN_CAN1].h_can);
}
#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
/**
*   CAN IRQ function
*
* @return       none
*/
////////////////////////////////////////////////////////////////////////////////
void FDCAN1_IT1_IRQHandler(void) /* parasoft-suppress NAMING-34 "allow naming" */
{
    HAL_FDCAN_IRQHandler(&gh_can_channel[eCAN_CAN1].h_can);
    can_irq_tx_template(&gh_can_channel[eCAN_CAN1], false);
}
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
/**
 *   Template for the receive interrupt. This template is used for each channel.
 *
 * @param[in]    p_channel - Pointer to CAN channel instance
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
static void can_irq_rx_template(can_channel_t *p_channel)
{
    can_message_t received_msg;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    CAN_RxHeaderTypeDef rxHeader = {0};

    // Get message from frame
    HAL_CAN_GetRxMessage(&p_channel->h_can, CAN_RX_FIFO0, &rxHeader, received_msg.data.byte);

    received_msg.len = rxHeader.DLC;
    if (rxHeader.IDE != CAN_ID_STD)
    {
        received_msg.id = rxHeader.ExtId;
    }
    else
    {
        received_msg.id = rxHeader.StdId;
    }
#else
    FDCAN_RxHeaderTypeDef rxHeader = {0};

    // Get message from frame
    HAL_FDCAN_GetRxMessage(&p_channel->h_can, FDCAN_RX_FIFO0, &rxHeader, received_msg.data.byte);

    switch (rxHeader.DataLength)
    {
    case FDCAN_DLC_BYTES_0:
        received_msg.len = 0U;
        break;
    case FDCAN_DLC_BYTES_1:
        received_msg.len = 1U;
        break;
    case FDCAN_DLC_BYTES_2:
        received_msg.len = 2U;
        break;
    case FDCAN_DLC_BYTES_3:
        received_msg.len = 3U;
        break;
    case FDCAN_DLC_BYTES_4:
        received_msg.len = 4U;
        break;
    case FDCAN_DLC_BYTES_5:
        received_msg.len = 5U;
        break;
    case FDCAN_DLC_BYTES_6:
        received_msg.len = 6U;
        break;
    case FDCAN_DLC_BYTES_7:
        received_msg.len = 7U;
        break;
    case FDCAN_DLC_BYTES_8:
        received_msg.len = 8U;
        break;
    case FDCAN_DLC_BYTES_12:
        received_msg.len = 12U;
        break;
    case FDCAN_DLC_BYTES_16:
        received_msg.len = 16U;
        break;
    case FDCAN_DLC_BYTES_20:
        received_msg.len = 20U;
        break;
    case FDCAN_DLC_BYTES_24:
        received_msg.len = 24U;
        break;
    case FDCAN_DLC_BYTES_32:
        received_msg.len = 32U;
        break;
    case FDCAN_DLC_BYTES_48:
        received_msg.len = 48U;
        break;
    case FDCAN_DLC_BYTES_64:
        received_msg.len = 64U;
        break;
    default:
        received_msg.len = 0U;
        break;
    }

    if (rxHeader.IdType != FDCAN_STANDARD_ID)
    {
        received_msg.id = rxHeader.Identifier;
    }
    else
    {
        received_msg.id = rxHeader.Identifier;
    }
#endif

    // Call callback function for receive
    if (p_channel->p_rx_callback != NULL)
    {
        p_channel->p_rx_callback(&received_msg);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Template for the transmit interrupt. This template is used for each channel.
 *
 * @param[in]    p_channel   - Pointer to CAN channel instance
 * @param[in]    start       - indication if functions is called from can_send or from
 *                           interrupt. When true, TX callback is not called
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
static void can_irq_tx_template(can_channel_t *p_channel, bool start)
{
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    CAN_TxHeaderTypeDef txHeader = {0};
#else
    FDCAN_TxHeaderTypeDef txHeader = {0};
#endif
    can_message_t can_msg;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    uint32_t canMailbox = 0;
#endif

    CAN_ASSERT(p_channel->initializedialized)

    // If transmission is in progress, this is in interrupts when frame is sent
    if (start == false)
    {
        // Callback for transmit
        if (p_channel->p_tx_callback != NULL)
        {
            p_channel->p_tx_callback();
        }

        p_channel->stats.tx_counter++;
    }

    // Do we have any messages waiting to be sent?
    if (fifo_get(&p_channel->tx_fifo, &can_msg) == eFIFO_OK)
    {
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        if (p_channel->extended_frame == false)
        {
            txHeader.IDE   = CAN_ID_STD;
            txHeader.StdId = can_msg.id;
        }
        else
        {
            txHeader.IDE   = CAN_ID_EXT;
            txHeader.ExtId = can_msg.id;
        }

        txHeader.DLC                = can_msg.len;
        txHeader.RTR                = CAN_RTR_DATA;
        txHeader.TransmitGlobalTime = DISABLE;

        HAL_CAN_AddTxMessage(&p_channel->h_can, &txHeader, can_msg.data.byte, &canMailbox);
#else
        if (p_channel->extended_frame == false)
        {
            txHeader.IdType     = FDCAN_STANDARD_ID;
            txHeader.Identifier = can_msg.id;
        }
        else
        {
            txHeader.IdType     = FDCAN_EXTENDED_ID;
            txHeader.Identifier = can_msg.id;
        }

        switch (can_msg.len)
        {
        case 0U:
            txHeader.DataLength = FDCAN_DLC_BYTES_0;
            break;
        case 1U:
            txHeader.DataLength = FDCAN_DLC_BYTES_1;
            break;
        case 2U:
            txHeader.DataLength = FDCAN_DLC_BYTES_2;
            break;
        case 3U:
            txHeader.DataLength = FDCAN_DLC_BYTES_3;
            break;
        case 4U:
            txHeader.DataLength = FDCAN_DLC_BYTES_4;
            break;
        case 5U:
            txHeader.DataLength = FDCAN_DLC_BYTES_5;
            break;
        case 6U:
            txHeader.DataLength = FDCAN_DLC_BYTES_6;
            break;
        case 7U:
            txHeader.DataLength = FDCAN_DLC_BYTES_7;
            break;
        case 8U:
            txHeader.DataLength = FDCAN_DLC_BYTES_8;
            break;
        case 12U:
            txHeader.DataLength = FDCAN_DLC_BYTES_12;
            break;
        case 16U:
            txHeader.DataLength = FDCAN_DLC_BYTES_16;
            break;
        case 20U:
            txHeader.DataLength = FDCAN_DLC_BYTES_20;
            break;
        case 24U:
            txHeader.DataLength = FDCAN_DLC_BYTES_24;
            break;
        case 32U:
            txHeader.DataLength = FDCAN_DLC_BYTES_32;
            break;
        case 48U:
            txHeader.DataLength = FDCAN_DLC_BYTES_48;
            break;
        case 64U:
            txHeader.DataLength = FDCAN_DLC_BYTES_64;
            break;
        default:
            txHeader.DataLength = FDCAN_DLC_BYTES_0;
            break;
        }

        txHeader.TxFrameType   = FDCAN_DATA_FRAME;
        txHeader.FDFormat      = FDCAN_CLASSIC_CAN;
        txHeader.BitRateSwitch = FDCAN_BRS_OFF;

        HAL_FDCAN_AddMessageToTxFifoQ(&p_channel->h_can, &txHeader, can_msg.data.byte);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   This function initialises CAN pins
 *
 * @param[in]    can_node - CAN node used
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
static void can_gpio_init(can_node_index_t can_node)
{
    GPIO_InitTypeDef gpioInitStruct;

    switch (can_node)
    {
    case eCAN_CAN1:
    {
        CAN1_TX_CLK_EN(); /* parasoft-suppress CODSTA-83 "External code." */
        CAN1_RX_CLK_EN(); /* parasoft-suppress CODSTA-83 "External code." */

        gpioInitStruct.Pin   = CAN1_TX_PIN;
        gpioInitStruct.Mode  = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull  = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        gpioInitStruct.Alternate = GPIO_AF9_CAN1;
#else
        gpioInitStruct.Alternate = GPIO_AF9_FDCAN1;
#endif
        HAL_GPIO_Init(CAN1_TX_PORT, &gpioInitStruct);

        gpioInitStruct.Pin   = CAN1_RX_PIN;
        gpioInitStruct.Mode  = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull  = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        gpioInitStruct.Alternate = GPIO_AF9_CAN1;
#else
        gpioInitStruct.Alternate = GPIO_AF9_FDCAN1;
#endif
        HAL_GPIO_Init(CAN1_RX_PORT, &gpioInitStruct);
        break;
    }

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
    case eCAN_CAN2:
    {
        CAN2_TX_CLK_EN();
        CAN2_RX_CLK_EN();

        gpioInitStruct.Pin   = CAN2_TX_PIN;
        gpioInitStruct.Mode  = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull  = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        gpioInitStruct.Alternate = GPIO_AF9_CAN2;
#else
        gpioInitStruct.Alternate = GPIO_AF9_FDCAN2;
#endif
        HAL_GPIO_Init(CAN2_TX_PORT, &gpioInitStruct);

        gpioInitStruct.Pin   = CAN2_RX_PIN;
        gpioInitStruct.Mode  = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull  = GPIO_NOPULL;
        gpioInitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        gpioInitStruct.Alternate = GPIO_AF9_CAN2;
#else
        gpioInitStruct.Alternate = GPIO_AF9_FDCAN2;
#endif
        HAL_GPIO_Init(CAN2_RX_PORT, &gpioInitStruct);
        break;
    }

    case eCAN_CAN3:
    {
        CAN3_TX_CLK_EN();
        CAN3_RX_CLK_EN();

        gpioInitStruct.Pin       = CAN3_TX_PIN;
        gpioInitStruct.Mode      = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull      = GPIO_NOPULL;
        gpioInitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        gpioInitStruct.Alternate = GPIO_AF11_CAN3;
        HAL_GPIO_Init(CAN3_TX_PORT, &gpioInitStruct);

        gpioInitStruct.Pin       = CAN3_RX_PIN;
        gpioInitStruct.Mode      = GPIO_MODE_AF_PP;
        gpioInitStruct.Pull      = GPIO_NOPULL;
        gpioInitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        gpioInitStruct.Alternate = GPIO_AF11_CAN3;
        HAL_GPIO_Init(CAN3_RX_PORT, &gpioInitStruct);
        break;
    }
#endif

    default:
    {
        // don't know, don't care
        break;
    }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup CAN_MODULE_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
 * @brief    Initialisation of CAN
 *
 * @note     CAN baudrate calculated with: http://www.bittiming.can-wiki.info/#bxCAN
 *
 * @param[in]    p_can_init_struct   - pointer to the CAN initialisation structure
 * @param[out]   p_channel           - pointer to the CAN channel
 * @return       status              - status of initialisation
 */
////////////////////////////////////////////////////////////////////////////////
can_err_t can_init(const can_init_t *const p_can_init_struct, p_can_channel_t *const p_channel)
{
    can_err_t status = eCAN_OK;

    if (p_can_init_struct == NULL)
    {
        status = eCAN_NULL_PTR;
        CAN_ASSERT(0);
    }
    // if node_id is eCAN_NUMBER_OF_CAN or bigger (for whatever reason) return error
    else if (p_can_init_struct->node_id >= eCAN_NUMBER_OF_CAN)
    {
        status = eCAN_NODE_NOT_EXISTING;
        CAN_ASSERT(0);
    }
    else if (p_channel == NULL)
    {
        status = eCAN_NULL_PTR;
        CAN_ASSERT(0);
    }
    else if (gh_can_channel[p_can_init_struct->node_id].initialized == true)
    {
        status = eCAN_NODE_MULTIPLE_CONFIGURATION;
        CAN_PRINTF("CAN node %d already initialized\n", (uint8_t)p_can_init_struct->node_id);
        CAN_ASSERT(0);
    }
    else if ((HAL_RCC_GetPCLK1Freq() != 54000000) && (HAL_RCC_GetPCLK1Freq() != 170000000))
    {
        status = eCAN_ERROR;
        CAN_PRINTF("CAN clock frequency is not supported with this driver\n");
        CAN_ASSERT(0);
    }
    else
    {
        can_gpio_init(p_can_init_struct->node_id);

        switch (p_can_init_struct->node_id)
        {
        case eCAN_CAN1:
        {
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
            gh_can_channel[p_can_init_struct->node_id].h_can.Instance = CAN1;
            __HAL_RCC_CAN1_CLK_ENABLE();
#else
            gh_can_channel[p_can_init_struct->node_id].h_can.Instance = FDCAN1;
            __HAL_RCC_FDCAN_CLK_ENABLE();
#endif
            break;
        }
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        case eCAN_CAN2:
        {
            gh_can_channel[p_can_init_struct->node_id].h_can.Instance = CAN2;
            // CAN1 CLK must be enabled when using CAN2, because CAN2 is slave of CAN1
            __HAL_RCC_CAN1_CLK_ENABLE();
            __HAL_RCC_CAN2_CLK_ENABLE();
            break;
        }

        case eCAN_CAN3:
        {
            gh_can_channel[p_can_init_struct->node_id].h_can.Instance = CAN3;
            __HAL_RCC_CAN3_CLK_ENABLE();
            break;
        }
#endif

        default:
        {
            // doesn't matter anymore
            break;
        }
        }

        // fill the structure for CAN STM HAL init
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.Mode = p_can_init_struct->mode;

        switch (p_can_init_struct->baudrate)
        {
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        case eCAN_BAUDRATE_10k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 300;
            break;

        case eCAN_BAUDRATE_20k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 150;
            break;

        case eCAN_BAUDRATE_50k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 60;
            break;

        case eCAN_BAUDRATE_100k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 30;
            break;

        case eCAN_BAUDRATE_125k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 24;
            break;

        case eCAN_BAUDRATE_250k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 12;
            break;

        case eCAN_BAUDRATE_500k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 6;
            break;

        case eCAN_BAUDRATE_1M:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.Prescaler = 3;
            break;
#else
        case eCAN_BAUDRATE_10k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 300;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 300;
            break;

        case eCAN_BAUDRATE_20k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 150;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 150;
            break;

        case eCAN_BAUDRATE_50k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 60;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 60;
            break;

        case eCAN_BAUDRATE_100k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 30;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 30;
            break;

        case eCAN_BAUDRATE_125k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 24;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 24;
            break;

        case eCAN_BAUDRATE_250k:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 12;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 12;
            break;

        case eCAN_BAUDRATE_500k:
            // gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler = 6;
            // gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 6;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 20;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 20;
            break;

        case eCAN_BAUDRATE_1M:
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataPrescaler    = 3;
            gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalPrescaler = 3;
            break;
#endif

        default:
            status = eCAN_WRONG_BAUDRATE;
            CAN_ASSERT(0);
            break;
        }

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.SyncJumpWidth        = CAN_SJW_1TQ;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TimeSeg1             = CAN_BS1_15TQ;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TimeSeg2             = CAN_BS2_2TQ;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TimeTriggeredMode    = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.AutoBusOff           = ENABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.AutoWakeUp           = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.AutoRetransmission   = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.ReceiveFifoLocked    = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TransmitFifoPriority = DISABLE;

        if (HAL_CAN_Init(&gh_can_channel[p_can_init_struct->node_id].h_can) != HAL_OK)
        {
            status                                                 = eCAN_ERROR;
            gh_can_channel[p_can_init_struct->node_id].initialized = false;
        }
        else
        {
            gh_can_channel[p_can_init_struct->node_id].initialized = true;
        }
#else
        /*gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataSyncJumpWidth      = 1;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataTimeSeg1           = 15;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataTimeSeg2           = 2;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalSyncJumpWidth   = 1;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalTimeSeg1        = 15;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalTimeSeg2        = 2;*/
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataSyncJumpWidth    = 1;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataTimeSeg1         = 14;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.DataTimeSeg2         = 2;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalSyncJumpWidth = 1;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalTimeSeg1      = 14;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.NominalTimeSeg2      = 2;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.AutoRetransmission   = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TransmitPause        = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.ProtocolException    = DISABLE;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.TxFifoQueueMode =
            FDCAN_TX_FIFO_OPERATION;
        gh_can_channel[p_can_init_struct->node_id].h_can.Init.FrameFormat = FDCAN_FRAME_CLASSIC;

        if (HAL_FDCAN_Init(&gh_can_channel[p_can_init_struct->node_id].h_can) != HAL_OK)
        {
            status                                                 = eCAN_ERROR;
            gh_can_channel[p_can_init_struct->node_id].initialized = false;
        }
        else
        {
            gh_can_channel[p_can_init_struct->node_id].initialized = true;
        }
#endif

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        // structure for CAN filter STM HAL init
        // https://schulz-m.github.io/2017/03/23/stm32-can-id-filter/
        CAN_FilterTypeDef filterConfig;
        if (p_can_init_struct->extended_frame == true)
        {
            filterConfig.FilterIdHigh     = (p_can_init_struct->can_id >> 13);
            filterConfig.FilterIdLow      = (p_can_init_struct->can_id << 3);
            filterConfig.FilterMaskIdHigh = (p_can_init_struct->can_id_mask >> 13);
            filterConfig.FilterMaskIdLow  = (p_can_init_struct->can_id_mask << 3);
        }
        else
        {
            filterConfig.FilterIdHigh     = p_can_init_struct->can_id << 5;
            filterConfig.FilterIdLow      = 0;
            filterConfig.FilterMaskIdHigh = p_can_init_struct->can_id_mask << 5;
            filterConfig.FilterMaskIdLow  = 0;
        }
        filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
        filterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
        filterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
        filterConfig.FilterActivation     = ENABLE;
        filterConfig.FilterBank           = 0;
        filterConfig.SlaveStartFilterBank = 14;

        if (HAL_CAN_ConfigFilter(
                &gh_can_channel[p_can_init_struct->node_id].h_can, &filterConfig
            ) != HAL_OK)
        {
            status = eCAN_ERROR;
        }
#else
        // structure for CAN filter STM HAL init
        // https://schulz-m.github.io/2017/03/23/stm32-can-id-filter/
        FDCAN_FilterTypeDef filterConfig;
        if (p_can_init_struct->extended_frame == true)
        {
            filterConfig.FilterID1 = p_can_init_struct->can_id;
            filterConfig.FilterID2 = p_can_init_struct->can_id_mask;
        }
        else
        {
            filterConfig.FilterID1 = p_can_init_struct->can_id;
            filterConfig.FilterID2 = p_can_init_struct->can_id_mask;
        }

        filterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
        filterConfig.FilterIndex  = 0;
        filterConfig.FilterType   = FDCAN_FILTER_MASK;
        filterConfig.IdType       = FDCAN_STANDARD_ID;

        if (HAL_FDCAN_ConfigFilter(
                &gh_can_channel[p_can_init_struct->node_id].h_can, &filterConfig
            ) != HAL_OK)
        {
            status = eCAN_ERROR;
        }
#endif

        // INITIALISE TX BUFFER
        if (fifo_create(
                &gh_can_channel[p_can_init_struct->node_id].tx_fifo, (uint16_t)CAN_TX_BUFFER_SIZE,
                (uint16_t)sizeof(can_message_t)
            ) != eFIFO_OK)
        {
            gh_can_channel[p_can_init_struct->node_id].initialized = false;

            CAN_PRINTF(
                "CAN: Unable to allocate memory for TX fifo buffer for node %d\n",
                (uint8_t)p_can_init_struct->node_id
            );
            CAN_ASSERT(0);
            status = eCAN_CHANNEL_NO_MEMORY;
        }

        gh_can_channel[p_can_init_struct->node_id].node_index  = p_can_init_struct->node_id;
        gh_can_channel[p_can_init_struct->node_id].can_id      = p_can_init_struct->can_id;
        gh_can_channel[p_can_init_struct->node_id].can_id_mask = p_can_init_struct->can_id_mask;
        gh_can_channel[p_can_init_struct->node_id].extended_frame =
            p_can_init_struct->extended_frame;
        gh_can_channel[p_can_init_struct->node_id].p_rx_callback = p_can_init_struct->p_rx_callback;
        gh_can_channel[p_can_init_struct->node_id].p_tx_callback = p_can_init_struct->p_tx_callback;
        gh_can_channel[p_can_init_struct->node_id].stats.rx_counter = 0;
        gh_can_channel[p_can_init_struct->node_id].stats.tx_counter = 0;

        *p_channel = &gh_can_channel[p_can_init_struct->node_id];

        switch (p_can_init_struct->node_id)
        {
        case eCAN_CAN1:
        {
#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
            HAL_NVIC_SetPriority(CAN1_RX0_IRQn, CAN1_RX0_PRIORITY, CAN1_RX0_SUB_PRIORITY);
            HAL_NVIC_SetPriority(CAN1_TX_IRQn, CAN1_TX_PRIORITY, CAN1_TX_SUB_PRIORITY);

            HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
            HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
#else
            HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, FDCAN1_IT0_IRQ_PRIORITY, 0UL);
            HAL_NVIC_SetPriority(FDCAN1_IT1_IRQn, FDCAN1_IT1_IRQ_PRIORITY, 0UL);

            HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
            HAL_NVIC_EnableIRQ(FDCAN1_IT1_IRQn);
#endif
            break;
        }

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        case eCAN_CAN2:
        {
            HAL_NVIC_SetPriority(CAN2_RX0_IRQn, CAN2_RX0_PRIORITY, CAN2_RX0_SUB_PRIORITY);
            HAL_NVIC_SetPriority(CAN2_TX_IRQn, CAN2_TX_PRIORITY, CAN2_TX_SUB_PRIORITY);

            HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
            HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
            break;
        }

        case eCAN_CAN3:
        {
            HAL_NVIC_SetPriority(CAN3_RX0_IRQn, CAN3_RX0_PRIORITY, CAN3_RX0_SUB_PRIORITY);
            HAL_NVIC_SetPriority(CAN3_TX_IRQn, CAN3_TX_PRIORITY, CAN3_TX_SUB_PRIORITY);

            HAL_NVIC_EnableIRQ(CAN3_RX0_IRQn);
            HAL_NVIC_EnableIRQ(CAN3_TX_IRQn);
            break;
        }
#endif

        default:
        {
            // don't care
            break;
        }
        }

#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
        // enable interrupt
        HAL_CAN_ActivateNotification(
            &gh_can_channel[p_can_init_struct->node_id].h_can, CAN_IT_RX_FIFO0_MSG_PENDING
        );
        HAL_CAN_ActivateNotification(
            &gh_can_channel[p_can_init_struct->node_id].h_can, CAN_IT_TX_MAILBOX_EMPTY
        );

        // Start the CAN module
        HAL_CAN_Start(&gh_can_channel[p_can_init_struct->node_id].h_can);
#else
        // enable interrupt
        HAL_FDCAN_ActivateNotification(
            &gh_can_channel[p_can_init_struct->node_id].h_can, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0UL
        );
        HAL_FDCAN_ActivateNotification(
            &gh_can_channel[p_can_init_struct->node_id].h_can, FDCAN_IT_TX_FIFO_EMPTY, 0UL
        );

        // Start the CAN module
        HAL_FDCAN_Start(&gh_can_channel[p_can_init_struct->node_id].h_can);
#endif
    }

    return (status);
}

////////////////////////////////////////////////////////////////////////////////
/**
 *   Send CAN message.
 *
 *   Sends a message to the channel queue. This does not actually send the message
 *   it only adds it to the queue. If channel is currently not transmitting its
 *   IRQ function is called, otherwise interrupt will handle and fetch next message
 *   from buffer and send it.
 *
 * @param[in]    channel     - Pointer to the channel over which we want to send.
 * @param[in]    p_can_msg   - Pointer to a can message structure which will be sent
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
can_err_t can_send(const p_can_channel_t channel, can_message_t *const p_can_msg)
{
    can_err_t status = eCAN_OK;

    if (channel == NULL)
    {
        CAN_PRINTF("CAN ERROR: Trying to send data to non existing can channel instance\n");
        CAN_ASSERT(0);
        status = eCAN_NULL_PTR;
    }
    else if (p_can_msg == NULL)
    {
        CAN_ASSERT(0);
        status = eCAN_NULL_PTR;
    }
    else
    {
        CAN_ASSERT(channel->initialized)

        // Mask CAN ID, depending on configuration of channel
        if (channel->extended_frame)
        {
            p_can_msg->id = (p_can_msg->id & 0x1FFFFFFF); // 29 bit CAN ID
        }
        else
        {
            p_can_msg->id = (p_can_msg->id & 0x7FFu); // 11 bit CAN ID
        }

        // Insert MSG into queue
        if (fifo_add(&channel->tx_fifo, p_can_msg) != eFIFO_OK)
        {
            status = eCAN_SEND_ERROR;
        }

        can_irq_tx_template(channel, true);
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
