////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci_ram.c
*@brief     osci ram hndl
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
#else
    #error
#endif

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function prototypes
////////////////////////////////////////////////////////////////////////////////
static void osci_decrease_record(void);

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/**
*   this is the function that perform everything needed to make a snapshot of memory
*
*   1. first loop is used to extract each data from memory and to convert to float32_t depending its type
*   In this loop there is a also read of trigger variable data in same manner, which can be directly from one of the channels or from independent address
*   In this loop data is stored in actual memory
*
*   2. second stage is to hndl trigger, depending the user configuration
*   In case trigger is set, we remember position and switch to potrigger samples and recording status
*
*   3. From this point on it is constant calculation how much samples can we store, since we must not overide required pretrigger samples
*   - in not continupus mode, the osci stops when reaching desired pretrigger samples
*   - only status and number of posttrigger and pretrrigger samples parameter is called directly
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
void osci_ram_write (void)
{
    uint8_t   i;

    //pointers for proper data extraction/conversion
    uint8_t   * pu8_value;
    uint16_t  * pu16_value;
    uint32_t  * pu32_value;
    int8_t    * pi8_value;
    int16_t   * pi16_value;
    int32_t   * pi32_value;
    float32_t * preal_value;

    //trigger related vars
    uint8_t             uc_trigger_value_found=0;
    float32_t           real_value;
    float32_t           real_trigger_value = 0;
    static float32_t    real_old_trigger_value=0;

    // read all required data and write it in current memory position
    for(i=0;i<g_str_osci.channel_count;i++)
    {
        //get pointer and make proper casting depending the object type
        switch(g_str_osci.channels[i].object_type)
        {
            case Object_Type_U8:
                pu8_value = g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pu8_value);
            break;
            case Object_Type_U16:
                pu16_value= (uint16_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pu16_value);
            break;
            case Object_Type_U32:
                pu32_value= (uint32_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pu32_value);
            break;
            case Object_Type_I8:
                pi8_value = (int8_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pi8_value);
            break;
            case Object_Type_I16:
                pi16_value= (int16_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pi16_value);
            break;
            case Object_Type_I32:
                pi32_value= (int32_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value= (float32_t)(*pi32_value);
            break;
            case Object_Type_Real:
                preal_value= (float32_t *)g_str_osci.channels[i].pu8_obj_address;
                real_value = (float32_t)(*preal_value);
            break;
            default:
                //this should not occur
                return;
            //break;
        }

        //check if user wants old trigger source logic, where trigger is one of the monitored channels
        if (g_str_osci.uc_trigger_source)
        {
            //remember trigger source value
            if ((g_str_osci.uc_trigger_source-1) == i)
            {
                real_trigger_value    = real_value;
                uc_trigger_value_found= 1;
            }
        }
        else // read trigger variable separately since not one of the monitored signals
        {
            //get pointer and make proper casting only at first read
            if (i == 0)
            {
                switch(g_str_osci.trigger_channel.object_type)
                {
                    case Object_Type_U8:
                        pu8_value         = g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pu8_value);
                    break;
                    case Object_Type_U16:
                        pu16_value        = (uint16_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pu16_value);
                    break;
                    case Object_Type_U32:
                        pu32_value        = (uint32_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pu32_value);
                    break;
                    case Object_Type_I8:
                        pi8_value         = (int8_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pi8_value);
                    break;
                    case Object_Type_I16:
                        pi16_value        = (int16_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pi16_value);
                    break;
                    case Object_Type_I32:
                        pi32_value        = (int32_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*pi32_value);
                    break;
                    case Object_Type_Real:
                        preal_value       = (float32_t *)g_str_osci.trigger_channel.pu8_obj_address;
                        real_trigger_value= (float32_t)(*preal_value);
                    break;
                    default:
                        //this should not occur
                        return;
                    //break;
                }
            }
            uc_trigger_value_found=1;
        }

        // store data
        g_str_osci.preal_osci_data_address[g_str_osci.ul_osci_data_stop+i]=real_value;
    }

    // at waiting state check anything related to trigger to change status
    // and number of recorder pretrigger / posttrigger samples
    if (g_str_osci.status == Status_Waiting)
    {
        // check trigger modes
        //immediate recording if continuous trigger mode
        if (g_str_osci.trigger_mode == Trigger_Mode_Continuous)
        {
            Oscilloscope_settings__Status    = g_str_osci.status = Status_Recording;
            g_str_osci.ul_triggered_position = g_str_osci.ul_osci_data_stop;
        }
        else if (uc_trigger_value_found)
        {
             //match
             if (g_str_osci.trigger_mode == Trigger_Mode_On_Match)
             {
                 if ((real_trigger_value < (g_str_osci.real_trigger_level+FLOAT_PRECISION_EQUAL_HYST))
                   &&(real_trigger_value > (g_str_osci.real_trigger_level-FLOAT_PRECISION_EQUAL_HYST)))
                 {
                     Oscilloscope_settings__Status   = g_str_osci.status = Status_Recording;
                     g_str_osci.ul_triggered_position= g_str_osci.ul_osci_data_stop;
                 }
             } // above
             else if (g_str_osci.trigger_mode == Trigger_Mode_Above)
             {
                 if (real_trigger_value > g_str_osci.real_trigger_level)
                 {
                     Oscilloscope_settings__Status   = g_str_osci.status = Status_Recording;
                     g_str_osci.ul_triggered_position= g_str_osci.ul_osci_data_stop;
                 }
             } //below
             else if (g_str_osci.trigger_mode == Trigger_Mode_Below)
             {
                 if (real_trigger_value < g_str_osci.real_trigger_level)
                 {
                     Oscilloscope_settings__Status   = g_str_osci.status = Status_Recording;
                     g_str_osci.ul_triggered_position= g_str_osci.ul_osci_data_stop;
                 }
             }
             //rising
             else if (g_str_osci.trigger_mode == Trigger_Mode_Rising)
             {
                 //first time we must set old_l_trigger_value to correct position-value
                 if (g_str_osci.ul_pretrigger_samples_recorded == 0)
                 {
                     //set it above trigger level
                     real_old_trigger_value=g_str_osci.real_trigger_level+1;
                 }

                 if ((real_trigger_value > g_str_osci.real_trigger_level)&&(real_old_trigger_value <= g_str_osci.real_trigger_level))
                 {
                     Oscilloscope_settings__Status   = g_str_osci.status = Status_Recording;
                     g_str_osci.ul_triggered_position= g_str_osci.ul_osci_data_stop;
                 }
                 real_old_trigger_value=real_trigger_value;
             }
             //falling
             else if (g_str_osci.trigger_mode == Trigger_Mode_Falling)
             {
                 //first time we must set old_l_trigger_value to correct position-value
                 if (g_str_osci.ul_pretrigger_samples_recorded == 0)
                 {
                     //set it below trigger level
                     real_old_trigger_value=g_str_osci.real_trigger_level-1;
                 }

                 if ((real_trigger_value < g_str_osci.real_trigger_level)&&(real_old_trigger_value >= g_str_osci.real_trigger_level))
                 {
                     Oscilloscope_settings__Status   = g_str_osci.status = Status_Recording;
                     g_str_osci.ul_triggered_position= g_str_osci.ul_osci_data_stop;
                 }
                 real_old_trigger_value=real_trigger_value;
             }
        }
    }

    //increment records
    //in case of waiting increase pretrigger samples, otherwise increase postrigger
    if (g_str_osci.status == Status_Waiting)
        g_str_osci.ul_pretrigger_samples_recorded++;
    else
    {
        g_str_osci.ul_posttrigger_samples_recorded++;

        //in continuos mode posttrigger samples can be owerwritten, otherwise not; sine we are protecting pretrigger samples as required
        if (g_str_osci.trigger_mode != Trigger_Mode_Continuous)
        {
            //check if limit reached
            if ((g_str_osci.ul_posttrigger_samples_recorded + g_str_osci.ul_pretrigger_samples_requested) >= g_str_osci.ul_max_records)
            {
                osci_decrease_record();

                //update satus and domain data size
                #if OSCI_CANOPEN_INTERFACE
                    co_setup_domain_size (&Oscilloscope_settings__Record,(g_str_osci.ul_posttrigger_samples_recorded + g_str_osci.ul_pretrigger_samples_recorded)*4*g_str_osci.channel_count);
                #else
                    #error
                #endif

                Oscilloscope_settings__Status = g_str_osci.status = Status_Finished;

                return;
            }
        }
    }

    //decrease nr of pretrigger samples
    osci_decrease_record();

    //increment new read stop
    g_str_osci.ul_osci_data_stop+=g_str_osci.channel_count;

    //check if next writes will record will go over end memory
    if ((g_str_osci.ul_osci_data_stop + g_str_osci.channel_count) > g_str_osci.ul_osci_data_size)
    {
        //wrap around allowed
        g_str_osci.ul_osci_data_stop=0;
        g_str_osci.uc_osci_write_complete_flag=1; // enable flag to know that we made at least one wrap around; we are full of data
    }

    if (g_str_osci.uc_osci_write_complete_flag)
    {
        //correct start position
        g_str_osci.ul_osci_data_start=g_str_osci.ul_osci_data_stop+g_str_osci.channel_count;
        if ((g_str_osci.ul_osci_data_start + g_str_osci.channel_count) > g_str_osci.ul_osci_data_size)
            g_str_osci.ul_osci_data_start=0;


        //check if we are allowed to write more records-pretrigger required samples
        //this check is made only when we are writing once more over same data space - wrap around
        //next write will decrease record, so check if we can do that at next point
        if (g_str_osci.ul_pretrigger_samples_requested > 0)
        {
            if (g_str_osci.ul_pretrigger_samples_requested == g_str_osci.ul_pretrigger_samples_recorded)
            {
                #if OSCI_CANOPEN_INTERFACE
                    co_setup_domain_size (&Oscilloscope_settings__Record,(g_str_osci.ul_posttrigger_samples_recorded + g_str_osci.ul_pretrigger_samples_recorded)*4*g_str_osci.channel_count);
                #else
                    #error
                #endif

                Oscilloscope_settings__Status = g_str_osci.status = Status_Finished;
                return;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
*   this function is used to simply decrease counters; since it is many times used in same manner
*
* @return       void
*/
////////////////////////////////////////////////////////////////////////////////
static void osci_decrease_record(void)
{
    if (g_str_osci.uc_osci_write_complete_flag)
    {
        //decreaase records
        if (g_str_osci.ul_pretrigger_samples_recorded > 0)
            g_str_osci.ul_pretrigger_samples_recorded--;
        else
        {
            g_str_osci.ul_posttrigger_samples_recorded--;
        }
    }

    Oscilloscope_settings__Pretrigger_samples_recorded  = g_str_osci.ul_pretrigger_samples_recorded;
    Oscilloscope_settings__Posttrigger_samples_recorded = g_str_osci.ul_posttrigger_samples_recorded;
}

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////



	




