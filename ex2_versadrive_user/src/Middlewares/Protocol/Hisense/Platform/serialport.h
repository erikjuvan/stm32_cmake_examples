//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        serialport.h
// CREATION DATE:      2020/01/14
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/01/21
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Managing all communication ports used in project - Hardware Abstraction Layer
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef SERIALPORT_HAL_H
#define SERIALPORT_HAL_H

#include "types.h"
#include "cfg/project_cfg.h"
#include "Platform/proc.h"


#ifndef SERIALPORT0_IN_USE
#error SERIALPORT: "SERIALPORT<n>_IN_USE" defines should be set !
#endif

#if ((SERIALPORT0_IN_USE == 0)  && (SERIALPORT1_IN_USE == 0)  && (SERIALPORT2_IN_USE == 0)  && \
     (SERIALPORT3_IN_USE == 0)  && (SERIALPORT4_IN_USE == 0)  && (SERIALPORT5_IN_USE == 0)  && \
     (SERIALPORT6_IN_USE == 0)  && (SERIALPORT7_IN_USE == 0)  && (SERIALPORT8_IN_USE == 0)  && \
     (SERIALPORT9_IN_USE == 0)  && (SERIALPORT10_IN_USE == 0) && (SERIALPORT11_IN_USE == 0) && \
     (SERIALPORT12_IN_USE == 0) && (SERIALPORT13_IN_USE == 0) && (SERIALPORT14_IN_USE == 0) && \
     (SERIALPORT15_IN_USE == 0))
#error SERIALPORT: At least one serial port (SERIALPORT<n>_IN_USE) should be used !
#endif


typedef enum serialport_number_t
{
    #if (SERIALPORT16_IN_USE)
    #error SERIALPORT:  Maximum 16 serial ports are supported !
    #elif (SERIALPORT15_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    SERIALPORT_NUMBER_11,
    SERIALPORT_NUMBER_12,
    SERIALPORT_NUMBER_13,
    SERIALPORT_NUMBER_14,
    SERIALPORT_NUMBER_15,
    #elif (SERIALPORT14_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    SERIALPORT_NUMBER_11,
    SERIALPORT_NUMBER_12,
    SERIALPORT_NUMBER_13,
    SERIALPORT_NUMBER_14,
    #elif (SERIALPORT13_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    SERIALPORT_NUMBER_11,
    SERIALPORT_NUMBER_12,
    SERIALPORT_NUMBER_13,
    #elif (SERIALPORT12_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    SERIALPORT_NUMBER_11,
    SERIALPORT_NUMBER_12,
    #elif (SERIALPORT11_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    SERIALPORT_NUMBER_11,
    #elif (SERIALPORT10_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    SERIALPORT_NUMBER_10,
    #elif (SERIALPORT9_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    SERIALPORT_NUMBER_9,
    #elif (SERIALPORT8_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    SERIALPORT_NUMBER_8,
    #elif (SERIALPORT7_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    SERIALPORT_NUMBER_7,
    #elif (SERIALPORT6_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    SERIALPORT_NUMBER_6,
    #elif (SERIALPORT5_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    SERIALPORT_NUMBER_5,
    #elif (SERIALPORT4_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    SERIALPORT_NUMBER_4,
    #elif (SERIALPORT3_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    SERIALPORT_NUMBER_3,
    #elif (SERIALPORT2_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    SERIALPORT_NUMBER_2,
    #elif (SERIALPORT1_IN_USE)
    SERIALPORT_NUMBER_0,
    SERIALPORT_NUMBER_1,
    #elif (SERIALPORT0_IN_USE)
    SERIALPORT_NUMBER_0,
    #endif
    SERIALPORT_NUMBER_COUNT
} SERIALPORT_NUMBER;

