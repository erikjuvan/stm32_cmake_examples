#include "../CANopen/gen_define.h"

#include "../CANopen/co_canopen.h"

// Array Definitions
///UNSIGNED8 Variables
UNSIGNED8 CO_u8_Node_ID_E;
UNSIGNED8 CO_u8_Communication_Error;

///INTEGER8 Variables

///UNSIGNED16 Variables
UNSIGNED16 CO_u16_Inhibit_Time_Emergency;
UNSIGNED16 CO_u16_Producer_Heartbeat_Time;
UNSIGNED16 CO_u16_CAN_Bitrate_CAN_Bitrate_E;

///INTEGER16 Variables

///UNSIGNED32 Variables
UNSIGNED32 CO_u32_COB_ID_SYNC;
UNSIGNED32 CO_u32_Communication_Cycle_Period;
UNSIGNED32 CO_u32_COB_ID_EMCY;
UNSIGNED32 CO_u32_Consumer_Heartbeat_Time;

///INTEGER32 Variables

///REAL32 Variables

///VIS_STRING Variables
char CO_vis_string_Manufacturer_Device_Name[32]={""};
char CO_vis_string_Device_ID[47]={"00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00"};
char CO_vis_string_Drive_manufacturer[6]={"EMSISO"};

///OS_STRING Variables

///CO_DOMAIN_PTR Variables
CO_DOMAIN_PTR CO_domain_Store_EDS;
extern volatile CO_DOMAIN_PTR Oscilloscope_settings__Record[];

