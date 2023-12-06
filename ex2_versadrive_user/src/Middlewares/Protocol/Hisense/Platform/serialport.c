//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        serialport.c
// CREATION DATE:      2020/01/14
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/27
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Managing all communication ports used in project - Hardware Abstraction Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------


#include "cfg/platform_cfg.h"
#include "cfg/project_cfg.h"
#include "types.h"
#include "Platform/serialport.h"
#include "Platform/proc.h"
#include "Platform/platform.h"

#ifndef RELOCATE_VECTORS_TO_RAM
#define RELOCATE_VECTORS_TO_RAM          1
#endif


#if (SERIALPORT16_IN_USE)
    #error SERIALPORT:  Maximum 16 serial ports are supported !
#elif (SERIALPORT15_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)       \
        SERIALPORT_NUMBER(11)       \
        SERIALPORT_NUMBER(12)       \
        SERIALPORT_NUMBER(13)       \
        SERIALPORT_NUMBER(14)       \
        SERIALPORT_NUMBER(15)
#elif (SERIALPORT14_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)       \
        SERIALPORT_NUMBER(11)       \
        SERIALPORT_NUMBER(12)       \
        SERIALPORT_NUMBER(13)       \
        SERIALPORT_NUMBER(14)
#elif (SERIALPORT13_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)       \
        SERIALPORT_NUMBER(11)       \
        SERIALPORT_NUMBER(12)       \
        SERIALPORT_NUMBER(13)
#elif (SERIALPORT12_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)       \
        SERIALPORT_NUMBER(11)       \
        SERIALPORT_NUMBER(12)
#elif (SERIALPORT11_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)       \
        SERIALPORT_NUMBER(11)
#elif (SERIALPORT10_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)        \
        SERIALPORT_NUMBER(10)
#elif (SERIALPORT9_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)        \
        SERIALPORT_NUMBER(9)
#elif (SERIALPORT8_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)        \
        SERIALPORT_NUMBER(8)
#elif (SERIALPORT7_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)        \
        SERIALPORT_NUMBER(7)
#elif (SERIALPORT6_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)        \
        SERIALPORT_NUMBER(6)
#elif (SERIALPORT5_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)        \
        SERIALPORT_NUMBER(5)
#elif (SERIALPORT4_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)        \
        SERIALPORT_NUMBER(4)
#elif (SERIALPORT3_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)        \
        SERIALPORT_NUMBER(3)
#elif (SERIALPORT2_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)        \
        SERIALPORT_NUMBER(2)
#elif (SERIALPORT1_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)        \
        SERIALPORT_NUMBER(1)
#elif (SERIALPORT0_IN_USE)
    #define SERIALPORT_NUMBERS      \
        SERIALPORT_NUMBER(0)
#endif

#define SERIALPORT_NUMBER(PORT_NUMBER)                          \
void serialport_TX_port##PORT_NUMBER##_ISR(void);
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

typedef struct serialport_callback_data_t
{
    callbackFunc RXdataCallback;
    void *RXlayer;
    callbackFunc TXdataCallback;
    void *TXlayer;
} SERIALPORT_CALLBACK_DATA;

SERIALPORT_CALLBACK_DATA serialport_callback_data[SERIALPORT_NUMBER_COUNT];

#if (PLATFORM_USED == PLATFORM_CYPRESS_PSOC5)
#include <project.h>

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_0_OVERSAMPLING                     8
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_1_OVERSAMPLING                     8
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_2_OVERSAMPLING                     8
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_3_OVERSAMPLING                     8
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_4_OVERSAMPLING                     8
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_5_OVERSAMPLING                     8
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_6_OVERSAMPLING                     8
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)
#define SERIALPORT_UART_7_OVERSAMPLING                     8
#if (SERIALPORT8_IN_USE  || SERIALPORT9_IN_USE  || SERIALPORT10_IN_USE ||SERIALPORT11_IN_USE || \
     SERIALPORT12_IN_USE || SERIALPORT13_IN_USE || SERIALPORT14_IN_USE ||SERIALPORT15_IN_USE || \
     SERIALPORT16_IN_USE)