/* Alternate RX pin for the same UART number. See serialport_init() function, alternate_rx_pin parameter. */
typedef enum SERIALPORT_ALTERNATE_RX_PIN_t
{
    SERIALPORT_ALTERNATE_RX_PIN_0,
    SERIALPORT_ALTERNATE_RX_PIN_1,
    SERIALPORT_ALTERNATE_RX_PIN_2,
    SERIALPORT_ALTERNATE_RX_PIN_3,
    SERIALPORT_ALTERNATE_RX_PIN_4,
    SERIALPORT_ALTERNATE_RX_PIN_5,
    SERIALPORT_ALTERNATE_RX_PIN_6,
    SERIALPORT_ALTERNATE_RX_PIN_7
} SERIALPORT_ALTERNATE_RX_PIN;

/* Alternate TX pin for the same UART number. See serialport_init() function, alternate_tx_pin parameter. */
typedef enum SERIALPORT_ALTERNATE_TX_PIN_t
{
    SERIALPORT_ALTERNATE_TX_PIN_0,
    SERIALPORT_ALTERNATE_TX_PIN_1,
    SERIALPORT_ALTERNATE_TX_PIN_2,
    SERIALPORT_ALTERNATE_TX_PIN_3,
    SERIALPORT_ALTERNATE_TX_PIN_4,
    SERIALPORT_ALTERNATE_TX_PIN_5,
    SERIALPORT_ALTERNATE_TX_PIN_6,
    SERIALPORT_ALTERNATE_TX_PIN_7
} SERIALPORT_ALTERNATE_TX_PIN;

typedef enum serialport_baudrate_t
{
    SERIALPORT_BAUDRATE_110           = 110,
    SERIALPORT_BAUDRATE_300           = 300,
    SERIALPORT_BAUDRATE_600           = 600,
    SERIALPORT_BAUDRATE_1200          = 1200,
    SERIALPORT_BAUDRATE_2400          = 2400,
    SERIALPORT_BAUDRATE_4800          = 4800,
    SERIALPORT_BAUDRATE_9600          = 9600,
    SERIALPORT_BAUDRATE_14400         = 14400,
    SERIALPORT_BAUDRATE_19200         = 19200,
    SERIALPORT_BAUDRATE_28800         = 28800,
    SERIALPORT_BAUDRATE_38400         = 38400,
    SERIALPORT_BAUDRATE_56000         = 56000,
    SERIALPORT_BAUDRATE_57600         = 57600,
    SERIALPORT_BAUDRATE_115200        = 115200,
    SERIALPORT_BAUDRATE_230400        = 230400,
    SERIALPORT_BAUDRATE_460800        = 460800,
    SERIALPORT_BAUDRATE_921600        = 921600,
    SERIALPORT_BAUDRATE_1000000       = 1000000,
    SERIALPORT_BAUDRATE_1152000       = 1152000,
    SERIALPORT_BAUDRATE_1500000       = 1500000,
    SERIALPORT_BAUDRATE_1843200       = 1843200,
    SERIALPORT_BAUDRATE_2000000       = 2000000,
    SERIALPORT_BAUDRATE_2500000       = 2500000,
    SERIALPORT_BAUDRATE_3000000       = 3000000,
    SERIALPORT_BAUDRATE_3500000       = 3500000,
    SERIALPORT_BAUDRATE_3686400       = 3686400,
    SERIALPORT_BAUDRATE_4000000       = 4000000,
    SERIALPORT_BAUDRATE_7372800       = 7372800
} SERIALPORT_BAUDRATE;

typedef enum serialport_rxstatus_t
{
    SERIALPORT_RXSTATUS_NONE                 = 0x00,
    SERIALPORT_RXSTATUS_FIFO_NOTEMPTY        = 0x01,
    SERIALPORT_RXSTATUS_1                    = 0x02,
    SERIALPORT_RXSTATUS_2                    = 0x04,
    SERIALPORT_RXSTATUS_3                    = 0x08,
    SERIALPORT_RXSTATUS_4                    = 0x10,
    SERIALPORT_RXSTATUS_5                    = 0x20,
    SERIALPORT_RXSTATUS_6                    = 0x40,
    SERIALPORT_RXSTATUS_7                    = 0x80
} SERIALPORT_RXSTATUS;


