/*
 * File: emDriveFOC_control_param.c
 *
 * Code generated for Simulink model 'emDriveFOC_control'.
 *
 * Model version                  : 12.13
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Mon Nov 27 10:45:55 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#include "simulink_model/emDriveFOC_control_global.h"
#include "simulink_model/emDriveFOC_control.h"

/* Exported data definition */

/* FLOW2 exported parameter list */

/*
   @@ SYMBOL = Ctrl_ALTTestMode__Enable
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "Enables ALT Test Mode. [bit]"
   @@ GROUP = Ctrl | ALTTestMode
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31AD
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_ALTTestMode__Enable = false;

/* Referenced by:
 * '<S2>/Constant2'
 * '<S5>/Constant8'
 * '<S6>/Constant8'
 * '<S46>/Constant8'
 * '<S54>/Constant8'
 * '<S118>/Constant2'
 * '<S85>/Constant8'
 */

/*
   @@ SYMBOL = Ctrl_ALTTestMode__RPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10000.0]
   @@ DESCRIPTION = "Determines frequency during the ALT test. [rpm]"
   @@ GROUP = Ctrl | ALTTestMode
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x31AD
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 1500.0
   @@ END
 */
volatile real32_T Ctrl_ALTTestMode__RPM = 1500.0F;

/* Referenced by: '<S46>/Constant1' */

/*
   @@ SYMBOL = Ctrl_AngleGenerator__Enable
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "Enable angle generator, which replaces real angle sensor. dissable sensorless that this angle will be used for FOC [bit]"
   @@ GROUP = Ctrl | AngleGenerator
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31DA
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 0
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_AngleGenerator__Enable = false;

/* Referenced by: '<S2>/Constant8' */

/*
   @@ SYMBOL = Ctrl_AngleGenerator__Offset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-180.0 ... 180.0]
   @@ DESCRIPTION = "Offset in electrical deg -180 to 180. 0 degree d axis -> phase 1 [deg]"
   @@ GROUP = Ctrl | AngleGenerator
   @@ DIMENSION = 1 1
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x31DA
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_AngleGenerator__Offset = 0.0F;

/* Referenced by: '<S46>/Constant5' */

/*
   @@ SYMBOL = Ctrl_AngleGenerator__RPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-1.0E+6 ... 1.0E+6]
   @@ DESCRIPTION = "Generated virtual RPM of motor. Motor pole pairs used in calculation [rpm]"
   @@ GROUP = Ctrl | AngleGenerator
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x31DA
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 0
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_AngleGenerator__RPM = 0.0F;

/* Referenced by: '<S46>/Constant4' */

/*
   @@ SYMBOL = Ctrl_CurrentReg_Calculated__Bandwith
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [1.0 ... 10000.0]
   @@ DESCRIPTION = "Target PI regulator bandwith. Motor resistance and inductance shall be exact. 100 Hz is safe start value, can be later increased [Hz]"
   @@ GROUP = Ctrl | CurrentReg | Calculated
   @@ DIMENSION = 1 1
   @@ UNIT = "Hz"
   @! CAN_INDEX = 0x313B
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 500.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg_Calculated__Bandwith = 500.0F;

/* Referenced by: '<S4>/Constant10' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__CalculationDelay
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "This is delay betwen calculated and applied PWM. Adjust so, that at high RPM no load Ud=0.  Unit current loop sample times.If set to big, will improve current regulator stability [sample_time]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "sample_time"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 1.7
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__CalculationDelay = 1.7F;

/* Referenced by: '<S58>/Constant7' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__DaxisIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Current D regulator I gain [V/As]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "V/As"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 7414.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__DaxisIgain = 7414.0F;

/* Referenced by: '<S83>/Constant1' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__DaxisPgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Current D regulator P gain [V/A]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "V/A"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 80.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__DaxisPgain = 80.0F;

/* Referenced by: '<S83>/Constant' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__DecoupleIdIqEnable
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 2.0]
   @@ DESCRIPTION = "Enable decoupling / feed forward of current regulators for I x XL term. If greater than 1 is enabled. If 0 then it is disabled. [1]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "1"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.6
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__DecoupleIdIqEnable = 0.6F;

/* Referenced by: '<S54>/Constant1' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__DecouplePsi
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "Enable decoupling / feed forward of Q current regulator for Omega x psi term [bit]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_CurrentReg__DecouplePsi = true;

/* Referenced by: '<S54>/Constant5' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__QaxisIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Current Q regulator I gain [V/As]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "V/As"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 7414.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__QaxisIgain = 7414.0F;

/* Referenced by: '<S84>/Constant1' */

/*
   @@ SYMBOL = Ctrl_CurrentReg__QaxisPgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Current Q regulator P gain [V/A]"
   @@ GROUP = Ctrl | CurrentReg
   @@ DIMENSION = 1 1
   @@ UNIT = "V/A"
   @! CAN_INDEX = 0x3130
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 60.0
   @@ END
 */
volatile real32_T Ctrl_CurrentReg__QaxisPgain = 60.0F;

/* Referenced by: '<S84>/Constant' */

/*
   @@ SYMBOL = Ctrl_FieldWeak__Igain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "I gain. See above. []"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3140
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 2.0
   @@ END
 */
volatile real32_T Ctrl_FieldWeak__Igain = 2.0F;

/* Referenced by: '<S70>/Constant1' */

/*
   @@ SYMBOL = Ctrl_FieldWeak__LPFtimeConstant
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "Time constat of LPF after field weakening regulator [s]"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3140
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.05
   @@ END
 */
volatile real32_T Ctrl_FieldWeak__LPFtimeConstant = 0.05F;

/* Referenced by: '<S28>/TimeConstant' */

/*
   @@ SYMBOL = Ctrl_FieldWeak__Pgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "P gain. Normed. Input is modulation factor error, output is ratio to MaxId.  P and I must be bigh enough that FW regulator is fast enough to folow in MTPV region - if motor ever comes there []"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3140
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.2
   @@ END
 */
