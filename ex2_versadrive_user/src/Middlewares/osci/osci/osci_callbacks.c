////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci_callbacks.c
*@brief     osci callback hndl
*@author    Zdenko Mezgec
*@date      10.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup OSCI
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "osci.h"
#include "../osci_cfg.h"
#include "osci_internal.h"

#if OSCI_CANOPEN_INTERFACE
    #include "Middlewares/Protocol/CANopen/CANopen/canopen.h"
    #include "Middlewares/Protocol/CANopen/CANopen/co_odaccess.h"
#endif

#if (OSCI_ENABLE_MOTOR_CONTROL_CALLBACK == 1)
    #include "Middlewares/motor_control/motor_control/motor_control.h"
#endif

#ifdef OSCI_FREERTOS
    #include "FreeRTOS.h"
    #include "task.h"
#elif defined(OSCI_KEIL_RTX)
    #include "cmsis_os.h"
#else
    #include "stm32g4xx_hal.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
volatile uint8_t g_Oscilloscope_Record_Data[]={0x00,0x00,0x00,0x00};

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_command               (const void *pdata);
static void osci_hndl_record_number         (const void *pdata);
static void osci_hndl_trigger_mode          (const void *pdata);
static void osci_hndl_trigger_level         (const void *pdata);
static void osci_Hndl_trigger_source        (const void *pdata);
static void osci_hndl_trigger_sourceindex   (const void *pdata);
static void osci_hndl_trigger_sourceindex   (const void *pdata);
static void osci_hndl_trigger_sourcesubIndex(const void *pdata);
static void osci_hndl_channel_count         (const void *pdata);
static void osci_hndl_selected_channel      (const void *pdata);
static void osci_hndl_channel_index         (const void *pdata);
static void osci_hndl_channel_subindex      (const void *pdata);
static void osci_hndl_sampling_rate         (const void *pdata);
static void osci_hndl_pretrigger_smpl_req   (const void *pdata);

static void osci_erasechannnelsconfiguration(void);
static void osci_config_to_default          (void);
static osci_complete_channel_config_status_t    osci_checkcompletechannelconfiguration (void);
static osci_gather_channel_info_t               osci_gatherchannelinformation          (void);

#if OSCI_CANOPEN_INTERFACE
    static RET_T osci_get_domain_data(uint16_t index, uint8_t subIndex, void *pData, uint32_t  size);
#else
    #error
#endif

