////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      canopen.c
*@brief     canopen module
*@author    Zdenko Mezgec
*@date      14.12.2020
*@version   V1.3.0
*
*@mainpage
*@section   Description
*
* The CANopen Stack is a software library
* that provides all communication services
* of the "CANopen Application Layer and Communication Profile"
*
* Use instruction 1.0: To properly use canopen, eds file must be properly linked inside project
*   open Project properties/C/C++ Build/Settings/Tasking linker
*   and add into Command line pattern next below line
*   --import-object="..\middleware\protocol\CANopen\CO_Dictionary\gen_objdict.zip"
*
* Use instruction 1.1: Upper SW layers shall call "co_init" function which is needed for proper initialization
*   This starts all services and canopen task as well. For proper startup user must properly configure
*   Node ID of channel        - CANOPEN_CAN_NODE_INDEX
*   CAN ID for RX filter      - CANOPEN_CAN_ID_MASTER
*   CAN ID Mask for RX filter - CANOPEN_CAN_ID_MASK
*   RTOS task priority        - CO_MAIN_TASK_PRIORITY
*   sdo write register size   - CANOPEN_REGISTER_SDOWRITE_CALLBACK_SIZE
*
* Use instruction 1.2: User can register fnc to be called upon sdo or pdo write for particular object (index, subindex) with "co_register_sdowrite_callback"
*   New Data is retrieved in callback parameter (co_register_sdowrite_callback(0x100b,0,example_callback_fnc))
*   example callback for retrieve:
*   void example_callback_fnc(const void *pdata)
*   {
*       const UNSIGNED8 * pu8_value_temp = pdata;
*
*       if (*pu8_value_temp == 3)
*   }
*
* Use instruction 2.1: User can use green and red leds according to standard. In this case add led drive inside "co_ledgreenind" and "co_ledredind"
*
* Use instruction 2.2: User can use "co_sdoservercheckwriteind" to check if desired write value on particular object is inside desired limits or permissions.
*   To prevent writing, return desired RET_T status
*
* Use instruction 2.3: User can use "co_saveind", "co_clearind", "co_loadind" fnc ti implement object save, clear load - usually connected with NVM
*   This is used for future implementations of parameters saving
*
* Use instruction 2.4: User can use "co_register_domainreaddata_callback" to register callback function for domain read data.
* By default canopen stack reads directly data from reserved memory. In case target object is special (read from external memory, osci,...) this callback can be used to overtake memory read
*
* Use instruction 2.5: User shall change strings in "co_setup_string_object_data" as needed
*
* Use instruction 2.6: User can call fnc co_setup_domain_size ti signal actual data in domain to canopen . Used for e.g. osci
*
*   \todo Clear, Load, Store
*   \todo DS402 profile
*   \todo Access levels
*   \todo hndl cfg .mode = eCAN_NORMAL_NODE,
*   \todo hndl cfg .extended_frame = false,
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <string.h>
#include "../canopen_cfg.h"
#include "gen_define.h"
#include "co_canopen.h"
#include "canopen.h"

#ifdef __ILLD__
    #include "Cpu\Std\IfxCpu.h"
    #include "Scu\Std\IfxScuWdt.h"
#elif defined(__arm__)
		#ifdef __TARGET_CPU_CORTEX_M7_FP_SP
				#include "stm32f7xx.h"
		#else
				#if 1
					#include "stm32g4xx.h"
				#else
					#error "CANOPEN module is currently only supported for CORTEX m7. Port the module if you wish to use it elsewhere."
				#endif
		#endif
#endif

#ifdef CO_FREERTOS
    #include "FreeRTOS.h"
    #include "semphr.h"
    #include "task.h"
    #include "timers.h"
    #include "stdbool.h"
#elif defined(CO_KEIL_RTX)
    #include "cmsis_os.h"
#endif