volatile real32_T Ctrl_FieldWeak__Pgain = 0.2F;

/* Referenced by: '<S70>/Constant' */

/*
   @@ SYMBOL = Ctrl_FieldWeak__TargetModulationFactor
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.5 ... 2.0]
   @@ DESCRIPTION = "Field weakening regulator will try to regulate modulation factor to this value. This value +0.2 (but not less as 1) is also max value for Id Iq regulators output []"
   @@ GROUP = Ctrl | FieldWeak
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3140
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.9
   @@ END
 */
volatile real32_T Ctrl_FieldWeak__TargetModulationFactor = 0.9F;

/* Referenced by: '<S65>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Gen__CurToVelRatio
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = SLONG [1 ... 128]
   @@ DESCRIPTION = "Ratio betwen current and velocity execution frequency []"
   @@ GROUP = Ctrl | Gen
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3100
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 16
   @@ END
 */
volatile int32_T Ctrl_Gen__CurToVelRatio = 16;

/* Referenced by:
 * '<S15>/Constant16'
 * '<S286>/Constant3'
 * '<S229>/Constant4'
 * '<S270>/Constant1'
 * '<S306>/Constant5'
 */

/*
   @@ SYMBOL = Ctrl_Gen__Fcur
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [1000.0 ... 16000.0]
   @@ DESCRIPTION = "Current loop frequency, ussaly same as PWM frequency [Hz]"
   @@ GROUP = Ctrl | Gen
   @@ DIMENSION = 1 1
   @@ UNIT = "Hz"
   @! CAN_INDEX = 0x3100
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 16000.0
   @@ END
 */
volatile real32_T Ctrl_Gen__Fcur = 16000.0F;

/* Referenced by:
 * '<Root>/Data Store Memory3'
 * '<S4>/Constant11'
 * '<S4>/Constant3'
 * '<S11>/Constant10'
 * '<S15>/Constant16'
 * '<S16>/Constant15'
 * '<S17>/Constant'
 * '<S18>/Constant16'
 * '<S19>/Constant16'
 * '<S20>/Constant14'
 * '<S22>/Constant11'
 * '<S22>/Constant4'
 * '<S286>/Constant3'
 * '<S229>/Constant4'
 * '<S270>/Constant1'
 * '<S306>/Constant5'
 */

/*
   @@ SYMBOL = Ctrl_Gen__MaxRPMcalculations
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "Maximum RPM which can be observed by quadrature PLL. Set this well above any realistic motor RPM [rpm]"
   @@ GROUP = Ctrl | Gen
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3100
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 25000.0
   @@ END
 */
volatile real32_T Ctrl_Gen__MaxRPMcalculations = 25000.0F;

/* Referenced by: '<S127>/Constant3' */

/*
   @@ SYMBOL = Ctrl_IdtoIq_Switch_Enable
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "Enables switch from regular to Id-to-Iq switched running of the motor [bit]"
   @@ GROUP = Ctrl | IdtoIq | Switch
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3172
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_IdtoIq_Switch_Enable = false;

/* Referenced by: '<S38>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Limits_OverrideCloseLoopRamp
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "Override []"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 17
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_Limits_OverrideCloseLoopRamp = false;

/* Referenced by: '<S6>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Limits__BridgeTemperature
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 150.0]
   @@ DESCRIPTION = "Power stage temperature limit [degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 9
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 95.0
   @@ END
 */
volatile real32_T Ctrl_Limits__BridgeTemperature = 95.0F;

/* Referenced by: '<S237>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Limits__BridgeTemperatureIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "I gain of limit regulator. Input C, output 0-1. [1/degCs]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degCs"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 11
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Limits__BridgeTemperatureIgain = 0.0F;

/* Referenced by: '<S237>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Limits__BridgeTemperaturePgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "P gain of limit regulator. Input C, output 0-1 [1/degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 10
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Limits__BridgeTemperaturePgain = 0.1F;

/* Referenced by: '<S237>/Constant' */

/*
   @@ SYMBOL = Ctrl_Limits__CloseLoopRamp
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1025.0]
   @@ DESCRIPTION = "Closed loop ramp [rpm/s]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm/s"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 18
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 400.0
   @@ END
 */
volatile real32_T Ctrl_Limits__CloseLoopRamp = 400.0F;

/* Referenced by: '<S6>/Constant5' */

/*
   @@ SYMBOL = Ctrl_Limits__IdMin
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-250.0 ... 0.0]
   @@ DESCRIPTION = "Minimum negative Id current. This is negative number. Prevents to deep field weakening. Shall be set to -psi/Ld. This is peak value not rms. [Ap]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = -4.9
   @@ END
 */
volatile real32_T Ctrl_Limits__IdMin = -4.9F;

/* Referenced by:
 * '<S52>/Constant6'
 * '<S53>/Constant6'
 */

/*
   @@ SYMBOL = Ctrl_Limits__IqMax
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "Maximum motoring Iq [Ap]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 5.5
   @@ END
 */
volatile real32_T Ctrl_Limits__IqMax = 5.5F;

/* Referenced by: '<S3>/Iq_max' */

/*
   @@ SYMBOL = Ctrl_Limits__IsMax
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 200.0]
   @@ DESCRIPTION = "Maximum phase current [Arms] [Arms]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "Arms"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 4.0
   @@ END
 */
volatile real32_T Ctrl_Limits__IsMax = 4.0F;

/* Referenced by: '<S3>/Is_max' */

/*
   @@ SYMBOL = Ctrl_Limits__IsMaxSpeedLimit
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 18000.0]
   @@ DESCRIPTION = "Motor speed at which Is_max limit is switched to ensure the reliable startup with high load and unbalance, and at the same time limits the motor current while operates with higher speeds. [rpm]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 22
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1000.0
   @@ END
 */