#error SERIALPORT:  Add defines for additional ports here.
#endif

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        serialport_baudrate_set(SERIALPORT_NUMBER_##PORT_NUMBER,                        \
                                SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE);            \
        UART_##PORT_NUMBER##_Start();                                                   \
        tx_complete[PORT_NUMBER] = 1;                                                   \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t divider;
    uint32_t divider_1;
    uint32_t actualBaudrate;
    uint32_t actualBaudrate_1;

    actualBaudrate = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        divider          = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY /     \
                          (SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * baudrate);    \
        if (divider < 2)                                                                \
        {                                                                               \
            divider = 2;    /* Divider lower than 2 is not possible */                  \
        }                                                                               \
        divider_1 = divider + 1;                                                        \
        actualBaudrate   = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY /     \
                          (SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * divider);     \
        actualBaudrate_1 = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY /     \
                          (SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * divider_1);   \
                                                                                        \
        /* Determine baudrate that is closer to desired baudrate */                     \
        if ((baudrate - actualBaudrate_1) < (actualBaudrate - baudrate))                \
        {                                                                               \
            divider = divider_1;                                                        \
            actualBaudrate = actualBaudrate_1;                                          \
        }                                                                               \
        UART_##PORT_NUMBER##_clock_SetDividerValue((uint16_t) divider);                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return actualBaudrate;
}   /* serialport_baudrate_set() */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                          \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                              \
    {                                                                                               \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                    \
        {                                                                                           \
            UART_##PORT_NUMBER##_RXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_6);        \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            UART_##PORT_NUMBER##_RXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_7);        \
        }                                                                                           \
        UART_##PORT_NUMBER##_RXInterrupt_Enable();                                                  \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_RXInterrupt_Disable();                                     \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_RXInterrupt_StartEx(address);                              \
        /*                                                                              \
        UART_##PORT_NUMBER##_RXInterrupt_Disable();                                     \
        UART_##PORT_NUMBER##_RXInterrupt_SetVector(address                              \
        UART_##PORT_NUMBER##_RXInterrupt_SetPriority(                                   \
            (uint8_t)UART_##PORT_NUMBER##_RXInterrupt_INTC_PRIOR_NUMBER - 1);           \
        */                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            UART_##PORT_NUMBER##_TXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_6);            \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            UART_##PORT_NUMBER##_TXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_7);            \
        }                                                                                               \
        UART_##PORT_NUMBER##_TXInterrupt_Enable();                                                      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_TXInterrupt_Disable();                                     \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_TXInterrupt_StartEx(address);                              \
        /*                                                                              \
        UART_##PORT_NUMBER##_TXInterrupt_Disable();                                     \
        UART_##PORT_NUMBER##_TXInterrupt_SetVector(address);                            \
        UART_##PORT_NUMBER##_TXInterrupt_SetPriority(                                   \
            (uint8_t)UART_##PORT_NUMBER##_TXInterrupt_INTC_PRIOR_NUMBER - 1);           \
        */                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;
    ch = 0;

    #if UART_0_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_0_ReadRxData() should be called instead of UART_0_RXDATA_REG
    #endif
    #if UART_1_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_1_ReadRxData() should be called instead of UART_1_RXDATA_REG
    #endif
    #if UART_2_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_2_ReadRxData() should be called instead of UART_2_RXDATA_REG
    #endif
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ch = UART_##PORT_NUMBER##_RXDATA_REG;                                           \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;
    uint8_t readStatus;

    returnStatus = SERIALPORT_RXSTATUS_NONE;

    #if UART_0_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_0_ReadRxStatus() should be called instead of "UART_0_RXSTATUS_REG & UART_0_RX_HW_MASK"
    #endif
    #if UART_1_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_1_ReadRxStatus() should be called instead of "UART_1_RXSTATUS_REG & UART_1_RX_HW_MASK"
    #endif
    #if UART_2_RX_INTERRUPT_ENABLED
    #error Do not use Internal RX interrupt ISR on this UART. UART_2_ReadRxStatus() should be called instead of "UART_2_RXSTATUS_REG & UART_2_RX_HW_MASK"
    #endif
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                  \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                      \
    {                                                                                       \
        readStatus = UART_##PORT_NUMBER##_RXSTATUS_REG & UART_##PORT_NUMBER##_RX_HW_MASK;   \
        if (readStatus & UART_##PORT_NUMBER##_RX_STS_FIFO_NOTEMPTY)                         \
        {                                                                                   \
            returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;                              \
        }                                                                                   \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Not needed for PSoC5, automatically done in serialport_ReadRxStatus(). */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_ClearRxBuffer();                                           \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_WriteTxData(data);                                         \
        UART_##PORT_NUMBER##_TXSTATUS_MASK_REG = UART_##PORT_NUMBER##_TX_STS_FIFO_EMPTY;\
        UART_##PORT_NUMBER##_TXSTATUS_REG;                                              \
        tx_complete[PORT_NUMBER] = 0;                                                   \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;
    uint32_t readStatus;

    returnStatus = SERIALPORT_TXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = UART_##PORT_NUMBER##_ReadTxStatus();                               \
        if (readStatus & UART_##PORT_NUMBER##_TX_STS_COMPLETE)                          \
        {                                                                               \
            tx_complete[PORT_NUMBER] = 1;                                               \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
        }                                                                               \
        UART_##PORT_NUMBER##_TXSTATUS_MASK_REG = UART_##PORT_NUMBER##_TX_STS_COMPLETE;  \
        UART_##PORT_NUMBER##_TXSTATUS_REG;                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    /* Not needed for PSoC5, automatically done in serialport_ReadTxStatus(). */
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        if (tx_complete[PORT_NUMBER] == 1)                                              \
        {                                                                               \
            ret = 1;                                                                    \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */



#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4)
#include <project.h>

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_0_OVERSAMPLING                     12
#define SERIALPORT_UART_0_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_1_OVERSAMPLING                     12
#define SERIALPORT_UART_1_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_2_OVERSAMPLING                     12
#define SERIALPORT_UART_2_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_3_OVERSAMPLING                     12
#define SERIALPORT_UART_3_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_4_OVERSAMPLING                     12
#define SERIALPORT_UART_4_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_5_OVERSAMPLING                     12
#define SERIALPORT_UART_5_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_6_OVERSAMPLING                     12
#define SERIALPORT_UART_6_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)
#define SERIALPORT_UART_7_OVERSAMPLING                     12
#define SERIALPORT_UART_7_FRACTIONALMULTIPLIER             32
#if (SERIALPORT8_IN_USE  || SERIALPORT9_IN_USE  || SERIALPORT10_IN_USE ||SERIALPORT11_IN_USE || \
     SERIALPORT12_IN_USE || SERIALPORT13_IN_USE || SERIALPORT14_IN_USE ||SERIALPORT15_IN_USE || \
     SERIALPORT16_IN_USE)
#error SERIALPORT:  Add defines for additional ports here.
#endif


void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    uint32_t level;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        serialport_baudrate_set(SERIALPORT_NUMBER_##PORT_NUMBER,                        \
                                SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE);            \
        UART_##PORT_NUMBER##_Start();                                                   \
        level = UART_##PORT_NUMBER##_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK / 2;               \
        UART_##PORT_NUMBER##_SetTxFifoLevel(level);                                     \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t clkDivider;
    uint32_t clkFractional;
    uint32_t actualBaudrate;

    actualBaudrate = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        clkDivider     = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY /       \
                        (SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * baudrate);      \
        if (clkDivider < 2)                                                             \
        {                                                                               \
            clkDivider = 2;    /* Divider lower than 2 is not possible */               \
        }                                                                               \
                                                                                        \
        clkFractional  = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY %       \
                        (SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * baudrate);      \
        clkFractional *= SERIALPORT_UART_##PORT_NUMBER##_FRACTIONALMULTIPLIER;          \
        clkFractional /=(SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING * baudrate);      \
                                                                                        \
        UART_##PORT_NUMBER##_clock_SetFractionalDividerRegister(                        \
                         (uint16_t) (clkDivider-1), (uint8_t) clkFractional);           \
        actualBaudrate = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY *       \
                         SERIALPORT_UART_##PORT_NUMBER##_FRACTIONALMULTIPLIER /         \
                         SERIALPORT_UART_##PORT_NUMBER##_OVERSAMPLING /                 \
                        (SERIALPORT_UART_##PORT_NUMBER##_FRACTIONALMULTIPLIER *         \
                            clkDivider +                                                \
                            clkFractional);                                             \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return actualBaudrate;
}   /* serialport_baudrate_set() */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        UART_##PORT_NUMBER##_SetRxInterruptMode(UART_##PORT_NUMBER##_INTR_RX_NOT_EMPTY);                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            UART_##PORT_NUMBER##_RXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_NORMAL_2);         \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            UART_##PORT_NUMBER##_RXInterrupt_SetPriority(PLATFORM_INTERRUPT_PRIORITY_LOW_3);            \
        }                                                                                               \
        UART_##PORT_NUMBER##_RXInterrupt_Enable();                                                      \
        }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        /* 0 - disable all possible RX interrupts */                                    \
        UART_##PORT_NUMBER##_SetRxInterruptMode(0);                                     \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_RXInterrupt_StartEx(address);                              \
        /*                                                                              \
        UART_##PORT_NUMBER##_RXInterrupt_Disable();                                     \
        UART_##PORT_NUMBER##_RXInterrupt_SetVector(address);                            \
        UART_##PORT_NUMBER##_RXInterrupt_SetPriority(                                   \
            (uint8_t)UART_##PORT_NUMBER##_RXInterrupt_INTC_PRIOR_NUMBER - 1);           \
        */                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* PSoC4 specific: the same ISR for RX and TX. Use ISR registered in serialport_RXinterrupt_register_ISR(). */

    /* Set Tx FIFO level interrupt source: */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                      \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                          \
    {                                                                                                           \
        UART_##PORT_NUMBER##_SetTxInterruptMode(UART_##PORT_NUMBER##_INTR_TX_FIFO_LEVEL);                       \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* PSoC4 specific: the same ISR for RX and TX. Use ISR registered in serialport_RXinterrupt_register_ISR(). */
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* PSoC4 specific: the same ISR for RX and TX. Use ISR registered in serialport_RXinterrupt_register_ISR(). */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;
    ch = 0;

    #if UART_0_INTERNAL_RX_SW_BUFFER
        #error UART_0_SpiUartReadRxData() should be called instead of UART_0_RX_FIFO_RD_REG
    #endif
    #if UART_1_INTERNAL_RX_SW_BUFFER
        #error UART_1_SpiUartReadRxData() should be called instead of UART_1_RX_FIFO_RD_REG
    #endif
    #if UART_2_INTERNAL_RX_SW_BUFFER
        #error UART_2_SpiUartReadRxData() should be called instead of UART_2_RX_FIFO_RD_REG
    #endif
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ch = (uint8_t) (UART_##PORT_NUMBER##_RX_FIFO_RD_REG);                           \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;
    uint32_t readStatus;

    returnStatus = SERIALPORT_RXSTATUS_NONE;

    /* UART_##PORT_NUMBER##_GetRxInterruptSource function is substituted with UART_##PORT_NUMBER##_INTR_RX_REG */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = (UART_##PORT_NUMBER##_INTR_RX_REG);                                \
        if (readStatus & UART_##PORT_NUMBER##_INTR_RX_NOT_EMPTY)                        \
        {                                                                               \
            returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;                          \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* UART_##PORT_NUMBER##_CLEAR_INTR_RX(UART_##PORT_NUMBER##_INTR_RX_NOT_EMPTY) is substituted with
       UART_##PORT_NUMBER##_INTR_RX_REG = (uint32) (UART_##PORT_NUMBER##_INTR_RX_NOT_EMPTY) */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        if (status == SERIALPORT_RXSTATUS_FIFO_NOTEMPTY)                                                \
        {                                                                                               \
            do{                                                                                         \
                UART_##PORT_NUMBER##_INTR_RX_REG = (uint32) (UART_##PORT_NUMBER##_INTR_RX_NOT_EMPTY);   \
            } while(0);                                                                                 \
        }                                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        UART_##PORT_NUMBER##_SpiUartClearRxBuffer();                                    \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #if UART_0_CHECK_TX_SW_BUFFER
    #error SW buffer must not be used. Instead of UART_0_SpiUartWriteTxData(), UART_0_TX_FIFO_WR_REG is used !
    #endif
    #if UART_1_CHECK_TX_SW_BUFFER
    #error SW buffer must not be used. Instead of UART_1_SpiUartWriteTxData(), UART_1_TX_FIFO_WR_REG is used !
    #endif
    #if UART_2_CHECK_TX_SW_BUFFER
    #error SW buffer must not be used. Instead of UART_2_SpiUartWriteTxData(), UART_2_TX_FIFO_WR_REG is used !
    #endif
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                      \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                          \
    {                                                                                                           \
        /* Write data to FIFO: */                                                                               \
        UART_##PORT_NUMBER##_TX_FIFO_WR_REG = data;                                                             \
        /* Clear Tx FIFO level interrupt status: */                                                             \
        UART_##PORT_NUMBER##_ClearTxInterruptSource(UART_##PORT_NUMBER##_INTR_TX_FIFO_LEVEL);                   \
        /* Set Tx FIFO level interrupt source: */                                                               \
        UART_##PORT_NUMBER##_SetTxInterruptMode(UART_##PORT_NUMBER##_INTR_TX_FIFO_LEVEL);                       \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;
    uint32_t readStatus;
    
    returnStatus = SERIALPORT_TXSTATUS_NONE;

    /* UART_##PORT_NUMBER##_GetTxInterruptSource() substituted with UART_##PORT_NUMBER##_INTR_TX_REG (execution from RAM) */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = UART_##PORT_NUMBER##_INTR_TX_REG;                                  \
        if (readStatus & UART_##PORT_NUMBER##_INTR_TX_FIFO_LEVEL)                       \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status) PROC_RAM_FUNCTION;
void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                  \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                      \
    {                                                                                                       \
        if (status == SERIALPORT_TXSTATUS_FIFO_EMPTY)                                                       \
        {                                                                                                   \
            /* Clear Tx FIFO level interrupt source: */                                                     \
            UART_##PORT_NUMBER##_SetTxInterruptMode(0);                                                     \
            /* Clear Tx FIFO level interrupt status: */                                                     \
            UART_##PORT_NUMBER##_ClearTxInterruptSource(UART_##PORT_NUMBER##_INTR_TX_FIFO_LEVEL);           \
        }                                                                                                   \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        if (UART_##PORT_NUMBER##_GET_TX_FIFO_SR_VALID != 1)                             \
        {                                                                               \
            ret = 1;                                                                    \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */


#elif (PLATFORM_USED == PLATFORM_STM8_STM8S207)

#include "../STM8_STM8S207/STM8S/stm8s_clk.h"

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        /* DEINIT before INIT */
        /* Clear the Idle Line Detected bit in the status rerister by a read
        to the UART1_SR register followed by a Read to the UART1_DR register */
        (void)UART1->SR;
        (void)UART1->DR;
        UART1->BRR2 =   (uint8_t)0x00;
        UART1->BRR1 =   (uint8_t)0x00;
        UART1->CR1 =    (uint8_t)0x00;
        UART1->CR2 =    (uint8_t)0x00;
        UART1->CR3 =    (uint8_t)0x00;
        UART1->CR4 =    (uint8_t)0x00;
        UART1->CR5 =    (uint8_t)0x00;
        UART1->GTR =    (uint8_t)0x00;
        UART1->PSCR =   (uint8_t)0x00;
        /* DISABLE UART1 */
        UART1->CR1 |= UART1_CR1_UARTD;

        /* INIT */
        serialport_baudrate_set(SERIALPORT_NUMBER_0, SERIALPORT0_DEFAULT_BAUDRATE);
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        /* DEINIT before INIT */
        /* Clear the Idle Line Detected bit in the status rerister by a read
        to the UART3_SR register followed by a Read to the UART3_DR register */
        (void) UART3->SR;
        (void) UART3->DR;
        UART3->BRR2 = UART3_BRR2_RESET_VALUE; /*Set UART3_BRR2 to reset value 0x00 */
        UART3->BRR1 = UART3_BRR1_RESET_VALUE; /*Set UART3_BRR1 to reset value 0x00 */
        UART3->CR1 = UART3_CR1_RESET_VALUE;  /*Set UART3_CR1 to reset value 0x00  */
        UART3->CR2 = UART3_CR2_RESET_VALUE;  /*Set UART3_CR2 to reset value 0x00  */
        UART3->CR3 = UART3_CR3_RESET_VALUE;  /*Set UART3_CR3 to reset value 0x00  */
        UART3->CR4 = UART3_CR4_RESET_VALUE;  /*Set UART3_CR4 to reset value 0x00  */
        UART3->CR6 = UART3_CR6_RESET_VALUE;  /*Set UART3_CR6 to reset value 0x00  */

        /* INIT */
        serialport_baudrate_set(SERIALPORT_NUMBER_1, SERIALPORT1_DEFAULT_BAUDRATE);
    }
    #endif
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t actualBaudrate = 0;

    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        uint32_t BaudRate_Mantissa    = 0;
        uint32_t BaudRate_Mantissa100 = 0;
        /* Clear the word length bit */
        UART1->CR1 &= (uint8_t)(~UART1_CR1_M);
    
        /* Set the word length bit to 8 bits data */
        UART1->CR1 |= (uint8_t)0x00;

        /* Clear the STOP bits */
        UART1->CR3 &= (uint8_t)(~UART1_CR3_STOP);
        /* Set the STOP bits number to stop bit 1, one stop bit is transmitted at the end of the frame */
        UART1->CR3 |= (uint8_t)0x00;
        
        /* Clear the Parity Control bit */
        UART1->CR1 &= (uint8_t)(~(UART1_CR1_PCEN | UART1_CR1_PS  ));
        /* Set the Parity Control bit to parity NONE */
        UART1->CR1 |= (uint8_t)0x00;
        
        /* Clear the LSB mantissa of UART1DIV  */
        UART1->BRR1 &= (uint8_t)(~UART1_BRR1_DIVM);
        /* Clear the MSB mantissa of UART1DIV  */
        UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVM);
        /* Clear the Fraction bits of UART1DIV */
        UART1->BRR2 &= (uint8_t)(~UART1_BRR2_DIVF);
        
        /* Set the UART1 BaudRates in BRR1 and BRR2 registers according to the SERIALPORT_BAUDRATE value */
        BaudRate_Mantissa    = ((uint32_t)platform_uart_clock_source_frequency_get(1) / ((uint32_t)baudrate << 4));
        BaudRate_Mantissa100 = (((uint32_t)platform_uart_clock_source_frequency_get(1) * 100) / ((uint32_t)baudrate << 4));
        /* Set the fraction of UART1DIV  */
        UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F); 
        /* Set the MSB mantissa of UART1DIV  */
        UART1->BRR2 |= (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0); 
        /* Set the LSB mantissa of UART1DIV  */
        UART1->BRR1 |= (uint8_t)BaudRate_Mantissa;
        
        /* Disable the Transmitter and Receiver before seting the LBCL, CPOL and CPHA bits */
        UART1->CR2 &= (uint8_t)~(UART1_CR2_TEN | UART1_CR2_REN); 
        /* Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
        UART1->CR3 &= (uint8_t)~(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL);
        /* Set the Clock Polarity, lock Phase, Last Bit Clock pulse. 0x80 sync mode disable */
        UART1->CR3 |= (uint8_t)((uint8_t)0x80 & (uint8_t)(UART1_CR3_CPOL | UART1_CR3_CPHA | UART1_CR3_LBCL));
        
        /* Set the Transmitter Enable bit */
        UART1->CR2 |= (uint8_t)UART1_CR2_TEN;
        /* Set the Receiver Enable bit */
        UART1->CR2 |= (uint8_t)UART1_CR2_REN;
        /* Clear the Clock Enable bit */
        UART1->CR3 &= (uint8_t)(~UART1_CR3_CKEN);

        actualBaudrate = ((uint32_t)platform_uart_clock_source_frequency_get(1) / ((UART1->BRR1)<<4|(UART1->BRR2)));
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        uint8_t BRR2_1                = 0;
        uint8_t BRR2_2                = 0;
        uint32_t BaudRate_Mantissa    = 0; 
        uint32_t BaudRate_Mantissa100 = 0;
        
        /* Clear the word length bit */
        UART3->CR1 &= (uint8_t)(~UART3_CR1_M);
        /* Set the word length bit for 8 bit WORD */
        UART3->CR1 |= (uint8_t)0x00; 
        
        /* Clear the STOP bits */
        UART3->CR3 &= (uint8_t)(~UART3_CR3_STOP);  
        /* One stop bit is  transmitted at the end of frame */
        UART3->CR3 |= (uint8_t)0x00;
        
        /* Clear the Parity Control bit */
        UART3->CR1 &= (uint8_t)(~(UART3_CR1_PCEN | UART3_CR1_PS));
        /* Set the Parity Control bit to parity NONE */
        UART3->CR1 |= (uint8_t)0x00;
        
        /* Clear the LSB mantissa of UART3DIV  */
        UART3->BRR1 &= (uint8_t)(~UART3_BRR1_DIVM);
        /* Clear the MSB mantissa of UART3DIV  */
        UART3->BRR2 &= (uint8_t)(~UART3_BRR2_DIVM);
        /* Clear the Fraction bits of UART3DIV */
        UART3->BRR2 &= (uint8_t)(~UART3_BRR2_DIVF);
        
        /* Set the UART3 BaudRates in BRR1 and BRR2 registers according to UART3_BaudRate value */
        BaudRate_Mantissa    = ((uint32_t)platform_uart_clock_source_frequency_get(3) / (baudrate << 4));
        BaudRate_Mantissa100 = (((uint32_t)platform_uart_clock_source_frequency_get(3) * 100) / (baudrate << 4));
        /* The fraction and MSB mantissa should be loaded in one step in the BRR2 register */
        /* Set the fraction of UART3DIV  */
        BRR2_1 = (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100))
                                        << 4) / 100) & (uint8_t)0x0F); 
        BRR2_2 = (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);
        
        UART3->BRR2 = (uint8_t)(BRR2_1 | BRR2_2);
        /* Set the LSB mantissa of UART3DIV  */
        UART3->BRR1 = (uint8_t)BaudRate_Mantissa;
        UART3->CR1 &= (uint8_t)(~UART3_CR1_UARTD);

        /* Set the Transmitter Enable bit */
        UART3->CR2 |= UART3_CR2_TEN;
        /* Set the Receiver Enable bit */
        UART3->CR2 |= UART3_CR2_REN;

        actualBaudrate = ((uint32_t)platform_uart_clock_source_frequency_get(3) / ((UART3->BRR1)<<4|(UART3->BRR2)));
    }
    #endif

    return actualBaudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // UART1_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Set the Receiver Enable bit */
        UART1->CR2 |= (uint8_t)UART1_CR2_REN;  
    
        /* Get the UART1 register index */
        uartreg = (uint8_t)((uint16_t)0x0205 >> 0x08);
        /* Get the UART1 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0205 & (uint8_t)0x0F));

        /* Enable the Interrupt bits according to mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 |= itpos;
        }
        else
        {
            UART1->CR4 |= itpos;
        }

        /* ENABLE UART1 */
        UART1->CR1 &= (uint8_t)(~UART1_CR1_UARTD); 
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        //UART3_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Get the UART3 register index */
        uartreg = (uint8_t)((uint16_t)0x0205 >> 0x08);
        
        /* Get the UART3 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0205 & (uint8_t)0x0F));

        /* Enable the Interrupt bits according to 0x0205 mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 |= itpos;
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 |= itpos;
        }
        else
        {
            UART3->CR6 |= itpos;
        }
    }
    #endif
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // UART1_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Clear the Receiver Disable bit */
        UART1->CR2 &= (uint8_t)(~UART1_CR2_REN);
    
        /* Get the UART1 register index */
        uartreg = (uint8_t)((uint16_t)0x0205 >> 0x08);
        /* Get the UART1 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0205 & (uint8_t)0x0F));

        /* Disable the interrupt bits according to mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 &= (uint8_t)(~itpos);
        }
        else
        {
            UART1->CR4 &= (uint8_t)(~itpos);
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        //UART3_IT_RXNE_OR    = (uint16_t)0x0205  /*!< Receive/Overrun interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Get the UART3 register index */
        uartreg = (uint8_t)((uint16_t)0x0205 >> 0x08);
        
        /* Get the UART3 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0205 & (uint8_t)0x0F));

        /* Disable the interrupt bits according to UART3_IT mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 &= (uint8_t)(~itpos);
        }
        else
        {
            UART3->CR6 &= (uint8_t)(~itpos);
        }
    }
    #endif
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* STM8 interrupt vector table is defined in stm8_interrupt_vector.c  */
    /* irq18 0x8050 UART1 RX register DATA FULL */
    /* irq21 0x805C UART3 RX register DATA FULL */
}   /* serialport_RXinterrupt_register_ISR */

#pragma PROC_RAM_FUNCTION_START
#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER
#pragma PROC_RAM_FUNCTION_END

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // UART1_IT_TXE        = (uint16_t)0x0277, /*!< Transmit interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Set the Transmitter Enable bit */
        UART1->CR2 |= (uint8_t)UART1_CR2_TEN;
    
        /* Get the UART1 register index */
        uartreg = (uint8_t)((uint16_t)0x0277 >> 0x08);
        /* Get the UART1 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0277 & (uint8_t)0x0F));

        /* Enable the Interrupt bits according to mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 |= itpos;
        }
        else
        {
            UART1->CR4 |= itpos;
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        //UART3_IT_TXE        = (uint16_t)0x0277, /**< Transmit interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Get the UART3 register index */
        uartreg = (uint8_t)((uint16_t)0x0277 >> 0x08);
        /* Get the UART3 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0277 & (uint8_t)0x0F));

        /* Enable the Interrupt bits according to 0x0277 mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 |= itpos;
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 |= itpos;
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 |= itpos;
        }
        else
        {
            UART3->CR6 |= itpos;
        }
    }
    #endif
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // UART1_IT_TXE        = (uint16_t)0x0277, /*!< Transmit interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Clear the Transmitter Disable bit */
        UART1->CR2 &= (uint8_t)(~UART1_CR2_TEN); 

        /* Get the UART1 register index */
        uartreg = (uint8_t)((uint16_t)0x0277 >> 0x08);
        /* Get the UART1 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0277 & (uint8_t)0x0F));

        /* Disable the interrupt bits according to mask */
        if (uartreg == 0x01)
        {
            UART1->CR1 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART1->CR2 &= (uint8_t)(~itpos);
        }
        else
        {
            UART1->CR4 &= (uint8_t)(~itpos);
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        //UART3_IT_TXE        = (uint16_t)0x0277, /**< Transmit interrupt */
        uint8_t uartreg = 0, itpos = 0x00;

        /* Get the UART3 register index */
        uartreg = (uint8_t)((uint16_t)0x0277 >> 0x08);
        
        /* Get the UART3 IT index */
        itpos = (uint8_t)((uint8_t)1 << (uint8_t)((uint8_t)0x0277 & (uint8_t)0x0F));

        /* Disable the interrupt bits according to 0x0277 mask */
        if (uartreg == 0x01)
        {
            UART3->CR1 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x02)
        {
            UART3->CR2 &= (uint8_t)(~itpos);
        }
        else if (uartreg == 0x03)
        {
            UART3->CR4 &= (uint8_t)(~itpos);
        }
        else
        {
            UART3->CR6 &= (uint8_t)(~itpos);
        }
    }
    #endif
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* STM8 interrupt vector table is defined in stm8_interrupt_vector.c  */
    /* irq17 0x804C UART1 TX complete */
    /* irq20 0x8058 UART3 TX complete */
}   /* serialport_TXinterrupt_register_ISR */

#pragma PROC_RAM_FUNCTION_START
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t data;
    data = 0;

    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        data = ((uint8_t)UART1->DR);
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        data = ((uint8_t)UART3->DR);
    }
    #endif

    return data;
}   /* serialport_ReadRxData */
#pragma PROC_RAM_FUNCTION_END

