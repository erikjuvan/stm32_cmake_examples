#include <stdlib.h>
#include <stdio.h>
#include "../CANopen/gen_define.h"
#include "../CANopen/co_canopen.h"

#if CAN_PROTOCOL_TYPE == 0
	#if CO_STACK_VERSION < 0x10600ul
	typedef PDO_MAP_TABLE_T PDO_TR_MAP_TABLE_T;
	typedef PDO_MAP_TABLE_T PDO_REC_MAP_TABLE_T;
	#endif
	#ifndef CO_ATTR_MAP_REC
	#define CO_ATTR_MAP_REC CO_ATTR_MAP
	#endif
	#ifndef CO_ATTR_MAP_TR
	#define CO_ATTR_MAP_TR CO_ATTR_MAP
	#endif

	/* application header file(s) */
	//#include <p402_state.h>
#endif

/* definition of memory specifier */
#ifndef CO_CONST
#define CO_CONST const
#endif

/* definition of compiler-specific storage classes */
#ifndef CO_STORAGE_CLASS
#define CO_STORAGE_CLASS
#endif
#ifndef CO_CONST_STORAGE_CLASS
#define CO_CONST_STORAGE_CLASS
#endif


/* number of objects */
#define CO_OD_ASSIGN_CNT 118u
#define CO_OBJ_DESC_CNT 558u

/* definition of managed variables */
static UNSIGNED8 CO_STORAGE_CLASS od_u8[27];
static UNSIGNED16 CO_STORAGE_CLASS od_u16[16];
static UNSIGNED32 CO_STORAGE_CLASS od_u32[89];

/* definition of constants */
static CO_CONST INTEGER8 CO_CONST_STORAGE_CLASS od_const_i8[7] = {
	(INTEGER8)0,
	(INTEGER8)-128,
	(INTEGER8)127,
	(INTEGER8)-2,
	(INTEGER8)2,
	(INTEGER8)1,
	(INTEGER8)10};
CO_CONST UNSIGNED8 CO_CONST_STORAGE_CLASS od_const_u8[18] = {
	(UNSIGNED8)0u,
	(UNSIGNED8)255u,
	(UNSIGNED8)5u,
	(UNSIGNED8)1u,
	(UNSIGNED8)4u,
	(UNSIGNED8)127u,
	(UNSIGNED8)3u,
	(UNSIGNED8)2u,
	(UNSIGNED8)254u,
	(UNSIGNED8)6u,
	(UNSIGNED8)20u,
	(UNSIGNED8)11u,
	(UNSIGNED8)7u,
	(UNSIGNED8)8u,
	(UNSIGNED8)25u,
	(UNSIGNED8)10u,
	(UNSIGNED8)21u,
	(UNSIGNED8)14u};
static CO_CONST INTEGER16 CO_CONST_STORAGE_CLASS od_const_i16[3] = {
	(INTEGER16)0,
	(INTEGER16)-32768,
	(INTEGER16)32767};
CO_CONST UNSIGNED16 CO_CONST_STORAGE_CLASS od_const_u16[3] = {
	(UNSIGNED16)0u,
	(UNSIGNED16)65535u,
	(UNSIGNED16)500u};
static CO_CONST INTEGER32 CO_CONST_STORAGE_CLASS od_const_i32[7] = {
	(INTEGER32)0L,
	(INTEGER32)-2147483648L,
	(INTEGER32)2147483647L,
	(INTEGER32)16L,
	(INTEGER32)1L,
	(INTEGER32)128L,
	(INTEGER32)8L};
CO_CONST UNSIGNED32 CO_CONST_STORAGE_CLASS od_const_u32[7] = {
	(UNSIGNED32)402UL,
	(UNSIGNED32)0UL,
	(UNSIGNED32)4294967295UL,
	(UNSIGNED32)128UL,
	(UNSIGNED32)793UL,
	(UNSIGNED32)1UL,
	(UNSIGNED32)1610547208UL};
static CO_CONST REAL32 CO_CONST_STORAGE_CLASS od_const_r32[113] = {
	(REAL32)0.0f,
	(REAL32)-2000000000.0f,
	(REAL32)2000000000.0f,
	(REAL32)25000.0f,
	(REAL32)0.0f,
	(REAL32)100000.0f,
	(REAL32)16000.0f,
	(REAL32)1000.0f,
	(REAL32)-79228162514264337593543950335.0f,
	(REAL32)79228162514264337593543950335.0f,
	(REAL32)0.0001f,
	(REAL32)0.0000020f,
	(REAL32)0.0000050f,
	(REAL32)0.5f,
	(REAL32)1.0f,
	(REAL32)0.02f,
	(REAL32)2.74f,
	(REAL32)100.0f,
	(REAL32)3.0f,
	(REAL32)0.00838f,
	(REAL32)0.01596f,
	(REAL32)0.125f,
	(REAL32)10.0f,
	(REAL32)0.1f,
	(REAL32)0.3434106f,
	(REAL32)2.873673f,
	(REAL32)91.76567f,
	(REAL32)4362.511f,
	(REAL32)200.0f,
	(REAL32)0.000000108f,
	(REAL32)23.6984f,
	(REAL32)-1000000.0f,
	(REAL32)1000000.0f,
	(REAL32)-288.7719f,
	(REAL32)750.0f,
	(REAL32)1200.0f,
	(REAL32)800.0f,
	(REAL32)700.0f,
	(REAL32)550.0f,
	(REAL32)180.0f,
	(REAL32)500.0f,
	(REAL32)95.0f,
	(REAL32)120.0f,
	(REAL32)80.0f,
	(REAL32)3.2f,
	(REAL32)-4.3f,
	(REAL32)-250.0f,
	(REAL32)4.5f,
	(REAL32)380.0f,
	(REAL32)2000.0f,
	(REAL32)0.4f,
	(REAL32)8000.0f,
	(REAL32)30000.0f,
	(REAL32)-8000.0f,
	(REAL32)-30000.0f,
	(REAL32)1013.0f,
	(REAL32)150.0f,
	(REAL32)130.0f,
	(REAL32)0.0002f,
	(REAL32)0.002f,
	(REAL32)400.0f,
	(REAL32)1025.0f,
	(REAL32)1050.0f,
	(REAL32)20.0f,
	(REAL32)18000.0f,
	(REAL32)85.0f,
	(REAL32)0.6f,
	(REAL32)2.0f,
	(REAL32)21.0f,
	(REAL32)6886.0f,
	(REAL32)40.0f,
	(REAL32)1.7f,
	(REAL32)-3.0f,
	(REAL32)-0.25f,
	(REAL32)-0.3f,
	(REAL32)10000.0f,
	(REAL32)0.9f,
	(REAL32)0.2f,
	(REAL32)0.05f,
	(REAL32)0.03f,
	(REAL32)0.000080f,
	(REAL32)0.005f,
	(REAL32)0.01f,
	(REAL32)227.0f,
	(REAL32)1496520.0f,
	(REAL32)100000000.0f,
	(REAL32)50.0f,
	(REAL32)1.5f,
	(REAL32)-10.0f,
	(REAL32)90.0f,
	(REAL32)650.0f,
	(REAL32)300.0f,
	(REAL32)22000.0f,
	(REAL32)6.0f,
	(REAL32)2.5f,
	(REAL32)1500.0f,
	(REAL32)-180.0f,
	(REAL32)0.03125f,
	(REAL32)12.0f,
	(REAL32)125.0f,
	(REAL32)5000.0f,
	(REAL32)8.0f,
	(REAL32)1100.0f,
	(REAL32)410.0f,
	(REAL32)450.0f,
	(REAL32)15.0f,
	(REAL32)350.0f,
	(REAL32)105.0f,
	(REAL32)150.0f,
	(REAL32)10.0f,
	(REAL32)410.0f,
	(REAL32)200.0f,
	(REAL32)350.0f};