volatile real32_T Ctrl_Limits__IsMaxSpeedLimit = 1000.0F;

/* Referenced by: '<S3>/Is_max2' */

/*
   @@ SYMBOL = Ctrl_Limits__IsMaxStartup
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 20.0]
   @@ DESCRIPTION = "Maximum phase current for speeds less than CtrlLimits [Arms] [A]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "A"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 21
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 4.0
   @@ END
 */
volatile real32_T Ctrl_Limits__IsMaxStartup = 4.0F;

/* Referenced by: '<S3>/Is_max1' */

/*
   @@ SYMBOL = Ctrl_Limits__MaximumRampLowRpm
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1050.0]
   @@ DESCRIPTION = "Open loop ramp override value [rpm/s]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm/s"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 20
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 400.0
   @@ END
 */
volatile real32_T Ctrl_Limits__MaximumRampLowRpm = 400.0F;

/* Referenced by: '<S6>/Constant6' */

/*
   @@ SYMBOL = Ctrl_Limits__MotorTemperature
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 180.0]
   @@ DESCRIPTION = "Motor temperature limit [degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 12
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 130.0
   @@ END
 */
volatile real32_T Ctrl_Limits__MotorTemperature = 130.0F;

/* Referenced by: '<S226>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Limits__MotorTemperatureIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "I gain of limit regulator. Input C, output 0-1. [1/degCs]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degCs"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 14
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Limits__MotorTemperatureIgain = 0.0F;

/* Referenced by: '<S226>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Limits__MotorTemperaturePgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "P gain of limit regulator. Input C, output 0-1 [1/degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 13
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Limits__MotorTemperaturePgain = 0.0F;

/* Referenced by: '<S226>/Constant' */

/*
   @@ SYMBOL = Ctrl_Limits__OpenLoopRampOverrideValue
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1025.0]
   @@ DESCRIPTION = "Open loop ramp override value [rpm/s]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm/s"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 19
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 400.0
   @@ END
 */
volatile real32_T Ctrl_Limits__OpenLoopRampOverrideValue = 400.0F;

/* Referenced by: '<S6>/Constant3' */

/*
   @@ SYMBOL = Ctrl_Limits__OutletTemperature
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 150.0]
   @@ DESCRIPTION = "Power stage temperature limit [degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 23
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 85.0
   @@ END
 */
volatile real32_T Ctrl_Limits__OutletTemperature = 85.0F;

/* Referenced by: '<S254>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Limits__OutletTemperatureIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "I gain of limit regulator. Input C, output 0-1. [1/degCs]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degCs"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 25
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Limits__OutletTemperatureIgain = 0.0F;

/* Referenced by: '<S254>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Limits__OutletTemperaturePgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "P gain of limit regulator. Input C, output 0-1 [1/degC]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/degC"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 24
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Limits__OutletTemperaturePgain = 0.1F;

/* Referenced by: '<S254>/Constant' */

/*
   @@ SYMBOL = Ctrl_Limits__PowerLimitIgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "I gain of limit regulator. Input C, output 0-1. [1/Ws]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/Ws"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 16
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.002
   @@ END
 */
volatile real32_T Ctrl_Limits__PowerLimitIgain = 0.002F;

/* Referenced by: '<S241>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Limits__PowerLimitPgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "P gain of limit regulator. Input C, output 0-1 [1/W]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "1/W"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 15
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.0002
   @@ END
 */
volatile real32_T Ctrl_Limits__PowerLimitPgain = 0.0002F;

/* Referenced by: '<S241>/Constant' */

/*
   @@ SYMBOL = Ctrl_Limits__RPMnegative
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-30000.0 ... 0.0]
   @@ DESCRIPTION = "Negative RPM limit - negative number [rpm]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = -9000.0
   @@ END
 */
volatile real32_T Ctrl_Limits__RPMnegative = -9000.0F;

/* Referenced by: '<S6>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Limits__RPMpositive
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 30000.0]
   @@ DESCRIPTION = "Positive RPM limit.  Used at input of velocity regulator [rpm]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 9000.0
   @@ END
 */
volatile real32_T Ctrl_Limits__RPMpositive = 9000.0F;

/* Referenced by:
 * '<S6>/Constant13'
 * '<S22>/Constant6'
 */

/*
   @@ SYMBOL = Ctrl_Limits__RPMramp
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "Ramp for RPM requested value [rpm/s]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm/s"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 8
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1013.0
   @@ END
 */
volatile real32_T Ctrl_Limits__RPMramp = 1013.0F;

/* Referenced by: '<S286>/Constant' */

/*
   @@ SYMBOL = Ctrl_Limits__UdcMaxToTorque
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 2000.0]
   @@ DESCRIPTION = "DC voltage at which torque will be increased (at positive RPM). Increasing torque at high DC voltage prevents regeneration and risign DC link. [V]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "V"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 380.0
   @@ END
 */
volatile real32_T Ctrl_Limits__UdcMaxToTorque = 380.0F;

/* Referenced by: '<S220>/Constant5' */

/*
   @@ SYMBOL = Ctrl_Limits__UdcMaxToTorqueGain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "How much Iq will be increased for each V of DC overvoltege. [A/V]"
   @@ GROUP = Ctrl | Limits
   @@ DIMENSION = 1 1
   @@ UNIT = "A/V"
   @! CAN_INDEX = 0x3120
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.4
   @@ END
 */
volatile real32_T Ctrl_Limits__UdcMaxToTorqueGain = 0.4F;

/* Referenced by: '<S220>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Measurment__LPF_TimeConst
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "Time constat of LPF fileter for Uph, Iph, P, RPM [s]"
   @@ GROUP = Ctrl | Measurment
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.03125
   @@ END
 */
volatile real32_T Ctrl_Measurment__LPF_TimeConst = 0.03125F;

/* Referenced by: '<S30>/TimeConstant' */