#pragma PROC_RAM_FUNCTION_START
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS status = SERIALPORT_RXSTATUS_NONE;

    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // 0x0020 = Read Data Register Not Empty flag
        uint16_t flagStatus = 0x0000;
        if ((UART1->SR & (uint8_t)0x0020) != (uint8_t)0x00)
        {
            /* UART1_FLAG is set*/
            status = SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
        }
        else
        {
            /* UART1_FLAG is reset*/
            status = SERIALPORT_RXSTATUS_NONE;
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        // UART3_FLAG_RXNE        = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
        uint16_t flagStatus = 0x0000;

        if ((UART3->SR & (uint8_t)0x0020) != (uint8_t)0x00)
        {
            /* UART3_FLAG is set*/
            status = SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = SERIALPORT_RXSTATUS_NONE;
        }
    }
    #endif

   return status;
}   /* serialport_ReadRxStatus */
#pragma PROC_RAM_FUNCTION_END

#pragma PROC_RAM_FUNCTION_START
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        if (status == SERIALPORT_RXSTATUS_FIFO_NOTEMPTY)
        {
            UART1->SR = (uint8_t)~(UART1_SR_RXNE);
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        if (status == SERIALPORT_RXSTATUS_FIFO_NOTEMPTY)
        {
            UART3->SR = (uint8_t)~(UART3_SR_RXNE);
        }
    }
    #endif
}   /* serialport_ClearRxStatus */
#pragma PROC_RAM_FUNCTION_END

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    /* Not needed on STM8S. */
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    serialport_TXinterruptEnable(portNumber);
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        /* Transmit Data */
        UART1->DR = data;
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        /* Transmit Data */
        UART3->DR = data;
    }
    #endif
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS status = SERIALPORT_TXSTATUS_NONE;

    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        // 0x0080 = Transmit Data Register Empty flag
        uint16_t flagStatus = 0x0000;
        if ((UART1->SR & (uint8_t)0x0080) != (uint8_t)0x00)
        {
            /* UART1_FLAG is set*/
            status = SERIALPORT_TXSTATUS_FIFO_EMPTY;
        }
        else
        {
            /* UART1_FLAG is reset*/
            status = SERIALPORT_TXSTATUS_NONE;
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        // UART3_FLAG_TXE         = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
        uint16_t flagStatus = 0x0000;

        if ((UART3->SR & (uint8_t)0x0080) != (uint8_t)0x00)
        {
            /* UART3_FLAG is set*/
            status = SERIALPORT_TXSTATUS_FIFO_EMPTY;
        }
        else
        {
            /* UART3_FLAG is reset*/
            status = SERIALPORT_TXSTATUS_NONE;
        }
    }
    #endif

   return status;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    #if (SERIALPORT0_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        if (status != SERIALPORT_TXSTATUS_FIFO_EMPTY)
        {
            /* TXE flag is cleared only by a write to the UART1_DR register */
            UART1->DR = 0x00;
        }
    }
    #endif
    #if (SERIALPORT1_IN_USE)
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        if (status != SERIALPORT_TXSTATUS_FIFO_EMPTY)
        {
            /* TXE flag is cleared only by a write to the UART1_DR register */
            UART3->DR = 0x00;
        }
    }
    #endif
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    #warning TODO:
    
    /* 
    Return:
         1 - Transmitting is not in progress (shift register empty)
         0 - transmitting is in progress (shift register not empty)
    */
    
    return 1;
}   /* serialport_TxComplete */

INTERRUPT void UART1TXInterrupt(void)
{
    #if (SERIALPORT0_IN_USE)
    while(serialport_ReadTxStatus(SERIALPORT_NUMBER_0) == SERIALPORT_TXSTATUS_NONE);
    serialport_TXinterruptDisable(SERIALPORT_NUMBER_0);
    #endif
}

INTERRUPT void UART1RXInterrupt(void)
{
    #if (SERIALPORT0_IN_USE)
    serialport_RX_port0_ISR();
    #endif
}

INTERRUPT void UART3TXInterrupt(void)
{
    #if (SERIALPORT1_IN_USE)
    while(serialport_ReadTxStatus(SERIALPORT_NUMBER_1) == SERIALPORT_TXSTATUS_NONE);
    serialport_TXinterruptDisable(SERIALPORT_NUMBER_1);
    #endif
}

INTERRUPT void UART3RXInterrupt(void)
{
    #if (SERIALPORT1_IN_USE)
    serialport_RX_port1_ISR();
    #endif
}

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9AF114MA)

