////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci_internal.h
*@brief     osci interconnection
*@author    Zdenko Mezgec
*@date      10.12.2020
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup OSCI
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef OSCI_INTERNAL_H
#define OSCI_INTERNAL_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "../osci_cfg.h"
#include "osci.h"
#include "project_config.h"

#if OSCI_CANOPEN_INTERFACE
    #include "Middlewares/Protocol/CANopen/CANopen/canopen.h"
    #include "Middlewares/Protocol/CANopen/CANopen/co_odaccess.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

#define FLOAT_PRECISION_EQUAL_HYST          0.000001f

typedef enum
{
    Status_Stopped,
    Status_Waiting,
    Status_Recording,
    Status_Finished,
    Status_Not_Configured,
    Status_Error_Flash_Missing,
    Status_Error_FLASH_Not_Erased,
    Status_Error_Pretrigger_Samples,
    Status_Saving_Current_Config,
    Status_Config_To_Default,
    Status_FLASH_Write_Error,
    Status_Sampling_Too_High_Error,
}osci_status_t;

typedef enum
{
    Status_complete_Channel_Config_OK,
    Status_complete_Channel_Config_ERR_1,
    Status_complete_Channel_Config_ERR_2,
    Status_complete_Channel_Config_ERR_3,
}osci_complete_channel_config_status_t;

typedef enum
{
    Status_gather_channel_info_OK,
    Status_gather_channel_info_ERR,
}osci_gather_channel_info_t;

typedef enum
{
    Command_Stop,
    Command_Run,
    Command_Unused_1,
    Command_Unused_2,
    Command_Save_Current_Config,
    Command_Config_To_Default,
    Command_Continue
}osci_commands_t;


typedef enum
{
    Memory_Flash_Supported,
    Memory_Flash_Not_Supported
}osci_memory_flash_support_t;

typedef enum
{
    Memory_Target_RAM,
    Memory_Target_FLASH
}osci_memory_target_t;

typedef enum
{
    Trigger_Mode_Continuous,
    Trigger_Mode_On_Match,
    Trigger_Mode_Below,
    Trigger_Mode_Above,
    Trigger_Mode_Rising,
    Trigger_Mode_Falling,
}osci_trigger_mode_t;

typedef enum
{
    Object_Type_U8,
    Object_Type_U16,
    Object_Type_U32,
    Object_Type_I8,
    Object_Type_I16,
    Object_Type_I32,
    Object_Type_Real
}osci_object_type_t;

typedef struct
{
    uint16_t            us_index;
    uint8_t             uc_subindex;
    osci_object_type_t  object_type;
    uint8_t*            pu8_obj_address;
}osci_channel_st;

typedef enum
{
    C_Status_Not_Configured,
    C_Status_Configured_OK,
    C_Status_Config_Error_Channel_Num_0,
    C_Status_Confir_Error_Index_Subindex
}osci_channel_config_status_t;

#if OSCI_CANOPEN_INTERFACE
    #define OSCI_INDEX_OSCILOSCOPE_SETTINGS                 0x2080
        #define OSCI_SUBINDEX_COMMAND                       1
        #define OSCI_SUBINDEX_MEMORY                        2
        #define OSCI_SUBINDEX_SAMPLING_RATE                 3
        #define OSCI_SUBINDEX_WRAP_AROUND                   4
        #define OSCI_SUBINDEX_TRIGGER_LEVEL                 5
        #define OSCI_SUBINDEX_TRIGGER_MODE                  6
        #define OSCI_SUBINDEX_TRIGGER_SOURCE                7
        #define OSCI_SUBINDEX_PRETRIGGER_SAMP_REQUESTED     8
        #define OSCI_SUBINDEX_PRETRIGGER_SAMP_RECORDED      9
        #define OSCI_SUBINDEX_POSTTRIGGER_SAMP_RECORDED     10
        #define OSCI_SUBINDEX_STATUS                        11
        #define OSCI_SUBINDEX_RECORD_NUMBER                 12
        #define OSCI_SUBINDEX_RECORD                        13
        #define OSCI_SUBINDEX_MEMORY_CAPACITY_SAMPLES       14
        #define OSCI_SUBINDEX_TRIGGER_SOURCEINDEX           15
        #define OSCI_SUBINDEX_TRIGGER_SOURCESUBINDEX        16
        #define OSCI_SUBINDEX_CONFIGURATION_NUMBER          17
        #define OSCI_SUBINDEX_RECORD_CHANNEL                18
        #define OSCI_SUBINDEX_RECORD_STEP                   19
        #define OSCI_SUBINDEX_NUMBER_OF_CONFIGURATIONS      20
    #define OSCI_INDEX_OSCILOSCOPE_CHANNELS                 0x2081
        #define OSCI_SUBINDEX_CHANNEL_COUNT                 1
        #define OSCI_SUBINDEX_SELECTED_CHANNEL              2
        #define OSCI_SUBINDEX_CHANNEL_INDEX                 3
        #define OSCI_SUBINDEX_CHANNEL_SUBINDEX              4
        #define OSCI_SUBINDEX_CHANNEL_CONFIGURATION_STATUS  5
    #define OSCI_INDEX_OSCILOSCOPE_TEST                     0x2083
        #define OSCI_SUBINDEX_TEST_SIG_1                    1
        #define OSCI_SUBINDEX_TEST_SIG_2                    2
        #define OSCI_SUBINDEX_TEST_SIG_3                    3
        #define OSCI_SUBINDEX_TEST_SIG_4                    4