/*
   @@ SYMBOL = Ctrl_Measurment__Pel_LPF_TimeConst
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "Time constat for additional filtering of Pel [s]"
   @@ GROUP = Ctrl | Measurment
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x31F0
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.05
   @@ END
 */
volatile real32_T Ctrl_Measurment__Pel_LPF_TimeConst = 0.05F;

/* Referenced by: '<S284>/TimeConstant' */

/*
   @@ SYMBOL = Ctrl_Mod__DeadTimeComp
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Multipled with DC voltage to get deat time compensation voltage. This is aproximately Tdeat_time/Tfoc []"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.02
   @@ END
 */
volatile real32_T Ctrl_Mod__DeadTimeComp = 0.02F;

/* Referenced by: '<S55>/Constant' */

/*
   @@ SYMBOL = Ctrl_Mod__MinOffTimeHighSide
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 0.0001]
   @@ DESCRIPTION = "Minimum time when high side is off or low side is on. Has no sense if DPWM1 modulation is used. [s]"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 2.0E-6
   @@ END
 */
volatile real32_T Ctrl_Mod__MinOffTimeHighSide = 2.0E-6F;

/* Referenced by: '<S20>/MinOffTimeHighSide' */

/*
   @@ SYMBOL = Ctrl_Mod__MinOffTimeLowSide
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 0.0001]
   @@ DESCRIPTION = "Minimum time when low side is off or high side is on. Typical set to > 0 if high side PWM must be always acitev because bootstrap power supply. Has no sense if DPWM1 modulation is used. [s]"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Mod__MinOffTimeLowSide = 0.0F;

/* Referenced by: '<S20>/MinOffTimeLowSide' */

/*
   @@ SYMBOL = Ctrl_Mod__MinOffTimeSecondHighSide
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 0.0001]
   @@ DESCRIPTION = "Minimumu time when phase with second highest PWM must have high side off or low side on. This is used to prevent that two high sides are simultaniusly on and current measurment with low side shunts is not possible. [s]"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 5.0E-6
   @@ END
 */
volatile real32_T Ctrl_Mod__MinOffTimeSecondHighSide = 5.0E-6F;

/* Referenced by: '<S20>/MinOffTimeSecondHighSide' */

/*
   @@ SYMBOL = Ctrl_Mod__MinPulseTime
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 0.0001]
   @@ DESCRIPTION = "Min on time for high or low side. Prevents short on pulses, which introduce excesive switching losses. Shall not be enabled if DPWM1 modulation is used at low modulation index. [s]"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Mod__MinPulseTime = 0.0F;

/* Referenced by: '<S20>/MinimumPulseTime' */

/*
   @@ SYMBOL = Ctrl_Mod__PWMOffset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "PWM offset scaling. 0.5 means no offset, above offset is positive and bellow is negative. []"
   @@ GROUP = Ctrl | Mod
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3104
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.5
   @@ END
 */
volatile real32_T Ctrl_Mod__PWMOffset = 0.5F;

/* Referenced by: '<S44>/Gain' */

/*
   @@ SYMBOL = Ctrl_Motor__IdToIqRatio
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "MTPA aproximation ratio between Id and Iq []"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Motor__IdToIqRatio = 0.1F;

/* Referenced by: '<S66>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Motor__InitialTempRise
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 200.0]
   @@ DESCRIPTION = "(vector 2 elements) Defines temperature difference betwen stator wires and ambient at power up. This value is used to intialise thermal model. Vector with value for each order of thermal model [deg C]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 2
   @@ UNIT = "deg C"
   @! CAN_INDEX = 0x310E
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = [0.0, 0.0]
   @@ END
 */
volatile real32_T Ctrl_Motor__InitialTempRise[2] = { 0.0F, 0.0F } ;

/* Referenced by: '<S247>/Constant3' */

/*
   @@ SYMBOL = Ctrl_Motor__IronLossCoef
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-7.922816E+28 ... 7.922816E+28]
   @@ DESCRIPTION = "Iron loss factor based on square RPM [V/RPM^2]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "V/RPM^2"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 2.20578
   @@ END
 */
volatile real32_T Ctrl_Motor__IronLossCoef = 2.20578F;

/* Referenced by: '<S229>/Constant5' */

/*
   @@ SYMBOL = Ctrl_Motor__Ld
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Line-N d-axis inductance. Must be lower as Lq. Inductances can be measured with HFI algorithm. See description in Ctrl_HFI_Status_Ldq [H]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "H"
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.00713
   @@ END
 */
volatile real32_T Ctrl_Motor__Ld = 0.00713F;

/* Referenced by:
 * '<S4>/Constant2'
 * '<S4>/Constant7'
 * '<S23>/Constant1'
 * '<S57>/Constant1'
 * '<S126>/Constant5'
 */

/*
   @@ SYMBOL = Ctrl_Motor__Lq
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Line-N q-axis inductance [H]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "H"
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.01092
   @@ END
 */
volatile real32_T Ctrl_Motor__Lq = 0.01092F;

/* Referenced by:
 * '<S4>/Constant8'
 * '<S23>/Constant'
 * '<S57>/Constant2'
 * '<S126>/Constant6'
 */

/*
   @@ SYMBOL = Ctrl_Motor__Np
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [1.0 ... 100.0]
   @@ DESCRIPTION = "Number of pole pairs []"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 3.0
   @@ END
 */
volatile real32_T Ctrl_Motor__Np = 3.0F;

/* Referenced by:
 * '<Root>/Data Store Memory'
 * '<Root>/Data Store Memory1'
 * '<S4>/OmegaEtoRPM'
 * '<S4>/RPMtoOmegaE'
 * '<S14>/OmegaEtoRPM'
 * '<S23>/Constant3'
 * '<S268>/Constant'
 * '<S46>/RPM_to_OmegaE'
 * '<S118>/SenzorlessStateDigram'
 */

