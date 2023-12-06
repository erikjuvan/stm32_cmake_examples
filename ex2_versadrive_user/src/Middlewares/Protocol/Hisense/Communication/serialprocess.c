//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        serialprocess.c
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

#include <string.h>
#include "cfg/project_cfg.h"
#include "wifi.h"
#include "hisense/hisense_LinkL.h"
#include "hisense/hisense_NetworkL.h"
#include "hisense/hisense_TransmissionL.h"
#include "hisense/hisense_ApplicationL.h"
#include "serialprocess.h"


typedef struct serialprocess_nutbuf_ctrl_t
{
    TIMER   timer;
} SERIALPROCESS_NETBUF_CTRL;


typedef struct serialprocess_ctrl_t
{
    SERIALPROCESS_NETBUF_CTRL   netbuf_ctrl;
} SERIALPROCESS_CTRL;


#if (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE)
static WIFI_LAYER serialprocess_wifiLayer_instance0;
static HISENSE_LINKL_LAYER serialprocess_hisense_linkLayer_instance0;
static HISENSE_NETWORKL_LAYER serialprocess_hisense_networkl_layer_instance0;
static HISENSE_TRANSMISSIONL_LAYER serialprocess_hisense_transmissionl_layer_instance0;
static HISENSE_APPLICATIONL_LAYER serialprocess_hisense_applicationl_layer_instance0;
#endif /* WIFI_INSTANCE0_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE1_HISENSE_PROTOCOL_USE)
static WIFI_LAYER serialprocess_wifiLayer_instance1;
static HISENSE_LINKL_LAYER serialprocess_hisense_linkLayer_instance1;
static HISENSE_NETWORKL_LAYER serialprocess_hisense_networkl_layer_instance1;
static HISENSE_TRANSMISSIONL_LAYER serialprocess_hisense_transmissionl_layer_instance1;
static HISENSE_APPLICATIONL_LAYER serialprocess_hisense_applicationl_layer_instance1;
#endif /* WIFI_INSTANCE1_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE2_HISENSE_PROTOCOL_USE)
static WIFI_LAYER serialprocess_wifiLayer_instance2;
static HISENSE_LINKL_LAYER serialprocess_hisense_linkLayer_instance2;
static HISENSE_NETWORKL_LAYER serialprocess_hisense_networkl_layer_instance2;
static HISENSE_TRANSMISSIONL_LAYER serialprocess_hisense_transmissionl_layer_instance2;
static HISENSE_APPLICATIONL_LAYER serialprocess_hisense_applicationl_layer_instance2;
#endif /* WIFI_INSTANCE2_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)
static WIFI_LAYER serialprocess_wifiLayer_instance3;
static HISENSE_LINKL_LAYER serialprocess_hisense_linkLayer_instance3;
static HISENSE_NETWORKL_LAYER serialprocess_hisense_networkl_layer_instance3;
static HISENSE_TRANSMISSIONL_LAYER serialprocess_hisense_transmissionl_layer_instance3;
static HISENSE_APPLICATIONL_LAYER serialprocess_hisense_applicationl_layer_instance3;
#endif /* WIFI_INSTANCE3_HISENSE_PROTOCOL_USE */

SERIALPROCESS_CTRL serialprocess_ctrl;
uint32_t serialprocess_netbuf_error     = 0;
uint32_t serialprocess_netbuf_error_cnt = 0;

static const WIFI_LAYER *wifilayers[NUMBER_OF_WIFI_INSTANCES] =
{
    #if (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE)
    &serialprocess_wifiLayer_instance0
    #endif
    #if (WIFI_INSTANCE1_HISENSE_PROTOCOL_USE)
    ,&serialprocess_wifiLayer_instance1
    #endif
    #if (WIFI_INSTANCE2_HISENSE_PROTOCOL_USE)
    ,&serialprocess_wifiLayer_instance2
    #endif
    #if (WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)
    ,&serialprocess_wifiLayer_instance3
    #endif
};