#endif

typedef struct
{
    osci_commands_t             command;
    osci_status_t               status;
    uint32_t                    memory_capacity;
    osci_memory_flash_support_t memory_flash_support;
    osci_memory_target_t        memory_target;
    uint8_t                     uc_osci_write_complete_flag;
    osci_trigger_mode_t         trigger_mode;
    uint8_t                     uc_trigger_source;
    float32_t                   real_trigger_level;
    osci_channel_st             trigger_channel;
    uint32_t                    ul_triggered_position;
    uint32_t                    ul_sampling_rate;
    uint32_t                    ul_pretrigger_samples_requested;
    uint32_t                    ul_pretrigger_samples_recorded;
    uint32_t                    ul_posttrigger_samples_recorded;
    uint32_t                    ul_record_number;
    uint32_t                    ul_last_valid_address;
    uint32_t                    ul_osci_data_start;
    uint32_t                    ul_osci_data_stop;
    uint8_t                     uc_configuration_number;
    uint32_t                    ul_osci_sampling_rate_counter;
    float32_t                   *preal_osci_data_address;
    uint32_t                    ul_main_ram_data_space[OSCI_MEMORY_SIZE];
    uint32_t                    ul_osci_data_size;
    uint32_t                    ul_max_records;
    uint32_t                    ul_record_num;
    uint32_t                    ul_data_byte_counter;
    uint32_t                    ul_start_data_offset_b;
    uint32_t                    ul_memory_end_location_b;
    uint8_t                         channel_count;
    osci_channel_config_status_t    channel_config_status;
    osci_channel_st                 channels[OSCI_MAX_CHANNEL_COUNT];
}osci_st;

extern osci_st g_str_osci;

extern volatile uint8_t   Oscilloscope_settings__Command;
extern volatile uint8_t   Oscilloscope_settings__Memory;
extern volatile uint32_t  Oscilloscope_settings__Sampling_rate;
extern volatile uint8_t   Oscilloscope_settings__Reserved_1;
extern volatile float32_t Oscilloscope_settings__Trigger_level;
extern volatile uint8_t   Oscilloscope_settings__Trigger_mode;
extern volatile uint8_t   Oscilloscope_settings__Trigger_source;
extern volatile uint32_t  Oscilloscope_settings__Pretrigger_samples_requested;
extern volatile uint32_t  Oscilloscope_settings__Pretrigger_samples_recorded;
extern volatile uint32_t  Oscilloscope_settings__Posttrigger_samples_recorded;
extern volatile uint8_t   Oscilloscope_settings__Status;
extern volatile uint32_t  Oscilloscope_settings__Record_number;
#if OSCI_CANOPEN_INTERFACE
    extern CO_DOMAIN_PTR Oscilloscope_settings__Record;
#endif
extern volatile uint32_t Oscilloscope_settings__Memory_capacity;
extern volatile uint16_t Oscilloscope_settings__Trigger_source_Index;
extern volatile uint8_t  Oscilloscope_settings__Trigger_source_Subindex;
extern volatile uint8_t  Oscilloscope_settings__Configuration_number;
extern volatile uint8_t  Oscilloscope_settings__Record_channel;
extern volatile int16_t  Oscilloscope_settings__Record_step;
extern volatile uint8_t  Oscilloscope_settings__Number_of_configurations;
extern volatile uint8_t  Oscilloscope_channels__Channel_count;
extern volatile uint8_t  Oscilloscope_channels__Selected_channel;
extern volatile uint16_t Oscilloscope_channels__Channel_index;
extern volatile uint8_t  Oscilloscope_channels__Channel_subindex;
extern volatile uint8_t  Oscilloscope_channels__Channel_configuration_status;
extern volatile uint8_t  Oscilloscope_test_signals__Test_signal_1;
extern volatile int8_t   Oscilloscope_test_signals__Test_signal_2;
extern volatile uint32_t Oscilloscope_test_signals__Test_signal_3;
extern volatile int32_t  Oscilloscope_test_signals__Test_signal_4;

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void osci_callbacks_register(void);
void osci_struct_to_default (void);
void osci_updateallmainvars (void);
void osci_ram_write         (void);

#endif /* OSCI_INTERNAL_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