/*
   @@ SYMBOL = Ctrl_Motor__Psi
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10.0]
   @@ DESCRIPTION = "Motor Psi. This number can be corrected with psi estimator. Shall be accurate. If you enter 0.471/motor_pole_pairs then 1 Nm = 1 Arms phase current. Real value is estimated in Ctrl_PsiEst_StatusPsi [Vs]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "Vs"
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.08
   @@ END
 */
volatile real32_T Ctrl_Motor__Psi = 0.08F;

/* Referenced by:
 * '<S57>/Constant11'
 * '<S158>/Constant1'
 */

/*
   @@ SYMBOL = Ctrl_Motor__Rs
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "Line-N resistance, ohms. All motor parameters used in sliding mode sensorless, torque calculation, psi estimator, RPM limiter, MTPA, IQ decoupling [Ohm]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "Ohm"
   @! CAN_INDEX = 0x310B
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1.46
   @@ END
 */
volatile real32_T Ctrl_Motor__Rs = 1.46F;

/* Referenced by:
 * '<S4>/Constant1'
 * '<S4>/Constant6'
 * '<S229>/Constant'
 * '<S126>/Constant3'
 */

/*
   @@ SYMBOL = Ctrl_Motor__Rth
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "(vector 2 elements) Motor thermal resistance betwen stator wires and temperature sensor. Use 0 to disable [K/W]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 2
   @@ UNIT = "K/W"
   @! CAN_INDEX = 0x310C
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = [0.3434106, 2.873673]
   @@ END
 */
volatile real32_T Ctrl_Motor__Rth[2] = { 0.343410611F, 2.87367296F } ;

/* Referenced by: '<S229>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Motor__Tau
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "(vector 2 elements) Motor time constants for thermal resistance [s]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 2
   @@ UNIT = "s"
   @! CAN_INDEX = 0x310D
   @! CAN_SUBINDEX = 0
   @! READ_LEVEL = 2
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = [91.76567, 4362.511]
   @@ END
 */
volatile real32_T Ctrl_Motor__Tau[2] = { 91.7656708F, 4362.51123F } ;

/* Referenced by: '<S229>/Constant3' */

/*
   @@ SYMBOL = Ctrl_Motor__TempFromRenabled
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "If temperature calculated form resistance is reported to output [bit]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_Motor__TempFromRenabled = true;

/* Referenced by: '<S223>/TempFromRenabled' */

/*
   @@ SYMBOL = Ctrl_Motor__TempFromRgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-1.0E+6 ... 1.0E+6]
   @@ DESCRIPTION = "k in euation Temp = k * Ud + n [C/V]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "C/V"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 23.6984
   @@ END
 */
volatile real32_T Ctrl_Motor__TempFromRgain = 23.6984F;

/* Referenced by: '<S269>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Motor__TempFromRoffset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-1.0E+6 ... 1.0E+6]
   @@ DESCRIPTION = "Iron loss factor based on square RPM [C]"
   @@ GROUP = Ctrl | Motor
   @@ DIMENSION = 1 1
   @@ UNIT = "C"
   @! CAN_INDEX = 0x310F
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = -288.7719
   @@ END
 */
volatile real32_T Ctrl_Motor__TempFromRoffset = -288.771912F;

/* Referenced by: '<S269>/Constant2' */

/*
   @@ SYMBOL = Ctrl_OpenLoop__Id
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Id in open loop mode start [Arms] [Arms]"
   @@ GROUP = Ctrl | OpenLoop
   @@ DIMENSION = 1 1
   @@ UNIT = "Arms"
   @! CAN_INDEX = 0x3170
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 2.0
   @@ END
 */
volatile real32_T Ctrl_OpenLoop__Id = 2.0F;

/* Referenced by:
 * '<S190>/Constant2'
 * '<S193>/Constant3'
 */

/*
   @@ SYMBOL = Ctrl_OpenLoop__RampIdRate
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "How fast rises/fall Id in align phase. Aligh phase durration is Ctrl_OpenLoop__Id / Ctrl_OpenLoop__RampIdRate [Arms/s]"
   @@ GROUP = Ctrl | OpenLoop
   @@ DIMENSION = 1 1
   @@ UNIT = "Arms/s"
   @! CAN_INDEX = 0x3170
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 20.0
   @@ END
 */
volatile real32_T Ctrl_OpenLoop__RampIdRate = 20.0F;

/* Referenced by: '<S190>/Constant' */

/*
   @@ SYMBOL = Ctrl_OpenLoop__RampRPMrate
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "How fast RPM rise durring start up ramp. Used also when HFI is active [rpm/s]"
   @@ GROUP = Ctrl | OpenLoop
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm/s"
   @! CAN_INDEX = 0x3170
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 400.0
   @@ END
 */
volatile real32_T Ctrl_OpenLoop__RampRPMrate = 400.0F;

/* Referenced by:
 * '<S6>/Constant2'
 * '<S192>/Constant'
 */

/*
   @@ SYMBOL = Ctrl_PowerLimit__BridgeTempHigh
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 120.0]
   @@ DESCRIPTION = "Bridge temperature above which power limit is changed from UpperLimit to MiddleLimit. [degC]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 95.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__BridgeTempHigh = 95.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__BridgeTempHigh' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__BridgeTempLow
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 120.0]
   @@ DESCRIPTION = "Bridge temperature below which power limit is changed from MiddleLimit to UpperLimit. [degC]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "degC"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 8
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 80.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__BridgeTempLow = 80.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__BridgeTempLow' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__DisableLimit
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1200.0]
   @@ DESCRIPTION = "Power at which power limit is disabled and power limit counter is started to decrease until it reaches to 0. [W]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 500.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__DisableLimit = 500.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__DisableLimit' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__Duration
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 500.0]
   @@ DESCRIPTION = "Power limit time duration in [s], for which electrical power between the Ctrl_Limits__PowerLimitUpper and Ctrl_Limits__PowerLimitLower is allowed. [s]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 180.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__Duration = 180.0F;

/* Referenced by: '<S21>/Ctrl_PowerLimit__Duration' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__DurationHyst
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 500.0]
   @@ DESCRIPTION = "Power limit time duration hysteresis in [s]. [s]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 10.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__DurationHyst = 10.0F;

/* Referenced by: '<S21>/Ctrl_PowerLimit__DurationHyst' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__LowerLimit
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1200.0]
   @@ DESCRIPTION = "Lower power limit in [W]. [W]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 700.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__LowerLimit = 700.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__LowerLimit' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__MiddleLimit
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1200.0]
   @@ DESCRIPTION = "Middle power limit in [W]. [W]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 750.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__MiddleLimit = 750.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__MiddleLimit' */

