//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        project_cfg_common.h
// CREATION DATE:      2020/09/02
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2021/11/13
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Configuration of project functionality for NXP LPC546XX platform.
//   This file is common for Bootloader and Application projects.
// ----------------------------------------------------------------------------
// Copyright (C) 2020-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PROJECT_CFG_COMMON_H
#define PROJECT_CFG_COMMON_H

#include "project_names.h"
#include "types.h"

/* Define which functionality is active on which WiFi instance, and settings */
#define WIFI_INSTANCE0_HISENSE_PROTOCOL_USE                 1
#define WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER            SERIALPORT_NUMBER_0
#define WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN SERIALPORT_ALTERNATE_RX_PIN_0
#define WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN SERIALPORT_ALTERNATE_TX_PIN_0
#define WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS           0
#define WIFI_INSTANCE0_FORWARDING_PORT0_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE0_FORWARDING_PORT1_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE0_FORWARDING_PORT2_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE0_FORWARDING_PORT3_WIFILAYERREFERENCE  NULL

#define WIFI_INSTANCE1_HISENSE_PROTOCOL_USE                 1
#define WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER            SERIALPORT_NUMBER_1
#define WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN SERIALPORT_ALTERNATE_RX_PIN_0
#define WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN SERIALPORT_ALTERNATE_TX_PIN_0
#define WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS           0
#define WIFI_INSTANCE1_FORWARDING_PORT0_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE1_FORWARDING_PORT1_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE1_FORWARDING_PORT2_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE1_FORWARDING_PORT3_WIFILAYERREFERENCE  NULL

#define WIFI_INSTANCE2_HISENSE_PROTOCOL_USE                 0
#define WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER            SERIALPORT_NUMBER_15
#define WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN SERIALPORT_ALTERNATE_RX_PIN_0
#define WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN SERIALPORT_ALTERNATE_TX_PIN_0
#define WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS           0
#define WIFI_INSTANCE2_FORWARDING_PORT0_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE2_FORWARDING_PORT1_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE2_FORWARDING_PORT2_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE2_FORWARDING_PORT3_WIFILAYERREFERENCE  NULL

#define WIFI_INSTANCE3_HISENSE_PROTOCOL_USE                 0
#define WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER            SERIALPORT_NUMBER_15
#define WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN SERIALPORT_ALTERNATE_RX_PIN_0
#define WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN SERIALPORT_ALTERNATE_TX_PIN_0
#define WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS           0
#define WIFI_INSTANCE3_FORWARDING_PORT0_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE3_FORWARDING_PORT1_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE3_FORWARDING_PORT2_WIFILAYERREFERENCE  NULL
#define WIFI_INSTANCE3_FORWARDING_PORT3_WIFILAYERREFERENCE  NULL

/* On which serial port WiFi module is actually connected: */
#define WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER               WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER

/* Number of serial ports used for Hisense protocol stack communication interfaces: */
#define NUMBER_OF_WIFI_INSTANCES                                                 \
    (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE + WIFI_INSTANCE1_HISENSE_PROTOCOL_USE + \
     WIFI_INSTANCE2_HISENSE_PROTOCOL_USE + WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)

/* Maximum number of interfaces the forwarding is used on: */
#define MAX_NUMBER_OF_FORWARD_PORTS                                                           \
    max4(                                                                                     \
        WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS, WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS, \
        WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS, WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS  \
    )

#define SERIALPORT0_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600   // PU (main board)
#define SERIALPORT1_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_230400 // MAT
#define SERIALPORT2_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT3_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT4_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT5_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT6_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT7_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT8_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT9_DEFAULT_BAUDRATE  SERIALPORT_BAUDRATE_9600
#define SERIALPORT10_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600
#define SERIALPORT11_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600
#define SERIALPORT12_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600
#define SERIALPORT13_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600
#define SERIALPORT14_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600
#define SERIALPORT15_DEFAULT_BAUDRATE SERIALPORT_BAUDRATE_9600

#define SERIALPORT0_IN_USE            1
#define SERIALPORT1_IN_USE            1
#define SERIALPORT2_IN_USE            0
#define SERIALPORT3_IN_USE            0
#define SERIALPORT4_IN_USE            0
#define SERIALPORT5_IN_USE            0
#define SERIALPORT6_IN_USE            0
#define SERIALPORT7_IN_USE            0
#define SERIALPORT8_IN_USE            0
#define SERIALPORT9_IN_USE            0
#define SERIALPORT10_IN_USE           0
#define SERIALPORT11_IN_USE           0
#define SERIALPORT12_IN_USE           0
#define SERIALPORT13_IN_USE           0
#define SERIALPORT14_IN_USE           0
#define SERIALPORT15_IN_USE           0
#define SERIALPORT16_IN_USE           0 /* Should not be set (Maximum 16 serial ports are supported) */