#include "mcu.h"
#include "gpio.h"

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    stc_mfs47_uart_smr_field_t  stcSMR  = { 0u };
    stc_mfs47_uart_scr_field_t  stcSCR  = { 0u };
    stc_mfs47_uart_escr_field_t stcESCR = { 0u };

    #if SERIALPORT0_IN_USE
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        SetPinFunc_SIN0_0();
        SetPinFunc_SOT0_0();
        //SetPinFunc_SIN0_1();
        //SetPinFunc_SOT0_1();
    }
    #endif

    #if SERIALPORT1_IN_USE
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        SetPinFunc_SIN1_1();
        SetPinFunc_SOT1_1();
    }
    #endif

    #if SERIALPORT2_IN_USE
    if (portNumber == SERIALPORT_NUMBER_2)
    {
        SetPinFunc_SIN2_2();
        SetPinFunc_SOT2_2();
    }
    #endif

    #if SERIALPORT3_IN_USE
    if (portNumber == SERIALPORT_NUMBER_3)
    {
        //SetPinFunc_SIN3_1();
        //SetPinFunc_SOT3_1();
        SetPinFunc_SIN3_2();
        SetPinFunc_SOT3_2();
    }
    #endif

    #if SERIALPORT4_IN_USE
    if (portNumber == SERIALPORT_NUMBER_4)
    {
        //SetPinFunc_SIN4_0();
        //SetPinFunc_SOT4_0();
        SetPinFunc_SIN4_1();
        SetPinFunc_SOT4_1();
        //SetPinFunc_SIN4_2();
        //SetPinFunc_SOT4_2();
    }
    #endif

    #if SERIALPORT5_IN_USE
    if (portNumber == SERIALPORT_NUMBER_5)
    {
        SetPinFunc_SIN5_0();
        SetPinFunc_SOT5_0();
        //SetPinFunc_SIN5_2();
        //SetPinFunc_SOT5_2();
    }
    #endif

    #if SERIALPORT6_IN_USE
    if (portNumber == SERIALPORT_NUMBER_6)
    {
        //SetPinFunc_SIN6_0();
        //SetPinFunc_SOT6_0();
        SetPinFunc_SIN6_1();
        SetPinFunc_SOT6_1();
    }
    #endif

    #if SERIALPORT7_IN_USE
    if (portNumber == SERIALPORT_NUMBER_7)
    {
        SetPinFunc_SIN7_1();
        SetPinFunc_SOT7_1();
    }
    #endif
    
    #if (SERIALPORT8_IN_USE  || SERIALPORT9_IN_USE  || SERIALPORT10_IN_USE || SERIALPORT11_IN_USE || \
         SERIALPORT12_IN_USE || SERIALPORT13_IN_USE || SERIALPORT14_IN_USE || SERIALPORT15_IN_USE)
        #error serialport.c: Hardware port 8 - 15 not available !
    #endif


    stcSMR.MD = 0;                      /* Normal mode */
    stcSMR.SOE = 1;                     /* Enable Serial data output enable bit */ 
    stcESCR.L0 = (0 & 0x01) >> 0 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.L1 = (0 & 0x02) >> 1 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.L2 = (0 & 0x04) >> 2 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.P   = 0;                    /* Parity type selection (0-even/1-odd) */
    stcESCR.PEN = 0;                    /* Parity disable */
    stcSMR.SBL   = 0;                   /* One stop bit */
    stcESCR.ESBL = 0;                   /* One stop bit */
    stcSMR.BDS = 0;                     /* Bit direction: LSB first */
    stcESCR.INV = 0;                    /* Don't set Signal system (NRZ/Int-NRZ) (invert data)*/
    stcESCR.FLWEN = 0;                  /* HW Flow control: off */


    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                                  \
    {                                                                                                                                   \
        tx_complete[portNumber] = 1;                                                                                                    \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.UPCL = 1;      /* Clear MFS by setting the Software Reset bit */                             \
                                                                                                                                        \
        serialport_baudrate_set(portNumber, SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE);                                                \
                                                                                                                                        \
        /* Set registers value */                                                                                                       \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SMR_f  = stcSMR;                                                        \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f  = stcSCR;                                                        \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->ESCR_f = stcESCR;                                                       \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->BGR_f.EXT = 0;                               /* Set internal clock */                              \
                                                                                                                                        \
        /* Configure FIFO */                                                                                                            \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL1 = 1;  /* Reset FIFO */                                      \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL2 = 1;  /* Reset FIFO */                                      \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR1_f.FRIIE = 1; /* Enable FIFO receive Idle detection */              \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR1_f.FSEL = 0;  /* Transmit FIFO:FIFO1, Received FIFO:FIFO2 */        \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FBYTE1 = 0;       /* Set FIFO count */                                  \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FBYTE2 = 8;       /* Set FIFO count */                                  \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FE1 = 1;   /* Enable FIFO  */                                    \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FE2 = 1;   /* Enable FIFO  */                                    \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.RIE = 1;     /* Enable receive interrupt */                                                  \
                                                        /* (Tx interrupt is enabled at Transmit in serialport_WriteTxData() function)*/ \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.TXE = 1;     /* Enable Tx */                                                                 \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.RXE = 1;     /* Enable Rx */                                                                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t u32Pclk1;
    uint32_t bgr;
    uint32_t bgrPlus1;
    uint8_t bgr0, bgr1;
    uint32_t actualBaudrate; 
    uint32_t actualBaudratePlus1;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                      \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                          \
    {                                                                                                           \
        u32Pclk1 = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY;                                      \
                                                                                                                \
        bgr = (u32Pclk1/((uint32_t)baudrate)) - 1;                                                              \
        bgrPlus1 = bgr + 1;                                                                                     \
        actualBaudrate = u32Pclk1 / (bgr + 1);                                                                  \
        actualBaudratePlus1 = u32Pclk1 / (bgrPlus1 + 1);                                                        \
                                                                                                                \
        /* Determine baudrate that is closer to desired baudrate */                                             \
        if ((baudrate - actualBaudratePlus1) < (actualBaudrate - baudrate))                                     \
        {                                                                                                       \
            bgr = bgrPlus1;                                                                                     \
            actualBaudrate = actualBaudratePlus1;                                                               \
        }                                                                                                       \
        bgr0 = (uint8_t) (bgr & 0x000000FF);                                                                    \
        bgr1 = ((FM3_MFS##PORT_NUMBER##_UART->BGR1_f.EXT)<<7) | ((uint8_t)((bgr & 0x0000EF00)>>8));             \
                                                                                                                \
        FM3_MFS##PORT_NUMBER##_UART->BGR0 = bgr0;                                                               \
        FM3_MFS##PORT_NUMBER##_UART->BGR1 = bgr1;                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return actualBaudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        serialport_ClearRxBuffer(portNumber);                                                           \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##RX_IRQn);                                                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##RX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_14);            \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##RX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_15);            \
        }                                                                                               \
        NVIC_EnableIRQ(MFS##PORT_NUMBER##RX_IRQn);                                                      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        NVIC_DisableIRQ(MFS##PORT_NUMBER##RX_IRQn);                                     \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##RX_IRQn);                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(MFS##PORT_NUMBER##RX_IRQn, (uint32_t)address);                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##TX_IRQn);                                                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##TX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_14);            \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##TX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_15);            \
        }                                                                                               \
        NVIC_EnableIRQ(MFS##PORT_NUMBER##TX_IRQn);                                                      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        NVIC_DisableIRQ(MFS##PORT_NUMBER##TX_IRQn);                                     \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##TX_IRQn);                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(MFS##PORT_NUMBER##TX_IRQn, (uint32_t)address);                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;

    ch = 0;
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ch = FM3_MFS##PORT_NUMBER##_UART->RDR;                                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;
    stc_mfs47_uart_ssr_field_t readStatus;

    returnStatus = SERIALPORT_RXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f;     \
                                                                                        \
        if ((readStatus.FRE == 0) && (readStatus.RDRF == 1))                            \
        {                                                                               \
            returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;                          \
        }                                                                               \
        /* Clear Framing error, Overrun error and Parity error:  */                     \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f.REC = 1;          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Rx status flag is cleared automatically by reading from the received data register (RDR) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL2 = 1;        \
        serialport_ReadRxData(portNumber);                                              \
        serialport_ClearRxStatus(portNumber, SERIALPORT_RXSTATUS_FIFO_NOTEMPTY);        \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        tx_complete[portNumber] = 0;                                                    \
        FM3_MFS##PORT_NUMBER##_UART->TDR = (uint16_t) data;                             \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 1;          \
        ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TBIE = 1;         \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;
    stc_mfs47_uart_ssr_field_t readStatus;

    returnStatus = SERIALPORT_TXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f;     \
                                                                                        \
        if (readStatus.TBI == 1)                                                        \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
            tx_complete[portNumber] = 1;                                                \
            ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 0;      \
            ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TBIE = 0;     \
        }                                                                               \
        else if (readStatus.TDRE == 1)                                                  \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
            ((FM3_MFS47_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 0;      \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    /* Already done in serialport_ReadTxStatus(). */
}   /* serialport_ClearTxStatus */


uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        if (tx_complete[PORT_NUMBER] == 1)                                              \
        {                                                                               \
            ret = 1;                                                                    \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_CYPRESS_MB9BF121J)

#include "mcu.h"
#include "gpio.h"

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    stc_mfs_uart_smr_field_t  stcSMR  = { 0u };
    stc_mfs_uart_scr_field_t  stcSCR  = { 0u };
    stc_mfs_uart_escr_field_t stcESCR = { 0u };

    #if SERIALPORT0_IN_USE
    if (portNumber == SERIALPORT_NUMBER_0)
    {
        SetPinFunc_SIN0_0();
        SetPinFunc_SOT0_0();
        //SetPinFunc_SIN0_1();
        //SetPinFunc_SOT0_1();
    }
    #endif

    #if SERIALPORT1_IN_USE
    if (portNumber == SERIALPORT_NUMBER_1)
    {
        SetPinFunc_SIN1_1();
        SetPinFunc_SOT1_1();
    }
    #endif

    #if SERIALPORT2_IN_USE
    if (portNumber == SERIALPORT_NUMBER_2)
    {
        SetPinFunc_SIN2_0();
        SetPinFunc_SOT2_0();
    }
    #endif

    #if (SERIALPORT3_IN_USE || SERIALPORT4_IN_USE)
        #error serialport.c: Hardware port 3 and 4 not available !
    #endif

    #if SERIALPORT5_IN_USE
    if (portNumber == SERIALPORT_NUMBER_3)
    {
        SetPinFunc_SIN5_1();
        SetPinFunc_SOT5_1();
    }
    #endif

    #if (SERIALPORT6_IN_USE  || SERIALPORT7_IN_USE  || \
         SERIALPORT8_IN_USE  || SERIALPORT9_IN_USE  || SERIALPORT10_IN_USE || SERIALPORT11_IN_USE || \
         SERIALPORT12_IN_USE || SERIALPORT13_IN_USE || SERIALPORT14_IN_USE || SERIALPORT15_IN_USE)
        #error serialport.c: Hardware port 6 - 15 not available !
    #endif


    stcSMR.MD = 0;                      /* Normal mode */
    stcSMR.SOE = 1;                     /* Enable Serial data output enable bit */ 
    stcESCR.L0 = (0 & 0x01) >> 0 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.L1 = (0 & 0x02) >> 1 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.L2 = (0 & 0x04) >> 2 ;      /* 0=8 bits,  1=5 bits, 2=6 bits, 3=7 bits, 4=9 bit */
    stcESCR.P   = 0;                    /* Parity type selection (0-even/1-odd) */
    stcESCR.PEN = 0;                    /* Parity disable */
    stcSMR.SBL   = 0;                   /* One stop bit */
    stcESCR.ESBL = 0;                   /* One stop bit */
    stcSMR.BDS = 0;                     /* Bit direction: LSB first */
    stcESCR.INV = 0;                    /* Don't set Signal system (NRZ/Int-NRZ) (invert data)*/
    stcESCR.FLWEN = 0;                  /* HW Flow control: off */


    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                                  \
    {                                                                                                                                   \
        tx_complete[portNumber] = 1;                                                                                                    \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.UPCL = 1;      /* Clear MFS by setting the Software Reset bit */                             \
                                                                                                                                        \
        serialport_baudrate_set(portNumber, SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE);                                                \
                                                                                                                                        \
        /* Set registers value */                                                                                                       \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SMR_f  = stcSMR;                                                          \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f  = stcSCR;                                                          \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->ESCR_f = stcESCR;                                                         \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->BGR_f.EXT = 0;                               /* Set internal clock */                              \
                                                                                                                                        \
        /* Configure FIFO */                                                                                                            \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL1 = 1;  /* Reset FIFO */                                        \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL2 = 1;  /* Reset FIFO */                                        \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR1_f.FRIIE = 1; /* Enable FIFO receive Idle detection */                \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR1_f.FSEL = 0;  /* Transmit FIFO:FIFO1, Received FIFO:FIFO2 */          \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FBYTE1 = 0;       /* Set FIFO count */                                    \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FBYTE2 = 8;       /* Set FIFO count */                                    \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FE1 = 1;   /* Enable FIFO  */                                      \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FE2 = 1;   /* Enable FIFO  */                                      \
                                                                                                                                        \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.RIE = 1;     /* Enable receive interrupt */                                                  \
                                                        /* (Tx interrupt is enabled at Transmit in serialport_WriteTxData() function)*/ \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.TXE = 1;     /* Enable Tx */                                                                 \
        FM3_MFS##PORT_NUMBER##_UART->SCR_f.RXE = 1;     /* Enable Rx */                                                                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t u32Pclk1;
    uint32_t bgr;
    uint32_t bgrPlus1;
    uint8_t bgr0, bgr1;
    uint32_t actualBaudrate; 
    uint32_t actualBaudratePlus1;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                      \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                          \
    {                                                                                                           \
        u32Pclk1 = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY;                                      \
                                                                                                                \
        bgr = (u32Pclk1/((uint32_t)baudrate)) - 1;                                                              \
        bgrPlus1 = bgr + 1;                                                                                     \
        actualBaudrate = u32Pclk1 / (bgr + 1);                                                                  \
        actualBaudratePlus1 = u32Pclk1 / (bgrPlus1 + 1);                                                        \
                                                                                                                \
        /* Determine baudrate that is closer to desired baudrate */                                             \
        if ((baudrate - actualBaudratePlus1) < (actualBaudrate - baudrate))                                     \
        {                                                                                                       \
            bgr = bgrPlus1;                                                                                     \
            actualBaudrate = actualBaudratePlus1;                                                               \
        }                                                                                                       \
        bgr0 = (uint8_t) (bgr & 0x000000FF);                                                                    \
        bgr1 = ((FM3_MFS##PORT_NUMBER##_UART->BGR1_f.EXT)<<7) | ((uint8_t)((bgr & 0x0000EF00)>>8));             \
                                                                                                                \
        FM3_MFS##PORT_NUMBER##_UART->BGR0 = bgr0;                                                               \
        FM3_MFS##PORT_NUMBER##_UART->BGR1 = bgr1;                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return actualBaudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        serialport_ClearRxBuffer(portNumber);                                                           \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##RX_IRQn);                                                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##RX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_14);            \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##RX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_15);            \
        }                                                                                               \
    NVIC_EnableIRQ(MFS##PORT_NUMBER##RX_IRQn);                                                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        NVIC_DisableIRQ(MFS##PORT_NUMBER##RX_IRQn);                                     \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##RX_IRQn);                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(MFS##PORT_NUMBER##RX_IRQn, (uint32_t)address);                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                  \
    {                                                                                                   \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##TX_IRQn);                                                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                        \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##TX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_14);            \
        }                                                                                               \
        else                                                                                            \
        {                                                                                               \
            NVIC_SetPriority(MFS##PORT_NUMBER##TX_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_15);            \
        }                                                                                               \
        NVIC_EnableIRQ(MFS##PORT_NUMBER##TX_IRQn);                                                      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        NVIC_DisableIRQ(MFS##PORT_NUMBER##TX_IRQn);                                     \
        NVIC_ClearPendingIRQ(MFS##PORT_NUMBER##TX_IRQn);                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(MFS##PORT_NUMBER##TX_IRQn, (uint32_t)address);                 \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;

    ch = 0;
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ch = FM3_MFS##PORT_NUMBER##_UART->RDR;                                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;
    stc_mfs_uart_ssr_field_t readStatus;

    returnStatus = SERIALPORT_RXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f;       \
                                                                                        \
        if ((readStatus.FRE == 0) && (readStatus.RDRF == 1))                            \
        {                                                                               \
            returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;                          \
        }                                                                               \
        /* Clear Framing error, Overrun error and Parity error:  */                     \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f.REC = 1;            \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Rx status flag is cleared automatically by reading from the received data register (RDR) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->FCR0_f.FCL2 = 1;          \
        serialport_ReadRxData(portNumber);                                              \
        serialport_ClearRxStatus(portNumber, SERIALPORT_RXSTATUS_FIFO_NOTEMPTY);        \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        tx_complete[portNumber] = 0;                                                    \
        FM3_MFS##PORT_NUMBER##_UART->TDR = (uint16_t) data;                             \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 1;            \
        ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TBIE = 1;           \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;
    stc_mfs_uart_ssr_field_t readStatus;

    returnStatus = SERIALPORT_TXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        readStatus = ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SSR_f;       \
                                                                                        \
        if (readStatus.TBI == 1)                                                        \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
            tx_complete[portNumber] = 1;                                                \
            ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 0;        \
            ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TBIE = 0;       \
        }                                                                               \
        else if (readStatus.TDRE == 1)                                                  \
        {                                                                               \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                             \
            ((FM3_MFS_UART_TypeDef*)FM3_MFS##PORT_NUMBER##_UART)->SCR_f.TIE = 0;        \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    /* Already done in serialport_ReadTxStatus(). */
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        if (tx_complete[PORT_NUMBER] == 1)                                              \
        {                                                                               \
            ret = 1;                                                                    \
        }                                                                               \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_NXP_LPC546XX)

#include "fsl_clock.h"
#include "fsl_usart.h"
#include "pin_mux.h"
#include "fsl_iocon.h"

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)
#define SERIALPORT_UART_8_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(8)
#define SERIALPORT_UART_9_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(9)
#define SERIALPORT_UART_10_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(10)
#define SERIALPORT_UART_11_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(11)
#define SERIALPORT_UART_12_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(12)
#define SERIALPORT_UART_13_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(13)
#define SERIALPORT_UART_14_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(14)
#define SERIALPORT_UART_15_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(15)


#define MAX_PORTS               10
#define MAX_ALTERNETE_PINS      4
#define PORT_NA                 255
#define PIN_NA                  255
#define FUNC1                   IOCON_PIO_FUNC1
#define FUNC2                   IOCON_PIO_FUNC2
#define FUNC3                   IOCON_PIO_FUNC3
#define FUNC4                   IOCON_PIO_FUNC4
#define FUNC5                   IOCON_PIO_FUNC5
#define FUNC6                   IOCON_PIO_FUNC6
#define FUNC7                   IOCON_PIO_FUNC7
#define FUNC_NA                 255
#define PIN_TYPE_D              0
#define PIN_TYPE_A              1
#define PIN_TYPE_NONE           255
typedef struct port_pin_config_t
{
    uint8_t     rx_port;
    uint8_t     rx_pin;
    uint8_t     rx_func;
    uint8_t     rx_pin_type;
    uint8_t     tx_port;
    uint8_t     tx_pin;
    uint8_t     tx_func;
    uint8_t     tx_pin_type;
} PORT_PIN_CONFIG;
const PORT_PIN_CONFIG port_pin_config[MAX_PORTS][MAX_ALTERNETE_PINS] =
{
    /* Port 0: */
    {
        /* Alternate pins: */
        {  0U,      24U,    FUNC1,   PIN_TYPE_D,        0U,      25U,    FUNC1,   PIN_TYPE_D},
        {  0U,      29U,    FUNC1,   PIN_TYPE_D,        0U,      30U,    FUNC1,   PIN_TYPE_D},
        {  1U,       5U,    FUNC1,   PIN_TYPE_D,        1U,       6U,    FUNC1,   PIN_TYPE_D},
        {  2U,       0U,    FUNC2,   PIN_TYPE_A,        2U,       1U,    FUNC2,   PIN_TYPE_A}
    },
    /* Port 1: */
    {
        /* Alternate pins: */
        {  1U,      10U,    FUNC2,   PIN_TYPE_D,        0U,      10U,    FUNC4,   PIN_TYPE_A},
        {  2U,       3U,    FUNC3,   PIN_TYPE_D,        1U,      11U,    FUNC2,   PIN_TYPE_D},
        {  4U,      28U,    FUNC4,   PIN_TYPE_D,        2U,       4U,    FUNC3,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     4U,      29U,    FUNC4,   PIN_TYPE_D}
    },
    /* Port 2: */
    {
        /* Alternate pins: */
        {  0U,      26U,    FUNC1,   PIN_TYPE_D,        0U,      27U,    FUNC1,   PIN_TYPE_D},
        {  1U,      24U,    FUNC1,   PIN_TYPE_D,        1U,      25U,    FUNC1,   PIN_TYPE_D},
        {  4U,       9U,    FUNC2,   PIN_TYPE_D,        4U,      10U,    FUNC2,   PIN_TYPE_D},
        {  4U,      20U,    FUNC3,   PIN_TYPE_D,        4U,      21U,    FUNC3,   PIN_TYPE_D}
    },
    /* Port 3: */
    {
        /* Alternate pins: */
        {  0U,       3U,    FUNC1,   PIN_TYPE_D,        0U,       2U,    FUNC1,   PIN_TYPE_D},
        {  1U,       1U,    FUNC1,   PIN_TYPE_D,        0U,      12U,    FUNC1,   PIN_TYPE_A},
        {  2U,      18U,    FUNC2,   PIN_TYPE_D,        2U,      19U,    FUNC2,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    },
    /* Port 4: */
    {
        /* Alternate pins: */
        {  0U,       5U,    FUNC2,   PIN_TYPE_D,        0U,      16U,    FUNC1,   PIN_TYPE_A},
        {  1U,      21U,    FUNC5,   PIN_TYPE_D,        1U,      20U,    FUNC5,   PIN_TYPE_D},
        {  3U,      26U,    FUNC3,   PIN_TYPE_D,        3U,      27U,    FUNC3,   PIN_TYPE_D},
        {  5U,       0U,    FUNC4,   PIN_TYPE_D,        5U,       1U,    FUNC4,   PIN_TYPE_D}
    },
    /* Port 5: */
    {
        /* Alternate pins: */
        {  0U,       8U,    FUNC3,   PIN_TYPE_D,        0U,       9U,    FUNC3,   PIN_TYPE_D},
        {  2U,      12U,    FUNC5,   PIN_TYPE_D,        2U,      13U,    FUNC5,   PIN_TYPE_D},
        {  5U,       7U,    FUNC3,   PIN_TYPE_D,        5U,       8U,    FUNC3,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    },
    /* Port 6: */
    {
        /* Alternate pins: */
        {  0U,      11U,    FUNC1,   PIN_TYPE_A,        0U,      22U,    FUNC1,   PIN_TYPE_D},
        {  1U,      13U,    FUNC2,   PIN_TYPE_D,        1U,      16U,    FUNC2,   PIN_TYPE_D},
        {  4U,       2U,    FUNC2,   PIN_TYPE_D,        4U,       3U,    FUNC2,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    },
    /* Port 7: */
    {
        /* Alternate pins: */
        {  0U,      20U,    FUNC7,   PIN_TYPE_D,        0U,      19U,    FUNC7,   PIN_TYPE_D},
        {  1U,      29U,    FUNC1,   PIN_TYPE_D,        1U,      30U,    FUNC1,   PIN_TYPE_D},
        {  2U,      19U,    FUNC3,   PIN_TYPE_D,        2U,      20U,    FUNC3,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    },
    /* Port 8: */
    {
        /* Alternate pins: */
        /* rx_port  rx_pin  rx_func  rx_pin_type        tx_port  tx_pin  tx_func  tx_pin_type */
        {  1U,      17U,    FUNC2,   PIN_TYPE_D,        1U,      18U,    FUNC2,   PIN_TYPE_D},
        {  2U,      17U,    FUNC5,   PIN_TYPE_D,        2U,      29U,    FUNC3,   PIN_TYPE_D},
        {  3U,      16U,    FUNC1,   PIN_TYPE_D,        3U,      17U,    FUNC1,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    },
    /* Port 9: */
    {
        /* Alternate pins: */
        {  3U,       2U,    FUNC2,   PIN_TYPE_D,        3U,       3U,    FUNC2,   PIN_TYPE_D},
        {  3U,      21U,    FUNC1,   PIN_TYPE_A,        3U,      22U,    FUNC1,   PIN_TYPE_A},
        {  4U,      15U,    FUNC3,   PIN_TYPE_D,        4U,      16U,    FUNC3,   PIN_TYPE_D},
        {PORT_NA,   PIN_NA, FUNC_NA, PIN_TYPE_NONE,     PORT_NA, PIN_NA, FUNC_NA, PIN_TYPE_NONE}
    }
};

/*
Alternate RX and TX pin configuration:

portNumber      SERIALPORT_ALTERNATE_RX_PIN                             SERIALPORT_ALTERNATE_TX_PIN

    0           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_24               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_25
    0           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO0_29               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO0_30
    0           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO1_5                SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO1_6
    0           SERIALPORT_ALTERNATE_RX_PIN_3 --> PIO2_0                SERIALPORT_ALTERNATE_TX_PIN_3 --> PIO2_1

    1           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO1_10               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_10
    1           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO2_3                SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO1_11
    1           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO4_28               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO2_4
    1                                                                   SERIALPORT_ALTERNATE_TX_PIN_3 --> PIO4_29

    2           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_26               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_27
    2           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO1_24               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO1_25
    2           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO4_9                SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO4_10
    2           SERIALPORT_ALTERNATE_RX_PIN_3 --> PIO4_20               SERIALPORT_ALTERNATE_TX_PIN_3 --> PIO4_21

    3           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_3                SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_2
    3           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO1_1                SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO0_12
    3           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO2_18               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO2_19

    4           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_5                SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_16
    4           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO1_21               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO1_20
    4           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO3_26               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO3_27
    4           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO5_0                SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO5_1

    5           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_8                SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_9
    5           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO2_12               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO2_13
    5           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO5_7                SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO5_8

    6           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_11               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_22
    6           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO1_13               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO1_16
    6           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO4_2                SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO4_3

    7           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO0_20               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO0_19
    7           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO1_29               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO1_30
    7           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO2_19               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO2_20

    8           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO1_17               SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO1_18
    8           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO2_17               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO2_29
    8           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO3_16               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO3_17

    9           SERIALPORT_ALTERNATE_RX_PIN_0 --> PIO3_2                SERIALPORT_ALTERNATE_TX_PIN_0 --> PIO3_3
    9           SERIALPORT_ALTERNATE_RX_PIN_1 --> PIO3_21               SERIALPORT_ALTERNATE_TX_PIN_1 --> PIO3_22
    9           SERIALPORT_ALTERNATE_RX_PIN_2 --> PIO4_15               SERIALPORT_ALTERNATE_TX_PIN_2 --> PIO4_16
*/

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    uint8_t rx_port, rx_pin, tx_port, tx_pin;
    SERIALPORT_BAUDRATE baudrate;
    uint32_t port_pin_rx_config, port_pin_tx_config;
    uint32_t port_pin_type_d_rx_config = (
            IOCON_PIO_MODE_INACT |              /* No addition pin function */
            IOCON_PIO_INV_DI |                  /* Input function is not inverted */
            IOCON_PIO_DIGITAL_EN |              /* Enables digital function */
            IOCON_PIO_INPFILT_OFF |             /* Input filter disabled */
            IOCON_PIO_SLEW_STANDARD |           /* Standard mode, output slew rate control is enabled */
            IOCON_PIO_OPENDRAIN_DI);            /* Open drain is disabled */
    uint32_t port_pin_type_d_tx_config = (
            IOCON_PIO_MODE_INACT |              /* No addition pin function */
            IOCON_PIO_INV_DI |                  /* Input function is not inverted */
            IOCON_PIO_DIGITAL_EN |              /* Enables digital function */
            IOCON_PIO_INPFILT_OFF |             /* Input filter disabled */
            IOCON_PIO_SLEW_STANDARD |           /* Standard mode, output slew rate control is enabled */
            IOCON_PIO_OPENDRAIN_DI);            /* Open drain is disabled */
    uint32_t port_pin_type_a_rx_config = (
            IOCON_PIO_MODE_INACT |              /* No addition pin function */
            IOCON_PIO_INV_DI |                  /* Input function is not inverted */
            IOCON_PIO_DIGITAL_EN |              /* Enables digital function */
            IOCON_PIO_INPFILT_OFF |             /* Input filter disabled */
            IOCON_PIO_OPENDRAIN_DI);            /* Open drain is disabled */
    uint32_t port_pin_type_a_tx_config = (
            IOCON_PIO_MODE_INACT |              /* No addition pin function */
            IOCON_PIO_INV_DI |                  /* Input function is not inverted */
            IOCON_PIO_DIGITAL_EN |              /* Enables digital function */
            IOCON_PIO_INPFILT_OFF |             /* Input filter disabled */
            IOCON_PIO_OPENDRAIN_DI);            /* Open drain is disabled */


    if ((portNumber >= MAX_PORTS) || (alternate_rx_pin >= MAX_ALTERNETE_PINS) || (alternate_tx_pin >= MAX_ALTERNETE_PINS))
    {
        return;
    }

    rx_port = port_pin_config[portNumber][alternate_rx_pin].rx_port;
    rx_pin  = port_pin_config[portNumber][alternate_rx_pin].rx_pin;
    if (port_pin_config[portNumber][alternate_rx_pin].rx_pin_type == PIN_TYPE_D)
    {
        port_pin_type_d_rx_config |= port_pin_config[portNumber][alternate_rx_pin].rx_func;
        port_pin_rx_config = port_pin_type_d_rx_config;
    }
    else if (port_pin_config[portNumber][alternate_rx_pin].rx_pin_type == PIN_TYPE_A)
    {
        port_pin_type_a_rx_config |= port_pin_config[portNumber][alternate_rx_pin].rx_func;
        port_pin_rx_config = port_pin_type_a_rx_config;
    }
    else
    {
        return;
    }

    tx_port = port_pin_config[portNumber][alternate_tx_pin].tx_port;
    tx_pin  = port_pin_config[portNumber][alternate_tx_pin].tx_pin;
    if (port_pin_config[portNumber][alternate_tx_pin].tx_pin_type == PIN_TYPE_D)
    {
        port_pin_type_d_tx_config |= port_pin_config[portNumber][alternate_tx_pin].tx_func;
        port_pin_tx_config = port_pin_type_d_tx_config;
    }
    else if (port_pin_config[portNumber][alternate_tx_pin].tx_pin_type == PIN_TYPE_A)
    {
        port_pin_type_a_tx_config |= port_pin_config[portNumber][alternate_tx_pin].tx_func;
        port_pin_tx_config = port_pin_type_a_tx_config;
    }
    else
    {
        return;
    }
    
    if ((rx_port == PORT_NA) || (rx_pin == PIN_NA) || 
        (port_pin_config[portNumber][alternate_rx_pin].rx_func == FUNC_NA) ||
        (port_pin_config[portNumber][alternate_rx_pin].rx_pin_type == PIN_TYPE_NONE) ||
        (tx_port == PORT_NA) || (tx_pin == PIN_NA) || 
        (port_pin_config[portNumber][alternate_tx_pin].tx_func == FUNC_NA) ||
        (port_pin_config[portNumber][alternate_tx_pin].tx_pin_type == PIN_TYPE_NONE)  )
    {
        return;
    }
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        tx_complete[portNumber] = 1;                                                    \
        baudrate = SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE;                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    /* Setup serialport hardware */
    CLOCK_EnableClock(kCLOCK_Iocon);
    IOCON_PinMuxSet(IOCON, rx_port, rx_pin, port_pin_rx_config);
    IOCON_PinMuxSet(IOCON, tx_port, tx_pin, port_pin_tx_config);
    serialport_baudrate_set(portNumber, baudrate);
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    uint32_t clock_frequency;
    uint32_t actual_baudrate;
    uint32_t osrval, brgval;
    usart_config_t config;

    clock_frequency = 0;
    actual_baudrate = 0;
    USART_GetDefaultConfig(&config);
    config.enableTx     = true;
    config.enableRx     = true;
    config.baudRate_Bps = baudrate;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        CLOCK_AttachClk(kFRO12M_to_FLEXCOMM##PORT_NUMBER);                              \
        clock_frequency = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY;       \
        USART_Init(USART##PORT_NUMBER, &config, clock_frequency);                       \
        osrval = ((USART_Type *)(USART##PORT_NUMBER))->OSR;                             \
        brgval = ((USART_Type *)(USART##PORT_NUMBER))->BRG;                             \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    if (clock_frequency != 0)
    {
        actual_baudrate = (clock_frequency / (osrval + 1)) / (brgval + 1);
    }
    return actual_baudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        serialport_ClearRxBuffer(portNumber);                                                                          \
        NVIC_ClearPendingIRQ(FLEXCOMM##PORT_NUMBER##_IRQn);                                                            \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                                       \
        {                                                                                                              \
            NVIC_SetPriority(FLEXCOMM##PORT_NUMBER##_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_14);                        \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            NVIC_SetPriority(FLEXCOMM##PORT_NUMBER##_IRQn, PLATFORM_INTERRUPT_PRIORITY_LOW_15);                        \
        }                                                                                                              \
        NVIC_EnableIRQ(FLEXCOMM##PORT_NUMBER##_IRQn);                                                                  \
        USART_EnableInterrupts(USART##PORT_NUMBER, kUSART_RxLevelInterruptEnable/* | kUSART_RxErrorInterruptEnable*/); \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        USART_DisableInterrupts(USART##PORT_NUMBER, kUSART_RxLevelInterruptEnable/* | kUSART_RxErrorInterruptEnable*/);\
        /* Disable only Rx part of IRQ, not whole interrupt. This allows TX works independently:                       \
        NVIC_DisableIRQ(FLEXCOMM##PORT_NUMBER##_IRQn);                                             */                  \
        NVIC_ClearPendingIRQ(FLEXCOMM##PORT_NUMBER##_IRQn);                                                            \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(FLEXCOMM##PORT_NUMBER##_IRQn, (uint32_t)address);              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* LPC546xx specific: the same ISR for RX and TX. 
       Setting only UART's internal interrupt enable. */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        USART_EnableInterrupts(USART##PORT_NUMBER, kUSART_TxLevelInterruptEnable/* | kUSART_TxErrorInterruptEnable*/); \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* LPC546xx specific: the same ISR for RX and TX.  
       Clearing only UART's internal interrupt enable. */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        USART_DisableInterrupts(USART##PORT_NUMBER, kUSART_TxLevelInterruptEnable/* | kUSART_TxErrorInterruptEnable*/);\
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* LPC546xx specific: the same ISR for RX and TX. Use ISR registered in serialport_RXinterrupt_register_ISR(). */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;

    ch = 0;
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        /* Instead of USART_ReadByte() function - executing from RAM: */                                               \
        ch = ((USART_Type *)(USART##PORT_NUMBER))->FIFORD;                                                             \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;

    returnStatus = SERIALPORT_RXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        /* Instead of USART_GetStatusFlags() function - executing from RAM: */                                         \
        if ((((USART_Type *)(USART##PORT_NUMBER))->FIFOSTAT) & (kUSART_RxFifoNotEmptyFlag /*| kUSART_RxError)*/))      \
        {                                                                                                              \
            returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;                                                         \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Rx status flag is cleared automatically by reading from the received data register (FIFORD) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        USART_FIFOCFG_EMPTYRX((((USART_Type *)(USART##PORT_NUMBER))->FIFOCFG));                                        \
        serialport_ReadRxData(portNumber);                                                                             \
        serialport_ClearRxStatus(portNumber, SERIALPORT_RXSTATUS_FIFO_NOTEMPTY);                                       \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        tx_complete[portNumber] = 0;                                                                                   \
        USART_WriteByte(USART##PORT_NUMBER, data);                                                                     \
        USART_EnableInterrupts(                                                                                        \
                USART##PORT_NUMBER, kUSART_TxLevelInterruptEnable/* | kUSART_TxErrorInterruptEnable*/);                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

/* Copy of USART_GetStatusFlags() function from fsl_usart.h, because it has to be run from RAM: */
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint32_t USART_GetStatusFlags_RAM(USART_Type *base)
{
    return base->FIFOSTAT;
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;

    returnStatus = SERIALPORT_TXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (USART_GetStatusFlags_RAM(USART##PORT_NUMBER) & (kUSART_TxFifoEmptyFlag /*| kUSART_TxError*/))              \
        {                                                                                                              \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                                                            \
            tx_complete[portNumber] = 1;                                                                               \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

/* Copy of USART_DisableInterrupts() function from fsl_usart.h, because it has to be run from RAM: */
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void USART_DisableInterrupts_RAM(USART_Type *base, uint32_t mask)
{
    base->FIFOINTENCLR = mask & 0xFUL;
}

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (status == SERIALPORT_TXSTATUS_FIFO_EMPTY)                                                                  \
        {                                                                                                              \
            USART_DisableInterrupts_RAM(                                                                               \
                     USART##PORT_NUMBER, kUSART_TxLevelInterruptEnable/* | kUSART_TxErrorInterruptEnable*/);           \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;
    
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (tx_complete[PORT_NUMBER] == 1)                                                                             \
        {                                                                                                              \
            if (((((USART_Type *)(USART##PORT_NUMBER))->STAT) & USART_STAT_TXIDLE_MASK) == USART_STAT_TXIDLE_MASK)     \
            {                                                                                                          \
                ret = 1;                                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_CYPRESS_PSOC4100SMAX)

#include "cy_pdl.h"
#include "cybsp.h"

#define SERIALPORT_UART_0_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(0)
#define SERIALPORT_UART_0_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_1_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(1)
#define SERIALPORT_UART_1_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_2_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(2)
#define SERIALPORT_UART_2_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_3_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(3)
#define SERIALPORT_UART_3_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_4_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(4)
#define SERIALPORT_UART_4_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_5_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(5)
#define SERIALPORT_UART_5_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_6_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(6)
#define SERIALPORT_UART_6_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_7_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(7)
#define SERIALPORT_UART_7_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_8_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(8)
#define SERIALPORT_UART_8_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_9_CLOCK_SOURCE_FREQUENCY           platform_uart_clock_source_frequency_get(9)
#define SERIALPORT_UART_9_FRACTIONALMULTIPLIER             32
#define SERIALPORT_UART_10_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(10)
#define SERIALPORT_UART_10_FRACTIONALMULTIPLIER            32
#define SERIALPORT_UART_11_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(11)
#define SERIALPORT_UART_11_FRACTIONALMULTIPLIER            32
#define SERIALPORT_UART_12_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(12)
#define SERIALPORT_UART_12_FRACTIONALMULTIPLIER            32
#define SERIALPORT_UART_13_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(13)
#define SERIALPORT_UART_13_FRACTIONALMULTIPLIER            32
#define SERIALPORT_UART_14_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(14)
#define SERIALPORT_UART_14_FRACTIONALMULTIPLIER            32
#define SERIALPORT_UART_15_CLOCK_SOURCE_FREQUENCY          platform_uart_clock_source_frequency_get(15)
#define SERIALPORT_UART_15_FRACTIONALMULTIPLIER            32

#define SERIALPORT_NUMBER(PORT_NUMBER)                                                                                                  \
cy_stc_scb_uart_context_t UART_##PORT_NUMBER##_context;
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    /* Setup serialport hardware */

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                                  \
    {                                                                                                                                   \
        /* Configure and enable the UART peripheral */                                                                                  \
        Cy_SCB_UART_Init(UART_##PORT_NUMBER##_HW, &UART_##PORT_NUMBER##_config, &UART_##PORT_NUMBER##_context);                         \
        Cy_SCB_UART_Enable(UART_##PORT_NUMBER##_HW);                                                                                    \
                                                                                                                                        \
        serialport_baudrate_set(portNumber, SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE);                                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    /* UART clock enumeration (portNumber parameter) must be the same as 
       CY_SYSCLK_DIV_16_5_BIT enumeration (second parameter of the Cy_SysClk_PeriphSetFracDivider() function). */

    uint32_t freq;
    uint32_t oversampl;
    uint32_t fractmpy;
    uint32_t dividerInt;
    uint32_t dividerIntPlus1;
    uint32_t dividerFrac;
    uint32_t dividerFracPlus1;
    uint32_t actualBaudrate; 
    uint32_t actualBaudratePlus1;

    /* Default init values: */
    freq = 1;
    oversampl = 1;
    fractmpy = 1;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                      \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                          \
    {                                                                                                           \
        freq      = SERIALPORT_UART_##PORT_NUMBER##_CLOCK_SOURCE_FREQUENCY;                                     \
        oversampl = UART_##PORT_NUMBER##_config.oversample;                                                     \
        fractmpy  = SERIALPORT_UART_##PORT_NUMBER##_FRACTIONALMULTIPLIER;                                       \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    dividerInt  = ((freq / oversampl) / baudrate) - 1;
    dividerFrac = ((freq / oversampl) % baudrate);
    dividerFrac = (dividerFrac * fractmpy) / baudrate;
    dividerIntPlus1 = dividerInt;
    dividerFracPlus1 = dividerFrac + 1;
    if (dividerFracPlus1 >= fractmpy)
    {
        dividerFracPlus1 = 0;
        dividerIntPlus1++;
    }
    if (dividerIntPlus1 >= 32767)
    {
        dividerInt      = 32766;
        dividerIntPlus1 = 32767;
    }

    actualBaudrate = (freq * fractmpy / oversampl) /
                     ((dividerInt + 1) * fractmpy + dividerFrac);
    actualBaudratePlus1 = (freq * fractmpy / oversampl) /
                     ((dividerIntPlus1 + 1) * fractmpy + dividerFracPlus1);

    if ((baudrate - actualBaudratePlus1) < (actualBaudrate - baudrate))
    {
        dividerInt = dividerIntPlus1;
        dividerFrac = dividerFracPlus1;
        actualBaudrate = actualBaudratePlus1;
    }

    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_5_BIT, (uint32_t) portNumber);
    Cy_SysClk_PeriphSetFracDivider(CY_SYSCLK_DIV_16_5_BIT, (uint32_t) portNumber, dividerInt, dividerFrac);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_5_BIT,  (uint32_t) portNumber);

    return actualBaudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        serialport_ClearRxBuffer(portNumber);                                                                          \
        NVIC_ClearPendingIRQ(UART_##PORT_NUMBER##_IRQ);                                                                \
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)                                                       \
        {                                                                                                              \
            NVIC_SetPriority(UART_##PORT_NUMBER##_IRQ, PLATFORM_INTERRUPT_PRIORITY_LOW_14);                            \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            NVIC_SetPriority(UART_##PORT_NUMBER##_IRQ, PLATFORM_INTERRUPT_PRIORITY_LOW_15);                            \
        }                                                                                                              \
        NVIC_EnableIRQ(UART_##PORT_NUMBER##_IRQ);                                                                      \
        Cy_SCB_ClearRxInterrupt(UART_##PORT_NUMBER##_HW, CY_SCB_RX_INTR_NOT_EMPTY);                                    \
        Cy_SCB_SetRxInterruptMask(UART_##PORT_NUMBER##_HW,                                                             \
                                  Cy_SCB_GetRxInterruptMask(UART_##PORT_NUMBER##_HW) | CY_SCB_RX_INTR_NOT_EMPTY);      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        Cy_SCB_SetRxInterruptMask(UART_##PORT_NUMBER##_HW,                                                             \
                                  Cy_SCB_GetRxInterruptMask(UART_##PORT_NUMBER##_HW) & ~CY_SCB_RX_INTR_NOT_EMPTY);     \
        /* Disable only Rx part of IRQ, not whole interrupt. This allows TX works independently:                       \
        NVIC_DisableIRQ(UART_##PORT_NUMBER##_IRQ);                                             */                      \
        NVIC_ClearPendingIRQ(UART_##PORT_NUMBER##_IRQ);                                                                \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    if (PROC_INT_VECT_TABLE == 0)
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        __NVIC_SetVector(UART_##PORT_NUMBER##_IRQ, (uint32_t)address);                  \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* PSOC4100SMAX specific: the same ISR for RX and TX. 
       Setting only UART's internal interrupt enable. */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        Cy_SCB_SetTxFifoLevel(UART_##PORT_NUMBER##_HW, (Cy_SCB_GetFifoSize(UART_##PORT_NUMBER##_HW) / 2UL));           \
        Cy_SCB_ClearTxInterrupt(UART_##PORT_NUMBER##_HW, CY_SCB_TX_INTR_LEVEL);                                        \
        Cy_SCB_SetTxInterruptMask(UART_##PORT_NUMBER##_HW,                                                             \
                                  Cy_SCB_GetTxInterruptMask(UART_##PORT_NUMBER##_HW) | CY_SCB_TX_INTR_LEVEL);          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* PSOC4100SMAX specific: the same ISR for RX and TX.  
       Clearing only UART's internal interrupt enable. */
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        Cy_SCB_SetTxInterruptMask(UART_##PORT_NUMBER##_HW,                                                             \
                                  Cy_SCB_GetTxInterruptMask(UART_##PORT_NUMBER##_HW) & ~CY_SCB_TX_INTR_LEVEL);         \
        Cy_SCB_ClearTxInterrupt(UART_##PORT_NUMBER##_HW, CY_SCB_TX_INTR_LEVEL);                                        \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* PSOC4100SMAX specific: the same ISR for RX and TX. Use ISR registered in serialport_RXinterrupt_register_ISR(). */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    uint8_t ch;

    ch = 0;
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        /* Instead of Cy_SCB_ReadRxFifo() function - executing from RAM: */                                            \
        ch = (uint8_t) (SCB_RX_FIFO_RD(UART_##PORT_NUMBER##_HW));                                                      \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return ch;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_RXSTATUS returnStatus;
    uint32_t status;

    returnStatus = SERIALPORT_RXSTATUS_NONE;
    status = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        /* Instead of Cy_SCB_GetRxInterruptStatusMasked() function - executing from RAM: */                            \
        status = SCB_INTR_RX_MASKED(UART_##PORT_NUMBER##_HW);                                                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    if ((status & CY_SCB_RX_INTR_NOT_EMPTY) != 0UL)
    {
        returnStatus |= SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
    }

    return returnStatus;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (status == SERIALPORT_RXSTATUS_FIFO_NOTEMPTY)                                                               \
        {                                                                                                              \
            /* Instead of Cy_SCB_ClearRxInterrupt() function - executing from RAM: */                                  \
            SCB_INTR_RX(UART_##PORT_NUMBER##_HW) = CY_SCB_RX_INTR_NOT_EMPTY;                                           \
            (void) SCB_INTR_RX(UART_##PORT_NUMBER##_HW);                                                               \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        Cy_SCB_ClearRxFifo(UART_##PORT_NUMBER##_HW);                                                                   \
        serialport_ReadRxData(portNumber);                                                                             \
        serialport_ClearRxStatus(portNumber, SERIALPORT_RXSTATUS_FIFO_NOTEMPTY);                                       \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        Cy_SCB_WriteTxFifo(UART_##PORT_NUMBER##_HW, (uint32_t) data);                                                  \
        Cy_SCB_ClearTxInterrupt(UART_##PORT_NUMBER##_HW, CY_SCB_TX_INTR_LEVEL);                                        \
        Cy_SCB_SetTxInterruptMask(UART_##PORT_NUMBER##_HW,                                                             \
                                  Cy_SCB_GetTxInterruptMask(UART_##PORT_NUMBER##_HW) | CY_SCB_TX_INTR_LEVEL);          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_WriteTxData */

PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_TXSTATUS returnStatus;

    returnStatus = SERIALPORT_TXSTATUS_NONE;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        /* Instead of Cy_SCB_GetTxInterruptStatusMasked() function - executing from RAM: */                            \
        if (SCB_INTR_TX_MASKED(UART_##PORT_NUMBER##_HW) & CY_SCB_TX_INTR_LEVEL)                                        \
        {                                                                                                              \
            returnStatus |= SERIALPORT_TXSTATUS_FIFO_EMPTY;                                                            \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return returnStatus;
}   /* serialport_ReadTxStatus */

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (status == SERIALPORT_TXSTATUS_FIFO_EMPTY)                                                                  \
        {                                                                                                              \
            /* Instead of Cy_SCB_SetTxInterruptMask() function - executing from RAM: */                                \
            SCB_INTR_TX_MASK(UART_##PORT_NUMBER##_HW) =                                                                \
                             SCB_INTR_TX_MASK(UART_##PORT_NUMBER##_HW) & ~CY_SCB_TX_INTR_LEVEL;                        \
            /* Instead of Cy_SCB_ClearTxInterrupt() function - executing from RAM: */                                  \
            SCB_INTR_TX(UART_##PORT_NUMBER##_HW) = CY_SCB_TX_INTR_LEVEL;                                               \
            (void) SCB_INTR_TX(UART_##PORT_NUMBER##_HW);                                                               \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    uint8_t ret;
    
    ret = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                                                             \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                                                 \
    {                                                                                                                  \
        if (Cy_SCB_UART_IsTxComplete(UART_##PORT_NUMBER##_HW))                                                         \
        {                                                                                                              \
            ret = 1;                                                                                                   \
        }                                                                                                              \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
    
    return ret;
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_STM_STM32G0xx)

/*
 * STM32G0xx specific:
 *
 * SERIALPORT_NUMBER_0 ... USART1 (UART_HandleTypeDef huart1 from usart.c)
 * SERIALPORT_NUMBER_1 ... USART2 (UART_HandleTypeDef huart2 from usart.c)
 * SERIALPORT_NUMBER_2 ... USART3 (UART_HandleTypeDef huart3 from usart.c)
 * SERIALPORT_NUMBER_3 ... USART4 (UART_HandleTypeDef huart4 from usart.c)
 * SERIALPORT_NUMBER_4 ... USART5 (UART_HandleTypeDef huart5 from usart.c)
 * SERIALPORT_NUMBER_5 ... USART6 (UART_HandleTypeDef huart6 from usart.c)
 *
 * SERIALPORT_NUMBER_6 ... LPUART1 (UART_HandleTypeDef hlpuart1 from usart.c)
 * SERIALPORT_NUMBER_7 ... LPUART2 (UART_HandleTypeDef hlpuart2 from usart.c)
 */

#if ((SERIALPORT6_IN_USE) || (SERIALPORT7_IN_USE) || (SERIALPORT8_IN_USE) || (SERIALPORT9_IN_USE) || (SERIALPORT10_IN_USE) ||  \
     (SERIALPORT11_IN_USE) || (SERIALPORT12_IN_USE) || (SERIALPORT13_IN_USE) || (SERIALPORT14_IN_USE) || (SERIALPORT15_IN_USE))
#error SERIALPORT: STM32G0xx supports only 6 UARTs !!!
#endif

#include "usart.h"
#include "stm32g0xx_it.h"
#if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
#if (PROJECT_TYPE == PROJECT_TYPE_APPLICATION)
#include "priorities.h"
#elif (PROJECT_TYPE == PROJECT_TYPE_BOOTLOADER)
#define USART1_IRQ_PRIORITY                         ( 2u )
#define USART2_IRQ_PRIORITY                         ( 3u )
#define USART3_4_5_6_IRQ_PRIORITY                   ( 3u )
#define LPUSART1_IRQ_PRIORITY                       ( 3u )
#define LPUSART2_IRQ_PRIORITY                       ( 3u )
#endif /* --> PROJECT_TYPE */
#endif /* --> PROJECT_NAME */

UART_HandleTypeDef *huart_from_enum(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
UART_HandleTypeDef *huart_from_enum(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    switch (portNumber)
    {
        #if SERIALPORT0_IN_USE
        case SERIALPORT_NUMBER_0:
            huart = &huart1;
            break;
        #endif
        #if SERIALPORT1_IN_USE
        case SERIALPORT_NUMBER_1:
            huart = &huart2;
            break;
        #endif
        #if SERIALPORT2_IN_USE
        case SERIALPORT_NUMBER_2:
            huart = &huart3;
            break;
        #endif
        #if SERIALPORT3_IN_USE
        case SERIALPORT_NUMBER_3:
            huart = &huart4;
            break;
        #endif
        #if SERIALPORT4_IN_USE
        case SERIALPORT_NUMBER_4:
            huart = &huart5;
            break;
        #endif
        #if SERIALPORT5_IN_USE
        case SERIALPORT_NUMBER_5:
            huart = &huart6;
            break;
        #endif
        #if SERIALPORT6_IN_USE
        case SERIALPORT_NUMBER_6:
            huart = &hlpuart1;
            break;
        #endif
        #if SERIALPORT7_IN_USE
        case SERIALPORT_NUMBER_7:
            huart = &hlpuart2;
            break;
        #endif
        default:
            huart = NULL;
            break;
    } /* -> switch (portNumber) */

    return huart;
}

#if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
uint32_t priority_from_enum(SERIALPORT_NUMBER portNumber)
{
    uint32_t priority;

    switch (portNumber)
    {
        #if SERIALPORT0_IN_USE
        case SERIALPORT_NUMBER_0:
            priority = USART1_IRQ_PRIORITY;
            break;
        #endif
        #if SERIALPORT1_IN_USE
        case SERIALPORT_NUMBER_1:
            priority = USART2_IRQ_PRIORITY;
            break;
        #endif
        #if SERIALPORT2_IN_USE
        case SERIALPORT_NUMBER_2:
        #endif
        #if SERIALPORT3_IN_USE
        case SERIALPORT_NUMBER_3:
        #endif
        #if SERIALPORT4_IN_USE
        case SERIALPORT_NUMBER_4:
        #endif
        #if SERIALPORT5_IN_USE
        case SERIALPORT_NUMBER_5:
        #endif
        #if (SERIALPORT2_IN_USE || SERIALPORT3_IN_USE || SERIALPORT4_IN_USE || SERIALPORT5_IN_USE)
            priority = USART3_4_5_6_IRQ_PRIORITY;
            break;
        #endif
        #if SERIALPORT6_IN_USE
        case SERIALPORT_NUMBER_6:
            priority = LPUSART1_IRQ_PRIORITY;
            break;
        #endif
        #if SERIALPORT7_IN_USE
        case SERIALPORT_NUMBER_7:
            priority = LPUSART2_IRQ_PRIORITY;
            break;
        #endif
        default:
            priority = 3; /* lowest priority */
            break;
    } /* -> switch (portNumber) */

    return priority;
}
#endif /* --> (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC) */

IRQn_Type irqn_from_enum(SERIALPORT_NUMBER portNumber)
{
    IRQn_Type irqn;

    switch (portNumber)
    {
        #if SERIALPORT0_IN_USE
        case SERIALPORT_NUMBER_0:
            irqn = USART1_IRQn;
            break;
        #endif
        #if SERIALPORT1_IN_USE
        case SERIALPORT_NUMBER_1:
            #if defined(STM32G0B0xx)
                irqn = USART2_IRQn;
            #elif defined(STM32G0B1xx)
                irqn = USART2_LPUART2_IRQn;
            #else
                #error serialport.c: MCU is not supported.
            #endif
            break;
        #endif
        #if SERIALPORT2_IN_USE
        case SERIALPORT_NUMBER_2:
        #endif
        #if SERIALPORT3_IN_USE
        case SERIALPORT_NUMBER_3:
        #endif
        #if SERIALPORT4_IN_USE
        case SERIALPORT_NUMBER_4:
        #endif
        #if SERIALPORT5_IN_USE
        case SERIALPORT_NUMBER_5:
        #endif
        #if (SERIALPORT2_IN_USE || SERIALPORT3_IN_USE || SERIALPORT4_IN_USE || SERIALPORT5_IN_USE)
            #if defined(STM32G0B0xx)
                irqn = USART3_4_5_6_IRQn;
            #elif defined(STM32G0B1xx)
                irqn = USART3_4_5_6_LPUART1_IRQn;
            #else
                #error serialport.c: MCU is not supported.
            #endif
            break;
        #endif
        #if SERIALPORT6_IN_USE
        case SERIALPORT_NUMBER_6:
            irqn = USART3_4_5_6_LPUART1_IRQn;
            break;
        #endif
        #if SERIALPORT7_IN_USE
        case SERIALPORT_NUMBER_7:
            irqn = USART2_LPUART2_IRQn;
            break;
        #endif
        default:
            irqn = (IRQn_Type) -128;
            break;
    } /* -> switch (portNumber) */

    return irqn;
}

SERIALPORT_BAUDRATE default_baudrate_from_enum(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_BAUDRATE baudrate;

    baudrate = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        baudrate = SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE;                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return baudrate;
}

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    SERIALPORT_BAUDRATE baudrate;

    /* HW pins initialization already done. */

    tx_complete[portNumber] = 1;

    baudrate = 0;
    baudrate = default_baudrate_from_enum(portNumber);
    if (baudrate != 0)
    {
        serialport_baudrate_set(portNumber, baudrate);
    }
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    UART_HandleTypeDef *huart;
    UART_ClockSourceTypeDef clocksource;
    uint32_t pclk;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        /* Logical copy from MX_USART1_UART_Init(): */
        huart->Init.BaudRate = baudrate;
        huart->Init.WordLength = UART_WORDLENGTH_8B;
        huart->Init.StopBits = UART_STOPBITS_1;
        huart->Init.Parity = UART_PARITY_NONE; /* If Parity is enabled (even or odd), huart->Init.WordLength has to be set to 9 bits, see RM Chapter 26.5.12 USART parity control */
        huart->Init.Mode = UART_MODE_TX_RX;
        huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart->Init.OverSampling = UART_OVERSAMPLING_16;
        huart->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
        huart->Init.ClockPrescaler = UART_PRESCALER_DIV1;
        huart->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        if (HAL_UART_Init(huart) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetTxFifoThreshold(huart, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_SetRxFifoThreshold(huart, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
        {
            Error_Handler();
        }
        if (HAL_UARTEx_EnableFifoMode(huart) != HAL_OK)
        {
            Error_Handler();
        }
    }

    /* Calculating actual baudrate (logical copy of code from UART_SetConfig()): */
    UART_GETCLOCKSOURCE(huart, clocksource);
    pclk = 0;

    /* Check LPUART instance */
    if (UART_INSTANCE_LOWPOWER(huart))
    {
        /* Retrieve frequency clock */
        switch (clocksource)
        {
          case UART_CLOCKSOURCE_PCLK1:
            pclk = HAL_RCC_GetPCLK1Freq();
            break;
          case UART_CLOCKSOURCE_HSI:
            pclk = (uint32_t) HSI_VALUE;
            break;
          case UART_CLOCKSOURCE_SYSCLK:
            pclk = HAL_RCC_GetSysClockFreq();
            break;
          case UART_CLOCKSOURCE_LSE:
            pclk = (uint32_t) LSE_VALUE;
            break;
          default:
            pclk = 0U;
            break;
        }
        if (pclk != 0)
        {
            /* See UART_DIV_LPUART define. */
            baudrate = pclk / (UARTPrescTable[huart->Init.ClockPrescaler] * huart->Instance->BRR) * 256U;
        }
    } /* --> UART_INSTANCE_LOWPOWER */
    /* Check UART Over Sampling to set Baud Rate Register */
    else if (huart->Init.OverSampling == UART_OVERSAMPLING_8)
    {
        switch (clocksource)
        {
          case UART_CLOCKSOURCE_PCLK1:
            pclk = HAL_RCC_GetPCLK1Freq();
            break;
          case UART_CLOCKSOURCE_HSI:
            pclk = (uint32_t) HSI_VALUE;
            break;
          case UART_CLOCKSOURCE_SYSCLK:
            pclk = HAL_RCC_GetSysClockFreq();
            break;
          case UART_CLOCKSOURCE_LSE:
            pclk = (uint32_t) LSE_VALUE;
            break;
          default:
            pclk = 0U;
            break;
        }
        if (pclk != 0)
        {
            /* See UART_DIV_SAMPLING8 define. */
            baudrate = pclk / (UARTPrescTable[huart->Init.ClockPrescaler] * huart->Instance->BRR) * 2U;
        }
    } /* --> UART_OVERSAMPLING_8 */
    else if (huart->Init.OverSampling == UART_OVERSAMPLING_16)
    {
        switch (clocksource)
        {
          case UART_CLOCKSOURCE_PCLK1:
            pclk = HAL_RCC_GetPCLK1Freq();
            break;
          case UART_CLOCKSOURCE_HSI:
            pclk = (uint32_t) HSI_VALUE;
            break;
          case UART_CLOCKSOURCE_SYSCLK:
            pclk = HAL_RCC_GetSysClockFreq();
            break;
          case UART_CLOCKSOURCE_LSE:
            pclk = (uint32_t) LSE_VALUE;
            break;
          default:
            pclk = 0U;
            break;
        }
        if (pclk != 0)
        {
            /* See UART_DIV_SAMPLING16 define. */
            baudrate = pclk / (UARTPrescTable[huart->Init.ClockPrescaler] * huart->Instance->BRR);
        }
    } /* --> UART_OVERSAMPLING_16 */

    return baudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    IRQn_Type irqn;
    #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
    uint32_t priority;
    #endif

    serialport_ClearRxBuffer(portNumber);

    #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
    priority = 3; /* Default: lowest priority */
    priority = priority_from_enum(portNumber);
    #endif
    irqn = (IRQn_Type) -128;
    irqn = irqn_from_enum(portNumber);
    if (irqn != (IRQn_Type) -128)
    {
        NVIC_ClearPendingIRQ(irqn);
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)
        {
            #if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
            NVIC_SetPriority(irqn, priority);
            #else
            NVIC_SetPriority(irqn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_2);
            #endif
        }
        else
        {
            NVIC_SetPriority(irqn, PLATFORM_INTERRUPT_PRIORITY_LOW_3);
        }
        NVIC_EnableIRQ(irqn);
    }

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        SET_BIT(huart->Instance->CR1, USART_CR1_RXNEIE_RXFNEIE); /* RXNE/RXFIFO not empty Interrupt Enable */
    }
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    /* Disable only Rx part of IRQ, not whole interrupt. This allows TX works independently. */

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_RXNEIE_RXFNEIE); /* RXNE/RXFIFO not empty Interrupt Enable */
    }
}   /* serialport_RXinterruptDisable */


#if (SERIALPORT0_IN_USE)
static void (*uart_1_callback)(void) = NULL;
void rx_tx_interrupt_callback_uart1(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart1(void)
{
    if (uart_1_callback != NULL)
    {
        uart_1_callback();
    }
}
#endif /* (SERIALPORT0_IN_USE) */

#if (SERIALPORT1_IN_USE)
static void (*uart_2_callback)(void) = NULL;
void rx_tx_interrupt_callback_uart2(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart2(void)
{
    if (uart_2_callback != NULL)
    {
        uart_2_callback();
    }
}
#endif /* (SERIALPORT1_IN_USE) */

#if ((SERIALPORT2_IN_USE) || (SERIALPORT3_IN_USE) || (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE))
#if (SERIALPORT2_IN_USE)
static void (*uart_3_callback)(void) = NULL;
#endif /* (SERIALPORT2_IN_USE) */
#if (SERIALPORT3_IN_USE)
static void (*uart_4_callback)(void) = NULL;
#endif /* (SERIALPORT3_IN_USE) */
#if (SERIALPORT4_IN_USE)
static void (*uart_5_callback)(void) = NULL;
#endif /* (SERIALPORT4_IN_USE) */
#if (SERIALPORT5_IN_USE)
static void (*uart_6_callback)(void) = NULL;
#endif /* (SERIALPORT5_IN_USE) */
void rx_tx_interrupt_callback_uart3456(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart3456(void)
{
    #if (SERIALPORT2_IN_USE)
    if (uart_3_callback != NULL)
    {
        uart_3_callback();
    }
    #endif /* (SERIALPORT2_IN_USE) */
    #if (SERIALPORT3_IN_USE)
    if (uart_4_callback != NULL)
    {
        uart_4_callback();
    }
    #endif /* (SERIALPORT3_IN_USE) */
    #if (SERIALPORT4_IN_USE)
    if (uart_5_callback != NULL)
    {
        uart_5_callback();
    }
    #endif /* (SERIALPORT4_IN_USE) */
    #if (SERIALPORT5_IN_USE)
    if (uart_6_callback != NULL)
    {
        uart_6_callback();
    }
    #endif /* (SERIALPORT5_IN_USE) */

    /* Call HAL driver IRQ handler for UARTs that are not used in the serialport.c module: */
    #if defined(STM32G0B0xx)
        USART3_4_5_6_IRQHandler();
    #elif defined(STM32G0B1xx)
        USART3_4_5_6_LPUART1_IRQHandler();
    #else
        #error serialport.c: MCU is not supported.
    #endif
}
#endif /* ((SERIALPORT2_IN_USE) || (SERIALPORT3_IN_USE) || (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE)) */


#if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)

#if (SERIALPORT0_IN_USE)
void serialport_rx_tx_interrupt_callback_uart1(void)
{
    rx_tx_interrupt_callback_uart1();
}
#endif /* (SERIALPORT0_IN_USE) */

#if (SERIALPORT1_IN_USE)
void serialport_rx_tx_interrupt_callback_uart2(void)
{
    rx_tx_interrupt_callback_uart2();
}
#endif /* (SERIALPORT1_IN_USE) */

#if ((SERIALPORT2_IN_USE) || (SERIALPORT3_IN_USE) || (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE))
void serialport_rx_tx_interrupt_callback_uart3456(void)
{
    rx_tx_interrupt_callback_uart3456();
}
#endif /* ((SERIALPORT2_IN_USE) || (SERIALPORT3_IN_USE) || (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE)) */

#endif /* --> (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC) */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    IRQn_Type irqn;

    #if (RELOCATE_VECTORS_TO_RAM == 1)
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }
    #endif /* --> (RELOCATE_VECTORS_TO_RAM == 1) */

    irqn = (IRQn_Type) -128;
    irqn = irqn_from_enum(portNumber);
    if (irqn != (IRQn_Type) -128)
    {
        #if (SERIALPORT0_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_0)
        {
            uart_1_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart1);
        }
        #endif /* (SERIALPORT0_IN_USE) */

        #if (SERIALPORT1_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_1)
        {
            uart_2_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart2);
        }
        #endif /* (SERIALPORT1_IN_USE) */

        #if (SERIALPORT2_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_2)
        {
            uart_3_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT2_IN_USE) */

        #if (SERIALPORT3_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_3)
        {
            uart_4_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT3_IN_USE) */

        #if (SERIALPORT4_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_4)
        {
            uart_5_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT4_IN_USE) */

        #if (SERIALPORT5_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_5)
        {
            uart_6_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT5_IN_USE) */
    }
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        /* TX FIFO interrupt enabled at start of Tx
           SET_BIT(huart->Instance->CR1, USART_CR1_TXFEIE);
           SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);
        */
    }
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        CLEAR_BIT(huart->Instance->CR3, USART_CR3_TXFTIE); /* TXFIFO threshold interrupt enable */
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_TCIE);   /* Transmission Complete Interrupt Enable */
    }
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    #if (RELOCATE_VECTORS_TO_RAM == 1)
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }
    #endif /* --> (RELOCATE_VECTORS_TO_RAM == 1) */

    /* Tx uses the same interrupt as Rx: nothing to do. */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint8_t data;

    data = 0xFF; /* Invalid data mark */
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        data = (uint8_t)READ_REG(huart->Instance->RDR);
    }

    return data;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint32_t isrflags;
    SERIALPORT_TXSTATUS status;

    status = SERIALPORT_RXSTATUS_NONE;
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        isrflags = READ_REG(huart->Instance->ISR);
        if ((isrflags & USART_ISR_RXNE_RXFNE) != 0) /* Read Data Register Not Empty/RXFIFO Not Empty */
        {
            status = SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
        }


        /* Ignore and clear errors that are not used in the driver:                  */

        /* UART parity error interrupt occurred -------------------------------------*/
        if ((isrflags & USART_ISR_PE) != 0U)
        {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF);
        }

        /* UART frame error interrupt occurred --------------------------------------*/
        if ((isrflags & USART_ISR_FE) != 0U)
        {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_FEF);
        }

        /* UART noise error interrupt occurred --------------------------------------*/
        if ((isrflags & USART_ISR_NE) != 0U)
        {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_NEF);
        }

        /* UART Over-Run interrupt occurred -----------------------------------------*/
        if ((isrflags & USART_ISR_ORE) != 0U)
        {
            __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_OREF);
        }
    }

    return status;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Rx status flag is cleared automatically by reading from the received data register (RDR) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        SET_BIT(huart->Instance->RQR, UART_RXDATA_FLUSH_REQUEST);
    }
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        tx_complete[portNumber] = 0;
        huart->Instance->TDR = data;
        SET_BIT(huart->Instance->CR3, USART_CR3_TXFTIE); /* TXFIFO threshold interrupt enable */
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_TCIE); /* Transmission Complete Interrupt Enable */
    }
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint32_t isrflags;
    SERIALPORT_TXSTATUS status;

    status = SERIALPORT_TXSTATUS_NONE;
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        isrflags = READ_REG(huart->Instance->ISR);
        if (((isrflags & USART_ISR_TXFE) != 0) || /* TXFIFO Empty Flag */
            ((isrflags & USART_ISR_TXFT) != 0))   /* TXFIFO Threshold Flag */
        {
            status = SERIALPORT_TXSTATUS_FIFO_EMPTY;
        }
    }

    return status;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    UART_HandleTypeDef *huart;
    uint32_t cr1its;
    uint32_t cr3its;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
         if (status & SERIALPORT_TXSTATUS_FIFO_EMPTY)
         {
             cr1its = READ_REG(huart->Instance->CR1);
             cr3its = READ_REG(huart->Instance->CR3);
             if (cr3its & USART_CR3_TXFTIE) /* TXFIFO threshold interrupt enable */
             {
                 CLEAR_BIT(huart->Instance->CR3, USART_CR3_TXFTIE); /* TXFIFO threshold interrupt enable */
                 SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);     /* Transmission Complete Interrupt Enable */
             }
             else if (cr1its & USART_CR1_TCIE) /* Transmission Complete Interrupt Enable */
             {
                 CLEAR_BIT(huart->Instance->CR1, USART_CR1_TCIE);   /* Transmission Complete Interrupt Enable */
                 CLEAR_BIT(huart->Instance->CR3, USART_CR3_TXFTIE); /* TXFIFO threshold interrupt enable */
                 tx_complete[portNumber] = 1;
             }
         }
    }
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    return tx_complete[portNumber];
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_STM_STM32F4xx)

/*
 * STM32F4xx specific:
 *
 * SERIALPORT_NUMBER_0 ... USART2 (UART_HandleTypeDef huart1 from usart.c)
 * SERIALPORT_NUMBER_1 ... UART4 (UART_HandleTypeDef huart2 from usart.c)
 */

#if ((SERIALPORT6_IN_USE) || (SERIALPORT7_IN_USE) || (SERIALPORT8_IN_USE) || (SERIALPORT9_IN_USE) || (SERIALPORT10_IN_USE) ||  \
     (SERIALPORT11_IN_USE) || (SERIALPORT12_IN_USE) || (SERIALPORT13_IN_USE) || (SERIALPORT14_IN_USE) || (SERIALPORT15_IN_USE))
#error SERIALPORT: STM32F4xx supports only 6 UARTs !!!
#endif

#include "usart.h"

UART_HandleTypeDef *huart_from_enum(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
UART_HandleTypeDef *huart_from_enum(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    switch (portNumber)
    {
        #if SERIALPORT0_IN_USE
        case SERIALPORT_NUMBER_0:
            huart = &huart1;
            break;
        #endif
        #if SERIALPORT1_IN_USE
        case SERIALPORT_NUMBER_1:
            huart = &huart2;
            break;
        #endif
        #if SERIALPORT2_IN_USE
        case SERIALPORT_NUMBER_2:
            huart = &huart2;
            break;
        #endif
        #if SERIALPORT3_IN_USE
        case SERIALPORT_NUMBER_3:
            huart = &huart4;
            break;
        #endif
        #if SERIALPORT4_IN_USE
        case SERIALPORT_NUMBER_4:
            huart = &huart4;
            break;
        #endif
        #if SERIALPORT5_IN_USE
        case SERIALPORT_NUMBER_5:
            huart = &huart6;
            break;
        #endif
        default:
            huart = NULL;
            break;
    } /* -> switch (portNumber) */

    return huart;
}

IRQn_Type irqn_from_enum(SERIALPORT_NUMBER portNumber)
{
    IRQn_Type irqn;

    switch (portNumber)
    {
        #if SERIALPORT0_IN_USE
        case SERIALPORT_NUMBER_0:
            irqn = USART1_IRQn;
            break;
        #endif
        #if SERIALPORT1_IN_USE
        case SERIALPORT_NUMBER_1:
            irqn = USART2_IRQn;
            break;
        #endif
        #if SERIALPORT2_IN_USE
        case SERIALPORT_NUMBER_2:
            irqn = USART2_IRQn;
        #endif
        #if SERIALPORT3_IN_USE
        case SERIALPORT_NUMBER_3:
            irqn = UART4_IRQn;
            break;
        #endif
        #if SERIALPORT4_IN_USE
        case SERIALPORT_NUMBER_4:
            irqn = UART4_IRQn;
        #endif
        #if SERIALPORT5_IN_USE
        case SERIALPORT_NUMBER_5:
        #endif
        default:
            irqn = (IRQn_Type) -128;
            break;

    } /* -> switch (portNumber) */

    return irqn;
}

SERIALPORT_BAUDRATE default_baudrate_from_enum(SERIALPORT_NUMBER portNumber)
{
    SERIALPORT_BAUDRATE baudrate;

    baudrate = 0;

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        baudrate = SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE;                          \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    return baudrate;
}

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    SERIALPORT_BAUDRATE baudrate;

    /* HW pins initialization already done. */

    tx_complete[portNumber] = 1;

    baudrate = 0;
    baudrate = default_baudrate_from_enum(portNumber);
    if (baudrate != 0)
    {
        serialport_baudrate_set(portNumber, baudrate);
    }
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    UART_HandleTypeDef *huart;
    uint32_t pclk;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        /* Logical copy from MX_USART1_UART_Init(): */
        huart->Init.BaudRate = baudrate;
        huart->Init.WordLength = UART_WORDLENGTH_8B;
        huart->Init.StopBits = UART_STOPBITS_1;
        huart->Init.Parity = UART_PARITY_NONE;
        huart->Init.Mode = UART_MODE_TX_RX;
        huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
        huart->Init.OverSampling = UART_OVERSAMPLING_16;
    }

    /* Calculating actual baudrate (logical copy of code from UART_SetConfig()): */

    pclk = 0;
#if defined(USART6) && defined(UART9) && defined(UART10)
    if ((huart->Instance == USART1) || (huart->Instance == USART6) || (huart->Instance == UART9) || (huart->Instance == UART10))
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#elif defined(USART6)
    if ((huart->Instance == USART1) || (huart->Instance == USART6))
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#else
    if (huart->Instance == USART1)
    {
      pclk = HAL_RCC_GetPCLK2Freq();
    }
#endif /* USART6 */
    else
    {
      pclk = HAL_RCC_GetPCLK1Freq();
    }
    if (pclk != 0)
    {
        baudrate = pclk / huart->Instance->BRR;
    }
    return baudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    IRQn_Type irqn;

    serialport_ClearRxBuffer(portNumber);

    irqn = (IRQn_Type) -128;
    irqn = irqn_from_enum(portNumber);
    if (irqn != (IRQn_Type) -128)
    {
        NVIC_ClearPendingIRQ(irqn);
        if (portNumber == WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER)
        {
            NVIC_SetPriority(irqn, PLATFORM_INTERRUPT_PRIORITY_NORMAL_2);
        }
        else
        {
            NVIC_SetPriority(irqn, PLATFORM_INTERRUPT_PRIORITY_LOW_3);
        }
        NVIC_EnableIRQ(irqn);
    }

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        SET_BIT(huart->Instance->CR1, USART_CR1_RXNEIE); /* RXNE/RXFIFO not empty Interrupt Enable */
    }
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    /* Disable only Rx part of IRQ, not whole interrupt. This allows TX works independently. */

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_RXNEIE); /* RXNE/RXFIFO not empty Interrupt Enable */
    }
}   /* serialport_RXinterruptDisable */


#if (SERIALPORT0_IN_USE)
static void (*uart_1_callback)(void) = NULL;
void rx_tx_interrupt_callback_uart1(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart1(void)
{
    if (uart_1_callback != NULL)
    {
        uart_1_callback();
    }
}
#endif /* (SERIALPORT0_IN_USE) */

#if (SERIALPORT1_IN_USE)
static void (*uart_2_callback)(void) = NULL;
void rx_tx_interrupt_callback_uart2(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart2(void)
{
    if (uart_2_callback != NULL)
    {
        uart_2_callback();
    }
}
#endif /* (SERIALPORT1_IN_USE) */

#if (SERIALPORT3_IN_USE)
static void (*uart_4_callback)(void) = NULL;
void rx_tx_interrupt_callback_uart4(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart4(void)
{
    if (uart_4_callback != NULL)
    {
        uart_4_callback();
    }
}
#endif /* (SERIALPORT1_IN_USE) */

#if ((SERIALPORT2_IN_USE)|| (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE))
#if (SERIALPORT2_IN_USE)
static void (*uart_3_callback)(void) = NULL;
#endif /* (SERIALPORT2_IN_USE) */
#if (SERIALPORT4_IN_USE)
static void (*uart_5_callback)(void) = NULL;
#endif /* (SERIALPORT4_IN_USE) */
#if (SERIALPORT5_IN_USE)
static void (*uart_6_callback)(void) = NULL;
#endif /* (SERIALPORT5_IN_USE) */
void rx_tx_interrupt_callback_uart456(void) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void rx_tx_interrupt_callback_uart456(void)
{
    #if (SERIALPORT2_IN_USE)
    if (uart_3_callback != NULL)
    {
        uart_3_callback();
    }
    #endif /* (SERIALPORT2_IN_USE) */
    #if (SERIALPORT4_IN_USE)
    if (uart_5_callback != NULL)
    {
        uart_5_callback();
    }
    #endif /* (SERIALPORT4_IN_USE) */
    #if (SERIALPORT5_IN_USE)
    if (uart_6_callback != NULL)
    {
        uart_6_callback();
    }
    #endif /* (SERIALPORT5_IN_USE) */
}
#endif /* ((SERIALPORT2_IN_USE) || (SERIALPORT4_IN_USE) || (SERIALPORT5_IN_USE)) */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    IRQn_Type irqn;

    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    irqn = (IRQn_Type) -128;
    irqn = irqn_from_enum(portNumber);
    if (irqn != (IRQn_Type) -128)
    {
        #if (SERIALPORT0_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_0)
        {
            uart_1_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart1);
        }
        #endif /* (SERIALPORT0_IN_USE) */

        #if (SERIALPORT1_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_1)
        {
            uart_2_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart2);
        }
        #endif /* (SERIALPORT1_IN_USE) */

        #if (SERIALPORT2_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_2)
        {
            uart_3_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT2_IN_USE) */

        #if (SERIALPORT3_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_3)
        {
            uart_4_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart4);
        }
        #endif /* (SERIALPORT3_IN_USE) */

        #if (SERIALPORT4_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_4)
        {
            uart_5_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT4_IN_USE) */

        #if (SERIALPORT5_IN_USE)
        if (portNumber == SERIALPORT_NUMBER_5)
        {
            uart_6_callback = (*address);
            NVIC_SetVector(irqn, (uint32_t) rx_tx_interrupt_callback_uart3456);
        }
        #endif /* (SERIALPORT5_IN_USE) */
    }
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        /* TX FIFO interrupt enabled at start of Tx
           SET_BIT(huart->Instance->CR1, USART_CR1_TXFEIE);
           SET_BIT(huart->Instance->CR1, USART_CR1_TCIE);
        */
    }
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        CLEAR_BIT(huart->Instance->CR1, USART_CR1_TCIE);   /* Transmission Complete Interrupt Enable */
    }
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    /* Tx uses the same interrupt as Rx: nothing to do. */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint8_t data;

    data = 0xFF; /* Invalid data mark */
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        data = (uint8_t)READ_REG(huart->Instance->DR);
    }

    return data;
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint32_t isrflags;
    SERIALPORT_TXSTATUS status;

    status = SERIALPORT_RXSTATUS_NONE;
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        isrflags = READ_REG(huart->Instance->SR);
        if ((isrflags & USART_SR_RXNE) != 0) /* Read Data Register Not Empty/RXFIFO Not Empty */
        {
            status = SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
        }


        /* Ignore and clear errors that are not used in the driver:                  */

        /* UART parity error interrupt occurred -------------------------------------*/
        if ((isrflags & USART_SR_PE) != 0U)
        {
        	__HAL_UART_CLEAR_PEFLAG(huart);
        }

        /* UART frame error interrupt occurred --------------------------------------*/
        if ((isrflags & USART_SR_FE) != 0U)
        {
        	__HAL_UART_CLEAR_FEFLAG(huart);
        }

        /* UART noise error interrupt occurred --------------------------------------*/
        if ((isrflags & USART_SR_NE) != 0U)
        {
        	__HAL_UART_CLEAR_NEFLAG(huart);
        }

        /* UART Over-Run interrupt occurred -----------------------------------------*/
        if ((isrflags & USART_SR_ORE) != 0U)
        {
        	__HAL_UART_CLEAR_OREFLAG(huart);
        }
    }

    return status;
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Rx status flag is cleared automatically by reading from the received data register (RDR) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
    	__HAL_UART_FLUSH_DRREGISTER(huart);
    }
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    UART_HandleTypeDef *huart;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        tx_complete[portNumber] = 0;
        huart->Instance->DR = data;
        SET_BIT(huart->Instance->CR1, USART_CR1_TCIE); /* Transmission Complete Interrupt Enable */
    }
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    UART_HandleTypeDef *huart;
    uint32_t isrflags;
    SERIALPORT_TXSTATUS status;

    status = SERIALPORT_TXSTATUS_NONE;
    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
        isrflags = READ_REG(huart->Instance->SR);
        if (((isrflags & USART_SR_TXE) != 0) || /* TXFIFO Empty Flag */
            ((isrflags & USART_SR_TC) != 0))   /* TXFIFO Threshold Flag */
        {
            status = SERIALPORT_TXSTATUS_FIFO_EMPTY;
        }
    }

    return status;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    UART_HandleTypeDef *huart;
    uint32_t cr1its;

    huart = NULL;
    huart = huart_from_enum(portNumber);
    if (huart != NULL)
    {
         if (status & SERIALPORT_TXSTATUS_FIFO_EMPTY)
         {
             cr1its = READ_REG(huart->Instance->CR1);
             if (cr1its & USART_CR1_TCIE) /* Transmission Complete Interrupt Enable */
             {
                 CLEAR_BIT(huart->Instance->CR1, USART_CR1_TCIE);   /* Transmission Complete Interrupt Enable */
                 tx_complete[portNumber] = 1;
             }
         }
    }
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    return tx_complete[portNumber];
}   /* serialport_TxComplete */


#elif (PLATFORM_USED == PLATFORM_STM32_STM32GXX)

#include <uart/uart/uart.h>

#if UART_USE_RTOS
#error "!! SHOULD NOT BE USED IN THIS PROJECT SINCE HISENSE USES THIS UART DIRVER IN A WAY WHERE DATA BYTES ARE SENT WITHIN TX IRQ !!"
#endif

#define SERIALPORT_NUMBER(PORT_NUMBER)                                          \
static void uart##PORT_NUMBER##_rx_callback(const uint8_t byte);                \
static void uart##PORT_NUMBER##_tx_callback();
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

#define SERIALPORT_NUMBER(PORT_NUMBER)                                          \
{                                                                               \
      .module_num = eUART_MODULE_##PORT_NUMBER,                                 \
      .baudrate = SERIALPORT##PORT_NUMBER##_DEFAULT_BAUDRATE,                   \
      .parity = eUART_PARITY_NONE,                                              \
      .stop_bits = eUART_STOP_BIT_1,                                            \
      .shiftdir = eUART_SHIFTDIR_LSB_FIRST,                                     \
      .tx_buffer_size = 256,                                                    \
      .p_rx_callback = uart##PORT_NUMBER##_rx_callback,                         \
      .p_tx_callback = uart##PORT_NUMBER##_tx_callback                          \
},
static uart_init_t g_uart[] =
{
        SERIALPORT_NUMBERS
};
#undef SERIALPORT_NUMBER
static p_uart_module_t g_uart_module[SERIALPORT_NUMBER_COUNT];
static struct
{
    uint8_t data;
    bool data_ready;
} g_rx_data[SERIALPORT_NUMBER_COUNT];


void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    /* Setup serialport hardware */

    #define SERIALPORT_NUMBER(PORT_NUMBER)                                              \
    if (portNumber == SERIALPORT_NUMBER_##PORT_NUMBER)                                  \
    {                                                                                   \
        uart_init(&g_uart[PORT_NUMBER], &g_uart_module[PORT_NUMBER]);                   \
        serialport_ClearRxBuffer(portNumber);                                           \
    }
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER

    /* Implement for other platform(s) */
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    /* Implement for other platform(s) */
    return g_uart[portNumber].baudrate;
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* Implement for other platform(s) */
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}                                                                       \
void serialport_TX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_TX_port##PORT_NUMBER##_ISR(void);
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* Implement for other platform(s) */
}   /* serialport_TXinterrupt_register_ISR */

PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    return g_rx_data[portNumber].data;
}   /* serialport_ReadRxData */

PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    if(g_rx_data[portNumber].data_ready)
        return SERIALPORT_RXSTATUS_FIFO_NOTEMPTY;
    else
        return SERIALPORT_RXSTATUS_NONE;
}   /* serialport_ReadRxStatus */

PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    /* Implement for other platform(s) */
    g_rx_data[portNumber].data_ready = false;
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    g_rx_data[portNumber].data_ready = false;
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    /* Implement for other platform(s) */
    uart_send(g_uart_module[portNumber], &data, 1);
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    if(uart_is_tx_not_full(g_uart_module[portNumber]))
        return SERIALPORT_TXSTATUS_FIFO_EMPTY;
    else
        return SERIALPORT_TXSTATUS_NONE;
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    /* Implement for other platform(s) */
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    // return   1 - Transmitting is not in progress (shift register empty);  0 - transmitting is in progress (shift register not empty).
    if(uart_is_tx_in_progress(g_uart_module[portNumber]))
        return 0;
    else
        return 1;
}   /* serialport_TxComplete */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                          \
static void uart##PORT_NUMBER##_rx_callback(const uint8_t byte)                 \
{                                                                               \
    g_rx_data[PORT_NUMBER].data = byte;                                         \
    g_rx_data[PORT_NUMBER].data_ready = true;                                   \
    serialport_RX_port##PORT_NUMBER##_ISR();                                    \
}                                                                               \
static void uart##PORT_NUMBER##_tx_callback()                                   \
{                                                                               \
    serialport_TX_port##PORT_NUMBER##_ISR();                                    \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

#elif (PLATFORM_USED == PLATFORM_template)

#warning serialport.c: Implement new platform:

static uint8_t tx_complete[SERIALPORT_NUMBER_COUNT];

void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin)
{
    /* Setup serialport hardware */
    
    tx_complete[portNumber] = 1;                                                    \
    
    serialport_baudrate_set(portNumber, ___baud_rate___);
    
    /* Implement for other platform(s) */
}   /* serialport_init */

uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate)
{
    /* Implement for other platform(s) */
}   /* serialport_baudrate_set */

void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */

    #warning: Serial port that is primary used for aFOTA FW update (where WiFi module is actually connected) must have higher interrupt priority than other UARTs.

}   /* serialport_RXinterruptEnable */

void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_RXinterruptDisable */

void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    /* Implement for other platform(s) */
}   /* serialport_RXinterrupt_register_ISR */

#define SERIALPORT_NUMBER(PORT_NUMBER)                                  \
void serialport_RX_port##PORT_NUMBER##_ISR(void) PROC_RAM_FUNCTION;     \
PROC_RAM_FUNCTION_ALWAYS_INFRONT                                        \
void serialport_RX_port##PORT_NUMBER##_ISR(void)                        \
{                                                                       \
    serialport_callback_data[PORT_NUMBER].RXdataCallback(               \
    serialport_callback_data[PORT_NUMBER].RXlayer);                     \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */

    #warning: Serial port that is primary used for aFOTA FW update (where WiFi module is actually connected) must have higher interrupt priority than other UARTs.

}   /* serialport_TXinterruptEnable */

void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_TXinterruptDisable */

void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address)
{
    /* Check if interrupt vectors are in RAM: */
    if (((ISRaddress)PROC_INT_VECT_TABLE >= (ISRaddress)RESET_CODE_START_ADDRESS) &&
        ((ISRaddress)PROC_INT_VECT_TABLE <  (ISRaddress)(RESET_CODE_START_ADDRESS + MCU_FLASH_SIZE)))
    {
        /* Vectors must be in RAM !!!
           proc_init() function must be called before to relocate vectors from ROM to RAM !!! */
        proc_halt(0);
    }

    /* Implement for other platform(s) */
}   /* serialport_TXinterrupt_register_ISR */

uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber)
{
    #warning Function must be executed from RAM !!!
    /* Implement for other platform(s) */
}   /* serialport_ReadRxData */

SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber)
{
    #warning Function must be executed from RAM !!!
    /* Implement for other platform(s) */
}   /* serialport_ReadRxStatus */

void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status) PROC_RAM_FUNCTION;
PROC_RAM_FUNCTION_ALWAYS_INFRONT
void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status)
{
    #warning Function must be executed from RAM !!!
    /* Implement for other platform(s) */
}   /* serialport_ClearRxStatus */

void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_ClearRxBuffer */

void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data)
{
    /* Implement for other platform(s) */
}   /* serialport_WriteTxData */

SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
}   /* serialport_ReadTxStatus */

void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status)
{
    /* Implement for other platform(s) */
}   /* serialport_ClearTxStatus */

uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber)
{
    /* Implement for other platform(s) */
    #warning TODO:
    
    /* 
    Return:
         1 - Transmitting is not in progress (shift register empty)
         0 - transmitting is in progress (shift register not empty)
    */
    
    return 1;
}   /* serialport_TxComplete */

#elif (PLATFORM_USED == PLATFORM_10)

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/

#else
    #error Serialport.c: Undefined or unsupported platform
#endif /* PLATFORM_USED */


/* 
Common part for all platforms:
*/

ISRaddress serialport_RX_portx_ISR[SERIALPORT_NUMBER_COUNT]=
{
    #define SERIALPORT_NUMBER(PORT_NUMBER) serialport_RX_port##PORT_NUMBER##_ISR,
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
};

void serialport_RX_data_callback_register(SERIALPORT_NUMBER portNumber, void* address, void* layer)
{
    if ((portNumber > SERIALPORT_NUMBER_COUNT) ||
        (address == NULL) ||
        (layer == NULL))
    {
        return;
    }

    serialport_callback_data[portNumber].RXdataCallback = (callbackFunc) address;
    serialport_callback_data[portNumber].RXlayer        = layer;

    serialport_RXinterrupt_register_ISR(portNumber, serialport_RX_portx_ISR[portNumber]);
}



#define SERIALPORT_NUMBER(PORT_NUMBER)                          \
void serialport_TX_port##PORT_NUMBER##_ISR(void)                \
{                                                               \
    serialport_callback_data[PORT_NUMBER].TXdataCallback(       \
    serialport_callback_data[PORT_NUMBER].TXlayer);             \
}
SERIALPORT_NUMBERS
#undef SERIALPORT_NUMBER

ISRaddress serialport_TX_portx_ISR[SERIALPORT_NUMBER_COUNT]=
{
    #define SERIALPORT_NUMBER(PORT_NUMBER) serialport_TX_port##PORT_NUMBER##_ISR,
    SERIALPORT_NUMBERS
    #undef SERIALPORT_NUMBER
};

void serialport_TX_data_callback_register(SERIALPORT_NUMBER portNumber, void* address, void* layer)
{
    if ((portNumber > SERIALPORT_NUMBER_COUNT) ||
        (address == NULL) ||
        (layer == NULL))
    {
        return;
    }

    serialport_callback_data[portNumber].TXdataCallback = (callbackFunc) address;
    serialport_callback_data[portNumber].TXlayer        = layer;

    serialport_TXinterrupt_register_ISR(portNumber, serialport_TX_portx_ISR[portNumber]);
}

/* [] END OF FILE */