static const uint32_t serialprocess_default_baudrates[] =
{
    #if (SERIALPORT16_IN_USE)
    #error SERIALPORT:  Maximum 16 serial ports are supported !
    #elif (SERIALPORT15_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    SERIALPORT11_DEFAULT_BAUDRATE,
    SERIALPORT12_DEFAULT_BAUDRATE,
    SERIALPORT13_DEFAULT_BAUDRATE,
    SERIALPORT14_DEFAULT_BAUDRATE,
    SERIALPORT15_DEFAULT_BAUDRATE,
    #elif (SERIALPORT14_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    SERIALPORT11_DEFAULT_BAUDRATE,
    SERIALPORT12_DEFAULT_BAUDRATE,
    SERIALPORT13_DEFAULT_BAUDRATE,
    SERIALPORT14_DEFAULT_BAUDRATE,
    #elif (SERIALPORT13_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    SERIALPORT11_DEFAULT_BAUDRATE,
    SERIALPORT12_DEFAULT_BAUDRATE,
    SERIALPORT13_DEFAULT_BAUDRATE,
    #elif (SERIALPORT12_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    SERIALPORT11_DEFAULT_BAUDRATE,
    SERIALPORT12_DEFAULT_BAUDRATE,
    #elif (SERIALPORT11_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    SERIALPORT11_DEFAULT_BAUDRATE,
    #elif (SERIALPORT10_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    SERIALPORT10_DEFAULT_BAUDRATE,
    #elif (SERIALPORT9_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    SERIALPORT9_DEFAULT_BAUDRATE,
    #elif (SERIALPORT8_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    SERIALPORT8_DEFAULT_BAUDRATE,
    #elif (SERIALPORT7_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    SERIALPORT7_DEFAULT_BAUDRATE,
    #elif (SERIALPORT6_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    SERIALPORT6_DEFAULT_BAUDRATE,
    #elif (SERIALPORT5_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    SERIALPORT5_DEFAULT_BAUDRATE,
    #elif (SERIALPORT4_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    SERIALPORT4_DEFAULT_BAUDRATE,
    #elif (SERIALPORT3_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    SERIALPORT3_DEFAULT_BAUDRATE,
    #elif (SERIALPORT2_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    SERIALPORT2_DEFAULT_BAUDRATE,
    #elif (SERIALPORT1_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    SERIALPORT1_DEFAULT_BAUDRATE,
    #elif (SERIALPORT0_IN_USE)
    SERIALPORT0_DEFAULT_BAUDRATE,
    #endif
};


/*
Private functions: 
*/

void serialprocess_netbuf_init()
{
    netbuf_init();
    timer_stop(&serialprocess_ctrl.netbuf_ctrl.timer);
    /* serialprocess_netbuf_error and serialprocess_netbuf_error_cnt are set to 0 only after Power-on-reset. Do not change it here !!! */
}

#define NB_VALID 1
/*
Checking of number of free net buffers.
If free net buffer count decreases below 25% of available netbuffers, timer starts.
If this timer elapses 30 seconds, serialprocess (and also netbuffers) resets.
If in this time free netbuffer count increases above 75% of free netbuffers amount, timer resets.
*/
void serialprocess_netbuf_monitor()
{
    static uint8_t netbuf_valid[5] = {NB_VALID, NB_VALID, NB_VALID, NB_VALID, NB_VALID};
    static uint8_t netbuf_valid_idx = 0;

    WORD free_netbufs;
    WORD netbuf_drops;

    free_netbufs = netbuf_getfreecount();
    netbuf_drops = netbuf_getdrops();
    netbuf_valid[netbuf_valid_idx] = netbuf_pool_isvalid();
    netbuf_valid_idx = (netbuf_valid_idx + 1) % sizeof(netbuf_valid);

    /* First passing below 1/4 of numbers of available number of netbuffers: */
    if (free_netbufs < (NETBUF_POOL_BUFS * 1 / 4) + 5)      /* 25% - low hysteresis limit;  +5: safety in case of minimum settings */
    {
        if (serialprocess_ctrl.netbuf_ctrl.timer.started == 0)
        {
            timer_start(&serialprocess_ctrl.netbuf_ctrl.timer);
        }
    }

    /* Number of free netbuffers is 60 minutes below 1/4 limit after first passing below 1/4 limit,
       if no netbuffers available
       if too many drops happened
       or if netbuffer pool is not valid 5 times in a row: */
    if ((timer_elapsed(&serialprocess_ctrl.netbuf_ctrl.timer) > 60*60*1000) ||
        (free_netbufs == 0) ||
        (netbuf_drops > 100) ||
        ((netbuf_valid[0] != NB_VALID) && (netbuf_valid[1] != NB_VALID) && (netbuf_valid[2] != NB_VALID) && (netbuf_valid[3] != NB_VALID) && (netbuf_valid[4] != NB_VALID)))
    {
        serialprocess_init();   /* --> netbuf_init(); */
        timer_stop(&serialprocess_ctrl.netbuf_ctrl.timer);
        serialprocess_netbuf_error = 0xDEADBEEF;            /* Indication that net buffers free count low limit was reached for 30 seconds */
        serialprocess_netbuf_error_cnt++;
        memset(&netbuf_valid, NB_VALID, sizeof(netbuf_valid));
    }

    /* If free netbuffers count returns above 3/4 limit (hysteresis 1/4 -- 3/4), timer is stopped: */
    if (free_netbufs > (NETBUF_POOL_BUFS * 3 / 4))      /* 75% - high hysteresis limit */
    {
        if (serialprocess_ctrl.netbuf_ctrl.timer.started == 1)
        {
            timer_stop(&serialprocess_ctrl.netbuf_ctrl.timer);
        }
    }
}