/* definition of application variables */
static UNSIGNED8 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_u8[53] = {
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(CO_u8_Node_ID_E), /* 0x100B:0*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(CO_u8_Communication_Error), /* 0x1029:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Command), /* 0x2080:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Memory), /* 0x2080:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Reserved_1), /* 0x2080:4*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Trigger_mode), /* 0x2080:6*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Trigger_source), /* 0x2080:7*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Status), /* 0x2080:11*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Trigger_source_Subindex), /* 0x2080:16*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Configuration_number), /* 0x2080:17*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Record_channel), /* 0x2080:18*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Number_of_configurations), /* 0x2080:20*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_channels__Channel_count), /* 0x2081:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_channels__Selected_channel), /* 0x2081:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_channels__Channel_subindex), /* 0x2081:4*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_channels__Channel_configuration_status), /* 0x2081:5*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_test_signals__Test_signal_1), /* 0x2082:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__RunStopIn), /* 0x3101:5*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__RunStopOut), /* 0x3101:6*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__TempFromRenabled), /* 0x310F:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_OverrideCloseLoopRamp), /* 0x3120:17*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusDisIntegration[0]), /* 0x3125:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusDisIntegration[1]), /* 0x3125:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusDisIntegration[2]), /* 0x3125:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusDisIntegration[3]), /* 0x3125:4*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusDisIntegration[4]), /* 0x3125:5*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__DecouplePsi), /* 0x3130:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusDissableIntegration), /* 0x3153:0*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusTracking), /* 0x3154:0*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__Enabled), /* 0x3160:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__Active), /* 0x3160:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_IdtoIq_Switch_Enable), /* 0x3172:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_ALTTestMode__Enable), /* 0x31AD:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__Override), /* 0x31D0:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__SineOrRect), /* 0x31D0:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__Override), /* 0x31D1:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__SineOrRect), /* 0x31D1:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_AngleGenerator__Enable), /* 0x31DA:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Doc_version[0]), /* 0x3200:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Doc_version[1]), /* 0x3200:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(Ctrl_Doc_version[2]), /* 0x3200:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(motor_control__protections), /* 0x5000:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(motor_control__warnigns), /* 0x5000:5*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(motor_control__hw_bridge_flag), /* 0x5000:10*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(motor_control__hw_bridge_cnt), /* 0x5000:11*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(current_rec__sector), /* 0x5004:1*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(current_rec__sector_old), /* 0x5004:2*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__measurement_request), /* 0x500A:3*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__direction_request), /* 0x500A:4*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__manual_request), /* 0x500A:9*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__emergency_stop), /* 0x500A:10*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__measure_state), /* 0x500A:11*/
	(UNSIGNED8 CO_CONST_STORAGE_CLASS *)&(app__selftest) /* 0x500A:12*/
};
static INTEGER8 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_i8[4] = {
	(INTEGER8 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_test_signals__Test_signal_2), /* 0x2082:2*/
	(INTEGER8 CO_CONST_STORAGE_CLASS *)&(motor_control__state), /* 0x5000:1*/
	(INTEGER8 CO_CONST_STORAGE_CLASS *)&(motor_control__control), /* 0x5000:2*/
	(INTEGER8 CO_CONST_STORAGE_CLASS *)&(app__state) /* 0x500A:1*/
};
static UNSIGNED16 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_u16[5] = {
	(UNSIGNED16 CO_CONST_STORAGE_CLASS *)&(CO_u16_Inhibit_Time_Emergency), /* 0x1015:0*/
	(UNSIGNED16 CO_CONST_STORAGE_CLASS *)&(CO_u16_Producer_Heartbeat_Time), /* 0x1017:0*/
	(UNSIGNED16 CO_CONST_STORAGE_CLASS *)&(CO_u16_CAN_Bitrate_CAN_Bitrate_E), /* 0x2001:0*/
	(UNSIGNED16 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Trigger_source_Index), /* 0x2080:15*/
	(UNSIGNED16 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_channels__Channel_index) /* 0x2081:3*/
};
static INTEGER16 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_i16[1] = {
	(INTEGER16 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Record_step) /* 0x2080:19*/
};
static UNSIGNED32 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_u32[22] = {
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(CO_u32_COB_ID_SYNC), /* 0x1005:0*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(CO_u32_Communication_Cycle_Period), /* 0x1006:0*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(CO_u32_COB_ID_EMCY), /* 0x1014:0*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(CO_u32_Consumer_Heartbeat_Time), /* 0x1016:1*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(identity_object__vendor_id), /* 0x1018:1*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(identity_object__product_code), /* 0x1018:2*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(identity_object__revision_number), /* 0x1018:3*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Sampling_rate), /* 0x2080:3*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Pretrigger_samples_requested), /* 0x2080:8*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Pretrigger_samples_recorded), /* 0x2080:9*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Posttrigger_samples_recorded), /* 0x2080:10*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Record_number), /* 0x2080:12*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Memory_capacity), /* 0x2080:14*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_test_signals__Test_signal_3), /* 0x2082:3*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__state_duration), /* 0x500A:2*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__target_rpm), /* 0x500A:5*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__ramp), /* 0x500A:6*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__duration), /* 0x500A:7*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__timeout), /* 0x500A:8*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__errors), /* 0x500A:13*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(app__warnings), /* 0x500A:14*/
	(UNSIGNED32 CO_CONST_STORAGE_CLASS *)&(system_status) /* 0x5100:0*/
};
static INTEGER32 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_i32[12] = {
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_test_signals__Test_signal_4), /* 0x2082:4*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen__CurToVelRatio), /* 0x3100:3*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__Counter), /* 0x3113:1*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__State), /* 0x3113:3*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_Status__State), /* 0x3161:1*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_Status__LastNonErrorState), /* 0x3161:2*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_Status__LatchedCriteriaFlags), /* 0x3161:3*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_CriteriaFlags), /* 0x316B:0*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__GenTarget), /* 0x31D0:1*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__GenTarget), /* 0x31D1:1*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Doc_FW__Version), /* 0x31FF:1*/
	(INTEGER32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Doc_FW__Date) /* 0x31FF:2*/
};
static REAL32 CO_CONST_STORAGE_CLASS * CO_CONST od_ptr_r32[234] = {
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Oscilloscope_settings__Trigger_level), /* 0x2080:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen__MaxRPMcalculations), /* 0x3100:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen__Fcur), /* 0x3100:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__RPMref), /* 0x3101:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__Iq_foc), /* 0x3101:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__Udc), /* 0x3101:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__MotorWireTemp), /* 0x3101:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Gen_Stat__TemperatureFromR), /* 0x3101:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__MinOffTimeLowSide), /* 0x3104:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__MinOffTimeHighSide), /* 0x3104:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__MinOffTimeSecondHighSide), /* 0x3104:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__MinPulseTime), /* 0x3104:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__PWMOffset), /* 0x3104:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod__DeadTimeComp), /* 0x3104:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod_StatusPWM[0]), /* 0x3105:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod_StatusPWM[1]), /* 0x3105:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Mod_StatusPWM[2]), /* 0x3105:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Rs), /* 0x310B:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Np), /* 0x310B:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Ld), /* 0x310B:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Lq), /* 0x310B:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Psi), /* 0x310B:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__IdToIqRatio), /* 0x310B:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Rth[0]), /* 0x310C:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Rth[1]), /* 0x310C:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Tau[0]), /* 0x310D:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__Tau[1]), /* 0x310D:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__InitialTempRise[0]), /* 0x310E:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__InitialTempRise[1]), /* 0x310E:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__IronLossCoef), /* 0x310F:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__TempFromRgain), /* 0x310F:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__TempFromRoffset), /* 0x310F:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__CopperLoss), /* 0x310F:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__IronLoss), /* 0x310F:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Motor__TotalLoss), /* 0x310F:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__UpperLimit), /* 0x3112:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__MiddleLimit), /* 0x3112:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__LowerLimit), /* 0x3112:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__DisableLimit), /* 0x3112:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__Duration), /* 0x3112:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__DurationHyst), /* 0x3112:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__BridgeTempHigh), /* 0x3112:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__BridgeTempLow), /* 0x3112:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__LimitStatus), /* 0x3113:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__StatusActive), /* 0x3113:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_PowerLimit__StatusLimitIq), /* 0x3113:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__IsMax), /* 0x3120:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__IdMin), /* 0x3120:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__IqMax), /* 0x3120:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__UdcMaxToTorque), /* 0x3120:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__UdcMaxToTorqueGain), /* 0x3120:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__RPMpositive), /* 0x3120:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__RPMnegative), /* 0x3120:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__RPMramp), /* 0x3120:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__BridgeTemperature), /* 0x3120:9*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__BridgeTemperaturePgain), /* 0x3120:10*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__BridgeTemperatureIgain), /* 0x3120:11*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__MotorTemperature), /* 0x3120:12*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__MotorTemperaturePgain), /* 0x3120:13*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__MotorTemperatureIgain), /* 0x3120:14*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__PowerLimitPgain), /* 0x3120:15*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__PowerLimitIgain), /* 0x3120:16*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__CloseLoopRamp), /* 0x3120:18*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__OpenLoopRampOverrideValue), /* 0x3120:19*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__MaximumRampLowRpm), /* 0x3120:20*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__IsMaxStartup), /* 0x3120:21*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__IsMaxSpeedLimit), /* 0x3120:22*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__OutletTemperature), /* 0x3120:23*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__OutletTemperaturePgain), /* 0x3120:24*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits__OutletTemperatureIgain), /* 0x3120:25*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusLim[0]), /* 0x3121:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusLim[1]), /* 0x3121:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusLim[2]), /* 0x3121:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusIsMax), /* 0x3122:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusUdcMaxToIq), /* 0x3123:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusRPMramp[0]), /* 0x3124:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusRPMramp[1]), /* 0x3124:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Limits_StatusRampIn), /* 0x3127:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__DecoupleIdIqEnable), /* 0x3130:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__DaxisPgain), /* 0x3130:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__DaxisIgain), /* 0x3130:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__QaxisPgain), /* 0x3130:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__QaxisIgain), /* 0x3130:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__CalculationDelay), /* 0x3130:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__CurrentOffset_Iu), /* 0x3130:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__CurrentOffset_Iv), /* 0x3130:9*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg__CurrentOffset_Iw), /* 0x3130:10*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIdqAct[0]), /* 0x3131:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIdqAct[1]), /* 0x3131:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIdqRef[0]), /* 0x3132:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIdqRef[1]), /* 0x3132:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUdq[0]), /* 0x3133:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUdq[1]), /* 0x3133:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUdq[2]), /* 0x3133:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUdq[3]), /* 0x3133:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIabc[0]), /* 0x3134:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIabc[1]), /* 0x3134:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIabc[2]), /* 0x3134:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUabc[0]), /* 0x3135:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUabc[1]), /* 0x3135:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusUabc[2]), /* 0x3135:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusRpm), /* 0x3136:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusTheta), /* 0x3137:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusModFactor), /* 0x3138:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusId[0]), /* 0x3139:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusId[1]), /* 0x3139:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusId[2]), /* 0x3139:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusId[3]), /* 0x3139:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_StatusIqmax), /* 0x313A:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_Calculated__Bandwith), /* 0x313B:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_Calculated__DaxisPgain), /* 0x313B:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_Calculated__DaxisIgain), /* 0x313B:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_Calculated__QaxisPgain), /* 0x313B:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_CurrentReg_Calculated__QaxisIgain), /* 0x313B:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak__TargetModulationFactor), /* 0x3140:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak__Pgain), /* 0x3140:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak__Igain), /* 0x3140:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak__LPFtimeConstant), /* 0x3140:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak_StatusUs[0]), /* 0x3141:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak_StatusUs[1]), /* 0x3141:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak_StatusUs[2]), /* 0x3141:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak_StatusId[0]), /* 0x3142:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_FieldWeak_StatusId[1]), /* 0x3142:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__Pgain), /* 0x3150:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__Igain), /* 0x3150:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__Dgain), /* 0x3150:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__DpartLPF), /* 0x3150:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__StartIq), /* 0x3150:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__PgainHigh), /* 0x3150:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__IgainHigh), /* 0x3150:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__Pgain_LoadMeas), /* 0x3150:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__Igain_LoadMeas), /* 0x3150:9*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__SpeedLimFirst), /* 0x3150:10*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg__SpeedLimSecond), /* 0x3150:11*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusRPM[0]), /* 0x3151:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusRPM[1]), /* 0x3151:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusRPM[2]), /* 0x3151:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[0]), /* 0x3152:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[1]), /* 0x3152:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[2]), /* 0x3152:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[3]), /* 0x3152:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[4]), /* 0x3152:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[5]), /* 0x3152:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_VelocityReg_StatusIq[6]), /* 0x3152:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__TransitionTimeToClose), /* 0x3160:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__TransitionTimeToOpen), /* 0x3160:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ObserverK1), /* 0x3160:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ObserverK2), /* 0x3160:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__PLL_Bandwith), /* 0x3160:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__PLL_BandwithReduction), /* 0x3160:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__OmegaPostLPFtimeConstant), /* 0x3160:9*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__CalculationDelay), /* 0x3160:10*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidPLLerrMax), /* 0x3160:11*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidBEMFratioMin), /* 0x3160:12*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidBEMFratioMax), /* 0x3160:13*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__EndRPM), /* 0x3160:14*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidMaxRPM), /* 0x3160:15*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidMinRPM), /* 0x3160:16*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__MaxRPM), /* 0x3160:17*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__ValidLPFtimeConstant), /* 0x3160:18*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__MinTimeInClose), /* 0x3160:19*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__PLLBWSpeedSwitch), /* 0x3160:20*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless__TimeInHoldState), /* 0x3160:21*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabEst[0]), /* 0x3163:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabEst[1]), /* 0x3163:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabErr[0]), /* 0x3164:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabErr[1]), /* 0x3164:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_EabEst[0]), /* 0x3165:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_EabEst[1]), /* 0x3165:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_Theta[0]), /* 0x3166:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_Theta[1]), /* 0x3166:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_Theta[2]), /* 0x3166:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_PLL_Err), /* 0x3167:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_RPM[0]), /* 0x3168:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_RPM[1]), /* 0x3168:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_CriteriaPLL), /* 0x3169:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_CriteriaBEMF), /* 0x316A:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_SensorlessCalculatedGain__ObserverF), /* 0x316C:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_SensorlessCalculatedGain__K1), /* 0x316C:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_SensorlessCalculatedGain__K2), /* 0x316C:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabIn[0]), /* 0x316D:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_IabIn[1]), /* 0x316D:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_UabIn[0]), /* 0x316E:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Sensorless_StatusBEMF_UabIn[1]), /* 0x316E:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_OpenLoop__Id), /* 0x3170:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_OpenLoop__RampIdRate), /* 0x3170:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_OpenLoop__RampRPMrate), /* 0x3170:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_OpenLoop_StatusRPMopen), /* 0x3171:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_ALTTestMode__RPM), /* 0x31AD:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_ALTTestMode_StatusUdq[0]), /* 0x31AF:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_ALTTestMode_StatusUdq[1]), /* 0x31AF:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Power__CompensationStart), /* 0x31B0:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Power__CompensationSlope), /* 0x31B0:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Power__CompensationOffset), /* 0x31B0:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Power__CompensatedPower), /* 0x31B1:0*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__Amplitude), /* 0x31D0:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__Offset), /* 0x31D0:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim1__Period), /* 0x31D0:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__Amplitude), /* 0x31D1:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__Offset), /* 0x31D1:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim2__Period), /* 0x31D1:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim_Status[0]), /* 0x31D4:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Stim_Status[1]), /* 0x31D4:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_AngleGenerator__RPM), /* 0x31DA:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_AngleGenerator__Offset), /* 0x31DA:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurment__LPF_TimeConst), /* 0x31F0:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurments__IphRms), /* 0x31F0:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurments__UphRms), /* 0x31F0:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurments__Pel), /* 0x31F0:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurments__RPM), /* 0x31F0:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(Ctrl_Measurment__Pel_LPF_TimeConst), /* 0x31F0:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(motor_control__dc_link), /* 0x5000:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(motor_control__DCLinkVoltage), /* 0x5000:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(motor_control__bridge_temp), /* 0x5000:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(motor_control__heatsink_temp), /* 0x5000:8*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(motor_control__internal_ntc_temp), /* 0x5000:9*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(performance__foc_loop), /* 0x5001:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(performance__velocity_loop), /* 0x5001:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__phase_overcurrent), /* 0x5003:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__overvoltage), /* 0x5003:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__undervoltage), /* 0x5003:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__bridge_overtemperature), /* 0x5003:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__phase_overcurrent_hw), /* 0x5003:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__phase_overvoltage_hw), /* 0x5003:6*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(protections__motor_overtemperature), /* 0x5003:7*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(test__DCLinkVoltageMin), /* 0x5006:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(test__DCLinkVoltageMax), /* 0x5006:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(test__DCLinkVoltageAvg), /* 0x5006:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__raw_inertia), /* 0x5010:1*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__raw_unbalance), /* 0x5010:2*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__raw_friction), /* 0x5010:3*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__overload), /* 0x5010:4*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__inertia), /* 0x5010:5*/
	(REAL32 CO_CONST_STORAGE_CLASS *)&(measurement__unbalance) /* 0x5010:6*/
};
VIS_STRING  od_vis_string[3] = {
	&(CO_vis_string_Manufacturer_Device_Name[0]), /* 0x1008:0*/
	&(CO_vis_string_Device_ID[0]), /* 0x2004:0*/
	&(CO_vis_string_Drive_manufacturer[0]) /* 0x6504:0*/
};
UNSIGNED32 od_vis_string_len[3] = {
	32ul,
	47ul,
	6ul 
};
CO_DOMAIN_PTR  *od_domain[2] = {
	&(CO_domain_Store_EDS),
	&(Oscilloscope_settings__Record)
};
UNSIGNED32 CO_STORAGE_CLASS od_domain_len[2] = {
	0ul,
	1ul 
};

