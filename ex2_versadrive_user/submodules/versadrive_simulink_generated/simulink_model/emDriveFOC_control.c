/*
 * File: emDriveFOC_control.c
 *
 * Code generated for Simulink model 'emDriveFOC_control'.
 *
 * Model version                  : 12.13
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Mon Nov 27 10:41:19 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#include "emDriveFOC_control.h"
#include "rtwtypes.h"
#include <math.h>
#include "emDriveFOC_control_types.h"
#include "emDriveFOC_control_global.h"

/* Named constants for Chart: '<S118>/SenzorlessStateDigram' */
#define emDriv_IN_TransitionToCloseLoop ((uint8_T)5U)
#define emDriveFOC_c_IN_NO_ACTIVE_CHILD ((uint8_T)0U)
#define emDriveFOC_control_IN_Align    ((uint8_T)1U)
#define emDriveFOC_control_IN_CloseLoop ((uint8_T)2U)
#define emDriveFOC_control_IN_Error    ((uint8_T)1U)
#define emDriveFOC_control_IN_Hold     ((uint8_T)3U)
#define emDriveFOC_control_IN_OpenLoop ((uint8_T)4U)
#define emDriveFOC_control_IN_Run      ((uint8_T)2U)
#define emDriveFOC_control_IN_Stop     ((uint8_T)3U)
#define emDrive_IN_TransitionToOpenLoop ((uint8_T)6U)

/* Named constants for Chart: '<S242>/PowerLimitStateMachine' */
#define emDr_IN_PowerLimitTempReduction ((uint8_T)3U)
#define emDr_IN_PowerLimitTimeReduction ((uint8_T)4U)
#define emDriveFOC__IN_PowerLimitNormal ((uint8_T)2U)
#define emDriveFOC_co_IN_PowerLimitInit ((uint8_T)1U)

/* user code (top of source file) */
#include "emDriveFOC_control_specific.h"

/* Exported data definition */

/* FLOW2 exported signal list */

/*
   @@ SYMBOL = Ctrl_ALTTestMode_StatusUdq
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Udq reference values during ALT test [V]"
   @@ GROUP = Ctrl | ALTTestMode
   @@ DIMENSION = 1 2
   @@ UNIT = "V"
   @! CAN_INDEX = 0x31AF
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_ALTTestMode_StatusUdq[2];/* '<S96>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_Calculated__DaxisIgain
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated D current regulator I gain [V/As]"
   @@ GROUP = Ctrl | CurrentReg | Calculated
   @@ DIMENSION = 1 1
   @@ UNIT = "V/As"
   @! CAN_INDEX = 0x313B
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_Calculated__DaxisIgain;/* '<S262>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_Calculated__DaxisPgain
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated D current regulator P gain [V/A]"
   @@ GROUP = Ctrl | CurrentReg | Calculated
   @@ DIMENSION = 1 1
   @@ UNIT = "V/A"
   @! CAN_INDEX = 0x313B
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_Calculated__DaxisPgain;/* '<S261>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_Calculated__QaxisIgain
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated  Q current regulator I gain [V/As]"
   @@ GROUP = Ctrl | CurrentReg | Calculated
   @@ DIMENSION = 1 1
   @@ UNIT = "V/As"
   @! CAN_INDEX = 0x313B
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_Calculated__QaxisIgain;/* '<S264>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_Calculated__QaxisPgain
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated Q current regulator P gain [V/A]"
   @@ GROUP = Ctrl | CurrentReg | Calculated
   @@ DIMENSION = 1 1
   @@ UNIT = "V/A"
   @! CAN_INDEX = 0x313B
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_Calculated__QaxisPgain;/* '<S263>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusIabc
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Phase currents after removing DC offset and applied reverse. @1-> Ia, @2-> Ib, @3-> Ic [Ap]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 3
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3134
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusIabc[3];/* '<S43>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusId
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Sources of Id reference current. @1-> IdExternal, @2->IdMTPA, @3->FieldWeakening, @4->IqRefReduction, This id reduction of Iq because FW in MTPV region. Active if FW calculated Id is lower as minimum Id (negative) [Ap]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 4
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3139
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusId[4];/* '<S69>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusIdqAct
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Current PI regulator @1->Id_act, @2->Iq_act [Ap]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 2
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3131
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusIdqAct[2];/* '<S88>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusIdqRef
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Current PI regulator @1->Id_ref, @2->Iq_ref [Ap]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 2
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3132
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusIdqRef[2];/* '<S86>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusIqmax
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Maximum avaliable Iq calculated from maximum Is and current Id [Ap]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x313A
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusIqmax;/* '<S80>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusModFactor
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Modulation factor []"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3138
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusModFactor;/* '<S68>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusRpm
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Actual  RPM [rpm]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3136
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusRpm;/* '<S60>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusTheta
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Electrical angle used in FOC [deg]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x3137
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusTheta;/* '<S59>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusUabc
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Phase voltages. Including third harmonic injection but not deat time and reverse. @1-> Ua, @2-> Ub, @3-> Uc [Vp]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 3
   @@ UNIT = "Vp"
   @! CAN_INDEX = 0x3135
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusUabc[3];/* '<S105>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_StatusUdq
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Current PI regulator  output  and decoupling @1->UdPIout, @2->UqPI_out, @3->UdDecoupling, @4->UqDecoupling, [Vp]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 4
   @@ UNIT = "Vp"
   @! CAN_INDEX = 0x3133
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_StatusUdq[4];/* '<S87>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Doc_FW__Date
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "FW date for core FOC algortihm [YYYYMMDD]"
   @@ GROUP = Ctrl | Doc | FW
   @@ DIMENSION = 1 1
   @@ UNIT = "YYYYMMDD"
   @! CAN_INDEX = 0x31FF
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile int32_T Ctrl_Doc_FW__Date;    /* '<Root>/FW_date' */

/*
   @@ SYMBOL = Ctrl_Doc_FW__Version
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "FW version for core FOC algortihm []"
   @@ GROUP = Ctrl | Doc | FW
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31FF
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @@ END
 */
volatile int32_T Ctrl_Doc_FW__Version; /* '<Root>/Add' */

/*
   @@ SYMBOL = Ctrl_Doc_version
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "FOC control algorithm version: @1 -> major, @2 -> minor, @3 -> patch []"
   @@ GROUP = Ctrl | Doc
   @@ DIMENSION = 1 3
   @@ UNIT = ""
   @! CAN_INDEX = 0x3200
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile uint8_T Ctrl_Doc_version[3];  /* '<S8>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_FieldWeak_StatusId
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated Id current.  @1-> IdOut, this goes to FOC, @2->IdPI, output of regulator [Ap]"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 2
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3142
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_FieldWeak_StatusId[2];/* '<S75>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_FieldWeak_StatusUs
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Phase voltage (vector sum of Ud and Uq). @1->UsAct, @2->UsTarget, @3->UsErr - negative if field weakening is required [Vp]"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 3
   @@ UNIT = "Vp"
   @! CAN_INDEX = 0x3141
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_FieldWeak_StatusUs[3];/* '<S76>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__Iq_foc
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Iq reference to FOC loop [Ap]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Gen_Stat__Iq_foc;/* '<S221>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__MotorWireTemp
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated motor wire temperature based on thermal loses in stator. Thermal model result is replaced with Ctrl_Gen_Stat__TemperatureFromR if Ctrl_Motor__TempFromRenabled is 1 [degC]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 1
   @@ END
 */
volatile real32_T Ctrl_Gen_Stat__MotorWireTemp;/* '<S234>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__RPMref
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "External  reference RPM [rpm]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Gen_Stat__RPMref;/* '<S7>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__RunStopIn
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "RunStop commanded value [bit]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @@ END
 */
volatile boolean_T Ctrl_Gen_Stat__RunStopIn;/* '<S10>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__RunStopOut
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "RunStop returned value. Will be stop, if sensorless is in error or Bridge is overheated [bit]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @@ END
 */
volatile boolean_T Ctrl_Gen_Stat__RunStopOut;/* '<S9>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__TemperatureFromR
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated motor temperature from resistance [W]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Gen_Stat__TemperatureFromR;/* '<S277>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Gen_Stat__Udc
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Udc [V]"
   @@ GROUP = Ctrl | Gen | Stat
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x3101
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Gen_Stat__Udc;  /* '<S222>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusDisIntegration
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "Disable integration of velocity regulator and stop RPM ramp.  @1->Power_limit, @2->Udc_limit,  @3-> MTPV_avtive,  @4-> Disable_positve_ramp,  @5->Disable_negative_ramp []"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 5
   @@ UNIT = ""
   @! CAN_INDEX = 0x3125
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile boolean_T Ctrl_Limits_StatusDisIntegration[5];/* '<S292>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusIsMax
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Limited Is  maximal as result of all limits [Ap]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3122
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Limits_StatusIsMax;/* '<S233>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusLim
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Limiting fuctions outpuits: Limiting fuctions outputs: @1->TotalIsLimit = min of all limits, @2-> MotorTemperatureLimit,@3->BridgeTemperatureLimit [1-0]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 3
   @@ UNIT = "1-0"
   @! CAN_INDEX = 0x3121
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Limits_StatusLim[3];/* '<S232>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusRPMramp
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "RPM rate limit (rising and falling): @1-> RPMin, @2->RPMout ,output of rate limiter [rpm]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 2
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3124
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Limits_StatusRPMramp[2];/* '<S290>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusRampIn
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Ramp in []"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3127
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Limits_StatusRampIn;/* '<S291>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Limits_StatusUdcMaxToIq
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "How much Iq has been increased to prevent regeneration and DC overvoltege [A]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3123
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Limits_StatusUdcMaxToIq;/* '<S225>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Measurments__IphRms
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Phase current rms [Arms]"
   @@ GROUP = Ctrl | Measurments
   @@ DIMENSION = 1 1
   @@ UNIT = "Arms"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Measurments__IphRms;/* '<S274>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Measurments__Pel
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Motor electrical power [W]"
   @@ GROUP = Ctrl | Measurments
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Measurments__Pel;/* '<S275>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Measurments__RPM
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "MotorRPM [RPM]"
   @@ GROUP = Ctrl | Measurments
   @@ DIMENSION = 1 1
   @@ UNIT = "RPM"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Measurments__RPM;/* '<S276>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Measurments__UphRms
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Phase voltage rms [Vrms]"
   @@ GROUP = Ctrl | Measurments
   @@ DIMENSION = 1 1
   @@ UNIT = "Vrms"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Measurments__UphRms;/* '<S273>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Mod_StatusPWM
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "PWM factors including dead time, limit 0-1 and reverse phase 2-3. @1-> PWMa, @2-> PWMb, @3-> PWMc []"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 3
   @@ UNIT = ""
   @! CAN_INDEX = 0x3105
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Mod_StatusPWM[3];/* '<S42>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Motor__CopperLoss
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Motor copper loses [W]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 2
   @@ END
 */
volatile real32_T Ctrl_Motor__CopperLoss;/* '<S248>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Motor__IronLoss
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Motor iron losses [W]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 2
   @@ END
 */
volatile real32_T Ctrl_Motor__IronLoss;/* '<S249>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Motor__TotalLoss
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Total motor losses [W]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 2
   @@ END
 */
volatile real32_T Ctrl_Motor__TotalLoss;/* '<S250>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_OpenLoop_StatusRPMopen
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "RPM open loop [rpm]"
   @@ GROUP = Ctrl | OpenLoop
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3171
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_OpenLoop_StatusRPMopen;/* '<S187>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__Counter
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Power limit counter value []"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3113
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @@ END
 */
volatile int32_T Ctrl_PowerLimit__Counter;/* '<S242>/PowerLimitStateMachine' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__LimitStatus
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Current power limit status. [W]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3113
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__LimitStatus;/* '<S242>/PowerLimitStateMachine' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__State
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Power limit state machine state []"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3113
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @@ END
 */
volatile int32_T Ctrl_PowerLimit__State;/* '<S242>/PowerLimitStateMachine' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__StatusActive
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Power limit output in range [0, 1]. [1]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "1"
   @! CAN_INDEX = 0x3113
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__StatusActive;/* '<S231>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__StatusLimitIq
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Reference value of Iq caluclated from power limit. [A]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3113
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__StatusLimitIq;/* '<S223>/Product4' */

/*
   @@ SYMBOL = Ctrl_Power__CompensatedPower
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Compensated electrical power. [W]"
   @@ GROUP = Ctrl | Power
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x31B1
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Power__CompensatedPower;/* '<S223>/Switch1' */

/*
   @@ SYMBOL = Ctrl_SensorlessCalculatedGain__K1
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated K1 gain []"
   @@ GROUP = Ctrl | SensorlessCalculatedGain
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x316C
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_SensorlessCalculatedGain__K1;/* '<S265>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_SensorlessCalculatedGain__K2
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Calculated K2 gain []"
   @@ GROUP = Ctrl | SensorlessCalculatedGain
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x316C
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_SensorlessCalculatedGain__K2;/* '<S266>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_CriteriaBEMF
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "BEMF ratio filtered []"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x316A
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_CriteriaBEMF;/* '<S164>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_CriteriaFlags
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Error bit flags None (0), Err_PLL (1), Err_BEMF (2), Err_Omega (4), Err_RPM_Polarity (8),No_SensorLess (16) []"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x316B
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile int32_T Ctrl_Sensorless_StatusBEMF_CriteriaFlags;/* '<S165>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_CriteriaPLL
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "absolute low pass filtered PLL error [deg]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 1
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x3169
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_CriteriaPLL;/* '<S163>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_EabEst
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Estimated EBMF voltage: @1->EaEst, @2->EbEst [Vp]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "Vp"
   @! CAN_INDEX = 0x3165
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_EabEst[2];/* '<S151>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_IabErr
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Iab estimator error [Ap]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3164
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_IabErr[2];/* '<S153>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_IabEst
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Est. Iab: @1-> IaEst,  @2-> IbEst [Ap]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3163
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_IabEst[2];/* '<S152>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_IabIn
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "In. Iab: @1-> IaIn,  @2-> IbIn [A]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "A"
   @! CAN_INDEX = 0x316D
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_IabIn[2];/* '<S133>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_PLL_Err
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Error at input of angle tracking PLL [deg]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF | PLL
   @@ DIMENSION = 1 1
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x3167
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_PLL_Err;/* '<S166>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_RPM
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "RPM: @1-> RPM_BEMF, output of BEMF observer and quadrature PLL , @2-> RPM_ref from external sensor [rpm]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3168
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_RPM[2];/* '<S155>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_Theta
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Angles: @1->BEMF_Theta_PLL, @2->Theta_ref from external sensor, @3->BEMF_Theta_error [deg]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 3
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x3166
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_Theta[3];/* '<S154>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_StatusBEMF_UabIn
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "In. Uab: @1-> UaIn,  @2-> UbIn [V]"
   @@ GROUP = Ctrl | Sensorless | StatusBEMF
   @@ DIMENSION = 1 2
   @@ UNIT = "V"
   @! CAN_INDEX = 0x316E
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 3
   @@ END
 */
volatile real32_T Ctrl_Sensorless_StatusBEMF_UabIn[2];/* '<S134>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_Status__LastNonErrorState
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Last state before entering Error or Stop state. Same neumeration as in Ctrl_Sensorless_Status__State []"
   @@ GROUP = Ctrl | Sensorless | Status
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3161
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 3
   @@ END
 */
volatile int32_T Ctrl_Sensorless_Status__LastNonErrorState;/* '<S188>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_Status__LatchedCriteriaFlags
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Latched BEMF criteria flags when state machine has entered Error or Stop state. Same value as Ctrl_Sensorless_StatusBEMF_CriteriaFlags []"
   @@ GROUP = Ctrl | Sensorless | Status
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3161
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 3
   @@ END
 */
volatile int32_T Ctrl_Sensorless_Status__LatchedCriteriaFlags;/* '<S189>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Sensorless_Status__State
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = SLONG
   @@ DESCRIPTION = "Indicates state in which is currently sensorless - as in enumStateSenless []"
   @@ GROUP = Ctrl | Sensorless | Status
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3161
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @@ END
 */
volatile int32_T Ctrl_Sensorless_Status__State;/* '<S186>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_Stim_Status
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Stimuli outputs @1->Stim1out ,@2->Stim2out, []"
   @@ GROUP = Ctrl | Stim
   @@ DIMENSION = 1 2
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D4
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_Stim_Status[2]; /* '<S296>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_VelocityReg_StatusDissableIntegration
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "Dissable integration. True in case, that Uq from current regulator is saturated and we have max torue [bit]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3153
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile boolean_T Ctrl_VelocityReg_StatusDissableIntegration;/* '<S304>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_VelocityReg_StatusIq
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "Velocity regulator out: @1->ToutPI regulator, @2->TLimNeg, @3->TLimPos, @4->TrackingValue if regulator is tracking, @5-> Ppart, @6-> Ipart,  @7-> Dpart [A]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 7
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3152
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_VelocityReg_StatusIq[7];/* '<S303>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_VelocityReg_StatusRPM
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = FLOAT
   @@ DESCRIPTION = "RPM to velocity regulator: @1-> RPM_req, @2->RPM_act, @3->RPM_err (positive when actual is higher as reference), [rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 3
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3151
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile real32_T Ctrl_VelocityReg_StatusRPM[3];/* '<S301>/Signal Conversion' */

/*
   @@ SYMBOL = Ctrl_VelocityReg_StatusTracking
   @@ A2L_TYPE = MEASURE
   @@ DATA_TYPE = UBYTE
   @@ DESCRIPTION = "Tracking enable. Output is tracking track value and PI regulator is not active [bit]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3154
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 4
   @@ END
 */
volatile boolean_T Ctrl_VelocityReg_StatusTracking;/* '<S305>/Signal Conversion' */

/* Constant parameters (default storage) */
const ConstP_emDriveFOC_control_T emDriveFOC_control_ConstP = {
  /* Expression: single([0,42.19,118.82,94.22,123.14,143.46]);
   * Referenced by: '<S85>/Ud Lookup Table'
   */
  { 0.0F, 42.19F, 118.82F, 94.22F, 123.14F, 143.46F },

  /* Expression: single([0,562.5,5625,11250,15750,18000]);
   * Referenced by: '<S85>/Ud Lookup Table'
   */
  { 0.0F, 562.5F, 5625.0F, 11250.0F, 15750.0F, 18000.0F }
};

/* Block signals and states (default storage) */
DW_emDriveFOC_control_T emDriveFOC_control_DW;

/* External inputs (root inport signals with default storage) */
ExtU_emDriveFOC_control_T emDriveFOC_control_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_emDriveFOC_control_T emDriveFOC_control_Y;

/* Real-time model */
static RT_MODEL_emDriveFOC_control_T emDriveFOC_control_M_;
RT_MODEL_emDriveFOC_control_T *const emDriveFOC_control_M =
  &emDriveFOC_control_M_;
FOC_FAST_CODE
  static uint32_T plook_u32f_bincka(real32_T u, const real32_T bp[], uint32_T
  maxIndex);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static uint32_T binsearch_u32f(real32_T u, const real32_T bp[], uint32_T
  startIndex, uint32_T maxIndex);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_control_StimUdTID0(real32_T rtu_In, int32_T rtu_Target,
  real32_T *rty_Out, DW_StimUd_emDriveFOC_control_T *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_control_StimUdTID1(const real32_T rtd_Stim[2],
  DW_StimUd_emDriveFOC_control_T *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_co_dqtoalphabeta(real32_T rtu_dq, real32_T rtu_dq_f,
  real32_T rtu_cs, real32_T rtu_cs_c, real32_T *rty_alphabeta, real32_T
  *rty_alphabeta_f);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_c_RunIdRamp_Init(DW_RunIdRamp_emDriveFOC_contr_T
  *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_contro_RunIdRamp(real_T rtu_Start, real32_T *rty_IdOut,
  boolean_T *rty_Finished, const real32_T *rtd_Tsc,
  DW_RunIdRamp_emDriveFOC_contr_T *localDW);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_RunRPM_ramp_Init(DW_RunRPM_ramp_emDriveFOC_con_T
  *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_cont_RunRPM_ramp(real32_T rtu_RPM_ref, const tyBusPos
  *rtu_InitPos, boolean_T rtu_Force, tyBusPos *rty_Pos, real32_T *rty_RPM,
  boolean_T *rty_Finished, const real32_T *rtd_OmegaEtoRPM, const real32_T
  *rtd_RPMtoOmegaE, const real32_T *rtd_Tsc, DW_RunRPM_ramp_emDriveFOC_con_T
  *localDW);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveF_RunPosModeMix_Init(DW_RunPosModeMix_emDriveFOC_c_T
  *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_co_RunPosModeMix(real32_T rtu_Mode, const tyBusPos
  *rtu_PosMode0, const tyBusPos *rtu_PosMode1, boolean_T rtu_Force, tyBusPos
  *rty_Pos, real32_T *rty_ModeOut, const real32_T *rtd_Tsc,
  DW_RunPosModeMix_emDriveFOC_c_T *localDW);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_c_RunRefValueMix(real32_T rtu_CloseOpenMode, real32_T
  rtu_T_RefIn, real32_T *rty_TrefOut, real32_T *rty_IdOut);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_c_ConstInit_Init(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_contro_ConstInitTID1(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_contro_ConstInitTID2(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_co_AngleGenConst(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void SenzorlessStateDigram_Init(void);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDri_SenzorlessStateDigram(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC__Sensorless_Init(void);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static void emDriveFOC_contr_Sensorless(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void PowerLimitDecisionStateflow(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void TemperatureAndPowerLim_Init(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void em_TemperatureAndPowerLimit(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDrive_MeasurmentsVel_Init(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_c_MeasurmentsVel(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_control_Const(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emD_RPM_ramp_and_limit_Init(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveF_RPM_ramp_and_limit(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_co_StimGenerator(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveF_Velocity_loop_Init(void);
FOC_RESTORE_CODE
  FOC_SLOW_CODE
  static void emDriveFOC_co_Velocity_loop(void);
FOC_RESTORE_CODE
  /* Forward declaration for local functions */
  FOC_FAST_CODE
  static void emDriveFOC_control_Run(real_T *Start, real32_T *Switch_d,
  boolean_T *RelationalOperator_h);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  static uint32_T plook_u32f_bincka(real32_T u, const real32_T bp[], uint32_T
  maxIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index only
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32f(u, bp, maxIndex >> 1U, maxIndex);
  } else {
    bpIndex = maxIndex;
  }

  return bpIndex;
}

FOC_RESTORE_CODE
  FOC_FAST_CODE
  static uint32_T binsearch_u32f(real32_T u, const real32_T bp[], uint32_T
  startIndex, uint32_T maxIndex)
{
  uint32_T bpIdx;
  uint32_T bpIndex;
  uint32_T iRght;

  /* Binary Search */
  bpIdx = startIndex;
  bpIndex = 0U;
  iRght = maxIndex;
  while (iRght - bpIndex > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1U;
  }

  return bpIndex;
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<Root>/ConstInit' */
  FOC_SLOW_CODE
  static void emDriveFOC_c_ConstInit_Init(void)
{
  /* Start for Constant: '<S22>/Constant17' */
  emDriveFOC_control_DW.Constant17 = 1.4F;
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<Root>/ConstInit' */
  FOC_SLOW_CODE
  static void emDriveFOC_contro_ConstInitTID1(void)
{
  real32_T rtb_Product5;

  /* DataTypeConversion: '<S21>/Data Type Conversion' incorporates:
   *  Constant: '<S21>/Ctrl_PowerLimit__Duration'
   *  Constant: '<S21>/conf.Tsv'
   *  Product: '<S21>/Divide'
   */
  emDriveFOC_control_DW.PowerLimitDuration = (int32_T)floorf
    (Ctrl_PowerLimit__Duration / 0.001F);

  /* DataTypeConversion: '<S21>/Data Type Conversion1' incorporates:
   *  Constant: '<S21>/Ctrl_PowerLimit__DurationHyst'
   *  Constant: '<S21>/conf.Tsv1'
   *  Product: '<S21>/Divide1'
   */
  emDriveFOC_control_DW.PowerLimitDurationHysteresis = (int32_T)floorf
    (Ctrl_PowerLimit__DurationHyst / 0.001F);

  /* Product: '<S22>/Product5' incorporates:
   *  Constant: '<S22>/Constant6'
   *  Product: '<S22>/Product6'
   */
  rtb_Product5 = emDriveFOC_control_DW.RPM_act / Ctrl_Limits__RPMpositive *
    emDriveFOC_control_DW.Gain4;

  /* MATLAB Function: '<S22>/ThetaPLLCalcK1' */
  /* MATLAB Function 'ThetaPLLCalcK': '<S33>:1' */
  /* '<S33>:1:2' K1 = Omega^2; */
  emDriveFOC_control_DW.K1_g = rtb_Product5 * rtb_Product5;

  /* '<S33>:1:3' K2 = 2*Damp/Omega; */
  emDriveFOC_control_DW.K2_g = 2.0F * emDriveFOC_control_DW.Constant17 /
    rtb_Product5;
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<Root>/ConstInit' */
  FOC_SLOW_CODE
  static void emDriveFOC_contro_ConstInitTID2(void)
{
  real32_T rtb_Product;

  /* Outputs for Atomic SubSystem: '<S16>/LPFalphaCriteria1' */
  /* MATLAB Function: '<S26>/MATLAB Function' incorporates:
   *  Constant: '<S16>/Constant15'
   *  Constant: '<S26>/TimeConstant'
   */
  /* MATLAB Function 'HPF_coef/MATLAB Function': '<S25>:1' */
  /* '<S25>:1:3' alpha = TimeConstant/(TimeConstant+ts); */
  /* MATLAB Function 'LPF_coef/MATLAB Function': '<S27>:1' */
  /* '<S27>:1:2' if TimeConstant ~= 0 */
  if (Ctrl_Sensorless__ValidLPFtimeConstant != 0.0F) {
    /* '<S27>:1:3' alpha = ts/(TimeConstant+ts); */
    emDriveFOC_control_DW.alpha_b = 1.0F / Ctrl_Gen__Fcur / (1.0F /
      Ctrl_Gen__Fcur + Ctrl_Sensorless__ValidLPFtimeConstant);
  } else {
    /* '<S27>:1:4' else */
    /* '<S27>:1:5' alpha=single(0); */
    emDriveFOC_control_DW.alpha_b = 0.0F;
  }

  /* End of MATLAB Function: '<S26>/MATLAB Function' */
  /* End of Outputs for SubSystem: '<S16>/LPFalphaCriteria1' */

  /* Product: '<S17>/Product' incorporates:
   *  Constant: '<S17>/Constant'
   *  Constant: '<S17>/Constant16'
   */
  emDriveFOC_control_DW.MinTimeInCloseLoop = Ctrl_Sensorless__MinTimeInClose *
    Ctrl_Gen__Fcur;

  /* Product: '<S17>/Product1' incorporates:
   *  Constant: '<S17>/Constant'
   *  Constant: '<S17>/Constant1'
   */
  emDriveFOC_control_DW.TimeInHold = Ctrl_Sensorless__TimeInHoldState *
    Ctrl_Gen__Fcur;

  /* Outputs for Atomic SubSystem: '<S18>/LPFalphaFieldWeak1' */
  /* MATLAB Function: '<S28>/MATLAB Function' incorporates:
   *  Constant: '<S18>/Constant16'
   *  Constant: '<S28>/TimeConstant'
   */
  /* MATLAB Function 'LPF_coef/MATLAB Function': '<S29>:1' */
  /* '<S29>:1:2' if TimeConstant ~= 0 */
  if (Ctrl_FieldWeak__LPFtimeConstant != 0.0F) {
    /* '<S29>:1:3' alpha = ts/(TimeConstant+ts); */
    emDriveFOC_control_DW.alpha_g = 1.0F / Ctrl_Gen__Fcur / (1.0F /
      Ctrl_Gen__Fcur + Ctrl_FieldWeak__LPFtimeConstant);
  } else {
    /* '<S29>:1:4' else */
    /* '<S29>:1:5' alpha=single(0); */
    emDriveFOC_control_DW.alpha_g = 0.0F;
  }

  /* End of MATLAB Function: '<S28>/MATLAB Function' */
  /* End of Outputs for SubSystem: '<S18>/LPFalphaFieldWeak1' */

  /* Outputs for Atomic SubSystem: '<S19>/LPFalphaMeasurment' */
  /* MATLAB Function: '<S30>/MATLAB Function' incorporates:
   *  Constant: '<S19>/Constant16'
   *  Constant: '<S30>/TimeConstant'
   */
  /* MATLAB Function 'LPF_coef/MATLAB Function': '<S31>:1' */
  /* '<S31>:1:2' if TimeConstant ~= 0 */
  if (Ctrl_Measurment__LPF_TimeConst != 0.0F) {
    /* '<S31>:1:3' alpha = ts/(TimeConstant+ts); */
    emDriveFOC_control_DW.alpha_c = 1.0F / Ctrl_Gen__Fcur / (1.0F /
      Ctrl_Gen__Fcur + Ctrl_Measurment__LPF_TimeConst);
  } else {
    /* '<S31>:1:4' else */
    /* '<S31>:1:5' alpha=single(0); */
    emDriveFOC_control_DW.alpha_c = 0.0F;
  }

  /* End of MATLAB Function: '<S30>/MATLAB Function' */
  /* End of Outputs for SubSystem: '<S19>/LPFalphaMeasurment' */

  /* Sum: '<S20>/Add7' incorporates:
   *  Constant: '<S20>/Constant12'
   *  Constant: '<S20>/Constant14'
   *  Constant: '<S20>/MinOffTimeHighSide'
   *  Gain: '<S20>/Gain5'
   *  Product: '<S20>/Product4'
   */
  emDriveFOC_control_DW.MinOffTimeHighSideF = 1.0F -
    Ctrl_Mod__MinOffTimeHighSide * Ctrl_Gen__Fcur * 2.0F;

  /* Sum: '<S20>/Add9' incorporates:
   *  Constant: '<S20>/Constant13'
   *  Constant: '<S20>/Constant14'
   *  Constant: '<S20>/MinOffTimeSecondHighSide'
   *  Gain: '<S20>/Gain6'
   *  Product: '<S20>/Product5'
   */
  emDriveFOC_control_DW.Ctrl_Mod__MinOffTimeSecondHighS = 1.0F -
    Ctrl_Mod__MinOffTimeSecondHighSide * Ctrl_Gen__Fcur * 2.0F;

  /* Gain: '<S20>/Gain1' incorporates:
   *  Constant: '<S20>/Constant14'
   *  Constant: '<S20>/Constant9'
   *  Constant: '<S20>/MinOffTimeLowSide'
   *  Gain: '<S20>/Gain4'
   *  Product: '<S20>/Product3'
   *  Sum: '<S20>/Add5'
   */
  emDriveFOC_control_DW.MinOffTimeLowSideF = -(1.0F -
    Ctrl_Mod__MinOffTimeLowSide * Ctrl_Gen__Fcur * 2.0F);

  /* Gain: '<S20>/Gain7' incorporates:
   *  Constant: '<S20>/Constant14'
   *  Constant: '<S20>/MinimumPulseTime'
   *  Product: '<S20>/Product6'
   */
  emDriveFOC_control_DW.MinPulseTimeF = Ctrl_Mod__MinPulseTime * Ctrl_Gen__Fcur *
    2.0F;

  /* Constant: '<S22>/Constant17' */
  emDriveFOC_control_DW.Constant17 = 1.4F;

  /* Gain: '<S22>/Gain4' incorporates:
   *  Constant: '<S22>/Constant16'
   */
  emDriveFOC_control_DW.Gain4 = 6.28318548F * Ctrl_Sensorless__PLL_Bandwith;

  /* Outputs for Atomic SubSystem: '<S22>/LPFalphaOmega' */
  /* MATLAB Function: '<S32>/MATLAB Function' incorporates:
   *  Constant: '<S22>/Constant11'
   *  Constant: '<S32>/TimeConstant'
   */
  /* MATLAB Function 'LPF_coef/MATLAB Function': '<S35>:1' */
  /* '<S35>:1:2' if TimeConstant ~= 0 */
  if (Ctrl_Sensorless__OmegaPostLPFtimeConstant != 0.0F) {
    /* '<S35>:1:3' alpha = ts/(TimeConstant+ts); */
    emDriveFOC_control_DW.alpha = 1.0F / Ctrl_Gen__Fcur / (1.0F / Ctrl_Gen__Fcur
      + Ctrl_Sensorless__OmegaPostLPFtimeConstant);
  } else {
    /* '<S35>:1:4' else */
    /* '<S35>:1:5' alpha=single(0); */
    emDriveFOC_control_DW.alpha = 0.0F;
  }

  /* End of MATLAB Function: '<S32>/MATLAB Function' */
  /* End of Outputs for SubSystem: '<S22>/LPFalphaOmega' */

  /* Product: '<S22>/Product' incorporates:
   *  Constant: '<S22>/Constant2'
   */
  rtb_Product = Ctrl_Sensorless__PLL_BandwithReduction *
    emDriveFOC_control_DW.Gain4;

  /* Product: '<S22>/Product1' incorporates:
   *  Constant: '<S22>/Constant1'
   *  Constant: '<S22>/Constant4'
   */
  emDriveFOC_control_DW.Delay = 1.0F / Ctrl_Gen__Fcur *
    Ctrl_Sensorless__CalculationDelay;

  /* MATLAB Function: '<S22>/ThetaPLLCalcK2' */
  /* MATLAB Function 'ThetaPLLCalcK': '<S34>:1' */
  /* '<S34>:1:2' K1 = Omega^2; */
  emDriveFOC_control_DW.K1 = rtb_Product * rtb_Product;

  /* '<S34>:1:3' K2 = 2*Damp/Omega; */
  emDriveFOC_control_DW.K2 = 2.8F / rtb_Product;

  /* Iq = CalculateIq(T,Id,Ld,Lq,Psi,Np) */
  /* Iq = T/(3/2*Np*(Psi+(Ld-Lq)*Id)); */
  /* MATLAB Function 'ConstInit/TtoIq_Const/CalcualteIqC': '<S36>:1' */
  /* Iq = T/((3/2*Np*Psi)+(3/2*Np*)*(Ld-Lq)*Id)); */
  /* '<S36>:1:7' C1= (3/2*Np); */
  /* '<S36>:1:8' C2= (3/2*Np)*(Ld-Lq); */
  /* Iq = T/(C1*Psi+C2*Id); */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S37>/AngleGenConst' */
  FOC_SLOW_CODE
  static void emDriveFOC_co_AngleGenConst(void)
{
  real32_T rtb_Switch1;

  /* Switch: '<S46>/Switch1' incorporates:
   *  Constant: '<S46>/Constant1'
   *  Constant: '<S46>/Constant4'
   *  Constant: '<S46>/Constant8'
   */
  if (Ctrl_ALTTestMode__Enable) {
    rtb_Switch1 = Ctrl_ALTTestMode__RPM;
  } else {
    rtb_Switch1 = Ctrl_AngleGenerator__RPM;
  }

  /* End of Switch: '<S46>/Switch1' */

  /* Product: '<S46>/Product' incorporates:
   *  Constant: '<S46>/RPM_to_OmegaE'
   */
  emDriveFOC_control_DW.Product = 0.104719758F * Ctrl_Motor__Np * rtb_Switch1;

  /* RelationalOperator: '<S46>/Relational Operator' incorporates:
   *  Constant: '<S46>/Constant7'
   */
  emDriveFOC_control_DW.RelationalOperator_le = (rtb_Switch1 == 0.0F);

  /* Gain: '<S46>/Gain' incorporates:
   *  Constant: '<S46>/Constant5'
   */
  emDriveFOC_control_DW.Gain = 0.0174532924F * Ctrl_AngleGenerator__Offset;
}

FOC_RESTORE_CODE
  /*
   * Output and update for atomic system:
   *    '<S54>/StimUd'
   *    '<S54>/StrimUq'
   *    '<S38>/StimId'
   *    '<S38>/StimIq'
   */
  FOC_FAST_CODE
  static void emDriveFOC_control_StimUdTID0(real32_T rtu_In, int32_T rtu_Target,
  real32_T *rty_Out, DW_StimUd_emDriveFOC_control_T *localDW)
{
  real32_T rtb_RateTransition1_p;

  /* RateTransition: '<S89>/Rate Transition' */
  *rty_Out = localDW->RateTransition_Buffer0;

  /* RateTransition: '<S89>/Rate Transition1' */
  rtb_RateTransition1_p = localDW->RateTransition1_Buffer0;

  /* Switch: '<S89>/Switch2' incorporates:
   *  Constant: '<S89>/Constant2'
   *  Constant: '<S89>/Constant5'
   *  RelationalOperator: '<S89>/Relational Operator'
   *  RelationalOperator: '<S89>/Relational Operator1'
   *  Switch: '<S89>/Switch'
   */
  if (Ctrl_Stim2__GenTarget == rtu_Target) {
    *rty_Out = rtb_RateTransition1_p;
  } else if (Ctrl_Stim1__GenTarget != rtu_Target) {
    *rty_Out = rtu_In;
  }

  /* End of Switch: '<S89>/Switch2' */
}

FOC_RESTORE_CODE
  /*
   * Output and update for atomic system:
   *    '<S54>/StimUd'
   *    '<S54>/StrimUq'
   *    '<S38>/StimId'
   *    '<S38>/StimIq'
   */
  FOC_FAST_CODE
  static void emDriveFOC_control_StimUdTID1(const real32_T rtd_Stim[2],
  DW_StimUd_emDriveFOC_control_T *localDW)
{
  real32_T rtb_IndexVector1_i;
  real32_T rtb_IndexVector4_l;

  /* MultiPortSwitch: '<S89>/Index Vector1' incorporates:
   *  Constant: '<S89>/Constant4'
   *  DataStoreRead: '<S89>/Data Store Read1'
   */
  rtb_IndexVector1_i = rtd_Stim[0];

  /* MultiPortSwitch: '<S89>/Index Vector4' incorporates:
   *  Constant: '<S89>/Constant1'
   *  DataStoreRead: '<S89>/Data Store Read1'
   */
  rtb_IndexVector4_l = rtd_Stim[1];

  /* RateTransition: '<S89>/Rate Transition' */
  localDW->RateTransition_Buffer0 = rtb_IndexVector1_i;

  /* RateTransition: '<S89>/Rate Transition1' */
  localDW->RateTransition1_Buffer0 = rtb_IndexVector4_l;
}

FOC_RESTORE_CODE
  /*
   * Output and update for atomic system:
   *    '<S38>/dq-to-alphabeta'
   *    '<S58>/dq-to-alphabeta'
   */
  FOC_FAST_CODE
  static void emDriveFOC_co_dqtoalphabeta(real32_T rtu_dq, real32_T rtu_dq_f,
  real32_T rtu_cs, real32_T rtu_cs_c, real32_T *rty_alphabeta, real32_T
  *rty_alphabeta_f)
{
  /* Sum: '<S115>/sumx' incorporates:
   *  Product: '<S115>/AxBx'
   *  Product: '<S115>/AyBy'
   */
  *rty_alphabeta = rtu_dq * rtu_cs - rtu_dq_f * rtu_cs_c;

  /* Sum: '<S115>/sumy' incorporates:
   *  Product: '<S115>/AxBy'
   *  Product: '<S115>/AyBx'
   */
  *rty_alphabeta_f = rtu_dq_f * rtu_cs + rtu_dq * rtu_cs_c;
}

FOC_RESTORE_CODE
  /* System initialize for function-call system: '<S125>/Run.IdRamp' */
  FOC_SLOW_CODE
  static void emDriveFOC_c_RunIdRamp_Init(DW_RunIdRamp_emDriveFOC_contr_T
  *localDW)
{
  /* SystemInitialize for Atomic SubSystem: '<S190>/Rate Limiter Dynamic with track' */
  /* InitializeConditions for Delay: '<S195>/Delay' */
  localDW->icLoad = true;

  /* End of SystemInitialize for SubSystem: '<S190>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for function-call system: '<S125>/Run.IdRamp' */
  FOC_FAST_CODE
  static void emDriveFOC_contro_RunIdRamp(real_T rtu_Start, real32_T *rty_IdOut,
  boolean_T *rty_Finished, const real32_T *rtd_Tsc,
  DW_RunIdRamp_emDriveFOC_contr_T *localDW)
{
  real32_T Delay_DSTATE;
  real32_T rtb_Gain2_d;
  real32_T rtb_Product_il;
  real32_T rtb_UkYk1_a;
  boolean_T DelayInput1_DSTATE;

  /* Outputs for Atomic SubSystem: '<S190>/Rate Limiter Dynamic with track' */
  /* Delay: '<S195>/Delay' */
  Delay_DSTATE = localDW->Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S190>/Rate Limiter Dynamic with track' */

  /* RelationalOperator: '<S196>/Compare' incorporates:
   *  Constant: '<S196>/Constant'
   */
  DelayInput1_DSTATE = (rtu_Start > 0.0);

  /* Gain: '<S190>/Gain2' incorporates:
   *  Constant: '<S190>/Constant2'
   */
  rtb_Gain2_d = 1.41421354F * Ctrl_OpenLoop__Id;

  /* Outputs for Atomic SubSystem: '<S190>/Rate Limiter Dynamic with track' */
  /* Delay: '<S195>/Delay' incorporates:
   *  Constant: '<S190>/Constant1'
   */
  if (localDW->icLoad) {
    Delay_DSTATE = 0.0F;
  }

  /* Switch: '<S195>/Switch' incorporates:
   *  Constant: '<S190>/Constant1'
   *  RelationalOperator: '<S194>/FixPt Relational Operator'
   *  RelationalOperator: '<S196>/Compare'
   *  Sum: '<S195>/Difference Inputs2'
   *  Switch: '<S197>/Switch2'
   *  UnitDelay: '<S194>/Delay Input1'
   *
   * Block description for '<S195>/Difference Inputs2':
   *
   *  Add in CPU
   *
   * Block description for '<S194>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((int32_T)DelayInput1_DSTATE > (int32_T)localDW->DelayInput1_DSTATE) {
    *rty_IdOut = 0.0F;
  } else {
    /* Product: '<S190>/Product' incorporates:
     *  Constant: '<S190>/Constant'
     *  Gain: '<S190>/Gain1'
     */
    rtb_Product_il = 1.41421354F * Ctrl_OpenLoop__RampIdRate * *rtd_Tsc;

    /* Sum: '<S195>/Difference Inputs1'
     *
     * Block description for '<S195>/Difference Inputs1':
     *
     *  Add in CPU
     */
    rtb_UkYk1_a = rtb_Gain2_d - Delay_DSTATE;

    /* Switch: '<S197>/Switch2' incorporates:
     *  RelationalOperator: '<S197>/LowerRelop1'
     *  RelationalOperator: '<S197>/UpperRelop'
     *  Switch: '<S197>/Switch'
     *  UnaryMinus: '<S190>/Unary Minus'
     */
    if (rtb_UkYk1_a > rtb_Product_il) {
      rtb_UkYk1_a = rtb_Product_il;
    } else if (rtb_UkYk1_a < -rtb_Product_il) {
      /* Switch: '<S197>/Switch' incorporates:
       *  UnaryMinus: '<S190>/Unary Minus'
       */
      rtb_UkYk1_a = -rtb_Product_il;
    }

    *rty_IdOut = rtb_UkYk1_a + Delay_DSTATE;
  }

  /* End of Switch: '<S195>/Switch' */

  /* Update for Delay: '<S195>/Delay' */
  localDW->icLoad = false;

  /* End of Outputs for SubSystem: '<S190>/Rate Limiter Dynamic with track' */

  /* RelationalOperator: '<S190>/Relational Operator' */
  *rty_Finished = (*rty_IdOut == rtb_Gain2_d);

  /* RelationalOperator: '<S196>/Compare' incorporates:
   *  UnitDelay: '<S194>/Delay Input1'
   *
   * Block description for '<S194>/Delay Input1':
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = DelayInput1_DSTATE;

  /* Outputs for Atomic SubSystem: '<S190>/Rate Limiter Dynamic with track' */
  Delay_DSTATE = *rty_IdOut;

  /* Delay: '<S195>/Delay' */
  localDW->Delay_DSTATE = Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S190>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* System initialize for function-call system: '<S125>/Run.RPM_ramp' */
  FOC_SLOW_CODE
  static void emDriveFOC_RunRPM_ramp_Init(DW_RunRPM_ramp_emDriveFOC_con_T
  *localDW)
{
  /* SystemInitialize for Atomic SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  /* InitializeConditions for Delay: '<S215>/Delay' */
  localDW->icLoad = true;

  /* End of SystemInitialize for SubSystem: '<S192>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for function-call system: '<S125>/Run.RPM_ramp' */
  FOC_FAST_CODE
  static void emDriveFOC_cont_RunRPM_ramp(real32_T rtu_RPM_ref, const tyBusPos
  *rtu_InitPos, boolean_T rtu_Force, tyBusPos *rty_Pos, real32_T *rty_RPM,
  boolean_T *rty_Finished, const real32_T *rtd_OmegaEtoRPM, const real32_T
  *rtd_RPMtoOmegaE, const real32_T *rtd_Tsc, DW_RunRPM_ramp_emDriveFOC_con_T
  *localDW)
{
  real32_T Delay_DSTATE;
  real32_T Memory_PreviousInput;
  real32_T rtb_Product_cz;
  real32_T rtb_UkYk1_k;
  boolean_T rtb_FixPtRelationalOperator_c;

  /* Outputs for Atomic SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  /* Delay: '<S215>/Delay' */
  Delay_DSTATE = localDW->Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  Memory_PreviousInput = localDW->Memory_PreviousInput;

  /* RelationalOperator: '<S213>/FixPt Relational Operator' incorporates:
   *  RelationalOperator: '<S216>/Compare'
   *  UnitDelay: '<S213>/Delay Input1'
   *
   * Block description for '<S213>/Delay Input1':
   *
   *  Store in Global RAM
   */
  rtb_FixPtRelationalOperator_c = ((int32_T)rtu_Force > (int32_T)
    localDW->DelayInput1_DSTATE);

  /* Outputs for Atomic SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  /* Delay: '<S215>/Delay' */
  if (localDW->icLoad) {
    Delay_DSTATE = 0.0F;
  }

  /* Switch: '<S215>/Switch' incorporates:
   *  Product: '<S192>/Product1'
   *  Sum: '<S215>/Difference Inputs2'
   *  Switch: '<S219>/Switch2'
   *
   * Block description for '<S215>/Difference Inputs2':
   *
   *  Add in CPU
   */
  if (rtb_FixPtRelationalOperator_c) {
    *rty_RPM = rtu_InitPos->omega_r * *rtd_OmegaEtoRPM;
  } else {
    /* Product: '<S192>/Product' incorporates:
     *  Constant: '<S192>/Constant'
     */
    rtb_Product_cz = Ctrl_OpenLoop__RampRPMrate * *rtd_Tsc;

    /* Sum: '<S215>/Difference Inputs1'
     *
     * Block description for '<S215>/Difference Inputs1':
     *
     *  Add in CPU
     */
    rtb_UkYk1_k = rtu_RPM_ref - Delay_DSTATE;

    /* Switch: '<S219>/Switch2' incorporates:
     *  RelationalOperator: '<S219>/LowerRelop1'
     *  RelationalOperator: '<S219>/UpperRelop'
     *  Switch: '<S219>/Switch'
     *  UnaryMinus: '<S192>/Unary Minus'
     */
    if (rtb_UkYk1_k > rtb_Product_cz) {
      rtb_UkYk1_k = rtb_Product_cz;
    } else if (rtb_UkYk1_k < -rtb_Product_cz) {
      /* Switch: '<S219>/Switch' incorporates:
       *  UnaryMinus: '<S192>/Unary Minus'
       */
      rtb_UkYk1_k = -rtb_Product_cz;
    }

    *rty_RPM = rtb_UkYk1_k + Delay_DSTATE;
  }

  /* End of Switch: '<S215>/Switch' */

  /* Update for Delay: '<S215>/Delay' */
  localDW->icLoad = false;

  /* End of Outputs for SubSystem: '<S192>/Rate Limiter Dynamic with track' */

  /* Product: '<S192>/Product2' */
  rtb_Product_cz = *rtd_RPMtoOmegaE * *rty_RPM;

  /* BusCreator: '<S192>/Bus Creator' */
  rty_Pos->theta_r = Memory_PreviousInput;
  rty_Pos->omega_r = rtb_Product_cz;

  /* Outputs for Atomic SubSystem: '<S214>/ThetaAdd' */
  /* MATLAB Function: '<S217>/ThetaAdd' incorporates:
   *  Product: '<S214>/Product'
   */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S218>:1' */
  /* '<S218>:1:3' y = u1+u2; */
  Memory_PreviousInput += rtb_Product_cz * *rtd_Tsc;

  /* '<S218>:1:4' if y > single(pi) */
  if (Memory_PreviousInput > 3.14159274F) {
    /* '<S218>:1:5' y=y-single(2*pi); */
    Memory_PreviousInput -= 6.28318548F;
  }

  /* '<S218>:1:7' if y < single(-pi) */
  if (Memory_PreviousInput < -3.14159274F) {
    /* '<S218>:1:8' y=y+single(2*pi); */
    Memory_PreviousInput += 6.28318548F;
  }

  /* End of MATLAB Function: '<S217>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S214>/ThetaAdd' */

  /* RelationalOperator: '<S192>/Relational Operator' */
  *rty_Finished = (*rty_RPM == rtu_RPM_ref);

  /* Update for UnitDelay: '<S213>/Delay Input1' incorporates:
   *  RelationalOperator: '<S216>/Compare'
   *
   * Block description for '<S213>/Delay Input1':
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_Force;

  /* Switch: '<S214>/Switch' */
  if (rtb_FixPtRelationalOperator_c) {
    Memory_PreviousInput = rtu_InitPos->theta_r;
  }

  /* End of Switch: '<S214>/Switch' */

  /* Outputs for Atomic SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  Delay_DSTATE = *rty_RPM;

  /* End of Outputs for SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  localDW->Memory_PreviousInput = Memory_PreviousInput;

  /* Outputs for Atomic SubSystem: '<S192>/Rate Limiter Dynamic with track' */
  /* Delay: '<S215>/Delay' */
  localDW->Delay_DSTATE = Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S192>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* System initialize for function-call system: '<S125>/Run.PosModeMix' */
  FOC_SLOW_CODE
  static void emDriveF_RunPosModeMix_Init(DW_RunPosModeMix_emDriveFOC_c_T
  *localDW)
{
  /* SystemInitialize for Atomic SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  /* InitializeConditions for Delay: '<S201>/Delay' */
  localDW->icLoad = true;

  /* End of SystemInitialize for SubSystem: '<S191>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for function-call system: '<S125>/Run.PosModeMix' */
  FOC_FAST_CODE
  static void emDriveFOC_co_RunPosModeMix(real32_T rtu_Mode, const tyBusPos
  *rtu_PosMode0, const tyBusPos *rtu_PosMode1, boolean_T rtu_Force, tyBusPos
  *rty_Pos, real32_T *rty_ModeOut, const real32_T *rtd_Tsc,
  DW_RunPosModeMix_emDriveFOC_c_T *localDW)
{
  int32_T rtb_Switch3_kx;
  real32_T Delay_DSTATE;
  real32_T UnitDelay1_DSTATE;
  real32_T rtb_Switch2_im;
  real32_T rtb_UkYk1_h;

  /* Outputs for Atomic SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  /* Delay: '<S201>/Delay' */
  Delay_DSTATE = localDW->Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  UnitDelay1_DSTATE = localDW->UnitDelay1_DSTATE;

  /* Switch: '<S191>/Switch3' */
  rtb_Switch3_kx = (rtu_Mode != 0.0F);

  /* Outputs for Atomic SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  /* Delay: '<S201>/Delay' */
  if (localDW->icLoad) {
    Delay_DSTATE = (real32_T)rtb_Switch3_kx;
  }

  /* Switch: '<S201>/Switch' incorporates:
   *  RelationalOperator: '<S200>/FixPt Relational Operator'
   *  RelationalOperator: '<S205>/Compare'
   *  Sum: '<S201>/Difference Inputs2'
   *  UnitDelay: '<S200>/Delay Input1'
   *
   * Block description for '<S201>/Difference Inputs2':
   *
   *  Add in CPU
   *
   * Block description for '<S200>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((int32_T)rtu_Force > (int32_T)localDW->DelayInput1_DSTATE) {
    *rty_ModeOut = (real32_T)rtb_Switch3_kx;
  } else {
    /* Sum: '<S201>/Difference Inputs1'
     *
     * Block description for '<S201>/Difference Inputs1':
     *
     *  Add in CPU
     */
    rtb_UkYk1_h = (real32_T)rtb_Switch3_kx - Delay_DSTATE;

    /* Product: '<S191>/Product' incorporates:
     *  Constant: '<S191>/Constant1'
     */
    rtb_Switch2_im = 1.0F / Ctrl_Sensorless__TransitionTimeToClose * *rtd_Tsc;

    /* Switch: '<S206>/Switch2' incorporates:
     *  RelationalOperator: '<S206>/LowerRelop1'
     */
    if (rtb_UkYk1_h <= rtb_Switch2_im) {
      /* UnaryMinus: '<S191>/Unary Minus' incorporates:
       *  Constant: '<S191>/Constant3'
       *  Product: '<S191>/Product1'
       */
      rtb_Switch2_im = -(1.0F / Ctrl_Sensorless__TransitionTimeToOpen * *rtd_Tsc);

      /* Switch: '<S206>/Switch' incorporates:
       *  RelationalOperator: '<S206>/UpperRelop'
       *  UnaryMinus: '<S191>/Unary Minus'
       */
      if (rtb_UkYk1_h >= rtb_Switch2_im) {
        rtb_Switch2_im = rtb_UkYk1_h;
      }

      /* End of Switch: '<S206>/Switch' */
    }

    /* End of Switch: '<S206>/Switch2' */
    *rty_ModeOut = rtb_Switch2_im + Delay_DSTATE;
  }

  /* End of Switch: '<S201>/Switch' */

  /* Update for Delay: '<S201>/Delay' */
  localDW->icLoad = false;

  /* End of Outputs for SubSystem: '<S191>/Rate Limiter Dynamic with track' */

  /* Outputs for Atomic SubSystem: '<S191>/ThetaSub' */
  /* MATLAB Function: '<S203>/ThetaSub' */
  /* MATLAB Function 'ThetaSub/ThetaSub': '<S208>:1' */
  /* '<S208>:1:3' y = u1-u2; */
  rtb_UkYk1_h = rtu_PosMode0->theta_r - rtu_PosMode1->theta_r;

  /* '<S208>:1:4' if y > single(pi) */
  if (rtb_UkYk1_h > 3.14159274F) {
    /* '<S208>:1:5' y=y-single(2*pi); */
    rtb_UkYk1_h -= 6.28318548F;
  }

  /* '<S208>:1:7' if y < single(-pi) */
  if (rtb_UkYk1_h < -3.14159274F) {
    /* '<S208>:1:8' y=y+single(2*pi); */
    rtb_UkYk1_h += 6.28318548F;
  }

  /* End of MATLAB Function: '<S203>/ThetaSub' */
  /* End of Outputs for SubSystem: '<S191>/ThetaSub' */

  /* Outputs for Atomic SubSystem: '<S191>/Unwrap' */
  /* Outputs for Atomic SubSystem: '<S204>/ThetaSub' */
  /* MATLAB Function: '<S210>/ThetaSub' incorporates:
   *  UnitDelay: '<S204>/Unit Delay'
   */
  /* MATLAB Function 'ThetaSub/ThetaSub': '<S212>:1' */
  /* '<S212>:1:3' y = u1-u2; */
  rtb_Switch2_im = rtb_UkYk1_h - localDW->UnitDelay_DSTATE;

  /* '<S212>:1:4' if y > single(pi) */
  if (rtb_Switch2_im > 3.14159274F) {
    /* '<S212>:1:5' y=y-single(2*pi); */
    rtb_Switch2_im -= 6.28318548F;
  }

  /* '<S212>:1:7' if y < single(-pi) */
  if (rtb_Switch2_im < -3.14159274F) {
    /* '<S212>:1:8' y=y+single(2*pi); */
    rtb_Switch2_im += 6.28318548F;
  }

  /* End of MATLAB Function: '<S210>/ThetaSub' */
  /* End of Outputs for SubSystem: '<S204>/ThetaSub' */

  /* Outputs for Atomic SubSystem: '<S204>/ThetaAdd' */
  /* MATLAB Function: '<S209>/ThetaAdd' */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S211>:1' */
  /* '<S211>:1:3' y = u1+u2; */
  UnitDelay1_DSTATE += rtb_Switch2_im;

  /* '<S211>:1:4' if y > single(pi) */
  if (UnitDelay1_DSTATE > 3.14159274F) {
    /* '<S211>:1:5' y=y-single(2*pi); */
    UnitDelay1_DSTATE -= 6.28318548F;
  }

  /* '<S211>:1:7' if y < single(-pi) */
  if (UnitDelay1_DSTATE < -3.14159274F) {
    /* '<S211>:1:8' y=y+single(2*pi); */
    UnitDelay1_DSTATE += 6.28318548F;
  }

  /* End of MATLAB Function: '<S209>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S204>/ThetaAdd' */

  /* Update for UnitDelay: '<S204>/Unit Delay' */
  localDW->UnitDelay_DSTATE = rtb_UkYk1_h;

  /* End of Outputs for SubSystem: '<S191>/Unwrap' */

  /* Outputs for Atomic SubSystem: '<S191>/ThetaAdd' */
  /* MATLAB Function: '<S202>/ThetaAdd' incorporates:
   *  Constant: '<S191>/Constant'
   *  Product: '<S191>/close1'
   *  Sum: '<S191>/Add'
   */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S207>:1' */
  /* '<S207>:1:3' y = u1+u2; */
  rtb_Switch2_im = (1.0F - *rty_ModeOut) * UnitDelay1_DSTATE +
    rtu_PosMode1->theta_r;

  /* '<S207>:1:4' if y > single(pi) */
  if (rtb_Switch2_im > 3.14159274F) {
    /* '<S207>:1:5' y=y-single(2*pi); */
    rtb_Switch2_im -= 6.28318548F;
  }

  /* '<S207>:1:7' if y < single(-pi) */
  if (rtb_Switch2_im < -3.14159274F) {
    /* '<S207>:1:8' y=y+single(2*pi); */
    rtb_Switch2_im += 6.28318548F;
  }

  /* End of MATLAB Function: '<S202>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S191>/ThetaAdd' */

  /* BusCreator: '<S191>/Bus Creator1' incorporates:
   *  Constant: '<S191>/Constant'
   *  Product: '<S191>/pro_mode0'
   *  Product: '<S191>/pro_mode1'
   *  Sum: '<S191>/Add'
   *  Sum: '<S191>/Add3'
   */
  rty_Pos->theta_r = rtb_Switch2_im;
  rty_Pos->omega_r = (1.0F - *rty_ModeOut) * rtu_PosMode0->omega_r +
    rtu_PosMode1->omega_r * *rty_ModeOut;

  /* Update for UnitDelay: '<S200>/Delay Input1' incorporates:
   *  RelationalOperator: '<S205>/Compare'
   *
   * Block description for '<S200>/Delay Input1':
   *
   *  Store in Global RAM
   */
  localDW->DelayInput1_DSTATE = rtu_Force;

  /* Outputs for Atomic SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  Delay_DSTATE = *rty_ModeOut;

  /* End of Outputs for SubSystem: '<S191>/Rate Limiter Dynamic with track' */

  /* Outputs for Atomic SubSystem: '<S191>/Unwrap' */
  /* Switch: '<S204>/Switch2' incorporates:
   *  Constant: '<S198>/Constant'
   *  Constant: '<S199>/Constant'
   *  Logic: '<S191>/Logical Operator'
   *  RelationalOperator: '<S198>/Compare'
   *  RelationalOperator: '<S199>/Compare'
   */
  if ((*rty_ModeOut == 0.0F) || (*rty_ModeOut == 1.0F)) {
    UnitDelay1_DSTATE = rtb_UkYk1_h;
  }

  /* End of Switch: '<S204>/Switch2' */
  /* End of Outputs for SubSystem: '<S191>/Unwrap' */
  localDW->UnitDelay1_DSTATE = UnitDelay1_DSTATE;

  /* Outputs for Atomic SubSystem: '<S191>/Rate Limiter Dynamic with track' */
  /* Delay: '<S201>/Delay' */
  localDW->Delay_DSTATE = Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S191>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for function-call system: '<S125>/Run.RefValueMix' */
  FOC_FAST_CODE
  static void emDriveFOC_c_RunRefValueMix(real32_T rtu_CloseOpenMode, real32_T
  rtu_T_RefIn, real32_T *rty_TrefOut, real32_T *rty_IdOut)
{
  /* Sum: '<S193>/Add1' incorporates:
   *  Product: '<S193>/pro_close1'
   */
  *rty_TrefOut = rtu_T_RefIn * rtu_CloseOpenMode;

  /* Sum: '<S193>/Add3' incorporates:
   *  Constant: '<S193>/Constant'
   *  Constant: '<S193>/Constant3'
   *  Gain: '<S193>/Gain2'
   *  Product: '<S193>/pro_open'
   *  Sum: '<S193>/Add'
   */
  *rty_IdOut = 1.41421354F * Ctrl_OpenLoop__Id * (1.0F - rtu_CloseOpenMode);
}

FOC_RESTORE_CODE
  /* Function for Chart: '<S118>/SenzorlessStateDigram' */
  FOC_FAST_CODE
  static void emDriveFOC_control_Run(real_T *Start, real32_T *Switch_d,
  boolean_T *RelationalOperator_h)
{
  tyBusPos BusCreator;
  real32_T ModeOut;
  real32_T RPM_sign_latch;
  real32_T Switch;
  uint32_T temporalCounter_i1;
  uint8_T is_Run;
  uint8_T is_c3_emDriveFOC_control;
  boolean_T RelationalOperator_l;
  boolean_T guard1;
  is_c3_emDriveFOC_control = emDriveFOC_control_DW.is_c3_emDriveFOC_control;
  RPM_sign_latch = emDriveFOC_control_DW.RPM_sign_latch;
  temporalCounter_i1 = emDriveFOC_control_DW.temporalCounter_i1;
  ModeOut = emDriveFOC_control_DW.ModeOut;
  is_Run = emDriveFOC_control_DW.is_Run;

  /* Inport: '<Root>/Control' */
  /* During 'Run': '<S125>:179' */
  /* '<S125>:182:1' sf_internal_predicateOutput = ~RunStop; */
  if (!emDriveFOC_control_U.Control.RunStop) {
    /* Transition: '<S125>:182' */
    /* Exit Internal 'Run': '<S125>:179' */
    switch (is_Run) {
     case emDriveFOC_control_IN_Align:
      /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
      /* Exit 'Align': '<S125>:71' */
      /* '<S125>:71:10' RPM_ramp(RPM_ref,InitPos,true); */
      /* Simulink Function 'RPM_ramp': '<S125>:88' */
      emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
        &emDriveFOC_control_DW.InitPos_m, true, &BusCreator, &Switch,
        &RelationalOperator_l, &emDriveFOC_control_DW.OmegaEtoRPM_k,
        &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
        &emDriveFOC_control_DW.RunRPM_ramp);

      /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;

     case emDriveFOC_control_IN_CloseLoop:
      /* Exit 'CloseLoop': '<S125>:118' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;

     case emDriveFOC_control_IN_Hold:
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;

     case emDriveFOC_control_IN_OpenLoop:
      /* Exit 'OpenLoop': '<S125>:86' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;

     case emDriv_IN_TransitionToCloseLoop:
      /* Exit 'TransitionToCloseLoop': '<S125>:102' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;

     case emDrive_IN_TransitionToOpenLoop:
      /* Exit 'TransitionToOpenLoop': '<S125>:129' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;
      break;
    }

    /* Exit 'Run': '<S125>:179' */
    is_c3_emDriveFOC_control = emDriveFOC_control_IN_Stop;
    emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Stop;

    /* Entry 'Stop': '<S125>:1' */
    /*  here we wait for RunStop event */
    /* '<S125>:1:4' RPM_Track = false; */
    emDriveFOC_control_DW.RPM_Track_a = false;

    /* '<S125>:1:5' IdOut=single(0); */
    emDriveFOC_control_DW.IdOut = 0.0F;

    /* '<S125>:1:6' TrefOut=0; */
    emDriveFOC_control_DW.TrefOut = 0.0F;

    /* '<S125>:1:7' PosOutOpen.theta_r=0; */
    emDriveFOC_control_DW.PosOutOpen.theta_r = 0.0F;

    /* '<S125>:1:8' PosOutOpen.omega_r=0; */
    emDriveFOC_control_DW.PosOutOpen.omega_r = 0.0F;

    /* '<S125>:1:9' InitPos.theta_r=0; */
    emDriveFOC_control_DW.InitPos_m.theta_r = 0.0F;

    /* '<S125>:1:10' InitPos.omega_r=0; */
    emDriveFOC_control_DW.InitPos_m.omega_r = 0.0F;
  } else {
    guard1 = false;
    switch (is_Run) {
     case emDriveFOC_control_IN_Align:
      emDriveFOC_control_DW.RPM_Track_a = false;

      /* During 'Align': '<S125>:71' */
      /* '<S125>:143:1' sf_internal_predicateOutput = IdRampFinished; */
      if (emDriveFOC_control_DW.IdRampFinished) {
        /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
        /* Transition: '<S125>:143' */
        /* OpenLoop */
        /* Exit 'Align': '<S125>:71' */
        /* '<S125>:71:10' RPM_ramp(RPM_ref,InitPos,true); */
        /* Simulink Function 'RPM_ramp': '<S125>:88' */
        emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
          &emDriveFOC_control_DW.InitPos_m, true, &BusCreator, &Switch,
          &RelationalOperator_l, &emDriveFOC_control_DW.OmegaEtoRPM_k,
          &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunRPM_ramp);

        /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */
        is_Run = emDriveFOC_control_IN_Hold;
        temporalCounter_i1 = 0U;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Hold;
      } else {
        /*  here we increase Id to align rotor */
        /* '<S125>:71:8' [IdOut,IdRampFinished]=IdRamp(false); */
        *Start = 0.0;

        /* Outputs for Function Call SubSystem: '<S125>/Run.IdRamp' */
        /* Simulink Function 'IdRamp': '<S125>:79' */
        emDriveFOC_contro_RunIdRamp(*Start, Switch_d, RelationalOperator_h,
          &emDriveFOC_control_DW.Tsc, &emDriveFOC_control_DW.RunIdRamp);

        /* End of Outputs for SubSystem: '<S125>/Run.IdRamp' */
        emDriveFOC_control_DW.IdOut = *Switch_d;
        emDriveFOC_control_DW.IdRampFinished = *RelationalOperator_h;
      }
      break;

     case emDriveFOC_control_IN_CloseLoop:
      emDriveFOC_control_DW.RPM_Track_a = false;

      /* During 'CloseLoop': '<S125>:118' */
      /* '<S125>:130:1' sf_internal_predicateOutput = after(MinTimeInCloseLoop,tick) && (CloseLoopErrorCode ~= 0); */
      if ((temporalCounter_i1 >= (uint32_T)
           emDriveFOC_control_DW.MinTimeInCloseLoop) &&
          (emDriveFOC_control_DW.Err != 0)) {
        /* Transition: '<S125>:130' */
        /* '<S125>:210:1' sf_internal_predicateOutput = ~CloseLoopRPM_OK; */
        if (!emDriveFOC_control_DW.LogicalOperator) {
          /* Transition: '<S125>:210' */
          /* Exit 'CloseLoop': '<S125>:118' */
          is_Run = emDrive_IN_TransitionToOpenLoop;
          emDriveFOC_control_DW.SenzorlessState =
            enumSenzorlessState_TransitionToOpenLoop;

          /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
          /* Entry 'TransitionToOpenLoop': '<S125>:129' */
          /*  we have loos close loop position, transitioning back to open loop */
          /* '<S125>:129:4' [PosOutOpen,RPM_Open,OpenLoopFinished]=RPM_ramp(RPM_ref,PosCloseLoop,true); */
          /* Simulink Function 'RPM_ramp': '<S125>:88' */
          emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
            &emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo, true,
            &emDriveFOC_control_DW.PosOutOpen, &emDriveFOC_control_DW.RPM_Open,
            &RelationalOperator_l, &emDriveFOC_control_DW.OmegaEtoRPM_k,
            &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
            &emDriveFOC_control_DW.RunRPM_ramp);

          /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */
          /* '<S125>:129:5' ModeOut=1; */
          ModeOut = 1.0F;

          /* Outputs for Function Call SubSystem: '<S125>/Run.PosModeMix' */
          /* '<S125>:129:6' RPM_Track = false; */
          /* '<S125>:129:7' PosModeMix(single(1),PosOutOpen,PosCloseLoop,true); */
          /* Simulink Function 'PosModeMix': '<S125>:104' */
          emDriveFOC_co_RunPosModeMix(1.0F, &emDriveFOC_control_DW.PosOutOpen,
            &emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo, true,
            &BusCreator, &Switch, &emDriveFOC_control_DW.Tsc,
            &emDriveFOC_control_DW.RunPosModeMix);

          /* End of Outputs for SubSystem: '<S125>/Run.PosModeMix' */
        } else {
          /* Transition: '<S125>:250' */
          guard1 = true;
        }

        /* '<S125>:249:1' sf_internal_predicateOutput = RPM_sign_latch ~= RPM_sign; */
      } else if (RPM_sign_latch != emDriveFOC_control_DW.Sign) {
        /* Transition: '<S125>:249' */
        guard1 = true;
      } else {
        /*  close loop operation */
        /* '<S125>:118:7' PosOut = PosCloseLoop; */
        emDriveFOC_control_DW.PosOut =
          emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo;

        /* '<S125>:118:8' TrefOut = T_RefIn; */
        emDriveFOC_control_DW.TrefOut = emDriveFOC_control_DW.Switch2_h;

        /* '<S125>:118:9' sf_internal_predicateOutput = after(MinTimeInCloseLoop,tick); */
        if (temporalCounter_i1 >= (uint32_T)
            emDriveFOC_control_DW.MinTimeInCloseLoop) {
          /* '<S125>:118:10' PLL_SlowEnable = 0; */
          emDriveFOC_control_DW.PLL_SlowEnable = 0.0;
        }
      }
      break;

     case emDriveFOC_control_IN_Hold:
      /* During 'Hold': '<S125>:244' */
      /* '<S125>:245:1' sf_internal_predicateOutput = after(TimeInHold,tick); */
      if (temporalCounter_i1 >= (uint32_T)emDriveFOC_control_DW.TimeInHold) {
        /* Transition: '<S125>:245' */
        is_Run = emDriveFOC_control_IN_OpenLoop;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_OpenLoop;

        /* Entry 'OpenLoop': '<S125>:86' */
        /*  here we increase RPM */
        /* '<S125>:86:4' RPM_Track = true; */
        emDriveFOC_control_DW.RPM_Track_a = true;

        /* '<S125>:86:5' RPM_Open=0; */
        emDriveFOC_control_DW.RPM_Open = 0.0F;

        /* '<S125>:86:6' TrefOut=0; */
        emDriveFOC_control_DW.TrefOut = 0.0F;

        /* '<S125>:86:7' PLL_SlowEnable = 1; */
        emDriveFOC_control_DW.PLL_SlowEnable = 1.0;
      }
      break;

     case emDriveFOC_control_IN_OpenLoop:
      emDriveFOC_control_DW.RPM_Track_a = true;

      /* During 'OpenLoop': '<S125>:86' */
      /* '<S125>:103:1' sf_internal_predicateOutput = CloseLoopErrorCode == 0; */
      if (emDriveFOC_control_DW.Err == 0) {
        /* Transition: '<S125>:103' */
        /* Exit 'OpenLoop': '<S125>:86' */
        is_Run = emDriv_IN_TransitionToCloseLoop;
        emDriveFOC_control_DW.SenzorlessState =
          enumSenzorlessState_TransitionToCloseLoop;

        /* Entry 'TransitionToCloseLoop': '<S125>:102' */
        /*  mix position from open/close loop and Id, Tref fro open/close loop */
        /* '<S125>:102:4' ModeOut=0; */
        ModeOut = 0.0F;

        /* '<S125>:102:5' RPM_Track = false; */
        emDriveFOC_control_DW.RPM_Track_a = false;

        /* '<S125>:102:6' RPM_sign_latch = RPM_sign; */
        RPM_sign_latch = emDriveFOC_control_DW.Sign;

        /* Outputs for Function Call SubSystem: '<S125>/Run.PosModeMix' */
        /* '<S125>:102:7' PosModeMix(single(0),PosOutOpen,PosCloseLoop,true); */
        /* Simulink Function 'PosModeMix': '<S125>:104' */
        emDriveFOC_co_RunPosModeMix(0.0F, &emDriveFOC_control_DW.PosOutOpen,
          &emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo, true,
          &BusCreator, &Switch, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunPosModeMix);

        /* End of Outputs for SubSystem: '<S125>/Run.PosModeMix' */

        /* '<S125>:219:1' sf_internal_predicateOutput = abs(RPM_Open)>Ctrl_Sensorless__EndRPM; */
      } else if (fabsf(emDriveFOC_control_DW.RPM_Open) > Ctrl_Sensorless__EndRPM)
      {
        /* Transition: '<S125>:219' */
        /* Exit 'OpenLoop': '<S125>:86' */
        is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;

        /* Exit 'Run': '<S125>:179' */
        is_c3_emDriveFOC_control = emDriveFOC_control_IN_Error;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Error;

        /* Entry 'Error': '<S125>:184' */
        /*  Error state */
        /* '<S125>:184:4' RPM_Track = false; */
        emDriveFOC_control_DW.RPM_Track_a = false;

        /* '<S125>:184:5' IdOut=single(0); */
        emDriveFOC_control_DW.IdOut = 0.0F;

        /* '<S125>:184:6' TrefOut=0; */
        emDriveFOC_control_DW.TrefOut = 0.0F;
      } else {
        /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
        /*  here we increase RPM */
        /* '<S125>:86:9' [PosOutOpen,RPM_Open,OpenLoopFinished] = RPM_ramp(RPM_ref,InitPos,false); */
        /* Simulink Function 'RPM_ramp': '<S125>:88' */
        emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
          &emDriveFOC_control_DW.InitPos_m, false,
          &emDriveFOC_control_DW.PosOutOpen, &Switch, &RelationalOperator_l,
          &emDriveFOC_control_DW.OmegaEtoRPM_k,
          &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunRPM_ramp);

        /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */
        emDriveFOC_control_DW.RPM_Open = Switch;

        /* '<S125>:86:10' PosOut = PosOutOpen; */
        emDriveFOC_control_DW.PosOut = emDriveFOC_control_DW.PosOutOpen;
      }
      break;

     case emDriv_IN_TransitionToCloseLoop:
      emDriveFOC_control_DW.RPM_Track_a = false;

      /* During 'TransitionToCloseLoop': '<S125>:102' */
      /* '<S125>:119:1' sf_internal_predicateOutput = ModeOut == 1; */
      if (ModeOut == 1.0F) {
        /* Transition: '<S125>:119' */
        /* Exit 'TransitionToCloseLoop': '<S125>:102' */
        is_Run = emDriveFOC_control_IN_CloseLoop;
        temporalCounter_i1 = 0U;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_CloseLoop;

        /* Entry 'CloseLoop': '<S125>:118' */
        /*  close loop operation */
        /* '<S125>:118:4' IdOut=single(0); */
        emDriveFOC_control_DW.IdOut = 0.0F;

        /* '<S125>:118:5' RPM_Track = false; */

        /* '<S125>:248:1' sf_internal_predicateOutput = RPM_sign_latch ~= RPM_sign; */
      } else if (RPM_sign_latch != emDriveFOC_control_DW.Sign) {
        /* Transition: '<S125>:248' */
        /* Transition: '<S125>:251' */
        /* Exit 'TransitionToCloseLoop': '<S125>:102' */
        is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;

        /* Exit 'Run': '<S125>:179' */
        is_c3_emDriveFOC_control = emDriveFOC_control_IN_Error;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Error;

        /* Entry 'Error': '<S125>:184' */
        /*  Error state */
        /* '<S125>:184:4' RPM_Track = false; */
        /* '<S125>:184:5' IdOut=single(0); */
        emDriveFOC_control_DW.IdOut = 0.0F;

        /* '<S125>:184:6' TrefOut=0; */
        emDriveFOC_control_DW.TrefOut = 0.0F;
      } else {
        /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
        /*  mix position from open/close loop and Id, Tref fro open/close loop */
        /* '<S125>:102:9' [PosOutOpen,RPM_Open,OpenLoopFinished] = RPM_ramp(RPM_ref,InitPos,false); */
        /* Simulink Function 'RPM_ramp': '<S125>:88' */
        emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
          &emDriveFOC_control_DW.InitPos_m, false,
          &emDriveFOC_control_DW.PosOutOpen, &emDriveFOC_control_DW.RPM_Open,
          &RelationalOperator_l, &emDriveFOC_control_DW.OmegaEtoRPM_k,
          &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunRPM_ramp);

        /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */

        /* Outputs for Function Call SubSystem: '<S125>/Run.PosModeMix' */
        /* '<S125>:102:10' [PosOut,ModeOut]=PosModeMix(single(1),PosOutOpen,PosCloseLoop,false); */
        /* Simulink Function 'PosModeMix': '<S125>:104' */
        emDriveFOC_co_RunPosModeMix(1.0F, &emDriveFOC_control_DW.PosOutOpen,
          &emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo, false,
          &emDriveFOC_control_DW.PosOut, &Switch, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunPosModeMix);

        /* End of Outputs for SubSystem: '<S125>/Run.PosModeMix' */
        ModeOut = Switch;

        /* Outputs for Function Call SubSystem: '<S125>/Run.RefValueMix' */
        /* '<S125>:102:11' [TrefOut,IdOut] = RefValueMix(ModeOut,T_RefIn); */
        /* Simulink Function 'RefValueMix': '<S125>:112' */
        emDriveFOC_c_RunRefValueMix(Switch, emDriveFOC_control_DW.Switch2_h,
          &emDriveFOC_control_DW.TrefOut, &emDriveFOC_control_DW.IdOut);

        /* End of Outputs for SubSystem: '<S125>/Run.RefValueMix' */
      }
      break;

     default:
      emDriveFOC_control_DW.RPM_Track_a = false;

      /* During 'TransitionToOpenLoop': '<S125>:129' */
      /* '<S125>:131:1' sf_internal_predicateOutput = ModeOut == 0; */
      if (ModeOut == 0.0F) {
        /* Transition: '<S125>:131' */
        /* Exit 'TransitionToOpenLoop': '<S125>:129' */
        is_Run = emDriveFOC_control_IN_OpenLoop;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_OpenLoop;

        /* Entry 'OpenLoop': '<S125>:86' */
        /*  here we increase RPM */
        /* '<S125>:86:4' RPM_Track = true; */
        emDriveFOC_control_DW.RPM_Track_a = true;

        /* '<S125>:86:5' RPM_Open=0; */
        emDriveFOC_control_DW.RPM_Open = 0.0F;

        /* '<S125>:86:6' TrefOut=0; */
        emDriveFOC_control_DW.TrefOut = 0.0F;

        /* '<S125>:86:7' PLL_SlowEnable = 1; */
        emDriveFOC_control_DW.PLL_SlowEnable = 1.0;
      } else {
        /* Outputs for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
        /*  we have loos close loop position, transitioning back to open loop */
        /* '<S125>:129:9' [PosOutOpen,RPM_Open,OpenLoopFinished] = RPM_ramp(RPM_ref,InitPos,false); */
        /* Simulink Function 'RPM_ramp': '<S125>:88' */
        emDriveFOC_cont_RunRPM_ramp(emDriveFOC_control_DW.RateTransition2,
          &emDriveFOC_control_DW.InitPos_m, false,
          &emDriveFOC_control_DW.PosOutOpen, &emDriveFOC_control_DW.RPM_Open,
          &RelationalOperator_l, &emDriveFOC_control_DW.OmegaEtoRPM_k,
          &emDriveFOC_control_DW.RPMtoOmegaE_c, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunRPM_ramp);

        /* End of Outputs for SubSystem: '<S125>/Run.RPM_ramp' */

        /* Outputs for Function Call SubSystem: '<S125>/Run.PosModeMix' */
        /* '<S125>:129:10' [PosOut,ModeOut]=PosModeMix(single(0),PosOutOpen,PosCloseLoop,false); */
        /* Simulink Function 'PosModeMix': '<S125>:104' */
        emDriveFOC_co_RunPosModeMix(0.0F, &emDriveFOC_control_DW.PosOutOpen,
          &emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo, false,
          &emDriveFOC_control_DW.PosOut, &Switch, &emDriveFOC_control_DW.Tsc,
          &emDriveFOC_control_DW.RunPosModeMix);

        /* End of Outputs for SubSystem: '<S125>/Run.PosModeMix' */
        ModeOut = Switch;

        /* Outputs for Function Call SubSystem: '<S125>/Run.RefValueMix' */
        /* '<S125>:129:11' [TrefOut,IdOut] = RefValueMix(ModeOut,T_RefIn); */
        /* Simulink Function 'RefValueMix': '<S125>:112' */
        emDriveFOC_c_RunRefValueMix(Switch, emDriveFOC_control_DW.Switch2_h,
          &emDriveFOC_control_DW.TrefOut, &emDriveFOC_control_DW.IdOut);

        /* End of Outputs for SubSystem: '<S125>/Run.RefValueMix' */
      }
      break;
    }

    if (guard1) {
      /* Transition: '<S125>:251' */
      /* Exit 'CloseLoop': '<S125>:118' */
      is_Run = emDriveFOC_c_IN_NO_ACTIVE_CHILD;

      /* Exit 'Run': '<S125>:179' */
      is_c3_emDriveFOC_control = emDriveFOC_control_IN_Error;
      emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Error;

      /* Entry 'Error': '<S125>:184' */
      /*  Error state */
      /* '<S125>:184:4' RPM_Track = false; */
      /* '<S125>:184:5' IdOut=single(0); */
      emDriveFOC_control_DW.IdOut = 0.0F;

      /* '<S125>:184:6' TrefOut=0; */
      emDriveFOC_control_DW.TrefOut = 0.0F;
    }
  }

  /* End of Inport: '<Root>/Control' */
  emDriveFOC_control_DW.is_Run = is_Run;
  emDriveFOC_control_DW.ModeOut = ModeOut;
  emDriveFOC_control_DW.temporalCounter_i1 = temporalCounter_i1;
  emDriveFOC_control_DW.RPM_sign_latch = RPM_sign_latch;
  emDriveFOC_control_DW.is_c3_emDriveFOC_control = is_c3_emDriveFOC_control;
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<S118>/SenzorlessStateDigram' */
  FOC_SLOW_CODE
  static void SenzorlessStateDigram_Init(void)
{
  emDriveFOC_control_DW.PosOut.theta_r = 0.0F;
  emDriveFOC_control_DW.PosOut.omega_r = 0.0F;

  /* Chart: '<S118>/SenzorlessStateDigram' */
  /* Entry: Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram */
  /* Entry Internal: Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram */
  /* Transition: '<S125>:2' */
  emDriveFOC_control_DW.is_c3_emDriveFOC_control = emDriveFOC_control_IN_Stop;
  emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Stop;

  /* Entry 'Stop': '<S125>:1' */
  /*  here we wait for RunStop event */
  /* '<S125>:1:4' RPM_Track = false; */
  /* '<S125>:1:5' IdOut=single(0); */
  /* '<S125>:1:6' TrefOut=0; */
  /* '<S125>:1:7' PosOutOpen.theta_r=0; */
  emDriveFOC_control_DW.PosOutOpen.theta_r = 0.0F;

  /* '<S125>:1:8' PosOutOpen.omega_r=0; */
  emDriveFOC_control_DW.PosOutOpen.omega_r = 0.0F;

  /* '<S125>:1:9' InitPos.theta_r=0; */
  emDriveFOC_control_DW.InitPos_m.theta_r = 0.0F;

  /* '<S125>:1:10' InitPos.omega_r=0; */
  emDriveFOC_control_DW.InitPos_m.omega_r = 0.0F;

  /* SystemInitialize for Function Call SubSystem: '<S125>/Run.IdRamp' */
  emDriveFOC_c_RunIdRamp_Init(&emDriveFOC_control_DW.RunIdRamp);

  /* End of SystemInitialize for SubSystem: '<S125>/Run.IdRamp' */

  /* SystemInitialize for Function Call SubSystem: '<S125>/Run.RPM_ramp' */
  emDriveFOC_RunRPM_ramp_Init(&emDriveFOC_control_DW.RunRPM_ramp);

  /* End of SystemInitialize for SubSystem: '<S125>/Run.RPM_ramp' */

  /* SystemInitialize for Function Call SubSystem: '<S125>/Run.PosModeMix' */
  emDriveF_RunPosModeMix_Init(&emDriveFOC_control_DW.RunPosModeMix);

  /* End of SystemInitialize for SubSystem: '<S125>/Run.PosModeMix' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S118>/SenzorlessStateDigram' */
  FOC_FAST_CODE
  static void emDri_SenzorlessStateDigram(void)
{
  real_T Start;
  real32_T Switch_d;
  boolean_T RelationalOperator_h;

  /* Gateway: Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram */
  if (emDriveFOC_control_DW.temporalCounter_i1 < MAX_uint32_T) {
    emDriveFOC_control_DW.temporalCounter_i1++;
  }

  /* During: Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram */
  switch (emDriveFOC_control_DW.is_c3_emDriveFOC_control) {
   case emDriveFOC_control_IN_Error:
    emDriveFOC_control_DW.RPM_Track_a = false;

    /* Inport: '<Root>/Control' */
    /* During 'Error': '<S125>:184' */
    /* '<S125>:185:1' sf_internal_predicateOutput = ~RunStop; */
    if (!emDriveFOC_control_U.Control.RunStop) {
      /* Transition: '<S125>:185' */
      /* Exit 'Error': '<S125>:184' */
      emDriveFOC_control_DW.is_c3_emDriveFOC_control =
        emDriveFOC_control_IN_Stop;
      emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Stop;

      /* Entry 'Stop': '<S125>:1' */
      /*  here we wait for RunStop event */
      /* '<S125>:1:4' RPM_Track = false; */
      /* '<S125>:1:5' IdOut=single(0); */
      emDriveFOC_control_DW.IdOut = 0.0F;

      /* '<S125>:1:6' TrefOut=0; */
      emDriveFOC_control_DW.TrefOut = 0.0F;

      /* '<S125>:1:7' PosOutOpen.theta_r=0; */
      emDriveFOC_control_DW.PosOutOpen.theta_r = 0.0F;

      /* '<S125>:1:8' PosOutOpen.omega_r=0; */
      emDriveFOC_control_DW.PosOutOpen.omega_r = 0.0F;

      /* '<S125>:1:9' InitPos.theta_r=0; */
      emDriveFOC_control_DW.InitPos_m.theta_r = 0.0F;

      /* '<S125>:1:10' InitPos.omega_r=0; */
      emDriveFOC_control_DW.InitPos_m.omega_r = 0.0F;

      /*  Error state */
      /* '<S125>:184:8' if CloseLoopErrorCode == 0 */
    } else if (emDriveFOC_control_DW.Err == 0) {
      /* '<S125>:184:9' PosOut = PosCloseLoop; */
      emDriveFOC_control_DW.PosOut =
        emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo;
    } else {
      /* '<S125>:184:10' else */
      /* '<S125>:184:11' PosOut.theta_r=0; */
      emDriveFOC_control_DW.PosOut.theta_r = 0.0F;

      /* '<S125>:184:12' PosOut.omega_r=0; */
      emDriveFOC_control_DW.PosOut.omega_r = 0.0F;
    }
    break;

   case emDriveFOC_control_IN_Run:
    emDriveFOC_control_Run(&Start, &Switch_d, &RelationalOperator_h);
    break;

   default:
    emDriveFOC_control_DW.RPM_Track_a = false;

    /* Inport: '<Root>/Control' */
    /* During 'Stop': '<S125>:1' */
    /* '<S125>:72:1' sf_internal_predicateOutput = RunStop; */
    if (emDriveFOC_control_U.Control.RunStop) {
      /* Transition: '<S125>:72' */
      /* Exit 'Stop': '<S125>:1' */
      emDriveFOC_control_DW.is_c3_emDriveFOC_control = emDriveFOC_control_IN_Run;

      /* Entry 'Run': '<S125>:179' */
      /* Entry Internal 'Run': '<S125>:179' */
      /* Transition: '<S125>:226' */
      /* '<S125>:74:1' sf_internal_predicateOutput = CloseLoopErrorCode ~= 0; */
      if (emDriveFOC_control_DW.Err != 0) {
        /* Transition: '<S125>:74' */
        emDriveFOC_control_DW.is_Run = emDriveFOC_control_IN_Align;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_Align;

        /* Outputs for Function Call SubSystem: '<S125>/Run.IdRamp' */
        /* Entry 'Align': '<S125>:71' */
        /*  here we increase Id to align rotor */
        /* '<S125>:71:4' IdRamp(true); */
        /* Simulink Function 'IdRamp': '<S125>:79' */
        emDriveFOC_contro_RunIdRamp(1.0, &Switch_d, &RelationalOperator_h,
          &emDriveFOC_control_DW.Tsc, &emDriveFOC_control_DW.RunIdRamp);

        /* End of Outputs for SubSystem: '<S125>/Run.IdRamp' */
        /*  intialise Id ramp */
        /* '<S125>:71:5' IdRampFinished = false; */
        emDriveFOC_control_DW.IdRampFinished = false;

        /* '<S125>:71:6' RPM_Track = false; */
      } else {
        /* Transition: '<S125>:134' */
        emDriveFOC_control_DW.is_Run = emDriveFOC_control_IN_CloseLoop;
        emDriveFOC_control_DW.temporalCounter_i1 = 0U;
        emDriveFOC_control_DW.SenzorlessState = enumSenzorlessState_CloseLoop;

        /* Entry 'CloseLoop': '<S125>:118' */
        /*  close loop operation */
        /* '<S125>:118:4' IdOut=single(0); */
        emDriveFOC_control_DW.IdOut = 0.0F;

        /* '<S125>:118:5' RPM_Track = false; */
      }

      /*  here we wait for RunStop event */
      /* '<S125>:1:12' if CloseLoopErrorCode == 0 */
    } else if (emDriveFOC_control_DW.Err == 0) {
      /* '<S125>:1:13' PosOut = PosCloseLoop; */
      emDriveFOC_control_DW.PosOut =
        emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo;
    } else {
      /* '<S125>:1:14' else */
      /* '<S125>:1:15' PosOut.theta_r=0; */
      emDriveFOC_control_DW.PosOut.theta_r = 0.0F;

      /* '<S125>:1:16' PosOut.omega_r=0; */
      emDriveFOC_control_DW.PosOut.omega_r = 0.0F;
    }
    break;
  }
}

FOC_RESTORE_CODE
  /* System initialize for enable system: '<S41>/Sensorless' */
  FOC_SLOW_CODE
  static void emDriveFOC__Sensorless_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: '<S118>/CloseLoopMode' */
  /* SystemInitialize for Atomic SubSystem: '<S127>/LPF_omega' */
  /* InitializeConditions for Delay: '<S136>/Delay' */
  emDriveFOC_control_DW.icLoad_d = true;

  /* End of SystemInitialize for SubSystem: '<S127>/LPF_omega' */

  /* SystemInitialize for Atomic SubSystem: '<S127>/ThetaPLL' */
  /* InitializeConditions for Delay: '<S143>/Delay' */
  emDriveFOC_control_DW.icLoad_e = true;

  /* End of SystemInitialize for SubSystem: '<S127>/ThetaPLL' */

  /* SystemInitialize for Atomic SubSystem: '<S122>/Sensorless_valid' */
  /* SystemInitialize for Atomic SubSystem: '<S158>/LPF_BackEMF' */
  /* InitializeConditions for Delay: '<S168>/Delay' */
  emDriveFOC_control_DW.icLoad_b = true;

  /* End of SystemInitialize for SubSystem: '<S158>/LPF_BackEMF' */

  /* SystemInitialize for Atomic SubSystem: '<S161>/LPF_PLLerr' */
  /* InitializeConditions for Delay: '<S183>/Delay' */
  emDriveFOC_control_DW.icLoad_fe = true;

  /* End of SystemInitialize for SubSystem: '<S161>/LPF_PLLerr' */
  /* End of SystemInitialize for SubSystem: '<S122>/Sensorless_valid' */
  /* End of SystemInitialize for SubSystem: '<S118>/CloseLoopMode' */

  /* SystemInitialize for Chart: '<S118>/SenzorlessStateDigram' */
  SenzorlessStateDigram_Init();
}

FOC_RESTORE_CODE
  /* Output and update for enable system: '<S41>/Sensorless' */
  FOC_FAST_CODE
  static void emDriveFOC_contr_Sensorless(void)
{
  int32_T i;
  real32_T Ctrl_Sensorless_StatusBEMF_RPM_;
  real32_T Delay_DSTATE_f;
  real32_T Delay_DSTATE_hv;
  real32_T Delay_DSTATE_oc;
  real32_T Delay_DSTATE_oq;
  real32_T UnitDelay2_2_DSTATE;
  real32_T UnitDelay3_DSTATE;
  real32_T UnitDelay_DSTATE_fm;
  real32_T rtb_Saturation_i_idx_0;
  real32_T rtb_Saturation_i_idx_1;
  real32_T rtb_y_g;
  real32_T temp;
  real32_T tmp;
  int8_T rtb_DataTypeConversion[6];
  boolean_T rtb_LogicalOperator1;
  boolean_T rtb_LogicalOperator4;
  boolean_T rtb_LogicalOperator_pi;
  boolean_T rtb_RelationalOperator1_fm;
  boolean_T rtb_status_RPM_f;
  boolean_T rtb_status_omega;

  /* Sum: '<S183>/Add' */
  Delay_DSTATE_hv = emDriveFOC_control_DW.Delay_DSTATE_hv;

  /* Sum: '<S168>/Add' */
  Delay_DSTATE_f = emDriveFOC_control_DW.Delay_DSTATE_f;

  /* Outputs for Enabled SubSystem: '<S41>/Sensorless' incorporates:
   *  EnablePort: '<S118>/Enable'
   */
  /* Outputs for Atomic SubSystem: '<S118>/CloseLoopMode' */
  /* Outputs for Atomic SubSystem: '<S127>/ThetaPLL' */
  /* Sum: '<S144>/Add2' */
  UnitDelay_DSTATE_fm = emDriveFOC_control_DW.UnitDelay_DSTATE_fm;

  /* End of Outputs for SubSystem: '<S127>/ThetaPLL' */
  /* End of Outputs for SubSystem: '<S118>/CloseLoopMode' */
  /* End of Outputs for SubSystem: '<S41>/Sensorless' */

  /* Update for UnitDelay generated from: '<S118>/Unit Delay2' */
  UnitDelay2_2_DSTATE = emDriveFOC_control_DW.UnitDelay2_2_DSTATE;
  Delay_DSTATE_oc = emDriveFOC_control_DW.Delay_DSTATE_oc;

  /* Switch: '<S136>/Switch' */
  Delay_DSTATE_oq = emDriveFOC_control_DW.Delay_DSTATE_oq;

  /* Outputs for Enabled SubSystem: '<S41>/Sensorless' incorporates:
   *  EnablePort: '<S118>/Enable'
   */
  /* Outputs for Atomic SubSystem: '<S118>/CloseLoopMode' */
  /* Product: '<S127>/Product' */
  UnitDelay3_DSTATE = emDriveFOC_control_DW.UnitDelay3_DSTATE;

  /* End of Outputs for SubSystem: '<S118>/CloseLoopMode' */

  /* Constant: '<S41>/EN_sensorless1' */
  if (Ctrl_Sensorless__Enabled) {
    /* Outputs for Atomic SubSystem: '<S118>/CloseLoopMode' */
    /* SignalConversion: '<S133>/Signal Conversion' */
    Ctrl_Sensorless_StatusBEMF_IabIn[0] = emDriveFOC_control_DW.DotProduct;
    Ctrl_Sensorless_StatusBEMF_IabIn[1] = emDriveFOC_control_DW.DotProduct1;

    /* Saturate: '<S131>/Saturation' incorporates:
     *  UnitDelay: '<S131>/Unit Delay'
     */
    if (emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] > 1.0E+6F) {
      Ctrl_Sensorless_StatusBEMF_IabEst[0] = 1.0E+6F;
    } else if (emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] < -1.0E+6F) {
      Ctrl_Sensorless_StatusBEMF_IabEst[0] = -1.0E+6F;
    } else {
      Ctrl_Sensorless_StatusBEMF_IabEst[0] =
        emDriveFOC_control_DW.UnitDelay_DSTATE_b[0];
    }

    /* Saturate: '<S132>/Saturation' incorporates:
     *  UnitDelay: '<S132>/Unit Delay'
     */
    if (emDriveFOC_control_DW.UnitDelay_DSTATE_f[0] > 1.0E+6F) {
      rtb_Saturation_i_idx_0 = 1.0E+6F;
    } else if (emDriveFOC_control_DW.UnitDelay_DSTATE_f[0] < -1.0E+6F) {
      rtb_Saturation_i_idx_0 = -1.0E+6F;
    } else {
      rtb_Saturation_i_idx_0 = emDriveFOC_control_DW.UnitDelay_DSTATE_f[0];
    }

    /* SignalConversion: '<S134>/Signal Conversion' incorporates:
     *  UnitDelay: '<S2>/Unit Delay1'
     */
    Ctrl_Sensorless_StatusBEMF_UabIn[0] =
      emDriveFOC_control_DW.UnitDelay1_DSTATE_n[0];

    /* Saturate: '<S131>/Saturation' incorporates:
     *  UnitDelay: '<S131>/Unit Delay'
     */
    if (emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] > 1.0E+6F) {
      Ctrl_Sensorless_StatusBEMF_IabEst[1] = 1.0E+6F;
    } else if (emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] < -1.0E+6F) {
      Ctrl_Sensorless_StatusBEMF_IabEst[1] = -1.0E+6F;
    } else {
      Ctrl_Sensorless_StatusBEMF_IabEst[1] =
        emDriveFOC_control_DW.UnitDelay_DSTATE_b[1];
    }

    /* Saturate: '<S132>/Saturation' incorporates:
     *  UnitDelay: '<S132>/Unit Delay'
     */
    if (emDriveFOC_control_DW.UnitDelay_DSTATE_f[1] > 1.0E+6F) {
      rtb_Saturation_i_idx_1 = 1.0E+6F;
    } else if (emDriveFOC_control_DW.UnitDelay_DSTATE_f[1] < -1.0E+6F) {
      rtb_Saturation_i_idx_1 = -1.0E+6F;
    } else {
      rtb_Saturation_i_idx_1 = emDriveFOC_control_DW.UnitDelay_DSTATE_f[1];
    }

    /* SignalConversion: '<S134>/Signal Conversion' incorporates:
     *  UnitDelay: '<S2>/Unit Delay1'
     */
    Ctrl_Sensorless_StatusBEMF_UabIn[1] =
      emDriveFOC_control_DW.UnitDelay1_DSTATE_n[1];

    /* Sum: '<S126>/Subtract' incorporates:
     *  Saturate: '<S131>/Saturation'
     */
    Ctrl_Sensorless_StatusBEMF_IabErr[0] = Ctrl_Sensorless_StatusBEMF_IabEst[0]
      - emDriveFOC_control_DW.DotProduct;
    Ctrl_Sensorless_StatusBEMF_IabErr[1] = Ctrl_Sensorless_StatusBEMF_IabEst[1]
      - emDriveFOC_control_DW.DotProduct1;

    /* MATLAB Function: '<S126>/BEMF_Observer' incorporates:
     *  Constant: '<S126>/Constant1'
     *  Constant: '<S126>/Constant3'
     *  Constant: '<S126>/Constant4'
     *  Constant: '<S126>/Constant5'
     *  Constant: '<S126>/Constant6'
     */
    /* MATLAB Function 'Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/BEMF_Observer': '<S130>:1' */
    /* '<S130>:1:2' de_est = single(zeros(2,1)); */
    /* '<S130>:1:3' di_est = single(zeros(2,1)); */
    /* as in % look in STM UM1052 but extended with IPM term Omega*(Lq-Ld)*i_est */
    /* '<S130>:1:7' de_est(1) = K2*Ierr(1) - 1 *Egain* Omega*e_est(2); */
    Ctrl_Sensorless_StatusBEMF_EabEst[0] = Ctrl_Sensorless__ObserverK2 *
      Ctrl_Sensorless_StatusBEMF_IabErr[0];

    /* '<S130>:1:8' de_est(2) = K2*Ierr(2) + 1 *Egain* Omega*e_est(1); */
    Ctrl_Sensorless_StatusBEMF_EabEst[1] = Ctrl_Sensorless__ObserverK2 *
      Ctrl_Sensorless_StatusBEMF_IabErr[1];

    /* ea_est_next = ea_est + K2*dt*(ia_est-ia) - 0.5* dt*d_OmegaPLL*eb_est; */
    /* eb_est_next = eb_est + K2*dt*(ib_est-ib) + 0.5* dt*d_OmegaPLL*ea_est; */
    /* di_est(1) =  ((Rs*Iab(1) - K1*Ierr(1) - e_est(1) + Uab(1) + Omega*(Lq-Ld)*Iab(2))/Ld); */
    /* di_est(2) =  ((Rs*Iab(2) - K1*Ierr(2) - e_est(2) + Uab(2) - Omega*(Lq-Ld)*Iab(1))/Ld); */
    /*  not clear why MotorRs has + sign, by theory - is correct */
    /* '<S130>:1:17' di_est(1) =  ((-Rs*Iab(1) - K1*Ierr(1) - e_est(1) + Uab(1) + Omega*(Lq-Ld)*Iab(2))/Ld); */
    temp = (Ctrl_Motor__Lq - Ctrl_Motor__Ld) * UnitDelay3_DSTATE;
    emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] = ((((-Ctrl_Motor__Rs *
      Ctrl_Sensorless_StatusBEMF_IabIn[0] - Ctrl_Sensorless__ObserverK1 *
      Ctrl_Sensorless_StatusBEMF_IabErr[0]) - rtb_Saturation_i_idx_0) +
      Ctrl_Sensorless_StatusBEMF_UabIn[0]) + temp *
      Ctrl_Sensorless_StatusBEMF_IabIn[1]) / Ctrl_Motor__Ld;

    /* '<S130>:1:18' di_est(2) =  ((-Rs*Iab(2) - K1*Ierr(2) - e_est(2) + Uab(2) - Omega*(Lq-Ld)*Iab(1))/Ld); */
    emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] = ((((-Ctrl_Motor__Rs *
      Ctrl_Sensorless_StatusBEMF_IabIn[1] - Ctrl_Sensorless__ObserverK1 *
      Ctrl_Sensorless_StatusBEMF_IabErr[1]) - rtb_Saturation_i_idx_1) +
      Ctrl_Sensorless_StatusBEMF_UabIn[1]) - temp *
      Ctrl_Sensorless_StatusBEMF_IabIn[0]) / Ctrl_Motor__Ld;

    /* Sum: '<S132>/Add2' incorporates:
     *  DataStoreRead: '<S126>/Data Store Read1'
     *  Product: '<S132>/Product'
     *  Saturate: '<S132>/Saturation'
     */
    /* dIab(1)=(Uab(1)-R*Iab(1)+omega*(Lq-Ld)*Iab(2)-Z(1))/Ld;   */
    /* dIab(2)=(Uab(2)-R*Iab(2)-omega*(Lq-Ld)*Iab(1)-Z(2))/Ld;  */
    /* ia_est_next = ia_est  - ((MotorRs*dt)/MotorLs)*ia_est + K1*dt*(ia_est-ia) - dt/MotorLs*ea_est + dt/MotorLs*va; */
    /* ib_est_next = ib_est  - ((MotorRs*dt)/MotorLs)*ib_est + K1*dt*(ib_est-ib) - dt/MotorLs*eb_est + dt/MotorLs*vb; */
    Ctrl_Sensorless_StatusBEMF_EabEst[0] = Ctrl_Sensorless_StatusBEMF_EabEst[0] *
      emDriveFOC_control_DW.Tsc + rtb_Saturation_i_idx_0;
    Ctrl_Sensorless_StatusBEMF_EabEst[1] = Ctrl_Sensorless_StatusBEMF_EabEst[1] *
      emDriveFOC_control_DW.Tsc + rtb_Saturation_i_idx_1;

    /* Trigonometry: '<S127>/Atan2' incorporates:
     *  UnaryMinus: '<S127>/Unary Minus1'
     */
    rtb_Saturation_i_idx_0 = emDriveAtan2(Ctrl_Sensorless_StatusBEMF_EabEst[0],
      -Ctrl_Sensorless_StatusBEMF_EabEst[1]);

    /* Logic: '<S127>/Logical Operator' incorporates:
     *  Abs: '<S127>/Abs'
     *  Constant: '<S127>/Constant1'
     *  DataStoreRead: '<S127>/Data Store Read4'
     *  Delay: '<S127>/Delay One Step'
     *  Product: '<S127>/Product1'
     *  RelationalOperator: '<S127>/Relational Operator'
     *  UnitDelay: '<S118>/Unit Delay7'
     */
    rtb_LogicalOperator_pi = ((emDriveFOC_control_DW.UnitDelay7_DSTATE != 0.0) ||
      (fabsf(emDriveFOC_control_DW.DelayOneStep_DSTATE_m) <=
       emDriveFOC_control_DW.RPMtoOmegaE_c * Ctrl_Sensorless__PLLBWSpeedSwitch));

    /* Product: '<S127>/Product' incorporates:
     *  Constant: '<S127>/Constant3'
     *  DataStoreRead: '<S127>/Data Store Read2'
     */
    UnitDelay3_DSTATE = Ctrl_Gen__MaxRPMcalculations *
      emDriveFOC_control_DW.RPMtoOmegaE_c;

    /* Logic: '<S122>/Logical Operator1' incorporates:
     *  Inport: '<Root>/Control'
     */
    rtb_LogicalOperator1 = !emDriveFOC_control_U.Control.RunStop;

    /* Outputs for Atomic SubSystem: '<S127>/ThetaPLL' */
    /* Delay: '<S143>/Delay' */
    if (emDriveFOC_control_DW.icLoad_e) {
      Delay_DSTATE_oc = rtb_Saturation_i_idx_0;
    }

    rtb_Saturation_i_idx_1 = Delay_DSTATE_oc;

    /* End of Delay: '<S143>/Delay' */

    /* Outputs for Atomic SubSystem: '<S138>/ThetaSub' */
    /* MATLAB Function: '<S146>/ThetaSub' */
    /* MATLAB Function 'ThetaSub/ThetaSub': '<S149>:1' */
    /* '<S149>:1:3' y = u1-u2; */
    rtb_y_g = rtb_Saturation_i_idx_0 - Delay_DSTATE_oc;

    /* '<S149>:1:4' if y > single(pi) */
    if (rtb_y_g > 3.14159274F) {
      /* '<S149>:1:5' y=y-single(2*pi); */
      rtb_y_g -= 6.28318548F;
    }

    /* '<S149>:1:7' if y < single(-pi) */
    if (rtb_y_g < -3.14159274F) {
      /* '<S149>:1:8' y=y+single(2*pi); */
      rtb_y_g += 6.28318548F;
    }

    /* End of MATLAB Function: '<S146>/ThetaSub' */
    /* End of Outputs for SubSystem: '<S138>/ThetaSub' */
    /* End of Outputs for SubSystem: '<S127>/ThetaPLL' */

    /* Switch generated from: '<S127>/Switch' */
    if (rtb_LogicalOperator_pi) {
      temp = emDriveFOC_control_DW.K1;
    } else {
      temp = emDriveFOC_control_DW.K1_g;
    }

    /* Outputs for Atomic SubSystem: '<S127>/ThetaPLL' */
    /* Gain: '<S129>/Gain' incorporates:
     *  Product: '<S138>/K1'
     *  Switch generated from: '<S127>/Switch'
     */
    Ctrl_Sensorless_StatusBEMF_PLL_Err = rtb_y_g * temp;

    /* Saturate: '<S144>/Saturation' incorporates:
     *  DataStoreRead: '<S127>/Data Store Read1'
     *  Product: '<S144>/Product'
     *  Sum: '<S144>/Add2'
     */
    if (UnitDelay_DSTATE_fm > 1.0E+6F) {
      UnitDelay_DSTATE_fm = 1.0E+6F;
    } else if (UnitDelay_DSTATE_fm < -1.0E+6F) {
      UnitDelay_DSTATE_fm = -1.0E+6F;
    }

    UnitDelay_DSTATE_fm += Ctrl_Sensorless_StatusBEMF_PLL_Err *
      emDriveFOC_control_DW.Tsc;

    /* Switch: '<S145>/Switch2' incorporates:
     *  RelationalOperator: '<S145>/LowerRelop1'
     */
    if (UnitDelay_DSTATE_fm <= UnitDelay3_DSTATE) {
      /* Switch: '<S145>/Switch' incorporates:
       *  RelationalOperator: '<S145>/UpperRelop'
       *  UnaryMinus: '<S138>/Unary Minus'
       */
      if (UnitDelay_DSTATE_fm < -UnitDelay3_DSTATE) {
        /* Product: '<S127>/Product' */
        UnitDelay3_DSTATE = -UnitDelay3_DSTATE;
      } else {
        /* Product: '<S127>/Product' */
        UnitDelay3_DSTATE = UnitDelay_DSTATE_fm;
      }

      /* End of Switch: '<S145>/Switch' */
    }

    /* End of Switch: '<S145>/Switch2' */
    /* End of Outputs for SubSystem: '<S127>/ThetaPLL' */

    /* Switch generated from: '<S127>/Switch' */
    /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S148>:1' */
    /* '<S148>:1:3' y = u1+u2; */
    if (rtb_LogicalOperator_pi) {
      temp = emDriveFOC_control_DW.K2;
    } else {
      temp = emDriveFOC_control_DW.K2_g;
    }

    /* Outputs for Atomic SubSystem: '<S127>/ThetaPLL' */
    /* Outputs for Atomic SubSystem: '<S143>/ThetaAdd' */
    /* MATLAB Function: '<S147>/ThetaAdd' incorporates:
     *  DataStoreRead: '<S127>/Data Store Read1'
     *  Product: '<S138>/K2'
     *  Product: '<S143>/Product'
     *  Sum: '<S138>/Add2'
     *  Switch generated from: '<S127>/Switch'
     */
    Delay_DSTATE_oc += (Ctrl_Sensorless_StatusBEMF_PLL_Err * temp +
                        UnitDelay3_DSTATE) * emDriveFOC_control_DW.Tsc;

    /* '<S148>:1:4' if y > single(pi) */
    if (Delay_DSTATE_oc > 3.14159274F) {
      /* '<S148>:1:5' y=y-single(2*pi); */
      Delay_DSTATE_oc -= 6.28318548F;
    }

    /* '<S148>:1:7' if y < single(-pi) */
    if (Delay_DSTATE_oc < -3.14159274F) {
      /* '<S148>:1:8' y=y+single(2*pi); */
      Delay_DSTATE_oc += 6.28318548F;
    }

    /* End of MATLAB Function: '<S147>/ThetaAdd' */
    /* End of Outputs for SubSystem: '<S143>/ThetaAdd' */

    /* Update for Delay: '<S143>/Delay' */
    emDriveFOC_control_DW.icLoad_e = false;

    /* Switch: '<S144>/Switch' */
    if (rtb_LogicalOperator1) {
      /* Saturate: '<S144>/Saturation' incorporates:
       *  UnitDelay: '<S144>/Unit Delay'
       */
      UnitDelay_DSTATE_fm = 0.0F;
    }

    /* End of Switch: '<S144>/Switch' */
    /* End of Outputs for SubSystem: '<S127>/ThetaPLL' */

    /* Outputs for Atomic SubSystem: '<S127>/LPF_omega' */
    /* Delay: '<S136>/Delay' */
    if (emDriveFOC_control_DW.icLoad_d) {
      /* Switch: '<S136>/Switch' */
      Delay_DSTATE_oq = UnitDelay3_DSTATE;
    }

    /* End of Delay: '<S136>/Delay' */

    /* Switch: '<S136>/Switch' incorporates:
     *  Product: '<S136>/Product'
     *  Sum: '<S136>/Add'
     *  Sum: '<S136>/Add1'
     */
    Delay_DSTATE_oq += (UnitDelay3_DSTATE - Delay_DSTATE_oq) *
      emDriveFOC_control_DW.alpha;

    /* Switch: '<S136>/Switch1' */
    if (emDriveFOC_control_DW.alpha != 0.0F) {
      rtb_Saturation_i_idx_0 = Delay_DSTATE_oq;
    } else {
      rtb_Saturation_i_idx_0 = UnitDelay3_DSTATE;
    }

    /* End of Switch: '<S136>/Switch1' */

    /* Update for Delay: '<S136>/Delay' */
    emDriveFOC_control_DW.icLoad_d = false;

    /* End of Outputs for SubSystem: '<S127>/LPF_omega' */

    /* Outputs for Atomic SubSystem: '<S127>/CompensateDelay' */
    /* Outputs for Atomic SubSystem: '<S135>/ThetaAdd' */
    /* MATLAB Function: '<S140>/ThetaAdd' incorporates:
     *  Product: '<S135>/Product'
     */
    /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S141>:1' */
    /* '<S141>:1:3' y = u1+u2; */
    rtb_Saturation_i_idx_1 += rtb_Saturation_i_idx_0 *
      emDriveFOC_control_DW.Delay;

    /* '<S141>:1:4' if y > single(pi) */
    if (rtb_Saturation_i_idx_1 > 3.14159274F) {
      /* '<S141>:1:5' y=y-single(2*pi); */
      rtb_Saturation_i_idx_1 -= 6.28318548F;
    }

    /* '<S141>:1:7' if y < single(-pi) */
    if (rtb_Saturation_i_idx_1 < -3.14159274F) {
      /* '<S141>:1:8' y=y+single(2*pi); */
      rtb_Saturation_i_idx_1 += 6.28318548F;
    }

    /* End of MATLAB Function: '<S140>/ThetaAdd' */
    /* End of Outputs for SubSystem: '<S135>/ThetaAdd' */
    /* End of Outputs for SubSystem: '<S127>/CompensateDelay' */

    /* Signum: '<S127>/Sign' */
    /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S142>:1' */
    /* '<S142>:1:3' y = u1+u2; */
    /* '<S142>:1:4' if y > single(pi) */
    /* '<S142>:1:7' if y < single(-pi) */
    /* MATLAB Function 'ThetaSub/ThetaSub': '<S150>:1' */
    /* '<S150>:1:3' y = u1-u2; */
    if (rtb_Saturation_i_idx_0 < 0.0F) {
      temp = -1.0F;
    } else {
      temp = (real32_T)(rtb_Saturation_i_idx_0 > 0.0F);
    }

    /* Outputs for Atomic SubSystem: '<S127>/ThetaSub' */
    /* Outputs for Atomic SubSystem: '<S127>/ThetaAdd' */
    /* MATLAB Function: '<S139>/ThetaSub' incorporates:
     *  Constant: '<S127>/Constant2'
     *  Gain: '<S127>/Gain'
     *  MATLAB Function: '<S137>/ThetaAdd'
     *  Signum: '<S127>/Sign'
     */
    rtb_Saturation_i_idx_1 -= 1.57079637F * temp + 1.57079637F;

    /* End of Outputs for SubSystem: '<S127>/ThetaAdd' */
    /* '<S150>:1:4' if y > single(pi) */
    if (rtb_Saturation_i_idx_1 > 3.14159274F) {
      /* '<S150>:1:5' y=y-single(2*pi); */
      rtb_Saturation_i_idx_1 -= 6.28318548F;
    }

    /* '<S150>:1:7' if y < single(-pi) */
    if (rtb_Saturation_i_idx_1 < -3.14159274F) {
      /* '<S150>:1:8' y=y+single(2*pi); */
      rtb_Saturation_i_idx_1 += 6.28318548F;
    }

    /* End of MATLAB Function: '<S139>/ThetaSub' */
    /* End of Outputs for SubSystem: '<S127>/ThetaSub' */

    /* SignalConversion: '<S154>/Signal Conversion' incorporates:
     *  Gain: '<S128>/Gain1'
     *  Gain: '<S128>/Gain2'
     */
    Ctrl_Sensorless_StatusBEMF_Theta[0] = 57.2957802F * rtb_Saturation_i_idx_1;
    Ctrl_Sensorless_StatusBEMF_Theta[1] = 57.2957802F *
      emDriveFOC_control_DW.theta_r;
    Ctrl_Sensorless_StatusBEMF_Theta[2] = 0.0F;

    /* Outputs for Atomic SubSystem: '<S122>/Sensorless_valid' */
    /* Product: '<S128>/Product' incorporates:
     *  DataStoreRead: '<S128>/Data Store Read2'
     *  Product: '<S129>/Product1'
     */
    Ctrl_Sensorless_StatusBEMF_RPM_ = emDriveFOC_control_DW.OmegaEtoRPM_k *
      rtb_Saturation_i_idx_0;

    /* End of Outputs for SubSystem: '<S122>/Sensorless_valid' */

    /* SignalConversion: '<S155>/Signal Conversion' incorporates:
     *  DataStoreRead: '<S128>/Data Store Read1'
     *  Product: '<S128>/Product'
     *  Product: '<S128>/Product1'
     */
    Ctrl_Sensorless_StatusBEMF_RPM[0] = Ctrl_Sensorless_StatusBEMF_RPM_;
    Ctrl_Sensorless_StatusBEMF_RPM[1] = emDriveFOC_control_DW.omega_r_c *
      emDriveFOC_control_DW.OmegaEtoRPM_k;

    /* Switch: '<S131>/Switch' */
    /* MATLAB Function 'ThetaSub/ThetaSub': '<S157>:1' */
    /* '<S157>:1:3' y = u1-u2; */
    /* '<S157>:1:4' if y > single(pi) */
    /* '<S157>:1:7' if y < single(-pi) */
    if (rtb_LogicalOperator1) {
      /* Update for UnitDelay: '<S131>/Unit Delay' */
      emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] = 0.0F;
      emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] = 0.0F;
    } else {
      /* Update for UnitDelay: '<S131>/Unit Delay' incorporates:
       *  DataStoreRead: '<S126>/Data Store Read1'
       *  Product: '<S131>/Product'
       *  Saturate: '<S131>/Saturation'
       *  Sum: '<S131>/Add2'
       */
      emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] =
        emDriveFOC_control_DW.UnitDelay_DSTATE_b[0] * emDriveFOC_control_DW.Tsc
        + Ctrl_Sensorless_StatusBEMF_IabEst[0];
      emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] =
        emDriveFOC_control_DW.UnitDelay_DSTATE_b[1] * emDriveFOC_control_DW.Tsc
        + Ctrl_Sensorless_StatusBEMF_IabEst[1];
    }

    /* End of Switch: '<S131>/Switch' */

    /* Outputs for Atomic SubSystem: '<S122>/Sensorless_valid' */
    /* MATLAB Function: '<S158>/Pitagora_calc' */
    /* MATLAB Function 'Pitagora_calc': '<S169>:1' */
    /* '<S169>:1:2' temp = D^2 + Q^2; */
    temp = Ctrl_Sensorless_StatusBEMF_EabEst[0] *
      Ctrl_Sensorless_StatusBEMF_EabEst[0] + Ctrl_Sensorless_StatusBEMF_EabEst[1]
      * Ctrl_Sensorless_StatusBEMF_EabEst[1];

    /* '<S169>:1:3' if temp > 0 */
    if (temp > 0.0F) {
      /*  this is necesary because ifx_sqrt returns NaN for sqrt(0) */
      /* '<S169>:1:4' S = sqrt(temp); */
      temp = emDriveSqrt(temp);
    } else {
      /* '<S169>:1:5' else */
      /* '<S169>:1:6' S = single(0); */
      temp = 0.0F;
    }

    /* End of MATLAB Function: '<S158>/Pitagora_calc' */

    /* Gain: '<S129>/Gain' incorporates:
     *  Constant: '<S158>/Constant1'
     *  Product: '<S158>/Product1'
     */
    Ctrl_Sensorless_StatusBEMF_PLL_Err = rtb_Saturation_i_idx_0 *
      Ctrl_Motor__Psi;

    /* Switch: '<S158>/Switch' */
    if (Ctrl_Sensorless_StatusBEMF_PLL_Err == 0.0F) {
      /* Gain: '<S129>/Gain' incorporates:
       *  Constant: '<S158>/Constant2'
       */
      Ctrl_Sensorless_StatusBEMF_PLL_Err = 1.0E-6F;
    }

    /* Abs: '<S158>/Abs2' incorporates:
     *  Product: '<S158>/Divide'
     *  Switch: '<S158>/Switch'
     */
    Ctrl_Sensorless_StatusBEMF_CriteriaBEMF = fabsf(temp /
      Ctrl_Sensorless_StatusBEMF_PLL_Err);

    /* Outputs for Atomic SubSystem: '<S158>/LPF_BackEMF' */
    /* Delay: '<S168>/Delay' */
    if (emDriveFOC_control_DW.icLoad_b) {
      /* Sum: '<S168>/Add' */
      Delay_DSTATE_f = Ctrl_Sensorless_StatusBEMF_CriteriaBEMF;
    }

    /* End of Delay: '<S168>/Delay' */

    /* Sum: '<S168>/Add' incorporates:
     *  Product: '<S168>/Product'
     *  Sum: '<S168>/Add1'
     */
    Delay_DSTATE_f += (Ctrl_Sensorless_StatusBEMF_CriteriaBEMF - Delay_DSTATE_f)
      * emDriveFOC_control_DW.alpha_b;

    /* Switch: '<S168>/Switch1' */
    if (emDriveFOC_control_DW.alpha_b != 0.0F) {
      /* Abs: '<S158>/Abs2' */
      Ctrl_Sensorless_StatusBEMF_CriteriaBEMF = Delay_DSTATE_f;
    }

    /* End of Switch: '<S168>/Switch1' */

    /* Update for Delay: '<S168>/Delay' */
    emDriveFOC_control_DW.icLoad_b = false;

    /* End of Outputs for SubSystem: '<S158>/LPF_BackEMF' */

    /* Logic: '<S158>/Logical Operator3' incorporates:
     *  Constant: '<S158>/Constant5'
     *  Constant: '<S158>/Constant6'
     *  Constant: '<S167>/Constant'
     *  Logic: '<S158>/Logical Operator'
     *  RelationalOperator: '<S158>/Relational Operator2'
     *  RelationalOperator: '<S158>/Relational Operator3'
     *  RelationalOperator: '<S167>/Compare'
     */
    rtb_LogicalOperator_pi = (((Ctrl_Sensorless_StatusBEMF_CriteriaBEMF >
      Ctrl_Sensorless__ValidBEMFratioMin) &&
      (Ctrl_Sensorless_StatusBEMF_CriteriaBEMF <
       Ctrl_Sensorless__ValidBEMFratioMax)) ||
      (Ctrl_Sensorless__ValidBEMFratioMin == 0.0F));

    /* Gain: '<S129>/Gain' */
    Ctrl_Sensorless_StatusBEMF_PLL_Err = 57.2957802F * rtb_y_g;

    /* Abs: '<S161>/Abs' */
    Ctrl_Sensorless_StatusBEMF_CriteriaPLL = fabsf
      (Ctrl_Sensorless_StatusBEMF_PLL_Err);

    /* Outputs for Atomic SubSystem: '<S161>/LPF_PLLerr' */
    /* Delay: '<S183>/Delay' */
    if (emDriveFOC_control_DW.icLoad_fe) {
      /* Sum: '<S183>/Add' */
      Delay_DSTATE_hv = Ctrl_Sensorless_StatusBEMF_CriteriaPLL;
    }

    /* End of Delay: '<S183>/Delay' */

    /* Sum: '<S183>/Add' incorporates:
     *  Product: '<S183>/Product'
     *  Sum: '<S183>/Add1'
     */
    Delay_DSTATE_hv += (Ctrl_Sensorless_StatusBEMF_CriteriaPLL - Delay_DSTATE_hv)
      * emDriveFOC_control_DW.alpha_b;

    /* Switch: '<S183>/Switch1' */
    if (emDriveFOC_control_DW.alpha_b != 0.0F) {
      /* Abs: '<S161>/Abs' */
      Ctrl_Sensorless_StatusBEMF_CriteriaPLL = Delay_DSTATE_hv;
    }

    /* End of Switch: '<S183>/Switch1' */

    /* Update for Delay: '<S183>/Delay' */
    emDriveFOC_control_DW.icLoad_fe = false;

    /* End of Outputs for SubSystem: '<S161>/LPF_PLLerr' */

    /* Logic: '<S161>/Logical Operator2' incorporates:
     *  Constant: '<S161>/Constant'
     *  Constant: '<S182>/Constant'
     *  RelationalOperator: '<S161>/Relational Operator1'
     *  RelationalOperator: '<S182>/Compare'
     */
    rtb_LogicalOperator4 = ((Ctrl_Sensorless__ValidPLLerrMax == 0.0F) ||
      (Ctrl_Sensorless_StatusBEMF_CriteriaPLL < Ctrl_Sensorless__ValidPLLerrMax));

    /* Switch: '<S129>/Switch1' incorporates:
     *  Constant: '<S129>/Constant3'
     *  Constant: '<S129>/Constant4'
     *  UnitDelay: '<S129>/Unit Delay'
     */
    if (emDriveFOC_control_DW.UnitDelay_DSTATE_e0) {
      rtb_y_g = Ctrl_Sensorless__ValidMinRPM;
    } else {
      rtb_y_g = Ctrl_Sensorless__ValidMaxRPM;
    }

    /* End of Switch: '<S129>/Switch1' */

    /* RelationalOperator: '<S160>/Relational Operator' incorporates:
     *  Abs: '<S160>/Abs'
     */
    rtb_status_omega = (fabsf(Ctrl_Sensorless_StatusBEMF_RPM_) >= rtb_y_g);

    /* Signum: '<S162>/Sign' */
    if (UnitDelay2_2_DSTATE < 0.0F) {
      temp = -1.0F;
    } else {
      temp = (real32_T)(UnitDelay2_2_DSTATE > 0.0F);
    }

    /* Signum: '<S162>/Sign1' */
    if (Ctrl_Sensorless_StatusBEMF_RPM_ < 0.0F) {
      tmp = -1.0F;
    } else {
      tmp = (real32_T)(Ctrl_Sensorless_StatusBEMF_RPM_ > 0.0F);
    }

    /* Logic: '<S162>/Logical Operator5' incorporates:
     *  Abs: '<S162>/Abs1'
     *  Logic: '<S162>/Logical Operator1'
     *  Logic: '<S162>/Logical Operator4'
     *  RelationalOperator: '<S162>/Relational Operator4'
     *  RelationalOperator: '<S162>/Relational Operator5'
     *  Signum: '<S162>/Sign'
     *  Signum: '<S162>/Sign1'
     *  UnitDelay generated from: '<S118>/Unit Delay2'
     */
    rtb_status_RPM_f = (((fabsf(UnitDelay2_2_DSTATE) >= rtb_y_g) && (temp == tmp))
                        || (!emDriveFOC_control_DW.UnitDelay2_1_DSTATE));

    /* RelationalOperator: '<S162>/Relational Operator1' incorporates:
     *  Constant: '<S129>/Constant1'
     */
    rtb_RelationalOperator1_fm = (Ctrl_Sensorless_StatusBEMF_RPM_ <
      Ctrl_Sensorless__MaxRPM);

    /* Switch: '<S159>/Switch3' */
    if (rtb_LogicalOperator4) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S171>/Constant'
       */
      rtb_DataTypeConversion[0] = (int8_T)enumSlidingErr_NoErr;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S170>/Constant'
       */
      rtb_DataTypeConversion[0] = (int8_T)enumSlidingErr_Err_PLL;
    }

    /* End of Switch: '<S159>/Switch3' */

    /* Switch: '<S159>/Switch4' */
    if (rtb_LogicalOperator_pi) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S175>/Constant'
       */
      rtb_DataTypeConversion[1] = (int8_T)enumSlidingErr_NoErr;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S174>/Constant'
       */
      rtb_DataTypeConversion[1] = (int8_T)enumSlidingErr_Err_BEMF;
    }

    /* End of Switch: '<S159>/Switch4' */

    /* Switch: '<S159>/Switch5' */
    if (rtb_status_omega) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S177>/Constant'
       */
      rtb_DataTypeConversion[2] = (int8_T)enumSlidingErr_NoErr;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S176>/Constant'
       */
      rtb_DataTypeConversion[2] = (int8_T)enumSlidingErr_Err_Omega;
    }

    /* End of Switch: '<S159>/Switch5' */

    /* Switch: '<S159>/Switch6' */
    if (rtb_status_RPM_f) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S179>/Constant'
       */
      rtb_DataTypeConversion[3] = (int8_T)enumSlidingErr_NoErr;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S178>/Constant'
       */
      rtb_DataTypeConversion[3] = (int8_T)enumSlidingErr_Err_RPM;
    }

    /* End of Switch: '<S159>/Switch6' */

    /* Switch: '<S159>/Switch1' */
    if (rtb_RelationalOperator1_fm) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S173>/Constant'
       */
      rtb_DataTypeConversion[4] = (int8_T)enumSlidingErr_NoErr;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S172>/Constant'
       */
      rtb_DataTypeConversion[4] = (int8_T)enumSlidingErr_Err_Overspeed;
    }

    /* End of Switch: '<S159>/Switch1' */

    /* Switch: '<S159>/Switch7' */
    if (rtb_LogicalOperator1) {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S180>/Constant'
       */
      rtb_DataTypeConversion[5] = (int8_T)enumSlidingErr_No_SensorLess;
    } else {
      /* DataTypeConversion: '<S159>/Data Type Conversion' incorporates:
       *  Constant: '<S181>/Constant'
       */
      rtb_DataTypeConversion[5] = (int8_T)enumSlidingErr_NoErr;
    }

    /* End of Switch: '<S159>/Switch7' */

    /* Sum: '<S159>/Add' */
    emDriveFOC_control_DW.Err = 0;
    for (i = 0; i < 6; i++) {
      emDriveFOC_control_DW.Err += rtb_DataTypeConversion[i];
    }

    /* End of Sum: '<S159>/Add' */

    /* Logic: '<S129>/Logical Operator' */
    emDriveFOC_control_DW.LogicalOperator = (rtb_status_omega &&
      rtb_status_RPM_f);

    /* SignalConversion: '<S165>/Signal Conversion' */
    Ctrl_Sensorless_StatusBEMF_CriteriaFlags = emDriveFOC_control_DW.Err;

    /* Update for UnitDelay: '<S129>/Unit Delay' incorporates:
     *  Logic: '<S129>/Logical Operator1'
     */
    emDriveFOC_control_DW.UnitDelay_DSTATE_e0 = (rtb_LogicalOperator4 &&
      rtb_LogicalOperator_pi && rtb_status_omega && rtb_status_RPM_f &&
      rtb_RelationalOperator1_fm);

    /* End of Outputs for SubSystem: '<S122>/Sensorless_valid' */

    /* Signum: '<S122>/Sign' */
    if (UnitDelay3_DSTATE < 0.0F) {
      emDriveFOC_control_DW.Sign = -1.0F;
    } else {
      emDriveFOC_control_DW.Sign = (real32_T)(UnitDelay3_DSTATE > 0.0F);
    }

    /* End of Signum: '<S122>/Sign' */

    /* Switch: '<S132>/Switch' */
    if (rtb_LogicalOperator1) {
      /* Update for UnitDelay: '<S132>/Unit Delay' */
      emDriveFOC_control_DW.UnitDelay_DSTATE_f[0] = 0.0F;
      emDriveFOC_control_DW.UnitDelay_DSTATE_f[1] = 0.0F;
    } else {
      /* Update for UnitDelay: '<S132>/Unit Delay' incorporates:
       *  Sum: '<S132>/Add2'
       */
      emDriveFOC_control_DW.UnitDelay_DSTATE_f[0] =
        Ctrl_Sensorless_StatusBEMF_EabEst[0];
      emDriveFOC_control_DW.UnitDelay_DSTATE_f[1] =
        Ctrl_Sensorless_StatusBEMF_EabEst[1];
    }

    /* End of Switch: '<S132>/Switch' */

    /* Update for Delay: '<S127>/Delay One Step' */
    emDriveFOC_control_DW.DelayOneStep_DSTATE_m = UnitDelay3_DSTATE;

    /* End of Outputs for SubSystem: '<S118>/CloseLoopMode' */

    /* BusCreator generated from: '<S118>/SenzorlessStateDigram' */
    emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo.theta_r =
      rtb_Saturation_i_idx_1;
    emDriveFOC_control_DW.BusConversion_InsertedFor_Senzo.omega_r =
      rtb_Saturation_i_idx_0;

    /* Logic: '<S118>/Logical Operator4' incorporates:
     *  Constant: '<S118>/Constant2'
     *  Constant: '<S118>/EN_sensorless'
     *  Logic: '<S118>/Logical Operator3'
     */
    rtb_LogicalOperator4 = (Ctrl_Sensorless__Active &&
      (!Ctrl_ALTTestMode__Enable));

    /* Chart: '<S118>/SenzorlessStateDigram' */
    emDri_SenzorlessStateDigram();

    /* SignalConversion: '<S186>/Signal Conversion' incorporates:
     *  DataTypeConversion: '<S124>/Data Type Conversion2'
     */
    Ctrl_Sensorless_Status__State = (int32_T)
      emDriveFOC_control_DW.SenzorlessState;

    /* SignalConversion: '<S187>/Signal Conversion' */
    Ctrl_OpenLoop_StatusRPMopen = emDriveFOC_control_DW.RPM_Open;

    /* Switch: '<S124>/Switch' incorporates:
     *  DataTypeConversion: '<S124>/Data Type Conversion2'
     *  Logic: '<S124>/Logical Operator'
     *  RelationalOperator: '<S124>/Relational Operator'
     *  RelationalOperator: '<S124>/Relational Operator1'
     *  Switch: '<S124>/Switch1'
     */
    if (((int32_T)emDriveFOC_control_DW.SenzorlessState != 7) && ((int32_T)
         emDriveFOC_control_DW.SenzorlessState != 1)) {
      /* Switch: '<S124>/Switch' */
      emDriveFOC_control_DW.UnitDelay_DSTATE_lx = (int32_T)
        emDriveFOC_control_DW.SenzorlessState;

      /* Switch: '<S124>/Switch1' */
      emDriveFOC_control_DW.UnitDelay1_DSTATE_j = emDriveFOC_control_DW.Err;
    }

    /* End of Switch: '<S124>/Switch' */

    /* SignalConversion: '<S188>/Signal Conversion' */
    Ctrl_Sensorless_Status__LastNonErrorState =
      emDriveFOC_control_DW.UnitDelay_DSTATE_lx;

    /* SignalConversion: '<S189>/Signal Conversion' */
    Ctrl_Sensorless_Status__LatchedCriteriaFlags =
      emDriveFOC_control_DW.UnitDelay1_DSTATE_j;

    /* Switch generated from: '<S118>/Switch' */
    if (rtb_LogicalOperator4) {
      /* Merge generated from: '<S41>/Merge' incorporates:
       *  SignalConversion generated from: '<S124>/Bus Selector'
       */
      emDriveFOC_control_DW.theta_r_d = emDriveFOC_control_DW.PosOut.theta_r;

      /* Merge generated from: '<S41>/Merge' incorporates:
       *  SignalConversion generated from: '<S124>/Bus Selector'
       */
      emDriveFOC_control_DW.omega_r_l = emDriveFOC_control_DW.PosOut.omega_r;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.Iq = emDriveFOC_control_DW.TrefOut;

      /* Merge generated from: '<S41>/Merge' incorporates:
       *  Product: '<S118>/Product'
       */
      emDriveFOC_control_DW.Id = emDriveFOC_control_DW.RateLimiter *
        emDriveFOC_control_DW.IdOut;
    } else {
      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.theta_r_d = emDriveFOC_control_DW.theta_r;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.omega_r_l = emDriveFOC_control_DW.omega_r_c;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.Iq = emDriveFOC_control_DW.Switch2_h;

      /* Merge generated from: '<S41>/Merge' incorporates:
       *  Constant: '<S117>/Constant'
       */
      emDriveFOC_control_DW.Id = 0.0F;
    }

    /* Merge generated from: '<S41>/Merge' incorporates:
     *  Switch generated from: '<S118>/Switch'
     */
    emDriveFOC_control_DW.RPM_Track_b = (rtb_LogicalOperator4 &&
      emDriveFOC_control_DW.RPM_Track_a);

    /* Switch generated from: '<S118>/Switch' */
    if (rtb_LogicalOperator4) {
      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.RPM_h = emDriveFOC_control_DW.RPM_Open;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.State = emDriveFOC_control_DW.SenzorlessState;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.ErrCode = emDriveFOC_control_DW.Err;
    } else {
      /* Merge generated from: '<S41>/Merge' incorporates:
       *  Constant: '<S117>/Constant3'
       */
      emDriveFOC_control_DW.RPM_h = 0.0F;

      /* Merge generated from: '<S41>/Merge' incorporates:
       *  Constant: '<S120>/Constant'
       */
      emDriveFOC_control_DW.State = enumSenzorlessState_None;

      /* Merge generated from: '<S41>/Merge' */
      emDriveFOC_control_DW.ErrCode = 32;
    }

    /* Update for UnitDelay generated from: '<S118>/Unit Delay2' */
    emDriveFOC_control_DW.UnitDelay2_1_DSTATE =
      emDriveFOC_control_DW.RPM_Track_a;

    /* Update for UnitDelay generated from: '<S118>/Unit Delay2' */
    UnitDelay2_2_DSTATE = emDriveFOC_control_DW.RPM_Open;

    /* Update for UnitDelay: '<S118>/Unit Delay7' */
    emDriveFOC_control_DW.UnitDelay7_DSTATE =
      emDriveFOC_control_DW.PLL_SlowEnable;
  }

  /* End of Constant: '<S41>/EN_sensorless1' */

  /* Outputs for Atomic SubSystem: '<S118>/CloseLoopMode' */
  /* Product: '<S127>/Product' */
  emDriveFOC_control_DW.UnitDelay3_DSTATE = UnitDelay3_DSTATE;

  /* End of Outputs for SubSystem: '<S118>/CloseLoopMode' */
  /* End of Outputs for SubSystem: '<S41>/Sensorless' */

  /* Switch: '<S136>/Switch' */
  emDriveFOC_control_DW.Delay_DSTATE_oq = Delay_DSTATE_oq;
  emDriveFOC_control_DW.Delay_DSTATE_oc = Delay_DSTATE_oc;

  /* Update for UnitDelay generated from: '<S118>/Unit Delay2' */
  emDriveFOC_control_DW.UnitDelay2_2_DSTATE = UnitDelay2_2_DSTATE;

  /* Outputs for Enabled SubSystem: '<S41>/Sensorless' incorporates:
   *  EnablePort: '<S118>/Enable'
   */
  /* Outputs for Atomic SubSystem: '<S118>/CloseLoopMode' */
  /* Outputs for Atomic SubSystem: '<S127>/ThetaPLL' */
  /* Sum: '<S144>/Add2' */
  emDriveFOC_control_DW.UnitDelay_DSTATE_fm = UnitDelay_DSTATE_fm;

  /* End of Outputs for SubSystem: '<S127>/ThetaPLL' */
  /* End of Outputs for SubSystem: '<S118>/CloseLoopMode' */
  /* End of Outputs for SubSystem: '<S41>/Sensorless' */

  /* Sum: '<S168>/Add' */
  emDriveFOC_control_DW.Delay_DSTATE_f = Delay_DSTATE_f;

  /* Sum: '<S183>/Add' */
  emDriveFOC_control_DW.Delay_DSTATE_hv = Delay_DSTATE_hv;
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S228>/PowerLimitDecisionStateflow' */
  FOC_SLOW_CODE
  static void PowerLimitDecisionStateflow(void)
{
  int32_T tmp;
  uint8_T is_c5_emDriveFOC_control;
  uint8_T temporalCounter_i1_i;
  boolean_T tmp_0;
  is_c5_emDriveFOC_control = emDriveFOC_control_DW.is_c5_emDriveFOC_control;
  temporalCounter_i1_i = emDriveFOC_control_DW.temporalCounter_i1_i;

  /* Chart: '<S242>/PowerLimitStateMachine' incorporates:
   *  Constant: '<S242>/Ctrl_PowerLimit__BridgeTempHigh'
   *  Constant: '<S242>/Ctrl_PowerLimit__BridgeTempLow'
   *  Constant: '<S242>/Ctrl_PowerLimit__DisableLimit'
   *  Constant: '<S242>/Ctrl_PowerLimit__LowerLimit'
   *  Constant: '<S242>/Ctrl_PowerLimit__MiddleLimit'
   *  Constant: '<S242>/Ctrl_PowerLimit__UpperLimit'
   *  Delay: '<S228>/Delay One Step'
   *  Inport: '<Root>/SensorsTsc'
   */
  /* Gateway: Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow/PowerLimitStateMachine */
  if (temporalCounter_i1_i < 1U) {
    temporalCounter_i1_i = 1U;
  }

  /* During: Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow/PowerLimitStateMachine */
  if (emDriveFOC_control_DW.is_active_c5_emDriveFOC_control == 0U) {
    /* Entry: Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow/PowerLimitStateMachine */
    emDriveFOC_control_DW.is_active_c5_emDriveFOC_control = 1U;

    /* Entry Internal: Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow/PowerLimitStateMachine */
    /* Transition: '<S245>:24' */
    is_c5_emDriveFOC_control = emDriveFOC_co_IN_PowerLimitInit;
    temporalCounter_i1_i = 0U;

    /* Entry 'PowerLimitInit': '<S245>:23' */
    /* '<S245>:23:4' State = 0; */
    Ctrl_PowerLimit__State = 0;

    /* '<S245>:23:5' cnt = 0; */
    Ctrl_PowerLimit__Counter = 0;

    /* '<S245>:23:6' PowerLim = PowerLimUpper; */
    Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__UpperLimit;
  } else {
    switch (is_c5_emDriveFOC_control) {
     case emDriveFOC_co_IN_PowerLimitInit:
      /* During 'PowerLimitInit': '<S245>:23' */
      /* '<S245>:27:1' sf_internal_predicateOutput = after(1,tick); */
      /* Transition: '<S245>:27' */
      /* Exit 'PowerLimitInit': '<S245>:23' */
      is_c5_emDriveFOC_control = emDriveFOC__IN_PowerLimitNormal;

      /* Entry 'PowerLimitNormal': '<S245>:1' */
      /* '<S245>:1:4' State = 1; */
      Ctrl_PowerLimit__State = 1;

      /* '<S245>:1:5' PowerLim = PowerLimUpper; */
      Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__UpperLimit;
      break;

     case emDriveFOC__IN_PowerLimitNormal:
      Ctrl_PowerLimit__State = 1;

      /* During 'PowerLimitNormal': '<S245>:1' */
      /* '<S245>:6:1' sf_internal_predicateOutput = cnt>=CounterLimit; */
      if (Ctrl_PowerLimit__Counter >= emDriveFOC_control_DW.PowerLimitDuration)
      {
        /* Transition: '<S245>:6' */
        /* Exit 'PowerLimitNormal': '<S245>:1' */
        is_c5_emDriveFOC_control = emDr_IN_PowerLimitTimeReduction;

        /* Entry 'PowerLimitTimeReduction': '<S245>:3' */
        /* '<S245>:3:4' State = 2; */
        Ctrl_PowerLimit__State = 2;

        /* '<S245>:3:5' PowerLim = PowerLimLower; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__LowerLimit;

        /* '<S245>:31:1' sf_internal_predicateOutput = bridgeTemp>=BridgeTempUpperLim; */
      } else if (emDriveFOC_control_U.SensorsTsc.BridgeT >=
                 Ctrl_PowerLimit__BridgeTempHigh) {
        /* Transition: '<S245>:31' */
        /* Exit 'PowerLimitNormal': '<S245>:1' */
        is_c5_emDriveFOC_control = emDr_IN_PowerLimitTempReduction;

        /* Entry 'PowerLimitTempReduction': '<S245>:30' */
        /* '<S245>:30:4' State = 3; */
        Ctrl_PowerLimit__State = 3;

        /* '<S245>:30:5' PowerLim = PowerLimMiddle; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__MiddleLimit;
      } else {
        /* '<S245>:1:7' PowerLim = PowerLimUpper; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__UpperLimit;

        /* '<S245>:1:8' if(PowerLimitActive<1)||(Pel>=PowerLimMiddle) */
        if ((emDriveFOC_control_DW.DelayOneStep_DSTATE < 1.0F) ||
            (Ctrl_Power__CompensatedPower >= Ctrl_PowerLimit__MiddleLimit)) {
          /* '<S245>:1:9' cnt = cnt + 1; */
          Ctrl_PowerLimit__Counter = Ctrl_PowerLimit__Counter + 1;
        }

        /* '<S245>:1:11' if(Pel<PowerLimDisable)&&(cnt>0) */
        if ((Ctrl_Power__CompensatedPower < Ctrl_PowerLimit__DisableLimit) &&
            (Ctrl_PowerLimit__Counter > 0)) {
          /* '<S245>:1:12' cnt = cnt - 1; */
          Ctrl_PowerLimit__Counter = Ctrl_PowerLimit__Counter - 1;
        }
      }
      break;

     case emDr_IN_PowerLimitTempReduction:
      Ctrl_PowerLimit__State = 3;

      /* During 'PowerLimitTempReduction': '<S245>:30' */
      /* '<S245>:32:1' sf_internal_predicateOutput = bridgeTemp<BridgeTempLowerLim; */
      if (emDriveFOC_control_U.SensorsTsc.BridgeT <
          Ctrl_PowerLimit__BridgeTempLow) {
        /* Transition: '<S245>:32' */
        /* Exit 'PowerLimitTempReduction': '<S245>:30' */
        is_c5_emDriveFOC_control = emDriveFOC__IN_PowerLimitNormal;

        /* Entry 'PowerLimitNormal': '<S245>:1' */
        /* '<S245>:1:4' State = 1; */
        Ctrl_PowerLimit__State = 1;

        /* '<S245>:1:5' PowerLim = PowerLimUpper; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__UpperLimit;

        /* '<S245>:39:1' sf_internal_predicateOutput = cnt>=CounterLimit; */
      } else if (Ctrl_PowerLimit__Counter >=
                 emDriveFOC_control_DW.PowerLimitDuration) {
        /* Transition: '<S245>:39' */
        /* Exit 'PowerLimitTempReduction': '<S245>:30' */
        is_c5_emDriveFOC_control = emDr_IN_PowerLimitTimeReduction;

        /* Entry 'PowerLimitTimeReduction': '<S245>:3' */
        /* '<S245>:3:4' State = 2; */
        Ctrl_PowerLimit__State = 2;

        /* '<S245>:3:5' PowerLim = PowerLimLower; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__LowerLimit;
      } else {
        /* '<S245>:30:7' PowerLim = PowerLimMiddle; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__MiddleLimit;

        /* '<S245>:30:8' if(PowerLimitActive<1) */
        if (emDriveFOC_control_DW.DelayOneStep_DSTATE < 1.0F) {
          /* '<S245>:30:9' cnt = cnt + 1; */
          Ctrl_PowerLimit__Counter = Ctrl_PowerLimit__Counter + 1;
        }

        /* '<S245>:30:11' if(Pel<PowerLimDisable)&&(cnt>0) */
        if ((Ctrl_Power__CompensatedPower < Ctrl_PowerLimit__DisableLimit) &&
            (Ctrl_PowerLimit__Counter > 0)) {
          /* '<S245>:30:12' cnt = cnt - 1; */
          Ctrl_PowerLimit__Counter = Ctrl_PowerLimit__Counter - 1;
        }
      }
      break;

     default:
      Ctrl_PowerLimit__State = 2;

      /* During 'PowerLimitTimeReduction': '<S245>:3' */
      /* '<S245>:5:1' sf_internal_predicateOutput = (cnt<CounterLimit-CounterLimitHysteresis)&&(bridgeTemp<BridgeTempLowerLim); */
      tmp_0 = ((emDriveFOC_control_DW.PowerLimitDuration >= 0) &&
               (emDriveFOC_control_DW.PowerLimitDurationHysteresis <
                emDriveFOC_control_DW.PowerLimitDuration - MAX_int32_T));
      if (tmp_0) {
        tmp = MAX_int32_T;
      } else if ((emDriveFOC_control_DW.PowerLimitDuration < 0) &&
                 (emDriveFOC_control_DW.PowerLimitDurationHysteresis >
                  emDriveFOC_control_DW.PowerLimitDuration - MIN_int32_T)) {
        tmp = MIN_int32_T;
      } else {
        tmp = emDriveFOC_control_DW.PowerLimitDuration -
          emDriveFOC_control_DW.PowerLimitDurationHysteresis;
      }

      if ((Ctrl_PowerLimit__Counter < tmp) &&
          (emDriveFOC_control_U.SensorsTsc.BridgeT <
           Ctrl_PowerLimit__BridgeTempLow)) {
        /* Transition: '<S245>:5' */
        /* Exit 'PowerLimitTimeReduction': '<S245>:3' */
        is_c5_emDriveFOC_control = emDriveFOC__IN_PowerLimitNormal;

        /* Entry 'PowerLimitNormal': '<S245>:1' */
        /* '<S245>:1:4' State = 1; */
        Ctrl_PowerLimit__State = 1;

        /* '<S245>:1:5' PowerLim = PowerLimUpper; */
        Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__UpperLimit;
      } else {
        /* '<S245>:42:1' sf_internal_predicateOutput = (cnt<CounterLimit-CounterLimitHysteresis)&&(bridgeTemp>=BridgeTempLowerLim); */
        if (tmp_0) {
          tmp = MAX_int32_T;
        } else if ((emDriveFOC_control_DW.PowerLimitDuration < 0) &&
                   (emDriveFOC_control_DW.PowerLimitDurationHysteresis >
                    emDriveFOC_control_DW.PowerLimitDuration - MIN_int32_T)) {
          tmp = MIN_int32_T;
        } else {
          tmp = emDriveFOC_control_DW.PowerLimitDuration -
            emDriveFOC_control_DW.PowerLimitDurationHysteresis;
        }

        if ((Ctrl_PowerLimit__Counter < tmp) &&
            (emDriveFOC_control_U.SensorsTsc.BridgeT >=
             Ctrl_PowerLimit__BridgeTempLow)) {
          /* Transition: '<S245>:42' */
          /* Exit 'PowerLimitTimeReduction': '<S245>:3' */
          is_c5_emDriveFOC_control = emDr_IN_PowerLimitTempReduction;

          /* Entry 'PowerLimitTempReduction': '<S245>:30' */
          /* '<S245>:30:4' State = 3; */
          Ctrl_PowerLimit__State = 3;

          /* '<S245>:30:5' PowerLim = PowerLimMiddle; */
          Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__MiddleLimit;
        } else {
          /* '<S245>:3:7' PowerLim = PowerLimLower; */
          Ctrl_PowerLimit__LimitStatus = Ctrl_PowerLimit__LowerLimit;

          /* '<S245>:3:8' if(Pel<PowerLimDisable)&&(cnt>0) */
          if ((Ctrl_Power__CompensatedPower < Ctrl_PowerLimit__DisableLimit) &&
              (Ctrl_PowerLimit__Counter > 0)) {
            /* '<S245>:3:9' cnt = cnt - 1; */
            Ctrl_PowerLimit__Counter = Ctrl_PowerLimit__Counter - 1;
          }
        }
      }
      break;
    }
  }

  /* End of Chart: '<S242>/PowerLimitStateMachine' */
  emDriveFOC_control_DW.temporalCounter_i1_i = temporalCounter_i1_i;
  emDriveFOC_control_DW.is_c5_emDriveFOC_control = is_c5_emDriveFOC_control;
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<S3>/TemperatureAndPowerLimit' */
  FOC_SLOW_CODE
  static void TemperatureAndPowerLim_Init(void)
{
  /* InitializeConditions for Delay: '<S229>/Delay' */
  emDriveFOC_control_DW.icLoad_f = true;

  /* SystemInitialize for Atomic SubSystem: '<S247>/ThermalModelVariableP' */
  /* InitializeConditions for Delay: '<S251>/Delay' */
  emDriveFOC_control_DW.icLoad_a = true;

  /* End of SystemInitialize for SubSystem: '<S247>/ThermalModelVariableP' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S3>/TemperatureAndPowerLimit' */
  FOC_SLOW_CODE
  static void em_TemperatureAndPowerLimit(void)
{
  real32_T Delay_DSTATE_b;
  real32_T Delay_DSTATE_duo;
  real32_T Delay_DSTATE_h;
  real32_T Delay_DSTATE_n;
  real32_T Delay_DSTATE_nd;
  real32_T rtb_K1_l_idx_0;
  real32_T rtb_K2_a_idx_0;
  real32_T rtb_Switch_g4;
  real32_T rtb_out;
  real32_T rtb_out_g;
  real32_T rtb_out_p;

  /* Outputs for Atomic SubSystem: '<S247>/ThermalModelVariableP' */
  /* MATLAB Function: '<S251>/P_variable_to_Temp' */
  Delay_DSTATE_b = emDriveFOC_control_DW.Delay_DSTATE_b;

  /* End of Outputs for SubSystem: '<S247>/ThermalModelVariableP' */

  /* Outputs for Atomic SubSystem: '<S230>/ PIregOutletTemp' */
  /* Sum: '<S254>/Sum' */
  Delay_DSTATE_n = emDriveFOC_control_DW.Delay_DSTATE_n;

  /* End of Outputs for SubSystem: '<S230>/ PIregOutletTemp' */

  /* Outputs for Atomic SubSystem: '<S227>/ PIregBridgeTemp' */
  /* Sum: '<S237>/Sum' */
  Delay_DSTATE_h = emDriveFOC_control_DW.Delay_DSTATE_h;

  /* End of Outputs for SubSystem: '<S227>/ PIregBridgeTemp' */

  /* Outputs for Atomic SubSystem: '<S223>/ PI regMotorTemp' */
  /* Sum: '<S226>/Sum' */
  Delay_DSTATE_nd = emDriveFOC_control_DW.Delay_DSTATE_nd;

  /* End of Outputs for SubSystem: '<S223>/ PI regMotorTemp' */

  /* Sum: '<S241>/Sum' */
  Delay_DSTATE_duo = emDriveFOC_control_DW.Delay_DSTATE_duo;

  /* Delay: '<S229>/Delay' incorporates:
   *  Constant: '<S223>/Constant'
   */
  if (emDriveFOC_control_DW.icLoad_f) {
    Delay_DSTATE_b = 0.0F;
  }

  /* End of Delay: '<S229>/Delay' */

  /* SignalConversion: '<S248>/Signal Conversion' incorporates:
   *  Constant: '<S229>/Constant'
   *  Constant: '<S229>/Constant1'
   *  Constant: '<S246>/Constant5'
   *  Constant: '<S246>/Constant6'
   *  Gain: '<S246>/Aluminum temperature coef'
   *  Product: '<S229>/Product'
   *  Product: '<S246>/Product1'
   *  Sum: '<S246>/Subtract'
   *  Sum: '<S246>/Subtract1'
   */
  Ctrl_Motor__CopperLoss = ((Delay_DSTATE_b - 25.0F) * 0.0043F + 1.0F) *
    Ctrl_Motor__Rs * (emDriveFOC_control_DW.Iph * emDriveFOC_control_DW.Iph *
                      3.0F);

  /* SignalConversion: '<S249>/Signal Conversion' incorporates:
   *  Constant: '<S229>/Constant5'
   *  Product: '<S229>/Product1'
   */
  Ctrl_Motor__IronLoss = emDriveFOC_control_DW.RPM_act *
    emDriveFOC_control_DW.RPM_act * Ctrl_Motor__IronLossCoef;

  /* Sum: '<S229>/Add' */
  Ctrl_Motor__TotalLoss = Ctrl_Motor__IronLoss + Ctrl_Motor__CopperLoss;

  /* MATLAB Function: '<S247>/ThermalTimeConstant' incorporates:
   *  Constant: '<S229>/Constant2'
   *  Constant: '<S229>/Constant3'
   *  Constant: '<S229>/Constant4'
   */
  /*  constants used in calcultion of thermal model */
  /* MATLAB Function 'ThermalTimeConstant': '<S252>:1' */
  /* '<S252>:1:4' K1 = coder.nullcopy(zeros(1,length(Rth),'single')); */
  /* '<S252>:1:5' K2 = coder.nullcopy(zeros(1,length(Rth),'single')); */
  /* '<S252>:1:6' for i=1:length(Rth) */
  rtb_out_g = Ctrl_Motor__Rth[0];
  rtb_Switch_g4 = Ctrl_Motor__Tau[0];

  /* '<S252>:1:7' if Tau(i) < 2*Ts */
  rtb_out_p = 1.0F / Ctrl_Gen__Fcur * (real32_T)Ctrl_Gen__CurToVelRatio;
  rtb_out = rtb_out_p * 2.0F;
  if (Ctrl_Motor__Tau[0] < rtb_out) {
    /* prevent to short time constants */
    /* '<S252>:1:8' Tau(i) = 2*Ts; */
    rtb_Switch_g4 = 1.0F / Ctrl_Gen__Fcur * (real32_T)Ctrl_Gen__CurToVelRatio *
      2.0F;
  }

  /* '<S252>:1:10' if Rth(i) == 0 */
  if (Ctrl_Motor__Rth[0] == 0.0F) {
    /* prevent to short time constants */
    /* '<S252>:1:11' Rth(i) = 1e-6; */
    rtb_out_g = 1.0E-6F;
  }

  /* '<S252>:1:14' K1(i) = 1/Rth(i); */
  rtb_K1_l_idx_0 = 1.0F / rtb_out_g;

  /* '<S252>:1:15' K2(i) = 1/Tau(i)*Rth(i)*Ts; */
  rtb_K2_a_idx_0 = 1.0F / rtb_Switch_g4 * rtb_out_g * rtb_out_p;
  rtb_out_g = Ctrl_Motor__Rth[1];
  rtb_Switch_g4 = Ctrl_Motor__Tau[1];

  /* '<S252>:1:7' if Tau(i) < 2*Ts */
  if (Ctrl_Motor__Tau[1] < rtb_out) {
    /* prevent to short time constants */
    /* '<S252>:1:8' Tau(i) = 2*Ts; */
    rtb_Switch_g4 = 1.0F / Ctrl_Gen__Fcur * (real32_T)Ctrl_Gen__CurToVelRatio *
      2.0F;
  }

  /* '<S252>:1:10' if Rth(i) == 0 */
  if (Ctrl_Motor__Rth[1] == 0.0F) {
    /* prevent to short time constants */
    /* '<S252>:1:11' Rth(i) = 1e-6; */
    rtb_out_g = 1.0E-6F;
  }

  /* Outputs for Atomic SubSystem: '<S247>/ThermalModelVariableP' */
  /* Delay: '<S251>/Delay' incorporates:
   *  Constant: '<S247>/Constant3'
   */
  /* '<S252>:1:14' K1(i) = 1/Rth(i); */
  /* '<S252>:1:15' K2(i) = 1/Tau(i)*Rth(i)*Ts; */
  if (emDriveFOC_control_DW.icLoad_a) {
    emDriveFOC_control_DW.Delay_DSTATE_dx[0] = Ctrl_Motor__InitialTempRise[0];
    emDriveFOC_control_DW.Delay_DSTATE_dx[1] = Ctrl_Motor__InitialTempRise[1];
  }

  /* Update for Delay: '<S251>/Delay' */
  /* MATLAB Function 'Derating/TemperatureAndPowerLimit/MotorTempThermalModel/PowerToTemperatureMotor/ThermalModelVariableP/P_variable_to_Temp': '<S253>:1' */
  /* '<S253>:1:3' Temp =coder.nullcopy(zeros(length(K1),1,'single')); */
  /* '<S253>:1:4' TempSum = single(0); */
  /* '<S253>:1:7' for j=1:length(K1) */
  emDriveFOC_control_DW.icLoad_a = false;

  /* MATLAB Function: '<S251>/P_variable_to_Temp' incorporates:
   *  Delay: '<S251>/Delay'
   *  SignalConversion generated from: '<S253>/ SFunction '
   */
  /* '<S253>:1:8' Temp(j)=(Pin(j)-(TempOld(j)*K1(j)))*K2(j)+TempOld(j); */
  rtb_out = (Ctrl_Motor__CopperLoss - emDriveFOC_control_DW.Delay_DSTATE_dx[0] *
             rtb_K1_l_idx_0) * rtb_K2_a_idx_0 +
    emDriveFOC_control_DW.Delay_DSTATE_dx[0];

  /* '<S253>:1:9' TempSum=TempSum+Temp(j); */
  rtb_K1_l_idx_0 = rtb_out;

  /* Update for Delay: '<S251>/Delay' */
  emDriveFOC_control_DW.Delay_DSTATE_dx[0] = rtb_out;

  /* MATLAB Function: '<S251>/P_variable_to_Temp' incorporates:
   *  Delay: '<S251>/Delay'
   *  MATLAB Function: '<S247>/ThermalTimeConstant'
   *  SignalConversion generated from: '<S253>/ SFunction '
   */
  /* '<S253>:1:8' Temp(j)=(Pin(j)-(TempOld(j)*K1(j)))*K2(j)+TempOld(j); */
  rtb_out = (Ctrl_Motor__TotalLoss - 1.0F / rtb_out_g *
             emDriveFOC_control_DW.Delay_DSTATE_dx[1]) * (1.0F / rtb_Switch_g4 *
    rtb_out_g * rtb_out_p) + emDriveFOC_control_DW.Delay_DSTATE_dx[1];

  /* '<S253>:1:9' TempSum=TempSum+Temp(j); */
  Delay_DSTATE_b = rtb_K1_l_idx_0 + rtb_out;

  /* Update for Delay: '<S251>/Delay' */
  emDriveFOC_control_DW.Delay_DSTATE_dx[1] = rtb_out;

  /* End of Outputs for SubSystem: '<S247>/ThermalModelVariableP' */

  /* Switch: '<S223>/Switch' incorporates:
   *  Constant: '<S223>/TempFromRenabled'
   */
  if (Ctrl_Motor__TempFromRenabled) {
    /* Switch: '<S223>/Switch' */
    Ctrl_Gen_Stat__MotorWireTemp = Ctrl_Gen_Stat__TemperatureFromR;
  } else {
    /* Switch: '<S223>/Switch' */
    Ctrl_Gen_Stat__MotorWireTemp = Delay_DSTATE_b;
  }

  /* End of Switch: '<S223>/Switch' */

  /* Outputs for Atomic SubSystem: '<S223>/ PI regMotorTemp' */
  /* Sum: '<S226>/Subtract1' incorporates:
   *  Constant: '<S226>/Constant4'
   */
  rtb_out_p = Ctrl_Gen_Stat__MotorWireTemp - Ctrl_Limits__MotorTemperature;

  /* Sum: '<S226>/Add' incorporates:
   *  Constant: '<S226>/Constant1'
   *  Constant: '<S226>/Constant2'
   *  Product: '<S226>/IOut'
   *  Product: '<S226>/Product'
   */
  Delay_DSTATE_nd += rtb_out_p * Ctrl_Limits__MotorTemperatureIgain * 0.001F;

  /* Switch: '<S236>/Switch2' incorporates:
   *  Constant: '<S226>/Constant3'
   *  Constant: '<S226>/Constant5'
   *  RelationalOperator: '<S236>/LowerRelop1'
   *  RelationalOperator: '<S236>/UpperRelop'
   *  Switch: '<S236>/Switch'
   */
  if (Delay_DSTATE_nd > 1.0F) {
    /* Sum: '<S226>/Add' */
    Delay_DSTATE_nd = 1.0F;
  } else if (Delay_DSTATE_nd < 0.0F) {
    /* Sum: '<S226>/Add' incorporates:
     *  Constant: '<S226>/Constant5'
     *  Switch: '<S236>/Switch'
     */
    Delay_DSTATE_nd = 0.0F;
  }

  /* End of Switch: '<S236>/Switch2' */

  /* Sum: '<S226>/Sum' incorporates:
   *  Constant: '<S226>/Constant'
   *  Product: '<S226>/POut'
   */
  rtb_out_g = rtb_out_p * Ctrl_Limits__MotorTemperaturePgain + Delay_DSTATE_nd;

  /* Switch: '<S235>/Switch2' incorporates:
   *  Constant: '<S226>/Constant3'
   *  Constant: '<S226>/Constant5'
   *  RelationalOperator: '<S235>/LowerRelop1'
   *  RelationalOperator: '<S235>/UpperRelop'
   *  Switch: '<S235>/Switch'
   */
  if (rtb_out_g > 1.0F) {
    rtb_out_g = 1.0F;
  } else if (rtb_out_g < 0.0F) {
    /* Switch: '<S235>/Switch' incorporates:
     *  Constant: '<S226>/Constant5'
     */
    rtb_out_g = 0.0F;
  }

  /* End of Outputs for SubSystem: '<S223>/ PI regMotorTemp' */

  /* Outputs for Atomic SubSystem: '<S227>/ PIregBridgeTemp' */
  /* Sum: '<S237>/Subtract1' incorporates:
   *  Constant: '<S237>/Constant4'
   *  Inport: '<Root>/SensorsTsc'
   */
  rtb_out_p = emDriveFOC_control_U.SensorsTsc.BridgeT -
    Ctrl_Limits__BridgeTemperature;

  /* Sum: '<S237>/Add' incorporates:
   *  Constant: '<S237>/Constant1'
   *  Constant: '<S237>/Constant2'
   *  Product: '<S237>/IOut'
   *  Product: '<S237>/Product'
   */
  Delay_DSTATE_h += rtb_out_p * Ctrl_Limits__BridgeTemperatureIgain * 0.001F;

  /* Switch: '<S240>/Switch2' incorporates:
   *  Constant: '<S237>/Constant3'
   *  Constant: '<S237>/Constant5'
   *  RelationalOperator: '<S240>/LowerRelop1'
   *  RelationalOperator: '<S240>/UpperRelop'
   *  Switch: '<S240>/Switch'
   */
  if (Delay_DSTATE_h > 1.0F) {
    /* Sum: '<S237>/Add' */
    Delay_DSTATE_h = 1.0F;
  } else if (Delay_DSTATE_h < 0.0F) {
    /* Sum: '<S237>/Add' incorporates:
     *  Constant: '<S237>/Constant5'
     *  Switch: '<S240>/Switch'
     */
    Delay_DSTATE_h = 0.0F;
  }

  /* End of Switch: '<S240>/Switch2' */

  /* Sum: '<S237>/Sum' incorporates:
   *  Constant: '<S237>/Constant'
   *  Product: '<S237>/POut'
   */
  rtb_out_p = rtb_out_p * Ctrl_Limits__BridgeTemperaturePgain + Delay_DSTATE_h;

  /* Switch: '<S239>/Switch2' incorporates:
   *  Constant: '<S237>/Constant3'
   *  Constant: '<S237>/Constant5'
   *  RelationalOperator: '<S239>/LowerRelop1'
   *  RelationalOperator: '<S239>/UpperRelop'
   *  Switch: '<S239>/Switch'
   */
  if (rtb_out_p > 1.0F) {
    rtb_out_p = 1.0F;
  } else if (rtb_out_p < 0.0F) {
    /* Switch: '<S239>/Switch' incorporates:
     *  Constant: '<S237>/Constant5'
     */
    rtb_out_p = 0.0F;
  }

  /* End of Switch: '<S239>/Switch2' */

  /* Outputs for Atomic SubSystem: '<S227>/PreventMinimalLimiting' */
  /* Switch: '<S238>/Switch' incorporates:
   *  Constant: '<S237>/Constant3'
   *  Constant: '<S238>/Constant'
   *  Sum: '<S237>/Add1'
   */
  if (1.0F - rtb_out_p > 0.99F) {
    rtb_Switch_g4 = 1.0F;
  } else {
    rtb_Switch_g4 = 1.0F - rtb_out_p;
  }

  /* End of Switch: '<S238>/Switch' */
  /* End of Outputs for SubSystem: '<S227>/PreventMinimalLimiting' */
  /* End of Outputs for SubSystem: '<S227>/ PIregBridgeTemp' */

  /* Switch: '<S223>/Switch1' incorporates:
   *  Constant: '<S223>/Offset'
   *  Constant: '<S223>/Power in W'
   *  Constant: '<S223>/Slope gain'
   *  Product: '<S223>/Product1'
   *  RelationalOperator: '<S223>/GreaterThan'
   *  Sum: '<S223>/Add'
   */
  if (emDriveFOC_control_DW.Switch1_h <= Ctrl_Power__CompensationStart) {
    Ctrl_Power__CompensatedPower = emDriveFOC_control_DW.Switch1_h;
  } else {
    Ctrl_Power__CompensatedPower = (emDriveFOC_control_DW.Switch1_h -
      emDriveFOC_control_DW.Switch1_h * Ctrl_Power__CompensationSlope) +
      Ctrl_Power__CompensationOffset;
  }

  /* End of Switch: '<S223>/Switch1' */

  /* Outputs for Atomic SubSystem: '<S228>/PowerLimitDecisionStateflow' */
  PowerLimitDecisionStateflow();

  /* End of Outputs for SubSystem: '<S228>/PowerLimitDecisionStateflow' */

  /* Sum: '<S241>/Sum1' */
  rtb_out_p = Ctrl_Power__CompensatedPower - Ctrl_PowerLimit__LimitStatus;

  /* Sum: '<S241>/Sum2' incorporates:
   *  Constant: '<S241>/Constant1'
   *  Constant: '<S241>/Constant2'
   *  Product: '<S241>/IOut'
   *  Product: '<S241>/Product'
   */
  Delay_DSTATE_duo += rtb_out_p * Ctrl_Limits__PowerLimitIgain * 0.001F;

  /* Switch: '<S244>/Switch2' incorporates:
   *  Constant: '<S241>/Constant3'
   *  Constant: '<S241>/Constant5'
   *  RelationalOperator: '<S244>/LowerRelop1'
   *  RelationalOperator: '<S244>/UpperRelop'
   *  Switch: '<S244>/Switch'
   */
  if (Delay_DSTATE_duo > 1.0F) {
    /* Sum: '<S241>/Sum2' */
    Delay_DSTATE_duo = 1.0F;
  } else if (Delay_DSTATE_duo < 0.0F) {
    /* Sum: '<S241>/Sum2' incorporates:
     *  Constant: '<S241>/Constant5'
     *  Switch: '<S244>/Switch'
     */
    Delay_DSTATE_duo = 0.0F;
  }

  /* End of Switch: '<S244>/Switch2' */

  /* Sum: '<S241>/Sum' incorporates:
   *  Constant: '<S241>/Constant'
   *  Product: '<S241>/POut'
   */
  rtb_out = Ctrl_Limits__PowerLimitPgain * rtb_out_p + Delay_DSTATE_duo;

  /* Switch: '<S243>/Switch2' incorporates:
   *  Constant: '<S241>/Constant3'
   *  Constant: '<S241>/Constant5'
   *  RelationalOperator: '<S243>/LowerRelop1'
   *  RelationalOperator: '<S243>/UpperRelop'
   *  Switch: '<S243>/Switch'
   */
  if (rtb_out > 1.0F) {
    rtb_out = 1.0F;
  } else if (rtb_out < 0.0F) {
    /* Switch: '<S243>/Switch' incorporates:
     *  Constant: '<S241>/Constant5'
     */
    rtb_out = 0.0F;
  }

  /* Outputs for Atomic SubSystem: '<S230>/ PIregOutletTemp' */
  /* Sum: '<S254>/Subtract1' incorporates:
   *  Constant: '<S254>/Constant4'
   *  Inport: '<Root>/SensorsTsc'
   */
  rtb_out_p = emDriveFOC_control_U.SensorsTsc.OutletT -
    Ctrl_Limits__OutletTemperature;

  /* Sum: '<S254>/Add' incorporates:
   *  Constant: '<S254>/Constant1'
   *  Constant: '<S254>/Constant2'
   *  Product: '<S254>/IOut'
   *  Product: '<S254>/Product'
   */
  Delay_DSTATE_n += rtb_out_p * Ctrl_Limits__OutletTemperatureIgain * 0.001F;

  /* Switch: '<S257>/Switch2' incorporates:
   *  Constant: '<S254>/Constant3'
   *  Constant: '<S254>/Constant5'
   *  RelationalOperator: '<S257>/LowerRelop1'
   *  RelationalOperator: '<S257>/UpperRelop'
   *  Switch: '<S257>/Switch'
   */
  if (Delay_DSTATE_n > 1.0F) {
    /* Sum: '<S254>/Add' */
    Delay_DSTATE_n = 1.0F;
  } else if (Delay_DSTATE_n < 0.0F) {
    /* Sum: '<S254>/Add' incorporates:
     *  Constant: '<S254>/Constant5'
     *  Switch: '<S257>/Switch'
     */
    Delay_DSTATE_n = 0.0F;
  }

  /* End of Switch: '<S257>/Switch2' */

  /* Sum: '<S254>/Sum' incorporates:
   *  Constant: '<S254>/Constant'
   *  Product: '<S254>/POut'
   */
  rtb_out_p = rtb_out_p * Ctrl_Limits__OutletTemperaturePgain + Delay_DSTATE_n;

  /* Switch: '<S256>/Switch2' incorporates:
   *  Constant: '<S254>/Constant3'
   *  Constant: '<S254>/Constant5'
   *  RelationalOperator: '<S256>/LowerRelop1'
   *  RelationalOperator: '<S256>/UpperRelop'
   *  Switch: '<S256>/Switch'
   */
  if (rtb_out_p > 1.0F) {
    rtb_out_p = 1.0F;
  } else if (rtb_out_p < 0.0F) {
    /* Switch: '<S256>/Switch' incorporates:
     *  Constant: '<S254>/Constant5'
     */
    rtb_out_p = 0.0F;
  }

  /* Outputs for Atomic SubSystem: '<S230>/PreventMinimalLimiting' */
  /* Switch: '<S255>/Switch' incorporates:
   *  Constant: '<S254>/Constant3'
   *  Constant: '<S255>/Constant'
   *  Sum: '<S254>/Add1'
   *  Switch: '<S256>/Switch2'
   */
  if (1.0F - rtb_out_p > 0.99F) {
    rtb_out_p = 1.0F;
  } else {
    rtb_out_p = 1.0F - rtb_out_p;
  }

  /* End of Outputs for SubSystem: '<S230>/ PIregOutletTemp' */

  /* Outputs for Atomic SubSystem: '<S223>/ PI regMotorTemp' */
  /* MinMax: '<S223>/Min' incorporates:
   *  Constant: '<S226>/Constant3'
   *  Sum: '<S226>/Add1'
   *  Switch: '<S235>/Switch2'
   *  Switch: '<S255>/Switch'
   */
  emDriveFOC_control_DW.Min = fminf(fminf(1.0F - rtb_out_g, rtb_Switch_g4),
    rtb_out_p);

  /* End of Outputs for SubSystem: '<S223>/ PI regMotorTemp' */
  /* End of Outputs for SubSystem: '<S230>/PreventMinimalLimiting' */

  /* SignalConversion: '<S231>/Signal Conversion' incorporates:
   *  Constant: '<S241>/Constant3'
   *  Sum: '<S241>/Add1'
   *  Switch: '<S243>/Switch2'
   */
  Ctrl_PowerLimit__StatusActive = 1.0F - rtb_out;

  /* Product: '<S223>/Product4' */
  Ctrl_PowerLimit__StatusLimitIq =
    emDriveFOC_control_DW.TmpRTBAtTemperatureAndPowerLimi *
    Ctrl_PowerLimit__StatusActive;

  /* RelationalOperator: '<S223>/Relational Operator' incorporates:
   *  Constant: '<S223>/MinStatoCurrPercent'
   */
  emDriveFOC_control_DW.RelationalOperator = (emDriveFOC_control_DW.Min <= 0.05F);

  /* SignalConversion: '<S232>/Signal Conversion' incorporates:
   *  Constant: '<S226>/Constant3'
   *  Sum: '<S226>/Add1'
   *  Switch: '<S235>/Switch2'
   */
  Ctrl_Limits_StatusLim[0] = emDriveFOC_control_DW.Min;

  /* Outputs for Atomic SubSystem: '<S223>/ PI regMotorTemp' */
  Ctrl_Limits_StatusLim[1] = 1.0F - rtb_out_g;

  /* End of Outputs for SubSystem: '<S223>/ PI regMotorTemp' */
  Ctrl_Limits_StatusLim[2] = rtb_Switch_g4;

  /* SignalConversion: '<S233>/Signal Conversion' incorporates:
   *  Product: '<S223>/Product'
   */
  Ctrl_Limits_StatusIsMax = emDriveFOC_control_DW.Gain2 *
    emDriveFOC_control_DW.Min;

  /* Update for Delay: '<S229>/Delay' */
  emDriveFOC_control_DW.icLoad_f = false;

  /* Update for Delay: '<S228>/Delay One Step' incorporates:
   *  Constant: '<S241>/Constant3'
   *  Sum: '<S241>/Add1'
   *  Switch: '<S243>/Switch2'
   */
  emDriveFOC_control_DW.DelayOneStep_DSTATE = 1.0F - rtb_out;

  /* Sum: '<S241>/Sum' */
  emDriveFOC_control_DW.Delay_DSTATE_duo = Delay_DSTATE_duo;

  /* Outputs for Atomic SubSystem: '<S223>/ PI regMotorTemp' */
  /* Sum: '<S226>/Sum' */
  emDriveFOC_control_DW.Delay_DSTATE_nd = Delay_DSTATE_nd;

  /* End of Outputs for SubSystem: '<S223>/ PI regMotorTemp' */

  /* Outputs for Atomic SubSystem: '<S227>/ PIregBridgeTemp' */
  /* Sum: '<S237>/Sum' */
  emDriveFOC_control_DW.Delay_DSTATE_h = Delay_DSTATE_h;

  /* End of Outputs for SubSystem: '<S227>/ PIregBridgeTemp' */

  /* Outputs for Atomic SubSystem: '<S230>/ PIregOutletTemp' */
  /* Sum: '<S254>/Sum' */
  emDriveFOC_control_DW.Delay_DSTATE_n = Delay_DSTATE_n;

  /* End of Outputs for SubSystem: '<S230>/ PIregOutletTemp' */

  /* Outputs for Atomic SubSystem: '<S247>/ThermalModelVariableP' */
  /* MATLAB Function: '<S251>/P_variable_to_Temp' */
  emDriveFOC_control_DW.Delay_DSTATE_b = Delay_DSTATE_b;

  /* End of Outputs for SubSystem: '<S247>/ThermalModelVariableP' */
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<S5>/MeasurmentsVel' */
  FOC_SLOW_CODE
  static void emDrive_MeasurmentsVel_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: '<S270>/LPF_Measurment' */
  /* InitializeConditions for Delay: '<S283>/Delay' */
  emDriveFOC_control_DW.icLoad_i = true;

  /* End of SystemInitialize for SubSystem: '<S270>/LPF_Measurment' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S5>/MeasurmentsVel' */
  FOC_SLOW_CODE
  static void emDriveFOC_c_MeasurmentsVel(void)
{
  real32_T Delay_DSTATE_m;
  real32_T UnitDelay4_DSTATE;
  real32_T UnitDelay4_DSTATE_o;
  real32_T temp;
  boolean_T rtb_Compare_j_tmp;

  /* Update for UnitDelay: '<S281>/Unit Delay4' */
  UnitDelay4_DSTATE_o = emDriveFOC_control_DW.UnitDelay4_DSTATE_o;

  /* Update for UnitDelay: '<S280>/Unit Delay4' */
  UnitDelay4_DSTATE = emDriveFOC_control_DW.UnitDelay4_DSTATE;

  /* Outputs for Atomic SubSystem: '<S270>/LPF_Measurment' */
  /* Switch: '<S283>/Switch1' */
  Delay_DSTATE_m = emDriveFOC_control_DW.Delay_DSTATE_m;

  /* End of Outputs for SubSystem: '<S270>/LPF_Measurment' */

  /* MATLAB Function: '<S268>/Pitagora_calc' */
  /* MATLAB Function 'Pitagora_calc': '<S271>:1' */
  /* '<S271>:1:2' temp = D^2 + Q^2; */
  temp = emDriveFOC_control_DW.Switch1_i[0] * emDriveFOC_control_DW.Switch1_i[0]
    + emDriveFOC_control_DW.Switch1_i[1] * emDriveFOC_control_DW.Switch1_i[1];

  /* '<S271>:1:3' if temp > 0 */
  if (temp > 0.0F) {
    /*  this is necesary because ifx_sqrt returns NaN for sqrt(0) */
    /* '<S271>:1:4' S = sqrt(temp); */
    temp = emDriveSqrt(temp);
  } else {
    /* '<S271>:1:5' else */
    /* '<S271>:1:6' S = single(0); */
    temp = 0.0F;
  }

  /* End of MATLAB Function: '<S268>/Pitagora_calc' */

  /* Gain: '<S268>/Gain' */
  emDriveFOC_control_DW.Uph = 0.707106769F * temp;

  /* MATLAB Function: '<S268>/Pitagora_calc1' */
  /* MATLAB Function 'Pitagora_calc': '<S272>:1' */
  /* '<S272>:1:2' temp = D^2 + Q^2; */
  temp = emDriveFOC_control_DW.Switch1_i[2] * emDriveFOC_control_DW.Switch1_i[2]
    + emDriveFOC_control_DW.Switch1_i[3] * emDriveFOC_control_DW.Switch1_i[3];

  /* '<S272>:1:3' if temp > 0 */
  if (temp > 0.0F) {
    /*  this is necesary because ifx_sqrt returns NaN for sqrt(0) */
    /* '<S272>:1:4' S = sqrt(temp); */
    temp = emDriveSqrt(temp);
  } else {
    /* '<S272>:1:5' else */
    /* '<S272>:1:6' S = single(0); */
    temp = 0.0F;
  }

  /* End of MATLAB Function: '<S268>/Pitagora_calc1' */

  /* Gain: '<S268>/Gain1' */
  emDriveFOC_control_DW.Iph = 0.707106769F * temp;

  /* Gain: '<S268>/Gain2' incorporates:
   *  Product: '<S268>/Product'
   *  Product: '<S268>/Product1'
   *  Sum: '<S268>/Add'
   */
  emDriveFOC_control_DW.Switch1_h = (emDriveFOC_control_DW.Switch1_i[0] *
    emDriveFOC_control_DW.Switch1_i[2] + emDriveFOC_control_DW.Switch1_i[1] *
    emDriveFOC_control_DW.Switch1_i[3]) * 1.5F;

  /* RelationalOperator: '<S282>/Compare' incorporates:
   *  Constant: '<S279>/Constant'
   *  Constant: '<S282>/Constant'
   *  Logic: '<S269>/Logical Operator'
   *  RelationalOperator: '<S269>/Relational Operator'
   */
  rtb_Compare_j_tmp = (emDriveFOC_control_DW.State_p != enumSenzorlessState_Hold);

  /* Switch: '<S269>/Switch' incorporates:
   *  Constant: '<S282>/Constant'
   *  RelationalOperator: '<S278>/FixPt Relational Operator'
   *  RelationalOperator: '<S282>/Compare'
   *  UnitDelay: '<S278>/Delay Input1'
   *
   * Block description for '<S278>/Delay Input1':
   *
   *  Store in Global RAM
   */
  if ((int32_T)rtb_Compare_j_tmp > (int32_T)
      emDriveFOC_control_DW.DelayInput1_DSTATE) {
    /* Switch: '<S269>/Switch' incorporates:
     *  Product: '<S269>/Divide'
     */
    emDriveFOC_control_DW.UnitDelay_DSTATE_l = UnitDelay4_DSTATE /
      UnitDelay4_DSTATE_o;
  }

  /* End of Switch: '<S269>/Switch' */

  /* Switch: '<S280>/Switch3' incorporates:
   *  Switch: '<S281>/Switch3'
   */
  if (rtb_Compare_j_tmp) {
    /* Update for UnitDelay: '<S280>/Unit Delay4' */
    UnitDelay4_DSTATE = 0.0F;

    /* Update for UnitDelay: '<S281>/Unit Delay4' */
    UnitDelay4_DSTATE_o = 0.0F;
  } else {
    /* Update for UnitDelay: '<S280>/Unit Delay4' incorporates:
     *  Abs: '<S269>/Abs'
     *  Sum: '<S280>/Add4'
     */
    UnitDelay4_DSTATE += fabsf(emDriveFOC_control_DW.Switch1_i[1]);

    /* Update for UnitDelay: '<S281>/Unit Delay4' incorporates:
     *  Constant: '<S269>/Constant'
     *  Sum: '<S281>/Add4'
     */
    UnitDelay4_DSTATE_o++;
  }

  /* End of Switch: '<S280>/Switch3' */

  /* Outputs for Atomic SubSystem: '<S270>/LPFalphaMeasurment1' */
  /* MATLAB Function: '<S284>/MATLAB Function' incorporates:
   *  Constant: '<S270>/Constant1'
   *  Constant: '<S284>/TimeConstant'
   */
  /* MATLAB Function 'LPF_coef/MATLAB Function': '<S285>:1' */
  /* '<S285>:1:2' if TimeConstant ~= 0 */
  if (Ctrl_Measurment__Pel_LPF_TimeConst != 0.0F) {
    /* '<S285>:1:3' alpha = ts/(TimeConstant+ts); */
    temp = 1.0F / Ctrl_Gen__Fcur * (real32_T)Ctrl_Gen__CurToVelRatio;
    temp /= temp + Ctrl_Measurment__Pel_LPF_TimeConst;
  } else {
    /* '<S285>:1:4' else */
    /* '<S285>:1:5' alpha=single(0); */
    temp = 0.0F;
  }

  /* End of MATLAB Function: '<S284>/MATLAB Function' */
  /* End of Outputs for SubSystem: '<S270>/LPFalphaMeasurment1' */

  /* Outputs for Atomic SubSystem: '<S270>/LPF_Measurment' */
  /* Delay: '<S283>/Delay' */
  if (emDriveFOC_control_DW.icLoad_i) {
    /* Switch: '<S283>/Switch' */
    Delay_DSTATE_m = emDriveFOC_control_DW.Switch1_h;
  }

  /* End of Delay: '<S283>/Delay' */

  /* Switch: '<S283>/Switch' incorporates:
   *  Product: '<S283>/Product'
   *  Sum: '<S283>/Add'
   *  Sum: '<S283>/Add1'
   */
  Delay_DSTATE_m += (emDriveFOC_control_DW.Switch1_h - Delay_DSTATE_m) * temp;

  /* Switch: '<S283>/Switch1' */
  if (temp != 0.0F) {
    /* Gain: '<S268>/Gain2' */
    emDriveFOC_control_DW.Switch1_h = Delay_DSTATE_m;
  }

  /* Update for Delay: '<S283>/Delay' */
  emDriveFOC_control_DW.icLoad_i = false;

  /* End of Outputs for SubSystem: '<S270>/LPF_Measurment' */

  /* Product: '<S268>/Product2' incorporates:
   *  Constant: '<S268>/Constant'
   */
  emDriveFOC_control_DW.RPM = 60.0F / (Ctrl_Motor__Np * 2.0F * 3.14159274F) *
    emDriveFOC_control_DW.Switch1_i[4];

  /* SignalConversion: '<S273>/Signal Conversion' */
  Ctrl_Measurments__UphRms = emDriveFOC_control_DW.Uph;

  /* SignalConversion: '<S274>/Signal Conversion' */
  Ctrl_Measurments__IphRms = emDriveFOC_control_DW.Iph;

  /* SignalConversion: '<S275>/Signal Conversion' */
  Ctrl_Measurments__Pel = emDriveFOC_control_DW.Switch1_h;

  /* SignalConversion: '<S276>/Signal Conversion' */
  Ctrl_Measurments__RPM = emDriveFOC_control_DW.RPM;

  /* SignalConversion: '<S277>/Signal Conversion' incorporates:
   *  Constant: '<S269>/Constant1'
   *  Constant: '<S269>/Constant2'
   *  Product: '<S269>/Product'
   *  Sum: '<S269>/Add'
   */
  Ctrl_Gen_Stat__TemperatureFromR = emDriveFOC_control_DW.UnitDelay_DSTATE_l *
    Ctrl_Motor__TempFromRgain + Ctrl_Motor__TempFromRoffset;

  /* Update for UnitDelay: '<S278>/Delay Input1' incorporates:
   *  Constant: '<S282>/Constant'
   *  RelationalOperator: '<S282>/Compare'
   *
   * Block description for '<S278>/Delay Input1':
   *
   *  Store in Global RAM
   */
  emDriveFOC_control_DW.DelayInput1_DSTATE = rtb_Compare_j_tmp;

  /* Outputs for Atomic SubSystem: '<S270>/LPF_Measurment' */
  /* Switch: '<S283>/Switch1' */
  emDriveFOC_control_DW.Delay_DSTATE_m = Delay_DSTATE_m;

  /* End of Outputs for SubSystem: '<S270>/LPF_Measurment' */

  /* Update for UnitDelay: '<S280>/Unit Delay4' */
  emDriveFOC_control_DW.UnitDelay4_DSTATE = UnitDelay4_DSTATE;

  /* Update for UnitDelay: '<S281>/Unit Delay4' */
  emDriveFOC_control_DW.UnitDelay4_DSTATE_o = UnitDelay4_DSTATE_o;
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<S6>/Const' */
  FOC_SLOW_CODE
  static void emDriveFOC_control_Const(void)
{
  real32_T Product_h_tmp;
  real32_T rtb_MinMax1;

  /* MinMax: '<S286>/MinMax1' incorporates:
   *  Constant: '<S286>/Constant'
   *  UnaryMinus: '<S286>/Unary Minus'
   */
  rtb_MinMax1 = fmaxf(emDriveFOC_control_DW.Product_h, -Ctrl_Limits__RPMramp);

  /* Product: '<S286>/Product' incorporates:
   *  Constant: '<S286>/Constant3'
   *  Product: '<S286>/Product1'
   */
  Product_h_tmp = (real32_T)Ctrl_Gen__CurToVelRatio / Ctrl_Gen__Fcur;

  /* Product: '<S286>/Product' incorporates:
   *  Constant: '<S286>/Constant'
   *  Constant: '<S286>/Constant3'
   *  MinMax: '<S286>/MinMax'
   */
  emDriveFOC_control_DW.Product_h = Product_h_tmp * fminf(Ctrl_Limits__RPMramp,
    emDriveFOC_control_DW.Product1);

  /* Product: '<S286>/Product1' */
  emDriveFOC_control_DW.Product1 = Product_h_tmp * rtb_MinMax1;
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<Root>/RPM_ramp_and_limit' */
  FOC_SLOW_CODE
  static void emD_RPM_ramp_and_limit_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: '<S6>/Rate Limiter Dynamic with track' */
  /* InitializeConditions for Delay: '<S288>/Delay' */
  emDriveFOC_control_DW.icLoad_jq = true;

  /* End of SystemInitialize for SubSystem: '<S6>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<Root>/RPM_ramp_and_limit' */
  FOC_SLOW_CODE
  static void emDriveF_RPM_ramp_and_limit(void)
{
  real32_T Delay_DSTATE_d;
  real32_T UnitDelay_DSTATE;
  real32_T rtb_pos;
  boolean_T rtb_Dis_pos;
  boolean_T rtb_LogicalOperator1;
  boolean_T rtb_RelationalOperator_l;

  /* Outputs for Atomic SubSystem: '<S6>/Rate Limiter Dynamic with track' */
  /* Delay: '<S288>/Delay' */
  Delay_DSTATE_d = emDriveFOC_control_DW.Delay_DSTATE_d;

  /* End of Outputs for SubSystem: '<S6>/Rate Limiter Dynamic with track' */

  /* Switch: '<S6>/Switch2' */
  UnitDelay_DSTATE = emDriveFOC_control_DW.UnitDelay_DSTATE;

  /* Signum: '<S287>/Sign' */
  if (UnitDelay_DSTATE < 0.0F) {
    rtb_pos = -1.0F;
  } else {
    rtb_pos = (real32_T)(UnitDelay_DSTATE > 0.0F);
  }

  /* RelationalOperator: '<S287>/Relational Operator' incorporates:
   *  Signum: '<S287>/Sign'
   */
  rtb_RelationalOperator_l = (rtb_pos > 0.0F);

  /* Logic: '<S287>/Logical Operator1' */
  rtb_LogicalOperator1 = (emDriveFOC_control_DW.MTPV_active ||
    emDriveFOC_control_DW.pow_lim);

  /* Logic: '<S287>/Logical Operator5' incorporates:
   *  Logic: '<S287>/Logical Operator6'
   */
  rtb_Dis_pos = ((rtb_RelationalOperator_l && rtb_LogicalOperator1) ||
                 emDriveFOC_control_DW.Udc_lim);

  /* Switch: '<S6>/Switch4' incorporates:
   *  Constant: '<S6>/Constant4'
   */
  if (Ctrl_Limits_OverrideCloseLoopRamp) {
    /* Switch: '<S6>/Switch4' */
    Ctrl_Limits_StatusRampIn = emDriveFOC_control_DW.RateTransition8;
  } else {
    /* Switch: '<S6>/Switch4' incorporates:
     *  Constant: '<S6>/Constant5'
     */
    Ctrl_Limits_StatusRampIn = Ctrl_Limits__CloseLoopRamp;
  }

  /* End of Switch: '<S6>/Switch4' */

  /* Switch: '<S6>/Switch3' incorporates:
   *  Constant: '<S6>/Constant2'
   *  Constant: '<S6>/Constant3'
   *  Constant: '<S6>/Constant6'
   *  Delay: '<S6>/Delay'
   *  Logic: '<S6>/Logical Operator2'
   *  RelationalOperator: '<S6>/GreaterThan'
   *  RelationalOperator: '<S6>/GreaterThan1'
   */
  if ((Ctrl_Limits_StatusRampIn <= Ctrl_Limits__MaximumRampLowRpm) ||
      (emDriveFOC_control_DW.Delay_DSTATE_o >
       Ctrl_Limits__OpenLoopRampOverrideValue)) {
    rtb_pos = Ctrl_Limits_StatusRampIn;
  } else {
    rtb_pos = Ctrl_OpenLoop__RampRPMrate;
  }

  /* End of Switch: '<S6>/Switch3' */

  /* Logic: '<S287>/Logical Operator4' incorporates:
   *  Logic: '<S287>/Logical Operator7'
   *  Logic: '<S287>/Logical Operator8'
   */
  rtb_RelationalOperator_l = (((!rtb_RelationalOperator_l) &&
    rtb_LogicalOperator1) || emDriveFOC_control_DW.Udc_lim);

  /* Switch: '<S289>/Switch2' incorporates:
   *  Constant: '<S6>/Constant1'
   *  Constant: '<S6>/Constant13'
   *  RelationalOperator: '<S289>/LowerRelop1'
   *  RelationalOperator: '<S289>/UpperRelop'
   *  Switch: '<S289>/Switch'
   */
  if (Ctrl_Gen_Stat__RPMref > Ctrl_Limits__RPMpositive) {
    /* Switch: '<S289>/Switch2' */
    emDriveFOC_control_DW.RPM_out = Ctrl_Limits__RPMpositive;
  } else if (Ctrl_Gen_Stat__RPMref < Ctrl_Limits__RPMnegative) {
    /* Switch: '<S289>/Switch2' incorporates:
     *  Constant: '<S6>/Constant1'
     *  Switch: '<S289>/Switch'
     */
    emDriveFOC_control_DW.RPM_out = Ctrl_Limits__RPMnegative;
  } else {
    /* Switch: '<S289>/Switch2' incorporates:
     *  Switch: '<S289>/Switch'
     */
    emDriveFOC_control_DW.RPM_out = Ctrl_Gen_Stat__RPMref;
  }

  /* End of Switch: '<S289>/Switch2' */

  /* Outputs for Atomic SubSystem: '<S6>/Rate Limiter Dynamic with track' */
  /* Delay: '<S288>/Delay' */
  if (emDriveFOC_control_DW.icLoad_jq) {
    Delay_DSTATE_d = emDriveFOC_control_DW.RPM_out;
  }

  /* Switch: '<S288>/Switch' incorporates:
   *  Logic: '<S6>/Logical Operator'
   *  Logic: '<S6>/Logical Operator1'
   *  Switch: '<S6>/Switch1'
   */
  if ((!emDriveFOC_control_DW.RateTransition6) ||
      emDriveFOC_control_DW.RPM_Track) {
    /* Switch: '<S6>/Switch' */
    if (emDriveFOC_control_DW.RPM_Track) {
      /* Switch: '<S288>/Switch' */
      UnitDelay_DSTATE = emDriveFOC_control_DW.RPM_ramp_track_value;
    } else {
      /* Switch: '<S288>/Switch' */
      UnitDelay_DSTATE = 0.0F;
    }

    /* End of Switch: '<S6>/Switch' */
  } else {
    if (rtb_Dis_pos) {
      /* Switch: '<S6>/Switch1' incorporates:
       *  Constant: '<S6>/Constant'
       */
      emDriveFOC_control_DW.Product1 = 0.0F;
    } else {
      /* Switch: '<S6>/Switch1' */
      emDriveFOC_control_DW.Product1 = rtb_pos;
    }

    /* Switch: '<S6>/Switch5' incorporates:
     *  Constant: '<S6>/Constant'
     *  UnaryMinus: '<S6>/Unary Minus'
     */
    if (rtb_RelationalOperator_l) {
      emDriveFOC_control_DW.Product_h = 0.0F;
    } else {
      emDriveFOC_control_DW.Product_h = -rtb_pos;
    }

    /* End of Switch: '<S6>/Switch5' */

    /* Outputs for Atomic SubSystem: '<S6>/Const' */
    emDriveFOC_control_Const();

    /* End of Outputs for SubSystem: '<S6>/Const' */

    /* Sum: '<S288>/Difference Inputs1'
     *
     * Block description for '<S288>/Difference Inputs1':
     *
     *  Add in CPU
     */
    rtb_pos = emDriveFOC_control_DW.RPM_out - Delay_DSTATE_d;

    /* Switch: '<S293>/Switch2' incorporates:
     *  RelationalOperator: '<S293>/LowerRelop1'
     *  RelationalOperator: '<S293>/UpperRelop'
     *  Switch: '<S293>/Switch'
     */
    if (rtb_pos > emDriveFOC_control_DW.Product_h) {
      rtb_pos = emDriveFOC_control_DW.Product_h;
    } else if (rtb_pos < emDriveFOC_control_DW.Product1) {
      /* Switch: '<S293>/Switch' */
      rtb_pos = emDriveFOC_control_DW.Product1;
    }

    /* Switch: '<S288>/Switch' incorporates:
     *  Sum: '<S288>/Difference Inputs2'
     *  Switch: '<S293>/Switch2'
     *
     * Block description for '<S288>/Difference Inputs2':
     *
     *  Add in CPU
     */
    UnitDelay_DSTATE = rtb_pos + Delay_DSTATE_d;
  }

  /* End of Switch: '<S288>/Switch' */

  /* Update for Delay: '<S288>/Delay' */
  emDriveFOC_control_DW.icLoad_jq = false;

  /* End of Outputs for SubSystem: '<S6>/Rate Limiter Dynamic with track' */

  /* SignalConversion: '<S292>/Signal Conversion' */
  Ctrl_Limits_StatusDisIntegration[0] = emDriveFOC_control_DW.pow_lim;
  Ctrl_Limits_StatusDisIntegration[1] = emDriveFOC_control_DW.Udc_lim;
  Ctrl_Limits_StatusDisIntegration[2] = emDriveFOC_control_DW.MTPV_active;
  Ctrl_Limits_StatusDisIntegration[3] = rtb_Dis_pos;
  Ctrl_Limits_StatusDisIntegration[4] = rtb_RelationalOperator_l;

  /* SignalConversion: '<S290>/Signal Conversion' */
  Ctrl_Limits_StatusRPMramp[0] = emDriveFOC_control_DW.RPM_out;
  Ctrl_Limits_StatusRPMramp[1] = UnitDelay_DSTATE;

  /* Switch: '<S6>/Switch2' incorporates:
   *  Constant: '<S6>/Constant8'
   */
  if (!Ctrl_ALTTestMode__Enable) {
    /* Switch: '<S289>/Switch2' incorporates:
     *  Switch: '<S6>/Switch2'
     */
    emDriveFOC_control_DW.RPM_out = UnitDelay_DSTATE;
  }

  /* Update for Delay: '<S6>/Delay' incorporates:
   *  Abs: '<S6>/Abs'
   */
  emDriveFOC_control_DW.Delay_DSTATE_o = fabsf(UnitDelay_DSTATE);
  Delay_DSTATE_d = UnitDelay_DSTATE;

  /* Switch: '<S6>/Switch2' */
  emDriveFOC_control_DW.UnitDelay_DSTATE = UnitDelay_DSTATE;

  /* Outputs for Atomic SubSystem: '<S6>/Rate Limiter Dynamic with track' */
  /* Delay: '<S288>/Delay' */
  emDriveFOC_control_DW.Delay_DSTATE_d = Delay_DSTATE_d;

  /* End of Outputs for SubSystem: '<S6>/Rate Limiter Dynamic with track' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<Root>/StimGenerator' */
  FOC_SLOW_CODE
  static void emDriveFOC_co_StimGenerator(void)
{
  real32_T rtb_Switch2_a;
  uint32_T UnitDelay_DSTATE_e;
  uint32_T UnitDelay_DSTATE_k;
  uint32_T rtb_DataTypeConversion3;
  uint32_T rtb_DataTypeConversion3_l;

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator1' */
  /* Update for UnitDelay: '<S295>/Unit Delay' */
  UnitDelay_DSTATE_e = emDriveFOC_control_DW.UnitDelay_DSTATE_e;

  /* End of Outputs for SubSystem: '<S11>/PulseGenerator1' */

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator' */
  /* Update for UnitDelay: '<S294>/Unit Delay' */
  UnitDelay_DSTATE_k = emDriveFOC_control_DW.UnitDelay_DSTATE_k;

  /* DataTypeConversion: '<S294>/Data Type Conversion3' incorporates:
   *  Constant: '<S11>/Constant1'
   *  Constant: '<S11>/Constant10'
   *  Product: '<S294>/Divide'
   */
  rtb_DataTypeConversion3_l = (uint32_T)(Ctrl_Stim1__Period / (1.0F /
    Ctrl_Gen__Fcur));

  /* Switch: '<S294>/Switch2' incorporates:
   *  Constant: '<S11>/Constant'
   *  Constant: '<S11>/Constant2'
   *  Constant: '<S11>/Constant3'
   *  DataTypeConversion: '<S294>/Data Type Conversion1'
   *  Gain: '<S294>/Gain1'
   *  Product: '<S294>/Divide1'
   *  Product: '<S294>/Product'
   *  Product: '<S294>/Product2'
   *  RelationalOperator: '<S294>/Relational Operator1'
   *  Switch: '<S294>/Switch1'
   *  Trigonometry: '<S294>/Trigonometric Function'
   */
  if (Ctrl_Stim1__SineOrRect) {
    rtb_Switch2_a = emDriveSin((real32_T)UnitDelay_DSTATE_k / (real32_T)
      rtb_DataTypeConversion3_l * 6.28318548F) * Ctrl_Stim1__Amplitude;
  } else if (UnitDelay_DSTATE_k >= (uint32_T)((real32_T)
              rtb_DataTypeConversion3_l * 0.5F)) {
    /* Switch: '<S294>/Switch1' incorporates:
     *  Constant: '<S11>/Constant'
     *  Gain: '<S294>/Gain'
     */
    rtb_Switch2_a = -Ctrl_Stim1__Amplitude;
  } else {
    /* Switch: '<S294>/Switch1' incorporates:
     *  Constant: '<S11>/Constant'
     */
    rtb_Switch2_a = Ctrl_Stim1__Amplitude;
  }

  /* End of Switch: '<S294>/Switch2' */

  /* Sum: '<S294>/Add' incorporates:
   *  Constant: '<S11>/Constant4'
   */
  emDriveFOC_control_DW.RPM_act = rtb_Switch2_a + Ctrl_Stim1__Offset;

  /* End of Outputs for SubSystem: '<S11>/PulseGenerator' */

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator1' */
  /* DataTypeConversion: '<S295>/Data Type Conversion3' incorporates:
   *  Constant: '<S11>/Constant10'
   *  Constant: '<S11>/Constant7'
   *  Product: '<S295>/Divide'
   */
  rtb_DataTypeConversion3 = (uint32_T)(Ctrl_Stim2__Period / (1.0F /
    Ctrl_Gen__Fcur));

  /* Switch: '<S295>/Switch2' incorporates:
   *  Constant: '<S11>/Constant5'
   *  Constant: '<S11>/Constant6'
   *  Constant: '<S11>/Constant8'
   *  DataTypeConversion: '<S295>/Data Type Conversion1'
   *  Gain: '<S295>/Gain1'
   *  Product: '<S295>/Divide1'
   *  Product: '<S295>/Product'
   *  Product: '<S295>/Product2'
   *  RelationalOperator: '<S295>/Relational Operator1'
   *  Switch: '<S295>/Switch1'
   *  Trigonometry: '<S295>/Trigonometric Function'
   */
  if (Ctrl_Stim2__SineOrRect) {
    rtb_Switch2_a = emDriveSin((real32_T)UnitDelay_DSTATE_e / (real32_T)
      rtb_DataTypeConversion3 * 6.28318548F) * Ctrl_Stim2__Amplitude;
  } else if (UnitDelay_DSTATE_e >= (uint32_T)((real32_T)rtb_DataTypeConversion3 *
              0.5F)) {
    /* Switch: '<S295>/Switch1' incorporates:
     *  Constant: '<S11>/Constant8'
     *  Gain: '<S295>/Gain'
     */
    rtb_Switch2_a = -Ctrl_Stim2__Amplitude;
  } else {
    /* Switch: '<S295>/Switch1' incorporates:
     *  Constant: '<S11>/Constant8'
     */
    rtb_Switch2_a = Ctrl_Stim2__Amplitude;
  }

  /* End of Switch: '<S295>/Switch2' */
  /* End of Outputs for SubSystem: '<S11>/PulseGenerator1' */

  /* Switch: '<S11>/Switch' incorporates:
   *  Constant: '<S11>/Constant11'
   */
  if (Ctrl_Stim1__GenTarget <= 0) {
    /* Sum: '<S294>/Add' */
    emDriveFOC_control_DW.RPM_act = 0.0F;
  }

  /* End of Switch: '<S11>/Switch' */

  /* Switch: '<S11>/Switch1' incorporates:
   *  Constant: '<S11>/Constant12'
   *  Constant: '<S11>/Constant9'
   *  Sum: '<S295>/Add'
   */
  if (Ctrl_Stim2__GenTarget > 0) {
    /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator1' */
    emDriveFOC_control_DW.Gain2 = rtb_Switch2_a + Ctrl_Stim2__Offset;

    /* End of Outputs for SubSystem: '<S11>/PulseGenerator1' */
  } else {
    emDriveFOC_control_DW.Gain2 = 0.0F;
  }

  /* End of Switch: '<S11>/Switch1' */

  /* SignalConversion: '<S296>/Signal Conversion' */
  Ctrl_Stim_Status[0] = emDriveFOC_control_DW.RPM_act;
  Ctrl_Stim_Status[1] = emDriveFOC_control_DW.Gain2;

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator' */
  /* Switch: '<S294>/Switch' incorporates:
   *  Constant: '<S294>/Constant1'
   *  Constant: '<S297>/FixPt Constant'
   *  RelationalOperator: '<S294>/Relational Operator'
   *  Sum: '<S297>/FixPt Sum1'
   */
  if (UnitDelay_DSTATE_k + 1U >= rtb_DataTypeConversion3_l) {
    UnitDelay_DSTATE_k = 0U;
  } else {
    UnitDelay_DSTATE_k++;
  }

  /* End of Switch: '<S294>/Switch' */
  /* End of Outputs for SubSystem: '<S11>/PulseGenerator' */

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator1' */
  /* Switch: '<S295>/Switch' incorporates:
   *  Constant: '<S295>/Constant1'
   *  Constant: '<S298>/FixPt Constant'
   *  RelationalOperator: '<S295>/Relational Operator'
   *  Sum: '<S298>/FixPt Sum1'
   */
  if (UnitDelay_DSTATE_e + 1U >= rtb_DataTypeConversion3) {
    UnitDelay_DSTATE_e = 0U;
  } else {
    UnitDelay_DSTATE_e++;
  }

  /* End of Switch: '<S295>/Switch' */
  /* End of Outputs for SubSystem: '<S11>/PulseGenerator1' */

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator' */
  /* Update for UnitDelay: '<S294>/Unit Delay' */
  emDriveFOC_control_DW.UnitDelay_DSTATE_k = UnitDelay_DSTATE_k;

  /* End of Outputs for SubSystem: '<S11>/PulseGenerator' */

  /* Outputs for Atomic SubSystem: '<S11>/PulseGenerator1' */
  /* Update for UnitDelay: '<S295>/Unit Delay' */
  emDriveFOC_control_DW.UnitDelay_DSTATE_e = UnitDelay_DSTATE_e;

  /* End of Outputs for SubSystem: '<S11>/PulseGenerator1' */
}

FOC_RESTORE_CODE
  /* System initialize for atomic system: '<Root>/Velocity_loop' */
  FOC_SLOW_CODE
  static void emDriveF_Velocity_loop_Init(void)
{
  /* SystemInitialize for Atomic SubSystem: '<S14>/Velocity_regulator' */
  /* SystemInitialize for Atomic SubSystem: '<S302>/Velocity PID' */
  /* InitializeConditions for Delay: '<S307>/Delay1' */
  emDriveFOC_control_DW.icLoad = true;

  /* InitializeConditions for Delay: '<S307>/Delay' */
  emDriveFOC_control_DW.icLoad_c = true;

  /* SystemInitialize for Atomic SubSystem: '<S307>/LPF' */
  /* InitializeConditions for Delay: '<S312>/Delay' */
  emDriveFOC_control_DW.icLoad_j = true;

  /* End of SystemInitialize for SubSystem: '<S307>/LPF' */
  /* End of SystemInitialize for SubSystem: '<S302>/Velocity PID' */
  /* End of SystemInitialize for SubSystem: '<S14>/Velocity_regulator' */
}

FOC_RESTORE_CODE
  /* Output and update for atomic system: '<Root>/Velocity_loop' */
  FOC_SLOW_CODE
  static void emDriveFOC_co_Velocity_loop(void)
{
  real32_T Delay1_DSTATE;
  real32_T Delay_DSTATE;
  real32_T Delay_DSTATE_a;
  real32_T rtb_Abs_d;
  real32_T rtb_Add1_n;
  real32_T rtb_Dgain;
  real32_T rtb_Igain;
  real32_T rtb_Pgain;
  real32_T rtb_Subtract2;
  real32_T rtb_Switch_p4;
  real32_T rtb_TrVal;
  real32_T tmp;
  boolean_T rtb_LogicalOperator1;

  /* Outputs for Atomic SubSystem: '<S14>/Velocity_regulator' */
  /* Outputs for Atomic SubSystem: '<S302>/Velocity PID' */
  /* Delay: '<S307>/Delay1' */
  Delay1_DSTATE = emDriveFOC_control_DW.Delay1_DSTATE;

  /* Delay: '<S307>/Delay' */
  Delay_DSTATE = emDriveFOC_control_DW.Delay_DSTATE;

  /* End of Outputs for SubSystem: '<S302>/Velocity PID' */
  /* End of Outputs for SubSystem: '<S14>/Velocity_regulator' */

  /* Switch: '<S312>/Switch' */
  Delay_DSTATE_a = emDriveFOC_control_DW.Delay_DSTATE_a;

  /* Product: '<S14>/Product' incorporates:
   *  Constant: '<S14>/OmegaEtoRPM'
   */
  emDriveFOC_control_DW.RPM_act = 60.0F / (Ctrl_Motor__Np * 2.0F * 3.14159274F) *
    emDriveFOC_control_DW.omega_r;

  /* Logic: '<S14>/Logical Operator4' */
  Ctrl_VelocityReg_StatusDissableIntegration =
    (emDriveFOC_control_DW.MTPV_active || emDriveFOC_control_DW.pow_lim ||
     emDriveFOC_control_DW.Udc_lim);

  /* SignalConversion: '<S301>/Signal Conversion' incorporates:
   *  Sum: '<S14>/Add'
   */
  Ctrl_VelocityReg_StatusRPM[0] = emDriveFOC_control_DW.RPM_out;
  Ctrl_VelocityReg_StatusRPM[1] = emDriveFOC_control_DW.RPM_act;
  Ctrl_VelocityReg_StatusRPM[2] = emDriveFOC_control_DW.RPM_act -
    emDriveFOC_control_DW.RPM_out;

  /* Outputs for Atomic SubSystem: '<S14>/Velocity_regulator' */
  /* Logic: '<S302>/Logical Operator1' incorporates:
   *  Logic: '<S14>/Logical Operator1'
   *  Logic: '<S14>/Logical Operator2'
   */
  rtb_LogicalOperator1 = (emDriveFOC_control_DW.RateTransition13 ||
    (!emDriveFOC_control_DW.RateTransition6));

  /* Switch: '<S302>/Switch' incorporates:
   *  Constant: '<S14>/Start torque'
   *  Constant: '<S302>/Constant'
   *  Product: '<S302>/Product'
   *  Signum: '<S302>/Sign'
   */
  if (rtb_LogicalOperator1) {
    rtb_TrVal = 0.0F;
  } else {
    if (emDriveFOC_control_DW.RPM_out < 0.0F) {
      /* Signum: '<S302>/Sign' */
      rtb_Add1_n = -1.0F;
    } else {
      /* Signum: '<S302>/Sign' */
      rtb_Add1_n = (real32_T)(emDriveFOC_control_DW.RPM_out > 0.0F);
    }

    rtb_TrVal = Ctrl_VelocityReg__StartIq * rtb_Add1_n;
  }

  /* End of Switch: '<S302>/Switch' */

  /* SignalConversion: '<S305>/Signal Conversion' incorporates:
   *  Logic: '<S302>/Logical Operator'
   */
  Ctrl_VelocityReg_StatusTracking = (emDriveFOC_control_DW.RPM_Track ||
    rtb_LogicalOperator1);

  /* Abs: '<S309>/Abs' */
  rtb_Abs_d = fabsf(emDriveFOC_control_DW.RPM_act);

  /* MATLAB Function: '<S309>/MATLAB Function' incorporates:
   *  Constant: '<S309>/Constant1'
   *  Constant: '<S309>/Constant11'
   *  Constant: '<S309>/Constant12'
   *  Constant: '<S309>/Constant13'
   *  Constant: '<S309>/Constant14'
   *  Constant: '<S309>/Constant15'
   *  Constant: '<S309>/Constant16'
   *  Constant: '<S309>/Constant2'
   *  Constant: '<S309>/Constant3'
   */
  /* MATLAB Function 'Velocity_loop/Velocity_regulator/Vel_PID_param/SpeedParamScheduler/MATLAB Function': '<S311>:1' */
  /* '<S311>:1:7' if(RPMMeas < Ctrl_VelocityReg__SpeedLimFirst) */
  if (rtb_Abs_d < Ctrl_VelocityReg__SpeedLimFirst) {
    /* '<S311>:1:8' Pgain = Ctrl_VelocityReg__Pgain; */
    rtb_Pgain = Ctrl_VelocityReg__Pgain;

    /* '<S311>:1:9' Igain = Ctrl_VelocityReg__Igain; */
    rtb_Igain = Ctrl_VelocityReg__Igain;

    /* '<S311>:1:10' Dgain = Ctrl_VelocityReg__Dgain; */
    rtb_Dgain = Ctrl_VelocityReg__Dgain;
  } else if (rtb_Abs_d < Ctrl_VelocityReg__SpeedLimSecond) {
    /* '<S311>:1:11' elseif(RPMMeas < Ctrl_VelocityReg__SpeedLimSecond) */
    /* '<S311>:1:12' Pgain = Ctrl_VelocityReg__Pgain_LoadMeas; */
    rtb_Pgain = Ctrl_VelocityReg__Pgain_LoadMeas;

    /* '<S311>:1:13' Igain = Ctrl_VelocityReg__Igain_LoadMeas; */
    rtb_Igain = Ctrl_VelocityReg__Igain_LoadMeas;

    /* '<S311>:1:14' Dgain = single(0); */
    rtb_Dgain = 0.0F;
  } else {
    /* '<S311>:1:15' else */
    /* '<S311>:1:16' Pgain = Ctrl_VelocityReg__PgainHigh; */
    rtb_Pgain = Ctrl_VelocityReg__PgainHigh;

    /* '<S311>:1:17' Igain = Ctrl_VelocityReg__IgainHigh; */
    rtb_Igain = Ctrl_VelocityReg__IgainHigh;

    /* '<S311>:1:18' Dgain = single(0); */
    rtb_Dgain = 0.0F;
  }

  /* End of MATLAB Function: '<S309>/MATLAB Function' */

  /* Outputs for Atomic SubSystem: '<S306>/LPF_coef1' */
  /* Switch: '<S308>/Switch' incorporates:
   *  Constant: '<S306>/Constant5'
   *  Constant: '<S308>/Constant'
   *  Constant: '<S308>/Constant1'
   *  Constant: '<S308>/Constant2'
   *  MATLAB Function: '<S308>/MATLAB Function1'
   */
  /* MATLAB Function 'LPF_coef1/MATLAB Function1': '<S310>:1' */
  /* '<S310>:1:3' alpha = ts/(TimeConstant+ts); */
  if (Ctrl_VelocityReg__DpartLPF != 0.0F) {
    /* MATLAB Function: '<S308>/MATLAB Function1' incorporates:
     *  Constant: '<S306>/Constant5'
     */
    rtb_Abs_d = (real32_T)Ctrl_Gen__CurToVelRatio / Ctrl_Gen__Fcur;
    rtb_Switch_p4 = rtb_Abs_d / (1.0F / (6.28318548F *
      Ctrl_VelocityReg__DpartLPF) + rtb_Abs_d);
  } else {
    rtb_Switch_p4 = 0.0F;
  }

  /* End of Switch: '<S308>/Switch' */
  /* End of Outputs for SubSystem: '<S306>/LPF_coef1' */

  /* Outputs for Atomic SubSystem: '<S302>/Velocity PID' */
  /* Sum: '<S307>/Subtract1' */
  rtb_Abs_d = emDriveFOC_control_DW.RPM_out - emDriveFOC_control_DW.RPM_act;

  /* Product: '<S307>/POut' */
  rtb_Pgain *= rtb_Abs_d;

  /* Delay: '<S307>/Delay1' */
  if (emDriveFOC_control_DW.icLoad) {
    Delay1_DSTATE = rtb_Abs_d;
  }

  /* Sum: '<S307>/Subtract2' */
  rtb_Subtract2 = rtb_Abs_d - Delay1_DSTATE;

  /* Outputs for Atomic SubSystem: '<S307>/LPF' */
  /* Delay: '<S312>/Delay' */
  if (emDriveFOC_control_DW.icLoad_j) {
    /* Switch: '<S312>/Switch' */
    Delay_DSTATE_a = rtb_Subtract2;
  }

  /* End of Delay: '<S312>/Delay' */

  /* Switch: '<S312>/Switch' incorporates:
   *  Product: '<S312>/Product'
   *  Sum: '<S312>/Add'
   *  Sum: '<S312>/Add1'
   */
  Delay_DSTATE_a += (rtb_Subtract2 - Delay_DSTATE_a) * rtb_Switch_p4;

  /* Update for Delay: '<S312>/Delay' */
  emDriveFOC_control_DW.icLoad_j = false;

  /* Switch: '<S312>/Switch1' */
  if (rtb_Switch_p4 != 0.0F) {
    rtb_Subtract2 = Delay_DSTATE_a;
  }

  /* End of Outputs for SubSystem: '<S307>/LPF' */

  /* Product: '<S307>/Divide' incorporates:
   *  Constant: '<S306>/Constant5'
   *  Product: '<S307>/IOut1'
   *  Product: '<S307>/Product'
   *  Switch: '<S307>/Switch2'
   *  Switch: '<S312>/Switch1'
   */
  rtb_Switch_p4 = (real32_T)Ctrl_Gen__CurToVelRatio / Ctrl_Gen__Fcur;

  /* Outputs for Atomic SubSystem: '<S307>/LPF' */
  rtb_Dgain = rtb_Subtract2 * rtb_Dgain / rtb_Switch_p4;

  /* End of Outputs for SubSystem: '<S307>/LPF' */

  /* Sum: '<S307>/Add2' */
  rtb_Subtract2 = rtb_Pgain + rtb_Dgain;

  /* Switch: '<S314>/Switch2' incorporates:
   *  RelationalOperator: '<S314>/LowerRelop1'
   *  RelationalOperator: '<S314>/UpperRelop'
   *  Switch: '<S314>/Switch'
   *  UnaryMinus: '<S300>/Unary Minus'
   */
  if (rtb_Subtract2 > emDriveFOC_control_DW.IqMax) {
    rtb_Subtract2 = emDriveFOC_control_DW.IqMax;
  } else if (rtb_Subtract2 < -emDriveFOC_control_DW.IqMax) {
    /* Switch: '<S314>/Switch' incorporates:
     *  UnaryMinus: '<S300>/Unary Minus'
     */
    rtb_Subtract2 = -emDriveFOC_control_DW.IqMax;
  }

  /* End of Switch: '<S314>/Switch2' */

  /* Sum: '<S307>/Add1' */
  rtb_Add1_n = rtb_TrVal - rtb_Subtract2;

  /* Delay: '<S307>/Delay' */
  if (emDriveFOC_control_DW.icLoad_c) {
    Delay_DSTATE = rtb_Add1_n;
  }

  /* Switch: '<S307>/Switch2' incorporates:
   *  Product: '<S307>/IOut'
   *  Product: '<S307>/Product'
   *  Sum: '<S307>/Add'
   *  Switch: '<S307>/Switch'
   *  UnitDelay: '<S307>/Unit Delay1'
   */
  if (!Ctrl_VelocityReg_StatusTracking) {
    /* Signum: '<S307>/Sign1' */
    if (rtb_Abs_d < 0.0F) {
      rtb_Add1_n = -1.0F;
    } else {
      rtb_Add1_n = (real32_T)(rtb_Abs_d > 0.0F);
    }

    /* Signum: '<S307>/Sign' */
    if (Delay_DSTATE < 0.0F) {
      tmp = -1.0F;
    } else {
      tmp = (real32_T)(Delay_DSTATE > 0.0F);
    }

    /* Switch: '<S307>/Switch' incorporates:
     *  Logic: '<S307>/Logical Operator'
     *  RelationalOperator: '<S307>/Relational Operator'
     *  Signum: '<S307>/Sign'
     *  Signum: '<S307>/Sign1'
     */
    if ((rtb_Add1_n == tmp) && Ctrl_VelocityReg_StatusDissableIntegration) {
      rtb_Add1_n = 0.0F;
    } else {
      rtb_Add1_n = rtb_Igain;
    }

    rtb_Add1_n = (rtb_Abs_d * rtb_Add1_n * rtb_Switch_p4 + Delay_DSTATE) +
      emDriveFOC_control_DW.UnitDelay1_DSTATE;
  }

  /* Switch: '<S307>/Switch3' */
  if (rtb_Igain != 0.0F) {
    rtb_Igain = rtb_Add1_n;
  } else {
    rtb_Igain = 0.0F;
  }

  /* End of Switch: '<S307>/Switch3' */

  /* Switch: '<S307>/Switch1' incorporates:
   *  Sum: '<S307>/Sum'
   */
  if (Ctrl_VelocityReg_StatusTracking) {
    rtb_Switch_p4 = rtb_TrVal;
  } else {
    rtb_Switch_p4 = rtb_Subtract2 + rtb_Igain;
  }

  /* End of Switch: '<S307>/Switch1' */

  /* Switch: '<S313>/Switch2' incorporates:
   *  RelationalOperator: '<S313>/LowerRelop1'
   *  RelationalOperator: '<S313>/UpperRelop'
   *  Switch: '<S313>/Switch'
   *  UnaryMinus: '<S300>/Unary Minus'
   */
  if (rtb_Switch_p4 > emDriveFOC_control_DW.IqMax) {
    /* Switch: '<S313>/Switch2' */
    emDriveFOC_control_DW.Switch2 = emDriveFOC_control_DW.IqMax;
  } else if (rtb_Switch_p4 < -emDriveFOC_control_DW.IqMax) {
    /* Switch: '<S313>/Switch' incorporates:
     *  Switch: '<S313>/Switch2'
     *  UnaryMinus: '<S300>/Unary Minus'
     */
    emDriveFOC_control_DW.Switch2 = -emDriveFOC_control_DW.IqMax;
  } else {
    /* Switch: '<S313>/Switch2' incorporates:
     *  Switch: '<S313>/Switch'
     */
    emDriveFOC_control_DW.Switch2 = rtb_Switch_p4;
  }

  /* End of Switch: '<S313>/Switch2' */

  /* Update for Delay: '<S307>/Delay1' */
  emDriveFOC_control_DW.icLoad = false;

  /* Update for Delay: '<S307>/Delay' */
  emDriveFOC_control_DW.icLoad_c = false;

  /* Update for UnitDelay: '<S307>/Unit Delay1' incorporates:
   *  Gain: '<S307>/Antiwindup back gain'
   *  Sum: '<S307>/SumI2'
   */
  emDriveFOC_control_DW.UnitDelay1_DSTATE = (emDriveFOC_control_DW.Switch2 -
    rtb_Switch_p4) * 0.5F;

  /* End of Outputs for SubSystem: '<S302>/Velocity PID' */

  /* SignalConversion: '<S303>/Signal Conversion' incorporates:
   *  UnaryMinus: '<S300>/Unary Minus'
   */
  Ctrl_VelocityReg_StatusIq[0] = emDriveFOC_control_DW.Switch2;
  Ctrl_VelocityReg_StatusIq[1] = -emDriveFOC_control_DW.IqMax;
  Ctrl_VelocityReg_StatusIq[2] = emDriveFOC_control_DW.IqMax;
  Ctrl_VelocityReg_StatusIq[3] = rtb_TrVal;
  Ctrl_VelocityReg_StatusIq[4] = rtb_Pgain;
  Ctrl_VelocityReg_StatusIq[5] = rtb_Igain;
  Ctrl_VelocityReg_StatusIq[6] = rtb_Dgain;

  /* Outputs for Atomic SubSystem: '<S302>/Velocity PID' */
  /* Update for Delay: '<S307>/Delay' */
  Delay_DSTATE = rtb_Add1_n;

  /* Update for Delay: '<S307>/Delay1' */
  Delay1_DSTATE = rtb_Abs_d;

  /* End of Outputs for SubSystem: '<S302>/Velocity PID' */
  /* End of Outputs for SubSystem: '<S14>/Velocity_regulator' */

  /* Switch: '<S312>/Switch' */
  emDriveFOC_control_DW.Delay_DSTATE_a = Delay_DSTATE_a;

  /* Outputs for Atomic SubSystem: '<S14>/Velocity_regulator' */
  /* Outputs for Atomic SubSystem: '<S302>/Velocity PID' */
  /* Delay: '<S307>/Delay' */
  emDriveFOC_control_DW.Delay_DSTATE = Delay_DSTATE;

  /* Delay: '<S307>/Delay1' */
  emDriveFOC_control_DW.Delay1_DSTATE = Delay1_DSTATE;

  /* End of Outputs for SubSystem: '<S302>/Velocity PID' */
  /* End of Outputs for SubSystem: '<S14>/Velocity_regulator' */
}

FOC_RESTORE_CODE
  /* Model step function for TID0 */
  FOC_FAST_CODE
  void emDriveFOC_control_step0(void)  /* Sample time: [6.25E-5s, 0.0s] */
{
  int32_T max_ind;
  real32_T rtb_Switch_co[3];
  real32_T Ctrl_CurrentReg_StatusUabc_0;
  real32_T Ctrl_CurrentReg_StatusUabc_1;
  real32_T Ctrl_Mod_StatusPWM_0;
  real32_T Delay_DSTATE_d3;
  real32_T Delay_DSTATE_e;
  real32_T Delay_DSTATE_g;
  real32_T Delay_DSTATE_hi;
  real32_T Delay_DSTATE_k;
  real32_T Q;
  real32_T UnitDelay2_DSTATE;
  real32_T UnitDelay3_DSTATE_a;
  real32_T UnitDelay5_DSTATE;
  real32_T UnitDelay_DSTATE_o;
  real32_T rtb_FeedFW;
  real32_T rtb_Gain1_n;
  real32_T rtb_Gain_a;
  real32_T rtb_MaxampforSVPWM;
  real32_T rtb_PFF_f;
  real32_T rtb_Product;
  real32_T rtb_Product1_c;
  real32_T rtb_Product2_k;
  real32_T rtb_Subtract1_c;
  real32_T rtb_Subtract1_f;
  real32_T rtb_Switch2_g0;
  real32_T rtb_Switch_dr;
  real32_T rtb_Switch_gc;
  real32_T rtb_Theta;
  real32_T rtb_TreqVel;
  real32_T rtb_TrigonometricFunction_o2_c;
  real32_T rtb_y_jh_0;
  real32_T temp;
  boolean_T rtb_LogicalOperator1_k_tmp;
  Delay_DSTATE_e = emDriveFOC_control_DW.Delay_DSTATE_e;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
  /* MATLAB Function: '<S53>/Pitagora_calc2' incorporates:
   *  UnitDelay: '<S38>/Unit Delay2'
   */
  UnitDelay2_DSTATE = emDriveFOC_control_DW.UnitDelay2_DSTATE;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* RelationalOperator: '<S52>/Relational Operator' incorporates:
   *  UnitDelay: '<S53>/Unit Delay3'
   */
  UnitDelay3_DSTATE_a = emDriveFOC_control_DW.UnitDelay3_DSTATE_a;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Id' */

  /* Outputs for Atomic SubSystem: '<S38>/FW transform' */
  /* Sum: '<S101>/sumy' incorporates:
   *  UnitDelay: '<S38>/Unit Delay5'
   */
  UnitDelay5_DSTATE = emDriveFOC_control_DW.UnitDelay5_DSTATE;

  /* End of Outputs for SubSystem: '<S38>/FW transform' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  UnitDelay_DSTATE_o = emDriveFOC_control_DW.UnitDelay_DSTATE_o;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* Outputs for Atomic SubSystem: '<S52>/FieldWeakening' */
  /* Outputs for Atomic SubSystem: '<S65>/ Field weakening PI' */
  /* Switch: '<S70>/Switch3' incorporates:
   *  Switch: '<S70>/Switch2'
   */
  Delay_DSTATE_k = emDriveFOC_control_DW.Delay_DSTATE_k;

  /* End of Outputs for SubSystem: '<S65>/ Field weakening PI' */
  /* End of Outputs for SubSystem: '<S52>/FieldWeakening' */
  /* End of Outputs for SubSystem: '<S38>/Calculate_Id' */

  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* Outputs for Atomic SubSystem: '<S54>/ PI regulator D' */
  /* Switch: '<S83>/Switch3' incorporates:
   *  Switch: '<S83>/Switch2'
   */
  Delay_DSTATE_g = emDriveFOC_control_DW.Delay_DSTATE_g;

  /* End of Outputs for SubSystem: '<S54>/ PI regulator D' */
  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* Switch: '<S47>/Switch' */
  Delay_DSTATE_d3 = emDriveFOC_control_DW.Delay_DSTATE_d3;

  /* Switch: '<S71>/Switch' */
  Delay_DSTATE_hi = emDriveFOC_control_DW.Delay_DSTATE_hi;

  /* DataStoreWrite: '<S4>/Data Store Write2' */
  emDriveFOC_control_DW.Tsc = emDriveFOC_control_DW.Constant11;

  /* DataStoreWrite: '<S4>/Data Store Write' */
  emDriveFOC_control_DW.RPMtoOmegaE_c = emDriveFOC_control_DW.RPMtoOmegaE;

  /* DataStoreWrite: '<S4>/Data Store Write1' */
  emDriveFOC_control_DW.OmegaEtoRPM_k = emDriveFOC_control_DW.OmegaEtoRPM;

  /* RateTransition: '<Root>/Rate Transition2' */
  emDriveFOC_control_DW.RateTransition2 = Ctrl_Gen_Stat__RPMref;

  /* RateTransition: '<Root>/Rate Transition' */
  rtb_TreqVel = emDriveFOC_control_DW.RateTransition_Buffer0;

  /* Outputs for Atomic SubSystem: '<S3>/Overvoltage' */
  /* Switch: '<S220>/Switch' incorporates:
   *  Constant: '<S220>/Constant2'
   *  Constant: '<S220>/Constant4'
   *  Constant: '<S220>/Constant5'
   *  Inport: '<Root>/SensorsTsc'
   *  Product: '<S220>/Product2'
   *  Sum: '<S220>/Add2'
   */
  rtb_Theta = fmaxf((emDriveFOC_control_U.SensorsTsc.Udc -
                     Ctrl_Limits__UdcMaxToTorque) *
                    Ctrl_Limits__UdcMaxToTorqueGain, 0.0F);

  /* Signum: '<S220>/Sign' */
  if (UnitDelay_DSTATE_o < 0.0F) {
    max_ind = -1;
  } else {
    max_ind = (UnitDelay_DSTATE_o > 0.0F);
  }

  /* SignalConversion: '<S225>/Signal Conversion' incorporates:
   *  Product: '<S220>/Product3'
   *  Signum: '<S220>/Sign'
   */
  Ctrl_Limits_StatusUdcMaxToIq = rtb_Theta * (real32_T)max_ind;

  /* RelationalOperator: '<S220>/Relational Operator' incorporates:
   *  Constant: '<S220>/Constant'
   */
  emDriveFOC_control_DW.Udc_lim_o = (rtb_Theta > 0.0F);

  /* RelationalOperator: '<S220>/Relational Operator2' incorporates:
   *  Constant: '<S3>/Iq_max'
   */
  emDriveFOC_control_DW.pow_lim_a = (Ctrl_Limits__IqMax !=
    Ctrl_PowerLimit__StatusLimitIq);

  /* Switch: '<S220>/Switch1' */
  if (rtb_TreqVel > 0.0F) {
    /* Switch: '<S220>/Switch1' incorporates:
     *  MinMax: '<S220>/MinMax'
     *  Sum: '<S220>/Add3'
     */
    emDriveFOC_control_DW.Switch2_h = fminf(rtb_TreqVel +
      Ctrl_Limits_StatusUdcMaxToIq, Ctrl_PowerLimit__StatusLimitIq);
  } else {
    /* Switch: '<S220>/Switch1' incorporates:
     *  Gain: '<S220>/Gain'
     *  MinMax: '<S220>/MinMax1'
     *  Sum: '<S220>/Add3'
     */
    emDriveFOC_control_DW.Switch2_h = fmaxf(rtb_TreqVel +
      Ctrl_Limits_StatusUdcMaxToIq, -Ctrl_PowerLimit__StatusLimitIq);
  }

  /* End of Switch: '<S220>/Switch1' */

  /* Switch: '<S224>/Switch2' incorporates:
   *  Constant: '<S3>/Iq_max'
   *  RelationalOperator: '<S224>/LowerRelop1'
   *  RelationalOperator: '<S224>/UpperRelop'
   *  Switch: '<S224>/Switch'
   *  UnaryMinus: '<S220>/Unary Minus'
   */
  if (emDriveFOC_control_DW.Switch2_h > Ctrl_Limits__IqMax) {
    /* Switch: '<S220>/Switch1' */
    emDriveFOC_control_DW.Switch2_h = Ctrl_Limits__IqMax;
  } else if (emDriveFOC_control_DW.Switch2_h < -Ctrl_Limits__IqMax) {
    /* Switch: '<S220>/Switch1' incorporates:
     *  Switch: '<S224>/Switch'
     *  UnaryMinus: '<S220>/Unary Minus'
     */
    emDriveFOC_control_DW.Switch2_h = -Ctrl_Limits__IqMax;
  }

  /* End of Switch: '<S224>/Switch2' */
  /* End of Outputs for SubSystem: '<S3>/Overvoltage' */

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/AngleGen' */
  /* Delay: '<S47>/Delay' */
  if (emDriveFOC_control_DW.icLoad_g) {
    /* Switch: '<S47>/Switch' */
    Delay_DSTATE_d3 = 0.0F;
  }

  /* End of Delay: '<S47>/Delay' */

  /* Switch: '<S47>/Switch' */
  if (emDriveFOC_control_DW.RelationalOperator_le) {
    /* Switch: '<S47>/Switch' */
    Delay_DSTATE_d3 = 0.0F;
  }

  /* End of Switch: '<S47>/Switch' */

  /* Outputs for Atomic SubSystem: '<S47>/ThetaAdd' */
  /* MATLAB Function: '<S49>/ThetaAdd' incorporates:
   *  DataStoreRead: '<S37>/Data Store Read4'
   *  Product: '<S47>/Product'
   */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S50>:1' */
  /* '<S50>:1:3' y = u1+u2; */
  rtb_TreqVel = emDriveFOC_control_DW.Tsc * emDriveFOC_control_DW.Product +
    Delay_DSTATE_d3;

  /* '<S50>:1:4' if y > single(pi) */
  if (rtb_TreqVel > 3.14159274F) {
    /* '<S50>:1:5' y=y-single(2*pi); */
    rtb_TreqVel -= 6.28318548F;
  }

  /* '<S50>:1:7' if y < single(-pi) */
  if (rtb_TreqVel < -3.14159274F) {
    /* '<S50>:1:8' y=y+single(2*pi); */
    rtb_TreqVel += 6.28318548F;
  }

  /* End of MATLAB Function: '<S49>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S47>/ThetaAdd' */

  /* Outputs for Atomic SubSystem: '<S37>/ThetaAdd' */
  /* MATLAB Function: '<S48>/ThetaAdd' */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S51>:1' */
  /* '<S51>:1:3' y = u1+u2; */
  emDriveFOC_control_DW.theta_r = Delay_DSTATE_d3 + emDriveFOC_control_DW.Gain;

  /* '<S51>:1:4' if y > single(pi) */
  if (emDriveFOC_control_DW.theta_r > 3.14159274F) {
    /* '<S51>:1:5' y=y-single(2*pi); */
    emDriveFOC_control_DW.theta_r -= 6.28318548F;
  }

  /* '<S51>:1:7' if y < single(-pi) */
  if (emDriveFOC_control_DW.theta_r < -3.14159274F) {
    /* '<S51>:1:8' y=y+single(2*pi); */
    emDriveFOC_control_DW.theta_r += 6.28318548F;
  }

  /* End of MATLAB Function: '<S48>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S37>/ThetaAdd' */

  /* Update for Delay: '<S47>/Delay' */
  emDriveFOC_control_DW.icLoad_g = false;

  /* End of Outputs for SubSystem: '<S2>/AngleGen' */

  /* Outputs for Atomic SubSystem: '<S2>/abc-to-alphabeta' */
  /* Gain: '<S45>/Gain1' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   *  Sum: '<S45>/Add'
   */
  rtb_Gain1_n = ((emDriveFOC_control_U.SensorsTsc.Is_abc[0] +
                  emDriveFOC_control_U.SensorsTsc.Is_abc[1]) +
                 emDriveFOC_control_U.SensorsTsc.Is_abc[2]) * 0.333333343F;

  /* Sum: '<S45>/Add1' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   */
  rtb_Subtract1_c = emDriveFOC_control_U.SensorsTsc.Is_abc[1] - rtb_Gain1_n;

  /* DotProduct: '<S45>/Dot Product' incorporates:
   *  Constant: '<S45>/Constant2'
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   *  Sum: '<S45>/Add1'
   */
  rtb_Switch2_g0 = (emDriveFOC_control_U.SensorsTsc.Is_abc[0] - rtb_Gain1_n) *
    0.666666687F + rtb_Subtract1_c * -0.333333343F;

  /* DotProduct: '<S45>/Dot Product1' incorporates:
   *  Constant: '<S45>/Constant3'
   */
  rtb_FeedFW = rtb_Subtract1_c * 0.577350259F;

  /* Sum: '<S45>/Add1' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   */
  rtb_Subtract1_c = emDriveFOC_control_U.SensorsTsc.Is_abc[2] - rtb_Gain1_n;

  /* DotProduct: '<S45>/Dot Product' incorporates:
   *  Constant: '<S45>/Constant2'
   */
  emDriveFOC_control_DW.DotProduct = rtb_Subtract1_c * -0.333333343F +
    rtb_Switch2_g0;

  /* DotProduct: '<S45>/Dot Product1' incorporates:
   *  Constant: '<S45>/Constant3'
   */
  emDriveFOC_control_DW.DotProduct1 = rtb_Subtract1_c * -0.577350259F +
    rtb_FeedFW;

  /* End of Outputs for SubSystem: '<S2>/abc-to-alphabeta' */

  /* Switch generated from: '<S2>/Switch1' incorporates:
   *  Constant: '<S2>/Constant2'
   *  Constant: '<S2>/Constant8'
   *  Inport: '<Root>/SensorsTsc'
   *  Logic: '<S2>/Logical Operator'
   */
  if (Ctrl_AngleGenerator__Enable || Ctrl_ALTTestMode__Enable) {
    emDriveFOC_control_DW.omega_r_c = emDriveFOC_control_DW.Product;
  } else {
    emDriveFOC_control_DW.theta_r = emDriveFOC_control_U.SensorsTsc.Pos.theta_r;
    emDriveFOC_control_DW.omega_r_c =
      emDriveFOC_control_U.SensorsTsc.Pos.omega_r;
  }

  /* End of Switch generated from: '<S2>/Switch1' */

  /* Outputs for Atomic SubSystem: '<S2>/SensorlessTop' */
  /* Outputs for Enabled SubSystem: '<S41>/Sensorless' */
  emDriveFOC_contr_Sensorless();

  /* End of Outputs for SubSystem: '<S41>/Sensorless' */

  /* Outputs for Enabled SubSystem: '<S41>/SensorlessDummy' incorporates:
   *  EnablePort: '<S119>/Enable'
   */
  /* Logic: '<S41>/Logical Operator' incorporates:
   *  Constant: '<S41>/EN_sensorless1'
   */
  if (!Ctrl_Sensorless__Enabled) {
    /* SignalConversion generated from: '<S119>/Signal Conversion' */
    emDriveFOC_control_DW.theta_r_d = emDriveFOC_control_DW.theta_r;

    /* SignalConversion generated from: '<S119>/Signal Conversion' */
    emDriveFOC_control_DW.omega_r_l = emDriveFOC_control_DW.omega_r_c;

    /* SignalConversion generated from: '<S119>/Signal Conversion' */
    emDriveFOC_control_DW.Iq = emDriveFOC_control_DW.Switch2_h;

    /* SignalConversion generated from: '<S119>/Signal Conversion' incorporates:
     *  Constant: '<S117>/Constant'
     */
    emDriveFOC_control_DW.Id = 0.0F;

    /* SignalConversion generated from: '<S119>/Signal Conversion' incorporates:
     *  Constant: '<S117>/Constant4'
     */
    emDriveFOC_control_DW.RPM_Track_b = false;

    /* SignalConversion generated from: '<S119>/Signal Conversion' incorporates:
     *  Constant: '<S117>/Constant3'
     */
    emDriveFOC_control_DW.RPM_h = 0.0F;

    /* SignalConversion generated from: '<S119>/Signal Conversion' incorporates:
     *  Constant: '<S120>/Constant'
     */
    emDriveFOC_control_DW.State = enumSenzorlessState_None;

    /* SignalConversion generated from: '<S119>/Signal Conversion' */
    emDriveFOC_control_DW.ErrCode = 32;
  }

  /* End of Logic: '<S41>/Logical Operator' */
  /* End of Outputs for SubSystem: '<S41>/SensorlessDummy' */
  /* End of Outputs for SubSystem: '<S2>/SensorlessTop' */

  /* Outputs for Atomic SubSystem: '<S2>/PreventZero' */
  /* Switch: '<S39>/Switch' incorporates:
   *  Constant: '<S39>/Constant1'
   *  Inport: '<Root>/SensorsTsc'
   */
  rtb_Gain1_n = fmaxf(emDriveFOC_control_U.SensorsTsc.Udc, 1.0E-6F);

  /* End of Outputs for SubSystem: '<S2>/PreventZero' */

  /* Outputs for Atomic SubSystem: '<S2>/PreventZero1' */
  /* Switch: '<S40>/Switch' incorporates:
   *  Constant: '<S40>/Constant1'
   */
  rtb_Switch_gc = fmaxf(Ctrl_Limits_StatusIsMax, 1.0E-6F);

  /* End of Outputs for SubSystem: '<S2>/PreventZero1' */

  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
  /* MATLAB Function: '<S53>/Pitagora_calc2' */
  /* MATLAB Function 'Pitagora_calc1': '<S79>:1' */
  /* '<S79>:1:3' temp = S^2 - D^2; */
  temp = rtb_Switch_gc * rtb_Switch_gc - UnitDelay2_DSTATE * UnitDelay2_DSTATE;

  /* '<S79>:1:4' if (temp > 0) */
  if (temp > 0.0F) {
    /* '<S79>:1:5' Q = sqrt(temp); */
    Q = temp;
    Q = emDriveSqrt(Q);
  } else {
    /* '<S79>:1:6' else */
    /* '<S79>:1:7' Q=single(0); */
    Q = 0.0F;
  }

  /* Sum: '<S53>/Add1' incorporates:
   *  Constant: '<S53>/Constant'
   *  Constant: '<S53>/Constant6'
   *  Gain: '<S53>/Gain1'
   *  Product: '<S53>/Product2'
   */
  rtb_PFF_f = -0.2F * Ctrl_Limits__IdMin * UnitDelay3_DSTATE_a + 1.0F;

  /* Saturate: '<S53>/Saturation' */
  if (rtb_PFF_f > 1.0F) {
    rtb_PFF_f = 1.0F;
  } else if (rtb_PFF_f < 0.0F) {
    rtb_PFF_f = 0.0F;
  }

  /* Switch: '<S82>/Switch2' incorporates:
   *  RelationalOperator: '<S82>/LowerRelop1'
   *  RelationalOperator: '<S82>/UpperRelop'
   *  Switch: '<S82>/Switch'
   *  UnaryMinus: '<S81>/Unary Minus'
   */
  if (emDriveFOC_control_DW.Iq > Q) {
    rtb_Subtract1_c = Q;
  } else if (emDriveFOC_control_DW.Iq < -Q) {
    /* Switch: '<S82>/Switch' incorporates:
     *  UnaryMinus: '<S81>/Unary Minus'
     */
    rtb_Subtract1_c = -Q;
  } else {
    rtb_Subtract1_c = emDriveFOC_control_DW.Iq;
  }

  /* SignalConversion: '<S80>/Signal Conversion' */
  Ctrl_CurrentReg_StatusIqmax = Q;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */

  /* SignalConversion generated from: '<S38>/FOC_data' */
  emDriveFOC_control_DW.RPM_Track_m = emDriveFOC_control_DW.RPM_Track_b;

  /* Switch: '<S38>/Switch' incorporates:
   *  Constant: '<S38>/Constant2'
   *  Switch: '<S38>/Switch1'
   */
  if (Ctrl_IdtoIq_Switch_Enable) {
    rtb_Switch_dr = 0.0F;

    /* Switch: '<S38>/Switch1' incorporates:
     *  Sum: '<S38>/Add3'
     */
    UnitDelay5_DSTATE -= emDriveFOC_control_DW.Id;
  } else {
    rtb_Switch_dr = emDriveFOC_control_DW.Id;
  }

  /* End of Switch: '<S38>/Switch' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* Outputs for Atomic SubSystem: '<S52>/FieldWeakening' */
  /* MATLAB Function: '<S65>/Pitagora_calc' incorporates:
   *  UnitDelay: '<S38>/Unit Delay1'
   */
  /* MATLAB Function 'Pitagora_calc': '<S73>:1' */
  /* '<S73>:1:2' temp = D^2 + Q^2; */
  temp = emDriveFOC_control_DW.UnitDelay1_DSTATE_m[0] *
    emDriveFOC_control_DW.UnitDelay1_DSTATE_m[0] +
    emDriveFOC_control_DW.UnitDelay1_DSTATE_m[1] *
    emDriveFOC_control_DW.UnitDelay1_DSTATE_m[1];

  /* '<S73>:1:3' if temp > 0 */
  if (temp > 0.0F) {
    /*  this is necesary because ifx_sqrt returns NaN for sqrt(0) */
    /* '<S73>:1:4' S = sqrt(temp); */
    rtb_Theta = emDriveSqrt(temp);
  } else {
    /* '<S73>:1:5' else */
    /* '<S73>:1:6' S = single(0); */
    rtb_Theta = 0.0F;
  }

  /* End of MATLAB Function: '<S65>/Pitagora_calc' */

  /* Gain: '<S65>/Max amp. for SVPWM' */
  rtb_MaxampforSVPWM = 0.577350259F * rtb_Gain1_n;

  /* Outputs for Atomic SubSystem: '<S65>/SafeDivide2' */
  /* Switch: '<S74>/Switch1' incorporates:
   *  Product: '<S74>/Divide1'
   */
  Ctrl_CurrentReg_StatusModFactor = rtb_Theta / rtb_MaxampforSVPWM;

  /* End of Outputs for SubSystem: '<S65>/SafeDivide2' */

  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* Logic: '<S65>/Logical Operator1' incorporates:
   *  Inport: '<Root>/Control'
   *  Logic: '<S38>/Logical Operator'
   *  Logic: '<S38>/Logical Operator1'
   *  Logic: '<S54>/Logical Operator1'
   */
  rtb_LogicalOperator1_k_tmp = (emDriveFOC_control_U.Control.AscActive ||
    (!emDriveFOC_control_U.Control.RunStop));

  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */

  /* Outputs for Atomic SubSystem: '<S65>/ Field weakening PI' */
  /* Sum: '<S70>/Subtract1' incorporates:
   *  Constant: '<S65>/Constant2'
   */
  rtb_Subtract1_f = Ctrl_CurrentReg_StatusModFactor -
    Ctrl_FieldWeak__TargetModulationFactor;

  /* Sum: '<S70>/Add1' incorporates:
   *  Constant: '<S70>/Constant'
   *  Product: '<S70>/POut'
   */
  temp = rtb_Subtract1_f * Ctrl_FieldWeak__Pgain;

  /* Switch: '<S78>/Switch2' incorporates:
   *  Constant: '<S65>/Constant'
   *  Constant: '<S65>/Constant8'
   *  RelationalOperator: '<S78>/LowerRelop1'
   *  RelationalOperator: '<S78>/UpperRelop'
   *  Switch: '<S78>/Switch'
   */
  if (temp > 2.0F) {
    temp = 2.0F;
  } else if (temp < 0.0F) {
    /* Switch: '<S78>/Switch' incorporates:
     *  Constant: '<S65>/Constant8'
     */
    temp = 0.0F;
  }

  /* End of Switch: '<S78>/Switch2' */

  /* Delay: '<S70>/Delay' */
  if (emDriveFOC_control_DW.icLoad_o) {
    /* Switch: '<S70>/Switch1' incorporates:
     *  Sum: '<S70>/Add2'
     */
    Delay_DSTATE_k = 0.0F - temp;
  }

  /* End of Delay: '<S70>/Delay' */

  /* Switch: '<S70>/Switch1' incorporates:
   *  Logic: '<S65>/Logical Operator1'
   *  Sum: '<S70>/Sum'
   *  Switch: '<S70>/Switch2'
   *  Switch: '<S70>/Switch3'
   */
  if (rtb_LogicalOperator1_k_tmp) {
    /* Switch: '<S70>/Switch1' incorporates:
     *  Sum: '<S70>/Add2'
     */
    Delay_DSTATE_k = 0.0F - temp;
    temp = 0.0F;
  } else {
    /* Switch: '<S70>/Switch1' incorporates:
     *  Constant: '<S70>/Constant1'
     *  DataStoreRead: '<S65>/Data Store Read4'
     *  Product: '<S70>/IOut'
     *  Product: '<S70>/Product'
     *  Sum: '<S70>/Add'
     *  Switch: '<S70>/Switch'
     *  UnitDelay: '<S70>/Unit Delay1'
     */
    Delay_DSTATE_k = (rtb_Subtract1_f * Ctrl_FieldWeak__Igain *
                      emDriveFOC_control_DW.Tsc + Delay_DSTATE_k) +
      emDriveFOC_control_DW.UnitDelay1_DSTATE_mx;

    /* Switch: '<S70>/Switch3' incorporates:
     *  Constant: '<S70>/Constant1'
     */
    if (Ctrl_FieldWeak__Igain != 0.0F) {
      rtb_TrigonometricFunction_o2_c = Delay_DSTATE_k;
    } else {
      rtb_TrigonometricFunction_o2_c = 0.0F;
    }

    temp += rtb_TrigonometricFunction_o2_c;
  }

  /* End of Switch: '<S70>/Switch1' */

  /* Switch: '<S77>/Switch2' incorporates:
   *  Constant: '<S65>/Constant'
   *  Constant: '<S65>/Constant8'
   *  RelationalOperator: '<S77>/LowerRelop1'
   *  RelationalOperator: '<S77>/UpperRelop'
   *  Switch: '<S77>/Switch'
   */
  if (temp > 2.0F) {
    rtb_Subtract1_f = 2.0F;
  } else if (temp < 0.0F) {
    /* Switch: '<S77>/Switch' incorporates:
     *  Constant: '<S65>/Constant8'
     */
    rtb_Subtract1_f = 0.0F;
  } else {
    rtb_Subtract1_f = temp;
  }

  /* End of Switch: '<S77>/Switch2' */

  /* Gain: '<S70>/Antiwindup back gain' incorporates:
   *  Sum: '<S70>/SumI2'
   *  UnitDelay: '<S70>/Unit Delay1'
   */
  emDriveFOC_control_DW.UnitDelay1_DSTATE_mx = (rtb_Subtract1_f - temp) * 0.5F;

  /* Update for Delay: '<S70>/Delay' */
  emDriveFOC_control_DW.icLoad_o = false;

  /* End of Outputs for SubSystem: '<S65>/ Field weakening PI' */

  /* Product: '<S65>/Product1' incorporates:
   *  Constant: '<S65>/Constant2'
   */
  temp = Ctrl_FieldWeak__TargetModulationFactor * rtb_MaxampforSVPWM;

  /* SignalConversion: '<S76>/Signal Conversion' incorporates:
   *  Sum: '<S65>/Add1'
   */
  Ctrl_FieldWeak_StatusUs[2] = temp - rtb_Theta;

  /* Product: '<S65>/Product3' incorporates:
   *  Gain: '<S52>/Gain'
   */
  rtb_Switch_gc = -1.41421354F * rtb_Switch_gc * rtb_Subtract1_f;

  /* Outputs for Atomic SubSystem: '<S65>/LPFFieldWeak' */
  /* Delay: '<S71>/Delay' */
  if (emDriveFOC_control_DW.icLoad_n) {
    /* Switch: '<S71>/Switch' */
    Delay_DSTATE_hi = rtb_Switch_gc;
  }

  /* End of Delay: '<S71>/Delay' */

  /* Switch: '<S71>/Switch' incorporates:
   *  Logic: '<S65>/Logical Operator1'
   */
  if (rtb_LogicalOperator1_k_tmp) {
    /* Switch: '<S71>/Switch' */
    Delay_DSTATE_hi = rtb_Switch_gc;
  } else {
    /* Switch: '<S71>/Switch' incorporates:
     *  Product: '<S71>/Product'
     *  Sum: '<S71>/Add'
     *  Sum: '<S71>/Add1'
     */
    Delay_DSTATE_hi += (rtb_Switch_gc - Delay_DSTATE_hi) *
      emDriveFOC_control_DW.alpha_g;
  }

  /* End of Switch: '<S71>/Switch' */

  /* Switch: '<S71>/Switch1' */
  if (emDriveFOC_control_DW.alpha_g != 0.0F) {
    rtb_Subtract1_f = Delay_DSTATE_hi;
  } else {
    rtb_Subtract1_f = rtb_Switch_gc;
  }

  /* End of Switch: '<S71>/Switch1' */

  /* Update for Delay: '<S71>/Delay' */
  emDriveFOC_control_DW.icLoad_n = false;

  /* End of Outputs for SubSystem: '<S65>/LPFFieldWeak' */

  /* Saturate: '<S65>/Saturation' incorporates:
   *  Constant: '<S65>/Constant2'
   *  Constant: '<S65>/Constant7'
   *  Sum: '<S65>/Add'
   */
  if (Ctrl_FieldWeak__TargetModulationFactor + 0.2F > 10.0F) {
    rtb_TrigonometricFunction_o2_c = 10.0F;
  } else if (Ctrl_FieldWeak__TargetModulationFactor + 0.2F < 1.0F) {
    rtb_TrigonometricFunction_o2_c = 1.0F;
  } else {
    rtb_TrigonometricFunction_o2_c = Ctrl_FieldWeak__TargetModulationFactor +
      0.2F;
  }

  /* Product: '<S65>/Product2' incorporates:
   *  Saturate: '<S65>/Saturation'
   */
  rtb_Product2_k = rtb_TrigonometricFunction_o2_c * rtb_MaxampforSVPWM;

  /* SignalConversion: '<S75>/Signal Conversion' */
  Ctrl_FieldWeak_StatusId[0] = rtb_Subtract1_f;
  Ctrl_FieldWeak_StatusId[1] = rtb_Switch_gc;

  /* SignalConversion: '<S76>/Signal Conversion' */
  Ctrl_FieldWeak_StatusUs[0] = rtb_Theta;
  Ctrl_FieldWeak_StatusUs[1] = temp;

  /* End of Outputs for SubSystem: '<S52>/FieldWeakening' */

  /* Outputs for Atomic SubSystem: '<S52>/MTPA' */
  /* Product: '<S66>/Product' incorporates:
   *  Abs: '<S66>/Abs'
   *  Constant: '<S66>/Constant2'
   */
  temp = -(fabsf(UnitDelay5_DSTATE) * Ctrl_Motor__IdToIqRatio);

  /* End of Outputs for SubSystem: '<S52>/MTPA' */

  /* Sum: '<S52>/Add3' */
  rtb_Theta = temp + rtb_Switch_dr;

  /* Switch: '<S67>/Switch2' incorporates:
   *  Constant: '<S52>/Constant2'
   *  RelationalOperator: '<S67>/LowerRelop1'
   */
  if (rtb_Subtract1_f > 0.0F) {
    rtb_Switch_gc = 0.0F;
  } else {
    /* Sum: '<S52>/Add6' incorporates:
     *  Constant: '<S52>/Constant6'
     */
    rtb_Switch_gc = Ctrl_Limits__IdMin - rtb_Theta;

    /* Switch: '<S67>/Switch' incorporates:
     *  RelationalOperator: '<S67>/UpperRelop'
     */
    if (rtb_Subtract1_f >= rtb_Switch_gc) {
      rtb_Switch_gc = rtb_Subtract1_f;
    }

    /* End of Switch: '<S67>/Switch' */
  }

  /* End of Switch: '<S67>/Switch2' */

  /* Sum: '<S52>/Add1' */
  UnitDelay2_DSTATE = rtb_Switch_gc + rtb_Theta;

  /* Sum: '<S52>/Add4' */
  UnitDelay3_DSTATE_a = rtb_Subtract1_f - rtb_Switch_gc;

  /* RelationalOperator: '<S52>/Relational Operator' incorporates:
   *  Constant: '<S52>/Constant3'
   */
  emDriveFOC_control_DW.RelationalOperator_li = (UnitDelay3_DSTATE_a != 0.0F);

  /* SignalConversion: '<S69>/Signal Conversion' */
  Ctrl_CurrentReg_StatusId[0] = rtb_Switch_dr;
  Ctrl_CurrentReg_StatusId[1] = temp;
  Ctrl_CurrentReg_StatusId[2] = rtb_Switch_gc;
  Ctrl_CurrentReg_StatusId[3] = UnitDelay3_DSTATE_a;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Id' */

  /* Outputs for Atomic SubSystem: '<S38>/StimId' */
  /* Constant: '<S38>/Constant5' */
  emDriveFOC_control_StimUdTID0(UnitDelay2_DSTATE, 3, &temp,
    &emDriveFOC_control_DW.StimId);

  /* End of Outputs for SubSystem: '<S38>/StimId' */

  /* Switch: '<S38>/Switch2' incorporates:
   *  Constant: '<S38>/Constant2'
   *  Constant: '<S38>/Constant6'
   *  Product: '<S53>/Product1'
   *  Saturate: '<S53>/Saturation'
   *  Sum: '<S38>/Add1'
   *  Switch: '<S82>/Switch2'
   */
  if (Ctrl_IdtoIq_Switch_Enable) {
    /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
    rtb_Subtract1_c = rtb_PFF_f * rtb_Subtract1_c + emDriveFOC_control_DW.Id;

    /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */
  } else {
    /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
    rtb_Subtract1_c *= rtb_PFF_f;

    /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */
  }

  /* Outputs for Atomic SubSystem: '<S38>/StimIq' */
  emDriveFOC_control_StimUdTID0(rtb_Subtract1_c, 4, &rtb_MaxampforSVPWM,
    &emDriveFOC_control_DW.StimIq);

  /* End of Switch: '<S38>/Switch2' */
  /* End of Outputs for SubSystem: '<S38>/StimIq' */

  /* Outputs for Atomic SubSystem: '<S38>/FW transform' */
  /* Trigonometry: '<S56>/Trigonometric Function' */
  rtb_Subtract1_f = emDriveSin(emDriveFOC_control_DW.theta_r_d);
  rtb_TrigonometricFunction_o2_c = emDriveCos(emDriveFOC_control_DW.theta_r_d);

  /* Sum: '<S101>/sumx' incorporates:
   *  Product: '<S101>/AxBx'
   *  Product: '<S101>/AyBy'
   */
  rtb_Switch_dr = emDriveFOC_control_DW.DotProduct *
    rtb_TrigonometricFunction_o2_c + emDriveFOC_control_DW.DotProduct1 *
    rtb_Subtract1_f;

  /* Switch: '<S38>/Switch1' incorporates:
   *  Product: '<S101>/AxBy'
   *  Product: '<S101>/AyBx'
   *  Sum: '<S101>/sumy'
   */
  UnitDelay5_DSTATE = emDriveFOC_control_DW.DotProduct1 *
    rtb_TrigonometricFunction_o2_c - emDriveFOC_control_DW.DotProduct *
    rtb_Subtract1_f;

  /* End of Outputs for SubSystem: '<S38>/FW transform' */

  /* Outputs for Atomic SubSystem: '<S38>/IQ decoupling' */
  /* Product: '<S57>/Product2' incorporates:
   *  Constant: '<S57>/Constant11'
   */
  rtb_Switch_gc = emDriveFOC_control_DW.omega_r_l * Ctrl_Motor__Psi;

  /* End of Outputs for SubSystem: '<S38>/IQ decoupling' */

  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* SignalConversion: '<S86>/Signal Conversion' */
  Ctrl_CurrentReg_StatusIdqRef[0] = temp;
  Ctrl_CurrentReg_StatusIdqRef[1] = rtb_MaxampforSVPWM;

  /* SignalConversion: '<S88>/Signal Conversion' */
  Ctrl_CurrentReg_StatusIdqAct[0] = rtb_Switch_dr;
  Ctrl_CurrentReg_StatusIdqAct[1] = UnitDelay5_DSTATE;

  /* Outputs for Atomic SubSystem: '<S38>/IQ decoupling' */
  /* Product: '<S54>/Product' incorporates:
   *  Constant: '<S54>/Constant1'
   *  Constant: '<S57>/Constant2'
   *  Product: '<S57>/Product'
   *  UnaryMinus: '<S57>/Unary Minus'
   */
  rtb_Product = -(emDriveFOC_control_DW.omega_r_l * Ctrl_Motor__Lq *
                  UnitDelay5_DSTATE) * Ctrl_CurrentReg__DecoupleIdIqEnable;

  /* End of Outputs for SubSystem: '<S38>/IQ decoupling' */

  /* Outputs for Atomic SubSystem: '<S54>/ PI regulator D' */
  /* Sum: '<S83>/Subtract1' */
  rtb_Theta = Ctrl_CurrentReg_StatusIdqRef[0] - Ctrl_CurrentReg_StatusIdqAct[0];

  /* Sum: '<S83>/Add1' incorporates:
   *  Constant: '<S83>/Constant'
   *  Product: '<S83>/POut'
   */
  rtb_PFF_f = rtb_Theta * Ctrl_CurrentReg__DaxisPgain + rtb_Product;

  /* Switch: '<S93>/Switch2' incorporates:
   *  RelationalOperator: '<S93>/LowerRelop1'
   *  RelationalOperator: '<S93>/UpperRelop'
   *  Switch: '<S93>/Switch'
   *  UnaryMinus: '<S91>/Unary Minus'
   */
  if (rtb_PFF_f > rtb_Product2_k) {
    rtb_PFF_f = rtb_Product2_k;
  } else if (rtb_PFF_f < -rtb_Product2_k) {
    /* Switch: '<S93>/Switch' incorporates:
     *  UnaryMinus: '<S91>/Unary Minus'
     */
    rtb_PFF_f = -rtb_Product2_k;
  }

  /* End of Switch: '<S93>/Switch2' */

  /* Delay: '<S83>/Delay' */
  if (emDriveFOC_control_DW.icLoad_p) {
    /* Switch: '<S83>/Switch1' incorporates:
     *  Constant: '<S54>/Constant4'
     *  Sum: '<S83>/Add2'
     */
    Delay_DSTATE_g = 0.0F - rtb_PFF_f;
  }

  /* End of Delay: '<S83>/Delay' */

  /* Switch: '<S83>/Switch1' incorporates:
   *  Constant: '<S54>/Constant4'
   *  Sum: '<S83>/Sum'
   *  Switch: '<S83>/Switch2'
   *  Switch: '<S83>/Switch3'
   */
  if (rtb_LogicalOperator1_k_tmp) {
    /* Switch: '<S83>/Switch1' incorporates:
     *  Constant: '<S54>/Constant4'
     *  Sum: '<S83>/Add2'
     */
    Delay_DSTATE_g = 0.0F - rtb_PFF_f;
    rtb_PFF_f = 0.0F;
  } else {
    /* Switch: '<S83>/Switch1' incorporates:
     *  Constant: '<S83>/Constant1'
     *  DataStoreRead: '<S54>/Data Store Read4'
     *  Product: '<S83>/IOut'
     *  Product: '<S83>/Product'
     *  Sum: '<S83>/Add'
     *  Switch: '<S83>/Switch'
     *  UnitDelay: '<S83>/Unit Delay1'
     */
    Delay_DSTATE_g = (rtb_Theta * Ctrl_CurrentReg__DaxisIgain *
                      emDriveFOC_control_DW.Tsc + Delay_DSTATE_g) +
      emDriveFOC_control_DW.UnitDelay1_DSTATE_b;

    /* Switch: '<S83>/Switch3' incorporates:
     *  Constant: '<S83>/Constant1'
     */
    if (Ctrl_CurrentReg__DaxisIgain != 0.0F) {
      rtb_Subtract1_c = Delay_DSTATE_g;
    } else {
      rtb_Subtract1_c = 0.0F;
    }

    rtb_PFF_f += rtb_Subtract1_c;
  }

  /* End of Switch: '<S83>/Switch1' */

  /* Switch: '<S92>/Switch2' incorporates:
   *  RelationalOperator: '<S92>/LowerRelop1'
   *  RelationalOperator: '<S92>/UpperRelop'
   *  Switch: '<S92>/Switch'
   *  UnaryMinus: '<S91>/Unary Minus'
   */
  if (rtb_PFF_f > rtb_Product2_k) {
    rtb_Switch2_g0 = rtb_Product2_k;
  } else if (rtb_PFF_f < -rtb_Product2_k) {
    /* Switch: '<S92>/Switch' incorporates:
     *  UnaryMinus: '<S91>/Unary Minus'
     */
    rtb_Switch2_g0 = -rtb_Product2_k;
  } else {
    rtb_Switch2_g0 = rtb_PFF_f;
  }

  /* End of Switch: '<S92>/Switch2' */

  /* Gain: '<S83>/Antiwindup back gain' incorporates:
   *  Sum: '<S83>/SumI2'
   *  UnitDelay: '<S83>/Unit Delay1'
   */
  emDriveFOC_control_DW.UnitDelay1_DSTATE_b = (rtb_Switch2_g0 - rtb_PFF_f) *
    0.5F;

  /* Update for Delay: '<S83>/Delay' */
  emDriveFOC_control_DW.icLoad_p = false;

  /* End of Outputs for SubSystem: '<S54>/ PI regulator D' */

  /* Switch: '<S54>/Switch2' incorporates:
   *  Constant: '<S54>/Constant2'
   *  Constant: '<S54>/Constant5'
   */
  if (Ctrl_CurrentReg__DecouplePsi) {
    rtb_Subtract1_c = rtb_Switch_gc;
  } else {
    rtb_Subtract1_c = 0.0F;
  }

  /* Outputs for Atomic SubSystem: '<S38>/IQ decoupling' */
  /* Sum: '<S54>/Add' incorporates:
   *  Constant: '<S54>/Constant1'
   *  Constant: '<S57>/Constant1'
   *  Product: '<S54>/Product1'
   *  Product: '<S57>/Product1'
   *  Switch: '<S54>/Switch2'
   */
  rtb_FeedFW = emDriveFOC_control_DW.omega_r_l * Ctrl_Motor__Ld * rtb_Switch_dr *
    Ctrl_CurrentReg__DecoupleIdIqEnable + rtb_Subtract1_c;

  /* End of Outputs for SubSystem: '<S38>/IQ decoupling' */

  /* Outputs for Atomic SubSystem: '<S54>/ PI regulator Q' */
  /* Sum: '<S84>/Subtract1' */
  rtb_Subtract1_c = Ctrl_CurrentReg_StatusIdqRef[1] -
    Ctrl_CurrentReg_StatusIdqAct[1];

  /* Sum: '<S84>/Add1' incorporates:
   *  Constant: '<S84>/Constant'
   *  Product: '<S84>/POut'
   */
  rtb_Theta = rtb_Subtract1_c * Ctrl_CurrentReg__QaxisPgain + rtb_FeedFW;

  /* Switch: '<S95>/Switch2' incorporates:
   *  RelationalOperator: '<S95>/LowerRelop1'
   *  RelationalOperator: '<S95>/UpperRelop'
   *  Switch: '<S95>/Switch'
   *  UnaryMinus: '<S91>/Unary Minus'
   */
  if (rtb_Theta > rtb_Product2_k) {
    rtb_Theta = rtb_Product2_k;
  } else if (rtb_Theta < -rtb_Product2_k) {
    /* Switch: '<S95>/Switch' incorporates:
     *  UnaryMinus: '<S91>/Unary Minus'
     */
    rtb_Theta = -rtb_Product2_k;
  }

  /* End of Switch: '<S95>/Switch2' */

  /* Sum: '<S84>/Add2' */
  rtb_PFF_f = rtb_Switch_gc - rtb_Theta;

  /* Delay: '<S84>/Delay' */
  if (emDriveFOC_control_DW.icLoad_jc) {
    Delay_DSTATE_e = rtb_PFF_f;
  }

  /* End of Delay: '<S84>/Delay' */

  /* Switch: '<S84>/Switch1' incorporates:
   *  Constant: '<S84>/Constant1'
   *  DataStoreRead: '<S54>/Data Store Read4'
   *  Logic: '<S54>/Logical Operator1'
   *  Product: '<S84>/IOut'
   *  Product: '<S84>/Product'
   *  Sum: '<S84>/Add'
   *  Sum: '<S84>/Sum'
   *  Switch: '<S84>/Switch'
   *  Switch: '<S84>/Switch2'
   *  Switch: '<S84>/Switch3'
   *  UnitDelay: '<S84>/Unit Delay1'
   */
  if (!rtb_LogicalOperator1_k_tmp) {
    rtb_PFF_f = (rtb_Subtract1_c * Ctrl_CurrentReg__QaxisIgain *
                 emDriveFOC_control_DW.Tsc + Delay_DSTATE_e) +
      emDriveFOC_control_DW.UnitDelay1_DSTATE_i;

    /* Switch: '<S84>/Switch3' incorporates:
     *  Constant: '<S84>/Constant1'
     *  DataStoreRead: '<S54>/Data Store Read4'
     *  Product: '<S84>/IOut'
     *  Product: '<S84>/Product'
     *  Sum: '<S84>/Add'
     *  Switch: '<S84>/Switch'
     *  UnitDelay: '<S84>/Unit Delay1'
     */
    if (Ctrl_CurrentReg__QaxisIgain != 0.0F) {
      rtb_Subtract1_c = rtb_PFF_f;
    } else {
      rtb_Subtract1_c = 0.0F;
    }

    rtb_Switch_gc = rtb_Theta + rtb_Subtract1_c;
  }

  /* End of Switch: '<S84>/Switch1' */

  /* Switch: '<S94>/Switch2' incorporates:
   *  RelationalOperator: '<S94>/LowerRelop1'
   */
  if (rtb_Switch_gc <= rtb_Product2_k) {
    /* Switch: '<S94>/Switch' incorporates:
     *  RelationalOperator: '<S94>/UpperRelop'
     *  UnaryMinus: '<S91>/Unary Minus'
     */
    if (rtb_Switch_gc < -rtb_Product2_k) {
      rtb_Product2_k = -rtb_Product2_k;
    } else {
      rtb_Product2_k = rtb_Switch_gc;
    }

    /* End of Switch: '<S94>/Switch' */
  }

  /* End of Switch: '<S94>/Switch2' */

  /* Gain: '<S84>/Antiwindup back gain' incorporates:
   *  Sum: '<S84>/SumI2'
   *  UnitDelay: '<S84>/Unit Delay1'
   */
  emDriveFOC_control_DW.UnitDelay1_DSTATE_i = (rtb_Product2_k - rtb_Switch_gc) *
    0.5F;

  /* Update for Delay: '<S84>/Delay' */
  emDriveFOC_control_DW.icLoad_jc = false;

  /* End of Outputs for SubSystem: '<S54>/ PI regulator Q' */

  /* Switch: '<S85>/Switch5' incorporates:
   *  Abs: '<S85>/Abs'
   *  Constant: '<S85>/Constant2'
   *  Constant: '<S85>/Constant8'
   *  Lookup_n-D: '<S85>/Ud Lookup Table'
   *  Switch: '<S85>/Switch2'
   */
  if (Ctrl_ALTTestMode__Enable) {
    rtb_Theta =
      emDriveFOC_control_ConstP.UdLookupTable_tableData[plook_u32f_bincka(fabsf
      (emDriveFOC_control_DW.RateTransition2),
      emDriveFOC_control_ConstP.UdLookupTable_bp01Data, 5U)];
  } else {
    rtb_Theta = 0.0F;
  }

  /* End of Switch: '<S85>/Switch5' */

  /* SignalConversion: '<S96>/Signal Conversion' incorporates:
   *  Constant: '<S85>/Constant7'
   *  Switch: '<S85>/Switch6'
   */
  Ctrl_ALTTestMode_StatusUdq[0] = rtb_Theta;
  Ctrl_ALTTestMode_StatusUdq[1] = 0.0F;

  /* Outputs for Atomic SubSystem: '<S54>/StimUd' */
  /* Constant: '<S54>/Constant6' */
  emDriveFOC_control_StimUdTID0(rtb_Switch2_g0, 1, &rtb_Switch_gc,
    &emDriveFOC_control_DW.StimUd);

  /* End of Outputs for SubSystem: '<S54>/StimUd' */

  /* Switch: '<S54>/Switch3' incorporates:
   *  Constant: '<S54>/Constant8'
   */
  if (Ctrl_ALTTestMode__Enable) {
    rtb_Switch_gc = rtb_Theta;
  }

  /* End of Switch: '<S54>/Switch3' */

  /* Outputs for Atomic SubSystem: '<S54>/StrimUq' */
  /* Constant: '<S54>/Constant7' */
  emDriveFOC_control_StimUdTID0(rtb_Product2_k, 2, &rtb_Theta,
    &emDriveFOC_control_DW.StrimUq);

  /* End of Outputs for SubSystem: '<S54>/StrimUq' */

  /* Switch: '<S54>/Switch4' incorporates:
   *  Constant: '<S54>/Constant8'
   *  Constant: '<S85>/Constant7'
   *  Switch: '<S85>/Switch6'
   */
  if (Ctrl_ALTTestMode__Enable) {
    rtb_Theta = 0.0F;
  }

  /* End of Switch: '<S54>/Switch4' */

  /* SignalConversion: '<S87>/Signal Conversion' */
  Ctrl_CurrentReg_StatusUdq[0] = rtb_Switch_gc;
  Ctrl_CurrentReg_StatusUdq[1] = rtb_Theta;
  Ctrl_CurrentReg_StatusUdq[2] = rtb_Product;
  Ctrl_CurrentReg_StatusUdq[3] = rtb_FeedFW;

  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */

  /* Outputs for Atomic SubSystem: '<S38>/ReverseTransform1' */
  /* Outputs for Atomic SubSystem: '<S58>/CompDelayFOC' */
  /* Outputs for Atomic SubSystem: '<S102>/ThetaAdd' */
  /* MATLAB Function: '<S109>/ThetaAdd' incorporates:
   *  Constant: '<S58>/Constant7'
   *  DataStoreRead: '<S58>/Data Store Read4'
   *  Product: '<S102>/Product'
   *  Product: '<S58>/Product1'
   */
  /* MATLAB Function 'ThetaAdd/ThetaAdd': '<S110>:1' */
  /* '<S110>:1:3' y = u1+u2; */
  rtb_Subtract1_c = emDriveFOC_control_DW.Tsc *
    Ctrl_CurrentReg__CalculationDelay * emDriveFOC_control_DW.omega_r_l +
    emDriveFOC_control_DW.theta_r_d;

  /* '<S110>:1:4' if y > single(pi) */
  if (rtb_Subtract1_c > 3.14159274F) {
    /* '<S110>:1:5' y=y-single(2*pi); */
    rtb_Subtract1_c -= 6.28318548F;
  }

  /* '<S110>:1:7' if y < single(-pi) */
  if (rtb_Subtract1_c < -3.14159274F) {
    /* '<S110>:1:8' y=y+single(2*pi); */
    rtb_Subtract1_c += 6.28318548F;
  }

  /* End of MATLAB Function: '<S109>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S102>/ThetaAdd' */
  /* End of Outputs for SubSystem: '<S58>/CompDelayFOC' */

  /* Trigonometry: '<S58>/Trigonometric Function' */
  rtb_Product2_k = emDriveSin(rtb_Subtract1_c);
  rtb_Product = emDriveCos(rtb_Subtract1_c);

  /* Gain: '<S104>/Gain2' */
  rtb_Switch2_g0 = 0.5F * rtb_Gain1_n;

  /* Switch: '<S58>/Switch' incorporates:
   *  DotProduct: '<S107>/Dot Product3'
   *  DotProduct: '<S107>/Dot Product4'
   *  Inport: '<Root>/Control'
   *  SignalConversion generated from: '<S107>/Dot Product2'
   *  Sum: '<S103>/Add1'
   */
  if (emDriveFOC_control_U.Control.AscActive) {
    /* Gain: '<S58>/Gain' */
    rtb_Subtract1_c = -0.5F * rtb_Gain1_n;
    rtb_Switch_co[0] = rtb_Subtract1_c;
    rtb_Switch_co[1] = rtb_Subtract1_c;
    rtb_Switch_co[2] = rtb_Subtract1_c;
  } else {
    /* Outputs for Atomic SubSystem: '<S58>/dq-to-alphabeta' */
    emDriveFOC_co_dqtoalphabeta(rtb_Switch_gc, rtb_Theta, rtb_Product,
      rtb_Product2_k, &rtb_Subtract1_c, &rtb_Gain_a);

    /* End of Outputs for SubSystem: '<S58>/dq-to-alphabeta' */

    /* Outputs for Atomic SubSystem: '<S58>/alphabeta-to-abc' */
    /* DotProduct: '<S107>/Dot Product3' incorporates:
     *  Constant: '<S107>/Constant6'
     *  SignalConversion generated from: '<S107>/Dot Product2'
     */
    rtb_FeedFW = rtb_Subtract1_c * -0.5F + rtb_Gain_a * 0.866025388F;

    /* DotProduct: '<S107>/Dot Product4' incorporates:
     *  Constant: '<S107>/Constant7'
     *  SignalConversion generated from: '<S107>/Dot Product2'
     */
    rtb_Product1_c = rtb_Subtract1_c * -0.5F + rtb_Gain_a * -0.866025388F;

    /* Outputs for Atomic SubSystem: '<S58>/SVPWM' */
    /* Gain: '<S103>/Gain' incorporates:
     *  DotProduct: '<S107>/Dot Product3'
     *  DotProduct: '<S107>/Dot Product4'
     *  MinMax: '<S103>/Max2'
     *  MinMax: '<S103>/Max3'
     *  SignalConversion generated from: '<S107>/Dot Product2'
     *  Sum: '<S103>/Add'
     */
    rtb_Gain_a = (fmaxf(fmaxf(rtb_Subtract1_c, rtb_FeedFW), rtb_Product1_c) +
                  fminf(fminf(rtb_Subtract1_c, rtb_FeedFW), rtb_Product1_c)) *
      0.5F;
    rtb_Switch_co[0] = rtb_Subtract1_c - rtb_Gain_a;
    rtb_Switch_co[1] = rtb_FeedFW - rtb_Gain_a;
    rtb_Switch_co[2] = rtb_Product1_c - rtb_Gain_a;

    /* End of Outputs for SubSystem: '<S58>/SVPWM' */
    /* End of Outputs for SubSystem: '<S58>/alphabeta-to-abc' */
  }

  /* End of Switch: '<S58>/Switch' */

  /* Product: '<S104>/Product' */
  rtb_FeedFW = rtb_Switch2_g0 * emDriveFOC_control_DW.MinOffTimeHighSideF;

  /* Product: '<S104>/Product2' */
  rtb_Subtract1_c = rtb_Switch2_g0 *
    emDriveFOC_control_DW.Ctrl_Mod__MinOffTimeSecondHighS;

  /* Product: '<S104>/Product1' */
  rtb_Product1_c = rtb_Switch2_g0 * emDriveFOC_control_DW.MinOffTimeLowSideF;

  /* MATLAB Function: '<S104>/Saturate' */
  /* MATLAB Function 'Current_loop/FOC/ReverseTransform1/Saturation/Saturate': '<S112>:1' */
  /* '<S112>:1:2' y=coder.nullcopy(zeros(3,1,'single')); */
  /* '<S112>:1:4' if max2<max */
  if (rtb_Subtract1_c < rtb_FeedFW) {
    /*  we must do max2 saturation */
    /* find max index */
    /* '<S112>:1:6' max_val=u(1); */
    rtb_Gain_a = rtb_Switch_co[0];

    /* '<S112>:1:7' max_ind=1; */
    max_ind = 1;

    /* '<S112>:1:8' for i=1:3 */
    /* '<S112>:1:9' if  (u(i)>max_val) */
    if (rtb_Switch_co[1] > rtb_Switch_co[0]) {
      /* '<S112>:1:10' max_val = u(i); */
      rtb_Gain_a = rtb_Switch_co[1];

      /* '<S112>:1:11' max_ind = i; */
      max_ind = 2;
    }

    /* '<S112>:1:9' if  (u(i)>max_val) */
    if (rtb_Switch_co[2] > rtb_Gain_a) {
      /* '<S112>:1:10' max_val = u(i); */
      /* '<S112>:1:11' max_ind = i; */
      max_ind = 3;
    }

    /* now find second largest index */
    /* '<S112>:1:15' switch max_ind */
    switch (max_ind) {
     case 1:
      /* '<S112>:1:16' case 1 */
      /* '<S112>:1:17' if u(2)>u(3) */
      if (rtb_Switch_co[1] > rtb_Switch_co[2]) {
        /* '<S112>:1:18' max2_ind = 2; */
        max_ind = 1;
      } else {
        /* '<S112>:1:19' else */
        /* '<S112>:1:20' max2_ind = 3; */
        max_ind = 2;
      }
      break;

     case 2:
      /* '<S112>:1:22' case 2 */
      /* '<S112>:1:23' if u(3)>u(1) */
      if (rtb_Switch_co[2] > rtb_Switch_co[0]) {
        /* '<S112>:1:24' max2_ind = 3; */
        max_ind = 2;
      } else {
        /* '<S112>:1:25' else */
        /* '<S112>:1:26' max2_ind = 1; */
        max_ind = 0;
      }
      break;

     default:
      /* '<S112>:1:28' otherwise */
      /*  index 3 */
      /* '<S112>:1:29' if u(1)>u(2) */
      if (rtb_Switch_co[0] > rtb_Switch_co[1]) {
        /* '<S112>:1:30' max2_ind = 1; */
        max_ind = 0;
      } else {
        /* '<S112>:1:31' else */
        /* '<S112>:1:32' max2_ind = 2; */
        max_ind = 1;
      }
      break;
    }

    /* saturate to max2 */
    /* '<S112>:1:37' if (u(max2_ind)>max2) */
    if (rtb_Switch_co[max_ind] > rtb_Subtract1_c) {
      /* '<S112>:1:38' u(max2_ind)=max2; */
      rtb_Switch_co[max_ind] = rtb_Subtract1_c;
    }
  }

  /* Product: '<S104>/Product7' */
  /* '<S112>:1:42' sat=false; */
  /*  saturate to max and min  */
  /* '<S112>:1:44' for i=1:3 */
  rtb_Gain_a = rtb_Switch2_g0 * emDriveFOC_control_DW.MinPulseTimeF;

  /* Switch: '<S104>/Switch2' */
  /* MATLAB Function 'Current_loop/FOC/ReverseTransform1/Saturation/MinPulseTime': '<S111>:1' */
  /* '<S111>:1:2' y=coder.nullcopy(zeros(3,1,'single')); */
  /* '<S111>:1:3' for i=1:3 */
  rtb_LogicalOperator1_k_tmp = (rtb_Gain_a > 0.0F);

  /* MATLAB Function: '<S104>/Saturate' */
  /* '<S112>:1:45' if (u(i)>max) */
  if (rtb_Switch_co[0] > rtb_FeedFW) {
    /* '<S112>:1:46' y(i)=max; */
    rtb_y_jh_0 = rtb_FeedFW;

    /* '<S112>:1:47' sat=true; */
  } else if (rtb_Switch_co[0] < rtb_Product1_c) {
    /* '<S112>:1:48' elseif (u(i)< min) */
    /* '<S112>:1:49' y(i)= min; */
    rtb_y_jh_0 = rtb_Product1_c;

    /* '<S112>:1:50' sat=true; */
  } else {
    /* '<S112>:1:51' else */
    /* '<S112>:1:52' y(i)=u(i); */
    rtb_y_jh_0 = rtb_Switch_co[0];
  }

  /* MATLAB Function: '<S104>/MinPulseTime' */
  /* '<S111>:1:4' if u(i)>(lim-dead) */
  rtb_Subtract1_c = rtb_Switch2_g0 - rtb_Gain_a;
  if (rtb_y_jh_0 > rtb_Subtract1_c) {
    /* we are in upper dead zone */
    /* '<S111>:1:5' if u(i)>(lim-dead*0.5) */
    if (rtb_y_jh_0 > rtb_Switch2_g0 - rtb_Gain_a * 0.5F) {
      /* '<S111>:1:6' y(i)=lim; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Switch2_g0;
    } else {
      /* '<S111>:1:7' else */
      /* '<S111>:1:8' y(i)=lim-dead; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Subtract1_c;
    }
  } else {
    Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0 + rtb_Gain_a;
    if (rtb_y_jh_0 < Ctrl_CurrentReg_StatusUabc_1) {
      /* '<S111>:1:10' elseif u(i)< (-lim+dead) */
      /* we are in lower dead zone */
      /* '<S111>:1:11' if u(i)<(-lim+dead*0.5) */
      if (rtb_y_jh_0 < rtb_Gain_a * 0.5F - rtb_Switch2_g0) {
        /* '<S111>:1:12' y(i)=-lim; */
        Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0;
      } else {
        /* '<S111>:1:13' else */
        /* '<S111>:1:14' y(i)=-lim+dead; */
      }
    } else {
      /* '<S111>:1:16' else */
      /* '<S111>:1:17' y(i)=u(i); */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
    }
  }

  /* Switch: '<S104>/Switch2' */
  if (!rtb_LogicalOperator1_k_tmp) {
    Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
  }

  Ctrl_CurrentReg_StatusUabc[0] = Ctrl_CurrentReg_StatusUabc_1;

  /* MATLAB Function: '<S104>/Saturate' */
  /* '<S112>:1:45' if (u(i)>max) */
  if (rtb_Switch_co[1] > rtb_FeedFW) {
    /* '<S112>:1:46' y(i)=max; */
    rtb_y_jh_0 = rtb_FeedFW;

    /* '<S112>:1:47' sat=true; */
  } else if (rtb_Switch_co[1] < rtb_Product1_c) {
    /* '<S112>:1:48' elseif (u(i)< min) */
    /* '<S112>:1:49' y(i)= min; */
    rtb_y_jh_0 = rtb_Product1_c;

    /* '<S112>:1:50' sat=true; */
  } else {
    /* '<S112>:1:51' else */
    /* '<S112>:1:52' y(i)=u(i); */
    rtb_y_jh_0 = rtb_Switch_co[1];
  }

  /* MATLAB Function: '<S104>/MinPulseTime' */
  /* '<S111>:1:4' if u(i)>(lim-dead) */
  if (rtb_y_jh_0 > rtb_Subtract1_c) {
    /* we are in upper dead zone */
    /* '<S111>:1:5' if u(i)>(lim-dead*0.5) */
    if (rtb_y_jh_0 > rtb_Switch2_g0 - rtb_Gain_a * 0.5F) {
      /* '<S111>:1:6' y(i)=lim; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Switch2_g0;
    } else {
      /* '<S111>:1:7' else */
      /* '<S111>:1:8' y(i)=lim-dead; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Subtract1_c;
    }
  } else {
    Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0 + rtb_Gain_a;
    if (rtb_y_jh_0 < Ctrl_CurrentReg_StatusUabc_1) {
      /* '<S111>:1:10' elseif u(i)< (-lim+dead) */
      /* we are in lower dead zone */
      /* '<S111>:1:11' if u(i)<(-lim+dead*0.5) */
      if (rtb_y_jh_0 < rtb_Gain_a * 0.5F - rtb_Switch2_g0) {
        /* '<S111>:1:12' y(i)=-lim; */
        Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0;
      } else {
        /* '<S111>:1:13' else */
        /* '<S111>:1:14' y(i)=-lim+dead; */
      }
    } else {
      /* '<S111>:1:16' else */
      /* '<S111>:1:17' y(i)=u(i); */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
    }
  }

  /* Switch: '<S104>/Switch2' */
  if (!rtb_LogicalOperator1_k_tmp) {
    Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
  }

  Ctrl_CurrentReg_StatusUabc[1] = Ctrl_CurrentReg_StatusUabc_1;

  /* MATLAB Function: '<S104>/Saturate' */
  /* '<S112>:1:45' if (u(i)>max) */
  if (rtb_Switch_co[2] > rtb_FeedFW) {
    /* '<S112>:1:46' y(i)=max; */
    rtb_y_jh_0 = rtb_FeedFW;

    /* '<S112>:1:47' sat=true; */
  } else if (rtb_Switch_co[2] < rtb_Product1_c) {
    /* '<S112>:1:48' elseif (u(i)< min) */
    /* '<S112>:1:49' y(i)= min; */
    rtb_y_jh_0 = rtb_Product1_c;

    /* '<S112>:1:50' sat=true; */
  } else {
    /* '<S112>:1:51' else */
    /* '<S112>:1:52' y(i)=u(i); */
    rtb_y_jh_0 = rtb_Switch_co[2];
  }

  /* MATLAB Function: '<S104>/MinPulseTime' */
  /* '<S111>:1:4' if u(i)>(lim-dead) */
  if (rtb_y_jh_0 > rtb_Subtract1_c) {
    /* we are in upper dead zone */
    /* '<S111>:1:5' if u(i)>(lim-dead*0.5) */
    if (rtb_y_jh_0 > rtb_Switch2_g0 - rtb_Gain_a * 0.5F) {
      /* '<S111>:1:6' y(i)=lim; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Switch2_g0;
    } else {
      /* '<S111>:1:7' else */
      /* '<S111>:1:8' y(i)=lim-dead; */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_Subtract1_c;
    }
  } else {
    Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0 + rtb_Gain_a;
    if (rtb_y_jh_0 < Ctrl_CurrentReg_StatusUabc_1) {
      /* '<S111>:1:10' elseif u(i)< (-lim+dead) */
      /* we are in lower dead zone */
      /* '<S111>:1:11' if u(i)<(-lim+dead*0.5) */
      if (rtb_y_jh_0 < rtb_Gain_a * 0.5F - rtb_Switch2_g0) {
        /* '<S111>:1:12' y(i)=-lim; */
        Ctrl_CurrentReg_StatusUabc_1 = -rtb_Switch2_g0;
      } else {
        /* '<S111>:1:13' else */
        /* '<S111>:1:14' y(i)=-lim+dead; */
      }
    } else {
      /* '<S111>:1:16' else */
      /* '<S111>:1:17' y(i)=u(i); */
      Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
    }
  }

  /* Switch: '<S104>/Switch2' */
  if (!rtb_LogicalOperator1_k_tmp) {
    Ctrl_CurrentReg_StatusUabc_1 = rtb_y_jh_0;
  }

  Ctrl_CurrentReg_StatusUabc[2] = Ctrl_CurrentReg_StatusUabc_1;

  /* Outputs for Atomic SubSystem: '<S58>/abc-to-alphabeta1' */
  /* Gain: '<S106>/Gain1' incorporates:
   *  Sum: '<S106>/Add'
   */
  rtb_Product1_c = ((Ctrl_CurrentReg_StatusUabc[0] + Ctrl_CurrentReg_StatusUabc
                     [1]) + Ctrl_CurrentReg_StatusUabc[2]) * 0.333333343F;

  /* Sum: '<S106>/Add1' */
  rtb_Subtract1_c = Ctrl_CurrentReg_StatusUabc[1] - rtb_Product1_c;

  /* DotProduct: '<S106>/Dot Product' incorporates:
   *  Constant: '<S106>/Constant2'
   *  Sum: '<S106>/Add1'
   */
  rtb_Switch2_g0 = (Ctrl_CurrentReg_StatusUabc[0] - rtb_Product1_c) *
    0.666666687F + rtb_Subtract1_c * -0.333333343F;

  /* DotProduct: '<S106>/Dot Product1' incorporates:
   *  Constant: '<S106>/Constant3'
   */
  rtb_FeedFW = rtb_Subtract1_c * 0.577350259F;

  /* Sum: '<S106>/Add1' */
  rtb_Subtract1_c = Ctrl_CurrentReg_StatusUabc[2] - rtb_Product1_c;

  /* DotProduct: '<S106>/Dot Product' incorporates:
   *  Constant: '<S106>/Constant2'
   */
  rtb_Switch2_g0 += rtb_Subtract1_c * -0.333333343F;

  /* DotProduct: '<S106>/Dot Product1' incorporates:
   *  Constant: '<S106>/Constant3'
   */
  rtb_Subtract1_c = rtb_Subtract1_c * -0.577350259F + rtb_FeedFW;

  /* End of Outputs for SubSystem: '<S58>/abc-to-alphabeta1' */
  /* End of Outputs for SubSystem: '<S38>/ReverseTransform1' */

  /* Switch: '<S55>/Switch1' incorporates:
   *  Constant: '<S55>/Constant'
   *  Constant: '<S99>/Constant6'
   *  Constant: '<S99>/Constant7'
   *  DotProduct: '<S106>/Dot Product'
   *  DotProduct: '<S106>/Dot Product1'
   *  DotProduct: '<S99>/Dot Product3'
   *  DotProduct: '<S99>/Dot Product4'
   *  Inport: '<Root>/Control'
   *  Product: '<S55>/Product'
   *  Signum: '<S55>/Sign'
   *  Sum: '<S55>/Add'
   */
  if (emDriveFOC_control_U.Control.AscActive) {
    /* Outputs for Atomic SubSystem: '<S38>/ReverseTransform1' */
    /* Outputs for Atomic SubSystem: '<S58>/abc-to-alphabeta1' */
    rtb_Switch_co[0] = rtb_Switch2_g0;

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc1' */
    rtb_Switch_co[1] = rtb_Switch2_g0 * -0.5F + rtb_Subtract1_c * 0.866025388F;
    rtb_Switch_co[2] = rtb_Switch2_g0 * -0.5F + rtb_Subtract1_c * -0.866025388F;

    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc1' */
    /* End of Outputs for SubSystem: '<S58>/abc-to-alphabeta1' */
    /* End of Outputs for SubSystem: '<S38>/ReverseTransform1' */
  } else {
    /* Outputs for Atomic SubSystem: '<S38>/dq-to-alphabeta' */
    emDriveFOC_co_dqtoalphabeta(temp, rtb_MaxampforSVPWM,
      rtb_TrigonometricFunction_o2_c, rtb_Subtract1_f, &rtb_Gain_a,
      &rtb_Product1_c);

    /* End of Outputs for SubSystem: '<S38>/dq-to-alphabeta' */

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc' */
    /* DotProduct: '<S98>/Dot Product3' incorporates:
     *  Constant: '<S98>/Constant6'
     *  SignalConversion generated from: '<S98>/Dot Product2'
     */
    rtb_FeedFW = rtb_Gain_a * -0.5F + rtb_Product1_c * 0.866025388F;

    /* DotProduct: '<S98>/Dot Product4' incorporates:
     *  Constant: '<S98>/Constant7'
     *  SignalConversion generated from: '<S98>/Dot Product2'
     */
    rtb_Product1_c = rtb_Gain_a * -0.5F + rtb_Product1_c * -0.866025388F;

    /* Signum: '<S55>/Sign' incorporates:
     *  SignalConversion generated from: '<S98>/Dot Product2'
     */
    if (rtb_Gain_a < 0.0F) {
      max_ind = -1;
    } else {
      max_ind = (rtb_Gain_a > 0.0F);
    }

    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc' */

    /* Outputs for Atomic SubSystem: '<S38>/ReverseTransform1' */
    /* Outputs for Atomic SubSystem: '<S58>/abc-to-alphabeta1' */
    rtb_Switch_co[0] = rtb_Switch2_g0 - (real32_T)max_ind * rtb_Gain1_n *
      Ctrl_Mod__DeadTimeComp;

    /* End of Outputs for SubSystem: '<S58>/abc-to-alphabeta1' */
    /* End of Outputs for SubSystem: '<S38>/ReverseTransform1' */

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc' */
    /* Signum: '<S55>/Sign' incorporates:
     *  Constant: '<S55>/Constant'
     *  DotProduct: '<S106>/Dot Product'
     *  DotProduct: '<S98>/Dot Product3'
     *  Product: '<S55>/Product'
     *  Sum: '<S55>/Add'
     */
    if (rtb_FeedFW < 0.0F) {
      max_ind = -1;
    } else {
      max_ind = (rtb_FeedFW > 0.0F);
    }

    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc' */

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc1' */
    /* Outputs for Atomic SubSystem: '<S38>/ReverseTransform1' */
    /* Outputs for Atomic SubSystem: '<S58>/abc-to-alphabeta1' */
    rtb_Switch_co[1] = (rtb_Switch2_g0 * -0.5F + rtb_Subtract1_c * 0.866025388F)
      - (real32_T)max_ind * rtb_Gain1_n * Ctrl_Mod__DeadTimeComp;

    /* End of Outputs for SubSystem: '<S58>/abc-to-alphabeta1' */
    /* End of Outputs for SubSystem: '<S38>/ReverseTransform1' */
    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc1' */

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc' */
    /* Signum: '<S55>/Sign' incorporates:
     *  Constant: '<S55>/Constant'
     *  Constant: '<S99>/Constant6'
     *  DotProduct: '<S106>/Dot Product'
     *  DotProduct: '<S106>/Dot Product1'
     *  DotProduct: '<S98>/Dot Product4'
     *  DotProduct: '<S99>/Dot Product3'
     *  Product: '<S55>/Product'
     *  Sum: '<S55>/Add'
     */
    if (rtb_Product1_c < 0.0F) {
      max_ind = -1;
    } else {
      max_ind = (rtb_Product1_c > 0.0F);
    }

    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc' */

    /* Outputs for Atomic SubSystem: '<S55>/alphabeta-to-abc1' */
    /* Outputs for Atomic SubSystem: '<S38>/ReverseTransform1' */
    /* Outputs for Atomic SubSystem: '<S58>/abc-to-alphabeta1' */
    rtb_Switch_co[2] = (rtb_Switch2_g0 * -0.5F + rtb_Subtract1_c * -0.866025388F)
      - (real32_T)max_ind * rtb_Gain1_n * Ctrl_Mod__DeadTimeComp;

    /* End of Outputs for SubSystem: '<S58>/abc-to-alphabeta1' */
    /* End of Outputs for SubSystem: '<S38>/ReverseTransform1' */
    /* End of Outputs for SubSystem: '<S55>/alphabeta-to-abc1' */
  }

  /* End of Switch: '<S55>/Switch1' */

  /* Outputs for Atomic SubSystem: '<S55>/abc-to-alphabeta2' */
  /* Gain: '<S97>/Gain1' incorporates:
   *  Sum: '<S97>/Add'
   */
  temp = ((rtb_Switch_co[0] + rtb_Switch_co[1]) + rtb_Switch_co[2]) *
    0.333333343F;

  /* End of Outputs for SubSystem: '<S55>/abc-to-alphabeta2' */

  /* SignalConversion: '<S59>/Signal Conversion' incorporates:
   *  Gain: '<S38>/ToDeg'
   */
  Ctrl_CurrentReg_StatusTheta = 57.2957802F * emDriveFOC_control_DW.theta_r_d;

  /* SignalConversion: '<S60>/Signal Conversion' incorporates:
   *  DataStoreRead: '<S38>/Data Store Read3'
   *  Product: '<S38>/Product1'
   */
  Ctrl_CurrentReg_StatusRpm = emDriveFOC_control_DW.omega_r_l *
    emDriveFOC_control_DW.OmegaEtoRPM_k;

  /* SignalConversion generated from: '<S38>/Data Type Conversion1' */
  emDriveFOC_control_DW.omega_r_b = emDriveFOC_control_DW.omega_r_l;

  /* SignalConversion generated from: '<S38>/Data Type Conversion2' */
  emDriveFOC_control_DW.State_p = emDriveFOC_control_DW.State;

  /* SignalConversion generated from: '<S38>/Data Type Conversion3' */
  emDriveFOC_control_DW.RPM_g = emDriveFOC_control_DW.RPM_h;

  /* Update for UnitDelay: '<S38>/Unit Delay1' */
  emDriveFOC_control_DW.UnitDelay1_DSTATE_m[0] = rtb_Switch_gc;
  emDriveFOC_control_DW.UnitDelay1_DSTATE_m[1] = rtb_Theta;

  /* End of Outputs for SubSystem: '<S2>/FOC' */

  /* Gain: '<S44>/Gain' */
  rtb_Switch_gc = Ctrl_Mod__PWMOffset * rtb_Gain1_n;

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* Logic: '<S13>/Logical Operator' incorporates:
   *  Constant: '<S299>/Constant'
   *  Inport: '<Root>/Control'
   *  Logic: '<S13>/Logical Operator1'
   *  RelationalOperator: '<S13>/Relational Operator'
   */
  Ctrl_Gen_Stat__RunStopOut = (emDriveFOC_control_U.Control.RunStop &&
    (emDriveFOC_control_DW.State_p != enumSenzorlessState_Error) &&
    (!emDriveFOC_control_DW.RelationalOperator));

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Sum: '<S44>/Add' incorporates:
   *  SignalConversion: '<S105>/Signal Conversion'
   */
  Ctrl_CurrentReg_StatusUabc_0 = Ctrl_CurrentReg_StatusUabc[0];

  /* SignalConversion: '<S42>/Signal Conversion' incorporates:
   *  Product: '<S44>/Divide'
   *  SignalConversion: '<S105>/Signal Conversion'
   *  Sum: '<S44>/Add'
   */
  Ctrl_Mod_StatusPWM_0 = (Ctrl_CurrentReg_StatusUabc_0 + rtb_Switch_gc) /
    rtb_Gain1_n;

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  Ctrl_Mod_StatusPWM[0] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* SignalConversion: '<S43>/Signal Conversion' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   */
  Ctrl_CurrentReg_StatusIabc[0] = emDriveFOC_control_U.SensorsTsc.Is_abc[0];

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* BusCreator: '<Root>/Bus Creator1' incorporates:
   *  Outport: '<Root>/ActuatorsTsc'
   */
  emDriveFOC_control_Y.ActuatorsTsc.Us_abc[0] = Ctrl_CurrentReg_StatusUabc_0;
  emDriveFOC_control_Y.ActuatorsTsc.PWM[0] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S55>/abc-to-alphabeta2' */
  /* Sum: '<S97>/Add1' */
  rtb_Subtract1_c = rtb_Switch_co[1] - temp;

  /* DotProduct: '<S97>/Dot Product' incorporates:
   *  Constant: '<S97>/Constant2'
   *  Sum: '<S97>/Add1'
   */
  rtb_Switch2_g0 = (rtb_Switch_co[0] - temp) * 0.666666687F + rtb_Subtract1_c *
    -0.333333343F;

  /* DotProduct: '<S97>/Dot Product1' incorporates:
   *  Constant: '<S97>/Constant3'
   */
  rtb_FeedFW = rtb_Subtract1_c * 0.577350259F;

  /* End of Outputs for SubSystem: '<S55>/abc-to-alphabeta2' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */

  /* Sum: '<S44>/Add' incorporates:
   *  SignalConversion: '<S105>/Signal Conversion'
   */
  Ctrl_CurrentReg_StatusUabc_0 = Ctrl_CurrentReg_StatusUabc[1];

  /* SignalConversion: '<S42>/Signal Conversion' incorporates:
   *  Product: '<S44>/Divide'
   *  SignalConversion: '<S105>/Signal Conversion'
   *  Sum: '<S44>/Add'
   */
  Ctrl_Mod_StatusPWM_0 = (Ctrl_CurrentReg_StatusUabc_0 + rtb_Switch_gc) /
    rtb_Gain1_n;

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  Ctrl_Mod_StatusPWM[1] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* SignalConversion: '<S43>/Signal Conversion' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   */
  Ctrl_CurrentReg_StatusIabc[1] = emDriveFOC_control_U.SensorsTsc.Is_abc[1];

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* BusCreator: '<Root>/Bus Creator1' incorporates:
   *  Outport: '<Root>/ActuatorsTsc'
   */
  emDriveFOC_control_Y.ActuatorsTsc.Us_abc[1] = Ctrl_CurrentReg_StatusUabc_0;
  emDriveFOC_control_Y.ActuatorsTsc.PWM[1] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S55>/abc-to-alphabeta2' */
  /* Sum: '<S97>/Add1' */
  rtb_Subtract1_c = rtb_Switch_co[2] - temp;

  /* DotProduct: '<S97>/Dot Product' incorporates:
   *  Constant: '<S97>/Constant2'
   */
  rtb_Switch2_g0 += rtb_Subtract1_c * -0.333333343F;

  /* DotProduct: '<S97>/Dot Product1' incorporates:
   *  Constant: '<S97>/Constant3'
   */
  rtb_Subtract1_c = rtb_Subtract1_c * -0.577350259F + rtb_FeedFW;

  /* End of Outputs for SubSystem: '<S55>/abc-to-alphabeta2' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */

  /* Sum: '<S44>/Add' incorporates:
   *  SignalConversion: '<S105>/Signal Conversion'
   */
  Ctrl_CurrentReg_StatusUabc_0 = Ctrl_CurrentReg_StatusUabc[2];

  /* SignalConversion: '<S42>/Signal Conversion' incorporates:
   *  Product: '<S44>/Divide'
   *  SignalConversion: '<S105>/Signal Conversion'
   *  Sum: '<S44>/Add'
   */
  Ctrl_Mod_StatusPWM_0 = (Ctrl_CurrentReg_StatusUabc_0 + rtb_Switch_gc) /
    rtb_Gain1_n;

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  Ctrl_Mod_StatusPWM[2] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* SignalConversion: '<S43>/Signal Conversion' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   *  SignalConversion generated from: '<S2>/Bus Selector1'
   */
  Ctrl_CurrentReg_StatusIabc[2] = emDriveFOC_control_U.SensorsTsc.Is_abc[2];

  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* BusCreator: '<Root>/Bus Creator1' incorporates:
   *  Outport: '<Root>/ActuatorsTsc'
   */
  emDriveFOC_control_Y.ActuatorsTsc.Us_abc[2] = Ctrl_CurrentReg_StatusUabc_0;
  emDriveFOC_control_Y.ActuatorsTsc.PWM[2] = Ctrl_Mod_StatusPWM_0;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S55>/abc-to-alphabeta2' */
  /* Update for UnitDelay: '<S2>/Unit Delay1' incorporates:
   *  DotProduct: '<S97>/Dot Product'
   *  DotProduct: '<S97>/Dot Product1'
   */
  emDriveFOC_control_DW.UnitDelay1_DSTATE_n[0] = rtb_Switch2_g0;
  emDriveFOC_control_DW.UnitDelay1_DSTATE_n[1] = rtb_Subtract1_c;

  /* End of Outputs for SubSystem: '<S55>/abc-to-alphabeta2' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* BusCreator: '<Root>/Bus Creator1' incorporates:
   *  Outport: '<Root>/ActuatorsTsc'
   */
  emDriveFOC_control_Y.ActuatorsTsc.RunStop = Ctrl_Gen_Stat__RunStopOut;

  /* DataTypeConversion: '<S5>/Data Type Conversion' */
  emDriveFOC_control_Y.DataTsc.SenzorlessState = (int32_T)
    emDriveFOC_control_DW.State_p;

  /* BusCreator: '<S5>/Bus Creator' incorporates:
   *  Outport: '<Root>/DataTsc'
   */
  emDriveFOC_control_Y.DataTsc.Uph = emDriveFOC_control_DW.Uph;
  emDriveFOC_control_Y.DataTsc.Iph = emDriveFOC_control_DW.Iph;
  emDriveFOC_control_Y.DataTsc.Pel = emDriveFOC_control_DW.Switch1_h;
  emDriveFOC_control_Y.DataTsc.StatusLim = emDriveFOC_control_DW.Min;

  /* SignalConversion: '<S221>/Signal Conversion' */
  Ctrl_Gen_Stat__Iq_foc = emDriveFOC_control_DW.Switch2_h;

  /* SignalConversion: '<S222>/Signal Conversion' incorporates:
   *  Inport: '<Root>/SensorsTsc'
   */
  Ctrl_Gen_Stat__Udc = emDriveFOC_control_U.SensorsTsc.Udc;

  /* Outputs for Atomic SubSystem: '<S5>/LPF_Measurment' */
  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S55>/abc-to-alphabeta2' */
  /* SignalConversion generated from: '<S267>/Delay' incorporates:
   *  DotProduct: '<S97>/Dot Product'
   *  DotProduct: '<S97>/Dot Product1'
   *  Product: '<S114>/AxBx'
   *  Product: '<S114>/AxBy'
   *  Product: '<S114>/AyBx'
   *  Product: '<S114>/AyBy'
   *  Sum: '<S114>/sumx'
   *  Sum: '<S114>/sumy'
   */
  emDriveFOC_control_DW.Switch1_i[0] = rtb_Switch2_g0 * rtb_Product +
    rtb_Subtract1_c * rtb_Product2_k;
  emDriveFOC_control_DW.Switch1_i[1] = rtb_Subtract1_c * rtb_Product -
    rtb_Switch2_g0 * rtb_Product2_k;

  /* End of Outputs for SubSystem: '<S55>/abc-to-alphabeta2' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  emDriveFOC_control_DW.Switch1_i[2] = rtb_Switch_dr;
  emDriveFOC_control_DW.Switch1_i[3] = UnitDelay5_DSTATE;
  emDriveFOC_control_DW.Switch1_i[4] = emDriveFOC_control_DW.omega_r_b;

  /* Switch: '<S267>/Switch1' */
  rtb_LogicalOperator1_k_tmp = (emDriveFOC_control_DW.alpha_c != 0.0F);
  for (max_ind = 0; max_ind < 5; max_ind++) {
    /* Delay: '<S267>/Delay' incorporates:
     *  SignalConversion generated from: '<S267>/Delay'
     *  Sum: '<S267>/Add1'
     *  Switch: '<S267>/Switch'
     */
    if (emDriveFOC_control_DW.icLoad_jg) {
      emDriveFOC_control_DW.Delay_DSTATE_du[max_ind] =
        emDriveFOC_control_DW.Switch1_i[max_ind];
    }

    rtb_Gain1_n = emDriveFOC_control_DW.Delay_DSTATE_du[max_ind];

    /* Sum: '<S267>/Add1' incorporates:
     *  SignalConversion generated from: '<S267>/Delay'
     *  Switch: '<S267>/Switch'
     */
    rtb_Switch_dr = emDriveFOC_control_DW.Switch1_i[max_ind];

    /* Switch: '<S267>/Switch' incorporates:
     *  Delay: '<S267>/Delay'
     *  Product: '<S267>/Product'
     *  SignalConversion generated from: '<S267>/Delay'
     *  Sum: '<S267>/Add'
     *  Sum: '<S267>/Add1'
     */
    rtb_Gain1_n += (rtb_Switch_dr - rtb_Gain1_n) * emDriveFOC_control_DW.alpha_c;
    emDriveFOC_control_DW.Delay_DSTATE_du[max_ind] = rtb_Gain1_n;

    /* Switch: '<S267>/Switch1' incorporates:
     *  SignalConversion generated from: '<S267>/Delay'
     *  Switch: '<S267>/Switch'
     */
    if (rtb_LogicalOperator1_k_tmp) {
      rtb_Switch_dr = rtb_Gain1_n;
    }

    emDriveFOC_control_DW.Switch1_i[max_ind] = rtb_Switch_dr;
  }

  /* Update for Delay: '<S267>/Delay' */
  emDriveFOC_control_DW.icLoad_jg = false;

  /* End of Outputs for SubSystem: '<S5>/LPF_Measurment' */

  /* SignalConversion: '<S10>/Signal Conversion' incorporates:
   *  Inport: '<Root>/Control'
   */
  Ctrl_Gen_Stat__RunStopIn = emDriveFOC_control_U.Control.RunStop;

  /* Update for Atomic SubSystem: '<S3>/Overvoltage' */
  /* Update for UnitDelay: '<S220>/Unit Delay' */
  UnitDelay_DSTATE_o = emDriveFOC_control_DW.omega_r_b;

  /* End of Update for SubSystem: '<S3>/Overvoltage' */

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/AngleGen' */
  /* Update for Delay: '<S47>/Delay' */
  Delay_DSTATE_d3 = rtb_TreqVel;

  /* End of Outputs for SubSystem: '<S2>/AngleGen' */

  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* Outputs for Atomic SubSystem: '<S54>/ PI regulator Q' */
  /* Update for Delay: '<S84>/Delay' */
  Delay_DSTATE_e = rtb_PFF_f;

  /* End of Outputs for SubSystem: '<S54>/ PI regulator Q' */
  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* Switch: '<S71>/Switch' */
  emDriveFOC_control_DW.Delay_DSTATE_hi = Delay_DSTATE_hi;

  /* Switch: '<S47>/Switch' */
  emDriveFOC_control_DW.Delay_DSTATE_d3 = Delay_DSTATE_d3;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* Outputs for Atomic SubSystem: '<S54>/ PI regulator D' */
  /* Switch: '<S83>/Switch3' incorporates:
   *  Switch: '<S83>/Switch2'
   */
  emDriveFOC_control_DW.Delay_DSTATE_g = Delay_DSTATE_g;

  /* End of Outputs for SubSystem: '<S54>/ PI regulator D' */
  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* Outputs for Atomic SubSystem: '<S52>/FieldWeakening' */
  /* Outputs for Atomic SubSystem: '<S65>/ Field weakening PI' */
  /* Switch: '<S70>/Switch3' incorporates:
   *  Switch: '<S70>/Switch2'
   */
  emDriveFOC_control_DW.Delay_DSTATE_k = Delay_DSTATE_k;

  /* End of Outputs for SubSystem: '<S65>/ Field weakening PI' */
  /* End of Outputs for SubSystem: '<S52>/FieldWeakening' */
  /* End of Outputs for SubSystem: '<S38>/Calculate_Id' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
  /* UnaryMinus: '<S81>/Unary Minus' incorporates:
   *  Switch: '<S82>/Switch2'
   */
  emDriveFOC_control_DW.Q = Q;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  emDriveFOC_control_DW.UnitDelay_DSTATE_o = UnitDelay_DSTATE_o;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/FW transform' */
  /* Sum: '<S101>/sumy' incorporates:
   *  UnitDelay: '<S38>/Unit Delay5'
   */
  emDriveFOC_control_DW.UnitDelay5_DSTATE = UnitDelay5_DSTATE;

  /* End of Outputs for SubSystem: '<S38>/FW transform' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* RelationalOperator: '<S52>/Relational Operator' incorporates:
   *  UnitDelay: '<S53>/Unit Delay3'
   */
  emDriveFOC_control_DW.UnitDelay3_DSTATE_a = UnitDelay3_DSTATE_a;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Id' */

  /* Outputs for Atomic SubSystem: '<S38>/Calculate_Iq' */
  /* MATLAB Function: '<S53>/Pitagora_calc2' incorporates:
   *  UnitDelay: '<S38>/Unit Delay2'
   */
  emDriveFOC_control_DW.UnitDelay2_DSTATE = UnitDelay2_DSTATE;

  /* End of Outputs for SubSystem: '<S38>/Calculate_Iq' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  emDriveFOC_control_DW.Delay_DSTATE_e = Delay_DSTATE_e;
}

FOC_RESTORE_CODE
  /* Model step function for TID1 */
//  FOC_FAST_CODE
  void emDriveFOC_control_step1(void)  /* Sample time: [0.001s, 0.0s] */
{
  real32_T Stim[2];
  real32_T PrevY;
  real32_T UnitDelay_DSTATE_c;
  real32_T rtb_RateTransition1;
  UnitDelay_DSTATE_c = emDriveFOC_control_DW.UnitDelay_DSTATE_c;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/SensorlessTop' */
  /* Outputs for Atomic SubSystem: '<S41>/CalculateRefSign' */
  /* RateLimiter: '<S116>/Rate Limiter' */
  PrevY = emDriveFOC_control_DW.PrevY;

  /* End of Outputs for SubSystem: '<S41>/CalculateRefSign' */
  /* End of Outputs for SubSystem: '<S2>/SensorlessTop' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* Outputs for Atomic SubSystem: '<Root>/StimGenerator' */
  emDriveFOC_co_StimGenerator();

  /* End of Outputs for SubSystem: '<Root>/StimGenerator' */

  /* DataStoreWrite: '<Root>/Data Store Write' */
  Stim[0] = emDriveFOC_control_DW.RPM_act;
  Stim[1] = emDriveFOC_control_DW.Gain2;

  /* RateTransition: '<Root>/Rate Transition1' incorporates:
   *  Inport: '<Root>/Control'
   */
  rtb_RateTransition1 = emDriveFOC_control_U.Control.RPM_ref;

  /* Outputs for Atomic SubSystem: '<Root>/StimRPM' */
  /* Switch: '<S12>/Switch2' incorporates:
   *  Constant: '<Root>/Constant3'
   *  Constant: '<S12>/Constant2'
   *  Constant: '<S12>/Constant5'
   *  RelationalOperator: '<S12>/Relational Operator'
   *  RelationalOperator: '<S12>/Relational Operator1'
   *  Switch: '<S12>/Switch'
   */
  if (Ctrl_Stim2__GenTarget == 6) {
    /* SignalConversion: '<S7>/Signal Conversion' incorporates:
     *  DataStoreWrite: '<Root>/Data Store Write'
     */
    Ctrl_Gen_Stat__RPMref = emDriveFOC_control_DW.Gain2;
  } else if (Ctrl_Stim1__GenTarget == 6) {
    /* Switch: '<S12>/Switch' incorporates:
     *  DataStoreWrite: '<Root>/Data Store Write'
     *  SignalConversion: '<S7>/Signal Conversion'
     */
    Ctrl_Gen_Stat__RPMref = emDriveFOC_control_DW.RPM_act;
  } else {
    /* SignalConversion: '<S7>/Signal Conversion' incorporates:
     *  Switch: '<S12>/Switch'
     */
    Ctrl_Gen_Stat__RPMref = rtb_RateTransition1;
  }

  /* End of Switch: '<S12>/Switch2' */
  /* End of Outputs for SubSystem: '<Root>/StimRPM' */

  /* RateTransition: '<Root>/Rate Transition8' incorporates:
   *  Inport: '<Root>/Control'
   */
  emDriveFOC_control_DW.RateTransition8 = emDriveFOC_control_U.Control.RPM_ramp;

  /* RateTransition: '<Root>/Rate Transition6' incorporates:
   *  Inport: '<Root>/Control'
   */
  emDriveFOC_control_DW.RateTransition6 = emDriveFOC_control_U.Control.RunStop;

  /* RateTransition generated from: '<Root>/Rate Transition3' */
  emDriveFOC_control_DW.MTPV_active =
    emDriveFOC_control_DW.RelationalOperator_li;

  /* RateTransition generated from: '<Root>/Rate Transition3' */
  emDriveFOC_control_DW.RPM_Track = emDriveFOC_control_DW.RPM_Track_m;

  /* RateTransition generated from: '<Root>/Rate Transition3' */
  emDriveFOC_control_DW.RPM_ramp_track_value = emDriveFOC_control_DW.RPM_g;

  /* RateTransition generated from: '<S3>/Rate Transition7' */
  emDriveFOC_control_DW.pow_lim = emDriveFOC_control_DW.pow_lim_a;

  /* RateTransition generated from: '<S3>/Rate Transition7' */
  emDriveFOC_control_DW.Udc_lim = emDriveFOC_control_DW.Udc_lim_o;

  /* Outputs for Atomic SubSystem: '<Root>/RPM_ramp_and_limit' */
  emDriveF_RPM_ramp_and_limit();

  /* End of Outputs for SubSystem: '<Root>/RPM_ramp_and_limit' */

  /* RateTransition generated from: '<Root>/Rate Transition3' */
  emDriveFOC_control_DW.omega_r = emDriveFOC_control_DW.omega_r_b;

  /* RateTransition generated from: '<Root>/Rate Transition3' */
  emDriveFOC_control_DW.IqMax = emDriveFOC_control_DW.Q;

  /* RateTransition: '<Root>/Rate Transition13' incorporates:
   *  Inport: '<Root>/Control'
   */
  emDriveFOC_control_DW.RateTransition13 =
    emDriveFOC_control_U.Control.AscActive;

  /* Outputs for Atomic SubSystem: '<Root>/Velocity_loop' */
  emDriveFOC_co_Velocity_loop();

  /* End of Outputs for SubSystem: '<Root>/Velocity_loop' */

  /* Outputs for Atomic SubSystem: '<Root>/ConstInit' */
  emDriveFOC_contro_ConstInitTID1();

  /* End of Outputs for SubSystem: '<Root>/ConstInit' */

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/AngleGen' */
  /* Outputs for Atomic SubSystem: '<S37>/AngleGenConst' */
  emDriveFOC_co_AngleGenConst();

  /* End of Outputs for SubSystem: '<S37>/AngleGenConst' */
  /* End of Outputs for SubSystem: '<S2>/AngleGen' */

  /* Outputs for Atomic SubSystem: '<S2>/FOC' */
  /* Outputs for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* Outputs for Atomic SubSystem: '<S54>/StimUd' */
  emDriveFOC_control_StimUdTID1(Stim, &emDriveFOC_control_DW.StimUd);

  /* End of Outputs for SubSystem: '<S54>/StimUd' */

  /* Outputs for Atomic SubSystem: '<S54>/StrimUq' */
  emDriveFOC_control_StimUdTID1(Stim, &emDriveFOC_control_DW.StrimUq);

  /* End of Outputs for SubSystem: '<S54>/StrimUq' */
  /* End of Outputs for SubSystem: '<S38>/CurrentRegulators' */

  /* Outputs for Atomic SubSystem: '<S38>/StimId' */
  emDriveFOC_control_StimUdTID1(Stim, &emDriveFOC_control_DW.StimId);

  /* End of Outputs for SubSystem: '<S38>/StimId' */

  /* Outputs for Atomic SubSystem: '<S38>/StimIq' */
  emDriveFOC_control_StimUdTID1(Stim, &emDriveFOC_control_DW.StimIq);

  /* End of Outputs for SubSystem: '<S38>/StimIq' */
  /* End of Outputs for SubSystem: '<S2>/FOC' */

  /* Outputs for Atomic SubSystem: '<S2>/SensorlessTop' */
  /* Outputs for Atomic SubSystem: '<S41>/CalculateRefSign' */
  /* Switch: '<S116>/Switch1' incorporates:
   *  RelationalOperator: '<S116>/Relational Operator'
   */
  if (emDriveFOC_control_DW.RateTransition2 != 0.0F) {
    /* Signum: '<S116>/Sign' */
    if (emDriveFOC_control_DW.RateTransition2 < 0.0F) {
      UnitDelay_DSTATE_c = -1.0F;
    } else {
      UnitDelay_DSTATE_c = (real32_T)(emDriveFOC_control_DW.RateTransition2 >
        0.0F);
    }

    /* End of Signum: '<S116>/Sign' */
  }

  /* End of Switch: '<S116>/Switch1' */

  /* RateLimiter: '<S116>/Rate Limiter' */
  rtb_RateTransition1 = UnitDelay_DSTATE_c - PrevY;
  if (rtb_RateTransition1 > 0.02F) {
    emDriveFOC_control_DW.RateLimiter = PrevY + 0.02F;
  } else if (rtb_RateTransition1 < -0.02F) {
    emDriveFOC_control_DW.RateLimiter = PrevY - 0.02F;
  } else {
    emDriveFOC_control_DW.RateLimiter = UnitDelay_DSTATE_c;
  }

  /* End of Outputs for SubSystem: '<S41>/CalculateRefSign' */
  /* End of Outputs for SubSystem: '<S2>/SensorlessTop' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */

  /* Switch: '<S3>/Switch1' incorporates:
   *  Abs: '<S3>/Abs'
   *  Constant: '<S3>/Is_max'
   *  Constant: '<S3>/Is_max1'
   *  Constant: '<S3>/Is_max2'
   *  RelationalOperator: '<S3>/GreaterThan'
   */
  if (fabsf(emDriveFOC_control_DW.RPM_act) > Ctrl_Limits__IsMaxSpeedLimit) {
    rtb_RateTransition1 = Ctrl_Limits__IsMax;
  } else {
    rtb_RateTransition1 = Ctrl_Limits__IsMaxStartup;
  }

  /* Gain: '<S3>/Gain2' incorporates:
   *  Switch: '<S3>/Switch1'
   */
  emDriveFOC_control_DW.Gain2 = 1.41421354F * rtb_RateTransition1;

  /* RateTransition generated from: '<S3>/TemperatureAndPowerLimit' incorporates:
   *  Constant: '<S3>/Iq_max'
   */
  emDriveFOC_control_DW.TmpRTBAtTemperatureAndPowerLimi = Ctrl_Limits__IqMax;

  /* Outputs for Atomic SubSystem: '<S5>/MeasurmentsVel' */
  emDriveFOC_c_MeasurmentsVel();

  /* End of Outputs for SubSystem: '<S5>/MeasurmentsVel' */

  /* Outputs for Atomic SubSystem: '<S3>/TemperatureAndPowerLimit' */
  em_TemperatureAndPowerLimit();

  /* End of Outputs for SubSystem: '<S3>/TemperatureAndPowerLimit' */

  /* Switch: '<S5>/Switch1' incorporates:
   *  Constant: '<S5>/Constant8'
   *  Switch: '<S5>/Switch2'
   */
  if (Ctrl_ALTTestMode__Enable) {
    /* Switch: '<S5>/Switch2' */
    if (emDriveFOC_control_DW.RateTransition6) {
      emDriveFOC_control_Y.DataTsc.RPM = Ctrl_Gen_Stat__RPMref;
    } else {
      emDriveFOC_control_Y.DataTsc.RPM = 0.0F;
    }
  } else {
    emDriveFOC_control_Y.DataTsc.RPM = emDriveFOC_control_DW.RPM;
  }

  /* End of Switch: '<S5>/Switch1' */

  /* RateTransition: '<Root>/Rate Transition' */
  emDriveFOC_control_DW.RateTransition_Buffer0 = emDriveFOC_control_DW.Switch2;

  /* Outputs for Atomic SubSystem: '<Root>/Current_loop' */
  /* Outputs for Atomic SubSystem: '<S2>/SensorlessTop' */
  /* Outputs for Atomic SubSystem: '<S41>/CalculateRefSign' */
  /* RateLimiter: '<S116>/Rate Limiter' */
  PrevY = emDriveFOC_control_DW.RateLimiter;
  emDriveFOC_control_DW.PrevY = PrevY;

  /* End of Outputs for SubSystem: '<S41>/CalculateRefSign' */
  /* End of Outputs for SubSystem: '<S2>/SensorlessTop' */
  /* End of Outputs for SubSystem: '<Root>/Current_loop' */
  emDriveFOC_control_DW.UnitDelay_DSTATE_c = UnitDelay_DSTATE_c;
}

FOC_RESTORE_CODE
  /* Model step function for TID2 */
//  FOC_FAST_CODE
  void emDriveFOC_control_step2(void)  /* Sample time: [1.0s, 0.0s] */
{
  real32_T e1obs;
  real32_T e2obs;

  /* SignalConversion: '<S261>/Signal Conversion' incorporates:
   *  Constant: '<S4>/Constant10'
   *  Constant: '<S4>/Constant7'
   *  MATLAB Function: '<S4>/CalculatedDgains'
   */
  /*  based on ST FOC recomendation */
  /*  Rs stator resistance */
  /*  Ls stator inductance */
  /*  Bandwith [Hz]. For 16 kHz FOC with procesing delay 2 Ts bandwith 500 Hz */
  /*  gives minimum overshoot. For beggining start with bandwith 100 Hz */
  /* MATLAB Function 'InitGlobals/CalculatedDgains': '<S258>:1' */
  /* '<S258>:1:7' Kp = Ls * Bandwith * 2 *pi; */
  /* P gain  */
  /* '<S258>:1:8' Ki = Rs * Bandwith * 2 *pi; */
  /* I gain  */
  /*  based on ST FOC recomendation */
  /*  Rs stator resistance */
  /*  Ls stator inductance */
  /*  Bandwith [Hz]. For 16 kHz FOC with procesing delay 2 Ts bandwith 500 Hz */
  /*  gives minimum overshoot. For beggining start with bandwith 100 Hz */
  /* MATLAB Function 'InitGlobals/CalculatedQgains': '<S259>:1' */
  /* '<S259>:1:7' Kp = Ls * Bandwith * 2 *pi; */
  /* P gain  */
  /* '<S259>:1:8' Ki = Rs * Bandwith * 2 *pi; */
  /* I gain  */
  Ctrl_CurrentReg_Calculated__DaxisPgain = Ctrl_Motor__Ld *
    Ctrl_CurrentReg_Calculated__Bandwith * 2.0F * 3.14159274F;

  /* MATLAB Function: '<S4>/CalculatedDgains' incorporates:
   *  Constant: '<S4>/Constant10'
   *  Constant: '<S4>/Constant6'
   *  MATLAB Function: '<S4>/CalculatedQgains'
   */
  Ctrl_CurrentReg_Calculated__DaxisIgain = Ctrl_Motor__Rs *
    Ctrl_CurrentReg_Calculated__Bandwith * 2.0F * 3.14159274F;

  /* SignalConversion: '<S263>/Signal Conversion' incorporates:
   *  Constant: '<S4>/Constant10'
   *  Constant: '<S4>/Constant8'
   *  MATLAB Function: '<S4>/CalculatedQgains'
   */
  Ctrl_CurrentReg_Calculated__QaxisPgain = Ctrl_Motor__Lq *
    Ctrl_CurrentReg_Calculated__Bandwith * 2.0F * 3.14159274F;

  /* SignalConversion: '<S264>/Signal Conversion' */
  Ctrl_CurrentReg_Calculated__QaxisIgain =
    Ctrl_CurrentReg_Calculated__DaxisIgain;

  /* MATLAB Function: '<S4>/EBMFObserverGains' incorporates:
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant2'
   *  Constant: '<S4>/Constant3'
   *  Constant: '<S4>/Constant4'
   */
  /*  look in STM UM1052 */
  /* MATLAB Function 'InitGlobals/EBMFObserverGains': '<S260>:1' */
  /* '<S260>:1:4' e1 = 1-(Rs*dt/Ld); */
  /* '<S260>:1:5' e2=1; */
  /* '<S260>:1:7' e1obs = e1 / ObserverF; */
  e1obs = (1.0F - 1.0F / Ctrl_Gen__Fcur * Ctrl_Motor__Rs / Ctrl_Motor__Ld) /
    Ctrl_SensorlessCalculatedGain__ObserverF;

  /* '<S260>:1:8' e2obs = e2 / ObserverF; */
  e2obs = 1.0F / Ctrl_SensorlessCalculatedGain__ObserverF;

  /* SignalConversion: '<S265>/Signal Conversion' incorporates:
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant2'
   *  Constant: '<S4>/Constant3'
   *  MATLAB Function: '<S4>/EBMFObserverGains'
   */
  /* '<S260>:1:10' K1 = -((e1obs+e2obs-2)/dt*Ld - Rs); */
  /* '<S260>:1:11' K2 = Ld * (1-e1obs-e2obs + e1obs*e2obs) / (dt*dt); */
  Ctrl_SensorlessCalculatedGain__K1 = -(((e1obs + e2obs) - 2.0F) / (1.0F /
    Ctrl_Gen__Fcur) * Ctrl_Motor__Ld - Ctrl_Motor__Rs);

  /* SignalConversion: '<S266>/Signal Conversion' incorporates:
   *  Constant: '<S4>/Constant2'
   *  Constant: '<S4>/Constant3'
   *  MATLAB Function: '<S4>/EBMFObserverGains'
   */
  Ctrl_SensorlessCalculatedGain__K2 = (((1.0F - e1obs) - e2obs) + e1obs * e2obs)
    * Ctrl_Motor__Ld / (1.0F / Ctrl_Gen__Fcur * (1.0F / Ctrl_Gen__Fcur));

  /* Constant: '<S4>/Constant11' */
  emDriveFOC_control_DW.Constant11 = 1.0F / Ctrl_Gen__Fcur;

  /* Constant: '<S4>/RPMtoOmegaE' */
  emDriveFOC_control_DW.RPMtoOmegaE = 0.104719758F * Ctrl_Motor__Np;

  /* Constant: '<S4>/OmegaEtoRPM' */
  emDriveFOC_control_DW.OmegaEtoRPM = 60.0F / (Ctrl_Motor__Np * 2.0F *
    3.14159274F);

  /* Outputs for Atomic SubSystem: '<Root>/ConstInit' */
  emDriveFOC_contro_ConstInitTID2();

  /* End of Outputs for SubSystem: '<Root>/ConstInit' */

  /* Sum: '<Root>/Add' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Constant: '<Root>/Constant6'
   *  Constant: '<Root>/FW_Version'
   *  Sum: '<Root>/Add1'
   */
  Ctrl_Doc_FW__Version = (int32_T)((uint32_T)Ctrl_Stim1__Override +
    Ctrl_Stim2__Override) + 1;
}

FOC_RESTORE_CODE
  /* Model initialize function */
  FOC_SLOW_CODE
  void emDriveFOC_control_initialize(void)
{
  /* Registration code */

  /* Set task counter limit used by the static main program */
  (emDriveFOC_control_M)->Timing.TaskCounters.cLimit[0] = 1;
  (emDriveFOC_control_M)->Timing.TaskCounters.cLimit[1] = 16;
  (emDriveFOC_control_M)->Timing.TaskCounters.cLimit[2] = 16000;

  /* block I/O */

  /* custom signals */
  Ctrl_Gen_Stat__Iq_foc = 0.0F;
  Ctrl_Gen_Stat__Udc = 0.0F;
  Ctrl_Gen_Stat__RPMref = 0.0F;
  Ctrl_CurrentReg_Calculated__DaxisPgain = 0.0F;
  Ctrl_CurrentReg_Calculated__DaxisIgain = 0.0F;
  Ctrl_CurrentReg_Calculated__QaxisPgain = 0.0F;
  Ctrl_CurrentReg_Calculated__QaxisIgain = 0.0F;
  Ctrl_SensorlessCalculatedGain__K1 = 0.0F;
  Ctrl_SensorlessCalculatedGain__K2 = 0.0F;
  Ctrl_VelocityReg_StatusRPM[0] = 0.0F;
  Ctrl_VelocityReg_StatusRPM[1] = 0.0F;
  Ctrl_VelocityReg_StatusRPM[2] = 0.0F;

  {
    int32_T i;
    for (i = 0; i < 7; i++) {
      Ctrl_VelocityReg_StatusIq[i] = 0.0F;
    }
  }

  Ctrl_Stim_Status[0] = 0.0F;
  Ctrl_Stim_Status[1] = 0.0F;
  Ctrl_Limits_StatusRPMramp[0] = 0.0F;
  Ctrl_Limits_StatusRPMramp[1] = 0.0F;
  Ctrl_Limits_StatusRampIn = 0.0F;
  Ctrl_Measurments__UphRms = 0.0F;
  Ctrl_Measurments__IphRms = 0.0F;
  Ctrl_Measurments__Pel = 0.0F;
  Ctrl_Measurments__RPM = 0.0F;
  Ctrl_Gen_Stat__TemperatureFromR = 0.0F;
  Ctrl_Motor__CopperLoss = 0.0F;
  Ctrl_Motor__IronLoss = 0.0F;
  Ctrl_Power__CompensatedPower = 0.0F;
  Ctrl_Motor__TotalLoss = 0.0F;
  Ctrl_PowerLimit__StatusActive = 0.0F;
  Ctrl_PowerLimit__StatusLimitIq = 0.0F;
  Ctrl_Limits_StatusLim[0] = 0.0F;
  Ctrl_Limits_StatusLim[1] = 0.0F;
  Ctrl_Limits_StatusLim[2] = 0.0F;
  Ctrl_Limits_StatusIsMax = 0.0F;
  Ctrl_Gen_Stat__MotorWireTemp = 0.0F;
  Ctrl_PowerLimit__LimitStatus = 0.0F;
  Ctrl_Limits_StatusUdcMaxToIq = 0.0F;
  Ctrl_Mod_StatusPWM[0] = 0.0F;
  Ctrl_Mod_StatusPWM[1] = 0.0F;
  Ctrl_Mod_StatusPWM[2] = 0.0F;
  Ctrl_CurrentReg_StatusIabc[0] = 0.0F;
  Ctrl_CurrentReg_StatusIabc[1] = 0.0F;
  Ctrl_CurrentReg_StatusIabc[2] = 0.0F;
  Ctrl_OpenLoop_StatusRPMopen = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabEst[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabEst[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabErr[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabErr[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabIn[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_IabIn[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_UabIn[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_UabIn[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_EabEst[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_EabEst[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_Theta[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_Theta[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_Theta[2] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_RPM[0] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_RPM[1] = 0.0F;
  Ctrl_Sensorless_StatusBEMF_CriteriaPLL = 0.0F;
  Ctrl_Sensorless_StatusBEMF_CriteriaBEMF = 0.0F;
  Ctrl_Sensorless_StatusBEMF_PLL_Err = 0.0F;
  Ctrl_CurrentReg_StatusTheta = 0.0F;
  Ctrl_CurrentReg_StatusRpm = 0.0F;
  Ctrl_CurrentReg_StatusUabc[0] = 0.0F;
  Ctrl_CurrentReg_StatusUabc[1] = 0.0F;
  Ctrl_CurrentReg_StatusUabc[2] = 0.0F;
  Ctrl_CurrentReg_StatusIdqRef[0] = 0.0F;
  Ctrl_CurrentReg_StatusIdqRef[1] = 0.0F;
  Ctrl_CurrentReg_StatusIdqAct[0] = 0.0F;
  Ctrl_CurrentReg_StatusIdqAct[1] = 0.0F;
  Ctrl_ALTTestMode_StatusUdq[0] = 0.0F;
  Ctrl_ALTTestMode_StatusUdq[1] = 0.0F;
  Ctrl_CurrentReg_StatusUdq[0] = 0.0F;
  Ctrl_CurrentReg_StatusUdq[1] = 0.0F;
  Ctrl_CurrentReg_StatusUdq[2] = 0.0F;
  Ctrl_CurrentReg_StatusUdq[3] = 0.0F;
  Ctrl_CurrentReg_StatusIqmax = 0.0F;
  Ctrl_CurrentReg_StatusModFactor = 0.0F;
  Ctrl_CurrentReg_StatusId[0] = 0.0F;
  Ctrl_CurrentReg_StatusId[1] = 0.0F;
  Ctrl_CurrentReg_StatusId[2] = 0.0F;
  Ctrl_CurrentReg_StatusId[3] = 0.0F;
  Ctrl_FieldWeak_StatusId[0] = 0.0F;
  Ctrl_FieldWeak_StatusId[1] = 0.0F;
  Ctrl_FieldWeak_StatusUs[0] = 0.0F;
  Ctrl_FieldWeak_StatusUs[1] = 0.0F;
  Ctrl_FieldWeak_StatusUs[2] = 0.0F;
  Ctrl_Doc_FW__Version = 0;
  Ctrl_PowerLimit__Counter = 0;
  Ctrl_PowerLimit__State = 0;
  Ctrl_Sensorless_Status__State = 0;
  Ctrl_Sensorless_Status__LastNonErrorState = 0;
  Ctrl_Sensorless_Status__LatchedCriteriaFlags = 0;
  Ctrl_Sensorless_StatusBEMF_CriteriaFlags = 0;
  Ctrl_Gen_Stat__RunStopOut = false;
  Ctrl_Gen_Stat__RunStopIn = false;
  Ctrl_VelocityReg_StatusTracking = false;
  Ctrl_VelocityReg_StatusDissableIntegration = false;

  {
    int32_T i;
    for (i = 0; i < 5; i++) {
      Ctrl_Limits_StatusDisIntegration[i] = false;
    }
  }

  /* Start for DataStoreMemory: '<Root>/Data Store Memory' */
  emDriveFOC_control_DW.RPMtoOmegaE_c = 0.104719758F * Ctrl_Motor__Np;

  /* Start for DataStoreMemory: '<Root>/Data Store Memory1' */
  emDriveFOC_control_DW.OmegaEtoRPM_k = 60.0F / (Ctrl_Motor__Np * 2.0F *
    3.14159274F);

  /* Start for DataStoreMemory: '<Root>/Data Store Memory3' */
  emDriveFOC_control_DW.Tsc = 1.0F / Ctrl_Gen__Fcur;

  /* SystemInitialize for Atomic SubSystem: '<Root>/ConstInit' */
  emDriveFOC_c_ConstInit_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/ConstInit' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/Current_loop' */
  /* SystemInitialize for Atomic SubSystem: '<S2>/AngleGen' */
  /* InitializeConditions for Delay: '<S47>/Delay' */
  emDriveFOC_control_DW.icLoad_g = true;

  /* End of SystemInitialize for SubSystem: '<S2>/AngleGen' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/FOC' */
  /* SystemInitialize for Atomic SubSystem: '<S38>/Calculate_Id' */
  /* SystemInitialize for Atomic SubSystem: '<S52>/FieldWeakening' */
  /* SystemInitialize for Atomic SubSystem: '<S65>/ Field weakening PI' */
  /* InitializeConditions for Delay: '<S70>/Delay' */
  emDriveFOC_control_DW.icLoad_o = true;

  /* End of SystemInitialize for SubSystem: '<S65>/ Field weakening PI' */

  /* SystemInitialize for Atomic SubSystem: '<S65>/LPFFieldWeak' */
  /* InitializeConditions for Delay: '<S71>/Delay' */
  emDriveFOC_control_DW.icLoad_n = true;

  /* End of SystemInitialize for SubSystem: '<S65>/LPFFieldWeak' */
  /* End of SystemInitialize for SubSystem: '<S52>/FieldWeakening' */
  /* End of SystemInitialize for SubSystem: '<S38>/Calculate_Id' */

  /* SystemInitialize for Atomic SubSystem: '<S38>/CurrentRegulators' */
  /* SystemInitialize for Atomic SubSystem: '<S54>/ PI regulator D' */
  /* InitializeConditions for Delay: '<S83>/Delay' */
  emDriveFOC_control_DW.icLoad_p = true;

  /* End of SystemInitialize for SubSystem: '<S54>/ PI regulator D' */

  /* SystemInitialize for Atomic SubSystem: '<S54>/ PI regulator Q' */
  /* InitializeConditions for Delay: '<S84>/Delay' */
  emDriveFOC_control_DW.icLoad_jc = true;

  /* End of SystemInitialize for SubSystem: '<S54>/ PI regulator Q' */
  /* End of SystemInitialize for SubSystem: '<S38>/CurrentRegulators' */
  /* End of SystemInitialize for SubSystem: '<S2>/FOC' */

  /* SystemInitialize for Atomic SubSystem: '<S2>/SensorlessTop' */
  /* SystemInitialize for Enabled SubSystem: '<S41>/Sensorless' */
  emDriveFOC__Sensorless_Init();

  /* End of SystemInitialize for SubSystem: '<S41>/Sensorless' */
  /* End of SystemInitialize for SubSystem: '<S2>/SensorlessTop' */
  /* End of SystemInitialize for SubSystem: '<Root>/Current_loop' */

  /* SystemInitialize for Atomic SubSystem: '<S3>/TemperatureAndPowerLimit' */
  TemperatureAndPowerLim_Init();

  /* End of SystemInitialize for SubSystem: '<S3>/TemperatureAndPowerLimit' */

  /* SystemInitialize for Atomic SubSystem: '<S5>/LPF_Measurment' */
  /* InitializeConditions for Delay: '<S267>/Delay' */
  emDriveFOC_control_DW.icLoad_jg = true;

  /* End of SystemInitialize for SubSystem: '<S5>/LPF_Measurment' */

  /* SystemInitialize for Atomic SubSystem: '<S5>/MeasurmentsVel' */
  emDrive_MeasurmentsVel_Init();

  /* End of SystemInitialize for SubSystem: '<S5>/MeasurmentsVel' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/RPM_ramp_and_limit' */
  emD_RPM_ramp_and_limit_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/RPM_ramp_and_limit' */

  /* SystemInitialize for Atomic SubSystem: '<Root>/Velocity_loop' */
  emDriveF_Velocity_loop_Init();

  /* End of SystemInitialize for SubSystem: '<Root>/Velocity_loop' */

  /* ConstCode for Constant: '<Root>/FW_date' */
  Ctrl_Doc_FW__Date = 20231127;

  /* ConstCode for SignalConversion: '<S8>/Signal Conversion' incorporates:
   *  Constant: '<Root>/ver_major'
   *  Constant: '<Root>/ver_minor'
   *  Constant: '<Root>/ver_patch'
   */
  Ctrl_Doc_version[0] = 0U;
  Ctrl_Doc_version[1] = 1U;
  Ctrl_Doc_version[2] = 0U;
}

FOC_RESTORE_CODE
  /*
   * File trailer for generated code.
   *
   * [EOF]
   */