/*
Checking if there is no Rx frame on main UART.
If there is no Rx frame for SERIALPROCESS_CONNECTION_MONITOR_TIME milliseconds, reset all UARTs and also netbuffers.
*/
void serialprocess_connection_monitor()
{
    #if (SERIALPROCESS_CONNECTION_MONITOR_TIME > 3000) /* Time lower than 3 seconds is not reasonable. */
    static TIMER connection_monitor_timer;
    uint32_t elapsed1, elapsed2;

    if (connection_monitor_timer.started == 0)
    {
        timer_start(&connection_monitor_timer);
    }

    elapsed1 = serialprocess_rx_packet_time_get(WIFI_MODULE_PRIMARY_SERIALPORT_NUMBER);
    elapsed2 = timer_elapsed(&connection_monitor_timer);

    if ((elapsed1 > SERIALPROCESS_CONNECTION_MONITOR_TIME) &&
        (elapsed2 > SERIALPROCESS_CONNECTION_MONITOR_TIME))
    {
        serialprocess_init();
        timer_start(&connection_monitor_timer);
    }
    #endif
}


/*
API functions: 
*/

void serialprocess_init(void)
{
    serialprocess_netbuf_init();

    #if (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE)
    serialprocess_connect_layers_instance0();
    /* Initialize WiFi communication & forwarding */
    wifi_init(&serialprocess_wifiLayer_instance0);
    wifi_interrupts_enable(&serialprocess_wifiLayer_instance0);
    wifi_initDone(&serialprocess_wifiLayer_instance0);
    #endif /* WIFI_INSTANCE0_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE1_HISENSE_PROTOCOL_USE)
    serialprocess_connect_layers_instance1();
    /* Initialize WiFi communication & forwarding */
    wifi_init(&serialprocess_wifiLayer_instance1);
    wifi_interrupts_enable(&serialprocess_wifiLayer_instance1);
    wifi_initDone(&serialprocess_wifiLayer_instance1);
    #endif /* WIFI_INSTANCE1_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE2_HISENSE_PROTOCOL_USE)
    serialprocess_connect_layers_instance2();
    /* Initialize WiFi communication & forwarding */
    wifi_init(&serialprocess_wifiLayer_instance2);
    wifi_interrupts_enable(&serialprocess_wifiLayer_instance2);
    wifi_initDone(&serialprocess_wifiLayer_instance2);
    #endif /* WIFI_INSTANCE2_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)
    serialprocess_connect_layers_instance3();
    /* Initialize WiFi communication & forwarding */
    wifi_init(&serialprocess_wifiLayer_instance3);
    wifi_interrupts_enable(&serialprocess_wifiLayer_instance3);
    wifi_initDone(&serialprocess_wifiLayer_instance3);
    #endif /* WIFI_INSTANCE3_HISENSE_PROTOCOL_USE */

    /* Check several setting consistency over all instances: */
    if (serialprocess_check_layer_consistency() != 1)
    {
        /* Serial port numbers must not be the same for different WiFi layer instances !!! */
        proc_halt(0);
    }
}