////////////////////////////////////////////////////////////////////////////////
// Preprocessor checks
////////////////////////////////////////////////////////////////////////////////
#if defined(CO_FREERTOS) && defined(CO_KEIL_RTX)
		#error "CANopen module can use only a single RTOS."
#endif

#if !defined(CO_FREERTOS) && !defined(CO_KEIL_RTX)
//	#error "CANopen module must use an RTOS, pick either FREERTOS or KEIL RTX."
#endif



////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_FREERTOS
    static void tCO_MAIN_TASK (void *argument);
    static void codrvTimerISR( TimerHandle_t xTimer );
    TaskHandle_t t_CO_MAIN_TASK;
		static void codrvTimerISR(TimerHandle_t xTimer);
#elif defined(CO_KEIL_RTX)
    void tCO_MAIN_TASK (void const *argument);                 // function prototype for Thread_1
	osThreadDef (tCO_MAIN_TASK, osPriorityNormal, 1, 0);       // define Thread_
	osThreadId t_CO_MAIN_TASK;
	static void codrvTimerISR(const void *arg);

    #define CO_MAILBOX_NUM_ITEMS 64
    #define CO_MAILBOX_DATA_REC  1
#endif

typedef struct {
    UNSIGNED16  index;
    UNSIGNED8   subIndex;
    co_domain_read_data_callback_fnc co_domain_read_data_callback;
}domain_read_callback_t;

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#ifdef __CTC__
    __far extern UNSIGNED32 _lc_ub_gen_objdict_zip; // linker labels
    __far extern UNSIGNED32 _lc_ue_gen_objdict_zip;
#elif defined(__arm__)
		extern unsigned int CanOPEN_zipedEDS_File$$Base;
		extern unsigned int CanOPEN_zipedEDS_File$$End;
#endif

#ifdef CO_FREERTOS
	SemaphoreHandle_t co_odaccess_mutex;
#elif defined(CO_KEIL_RTX)
  osMutexDef(co_odaccess_mutex);   
	osMutexId(co_odaccess_mutex_id); 
	osTimerDef(co_timer, codrvTimerISR);   // when the timer expires, the function toggle_power is called
	osTimerId co_timer_id;
	osMessageQId  	CO_MainTask_event;	
	osMessageQDef  (CO_MainTask_event, CO_MAILBOX_NUM_ITEMS, uint8_t);     
#endif

static BOOL_T initialized = CO_FALSE;

domain_read_callback_t domain_read_callback[CANOPEN_DOMAIN_READ_CALLBACK_TABLE_SIZE];
volatile uint8_t g_domain_read_callback_size = 0;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void     pdoInd_Sync_and_Async(UNSIGNED16);
static void     hbState(UNSIGNED8   nodeId, CO_ERRCTRL_T state,CO_NMT_STATE_T   nmtState);
static void     pdoRecEvent(UNSIGNED16);
static void     canInd(CO_CAN_STATE_T);
static void     commInd(CO_COMM_STATE_EVENT_T);