CO_CONST CO_OBJECT_DESC_T od_description[CO_OBJ_DESC_CNT] CO_CONST_STORAGE_CLASS = {
	{ (UNSIGNED8)0u, CO_DTYPE_U32_CONST, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1000:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_EMCY, (UNSIGNED16)4097u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1001:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1003:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1003:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1003:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1003:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1003:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4099u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1003:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U32_SYNC, (UNSIGNED16)4101u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1005:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U32_SYNC, (UNSIGNED16)4102u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1006:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_VS_PTR, (UNSIGNED16)0u, CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x1008:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)3u, (UNSIGNED16)1u}, /* 0x100b:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1010:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_STORE, (UNSIGNED16)4112u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1010:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_STORE, (UNSIGNED16)4112u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1010:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_STORE, (UNSIGNED16)4112u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1010:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_STORE, (UNSIGNED16)4112u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1010:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1011:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_STORE, (UNSIGNED16)4113u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1011:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_STORE, (UNSIGNED16)4113u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1011:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_STORE, (UNSIGNED16)4113u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1011:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_STORE, (UNSIGNED16)4113u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1011:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_U32_EMCY, (UNSIGNED16)4116u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1014:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U16_EMCY, (UNSIGNED16)4117u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1015:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)5u}, /* 0x1016:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_ERRCTRL, (UNSIGNED16)4118u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1016:1*/
	{ (UNSIGNED8)0u, CO_DTYPE_U16_ERRCTRL, (UNSIGNED16)4119u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1017:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1018:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_PTR, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1018:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_PTR, (UNSIGNED16)5u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1018:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_PTR, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1018:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_DOMAIN, (UNSIGNED16)0u, CO_ATTR_READ, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x1021:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1029:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_ERRCTRL, (UNSIGNED16)4137u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x1029:1*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1200:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_SDO_SERVER, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1200:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_SDO_SERVER, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1200:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)6u, (UNSIGNED16)6u}, /* 0x1280:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_SDO_CLIENT, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1280:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_SDO_CLIENT, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1280:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_SDO_CLIENT, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)5u}, /* 0x1280:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)2u, (UNSIGNED16)7u, (UNSIGNED16)2u}, /* 0x1400:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1400:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_RPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)8u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1400:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_RPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1400:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_RPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1400:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_RPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1400:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)2u, (UNSIGNED16)7u, (UNSIGNED16)2u}, /* 0x1401:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1401:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_RPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)8u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1401:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_RPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1401:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_RPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1401:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_RPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1401:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)2u, (UNSIGNED16)7u, (UNSIGNED16)2u}, /* 0x1402:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1402:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_RPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)8u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1402:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_RPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1402:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_RPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1402:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_RPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1402:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)2u, (UNSIGNED16)7u, (UNSIGNED16)2u}, /* 0x1403:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1403:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_RPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)8u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1403:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_RPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1403:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_RPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1403:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_RPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1403:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1600:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_RMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1600:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1601:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_RMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1601:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1602:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_RMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1602:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1603:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_RMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)1u}, /* 0x1603:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)7u, (UNSIGNED16)9u}, /* 0x1800:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1800:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1800:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1800:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1800:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1800:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_TPDO, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1800:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)7u, (UNSIGNED16)9u}, /* 0x1801:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1801:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1801:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1801:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1801:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1801:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_TPDO, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1801:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)7u, (UNSIGNED16)9u}, /* 0x1802:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1802:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1802:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1802:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1802:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1802:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_TPDO, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1802:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)7u, (UNSIGNED16)9u}, /* 0x1803:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x1803:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1803:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1803:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1803:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U16_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1803:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_TPDO, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1803:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1a00:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_TMAP, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a00:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1a01:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_TMAP, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a01:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1a02:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_TMAP, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a02:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1a03:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_TMAP, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)6u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x1a03:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1f50:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_DOMAIN, (UNSIGNED16)0u, CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x1f50:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_DOMAIN, (UNSIGNED16)0u, CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x1f50:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1f51:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_VAR, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1f51:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_VAR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x1f51:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U16_PTR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2001:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_VS_PTR, (UNSIGNED16)1u, CO_ATTR_READ | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x2004:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)10u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)10u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)9u}, /* 0x2080:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x2080:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_PTR, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)5u, (UNSIGNED16)5u, (UNSIGNED16)2u}, /* 0x2080:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_PTR, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x2080:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_PTR, (UNSIGNED16)5u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)2u}, /* 0x2080:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U8_PTR, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_PTR, (UNSIGNED16)8u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_U32_PTR, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_U32_PTR, (UNSIGNED16)10u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_U8_PTR, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)11u}, /* 0x2080:11*/
	{ (UNSIGNED8)12u, CO_DTYPE_U32_PTR, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:12*/
	{ (UNSIGNED8)13u, CO_DTYPE_DOMAIN, (UNSIGNED16)1u, CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x2080:13*/
	{ (UNSIGNED8)14u, CO_DTYPE_U32_PTR, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:14*/
	{ (UNSIGNED8)15u, CO_DTYPE_U16_PTR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:15*/
	{ (UNSIGNED8)16u, CO_DTYPE_U8_PTR, (UNSIGNED16)8u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:16*/
	{ (UNSIGNED8)17u, CO_DTYPE_U8_PTR, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:17*/
	{ (UNSIGNED8)18u, CO_DTYPE_U8_PTR, (UNSIGNED16)10u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:18*/
	{ (UNSIGNED8)19u, CO_DTYPE_I16_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2080:19*/
	{ (UNSIGNED8)20u, CO_DTYPE_U8_PTR, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2080:20*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)13u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U16_PTR, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_PTR, (UNSIGNED16)14u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U8_PTR, (UNSIGNED16)15u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2081:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2082:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)16u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x2082:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_I8_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2082:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U32_PTR, (UNSIGNED16)13u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)1u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2082:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_I32_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x2082:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3100:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3100:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)7u, (UNSIGNED16)6u}, /* 0x3100:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_I32_PTR, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3100:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)12u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3101:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3101:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3101:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)5u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3101:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3101:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U8_PTR, (UNSIGNED16)17u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3101:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U8_PTR, (UNSIGNED16)18u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3101:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3101:7*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3104:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)8u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)10u}, /* 0x3104:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)11u, (UNSIGNED16)4u, (UNSIGNED16)10u}, /* 0x3104:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)10u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)12u, (UNSIGNED16)4u, (UNSIGNED16)10u}, /* 0x3104:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)10u}, /* 0x3104:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)13u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3104:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)13u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)15u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3104:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3105:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)14u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3105:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)15u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3105:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)16u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3105:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x310b:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)17u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)16u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x310b:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)18u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)18u, (UNSIGNED16)14u, (UNSIGNED16)17u}, /* 0x310b:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)19u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)19u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x310b:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)20u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)20u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x310b:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)21u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)21u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x310b:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)22u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)23u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x310b:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x310c:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)23u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)24u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x310c:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)24u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)25u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x310c:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x310d:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)25u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)26u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x310d:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)26u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)27u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x310d:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x310e:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)27u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)28u}, /* 0x310e:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)28u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)28u}, /* 0x310e:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)12u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x310f:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)29u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)29u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x310f:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)19u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x310f:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)30u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)30u, (UNSIGNED16)31u, (UNSIGNED16)32u}, /* 0x310f:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)31u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)33u, (UNSIGNED16)31u, (UNSIGNED16)32u}, /* 0x310f:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)32u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x310f:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)33u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x310f:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)34u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x310f:7*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3110:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)0u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3110:1*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)13u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)13u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3112:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)35u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)34u, (UNSIGNED16)4u, (UNSIGNED16)35u}, /* 0x3112:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)36u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)36u, (UNSIGNED16)4u, (UNSIGNED16)35u}, /* 0x3112:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)37u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)37u, (UNSIGNED16)4u, (UNSIGNED16)35u}, /* 0x3112:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)38u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)38u, (UNSIGNED16)4u, (UNSIGNED16)35u}, /* 0x3112:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)39u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)39u, (UNSIGNED16)4u, (UNSIGNED16)40u}, /* 0x3112:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)40u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)22u, (UNSIGNED16)4u, (UNSIGNED16)40u}, /* 0x3112:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)41u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)41u, (UNSIGNED16)4u, (UNSIGNED16)42u}, /* 0x3112:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)42u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)43u, (UNSIGNED16)4u, (UNSIGNED16)42u}, /* 0x3112:8*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3113:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I32_PTR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x3113:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)43u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3113:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_I32_PTR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x3113:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)44u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3113:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)45u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3113:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)14u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)14u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3120:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)46u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)44u, (UNSIGNED16)4u, (UNSIGNED16)28u}, /* 0x3120:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)47u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)45u, (UNSIGNED16)46u, (UNSIGNED16)4u}, /* 0x3120:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)48u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)47u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3120:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)49u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)48u, (UNSIGNED16)4u, (UNSIGNED16)49u}, /* 0x3120:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)50u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)50u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3120:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)51u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)51u, (UNSIGNED16)4u, (UNSIGNED16)52u}, /* 0x3120:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)52u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)53u, (UNSIGNED16)54u, (UNSIGNED16)4u}, /* 0x3120:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)53u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)55u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3120:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_R32_PTR, (UNSIGNED16)54u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)41u, (UNSIGNED16)4u, (UNSIGNED16)56u}, /* 0x3120:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_R32_PTR, (UNSIGNED16)55u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)23u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x3120:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_R32_PTR, (UNSIGNED16)56u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3120:11*/
	{ (UNSIGNED8)12u, CO_DTYPE_R32_PTR, (UNSIGNED16)57u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)57u, (UNSIGNED16)4u, (UNSIGNED16)39u}, /* 0x3120:12*/
	{ (UNSIGNED8)13u, CO_DTYPE_R32_PTR, (UNSIGNED16)58u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x3120:13*/
	{ (UNSIGNED8)14u, CO_DTYPE_R32_PTR, (UNSIGNED16)59u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3120:14*/
	{ (UNSIGNED8)15u, CO_DTYPE_R32_PTR, (UNSIGNED16)60u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)58u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3120:15*/
	{ (UNSIGNED8)16u, CO_DTYPE_R32_PTR, (UNSIGNED16)61u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)59u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3120:16*/
	{ (UNSIGNED8)17u, CO_DTYPE_U8_PTR, (UNSIGNED16)20u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x3120:17*/
	{ (UNSIGNED8)18u, CO_DTYPE_R32_PTR, (UNSIGNED16)62u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)60u, (UNSIGNED16)4u, (UNSIGNED16)61u}, /* 0x3120:18*/
	{ (UNSIGNED8)19u, CO_DTYPE_R32_PTR, (UNSIGNED16)63u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)60u, (UNSIGNED16)4u, (UNSIGNED16)61u}, /* 0x3120:19*/
	{ (UNSIGNED8)20u, CO_DTYPE_R32_PTR, (UNSIGNED16)64u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)60u, (UNSIGNED16)4u, (UNSIGNED16)62u}, /* 0x3120:20*/
	{ (UNSIGNED8)21u, CO_DTYPE_R32_PTR, (UNSIGNED16)65u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)47u, (UNSIGNED16)4u, (UNSIGNED16)63u}, /* 0x3120:21*/
	{ (UNSIGNED8)22u, CO_DTYPE_R32_PTR, (UNSIGNED16)66u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)4u, (UNSIGNED16)64u}, /* 0x3120:22*/
	{ (UNSIGNED8)23u, CO_DTYPE_R32_PTR, (UNSIGNED16)67u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)65u, (UNSIGNED16)4u, (UNSIGNED16)56u}, /* 0x3120:23*/
	{ (UNSIGNED8)24u, CO_DTYPE_R32_PTR, (UNSIGNED16)68u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)23u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x3120:24*/
	{ (UNSIGNED8)25u, CO_DTYPE_R32_PTR, (UNSIGNED16)69u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3120:25*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3121:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)70u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3121:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)71u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3121:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)72u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3121:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)73u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3122:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)74u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3123:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3124:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)75u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3124:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)76u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3124:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)21u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)22u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)23u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_PTR, (UNSIGNED16)24u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U8_PTR, (UNSIGNED16)25u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3125:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)77u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3127:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)15u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)15u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3130:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)78u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)66u, (UNSIGNED16)4u, (UNSIGNED16)67u}, /* 0x3130:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)26u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x3130:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)79u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)68u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3130:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)80u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)69u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3130:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)81u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)70u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3130:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)82u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)69u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3130:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)83u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)71u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x3130:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)84u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)72u, (UNSIGNED16)18u}, /* 0x3130:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_R32_PTR, (UNSIGNED16)85u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)73u, (UNSIGNED16)72u, (UNSIGNED16)18u}, /* 0x3130:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_R32_PTR, (UNSIGNED16)86u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)74u, (UNSIGNED16)72u, (UNSIGNED16)18u}, /* 0x3130:10*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3131:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)87u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3131:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)88u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3131:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3132:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)89u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3132:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)90u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3132:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3133:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)91u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3133:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)92u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3133:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)93u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3133:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)94u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3133:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3134:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)95u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3134:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)96u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3134:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)97u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3134:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3135:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)98u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3135:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)99u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3135:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)100u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3135:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)101u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3136:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)102u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3137:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)103u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3138:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3139:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)104u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3139:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)105u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3139:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)106u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3139:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)107u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3139:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)108u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x313a:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)2u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x313b:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)109u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)60u, (UNSIGNED16)14u, (UNSIGNED16)75u}, /* 0x313b:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)110u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x313b:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)111u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x313b:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)112u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x313b:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)113u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x313b:5*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3140:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)114u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)76u, (UNSIGNED16)13u, (UNSIGNED16)67u}, /* 0x3140:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)115u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)77u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3140:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)116u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)67u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3140:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)117u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)78u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x3140:4*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3141:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)118u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3141:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)119u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3141:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)120u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3141:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3142:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)121u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3142:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)122u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3142:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)11u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3150:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)123u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)59u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3150:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)124u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)79u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3150:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)125u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)80u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3150:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)126u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3150:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)127u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)14u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3150:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)128u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)81u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3150:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)129u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)82u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3150:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)130u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)79u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3150:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_R32_PTR, (UNSIGNED16)131u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)77u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3150:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_R32_PTR, (UNSIGNED16)132u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)49u, (UNSIGNED16)4u, (UNSIGNED16)64u}, /* 0x3150:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_R32_PTR, (UNSIGNED16)133u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)49u, (UNSIGNED16)4u, (UNSIGNED16)64u}, /* 0x3150:11*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3151:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)134u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3151:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)135u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3151:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)136u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3151:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)12u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3152:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)137u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)138u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)139u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)140u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)141u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)142u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)143u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3152:7*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_PTR, (UNSIGNED16)27u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3153:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_PTR, (UNSIGNED16)28u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3154:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)16u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)16u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3160:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)29u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x3160:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)30u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x3160:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)144u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)78u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)145u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)78u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)146u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)83u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3160:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)147u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)84u, (UNSIGNED16)4u, (UNSIGNED16)85u}, /* 0x3160:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)148u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)86u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3160:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)149u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)77u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_R32_PTR, (UNSIGNED16)150u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_R32_PTR, (UNSIGNED16)151u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)87u, (UNSIGNED16)88u, (UNSIGNED16)22u}, /* 0x3160:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_R32_PTR, (UNSIGNED16)152u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)89u, (UNSIGNED16)4u, (UNSIGNED16)39u}, /* 0x3160:11*/
	{ (UNSIGNED8)12u, CO_DTYPE_R32_PTR, (UNSIGNED16)153u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)23u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:12*/
	{ (UNSIGNED8)13u, CO_DTYPE_R32_PTR, (UNSIGNED16)154u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)18u, (UNSIGNED16)14u, (UNSIGNED16)17u}, /* 0x3160:13*/
	{ (UNSIGNED8)14u, CO_DTYPE_R32_PTR, (UNSIGNED16)155u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)36u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3160:14*/
	{ (UNSIGNED8)15u, CO_DTYPE_R32_PTR, (UNSIGNED16)156u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)90u, (UNSIGNED16)4u, (UNSIGNED16)75u}, /* 0x3160:15*/
	{ (UNSIGNED8)16u, CO_DTYPE_R32_PTR, (UNSIGNED16)157u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)91u, (UNSIGNED16)4u, (UNSIGNED16)75u}, /* 0x3160:16*/
	{ (UNSIGNED8)17u, CO_DTYPE_R32_PTR, (UNSIGNED16)158u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)92u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3160:17*/
	{ (UNSIGNED8)18u, CO_DTYPE_R32_PTR, (UNSIGNED16)159u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:18*/
	{ (UNSIGNED8)19u, CO_DTYPE_R32_PTR, (UNSIGNED16)160u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)23u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x3160:19*/
	{ (UNSIGNED8)20u, CO_DTYPE_R32_PTR, (UNSIGNED16)161u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)49u, (UNSIGNED16)4u, (UNSIGNED16)64u}, /* 0x3160:20*/
	{ (UNSIGNED8)21u, CO_DTYPE_R32_PTR, (UNSIGNED16)162u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)77u, (UNSIGNED16)4u, (UNSIGNED16)14u}, /* 0x3160:21*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3161:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I32_PTR, (UNSIGNED16)4u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x3161:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_I32_PTR, (UNSIGNED16)5u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x3161:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_I32_PTR, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x3161:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3163:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)163u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3163:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)164u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3163:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3164:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)165u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3164:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)166u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3164:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3165:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)167u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3165:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)168u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3165:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3166:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)169u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3166:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)170u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3166:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)171u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3166:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)172u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3167:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3168:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)173u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3168:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)174u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3168:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)175u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3169:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)176u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316a:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_I32_PTR, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x316b:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x316c:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)177u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)93u, (UNSIGNED16)4u, (UNSIGNED16)17u}, /* 0x316c:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)178u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316c:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)179u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316c:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x316d:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)180u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316d:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)181u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316d:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x316e:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)182u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316e:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)183u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x316e:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3170:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)184u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)94u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x3170:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)185u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)63u, (UNSIGNED16)4u, (UNSIGNED16)5u}, /* 0x3170:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)186u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)60u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x3170:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)187u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x3171:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3172:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)31u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x3172:1*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31ad:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)32u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31ad:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)188u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)95u, (UNSIGNED16)4u, (UNSIGNED16)75u}, /* 0x31ad:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31af:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)189u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31af:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)190u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31af:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31b0:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)191u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)40u, (UNSIGNED16)4u, (UNSIGNED16)95u}, /* 0x31b0:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)192u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)77u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x31b0:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)193u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)65u, (UNSIGNED16)4u, (UNSIGNED16)7u}, /* 0x31b0:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_R32_PTR, (UNSIGNED16)194u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31b1:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31d0:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I32_PTR, (UNSIGNED16)8u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)6u}, /* 0x31d0:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)33u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31d0:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)34u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31d0:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)195u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x31d0:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)196u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)31u, (UNSIGNED16)32u}, /* 0x31d0:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)197u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)14u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x31d0:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31d1:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I32_PTR, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)6u}, /* 0x31d1:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)35u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)3u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31d1:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)36u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31d1:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)198u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x31d1:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)199u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)31u, (UNSIGNED16)32u}, /* 0x31d1:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)200u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)14u, (UNSIGNED16)4u, (UNSIGNED16)32u}, /* 0x31d1:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31d4:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)201u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31d4:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)202u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31d4:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31da:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)37u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)3u}, /* 0x31da:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)203u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)31u, (UNSIGNED16)32u}, /* 0x31da:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)204u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)4u, (UNSIGNED16)96u, (UNSIGNED16)39u}, /* 0x31da:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31f0:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)205u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)97u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x31f0:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)206u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31f0:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)207u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31f0:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)208u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31f0:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)209u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x31f0:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)210u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)78u, (UNSIGNED16)4u, (UNSIGNED16)22u}, /* 0x31f0:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x31ff:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I32_PTR, (UNSIGNED16)10u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x31ff:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_I32_PTR, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x31ff:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3200:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)38u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3200:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)39u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3200:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)40u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x3200:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)11u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)11u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5000:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I8_PTR, (UNSIGNED16)1u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)3u, (UNSIGNED16)4u}, /* 0x5000:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_I8_PTR, (UNSIGNED16)2u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)5u}, /* 0x5000:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)41u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5000:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)211u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)98u, (UNSIGNED16)42u}, /* 0x5000:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U8_PTR, (UNSIGNED16)42u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5000:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)212u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5000:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)213u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5000:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_R32_PTR, (UNSIGNED16)214u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5000:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_R32_PTR, (UNSIGNED16)215u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5000:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_U8_PTR, (UNSIGNED16)43u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5000:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_U8_PTR, (UNSIGNED16)44u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5000:11*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5001:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)216u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)4u, (UNSIGNED16)99u}, /* 0x5001:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)217u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)4u, (UNSIGNED16)100u}, /* 0x5001:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)12u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)12u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5003:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)218u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)101u, (UNSIGNED16)0u, (UNSIGNED16)102u}, /* 0x5003:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)219u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)103u, (UNSIGNED16)0u, (UNSIGNED16)104u}, /* 0x5003:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)220u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)17u, (UNSIGNED16)105u, (UNSIGNED16)106u}, /* 0x5003:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)221u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)107u, (UNSIGNED16)0u, (UNSIGNED16)108u}, /* 0x5003:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)222u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)101u, (UNSIGNED16)0u, (UNSIGNED16)109u}, /* 0x5003:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)223u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)110u, (UNSIGNED16)0u, (UNSIGNED16)104u}, /* 0x5003:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_R32_PTR, (UNSIGNED16)224u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)56u, (UNSIGNED16)0u, (UNSIGNED16)111u}, /* 0x5003:7*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)7u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)7u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5004:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_U8_PTR, (UNSIGNED16)45u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5004:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U8_PTR, (UNSIGNED16)46u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5004:2*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)6u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)6u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5006:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)225u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)4u, (UNSIGNED16)112u}, /* 0x5006:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)226u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)4u, (UNSIGNED16)112u}, /* 0x5006:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)227u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5006:3*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)17u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)17u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_I8_PTR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)3u, (UNSIGNED16)6u}, /* 0x500a:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_U32_PTR, (UNSIGNED16)14u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_U8_PTR, (UNSIGNED16)47u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_U8_PTR, (UNSIGNED16)48u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_U32_PTR, (UNSIGNED16)15u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_U32_PTR, (UNSIGNED16)16u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:6*/
	{ (UNSIGNED8)7u, CO_DTYPE_U32_PTR, (UNSIGNED16)17u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:7*/
	{ (UNSIGNED8)8u, CO_DTYPE_U32_PTR, (UNSIGNED16)18u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:8*/
	{ (UNSIGNED8)9u, CO_DTYPE_U8_PTR, (UNSIGNED16)49u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:9*/
	{ (UNSIGNED8)10u, CO_DTYPE_U8_PTR, (UNSIGNED16)50u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:10*/
	{ (UNSIGNED8)11u, CO_DTYPE_U8_PTR, (UNSIGNED16)51u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:11*/
	{ (UNSIGNED8)12u, CO_DTYPE_U8_PTR, (UNSIGNED16)52u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x500a:12*/
	{ (UNSIGNED8)13u, CO_DTYPE_U32_PTR, (UNSIGNED16)19u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:13*/
	{ (UNSIGNED8)14u, CO_DTYPE_U32_PTR, (UNSIGNED16)20u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x500a:14*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_CONST, (UNSIGNED16)9u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_DEFVAL | CO_ATTR_LIMIT, (UNSIGNED16)9u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5010:0*/
	{ (UNSIGNED8)1u, CO_DTYPE_R32_PTR, (UNSIGNED16)228u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:1*/
	{ (UNSIGNED8)2u, CO_DTYPE_R32_PTR, (UNSIGNED16)229u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:2*/
	{ (UNSIGNED8)3u, CO_DTYPE_R32_PTR, (UNSIGNED16)230u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:3*/
	{ (UNSIGNED8)4u, CO_DTYPE_R32_PTR, (UNSIGNED16)231u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:4*/
	{ (UNSIGNED8)5u, CO_DTYPE_R32_PTR, (UNSIGNED16)232u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:5*/
	{ (UNSIGNED8)6u, CO_DTYPE_R32_PTR, (UNSIGNED16)233u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)8u, (UNSIGNED16)9u}, /* 0x5010:6*/
	{ (UNSIGNED8)0u, CO_DTYPE_U32_PTR, (UNSIGNED16)21u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_MAP_TR | CO_ATTR_LIMIT, (UNSIGNED16)0u, (UNSIGNED16)1u, (UNSIGNED16)2u}, /* 0x5100:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_U8_VAR, (UNSIGNED16)3u, CO_ATTR_NUM | CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_MAP_TR | CO_ATTR_MAP_REC | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)1u}, /* 0x5fff:0*/
	{ (UNSIGNED8)0u, CO_DTYPE_VS_PTR, (UNSIGNED16)2u, CO_ATTR_READ | CO_ATTR_WRITE | CO_ATTR_DEFVAL, (UNSIGNED16)0u, (UNSIGNED16)0u, (UNSIGNED16)0u}, /* 0x6504:0*/
};

/* object dictionary assignment */
CO_CONST CO_OD_ASSIGN_T od_assign[CO_OD_ASSIGN_CNT] CO_CONST_STORAGE_CLASS = {
	{ 0x1000u, 1u, 0u, CO_ODTYPE_VAR, 0u },
	{ 0x1001u, 1u, 0u, CO_ODTYPE_VAR, 1u },
	{ 0x1003u, 6u, 5u, CO_ODTYPE_ARRAY, 2u },
	{ 0x1005u, 1u, 0u, CO_ODTYPE_VAR, 8u },
	{ 0x1006u, 1u, 0u, CO_ODTYPE_VAR, 9u },
	{ 0x1008u, 1u, 0u, CO_ODTYPE_VAR, 10u },
	{ 0x100bu, 1u, 0u, CO_ODTYPE_VAR, 11u },
	{ 0x1010u, 5u, 4u, CO_ODTYPE_ARRAY, 12u },
	{ 0x1011u, 5u, 4u, CO_ODTYPE_ARRAY, 17u },
	{ 0x1014u, 1u, 0u, CO_ODTYPE_VAR, 22u },
	{ 0x1015u, 1u, 0u, CO_ODTYPE_VAR, 23u },
	{ 0x1016u, 2u, 1u, CO_ODTYPE_ARRAY, 24u },
	{ 0x1017u, 1u, 0u, CO_ODTYPE_VAR, 26u },
	{ 0x1018u, 4u, 3u, CO_ODTYPE_ARRAY, 27u },
	{ 0x1021u, 1u, 0u, CO_ODTYPE_VAR, 31u },
	{ 0x1029u, 2u, 1u, CO_ODTYPE_ARRAY, 32u },
	{ 0x1200u, 3u, 2u, CO_ODTYPE_ARRAY, 34u },
	{ 0x1280u, 4u, 3u, CO_ODTYPE_STRUCT, 37u },
	{ 0x1400u, 6u, 5u, CO_ODTYPE_STRUCT, 41u },
	{ 0x1401u, 6u, 5u, CO_ODTYPE_STRUCT, 47u },
	{ 0x1402u, 6u, 5u, CO_ODTYPE_STRUCT, 53u },
	{ 0x1403u, 6u, 5u, CO_ODTYPE_STRUCT, 59u },
	{ 0x1600u, 9u, 8u, CO_ODTYPE_ARRAY, 65u },
	{ 0x1601u, 9u, 8u, CO_ODTYPE_ARRAY, 74u },
	{ 0x1602u, 9u, 8u, CO_ODTYPE_ARRAY, 83u },
	{ 0x1603u, 9u, 8u, CO_ODTYPE_ARRAY, 92u },
	{ 0x1800u, 7u, 6u, CO_ODTYPE_STRUCT, 101u },
	{ 0x1801u, 7u, 6u, CO_ODTYPE_STRUCT, 108u },
	{ 0x1802u, 7u, 6u, CO_ODTYPE_STRUCT, 115u },
	{ 0x1803u, 7u, 6u, CO_ODTYPE_STRUCT, 122u },
	{ 0x1a00u, 9u, 8u, CO_ODTYPE_ARRAY, 129u },
	{ 0x1a01u, 9u, 8u, CO_ODTYPE_ARRAY, 138u },
	{ 0x1a02u, 9u, 8u, CO_ODTYPE_ARRAY, 147u },
	{ 0x1a03u, 9u, 8u, CO_ODTYPE_ARRAY, 156u },
	{ 0x1f50u, 3u, 2u, CO_ODTYPE_ARRAY, 165u },
	{ 0x1f51u, 3u, 2u, CO_ODTYPE_ARRAY, 168u },
	{ 0x2001u, 1u, 0u, CO_ODTYPE_VAR, 171u },
	{ 0x2004u, 1u, 0u, CO_ODTYPE_VAR, 172u },
	{ 0x2080u, 21u, 20u, CO_ODTYPE_STRUCT, 173u },
	{ 0x2081u, 6u, 5u, CO_ODTYPE_STRUCT, 194u },
	{ 0x2082u, 5u, 4u, CO_ODTYPE_STRUCT, 200u },
	{ 0x3100u, 4u, 3u, CO_ODTYPE_STRUCT, 205u },
	{ 0x3101u, 8u, 7u, CO_ODTYPE_STRUCT, 209u },
	{ 0x3104u, 7u, 6u, CO_ODTYPE_ARRAY, 217u },
	{ 0x3105u, 4u, 3u, CO_ODTYPE_ARRAY, 224u },
	{ 0x310bu, 7u, 6u, CO_ODTYPE_ARRAY, 228u },
	{ 0x310cu, 3u, 2u, CO_ODTYPE_ARRAY, 235u },
	{ 0x310du, 3u, 2u, CO_ODTYPE_ARRAY, 238u },
	{ 0x310eu, 3u, 2u, CO_ODTYPE_ARRAY, 241u },
	{ 0x310fu, 8u, 7u, CO_ODTYPE_STRUCT, 244u },
	{ 0x3110u, 2u, 1u, CO_ODTYPE_ARRAY, 252u },
	{ 0x3112u, 9u, 8u, CO_ODTYPE_ARRAY, 254u },
	{ 0x3113u, 6u, 5u, CO_ODTYPE_STRUCT, 263u },
	{ 0x3120u, 26u, 25u, CO_ODTYPE_STRUCT, 269u },
	{ 0x3121u, 4u, 3u, CO_ODTYPE_ARRAY, 295u },
	{ 0x3122u, 1u, 0u, CO_ODTYPE_VAR, 299u },
	{ 0x3123u, 1u, 0u, CO_ODTYPE_VAR, 300u },
	{ 0x3124u, 3u, 2u, CO_ODTYPE_ARRAY, 301u },
	{ 0x3125u, 6u, 5u, CO_ODTYPE_ARRAY, 304u },
	{ 0x3127u, 1u, 0u, CO_ODTYPE_VAR, 310u },
	{ 0x3130u, 11u, 10u, CO_ODTYPE_STRUCT, 311u },
	{ 0x3131u, 3u, 2u, CO_ODTYPE_ARRAY, 322u },
	{ 0x3132u, 3u, 2u, CO_ODTYPE_ARRAY, 325u },
	{ 0x3133u, 5u, 4u, CO_ODTYPE_ARRAY, 328u },
	{ 0x3134u, 4u, 3u, CO_ODTYPE_ARRAY, 333u },
	{ 0x3135u, 4u, 3u, CO_ODTYPE_ARRAY, 337u },
	{ 0x3136u, 1u, 0u, CO_ODTYPE_VAR, 341u },
	{ 0x3137u, 1u, 0u, CO_ODTYPE_VAR, 342u },
	{ 0x3138u, 1u, 0u, CO_ODTYPE_VAR, 343u },
	{ 0x3139u, 5u, 4u, CO_ODTYPE_ARRAY, 344u },
	{ 0x313au, 1u, 0u, CO_ODTYPE_VAR, 349u },
	{ 0x313bu, 6u, 5u, CO_ODTYPE_ARRAY, 350u },
	{ 0x3140u, 5u, 4u, CO_ODTYPE_ARRAY, 356u },
	{ 0x3141u, 4u, 3u, CO_ODTYPE_ARRAY, 361u },
	{ 0x3142u, 3u, 2u, CO_ODTYPE_ARRAY, 365u },
	{ 0x3150u, 12u, 11u, CO_ODTYPE_ARRAY, 368u },
	{ 0x3151u, 4u, 3u, CO_ODTYPE_ARRAY, 380u },
	{ 0x3152u, 8u, 7u, CO_ODTYPE_ARRAY, 384u },
	{ 0x3153u, 1u, 0u, CO_ODTYPE_VAR, 392u },
	{ 0x3154u, 1u, 0u, CO_ODTYPE_VAR, 393u },
	{ 0x3160u, 22u, 21u, CO_ODTYPE_STRUCT, 394u },
	{ 0x3161u, 4u, 3u, CO_ODTYPE_ARRAY, 416u },
	{ 0x3163u, 3u, 2u, CO_ODTYPE_ARRAY, 420u },
	{ 0x3164u, 3u, 2u, CO_ODTYPE_ARRAY, 423u },
	{ 0x3165u, 3u, 2u, CO_ODTYPE_ARRAY, 426u },
	{ 0x3166u, 4u, 3u, CO_ODTYPE_ARRAY, 429u },
	{ 0x3167u, 1u, 0u, CO_ODTYPE_VAR, 433u },
	{ 0x3168u, 3u, 2u, CO_ODTYPE_ARRAY, 434u },
	{ 0x3169u, 1u, 0u, CO_ODTYPE_VAR, 437u },
	{ 0x316au, 1u, 0u, CO_ODTYPE_VAR, 438u },
	{ 0x316bu, 1u, 0u, CO_ODTYPE_VAR, 439u },
	{ 0x316cu, 4u, 3u, CO_ODTYPE_ARRAY, 440u },
	{ 0x316du, 3u, 2u, CO_ODTYPE_ARRAY, 444u },
	{ 0x316eu, 3u, 2u, CO_ODTYPE_ARRAY, 447u },
	{ 0x3170u, 4u, 3u, CO_ODTYPE_ARRAY, 450u },
	{ 0x3171u, 1u, 0u, CO_ODTYPE_VAR, 454u },
	{ 0x3172u, 2u, 1u, CO_ODTYPE_ARRAY, 455u },
	{ 0x31adu, 3u, 2u, CO_ODTYPE_STRUCT, 457u },
	{ 0x31afu, 3u, 2u, CO_ODTYPE_ARRAY, 460u },
	{ 0x31b0u, 4u, 3u, CO_ODTYPE_ARRAY, 463u },
	{ 0x31b1u, 1u, 0u, CO_ODTYPE_VAR, 467u },
	{ 0x31d0u, 7u, 6u, CO_ODTYPE_STRUCT, 468u },
	{ 0x31d1u, 7u, 6u, CO_ODTYPE_STRUCT, 475u },
	{ 0x31d4u, 3u, 2u, CO_ODTYPE_ARRAY, 482u },
	{ 0x31dau, 4u, 3u, CO_ODTYPE_STRUCT, 485u },
	{ 0x31f0u, 7u, 6u, CO_ODTYPE_ARRAY, 489u },
	{ 0x31ffu, 3u, 2u, CO_ODTYPE_ARRAY, 496u },
	{ 0x3200u, 4u, 3u, CO_ODTYPE_ARRAY, 499u },
	{ 0x5000u, 12u, 11u, CO_ODTYPE_STRUCT, 503u },
	{ 0x5001u, 3u, 2u, CO_ODTYPE_ARRAY, 515u },
	{ 0x5003u, 8u, 7u, CO_ODTYPE_ARRAY, 518u },
	{ 0x5004u, 3u, 2u, CO_ODTYPE_ARRAY, 526u },
	{ 0x5006u, 4u, 3u, CO_ODTYPE_ARRAY, 529u },
	{ 0x500au, 15u, 14u, CO_ODTYPE_STRUCT, 533u },
	{ 0x5010u, 7u, 6u, CO_ODTYPE_ARRAY, 548u },
	{ 0x5100u, 1u, 0u, CO_ODTYPE_VAR, 555u },
	{ 0x5fffu, 1u, 0u, CO_ODTYPE_VAR, 556u },
	{ 0x6504u, 1u, 0u, CO_ODTYPE_VAR, 557u },
};

UNSIGNED8 co_vis_string_table_len (void)
{
	return 3;
}

UNSIGNED8 co_od_domain_table_len (void)
{
	return 2;
}
CO_CONST CO_OD_DATA_VARIABLES_T od_data_variables = {
	&od_const_u8[0],
	&od_const_u16[0],
	&od_const_u32[0],
	&od_const_r32[0],
	&od_u8[0],
	&od_u16[0],
	&od_u32[0],
	NULL,
	&od_const_i8[0],
	&od_const_i16[0],
	&od_const_i32[0],
	NULL,
	NULL,
	NULL,
	&od_ptr_u8[0],
	&od_ptr_u16[0],
	&od_ptr_u32[0],
	&od_ptr_i8[0],
	&od_ptr_i16[0],
	&od_ptr_i32[0],
	&od_ptr_r32[0],
	NULL,
	NULL,
	&od_vis_string[0],
	&od_vis_string_len[0],
	NULL,
	NULL,
	&od_domain[0],
	&od_domain_len[0],
};