typedef enum serialport_txstatus_t
{
    SERIALPORT_TXSTATUS_NONE                 = 0x00,
    SERIALPORT_TXSTATUS_FIFO_EMPTY           = 0x01,
    SERIALPORT_TXSTATUS_1                    = 0x02,
    SERIALPORT_TXSTATUS_2                    = 0x04,
    SERIALPORT_TXSTATUS_3                    = 0x08,
    SERIALPORT_TXSTATUS_4                    = 0x10,
    SERIALPORT_TXSTATUS_5                    = 0x20,
    SERIALPORT_TXSTATUS_6                    = 0x40,
    SERIALPORT_TXSTATUS_7                    = 0x80
} SERIALPORT_TXSTATUS;



// DESCRIPTION:
//   Initializes specified communication port: configure hardware pin as UART and set default baudrate (SERIALPORT<n>_DEFAULT_BAUDRATE), 8 data bits, 1 stop bit and no parity.
// INPUTS:
//   portNumber         - Port number to initialize (see SERIALPORT_NUMBER). Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   alternate_rx_pin   - Alternate hardware Pin for Rx, if UART number <portNumber> can be used on more than one rx pins.
//   alternate_tx_pin   - Alternate hardware Pin for Tx, if UART number <portNumber> can be used on more than one tx pins.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_init(SERIALPORT_NUMBER portNumber, SERIALPORT_ALTERNATE_RX_PIN alternate_rx_pin, SERIALPORT_ALTERNATE_TX_PIN alternate_tx_pin);


// DESCRIPTION:
//   Sets baud rate, 8 data bits, 1 stop bit, no parity for specified communication port.
// INPUTS:
//   portNumber - Port number to initialize (see SERIALPORT_NUMBER). Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   baudrate   - Baudrate to set (see SERIALPORT_BAUDRATE).
// OUTPUT:
//   --
// RETURNS:
//   Actual baudrate set for the communication port (because of integer divider).
// ----------------------------------------------------------------------------
uint32_t serialport_baudrate_set(SERIALPORT_NUMBER portNumber, SERIALPORT_BAUDRATE baudrate);


// DESCRIPTION:
//   Enables RX interrupt for communication port.
// INPUTS:
//   portNumber - Port number to enable RX interrupt. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_RXinterruptEnable(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Disables RX interrupt for communication port.
// INPUTS:
//   portNumber - Port number to disable RX interrupt. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_RXinterruptDisable(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Registers interrupt service routine for RX interrupt for communication port "portNumber".
// INPUTS:
//   portNumber - Port number to register RX ISR. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   address - address of the interrupt service routine.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_RXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address);


// DESCRIPTION:
//   Enables TX interrupt for communication port "portNumber".
// INPUTS:
//   portNumber - Port number to enable TX interrupt. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_TXinterruptEnable(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Disables TX interrupt for communication port "portNumber".
// INPUTS:
//   portNumber - Port number to disable TX interrupt. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_TXinterruptDisable(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Registers interrupt service routine for TX interrupt for communication port "portNumber".
// INPUTS:
//   portNumber - Port number to register TX interrupt. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   address    - Address of the interrupt service routine.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_TXinterrupt_register_ISR(SERIALPORT_NUMBER portNumber, ISRaddress address);