#define NUMBER_OF_SERIAL_PORTS_USED                                                        \
    (SERIALPORT0_IN_USE + SERIALPORT1_IN_USE + SERIALPORT2_IN_USE + SERIALPORT3_IN_USE +   \
     SERIALPORT4_IN_USE + SERIALPORT5_IN_USE + SERIALPORT6_IN_USE + SERIALPORT7_IN_USE +   \
     SERIALPORT8_IN_USE + SERIALPORT9_IN_USE + SERIALPORT10_IN_USE + SERIALPORT11_IN_USE + \
     SERIALPORT12_IN_USE + SERIALPORT13_IN_USE + SERIALPORT14_IN_USE + SERIALPORT15_IN_USE)

/* Number of masters in whole appliance (not just this MCU), including the WiFi module: */
#define NUMBER_OF_MASTERS                            1

/* For non-Windows based platforms, please comment out or remove this line: */
/* #define PC_PRINTF */

/* Value used at Triggering of start of FW update command (8#-11#): */
// #define HARDWARE_VERSION                                            0x3099AAAA

/* Hisense communication protocol definitions */
#define HISENSE_APPLIANCEADDRESSCODE_DEVICETYPE      0x08
#define HISENSE_APPLIANCEADDRESSCODE_DEVICEMCUNUMBER 0x02

#define HISENSE_PROTOCOL_BIGDATA_1_0_SUPPORTED       1
#define HISENSE_PROTOCOL_BIGDATA_1_1_SUPPORTED       0
#define HISENSE_PROTOCOL_BIGDATA_1_2_SUPPORTED       0
#define HISENSE_PROTOCOL_BIGDATA_1_3_SUPPORTED       0
#define HISENSE_PROTOCOL_BIGDATA_1_4_SUPPORTED       0

/* If application is "Class B" application */
#define APPLICATION_CLASS_B                          0

/*
----------------------------------------------------------------------------------------------------
Don't change values below:
----------------------------------------------------------------------------------------------------
*/

#define PROJECT_TYPE_APPLICATION                     0
#define PROJECT_TYPE_BOOTLOADER                      1

#define PROJECT_NAME                                 PROJECT_NAME_WM22_MCU

/*
How to transit from bootloader to application:
   - with system reset or
   - with jump to address
and from application to bootloader:
   - with system reset,
   - with jump to address or
   - with watchdog timeout reset:
*/
#define TRANSITION_RESET                             0
#define TRANSITION_JUMP                              1
#define TRANSITION_WATCHDOG_TIMEOUT_RESET            2

#define FW_UPDATE_BIGDATA_HEADER_SIZE                4

#define APPLICATION_LL_HEADER_SIZE                                                                 \
    6 /* Frame header(2 bytes) + LL Response ID(1 byte) + LL Identification 1 (1 byte) + NL length \
         (2 bytes) */
#define APPLICATION_NL_HEADER_SIZE                                                           \
    6 /* NL Response ID (1 byte) + NL Identification 1 (1 byte) + Address code 1 (2 bytes) + \
         Address code 2 (2 bytes) */
#define APPLICATION_TL_HEADER_SIZE 2 /* TL Response ID (1 byte) + TL Identification 1 (1 byte) */
#define APPLICATION_AL_HEADER_SIZE                                                                \
    6 /* Message type (1 byte) + Message Subtype (1 byte) + AL Response ID (1 byte) + AL frame (1 \
         byte) + AL Frame No (2 bytes) */
#define APPLICATION_LL_TAIL_SIZE 4 /* Checksum/CRC (2 bytes) + Frame tail (2 bytes) */

#if HISENSE_PROTOCOL_BIGDATA_1_4_SUPPORTED
#define FW_UPDATE_BIGDATA_MAX_DATA_SIZE                                                        \
    2048 /* Supported Hisense frames: 1#-0# (128 bytes), 1#-1# (256 bytes), 1#-2# (512 bytes), \
            1#-3# (1024 bytes), 1#-4# (2048 bytes) */