/*
   @@ SYMBOL = Ctrl_PowerLimit__UpperLimit
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1200.0]
   @@ DESCRIPTION = "Upper power limit in [W]. [W]"
   @@ GROUP = Ctrl | PowerLimit
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x3112
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 750.0
   @@ END
 */
volatile real32_T Ctrl_PowerLimit__UpperLimit = 750.0F;

/* Referenced by: '<S242>/Ctrl_PowerLimit__UpperLimit' */

/*
   @@ SYMBOL = Ctrl_Power__CompensationOffset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Electrical power compensation offset. [W]"
   @@ GROUP = Ctrl | Power
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x31B0
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 85.0
   @@ END
 */
volatile real32_T Ctrl_Power__CompensationOffset = 85.0F;

/* Referenced by: '<S223>/Offset' */

/*
   @@ SYMBOL = Ctrl_Power__CompensationSlope
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Electrical power compensation slope. []"
   @@ GROUP = Ctrl | Power
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31B0
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.2
   @@ END
 */
volatile real32_T Ctrl_Power__CompensationSlope = 0.2F;

/* Referenced by: '<S223>/Slope gain' */

/*
   @@ SYMBOL = Ctrl_Power__CompensationStart
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1500.0]
   @@ DESCRIPTION = "Electrical power at which compensation starts. [W]"
   @@ GROUP = Ctrl | Power
   @@ DIMENSION = 1 1
   @@ UNIT = "W"
   @! CAN_INDEX = 0x31B0
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 500.0
   @@ END
 */
volatile real32_T Ctrl_Power__CompensationStart = 500.0F;

/* Referenced by: '<S223>/Power in W' */

/*
   @@ SYMBOL = Ctrl_SensorlessCalculatedGain__ObserverF
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "Observer frequency setting. 4 is good starting value. Higher value will increase gains and reduce tracking error []"
   @@ GROUP = Ctrl | SensorlessCalculatedGain
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x316C
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 6.0
   @@ END
 */
volatile real32_T Ctrl_SensorlessCalculatedGain__ObserverF = 6.0F;

/* Referenced by: '<S4>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Sensorless__Active
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "1->sensorless angle used for control, 0->sensored angle not used for control, but HFI Uab injection and Id injection active, sliding mode and hfi algoriths running. Also HFI voltage is generated, set Ctrl_HFI__UabAmplitude to prevent this [bit]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_Sensorless__Active = true;

/* Referenced by: '<S118>/EN_sensorless' */

/*
   @@ SYMBOL = Ctrl_Sensorless__CalculationDelay
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-10.0 ... 10.0]
   @@ DESCRIPTION = "Sensorless claculation delay. Compensates also time lag of BEMF observer. Unit current loop sample times [sample_time]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "sample_time"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 10
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 1.5
   @@ END
 */
volatile real32_T Ctrl_Sensorless__CalculationDelay = 1.5F;

/* Referenced by: '<S22>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__Enabled
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "If sensorless algorithm  is executed [bit]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_Sensorless__Enabled = true;

/* Referenced by: '<S41>/EN_sensorless1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__EndRPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "End value of ramp RPM. Must be higher as Ctrl_Sensorless__ValidMaxRPM. If senzorless is not valid till this RPM, error will be triggered. Used also when accelerating with HFI. [rpm]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 14
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 800.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__EndRPM = 800.0F;

/* Referenced by: '<S118>/SenzorlessStateDigram' */

/*
   @@ SYMBOL = Ctrl_Sensorless__MaxRPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "Error detected if estimated RPM is above this. Protection agains overspeed. [rpm]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 17
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 22000.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__MaxRPM = 22000.0F;

/* Referenced by: '<S129>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__MinTimeInClose
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "Minimum time in closed loop [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 19
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Sensorless__MinTimeInClose = 0.1F;

/* Referenced by:
 * '<S17>/Constant16'
 * '<S118>/SenzorlessStateDigram'
 */

/*
   @@ SYMBOL = Ctrl_Sensorless__ObserverK1
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "K1 observer gain. Used in estimation Iab []"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 191.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ObserverK1 = 191.0F;

/* Referenced by: '<S126>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ObserverK2
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+8]
   @@ DESCRIPTION = "K2 observer gain. Used in estimation Eab []"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 1.271528E+6
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ObserverK2 = 1.271528E+6F;

/* Referenced by: '<S126>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__OmegaPostLPFtimeConstant
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Time constant of LPF filter for omega at quadrature PLL output. Adds time lag to omega and creates instability in velocity loop. Also sensorless BEMF criteria can fail if omega lags to much. Filtering partily prevents oscilation of Eab_est [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 9
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 2
   @! PERSIST = 1
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__OmegaPostLPFtimeConstant = 0.0F;

/* Referenced by: '<S32>/TimeConstant' */

/*
   @@ SYMBOL = Ctrl_Sensorless__PLLBWSpeedSwitch
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 18000.0]
   @@ DESCRIPTION = "Motor speed at which PLL bandwith is switched. [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 20
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 2000.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__PLLBWSpeedSwitch = 2000.0F;

/* Referenced by: '<S127>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__PLL_Bandwith
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Bandwith of PLL angle estimator which filters angle calculated from atan Eest [Hz]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "Hz"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 60.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__PLL_Bandwith = 60.0F;

/* Referenced by: '<S22>/Constant16' */