uint8_t serialprocess_port_init(SERIALPORT_NUMBER serialport)
{
    #if (NUMBER_OF_WIFI_INSTANCES > 0)
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;

    if (serialport >= SERIALPORT_NUMBER_COUNT)
    {
        return 0;
    }

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return 0;
    }

    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return 0;
    }

    wifi_init(wifilayer);
    wifi_interrupts_enable(wifilayer);
    wifi_initDone(wifilayer);
    return 1;

    #else
    return 1;
    #endif /* (NUMBER_OF_WIFI_INSTANCES > 0) */
}

uint32_t serialprocess_hisense_protocol_counter_get(SERIALPORT_NUMBER serialport, SERIALPROCESS_HISENSE_COUNTER counter)
{
    #if (NUMBER_OF_WIFI_INSTANCES > 0)
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;
    uint32_t cnt;

    if (serialport >= SERIALPORT_NUMBER_COUNT)
    {
        return 0;
    }

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return 0;
    }

    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return 0;
    }

    if (counter >= SERIALPROCESS_HISENSE_COUNTER_MAX)
    {
        return 0;
    }

    cnt = 0;
    switch (counter)
    {
        case SERIALPROCESS_NETBUFFER_ERROR_COUNTER:
        {
            /* The counter is not related to port number (WiFi instance). */
            cnt = serialprocess_netbuf_error_cnt;
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_WIFI_RX:
        {
            cnt = wifi_counter_get(wifilayer, WIFI_LAYER_COUNTER_RX);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_WIFI_RX_QUEUEFULL:
        {
            cnt = wifi_counter_get(wifilayer, WIFI_LAYER_COUNTER_RX_QUEUEFULL);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_WIFI_TX:
        {
            cnt = wifi_counter_get(wifilayer, WIFI_LAYER_COUNTER_TX);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_WIFI_TX_QUEUEFULL:
        {
            cnt = wifi_counter_get(wifilayer, WIFI_LAYER_COUNTER_TX_QUEUEFULL);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_WIFI_TX_TIMEOUT:
        {
            cnt = wifi_counter_get(wifilayer, WIFI_LAYER_COUNTER_TX_TIMEOUT);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_LL_CRCOK:
        {
            cnt = hisense_LinkL_counter_get(wifilayer->linkLayer, HISENSE_LINKL_LAYER_COUNTER_CRCOK);
            break;
        }

        case SERIALPROCESS_HISENSE_COUNTER_LL_CRCFAIL:
        {
            cnt = hisense_LinkL_counter_get(wifilayer->linkLayer, HISENSE_LINKL_LAYER_COUNTER_CRCFAIL);
            break;
        }

        default:
        {
            cnt = 0;
            break;
        }
    }
    return cnt;
    #else
    return 0;
    #endif /* (NUMBER_OF_WIFI_INSTANCES > 0) */
}

uint32_t serialprocess_rx_packet_time_get(SERIALPORT_NUMBER serialport)
{
    #if (NUMBER_OF_WIFI_INSTANCES > 0)
    uint32_t wifilayer_address;
    WIFI_LAYER* wifilayer;
    uint32_t elapsed;

    if (serialport >= SERIALPORT_NUMBER_COUNT)
    {
        return 0xFFFFFFFF;
    }

    if (serialprocess_wifi_instance_get(serialport, &wifilayer_address) != 1)
    {
        return 0xFFFFFFFF;
    }

    wifilayer = (WIFI_LAYER*) wifilayer_address;
    if (wifilayer->serialPort != serialport)
    {
        return 0xFFFFFFFF;
    }

    elapsed = wifi_rx_packet_receive_time(wifilayer);

    return elapsed;
    #else
    return 0xFFFFFFFF;
    #endif /* (NUMBER_OF_WIFI_INSTANCES > 0) */

}

#if (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE)
void serialprocess_connect_layers_instance0(void)
{
    /* Setting Wifi communication part: */
    serialprocess_wifiLayer_instance0.serialPort = WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER;
    serialprocess_wifiLayer_instance0.rx_alt_pin = WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN;
    serialprocess_wifiLayer_instance0.tx_alt_pin = WIFI_INSTANCE0_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN;
    serialprocess_wifiLayer_instance0.baudrate = (SERIALPORT_BAUDRATE) serialprocess_default_baudrates[serialprocess_wifiLayer_instance0.serialPort];
    /* Linking upwards: */
    serialprocess_hisense_transmissionl_layer_instance0.applicationLayer = &serialprocess_hisense_applicationl_layer_instance0;
    serialprocess_hisense_networkl_layer_instance0.transmissionLayer = &serialprocess_hisense_transmissionl_layer_instance0;
    serialprocess_hisense_linkLayer_instance0.networkLayer = &serialprocess_hisense_networkl_layer_instance0;
    serialprocess_wifiLayer_instance0.linkLayer = &serialprocess_hisense_linkLayer_instance0;
    /* Linking downwards: */
    serialprocess_hisense_applicationl_layer_instance0.transmissionLayer = (void*) &serialprocess_hisense_transmissionl_layer_instance0;
    serialprocess_hisense_transmissionl_layer_instance0.networkLayer = (void*) &serialprocess_hisense_networkl_layer_instance0;
    serialprocess_hisense_networkl_layer_instance0.linkLayer = (void*) &serialprocess_hisense_linkLayer_instance0;
    serialprocess_hisense_linkLayer_instance0.wifiLayer = (void*) &serialprocess_wifiLayer_instance0;

    /* Setting forwarding part: */
    #if (FORWARDING_USE == 1)
    memset(&serialprocess_wifiLayer_instance0.forward, 0, sizeof(serialprocess_wifiLayer_instance0.forward));
    serialprocess_wifiLayer_instance0.forward.number_of_forwarding_ports = WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS;
    #if (WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS > 0)
    serialprocess_wifiLayer_instance0.forward.wifilayer_reference[0] = (void*) (&WIFI_INSTANCE0_FORWARDING_PORT0_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS > 1)
    serialprocess_wifiLayer_instance0.forward.wifilayer_reference[1] = (void*) (&WIFI_INSTANCE0_FORWARDING_PORT1_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS > 2)
    serialprocess_wifiLayer_instance0.forward.wifilayer_reference[2] = (void*) (&WIFI_INSTANCE0_FORWARDING_PORT2_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS > 3)
    serialprocess_wifiLayer_instance0.forward.wifilayer_reference[3] = (void*) (&WIFI_INSTANCE0_FORWARDING_PORT3_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE0_NUMBER_OF_FORWARDING_PORTS > 4)
        #error SERIALPROCESS: Maximum 4 forward ports are supported !
    #endif
    #endif /* (FORWARDING_USE == 1) */

    /* Setting structure for synchronized TX */
    #if (SYNCHRONIZED_TX_USE == 1)
    serialprocess_wifiLayer_instance0.synchronized_tx_ctrl.synchronized_tx_enabled = WIFI_INSTANCE0_SYNCHRONIZED_TX;
    timer_start(&serialprocess_wifiLayer_instance0.synchronized_tx_ctrl.last_synchronization_time);
    #endif /* (SYNCHRONIZED_TX_USE == 1) */

    /* Setting structure for RX synchronization */
    #if (RX_SYNCHRONIZATION_USE == 1)
    serialprocess_wifiLayer_instance0.rx_synchronization_ctrl.rx_synchronization_enabled = WIFI_INSTANCE0_RX_SYNCHRONIZATION;
    #endif /* (RX_SYNCHRONIZATION_USE == 1) */
}
#endif /* WIFI_INSTANCE0_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE1_HISENSE_PROTOCOL_USE)
void serialprocess_connect_layers_instance1(void)
{
    /* Setting Wifi communication part: */
    serialprocess_wifiLayer_instance1.serialPort = WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER;
    serialprocess_wifiLayer_instance1.rx_alt_pin = WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN;
    serialprocess_wifiLayer_instance1.tx_alt_pin = WIFI_INSTANCE1_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN;
    serialprocess_wifiLayer_instance1.baudrate = (SERIALPORT_BAUDRATE) serialprocess_default_baudrates[serialprocess_wifiLayer_instance1.serialPort];
    /* Linking upwards: */
    serialprocess_hisense_transmissionl_layer_instance1.applicationLayer = &serialprocess_hisense_applicationl_layer_instance1;
    serialprocess_hisense_networkl_layer_instance1.transmissionLayer = &serialprocess_hisense_transmissionl_layer_instance1;
    serialprocess_hisense_linkLayer_instance1.networkLayer = &serialprocess_hisense_networkl_layer_instance1;
    serialprocess_wifiLayer_instance1.linkLayer = &serialprocess_hisense_linkLayer_instance1;
    /* Linking downwards: */
    serialprocess_hisense_applicationl_layer_instance1.transmissionLayer = (void*) &serialprocess_hisense_transmissionl_layer_instance1;
    serialprocess_hisense_transmissionl_layer_instance1.networkLayer = (void*) &serialprocess_hisense_networkl_layer_instance1;
    serialprocess_hisense_networkl_layer_instance1.linkLayer = (void*) &serialprocess_hisense_linkLayer_instance1;
    serialprocess_hisense_linkLayer_instance1.wifiLayer = (void*) &serialprocess_wifiLayer_instance1;

    /* Setting forwarding part: */
    #if (FORWARDING_USE == 1)
    memset(&serialprocess_wifiLayer_instance1.forward, 0, sizeof(serialprocess_wifiLayer_instance1.forward));
    serialprocess_wifiLayer_instance1.forward.number_of_forwarding_ports = WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS;
    #if (WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS > 0)
    serialprocess_wifiLayer_instance1.forward.wifilayer_reference[0] = (void*) (&WIFI_INSTANCE1_FORWARDING_PORT0_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS > 1)
    serialprocess_wifiLayer_instance1.forward.wifilayer_reference[1] = (void*) (&WIFI_INSTANCE1_FORWARDING_PORT1_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS > 2)
    serialprocess_wifiLayer_instance1.forward.wifilayer_reference[2] = (void*) (&WIFI_INSTANCE1_FORWARDING_PORT2_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS > 3)
    serialprocess_wifiLayer_instance1.forward.wifilayer_reference[3] = (void*) (&WIFI_INSTANCE1_FORWARDING_PORT3_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE1_NUMBER_OF_FORWARDING_PORTS > 4)
        #error SERIALPROCESS: Maximum 4 forward ports are supported !
    #endif
    #endif /* (FORWARDING_USE == 1) */

    /* Setting structure for synchronized TX */
    #if (SYNCHRONIZED_TX_USE == 1)
    serialprocess_wifiLayer_instance1.synchronized_tx_ctrl.synchronized_tx_enabled = WIFI_INSTANCE1_SYNCHRONIZED_TX;
    timer_start(&serialprocess_wifiLayer_instance1.synchronized_tx_ctrl.last_synchronization_time);
    #endif /* (SYNCHRONIZED_TX_USE == 1) */

    /* Setting structure for RX synchronization */
    #if (RX_SYNCHRONIZATION_USE == 1)
    serialprocess_wifiLayer_instance1.rx_synchronization_ctrl.rx_synchronization_enabled = WIFI_INSTANCE1_RX_SYNCHRONIZATION;
    #endif /* (RX_SYNCHRONIZATION_USE == 1) */
}
#endif /* WIFI_INSTANCE1_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE2_HISENSE_PROTOCOL_USE)
void serialprocess_connect_layers_instance2(void)
{
    /* Setting Wifi communication part: */
    serialprocess_wifiLayer_instance2.serialPort = WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER;
    serialprocess_wifiLayer_instance2.rx_alt_pin = WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN;
    serialprocess_wifiLayer_instance2.tx_alt_pin = WIFI_INSTANCE2_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN;
    serialprocess_wifiLayer_instance2.baudrate = (SERIALPORT_BAUDRATE) serialprocess_default_baudrates[serialprocess_wifiLayer_instance2.serialPort];
    /* Linking upwards: */
    serialprocess_hisense_transmissionl_layer_instance2.applicationLayer = &serialprocess_hisense_applicationl_layer_instance2;
    serialprocess_hisense_networkl_layer_instance2.transmissionLayer = &serialprocess_hisense_transmissionl_layer_instance2;
    serialprocess_hisense_linkLayer_instance2.networkLayer = &serialprocess_hisense_networkl_layer_instance2;
    serialprocess_wifiLayer_instance2.linkLayer = &serialprocess_hisense_linkLayer_instance2;
    /* Linking downwards: */
    serialprocess_hisense_applicationl_layer_instance2.transmissionLayer = (void*) &serialprocess_hisense_transmissionl_layer_instance2;
    serialprocess_hisense_transmissionl_layer_instance2.networkLayer = (void*) &serialprocess_hisense_networkl_layer_instance2;
    serialprocess_hisense_networkl_layer_instance2.linkLayer = (void*) &serialprocess_hisense_linkLayer_instance2;
    serialprocess_hisense_linkLayer_instance2.wifiLayer = (void*) &serialprocess_wifiLayer_instance2;

    /* Setting forwarding part: */
    #if (FORWARDING_USE == 1)
    memset(&serialprocess_wifiLayer_instance2.forward, 0, sizeof(serialprocess_wifiLayer_instance2.forward));
    serialprocess_wifiLayer_instance2.forward.number_of_forwarding_ports = WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS;
    #if (WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS > 0)
    serialprocess_wifiLayer_instance2.forward.wifilayer_reference[0] = (void*) (&WIFI_INSTANCE2_FORWARDING_PORT0_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS > 1)
    serialprocess_wifiLayer_instance2.forward.wifilayer_reference[1] = (void*) (&WIFI_INSTANCE2_FORWARDING_PORT1_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS > 2)
    serialprocess_wifiLayer_instance2.forward.wifilayer_reference[2] = (void*) (&WIFI_INSTANCE2_FORWARDING_PORT2_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS > 3)
    serialprocess_wifiLayer_instance2.forward.wifilayer_reference[3] = (void*) (&WIFI_INSTANCE2_FORWARDING_PORT3_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE2_NUMBER_OF_FORWARDING_PORTS > 4)
        #error SERIALPROCESS: Maximum 4 forward ports are supported !
    #endif
    #endif /* (FORWARDING_USE == 1) */

    /* Setting structure for synchronized TX */
    #if (SYNCHRONIZED_TX_USE == 1)
    serialprocess_wifiLayer_instance2.synchronized_tx_ctrl.synchronized_tx_enabled = WIFI_INSTANCE2_SYNCHRONIZED_TX;
    timer_start(&serialprocess_wifiLayer_instance2.synchronized_tx_ctrl.last_synchronization_time);
    #endif /* (SYNCHRONIZED_TX_USE == 1) */

    /* Setting structure for RX synchronization */
    #if (RX_SYNCHRONIZATION_USE == 1)
    serialprocess_wifiLayer_instance2.rx_synchronization_ctrl.rx_synchronization_enabled = WIFI_INSTANCE2_RX_SYNCHRONIZATION;
    #endif /* (RX_SYNCHRONIZATION_USE == 1) */
}
#endif /* WIFI_INSTANCE2_HISENSE_PROTOCOL_USE */

#if (WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)
void serialprocess_connect_layers_instance3(void)
{
    /* Setting Wifi communication part: */
    serialprocess_wifiLayer_instance3.serialPort = WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER;
    serialprocess_wifiLayer_instance3.rx_alt_pin = WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER_RX_ALT_PIN;
    serialprocess_wifiLayer_instance3.tx_alt_pin = WIFI_INSTANCE3_HISENSE_SERIALPORT_NUMBER_TX_ALT_PIN;
    serialprocess_wifiLayer_instance3.baudrate = (SERIALPORT_BAUDRATE) serialprocess_default_baudrates[serialprocess_wifiLayer_instance3.serialPort];
    /* Linking upwards: */
    serialprocess_hisense_transmissionl_layer_instance3.applicationLayer = &serialprocess_hisense_applicationl_layer_instance3;
    serialprocess_hisense_networkl_layer_instance3.transmissionLayer = &serialprocess_hisense_transmissionl_layer_instance3;
    serialprocess_hisense_linkLayer_instance3.networkLayer = &serialprocess_hisense_networkl_layer_instance3;
    serialprocess_wifiLayer_instance3.linkLayer = &serialprocess_hisense_linkLayer_instance3;
    /* Linking downwards: */
    serialprocess_hisense_applicationl_layer_instance3.transmissionLayer = (void*) &serialprocess_hisense_transmissionl_layer_instance3;
    serialprocess_hisense_transmissionl_layer_instance3.networkLayer = (void*) &serialprocess_hisense_networkl_layer_instance3;
    serialprocess_hisense_networkl_layer_instance3.linkLayer = (void*) &serialprocess_hisense_linkLayer_instance3;
    serialprocess_hisense_linkLayer_instance3.wifiLayer = (void*) &serialprocess_wifiLayer_instance3;

    /* Setting forwarding part: */
    #if (FORWARDING_USE == 1)
    memset(&serialprocess_wifiLayer_instance3.forward, 0, sizeof(serialprocess_wifiLayer_instance3.forward));
    serialprocess_wifiLayer_instance3.forward.number_of_forwarding_ports = WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS;
    #if (WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS > 0)
    serialprocess_wifiLayer_instance3.forward.wifilayer_reference[0] = (void*) (&WIFI_INSTANCE3_FORWARDING_PORT0_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS > 1)
    serialprocess_wifiLayer_instance3.forward.wifilayer_reference[1] = (void*) (&WIFI_INSTANCE3_FORWARDING_PORT1_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS > 2)
    serialprocess_wifiLayer_instance3.forward.wifilayer_reference[2] = (void*) (&WIFI_INSTANCE3_FORWARDING_PORT2_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS > 3)
    serialprocess_wifiLayer_instance3.forward.wifilayer_reference[3] = (void*) (&WIFI_INSTANCE3_FORWARDING_PORT3_WIFILAYERREFERENCE);
    #endif 
    #if (WIFI_INSTANCE3_NUMBER_OF_FORWARDING_PORTS > 4)
        #error SERIALPROCESS: Maximum 4 forward ports are supported !
    #endif
    #endif /* (FORWARDING_USE == 1) */

    /* Setting TX synchrnoized structure */
    #if (SYNCHRONIZED_TX_USE == 1)
    serialprocess_wifiLayer_instance3.synchronized_tx_ctrl.synchronized_tx_enabled = WIFI_INSTANCE3_SYNCHRONIZED_TX;
    timer_start(&serialprocess_wifiLayer_instance3.synchronized_tx_ctrl.last_synchronization_time);
    #endif /* (SYNCHRONIZED_TX_USE == 1) */

    /* Setting structure for RX synchronization */
    #if (RX_SYNCHRONIZATION_USE == 1)
    serialprocess_wifiLayer_instance3.rx_synchronization_ctrl.rx_synchronization_enabled = WIFI_INSTANCE3_RX_SYNCHRONIZATION;
    #endif /* (RX_SYNCHRONIZATION_USE == 1) */
}
#endif /* WIFI_INSTANCE3_HISENSE_PROTOCOL_USE */

uint8_t serialprocess_check_layer_consistency(void)
{
    #if (NUMBER_OF_WIFI_INSTANCES > 1)
    uint8_t i, j;

    for (i = 0; i < NUMBER_OF_WIFI_INSTANCES - 1; i++)
    {
        for (j = i + 1; j < NUMBER_OF_WIFI_INSTANCES; j++)
        {
            if (wifilayers[i]->serialPort == wifilayers[j]->serialPort)
            {
                return 0;
            }
        }
    }
    #endif /* (NUMBER_OF_WIFI_INSTANCES > 1) */

    return 1;
}

uint8_t serialprocess_wifi_instance_get(SERIALPORT_NUMBER serialport, uint32_t* wifilayer_addr)
{
    #if (NUMBER_OF_WIFI_INSTANCES > 0)
    uint8_t i;

    for (i = 0; i < NUMBER_OF_WIFI_INSTANCES; i++)
    {
        if (wifilayers[i]->serialPort == serialport)
        {
            *wifilayer_addr = (uint32_t) (wifilayers[i]);
            return 1;
        }
    }
    return 0;
    #else
    return 0;
    #endif /* (NUMBER_OF_WIFI_INSTANCES > 0) */
}

void serialprocess_run(void)
{
    serialprocess_netbuf_monitor();
    serialprocess_connection_monitor();

    #if (WIFI_INSTANCE0_HISENSE_PROTOCOL_USE)
    wifi_run(&serialprocess_wifiLayer_instance0);
    #endif /* WIFI_INSTANCE0_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE1_HISENSE_PROTOCOL_USE)
    wifi_run(&serialprocess_wifiLayer_instance1);
    #endif /* WIFI_INSTANCE1_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE2_HISENSE_PROTOCOL_USE)
    bootloader_run((void*)&serialprocess_wifiLayer_instance2);
    wifi_run(&serialprocess_wifiLayer_instance2);
    #endif /* WIFI_INSTANCE2_HISENSE_PROTOCOL_USE */

    #if (WIFI_INSTANCE3_HISENSE_PROTOCOL_USE)
    bootloader_run((void*)&serialprocess_wifiLayer_instance3);
    wifi_run(&serialprocess_wifiLayer_instance3);
    #endif /* WIFI_INSTANCE3_HISENSE_PROTOCOL_USE */
}

/* [] END OF FILE */