#elif HISENSE_PROTOCOL_BIGDATA_1_3_SUPPORTED
#define FW_UPDATE_BIGDATA_MAX_DATA_SIZE                                                        \
    1024 /* Supported Hisense frames: 1#-0# (128 bytes), 1#-1# (256 bytes), 1#-2# (512 bytes), \
            1#-3# (1024 bytes) */
#elif HISENSE_PROTOCOL_BIGDATA_1_2_SUPPORTED
#define FW_UPDATE_BIGDATA_MAX_DATA_SIZE \
    512 /* Supported Hisense frames: 1#-0# (128 bytes), 1#-1# (256 bytes), 1#-2# (512 bytes) */
#elif HISENSE_PROTOCOL_BIGDATA_1_1_SUPPORTED
#define FW_UPDATE_BIGDATA_MAX_DATA_SIZE \
    256 /* Supported Hisense frames: 1#-0# (128 bytes), 1#-1# (256 bytes) */
#elif HISENSE_PROTOCOL_BIGDATA_1_0_SUPPORTED
#define FW_UPDATE_BIGDATA_MAX_DATA_SIZE 128 /* Supported Hisense frame:  1#-0# (128 bytes) */
#else
#error At least one Hisense Big data frame should be enabled
#endif

#define APPLICATION_LL_MAX_FRAME_SIZE                                                       \
    (APPLICATION_LL_HEADER_SIZE + APPLICATION_NL_HEADER_SIZE + APPLICATION_TL_HEADER_SIZE + \
     APPLICATION_AL_HEADER_SIZE + FW_UPDATE_BIGDATA_HEADER_SIZE +                           \
     FW_UPDATE_BIGDATA_MAX_DATA_SIZE + APPLICATION_LL_TAIL_SIZE)
#define APPLICATION_AL_MAX_DATA_SIZE \
    (FW_UPDATE_BIGDATA_HEADER_SIZE + FW_UPDATE_BIGDATA_MAX_DATA_SIZE)

/* ****************************************************************************************************************
 */
/* Setup checking: */

#if (NUMBER_OF_WIFI_INSTANCES > 4)
#error Maximum number of WiFi instances is 4.
#endif /* (NUMBER_OF_WIFI_INSTANCES > 4) */

#if (NUMBER_OF_WIFI_INSTANCES != NUMBER_OF_SERIAL_PORTS_USED)
#error Number of WiFi instances must be the same as number of serial ports used.
#endif /* (NUMBER_OF_WIFI_INSTANCES != NUMBER_OF_SERIAL_PORTS_USED) */

#if (MAX_NUMBER_OF_FORWARD_PORTS > 4)
#error Maximum number of forwarding ports is 4.
#endif /* (MAX_NUMBER_OF_FORWARD_PORTS > 4) */

#if (MAX_NUMBER_OF_FORWARD_PORTS > (NUMBER_OF_WIFI_INSTANCES - 1))
#error Maximum number of FORWARD ports exceeded.
#endif /* (MAX_NUMBER_OF_FORWARD_PORTS > (NUMBER_OF_WIFI_INSTANCES - 1)) */

/* ****************************************************************************************************************
 */

/* *********************************************************************** */
/*                  MODULES CONFIGURATION                                  */
/* *********************************************************************** */

/***** NETBUF **************************************************************/
/* Optimization for lowest Big data frame that can be received (128 bytes).
   10 bytes for additional 0xF4 (stuffing bytes) that can be present in data.
   24 bytes is NETBUF header size (sizeof(struct t_netbuf_hdr)).
*/
#define NETBUF_SIZE                                                                         \
    (APPLICATION_LL_HEADER_SIZE + APPLICATION_NL_HEADER_SIZE + APPLICATION_TL_HEADER_SIZE + \
     APPLICATION_AL_HEADER_SIZE + FW_UPDATE_BIGDATA_HEADER_SIZE + 128 + 10 +                \
     APPLICATION_LL_TAIL_SIZE + 24)

#define NETBUF_POOL_BUFS_CALC \
    (NUMBER_OF_WIFI_INSTANCES * NUMBER_OF_MASTERS * (FW_UPDATE_BIGDATA_MAX_DATA_SIZE / 128))
#if (NETBUF_POOL_BUFS_CALC < 10)
#define NETBUF_POOL_BUFS 10
#else
#define NETBUF_POOL_BUFS NETBUF_POOL_BUFS_CALC
#endif

#define NETBUF_RESERVE_BUFS 0
#define NETBUF_TRACE_LEVEL  0

#endif /* PROJECT_CFG_COMMON_H */
/* [] END OF FILE */