static RET_T    sdoServerReadInd(BOOL_T execute, UNSIGNED8 sdoNr, UNSIGNED16 index,UNSIGNED8   subIndex);
static RET_T    sdoDomainWriteInd(UNSIGNED16  index,UNSIGNED8   subIndex,UNSIGNED32  domainBufSize,UNSIGNED32  transferedSize);
static RET_T    sdoServerCheckWriteIndInt(BOOL_T execute, UNSIGNED8 sdoNr, UNSIGNED16 index, UNSIGNED8 subIndex, const UNSIGNED8 *pData);
static RET_T    nmtInd(BOOL_T  execute, CO_NMT_STATE_T newState);
static RET_T    coMaster_Init (void);
static RET_T    coEventRegister (void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   Inicialization of registering events
*
* @return       RET_T   status
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T coEventRegister (void)
{
    if (coEventRegister_NMT(nmtInd) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }
    if (coEventRegister_ERRCTRL(hbState) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }
    if (coEventRegister_SDO_SERVER_READ(sdoServerReadInd) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }
    if (coEventRegister_SDO_SERVER_CHECK_WRITE(sdoServerCheckWriteIndInt) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_PDO(pdoInd_Sync_and_Async) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_PDO_SYNC(pdoInd_Sync_and_Async) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_PDO_REC_EVENT(pdoRecEvent) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_LED_GREEN(co_ledgreenind) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_LED_RED(co_ledredind) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_CAN_STATE(canInd) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_COMM_EVENT(commInd) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_SDO_SERVER_DOMAIN_WRITE(sdoDomainWriteInd) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_SAVE_PARA(co_saveind) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    if (coEventRegister_CLEAR_PARA(co_clearind) != RET_OK)
    {
        return RET_INTERNAL_ERROR;
    }

    return RET_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   init master cobid
*
* @return       RET_T
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T coMaster_Init (void)
{
    UNSIGNED8   sdo;
    RET_T retVal;

    sdo=1;
    retVal = coOdSetCobid(0x1280 + sdo - 1, 1, 0x600 + CO_u8_Node_ID_E);
    if (retVal != RET_OK)  {
        return(retVal);
    }
    retVal = coOdSetCobid(0x1280 + sdo - 1, 2, 0x580 + CO_u8_Node_ID_E);
    if (retVal != RET_OK)  {
        return(retVal);
    }

    return RET_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   pdo write indications callback event
*
* @param[in]    pdoNr   pdo nr
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void pdoInd_Sync_and_Async(UNSIGNED16    pdoNr)
{
    UNSIGNED8   mapCnt;
    UNSIGNED32  mapVal;
    UNSIGNED8   i;
    UNSIGNED16  index;
    UNSIGNED8   subIndex;

    /* get all mapped objects */
    coOdGetObj_u8(0x1600 + pdoNr - 1, 0, &mapCnt);
    for (i = 0; i < mapCnt; i++)  {
        coOdGetObj_u32(0x1600 + pdoNr - 1, i + 1, &mapVal);

        index = mapVal >> 16;
        subIndex = (mapVal >> 8) & 0xff;

        canopen_sdowrite_call_callback(index,subIndex);
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   reset device fnc
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void co_reset_device (void)
{
#ifdef __ILLD__
    UNSIGNED16 endinitSfty_pw;

    endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();
    IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
    IfxCpu_triggerSwReset();
#elif defined(__arm__)
    NVIC_SystemReset(); 
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
*   nmt state indication callback event
*
* @param[in]    execute     pre or post write
* @param[in]    newState    new state
* @return       RET_T
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T nmtInd(BOOL_T  execute,CO_NMT_STATE_T  newState)
{
    if (execute == CO_TRUE)
    {
        if ((newState == CO_NMT_STATE_PREOP) || (newState == CO_NMT_STATE_STOPPED))
        {
            return RET_OK;
        }
        else if (newState == CO_NMT_STATE_OPERATIONAL)
        {
            return RET_OK;
        }
        else if (newState == CO_NMT_STATE_RESET_NODE)
        {
            co_reset_device();
        }
    }

    return(RET_OK);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   sdo domain write indication callback event; data is captured from dedicated memory for each object separatelly
*
* @param[in]    index           index of object
* @param[in]    subIndex        subindex of object
* @param[in]    domainBufSize   size of received data
* @param[in]    transferedSize  domain transfered size
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T sdoDomainWriteInd(UNSIGNED16  index,UNSIGNED8   subIndex,UNSIGNED32  domainBufSize,UNSIGNED32  transferedSize)
{
    return(RET_OK);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   sdo write indication callback event; user shall use fnc co_register_sdowrite_callback
*
* @param[in]    execute     state of write (pre or post)
* @param[in]    sdoNr       sdo number
* @param[in]    index       index
* @param[in]    subIndex    subindex
* @param[in]    pData       data
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T sdoServerCheckWriteIndInt(BOOL_T execute, UNSIGNED8 sdoNr, UNSIGNED16 index, UNSIGNED8 subIndex, const UNSIGNED8 *pData)
{
    if (execute == CO_FALSE)
    {
        return co_sdoservercheckwriteind(index,subIndex,pData);
    }
    return(RET_OK);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   sdo read indication callback event
*
* @param[in]    execute     state of read (pre or post)
* @param[in]    sdoNr       sdo number
* @param[in]    index       index
* @param[in]    subIndex    subindex
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T sdoServerReadInd(BOOL_T execute,UNSIGNED8  sdoNr,UNSIGNED16    index,UNSIGNED8 subIndex)
{
    return(RET_OK);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   pdo receive indication callback event
*
* @param[in]    pdoNr       nr of received pdo
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void pdoRecEvent(UNSIGNED16  pdoNr)
{

}

////////////////////////////////////////////////////////////////////////////////
/**
*   error control indication callback event
*
* @param[in]    nodeId      node id
* @param[in]    state       error
* @param[in]    nmtState    nmt state
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void hbState(UNSIGNED8   nodeId,CO_ERRCTRL_T state,CO_NMT_STATE_T    nmtState)
{

}

////////////////////////////////////////////////////////////////////////////////
/**
*   can state indication callback event
*
* @param[in]    canState    can state event
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void canInd(CO_CAN_STATE_T canState)
{
    switch (canState)
    {
        case CO_CAN_STATE_BUS_OFF:

            break;
        case CO_CAN_STATE_BUS_ON:

            break;
        case CO_CAN_STATE_PASSIVE:

            break;
        case CO_CAN_STATE_UNCHANGED:
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   communication state indication callback event
*
* @param[in]    commEvent   comm state event
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void commInd(CO_COMM_STATE_EVENT_T   commEvent)
{
    switch (commEvent)  {
        case CO_COMM_STATE_EVENT_BUS_OFF:

            break;
        case CO_COMM_STATE_EVENT_BUS_OFF_RECOVERY:

            break;
        case CO_COMM_STATE_EVENT_BUS_ON:

            break;
        case CO_COMM_STATE_EVENT_PASSIVE:

            break;
        case CO_COMM_STATE_EVENT_ACTIVE:

            break;
        case CO_COMM_STATE_EVENT_CAN_OVERRUN:

            break;
        case CO_COMM_STATE_EVENT_REC_QUEUE_FULL:

            break;
        case CO_COMM_STATE_EVENT_REC_QUEUE_OVERFLOW:

            break;
        case CO_COMM_STATE_EVENT_TR_QUEUE_FULL:

            break;
        case CO_COMM_STATE_EVENT_TR_QUEUE_OVERFLOW:

            break;
        case CO_COMM_STATE_EVENT_TR_QUEUE_EMPTY:

            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   setup eds size and pointer for proper domain read
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void co_setup_eds_pointer_and_size (void)
{
    UNSIGNED8 * pc_edsaddress;
    UNSIGNED8  u8_counter;
    UNSIGNED32 u32_eds_length;

#ifdef __CTC__
    pc_edsaddress = (UNSIGNED8*)&_lc_ub_gen_objdict_zip;
		u32_eds_length = (UNSIGNED32)&_lc_ue_gen_objdict_zip - (UNSIGNED32)&_lc_ub_gen_objdict_zip;      // lengt in bytes 
#elif defined(__arm__)
		pc_edsaddress = (UNSIGNED8*)&CanOPEN_zipedEDS_File$$Base;
		//u32_eds_length = (UNSIGNED32)&CanOPEN_zipedEDS_File$$Length;
		// We get start and end
		u32_eds_length = (UNSIGNED32)&CanOPEN_zipedEDS_File$$End - (UNSIGNED32)&CanOPEN_zipedEDS_File$$Base;
#endif

    //setup pointer
    CO_domain_Store_EDS=(CO_DOMAIN_PTR)pc_edsaddress;

    // setup length
    for(u8_counter=0;u8_counter<co_od_domain_table_len();u8_counter++)
    {
        if (od_domain[u8_counter] == &CO_domain_Store_EDS)
        {
            od_domain_len[u8_counter]=u32_eds_length;
            return;
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
/**
*   callback fnc for reading data through domain transfer
*
*   by default canopen stack reads directly data from reserved memory. In case target object is special (read from external memory, osci,...) p_regular_read_segment shall be set to 0 and overtake read data
*
* @param[out]   pData       return data through pointer
* @param[in]    size        number of desired data to read
* @param[in]    index       read from desired object index
* @param[in]    subIndex    read from desired object subindex
* @param[out]   p_regular_read_segment  return info if user will overtake read before canopne stack. In this case set *p_regular_read_segment to 0
* @return       RET_T       status
*/
////////////////////////////////////////////////////////////////////////////////
RET_T co_odgetobjdomain(void *pData,UNSIGNED32 size,UNSIGNED16 index,UNSIGNED8 subIndex,UNSIGNED8 * p_regular_read_segment)
{
    uint8_t i;

    *p_regular_read_segment = 1;

    //search for other module callbacks
    //check table size
    if (g_domain_read_callback_size != 0)
    {
        //search callback
        for(i=0;i<g_domain_read_callback_size;i++)
        {
            //check index
            if (domain_read_callback[i].index == index)
            {
                //check subindex
                if (domain_read_callback[i].subIndex == subIndex)
                {
                    //callback found
                    *p_regular_read_segment = 0;
                    return domain_read_callback[i].co_domain_read_data_callback(index, subIndex, pData, size);
                }
            }
        }
    }

    return(RET_OK);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   setup domain size
*
* @param[in]    p_co_domain        domain ptr
* @param[in]    domain_data_size   domain size
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void co_setup_domain_size (CO_DOMAIN_PTR * p_co_domain, uint32_t domain_data_size)
{
    UNSIGNED8  u8_counter;

    for(u8_counter=0;u8_counter<co_od_domain_table_len();u8_counter++)
    {
        if (od_domain[u8_counter] == p_co_domain)
        {
            od_domain_len[u8_counter]=domain_data_size;

            return;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   setup eds size and pointer for proper domain read
*
* @return       RET_T
*/
////////////////////////////////////////////////////////////////////////////////
RET_T co_setup_vis_string (char * vis_string_domain_pointer, char * new_string)
{
    UNSIGNED8  u8_counter;

    // search string
    for(u8_counter=0;u8_counter<co_vis_string_table_len();u8_counter++)
    {
        if ((UNSIGNED8 *)od_vis_string[u8_counter] == (UNSIGNED8 *)vis_string_domain_pointer)
        {
            //check for max size
            if (od_vis_string_len[u8_counter] < strlen(new_string))
            {
                return RET_STRING_SET_TOO_LONG;
            }

            od_vis_string_len[u8_counter]=strlen(new_string)+1;
            strcpy((char *)vis_string_domain_pointer,new_string);

            return RET_OK;
        }
    }

    return RET_STRING_ADDRESS_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   init Timer to provide timing interval for canopen
*
* @param[in]    timerInterval   period in us
* @return       RET_T
*/
////////////////////////////////////////////////////////////////////////////////
RET_T codrvTimerSetup(UNSIGNED32 timerInterval)
{
#ifdef CO_FREERTOS
    TimerHandle_t timer_id;

	timer_id = xTimerCreate("CO Timer", timerInterval/1000, true, NULL, codrvTimerISR);
    xTimerStart(timer_id, 0);
#elif defined(CO_KEIL_RTX)
    co_timer_id = osTimerCreate(osTimer(co_timer), osTimerPeriodic, NULL);
		osTimerStart(co_timer_id, timerInterval/1000);
#endif

    return RET_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   periodic timer that is called from timer interrupt or any other system timer, os
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_FREERTOS
static void codrvTimerISR( TimerHandle_t xTimer )
#elif defined(CO_KEIL_RTX)
static void codrvTimerISR(const void *arg)
#else
static void __attribute__((__used__)) codrvTimerISR(const void *arg)
#endif
{
    coTimerTick();
}

////////////////////////////////////////////////////////////////////////////////
/**
*   canopen task
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
#ifdef CO_FREERTOS
void tCO_MAIN_TASK (void *argument)
{

    const TickType_t    xMaxBlockTime = pdMS_TO_TICKS( 5 );
    BaseType_t          xResult;
    UNSIGNED32          ulNotifiedValue;

    for (;;)
    {
        // Wait to be notified of an interrupt
        xResult = xTaskNotifyWait( pdFALSE,  /* Don't clear bits on entry. */
                           0xffffffff,        /* Clear all bits on exit. */
                           &ulNotifiedValue, /* Stores the notified value. */
                           xMaxBlockTime );

        if( xResult == pdPASS )
        {
            // A notification was received.
            coCommTask();
        }
        else
        {
            // Did not receive a notification within the expected time.
            coCommTask();
        }
    }
}
#elif defined(CO_KEIL_RTX)
void tCO_MAIN_TASK (void const *argument)
{
	for(;;)
	{
		// WARNING: This calls co_CommTask everytime a message is received. But coCommTask should
		// also be called periodically every 5 ms.
		osMessageGet(CO_MainTask_event, 0);
		coCommTask();
	}
}
#endif



////////////////////////////////////////////////////////////////////////////////
/**
*   fnc to notify canopen task from CAN receive to start with parsing immediatelly
*
* @return   void
*/
////////////////////////////////////////////////////////////////////////////////
void coStartCOMMTaskImmediatelly_FromISR (void)
{
#ifdef CO_FREERTOS
    BaseType_t xHigherPriorityTaskWoken;

    // Clear the interrupt source.
    //prvClearInterrupt();

    xTaskNotifyFromISR( t_CO_MAIN_TASK,
                       0x1,
                       eSetBits,
                       &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
#elif defined(CO_KEIL_RTX)
	osMessagePut(CO_MainTask_event, (uint32_t)CO_MAILBOX_DATA_REC, 0);  
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
*   fnc to lock shared resources in od access according to stack instructions
*
* @return   void
*/
////////////////////////////////////////////////////////////////////////////////
void co_odaccess_os_lock(void)
{
#ifdef CO_FREERTOS
    xSemaphoreTake(co_odaccess_mutex, portMAX_DELAY);
#elif defined(CO_KEIL_RTX)
    osMutexWait(co_odaccess_mutex_id, osWaitForever);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
*   fnc to unlock shared resources in od access according to stack instructions
*
* @return   void
*/
////////////////////////////////////////////////////////////////////////////////
void co_odaccess_os_unlock(void)
{
#ifdef CO_FREERTOS
    xSemaphoreGive(co_odaccess_mutex);
#elif defined(CO_KEIL_RTX)
    osMutexRelease(co_odaccess_mutex_id);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   canopen init function
*
* @return       canopen_init_t  init status
*/
////////////////////////////////////////////////////////////////////////////////
canopen_init_t co_init (void)
{
#ifdef CO_FREERTOS
    BaseType_t xreturned;
#endif

    if (initialized == CO_FALSE)
    {
        CO_u8_Node_ID_E = 1;

    #ifdef CO_FREERTOS
        co_odaccess_mutex = xSemaphoreCreateMutex();
        if( co_odaccess_mutex == NULL )
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_MUTEX_ERR;
        }
    #elif defined (CO_KEIL_RTX)
        co_odaccess_mutex_id = osMutexCreate(osMutex(co_odaccess_mutex));
				if( co_odaccess_mutex_id == NULL)
				{
					 CO_ASSERT(0);
					 return eCANOPEN_INIT_MUTEX_ERR;
				}
    #endif

        canopen_sdowrite_init_callback();

        co_setup_eds_pointer_and_size();

        co_setup_string_object_data();

        if (codrvTimerSetup(CO_TIMER_INTERVAL) != RET_OK)
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_TIMER_ERR;
        }

        if (coCanOpenStackInit(co_loadind) != RET_OK)
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_STACK_ERR;
        }

        if (coEventRegister() != RET_OK)
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_EVENT_ERR;
        }

        if (codrvCanInit(CO_u16_CAN_Bitrate_CAN_Bitrate_E) != RET_OK)
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_CAN_ERR;
        }

        if (coMaster_Init() != RET_OK)
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_MASTER_ERR;
        }

    #ifdef CO_FREERTOS
        xreturned = xTaskCreate(tCO_MAIN_TASK, "CO Main Task", CANOPEN_MAIN_TASK_STACK, NULL, CANOPEN_MAIN_TASK_PRIORITY,  &t_CO_MAIN_TASK);
        if( xreturned != pdPASS )
        {
            CO_ASSERT(0);
            return eCANOPEN_INIT_TASK_ERR;
        }
    #elif defined(CO_KEIL_RTX)
        t_CO_MAIN_TASK = osThreadCreate(osThread(tCO_MAIN_TASK), NULL);
				if (t_CO_MAIN_TASK == NULL)
				{
						CO_ASSERT(0);
						return eCANOPEN_INIT_TASK_ERR;
				}
    #endif

        initialized = CO_TRUE;
    }

    return eCANOPEN_INIT_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   register callback fnc desired object write (sdo or pdo external change)
*
* @param[in]    index           index of desired object
* @param[in]    subIndex        subindex of desired object
* @param[in]    co_callback     callback fnc
* @return       canopen_sdo_reg_t   register callback success
*/
////////////////////////////////////////////////////////////////////////////////
canopen_sdo_reg_t co_register_sdowrite_callback(UNSIGNED16  index,UNSIGNED8   subIndex,co_callback_fnc co_callback)
{
    return icanopen_sdowrite_register_callback(index,subIndex,co_callback);
}

////////////////////////////////////////////////////////////////////////////////
/**
*   register callback fnc for domain data read
*
* @param[in]    index           index of desired object
* @param[in]    subindex        subindex of desired object
* @param[in]    co_domain_read_data_callback     callback fnc
* @return       canopen_domain_read_data_reg_t   register callback success
*/
////////////////////////////////////////////////////////////////////////////////
canopen_domain_read_data_reg_t   co_register_domainreaddata_callback(UNSIGNED16  index, UNSIGNED8   subindex, co_domain_read_data_callback_fnc co_domain_read_data_callback)
{
    if (g_domain_read_callback_size >= CANOPEN_DOMAIN_READ_CALLBACK_TABLE_SIZE)
    {
        return eCANOPEN_DOMAIN_READ_REG_ERR;
    }

    domain_read_callback[g_domain_read_callback_size].index = index;
    domain_read_callback[g_domain_read_callback_size].subIndex = subindex;
    domain_read_callback[g_domain_read_callback_size].co_domain_read_data_callback = co_domain_read_data_callback;

    //add, at last
    g_domain_read_callback_size++;

    return eCANOPEN_DOMAIN_READ_REG_OK;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   Function that shall be called periodically when there is no OS
*
* @param[in] - void
*/
////////////////////////////////////////////////////////////////////////////////
void co_hndl(void)
{
    static uint32_t last_loop_time = 0;
    const uint32_t current_time = HAL_GetTick();

    if( (current_time - last_loop_time) >= 10 )
    {
        coTimerTick();
    }

    coCommTask();
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

	




