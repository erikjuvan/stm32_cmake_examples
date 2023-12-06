//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        serialprocess.h
// CREATION DATE:      2020/01/16
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/03/01
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Common communication handling and layer structures storage for all communication ports.
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.


#ifndef SERIALPROCESS_H
#define SERIALPROCESS_H

#include "types.h"
#include "Platform/serialport.h"

typedef enum
{
    SERIALPROCESS_NETBUFFER_ERROR_COUNTER,          /* How many times netbuffers pool run out or netbuf pool was invalid. Not related to port number. */
    SERIALPROCESS_HISENSE_COUNTER_WIFI_RX,          /* Whole LL packet came on RX                                                       */
    SERIALPROCESS_HISENSE_COUNTER_WIFI_RX_QUEUEFULL,/* Whole LL packet came on RX, but RX queue was full to process the packet          */
    SERIALPROCESS_HISENSE_COUNTER_WIFI_TX,          /* Whole LL packet sent out successfully within time anticipated by current baudrate*/
    SERIALPROCESS_HISENSE_COUNTER_WIFI_TX_QUEUEFULL,/* Reply was prepared to sent out, but TX queue was full                            */
    SERIALPROCESS_HISENSE_COUNTER_WIFI_TX_TIMEOUT,  /* Packet was not able to sent out within time anticipated by current baudrate.     */
    SERIALPROCESS_HISENSE_COUNTER_LL_CRCOK,         /* Hisense Link layer CRC verification OK.                                          */
    SERIALPROCESS_HISENSE_COUNTER_LL_CRCFAIL,       /* Hisense Link layer CRC verification failed.                                      */
    /*
    Add counters here, if needed.
    */
    SERIALPROCESS_HISENSE_COUNTER_MAX
} SERIALPROCESS_HISENSE_COUNTER;


// DESCRIPTION:
//   Initializes serial communication ports.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void serialprocess_init(void);


// DESCRIPTION:
//   Initializes serial communication port passed in parameter.
// INPUTS:
//   serialport - serial port to init
// OUTPUT:
//   --
// RETURNS:
//   1 - Initialization successful
//   0 - Initialization NOT successful (no wifilayer instance has serialport number passed in the parameter).
// ----------------------------------------------------------------------------
uint8_t serialprocess_port_init(SERIALPORT_NUMBER serialport);


// DESCRIPTION:
//   Returns Hisense protocol counters.
// INPUTS:
//   serialport - serial port for which to get the counter
//   counter    - which counter to get
// OUTPUT:
//   --
// RETURNS:
//   Value of the counter passed in parameter "counter".

// ----------------------------------------------------------------------------
uint32_t serialprocess_hisense_protocol_counter_get(SERIALPORT_NUMBER serialport, SERIALPROCESS_HISENSE_COUNTER counter);


// DESCRIPTION:
//   Function returns time from now in milliseconds when the last valid (CRC OK) frame was received on serial port
//   for this MCU or for MCU(s) on forwarding port(s).
//   If there was no packet received yet, the function returns 0xFFFFFFFF.
// INPUTS:
//   serialport - serial port for which to get the time
// OUTPUT:
//   --
// RETURNS:
//   Time in milliseconds elapsed from last correct frame received on the serialport.

// ----------------------------------------------------------------------------
uint32_t serialprocess_rx_packet_time_get(SERIALPORT_NUMBER serialport);


// DESCRIPTION:
//   Connect communication layers for instance 0.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void serialprocess_connect_layers_instance0(void);


// DESCRIPTION:
//   Connect communication layers for instance 1.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void serialprocess_connect_layers_instance1(void);


// DESCRIPTION:
//   Connect communication layers for instance 2.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void serialprocess_connect_layers_instance2(void);


// DESCRIPTION:
//   Connect communication layers for instance 3.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------  
void serialprocess_connect_layers_instance3(void);


// DESCRIPTION:
//   Check several setting consistency over all instances.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   1 - Settings are OK.
//   0 - Settings are NOT OK.
// ----------------------------------------------------------------------------  
uint8_t serialprocess_check_layer_consistency(void);


// DESCRIPTION:
//   Gets WiFi layer instance that uses the serialport passed in parameter.
// INPUTS:
//   serialport - serial port number to retrieve the WiFi layer for.
// OUTPUT: 
//   Memory address of the WiFi layer that uses the serialport passed in parameter.
// RETURNS:
//   1 - Returned value is correct
//   0 - Returned value is not correct (no wifilayer instance has serialport number passed in the parameter).
// ----------------------------------------------------------------------------  
uint8_t serialprocess_wifi_instance_get(SERIALPORT_NUMBER serialport, uint32_t* wifilayer_addr);


// DESCRIPTION:
//   Communication process function.
// INPUTS:
//   --
// OUTPUT: 
//   --
// RETURNS:
//   --
// ---------------------------------------------------------------------------- 
void serialprocess_run(void);


#endif /* SERIALPROCESS_H */

/* [] END OF FILE */