/*
   @@ SYMBOL = Ctrl_Sensorless__PLL_BandwithReduction
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Reduction of PLL bandwith if senzorless is not in CloseLoop mode. Use to slow down PLL with open loop and HFI start []"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 8
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.2
   @@ END
 */
volatile real32_T Ctrl_Sensorless__PLL_BandwithReduction = 0.2F;

/* Referenced by: '<S22>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Sensorless__TimeInHoldState
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "How long we are in hold state (constant Id after align), where Ud is measured for motor resistance calc. [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 21
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Sensorless__TimeInHoldState = 0.1F;

/* Referenced by: '<S17>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__TransitionTimeToClose
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Time for transition from open mode (HFI, open loop) to close loop (BEMF) [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.05
   @@ END
 */
volatile real32_T Ctrl_Sensorless__TransitionTimeToClose = 0.05F;

/* Referenced by: '<S191>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Sensorless__TransitionTimeToOpen
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Time for transition from close loop (BEMF observer) to open loop (open loop, HFI) [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.05
   @@ END
 */
volatile real32_T Ctrl_Sensorless__TransitionTimeToOpen = 0.05F;

/* Referenced by: '<S191>/Constant3' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidBEMFratioMax
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [1.0 ... 100.0]
   @@ DESCRIPTION = "Valid criteria: maximumu ratio betwen sensorless estimated and calculated from omega. Motor psi must be accurate []"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 13
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 3.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidBEMFratioMax = 3.0F;

/* Referenced by: '<S158>/Constant6' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidBEMFratioMin
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Valid criteria: minimum ratio betwen BEMF sensorless estimated and calculated from omega. 0= always valid. Motor psi must be accurate []"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 12
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.1
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidBEMFratioMin = 0.1F;

/* Referenced by: '<S158>/Constant5' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidLPFtimeConstant
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0]
   @@ DESCRIPTION = "Valid criteria:  time constant used for filtering PLL err and BEMF ratio [s]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 18
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 0.01
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidLPFtimeConstant = 0.01F;

/* Referenced by: '<S26>/TimeConstant' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidMaxRPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10000.0]
   @@ DESCRIPTION = "Valid criteria: sensorless is valid when RPM is greater as this [rpm]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 15
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 650.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidMaxRPM = 650.0F;

/* Referenced by:
 * '<S118>/SenzorlessStateDigram'
 * '<S129>/Constant4'
 */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidMinRPM
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 10000.0]
   @@ DESCRIPTION = "Valid criteria: sensorless is not valid when RPM is lower as this [rpm]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 16
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 300.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidMinRPM = 300.0F;

/* Referenced by: '<S129>/Constant3' */

/*
   @@ SYMBOL = Ctrl_Sensorless__ValidPLLerrMax
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 180.0]
   @@ DESCRIPTION = "Valid criteria: maximum  abs PLL err degree (low pass filtered) before declared sensorless is not valid. 0= always valid [deg]"
   @@ GROUP = Ctrl | Sensorless
   @@ DIMENSION = 1 1
   @@ UNIT = "deg"
   @! CAN_INDEX = 0x3160
   @! CAN_SUBINDEX = 11
   @! READ_LEVEL = 3
   @! WRITE_LEVEL = 3
   @! PERSIST = 1
   @! DEFAULT = 90.0
   @@ END
 */
volatile real32_T Ctrl_Sensorless__ValidPLLerrMax = 90.0F;

/* Referenced by: '<S161>/Constant' */

/*
   @@ SYMBOL = Ctrl_Stim1__Amplitude
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Amplitue of test pulse generator []"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Stim1__Amplitude = 0.0F;

/* Referenced by: '<S11>/Constant' */

/*
   @@ SYMBOL = Ctrl_Stim1__GenTarget
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = SLONG [0 ... 8]
   @@ DESCRIPTION = "Point where stimuli generator is inserted. Off =0, Ud=1, Uq=2, Id=3, Iq=4, Iq_ref=5, RPM_ref=6 , Theta (in radians) =7, FiledWeakenigTargetModulationFactor = 8 []"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0
   @@ END
 */
volatile int32_T Ctrl_Stim1__GenTarget = 0;

/* Referenced by:
 * '<S11>/Constant11'
 * '<S12>/Constant2'
 * '<S61>/Constant2'
 * '<S62>/Constant2'
 * '<S89>/Constant2'
 * '<S90>/Constant2'
 */

/*
   @@ SYMBOL = Ctrl_Stim1__Offset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-1.0E+6 ... 1.0E+6]
   @@ DESCRIPTION = "Offset of test pulse generator []"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Stim1__Offset = 0.0F;

/* Referenced by: '<S11>/Constant4' */

/*
   @@ SYMBOL = Ctrl_Stim1__Override
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "1->signal will override original value, 0-> signal will be added to original [bit]"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_Stim1__Override = true;

/* Referenced by: '<Root>/Constant6' */

/*
   @@ SYMBOL = Ctrl_Stim1__Period
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Period of test pulse generator [s]"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 1.0
   @@ END
 */
volatile real32_T Ctrl_Stim1__Period = 1.0F;

/* Referenced by: '<S11>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Stim1__SineOrRect
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "0-> rectengular pulses, 1-> sine pulses [bit]"
   @@ GROUP = Ctrl | Stim1
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31D0
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_Stim1__SineOrRect = false;

/* Referenced by: '<S11>/Constant2' */

/*
   @@ SYMBOL = Ctrl_Stim2__Amplitude
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Amplitue of test pulse generator []"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Stim2__Amplitude = 0.0F;

/* Referenced by: '<S11>/Constant8' */

/*
   @@ SYMBOL = Ctrl_Stim2__GenTarget
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = SLONG [0 ... 8]
   @@ DESCRIPTION = "Point where stimuli generator is inserted. Off =0, Ud=1, Uq=2, Id=3, Iq=4, Iq_ref=5, RPM_ref=6 , Theta (in radians) =7, FiledWeakenigTargetModulationFactor = 8 []"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0
   @@ END
 */
