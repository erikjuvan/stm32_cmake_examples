////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci.c
*@brief     osci module
*@author    Zdenko Mezgec
*@date      10.12.2020
*@version   V1.2.0
*
*@mainpage
*@section   Description
*
* Osci module is used to monitor required objects, parameters, signals and store their value inside reserved RAM memory in float_32 format
*
* Next functions shall be used to use osci properly
* - osci_init (basic module init fnc that sets everything to default and registers all neccessary callbacks with canopen
* - osci_hndl (this shall be called within irq or task and this is where actual snapshot of desired variables is stored). Usually this is called in FOC or somewhere with high sampling rate
*
* Osci is basically exposed to external world through several functions, which are neccessary to properly configure and manage osci.
*
* All these are automatically connected to canopen objects, but can work without it by calling directly these functions
* These functions are
* - osci_hndl_command               (osci_commands_t are used to Stop, Run, Continue, Config_To_Default, Save_Current_Config)
*
* - osci_hndl_sampling_rate         (determine sampling rate to read memory)
*
* - osci_hndl_trigger_mode          (different trigger possibilities Continuous, On_Match, Below, Above, Rising, Falling
* - osci_hndl_trigger_level         (trigger level to use when trigger mode not in continous mode); this is switch from pretrigger to posttriggger samples record;
* - osci_hndl_trigger_source        (user can choose to have trigger from actual monitor channels if required; use num of channel)
* - osci_hndl_trigger_sourceindex   (user can choose trigger channel with desired index, independently from monitored signals)
* - osci_hndl_trigger_sourcesubIndex(user can choose trigger channel with desired subindex, independently from monitored signals)
*
* - osci_hndl_pretrigger_smpl_req   (determine number of req pretrigger samples)*
*
* - osci_hndl_channel_count         (declare number of channels to monitor)
* - osci_hndl_selected_channel      (part of configuratin procedure to setup desired monitored signal num)
* - osci_hndl_channel_index         (setup index of desired signal)
* - osci_hndl_channel_subindex      (setup subindex of desired signal)
*
* - osci_hndl_get_domain_data       (callback from canopen to read data directly; can be exposed to external modules to check data directly)
* - osci_hndl_record_number         (this is left for future to set index in memory; currently it is used only to reset all pointer to 0 for new data reading);
*
*   \todo EEPROM support to store osci configuration
*   \todo FLASH  support to store osci data in FLASH target
*   \todo add support to use osci through anything else than canopen (when needed). Add functions for osci manipulation (for canopen this is done automatically)
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
#include "../osci_cfg.h"
#include "osci.h"

#if OSCI_CANOPEN_INTERFACE
    #include "Middlewares/Protocol/CANopen/CANopen/canopen.h"
#endif

#include "osci_internal.h"


////////////////////////////////////////////////////////////////////////////////
// Preprocessor checks
////////////////////////////////////////////////////////////////////////////////
#if defined(OSCI_FREERTOS) && defined(OSCI_KEIL_RTX)
		#error "OSCI module can use only a single RTOS."
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#if OSCI_SIMULATE_SIGNALS
    volatile uint32_t gul_simulate_long_data_counter = 0;   //used only for simulation
    volatile uint8_t  guc_simulate_char_data_counter = 0;   //used only for simulation
#endif

osci_st g_str_osci;

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
#if OSCI_SIMULATE_SIGNALS
    static void osci_simulate_trigger_signals(void);        //used only for simulation
#endif

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   this functions is called at init or when major changes appear ; going back to default and update of all params shall be performed
*   basically it is just an update of parameters used to expose current status to external world
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_updateallmainvars(void)
{
    Oscilloscope_settings__Status                           = g_str_osci.status;
    Oscilloscope_settings__Memory                           = g_str_osci.memory_target;
    Oscilloscope_settings__Configuration_number             = g_str_osci.uc_configuration_number;
    Oscilloscope_settings__Number_of_configurations         = 0;
    Oscilloscope_settings__Trigger_mode                     = g_str_osci.trigger_mode;
    Oscilloscope_settings__Trigger_source                   = g_str_osci.uc_trigger_source;
    Oscilloscope_settings__Trigger_level                    = g_str_osci.real_trigger_level;
    Oscilloscope_settings__Trigger_source_Subindex          = g_str_osci.trigger_channel.uc_subindex;
    Oscilloscope_settings__Trigger_source_Index             = g_str_osci.trigger_channel.us_index;
    Oscilloscope_settings__Sampling_rate                    = g_str_osci.ul_sampling_rate;
    Oscilloscope_settings__Pretrigger_samples_requested     = g_str_osci.ul_pretrigger_samples_requested;
    Oscilloscope_settings__Posttrigger_samples_recorded     = g_str_osci.ul_posttrigger_samples_recorded;
    Oscilloscope_settings__Pretrigger_samples_recorded      = g_str_osci.memory_capacity*4;
    Oscilloscope_channels__Channel_count                    = g_str_osci.channel_count;
    Oscilloscope_settings__Memory_capacity                  = g_str_osci.memory_capacity*4;
}

////////////////////////////////////////////////////////////////////////////////
/**
*   this function is used to properly setup complete osci to default. used at boot up and on command default
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_struct_to_default(void)
{
    uint8_t uc_count;

    g_str_osci.status                           = Status_Not_Configured;
    g_str_osci.uc_configuration_number          = 0;
    g_str_osci.ul_pretrigger_samples_requested  = 0;
    g_str_osci.ul_pretrigger_samples_recorded   = 0;
    g_str_osci.ul_posttrigger_samples_recorded  = 0;
    g_str_osci.ul_triggered_position            = 0xffffffffU;
    g_str_osci.memory_target                    = Memory_Target_RAM;
    g_str_osci.memory_flash_support             = Memory_Flash_Not_Supported;
    g_str_osci.memory_capacity                  = OSCI_MEMORY_SIZE;
    g_str_osci.channel_count                    = 0;
    g_str_osci.channel_config_status            = C_Status_Not_Configured;

    for(uc_count=0;uc_count<OSCI_MAX_CHANNEL_COUNT;uc_count++)
    {
        g_str_osci.channels[uc_count].us_index      = 0;
        g_str_osci.channels[uc_count].uc_subindex   = 0xff;
        g_str_osci.channels[uc_count].object_type   = Object_Type_U8;
    }

    g_str_osci.trigger_mode                 = Trigger_Mode_Continuous;
    g_str_osci.real_trigger_level           = 0;
    g_str_osci.uc_trigger_source            = 0;
    g_str_osci.trigger_channel.us_index     = 0;
    g_str_osci.trigger_channel.uc_subindex  = 0xff;
    g_str_osci.trigger_channel.object_type  = Object_Type_U8;

    g_str_osci.ul_sampling_rate= 1;

    #if OSCI_CANOPEN_INTERFACE
        co_setup_domain_size (&Oscilloscope_settings__Record, 0UL);
    #endif
}


#if OSCI_SIMULATE_SIGNALS
////////////////////////////////////////////////////////////////////////////////
/**
*   trigger simulated signals if required; step each one inside predefined range by 1
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_simulate_trigger_signals(void)
{
    //simulate signals
    if (gul_simulate_long_data_counter==0)
    {
        Oscilloscope_test_signals__Test_signal_3= 0;
        Oscilloscope_test_signals__Test_signal_4= -100000;
    }
    Oscilloscope_test_signals__Test_signal_3+= 1;
    Oscilloscope_test_signals__Test_signal_4+= 1;
    if (gul_simulate_long_data_counter++ >= 200000)
    {
        gul_simulate_long_data_counter=0;
    }

    if (guc_simulate_char_data_counter == 0)
    {
        Oscilloscope_test_signals__Test_signal_1= 0;
        Oscilloscope_test_signals__Test_signal_2= -100;
    }

    Oscilloscope_test_signals__Test_signal_1+= 1;
    Oscilloscope_test_signals__Test_signal_2+= 1;
    if (guc_simulate_char_data_counter++ >= 200)
    {
        guc_simulate_char_data_counter=0;
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup OSCI_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   osci init that setup everything for osci
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_init(void)
{
    osci_struct_to_default();

    osci_updateallmainvars();

    osci_callbacks_register();
}

////////////////////////////////////////////////////////////////////////////////
/**
*   osci main tick that shall be called from task or IRQ
*   at the beginning major commands are check to void unneccessary calls;
*   additional there is a check of sampling rate to make a snapshot of memory at exact time
*   and finally actual call to osci which hndl data extraction, memory storage, triggers and stop when reaching end
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_hndl(void)
{
    // initial checks
    // check if recording and waiting, otherwise exit
    if ((g_str_osci.status != Status_Recording)
      &&(g_str_osci.status != Status_Waiting))
    {
        return;
    }

    //check if sample time not reached
    g_str_osci.ul_osci_sampling_rate_counter++;
    if (g_str_osci.ul_osci_sampling_rate_counter < g_str_osci.ul_sampling_rate)
    {
        return;
    }
    // reinit counter
    g_str_osci.ul_osci_sampling_rate_counter=0;

#if OSCI_SIMULATE_SIGNALS
        osci_simulate_trigger_signals();
#endif

    osci_ram_write();
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
	