// DESCRIPTION:
//   Reads one byte of data from communication port "portNumber".
// INPUTS:
//   portNumber - Port number from which to read data. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   Data read from communication port.
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT uint8_t serialport_ReadRxData(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Reads status of the Rx interrupt of the communication port "portNumber".
// INPUTS:
//   portNumber - Port number to retreive the status. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   Rx status of the WiFi communication port:
//   See SERIALPORT_RXSTATUS. SERIALPORT_RXSTATUS_FIFO_NOTEMPTY is set, when new data is available in Rx FIFO.
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT SERIALPORT_RXSTATUS serialport_ReadRxStatus(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Clears status of the Rx interrupt of the communication port "portNumber".
// INPUTS:
//   portNumber - Port number to clear RX status. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   status     - status to clear (see SERIALPORT_RXSTATUS). SERIALPORT_RXSTATUS_FIFO_NOTEMPTY status can be cleared. 
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT void serialport_ClearRxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_RXSTATUS status);


// DESCRIPTION:
//   Clears Rx hardwade FIFO buffer of the communication port "portNumber".
// INPUTS:
//   portNumber - Port number to clear RX buffer. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_ClearRxBuffer(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Writes data to communication port "portNumber".
// INPUTS:
//   portNumber - Port number to write data. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   data       - Data to write to communication port.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_WriteTxData(SERIALPORT_NUMBER portNumber, uint8_t data);


// DESCRIPTION:
//   Reads status of the Tx interrupt of the communication port "portNumber".
// INPUTS:
//   portNumber - Port number to read TX status. Port number enumeration is the same as port number enumeration in MCU's datasheet.
// OUTPUT:
//   --
// RETURNS:
//   Tx status of the communication communication port, see SERIALPORT_TXSTATUS. SERIALPORT_TXSTATUS_FIFO_EMPTY is set, when Tx hardware FIFO buffer is empty.
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT SERIALPORT_TXSTATUS serialport_ReadTxStatus(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Clears status of the Tx interrupt of the communication port "portNumber".
// INPUTS:
//   portNumber - Port number to clear TX status. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   status     - Status to clear (see SERIALPORT_TXSTATUS). SERIALPORT_TXSTATUS_FIFO_EMPTY status can be cleared.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT void serialport_ClearTxStatus(SERIALPORT_NUMBER portNumber, SERIALPORT_TXSTATUS status);


/*******************************************************************************************************************//**
* @brief    Checks if UART's shift register is empty - not transmitting.
* 
* @param    portNumber - Port number to check of transmitting. Port number enumeration is the same as port number enumeration in MCU's datasheet.
* 
* @return   1 - Transmitting is not in progress (shift register empty);  0 - Transmitting is in progress (shift register not empty).

* @details  Example:
* @code
*    serialport_WriteTxData(0, 'a');
*    while (serialport_TxComplete(0) == 0)
*    {
*       // Wait transmitting is finished
*    }
* @endcode
*
***********************************************************************************************************************/
uint8_t serialport_TxComplete(SERIALPORT_NUMBER portNumber);


// DESCRIPTION:
//   Registers data callback function that has to be called when new data is received.
// INPUTS:
//   portNumber - Port number to register data callback. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   address    - Address of the data callback function.
//   layer      - Layer address which is passed into data callback function when new data is received.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_RX_data_callback_register(SERIALPORT_NUMBER portNumber, void* address, void* layer);


// DESCRIPTION:
//   Registers data callback function that has to be called when data is sent to port (FIFO empty).
// INPUTS:
//   portNumber - Port number to register data callback. Port number enumeration is the same as port number enumeration in MCU's datasheet.
//   address    - Address of the data callback function.
//   layer      - Layer address which is passed into data callback function when data is sent to port (FIFO empty) 
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void serialport_TX_data_callback_register(SERIALPORT_NUMBER portNumber, void* address, void* layer);


#if (PLATFORM_USED == PLATFORM_STM8_STM8S207)
INTERRUPT void UART1TXInterrupt(void);
INTERRUPT void UART1RXInterrupt(void);
INTERRUPT void UART3TXInterrupt(void);
INTERRUPT void UART3RXInterrupt(void);
#endif /* --> (PLATFORM_USED == PLATFORM_STM8_STM8S207) */

#if (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC)
void serialport_rx_tx_interrupt_callback_uart1(void);
void serialport_rx_tx_interrupt_callback_uart2(void);
void serialport_rx_tx_interrupt_callback_uart3456(void);
#endif /* --> (PROJECT_NAME == PROJECT_NAME_DW60_3_GCUP_BLDC) */



#endif /* SERIALPORT_HAL_H */
/* [] END OF FILE */