static void osci_command_save_current_config (void);
static void osci_command_config_to_default   (void);
static void osci_command_stop                (void);
static void osci_command_run_continue        (uint8_t uc_command);


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   register callbacks for main write objects and domain data read
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_callbacks_register(void)
{
#if OSCI_CANOPEN_INTERFACE
    // link actual array to pointer
    Oscilloscope_settings__Record = (CO_DOMAIN_PTR)&g_Oscilloscope_Record_Data;

    // register callback to canopen for canope domain read
    co_register_domainreaddata_callback ((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS,(uint8_t)OSCI_SUBINDEX_RECORD,osci_get_domain_data);

    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_COMMAND,                   osci_hndl_command);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_RECORD_NUMBER,             osci_hndl_record_number);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_TRIGGER_MODE,              osci_hndl_trigger_mode);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_TRIGGER_LEVEL,             osci_hndl_trigger_level);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_TRIGGER_SOURCE,            osci_Hndl_trigger_source);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_TRIGGER_SOURCEINDEX,       osci_hndl_trigger_sourceindex);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_TRIGGER_SOURCESUBINDEX,    osci_hndl_trigger_sourcesubIndex);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_SAMPLING_RATE,             osci_hndl_sampling_rate);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_SETTINGS, (uint8_t)OSCI_SUBINDEX_PRETRIGGER_SAMP_REQUESTED, osci_hndl_pretrigger_smpl_req);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_COUNT,             osci_hndl_channel_count);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_SELECTED_CHANNEL,          osci_hndl_selected_channel);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_INDEX,             osci_hndl_channel_index);
    co_register_sdowrite_callback((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_SUBINDEX,          osci_hndl_channel_subindex);
#endif

#if (OSCI_ENABLE_MOTOR_CONTROL_CALLBACK == 1)
    // Register callback, which is then called from motor control FOC loop
    motor_control_register_callback(&osci_hndl);
#endif
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callbacks for command (osci_commands_t are used to Stop, Run, Continue, Save_Current_Config, Config_To_Default)
*
* @param[in]    pdata      command
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_command (const void *pdata)
{
    uint8_t * pu8_value= (uint8_t *)(pdata);
    uint8_t uc_command = *pu8_value;

    //save current param
    if (uc_command == Command_Save_Current_Config)
    {
        osci_command_save_current_config();
        return;
    }

    //current param to default
    if (uc_command == Command_Config_To_Default)
    {
        osci_command_config_to_default();
        return;
    }

    //stop command
    if (uc_command == Command_Stop)
    {
        osci_command_stop();
        return;
    }

    //run command
    if ((uc_command == Command_Run) || (uc_command == Command_Continue))
    {
        osci_command_run_continue(uc_command);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for record number - (this is left for future to set index in memory; currently it is used only to reset all pointer to 0 for new data reading)
*
* @param[in]    pdata      unused
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_record_number (const void *pdata)
{
    int32_t l_temp_pos;

    g_str_osci.ul_record_num        = 0;
    g_str_osci.ul_data_byte_counter = 0;

    //decrement last position to get correct sample
    l_temp_pos = (int32_t)g_str_osci.ul_osci_data_start - (int32_t)g_str_osci.channel_count;
    if (l_temp_pos < 0)
    {
        l_temp_pos = (int32_t)g_str_osci.ul_memory_end_location_b - ((int32_t)g_str_osci.channel_count * 4);
    }
    else
    {
        l_temp_pos = l_temp_pos * 4;
    }

    g_str_osci.ul_start_data_offset_b = (uint32_t)l_temp_pos;


    g_str_osci.ul_memory_end_location_b = g_str_osci.ul_max_records     * 4 * g_str_osci.channel_count;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for sampling rate
*
* @param[in]    pdata      sampling rate
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_sampling_rate (const void *pdata)
{
    uint32_t * pu32_value        = (uint32_t *)(pdata);
    uint32_t   u32_sampling_rate = *pu32_value;

    g_str_osci.ul_sampling_rate = u32_sampling_rate;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for pretriger sampling req
*
* @param[in]    pdata      num of req samples
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_pretrigger_smpl_req (const void *pdata)
{
    uint32_t * pu32_value        = (uint32_t *)(pdata);
    uint32_t   u32_pre_smpl_req  = *pu32_value;

    g_str_osci.ul_pretrigger_samples_requested = u32_pre_smpl_req;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for trigger mode; different trigger possibilities Continuous, On_Match, Below, Above, Rising, Falling
*
* @param[in]    pdata      trigger mode
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_trigger_mode (const void *pdata)
{
    uint8_t * pu8_value     = (uint8_t *)(pdata);
    g_str_osci.trigger_mode = (osci_trigger_mode_t)*pu8_value;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for trigger level; trigger level to use when trigger mode not in continous mode; this is switch from pretrigger to posttriggger samples record
*
* @param[in]    pdata      trigger level
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_trigger_level (const void *pdata)
{
    float32_t * preal_value       = (float32_t *)(pdata);
    g_str_osci.real_trigger_level = *preal_value;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for trigger source; user can choose to have trigger from actual monitor channels if required; use number of channel
*
* @param[in]    pdata      trigger source
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_Hndl_trigger_source (const void *pdata)
{
    uint8_t * pu8_value          = (uint8_t *)(pdata);
    g_str_osci.uc_trigger_source = *pu8_value;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for trigger source index; user can choose trigger channel with desired index, independently from monitored signals
*
* @param[in]    pdata      trigger source index
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_trigger_sourceindex (const void *pdata)
{
    uint16_t * pu16_value               = (uint16_t *)(pdata);
    g_str_osci.trigger_channel.us_index = *pu16_value;

    //user uses new logic
    g_str_osci.uc_trigger_source=0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for trigger source subindex; user can choose trigger channel with desired subindex, independently from monitored signals
*
* @param[in]    pdata      trigger source subindex
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_trigger_sourcesubIndex (const void *pdata)
{
    uint8_t * pu8_value                    = (uint8_t *)(pdata);
    g_str_osci.trigger_channel.uc_subindex = *pu8_value;

    //user uses new logic
    g_str_osci.uc_trigger_source=0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for channel count; declare number of channels to monitor
*
* @param[in]    pdata      channel count
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_channel_count (const void *pdata)
{
    uint8_t * pu8_value      = (uint8_t *)(pdata);
    uint8_t uc_channel_count = *pu8_value;

    //check if osci running; stop it in this case
        // check if recording or waiting
        if ((g_str_osci.status == Status_Recording)
          ||(g_str_osci.status == Status_Waiting))
        {
            //OSCI: Changing channel configuration while osci running. Stop OSCI
        	g_str_osci.status = Status_Stopped;
            Oscilloscope_settings__Status = g_str_osci.status;

            //make small delay to stop all writings on interrupt level
            #ifdef OSCI_FREERTOS
                vTaskDelay(2);
            #elif defined(OSCI_KEIL_RTX)
                osDelay(2);
            #else
                HAL_Delay(2);
            #endif
        }

    if (uc_channel_count == 0)
    {
        //OSCI Channel: Count 0 --> Erase channel config
        osci_erasechannnelsconfiguration();
        osci_checkcompletechannelconfiguration();
        return;
    }


    //OSCI Channel: Count changed --> Store and show old settings
    g_str_osci.channel_count = uc_channel_count;
    // whatever change of channel count reads stored settings for selected channel 1 and show to user
    #if OSCI_CANOPEN_INTERFACE
        coOdPutObj_u8((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_SELECTED_CHANNEL, (uint8_t)1);
        coOdPutObj_u16((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_INDEX,   g_str_osci.channels[0].us_index);
        coOdPutObj_u8((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_SUBINDEX,g_str_osci.channels[0].uc_subindex);
    #endif

    osci_checkcompletechannelconfiguration();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for selected channel; part of configuratin procedure to setup desired monitored signal num
*
* @param[in]    pdata      selected channel
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_selected_channel (const void *pdata)
{
    uint8_t * pu8_value         = (uint8_t *)(pdata);
    uint8_t uc_selected_channel = *pu8_value;

    //check if count is different then 0
    if (g_str_osci.channel_count)
    {
        // selected channel 0 is not acceptable
        if (uc_selected_channel == 0)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        if (uc_selected_channel > OSCI_MAX_CHANNEL_COUNT)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        //check stored vars to show to user
        #if OSCI_CANOPEN_INTERFACE
            coOdPutObj_u16((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_INDEX,   g_str_osci.channels[uc_selected_channel-1].us_index);
            coOdPutObj_u8((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_CHANNEL_SUBINDEX,g_str_osci.channels[uc_selected_channel-1].uc_subindex);
        #else
            #error
        #endif
    }

    osci_checkcompletechannelconfiguration();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for channel index; setup index of desired signal
*
* @param[in]    pdata      channel index
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_channel_index (const void *pdata)
{
    uint16_t * pu16_value    = (uint16_t *)(pdata);
    uint16_t us_channel_index= *pu16_value;
    uint8_t  uc_selected_channel;

    //check if count is different then 0
    if (g_str_osci.channel_count)
    {
        #if OSCI_CANOPEN_INTERFACE
            coOdGetObj_u8((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_SELECTED_CHANNEL,&uc_selected_channel);
        #else
            #error
        #endif

        // selected channel 0 is not acceptable
        if (uc_selected_channel == 0)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        if (uc_selected_channel > OSCI_MAX_CHANNEL_COUNT)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        //store user change inside struct
        g_str_osci.channels[uc_selected_channel-1].us_index = us_channel_index;
    }

    osci_checkcompletechannelconfiguration();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        callback for channel subindex; setup subindex of desired signal
*
* @param[in]    pdata      channel subindex
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_hndl_channel_subindex (const void *pdata)
{
    uint8_t * pu8_value         = (uint8_t *)(pdata);
    uint8_t uc_channel_subindex = *pu8_value;
    uint8_t uc_selected_channel;

    //check if count is different then 0
    if (g_str_osci.channel_count)
    {
        #if OSCI_CANOPEN_INTERFACE
            coOdGetObj_u8((uint16_t)OSCI_INDEX_OSCILOSCOPE_CHANNELS, (uint8_t)OSCI_SUBINDEX_SELECTED_CHANNEL,&uc_selected_channel);
        #endif

        // selected channel 0 is not acceptable
        if (uc_selected_channel == 0)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        if (uc_selected_channel > OSCI_MAX_CHANNEL_COUNT)
        {
            //OSCI Channel: Selected channel more then max. Ingore any entry
            // dont check configuration cause this is valid state
            return;
        }

        //store user change inside struct
        g_str_osci.channels[uc_selected_channel-1].uc_subindex = uc_channel_subindex;
    }

    osci_checkcompletechannelconfiguration();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        osci to default; wait a little bit to stop properly if; void problem with accessing shared memory
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_config_to_default(void)
{
    g_str_osci.status = Status_Config_To_Default;

    //make small delay to stop all writings on interrupt level - prevent erase condition on shared memory
#ifdef OSCI_FREERTOS
    vTaskDelay(2);
#elif defined(OSCI_KEIL_RTX)
    osDelay(2);
#else
    HAL_Delay(2);
#endif

    osci_struct_to_default();
    osci_updateallmainvars();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        erase channel configuration; setup to default
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_erasechannnelsconfiguration(void)
{
    uint8_t uc_count;

    g_str_osci.status=Status_Not_Configured;
    g_str_osci.channel_config_status = C_Status_Not_Configured;
    g_str_osci.channel_count=0;

    for(uc_count=0;uc_count<OSCI_MAX_CHANNEL_COUNT;uc_count++)
    {
        g_str_osci.channels[uc_count].us_index    = 0;
        g_str_osci.channels[uc_count].uc_subindex = 0xff;
        g_str_osci.channels[uc_count].object_type = Object_Type_U8;
    }

    Oscilloscope_settings__Status                       = Status_Not_Configured;
    Oscilloscope_channels__Channel_configuration_status = C_Status_Not_Configured;
    Oscilloscope_channels__Selected_channel  = 1;
    Oscilloscope_channels__Channel_count     = 0;
    Oscilloscope_channels__Channel_index     = 0;
    Oscilloscope_channels__Channel_subindex  = 0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        check complete configuration; check if all channels properly setup.
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static osci_complete_channel_config_status_t osci_checkcompletechannelconfiguration (void)
{
    uint8_t i;

    if (g_str_osci.channel_count == 0)
    {
    	g_str_osci.channel_config_status = C_Status_Config_Error_Channel_Num_0;
        Oscilloscope_channels__Channel_configuration_status = g_str_osci.channel_config_status;
        g_str_osci.status   = Status_Not_Configured;
        Oscilloscope_settings__Status = g_str_osci.status;
        return Status_complete_Channel_Config_ERR_1;
    }

    const uint8_t count = g_str_osci.channel_count;
    for(i=0;i<count;i++)
    {
     if (g_str_osci.channels[i].us_index == 0)
     {
    	 g_str_osci.channel_config_status = C_Status_Confir_Error_Index_Subindex;
         Oscilloscope_channels__Channel_configuration_status = g_str_osci.channel_config_status;
         g_str_osci.status   = Status_Not_Configured;
         Oscilloscope_settings__Status = g_str_osci.status;
         return Status_complete_Channel_Config_ERR_2;
     }

     if (g_str_osci.channels[i].uc_subindex == 0xff)
     {
    	 g_str_osci.channel_config_status = C_Status_Confir_Error_Index_Subindex;
         Oscilloscope_channels__Channel_configuration_status = g_str_osci.channel_config_status;
         g_str_osci.status   = Status_Not_Configured;
         Oscilloscope_settings__Status = g_str_osci.status;
         return Status_complete_Channel_Config_ERR_3;
     }
    }

    g_str_osci.channel_config_status = C_Status_Configured_OK;
    Oscilloscope_channels__Channel_configuration_status = g_str_osci.channel_config_status;
    g_str_osci.status   = Status_Stopped;
    Oscilloscope_settings__Status = g_str_osci.status;

    return Status_complete_Channel_Config_OK;
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        gather channel configuration information regarding used data types
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static osci_gather_channel_info_t osci_gatherchannelinformation (void)
{
#if OSCI_CANOPEN_INTERFACE
    uint8_t i;
    CO_CONST CO_OBJECT_DESC_T *pDesc;

    const uint8_t count = g_str_osci.channel_count;
    for(i=0;i<count;i++)
    {
        g_str_osci.channels[i].pu8_obj_address=coOdGetObjAddr(g_str_osci.channels[i].us_index, g_str_osci.channels[i].uc_subindex);

        // get object type
            if (coOdGetObjDescPtr(g_str_osci.channels[i].us_index, g_str_osci.channels[i].uc_subindex, &pDesc) == RET_OK)
            {
                //check type
                switch(pDesc->dType)
                {
                    case CO_DTYPE_U32_VAR:
                    case CO_DTYPE_U32_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_U32;
                         break;

                    case CO_DTYPE_U16_VAR:
                    case CO_DTYPE_U16_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_U16;
                         break;

                    case CO_DTYPE_U8_VAR:
                    case CO_DTYPE_U8_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_U8;
                         break;

                    case CO_DTYPE_I32_VAR:
                    case CO_DTYPE_I32_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_I32;
                         break;

                    case CO_DTYPE_I16_VAR:
                    case CO_DTYPE_I16_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_I16;
                         break;

                    case CO_DTYPE_I8_VAR:
                    case CO_DTYPE_I8_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_I8;
                         break;

                    case CO_DTYPE_R32_VAR:
                    case CO_DTYPE_R32_PTR:
                        g_str_osci.channels[i].object_type = Object_Type_Real;
                        //ERROR: Not supporting Real variables:g_str_osci.channels[i].us_index, g_str_osci.channels[i].uc_subindex
                     break;

                    default:
                        //BUG!!! WEIRD TYPE:0x%x:g_str_osci.channels[i].us_index, g_str_osci.channels[i].uc_subindex,pDesc->dType
                        return Status_gather_channel_info_ERR;
                    //break;
                }
            }
            else
            {
                return Status_gather_channel_info_ERR;
            }
    }

    //gather information for trigger source if new logic used
    if (!g_str_osci.uc_trigger_source)
    {
        g_str_osci.trigger_channel.pu8_obj_address=coOdGetObjAddr(g_str_osci.trigger_channel.us_index, g_str_osci.trigger_channel.uc_subindex);
        // get object type
        if (coOdGetObjDescPtr(g_str_osci.trigger_channel.us_index, g_str_osci.trigger_channel.uc_subindex, &pDesc) == RET_OK)
        {
            //check type
            switch(pDesc->dType)
            {
                case CO_DTYPE_U32_VAR:
                case CO_DTYPE_U32_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_U32;
                     break;

                case CO_DTYPE_U16_VAR:
                case CO_DTYPE_U16_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_U16;
                     break;

                case CO_DTYPE_U8_VAR:
                case CO_DTYPE_U8_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_U8;
                     break;

                case CO_DTYPE_I32_VAR:
                case CO_DTYPE_I32_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_I32;
                     break;

                case CO_DTYPE_I16_VAR:
                case CO_DTYPE_I16_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_I16;
                     break;

                case CO_DTYPE_I8_VAR:
                case CO_DTYPE_I8_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_I8;
                     break;

                case CO_DTYPE_R32_VAR:
                case CO_DTYPE_R32_PTR:
                    g_str_osci.trigger_channel.object_type = Object_Type_Real;
                    //eERROR: Not supporting Real variables:g_str_osci.trigger_channel.us_index, g_str_osci.trigger_channel.uc_subindex
                     break;

                default:
                    //BUG!!! WEIRD TYPE:g_str_osci.trigger_channel.us_index, g_str_osci.trigger_channel.uc_subindex,pDesc->dType
                    return Status_gather_channel_info_ERR;
                //break;
            }
        }
        else
        {
            return Status_gather_channel_info_ERR;
        }
    }
#else
    #error
#endif

    return Status_gather_channel_info_OK;
}

#if OSCI_CANOPEN_INTERFACE
////////////////////////////////////////////////////////////////////////////////
/**
*   return data to domain from memory (fifo logic)
*
* @param[in]    index    index of desired object
* @param[in]    subIndex subindex of desired object
* @param[in]    pData    data return
* @param[in]    size     required size
* @return       RET_T    return canopen satus
*/
////////////////////////////////////////////////////////////////////////////////
static RET_T osci_get_domain_data(uint16_t index, uint8_t subIndex, void *pData, uint32_t  size)
{
    uint8_t i;
    int32_t i_current_index =0;
    uint8_t *pc_data        =(uint8_t *)pData;
    uint8_t *pc_source_data =(uint8_t *)g_str_osci.ul_main_ram_data_space;

    const uint32_t temp_size = size;
    for(i=0;i<temp_size;i++)
    {
        g_str_osci.ul_data_byte_counter++;

        pc_data[i_current_index]=pc_source_data[g_str_osci.ul_start_data_offset_b];
        g_str_osci.ul_start_data_offset_b++;

        i_current_index++;

        if (g_str_osci.ul_start_data_offset_b >= g_str_osci.ul_memory_end_location_b)
        {
            g_str_osci.ul_start_data_offset_b=0;
        }
    }
    return RET_OK;
}
#else
    #error
#endif

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        execute command save current config
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_command_save_current_config (void)
{
    // left for future
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        execute command config to default
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_command_config_to_default (void)
{
    osci_config_to_default();
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        execute command stop
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_command_stop (void)
{
    //OSCI Command stop accepted
    //OSCI Status Stopped
	g_str_osci.status = Status_Stopped;
    Oscilloscope_settings__Status = g_str_osci.status;

    #if OSCI_CANOPEN_INTERFACE
        co_setup_domain_size (&Oscilloscope_settings__Record, (g_str_osci.ul_posttrigger_samples_recorded + g_str_osci.ul_pretrigger_samples_recorded) * 4 * g_str_osci.channel_count);
    #else
        #error
    #endif
}

////////////////////////////////////////////////////////////////////////////////
/*!
* @brief        execute command run continue
*
* @param[in]    uc_command      command
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_command_run_continue (uint8_t uc_command)
{
    //check current status; only if stopped or finished it can be runned
    // there are some errors that can be overrun
    if ((g_str_osci.status == Status_Error_Pretrigger_Samples)
      ||(g_str_osci.status == Status_FLASH_Write_Error))
    {
        g_str_osci.status = Status_Stopped;
    }
    else
    {
        if ((g_str_osci.status != Status_Stopped)
          &&(g_str_osci.status != Status_Finished))
        {
            return;
        }
    }

    // additional checks; check flash support and desired memory target
    if ((g_str_osci.memory_flash_support == Memory_Flash_Not_Supported)
        &&(Oscilloscope_settings__Memory == Memory_Target_FLASH))
    {
        //OSCI Command run ignored; FLASH MISSING
    	g_str_osci.status = Status_Error_Flash_Missing;
        Oscilloscope_settings__Status = g_str_osci.status;

        return;
    }

    //check configuration
    if (g_str_osci.channel_config_status != C_Status_Configured_OK)
    {
        //OSCI Command run ignored; Channels not configured
        return;
    }

    if (osci_checkcompletechannelconfiguration() != Status_complete_Channel_Config_OK)
    {
        //OSCI Command run ignored; Channel configuration check failed
        return;
    }

    if (osci_gatherchannelinformation() != Status_gather_channel_info_OK)
    {
        //OSCI Command run ignored; Channel information gathering failed
        return;
    }

    // if osci finished and user wants to continue, then this is same as command run
    if ((uc_command == Command_Run) || (g_str_osci.status == Status_Finished))
    {
        //update all important variables just before run
        g_str_osci.memory_target                    = (osci_memory_target_t)Oscilloscope_settings__Memory;
        g_str_osci.ul_sampling_rate                 = Oscilloscope_settings__Sampling_rate;
        g_str_osci.ul_pretrigger_samples_requested  = Oscilloscope_settings__Pretrigger_samples_requested;

        if ((g_str_osci.ul_pretrigger_samples_requested * g_str_osci.channel_count) >= g_str_osci.memory_capacity)
        {
            //OSCI Command run ignored; Pretrigger samples more then memor capacity
        	g_str_osci.status = Status_Error_Pretrigger_Samples;
            Oscilloscope_settings__Status = g_str_osci.status;
            return;
        }

        g_str_osci.ul_pretrigger_samples_recorded  = 0;
        Oscilloscope_settings__Pretrigger_samples_recorded  = g_str_osci.ul_pretrigger_samples_recorded;
        g_str_osci.ul_posttrigger_samples_recorded = 0;
        Oscilloscope_settings__Posttrigger_samples_recorded = g_str_osci.ul_posttrigger_samples_recorded;

        g_str_osci.ul_osci_sampling_rate_counter    = 0;
        g_str_osci.ul_osci_data_start               = 0;
        g_str_osci.ul_osci_data_stop                = 0;
        g_str_osci.ul_triggered_position            = 0xffffffffU;
        g_str_osci.uc_osci_write_complete_flag      = 0;

        g_str_osci.preal_osci_data_address = (float32_t *)&g_str_osci.ul_main_ram_data_space[0];
        g_str_osci.ul_osci_data_size       = OSCI_MEMORY_SIZE;
        g_str_osci.ul_max_records          = g_str_osci.ul_osci_data_size / g_str_osci.channel_count;
        g_str_osci.ul_last_valid_address   = 0;

        //OSCI Command run accepted
        //OSCI Status Waiting
        g_str_osci.status = Status_Waiting;
        Oscilloscope_settings__Status = g_str_osci.status;
    }
    else
    {
        //OSCI Command continue accepted
        //OSCI Status Waiting
        if (g_str_osci.ul_triggered_position == 0xffffffffU)
        {
        	g_str_osci.status = Status_Waiting;
            Oscilloscope_settings__Status = g_str_osci.status;
        }
        else
        {
        	g_str_osci.status=Status_Recording;
            Oscilloscope_settings__Status = g_str_osci.status;
        }
    }
}



////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////


	




