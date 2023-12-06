////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      osci_parameters.c
*@brief     osci parameters needed for oci
*@author    Zdenko Mezgec
*@date      10.12.2020
*
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup OSCI_PARAMETERS
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "osci.h"
#include "../osci_cfg.h"

#if OSCI_CANOPEN_INTERFACE
    #include "Middlewares/Protocol/CANopen/CANopen/canopen.h"
#endif

#include "osci_internal.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/*
@! SYMBOL = Oscilloscope_settings__Command
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 6] [0 ... 6]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 1
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Command = Command_Stop;

/*
@! SYMBOL = Oscilloscope_settings__Memory
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 1] [0 ... 1]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 2
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Memory = Memory_Target_RAM;

/*
@! SYMBOL = Oscilloscope_settings__Sampling_rate
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = ULONG [1 ... 4294967295] [1 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 3
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 1
@! END
*/
volatile uint32_t Oscilloscope_settings__Sampling_rate = 1;

/*
@! SYMBOL = Oscilloscope_settings__Reserved_1
@! A2L_TYPE = MEASURE
@! DATA_TYPE = UBYTE [0 ... 1] [0 ... 1]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 4
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Reserved_1 = 0;

/*
@! SYMBOL = Oscilloscope_settings__Trigger_level
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = FLOAT [-2.0E+9 ... 2.0E+9] [-2.0E+9 ... 2.0E+9]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 5
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile float32_t Oscilloscope_settings__Trigger_level = 0;

/*
@! SYMBOL = Oscilloscope_settings__Trigger_mode
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 5] [0 ... 5]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 6
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Trigger_mode = Trigger_Mode_Continuous;

/*
@! SYMBOL = Oscilloscope_settings__Trigger_source
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 7
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Trigger_source = 0;

/*
@! SYMBOL = Oscilloscope_settings__Pretrigger_samples_requested
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 8
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_settings__Pretrigger_samples_requested = 0;

/*
@! SYMBOL = Oscilloscope_settings__Pretrigger_samples_recorded
@! A2L_TYPE = MEASURE
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 9
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_settings__Pretrigger_samples_recorded = 0;

/*
@! SYMBOL = Oscilloscope_settings__Posttrigger_samples_recorded
@! A2L_TYPE = MEASURE
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 10
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_settings__Posttrigger_samples_recorded = 0;

/*
@! SYMBOL = Oscilloscope_settings__Status
@! A2L_TYPE = MEASURE
@! DATA_TYPE = UBYTE [0 ... 11] [0 ... 11]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 11
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 4
@! END
*/
volatile uint8_t Oscilloscope_settings__Status = Status_Not_Configured;

/*
@! SYMBOL = Oscilloscope_settings__Record_number
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 12
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_settings__Record_number = 0;

/*
@! SYMBOL = Oscilloscope_settings__Record
@! A2L_TYPE = MEASURE
@! DATA_TYPE = DOMAIN
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 13
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
#if OSCI_CANOPEN_INTERFACE
    CO_DOMAIN_PTR Oscilloscope_settings__Record;
#endif

/*
@! SYMBOL = Oscilloscope_settings__Memory_capacity
@! A2L_TYPE = MEASURE
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 14
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_settings__Memory_capacity = 0;

/*
@! SYMBOL = Oscilloscope_settings__Trigger_source_Index
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UWORD [0 ... 65535] [0 ... 65535]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 15
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint16_t Oscilloscope_settings__Trigger_source_Index = 0;

/*
@! SYMBOL = Oscilloscope_settings__Trigger_source_Subindex
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 16
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Trigger_source_Subindex = 0;

/*
@! SYMBOL = Oscilloscope_settings__Configuration_number
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 17
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Configuration_number = 0;

/*
@! SYMBOL = Oscilloscope_settings__Record_channel
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 18
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Record_channel = 0;

/*
@! SYMBOL = Oscilloscope_settings__Record_step
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = SWORD [-32768 ... 32767] [-32768 ... 32767]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 19
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile int16_t Oscilloscope_settings__Record_step = 0;

/*
@! SYMBOL = Oscilloscope_settings__Number_of_configurations
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2080
@! CAN_SUBINDEX = 20
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_settings__Number_of_configurations = 0;

/*
@! SYMBOL = Oscilloscope_channels__Channel_count
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2081
@! CAN_SUBINDEX = 1
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_channels__Channel_count = 0;

/*
@! SYMBOL = Oscilloscope_channels__Selected_channel
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2081
@! CAN_SUBINDEX = 2
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_channels__Selected_channel = 0;

/*
@! SYMBOL = Oscilloscope_channels__Channel_index
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UWORD [0 ... 65535] [0 ... 65535]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2081
@! CAN_SUBINDEX = 3
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint16_t Oscilloscope_channels__Channel_index = 0;

/*
@! SYMBOL = Oscilloscope_channels__Channel_subindex
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2081
@! CAN_SUBINDEX = 4
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_channels__Channel_subindex = 0;

/*
@! SYMBOL = Oscilloscope_channels__Channel_configuration_status
@! A2L_TYPE = MEASURE
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2081
@! CAN_SUBINDEX = 5
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_channels__Channel_configuration_status = 0;

/*
@! SYMBOL = Oscilloscope_test_signals__Test_signal_1
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = UBYTE [0 ... 255] [0 ... 255]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2082
@! CAN_SUBINDEX = 1
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint8_t Oscilloscope_test_signals__Test_signal_1 = 0;

/*
@! SYMBOL = Oscilloscope_test_signals__Test_signal_2
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = SBYTE [-128 ... 127] [-128 ... 127]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2082
@! CAN_SUBINDEX = 2
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile int8_t Oscilloscope_test_signals__Test_signal_2 = 0;

/*
@! SYMBOL = Oscilloscope_test_signals__Test_signal_3
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = ULONG [0 ... 4294967295] [0 ... 4294967295]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2082
@! CAN_SUBINDEX = 3
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile uint32_t Oscilloscope_test_signals__Test_signal_3 = 0;

/*
@! SYMBOL = Oscilloscope_test_signals__Test_signal_4
@! A2L_TYPE = PARAMETER
@! DATA_TYPE = SLONG [-2147483648 ... 2147483647] [-2147483648 ... 2147483647]
@! DESCRIPTION = ""
@! GROUP = Oscilloscope
@! DIMENSION = 1 1
@! UNIT = ""
@! CAN_INDEX = 0x2082
@! CAN_SUBINDEX = 4
@! READ_LEVEL = 3
@! WRITE_LEVEL = 3
@! PERSIST = 0
@! DEFAULT = 0
@! END
*/
volatile int32_t Oscilloscope_test_signals__Test_signal_4 = 0;

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
	