// EEPROM variables
//CO_u8_Node_ID_E:0x100b:0
//CO_u16_CAN_Bitrate_CAN_Bitrate_E:0x2001:0
//Ctrl_Gen__MaxRPMcalculations:0x3100:1
//Ctrl_Gen__Fcur:0x3100:2
//Ctrl_Gen__CurToVelRatio:0x3100:3
//Ctrl_Mod__MinOffTimeLowSide:0x3104:1
//Ctrl_Mod__MinOffTimeHighSide:0x3104:2
//Ctrl_Mod__MinOffTimeSecondHighSide:0x3104:3
//Ctrl_Mod__MinPulseTime:0x3104:4
//Ctrl_Mod__PWMOffset:0x3104:5
//Ctrl_Mod__DeadTimeComp:0x3104:6
//Ctrl_Motor__Rs:0x310b:1
//Ctrl_Motor__Np:0x310b:2
//Ctrl_Motor__Ld:0x310b:3
//Ctrl_Motor__Lq:0x310b:4
//Ctrl_Motor__Psi:0x310b:5
//Ctrl_Motor__IdToIqRatio:0x310b:6
//Ctrl_Motor__Rth[0]:0x310c:1
//Ctrl_Motor__Rth[1]:0x310c:2
//Ctrl_Motor__Tau[0]:0x310d:1
//Ctrl_Motor__Tau[1]:0x310d:2
//Ctrl_Motor__InitialTempRise[0]:0x310e:1
//Ctrl_Motor__InitialTempRise[1]:0x310e:2
//Ctrl_Motor__IronLossCoef:0x310f:1
//Ctrl_Motor__TempFromRenabled:0x310f:2
//Ctrl_Motor__TempFromRgain:0x310f:3
//Ctrl_Motor__TempFromRoffset:0x310f:4
//Ctrl_PowerLimit__UpperLimit:0x3112:1
//Ctrl_PowerLimit__MiddleLimit:0x3112:2
//Ctrl_PowerLimit__LowerLimit:0x3112:3
//Ctrl_PowerLimit__DisableLimit:0x3112:4
//Ctrl_PowerLimit__Duration:0x3112:5
//Ctrl_PowerLimit__DurationHyst:0x3112:6
//Ctrl_PowerLimit__BridgeTempHigh:0x3112:7
//Ctrl_PowerLimit__BridgeTempLow:0x3112:8
//Ctrl_Limits__IsMax:0x3120:1
//Ctrl_Limits__IdMin:0x3120:2
//Ctrl_Limits__IqMax:0x3120:3
//Ctrl_Limits__UdcMaxToTorque:0x3120:4
//Ctrl_Limits__UdcMaxToTorqueGain:0x3120:5
//Ctrl_Limits__RPMpositive:0x3120:6
//Ctrl_Limits__RPMnegative:0x3120:7
//Ctrl_Limits__RPMramp:0x3120:8
//Ctrl_Limits__BridgeTemperature:0x3120:9
//Ctrl_Limits__BridgeTemperaturePgain:0x3120:10
//Ctrl_Limits__BridgeTemperatureIgain:0x3120:11
//Ctrl_Limits__MotorTemperature:0x3120:12
//Ctrl_Limits__MotorTemperaturePgain:0x3120:13
//Ctrl_Limits__MotorTemperatureIgain:0x3120:14
//Ctrl_Limits__PowerLimitPgain:0x3120:15
//Ctrl_Limits__PowerLimitIgain:0x3120:16
//Ctrl_Limits_OverrideCloseLoopRamp:0x3120:17
//Ctrl_Limits__CloseLoopRamp:0x3120:18
//Ctrl_Limits__OpenLoopRampOverrideValue:0x3120:19
//Ctrl_Limits__MaximumRampLowRpm:0x3120:20
//Ctrl_Limits__IsMaxStartup:0x3120:21
//Ctrl_Limits__IsMaxSpeedLimit:0x3120:22
//Ctrl_Limits__OutletTemperature:0x3120:23
//Ctrl_Limits__OutletTemperaturePgain:0x3120:24
//Ctrl_Limits__OutletTemperatureIgain:0x3120:25
//Ctrl_CurrentReg__DecoupleIdIqEnable:0x3130:1
//Ctrl_CurrentReg__DecouplePsi:0x3130:2
//Ctrl_CurrentReg__DaxisPgain:0x3130:3
//Ctrl_CurrentReg__DaxisIgain:0x3130:4
//Ctrl_CurrentReg__QaxisPgain:0x3130:5
//Ctrl_CurrentReg__QaxisIgain:0x3130:6
//Ctrl_CurrentReg__CalculationDelay:0x3130:7
//Ctrl_CurrentReg__CurrentOffset_Iu:0x3130:8
//Ctrl_CurrentReg__CurrentOffset_Iv:0x3130:9
//Ctrl_CurrentReg__CurrentOffset_Iw:0x3130:10
//Ctrl_CurrentReg_Calculated__Bandwith:0x313b:1
//Ctrl_FieldWeak__TargetModulationFactor:0x3140:1
//Ctrl_FieldWeak__Pgain:0x3140:2
//Ctrl_FieldWeak__Igain:0x3140:3
//Ctrl_FieldWeak__LPFtimeConstant:0x3140:4
//Ctrl_VelocityReg__Pgain:0x3150:1
//Ctrl_VelocityReg__Igain:0x3150:2
//Ctrl_VelocityReg__Dgain:0x3150:3
//Ctrl_VelocityReg__DpartLPF:0x3150:4
//Ctrl_VelocityReg__StartIq:0x3150:5
//Ctrl_VelocityReg__PgainHigh:0x3150:6
//Ctrl_VelocityReg__IgainHigh:0x3150:7
//Ctrl_VelocityReg__Pgain_LoadMeas:0x3150:8
//Ctrl_VelocityReg__Igain_LoadMeas:0x3150:9
//Ctrl_VelocityReg__SpeedLimFirst:0x3150:10
//Ctrl_VelocityReg__SpeedLimSecond:0x3150:11
//Ctrl_Sensorless__Enabled:0x3160:1
//Ctrl_Sensorless__Active:0x3160:2
//Ctrl_Sensorless__TransitionTimeToClose:0x3160:3
//Ctrl_Sensorless__TransitionTimeToOpen:0x3160:4
//Ctrl_Sensorless__ObserverK1:0x3160:5
//Ctrl_Sensorless__ObserverK2:0x3160:6
//Ctrl_Sensorless__PLL_Bandwith:0x3160:7
//Ctrl_Sensorless__PLL_BandwithReduction:0x3160:8
//Ctrl_Sensorless__OmegaPostLPFtimeConstant:0x3160:9
//Ctrl_Sensorless__CalculationDelay:0x3160:10
//Ctrl_Sensorless__ValidPLLerrMax:0x3160:11
//Ctrl_Sensorless__ValidBEMFratioMin:0x3160:12
//Ctrl_Sensorless__ValidBEMFratioMax:0x3160:13
//Ctrl_Sensorless__EndRPM:0x3160:14
//Ctrl_Sensorless__ValidMaxRPM:0x3160:15
//Ctrl_Sensorless__ValidMinRPM:0x3160:16
//Ctrl_Sensorless__MaxRPM:0x3160:17
//Ctrl_Sensorless__ValidLPFtimeConstant:0x3160:18
//Ctrl_Sensorless__MinTimeInClose:0x3160:19
//Ctrl_Sensorless__PLLBWSpeedSwitch:0x3160:20
//Ctrl_Sensorless__TimeInHoldState:0x3160:21
//Ctrl_SensorlessCalculatedGain__ObserverF:0x316c:1
//Ctrl_OpenLoop__Id:0x3170:1
//Ctrl_OpenLoop__RampIdRate:0x3170:2
//Ctrl_OpenLoop__RampRPMrate:0x3170:3
//Ctrl_IdtoIq_Switch_Enable:0x3172:1
//Ctrl_ALTTestMode__Enable:0x31ad:1
//Ctrl_ALTTestMode__RPM:0x31ad:2
//Ctrl_Power__CompensationStart:0x31b0:1
//Ctrl_Power__CompensationSlope:0x31b0:2
//Ctrl_Power__CompensationOffset:0x31b0:3
//Ctrl_AngleGenerator__Offset:0x31da:3
//Ctrl_Measurment__LPF_TimeConst:0x31f0:1
//Ctrl_Measurment__Pel_LPF_TimeConst:0x31f0:6
//protections__phase_overcurrent:0x5003:1
//protections__overvoltage:0x5003:2
//protections__undervoltage:0x5003:3
//protections__bridge_overtemperature:0x5003:4
//protections__phase_overcurrent_hw:0x5003:5
//protections__phase_overvoltage_hw:0x5003:6
//protections__motor_overtemperature:0x5003:7
//current_rec__sector:0x5004:1
//current_rec__sector_old:0x5004:2