volatile int32_T Ctrl_Stim2__GenTarget = 0;

/* Referenced by:
 * '<S11>/Constant12'
 * '<S12>/Constant5'
 * '<S61>/Constant5'
 * '<S62>/Constant5'
 * '<S89>/Constant5'
 * '<S90>/Constant5'
 */

/*
   @@ SYMBOL = Ctrl_Stim2__Offset
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [-1.0E+6 ... 1.0E+6]
   @@ DESCRIPTION = "Offset of test pulse generator []"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = ""
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0.0
   @@ END
 */
volatile real32_T Ctrl_Stim2__Offset = 0.0F;

/* Referenced by: '<S11>/Constant9' */

/*
   @@ SYMBOL = Ctrl_Stim2__Override
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "1->signal will override original value, 0-> signal will be added to original [bit]"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 1
   @@ END
 */
volatile boolean_T Ctrl_Stim2__Override = true;

/* Referenced by: '<Root>/Constant1' */

/*
   @@ SYMBOL = Ctrl_Stim2__Period
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "Period of test pulse generator [s]"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = "s"
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 1.0
   @@ END
 */
volatile real32_T Ctrl_Stim2__Period = 1.0F;

/* Referenced by: '<S11>/Constant7' */

/*
   @@ SYMBOL = Ctrl_Stim2__SineOrRect
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = UBYTE [0 ... 1]
   @@ DESCRIPTION = "0-> rectengular pulses, 1-> sine pulses [bit]"
   @@ GROUP = Ctrl | Stim2
   @@ DIMENSION = 1 1
   @@ UNIT = "bit"
   @! CAN_INDEX = 0x31D1
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 1
   @! PERSIST = 0
   @! DEFAULT = 0
   @@ END
 */
volatile boolean_T Ctrl_Stim2__SineOrRect = false;

/* Referenced by: '<S11>/Constant6' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__Dgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "D gain [Am/(rpm/s)]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Am/(rpm/s)"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 3
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 8.0E-5
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__Dgain = 8.0E-5F;

/* Referenced by: '<S309>/Constant1' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__DpartLPF
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100000.0]
   @@ DESCRIPTION = "Corner frequency of LPF at D part [Hz]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Hz"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 4
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1000.0
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__DpartLPF = 1000.0F;

/* Referenced by:
 * '<S308>/Constant'
 * '<S308>/Constant2'
 */

/*
   @@ SYMBOL = Ctrl_VelocityReg__Igain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "I gain [Am/rpms]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Am/rpms"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 2
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.05
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__Igain = 0.05F;

/* Referenced by: '<S309>/Constant14' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__IgainHigh
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "I gain [Am/rpms]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Am/rpms"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 7
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.02
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__IgainHigh = 0.02F;

/* Referenced by: '<S309>/Constant11' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__Igain_LoadMeas
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1.0E+6]
   @@ DESCRIPTION = "I gain [Am/rpms]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Am/rpms"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 9
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.02
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__Igain_LoadMeas = 0.02F;

/* Referenced by: '<S309>/Constant16' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__Pgain
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Velocity regulator P gain. Input RPM (RPM), output torque (Nm) [A/rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A/rpm"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 1
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.003
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__Pgain = 0.003F;

/* Referenced by: '<S309>/Constant13' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__PgainHigh
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Velocity regulator P gain. Input RPM (RPM), output torque (Nm) [A/rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A/rpm"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 6
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.005
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__PgainHigh = 0.005F;

/* Referenced by: '<S309>/Constant12' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__Pgain_LoadMeas
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Velocity regulator P gain. Input RPM (RPM), output torque (Nm) [A/rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "A/rpm"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 8
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 0.003
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__Pgain_LoadMeas = 0.003F;

/* Referenced by: '<S309>/Constant15' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__SpeedLimFirst
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 18000.0]
   @@ DESCRIPTION = "First speed param limit [rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 10
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 2000.0
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__SpeedLimFirst = 2000.0F;

/* Referenced by: '<S309>/Constant2' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__SpeedLimSecond
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 18000.0]
   @@ DESCRIPTION = "Second speed param limit [rpm]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "rpm"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 11
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 2000.0
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__SpeedLimSecond = 2000.0F;

/* Referenced by: '<S309>/Constant3' */

/*
   @@ SYMBOL = Ctrl_VelocityReg__StartIq
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 1000.0]
   @@ DESCRIPTION = "Start Iq of velocity regulator when switching from open to close loop - used only in senzorless [Ap]"
   @@ GROUP = Ctrl | VelocityReg
   @@ DIMENSION = 1 1
   @@ UNIT = "Ap"
   @! CAN_INDEX = 0x3150
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 1.0
   @@ END
 */
volatile real32_T Ctrl_VelocityReg__StartIq = 1.0F;

/* Referenced by: '<S14>/Start torque' */

/*
   @@ SYMBOL = Ctrl_WM_AU_Ball_HPF_CornerF
   @@ A2L_TYPE = PARAMETER
   @@ DATA_TYPE = FLOAT [0.0 ... 100.0]
   @@ DESCRIPTION = "Corner frequency of second pole HPF used for AU ball detection [Hz]"
   @@ GROUP = Ctrl | WM | AU | Ball | HPF
   @@ DIMENSION = 1 1
   @@ UNIT = "Hz"
   @! CAN_INDEX = 0x3110
   @! CAN_SUBINDEX = 5
   @! READ_LEVEL = 4
   @! WRITE_LEVEL = 4
   @! PERSIST = 1
   @! DEFAULT = 10.0
   @@ END
 */
volatile real32_T Ctrl_WM_AU_Ball_HPF_CornerF = 10.0F;

/* Referenced by: '<S24>/Constant' */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
