/*
 * File: emDriveFOC_control_global.h
 *
 * Code generated for Simulink model 'emDriveFOC_control'.
 *
 * Model version                  : 12.12
 * Simulink Coder version         : 9.9 (R2023a) 19-Nov-2022
 * C/C++ source code generated on : Fri Nov 24 14:23:41 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_emDriveFOC_control_global_h_
#define RTW_HEADER_emDriveFOC_control_global_h_
#include "rtwtypes.h"
#include "emDriveFOC_control_types.h"

/* Exported data declaration */

/* FLOW2 exported parameter list */
volatile extern boolean_T Ctrl_ALTTestMode__Enable;/* Referenced by:
                                                    * '<S2>/Constant2'
                                                    * '<S5>/Constant8'
                                                    * '<S6>/Constant8'
                                                    * '<S46>/Constant8'
                                                    * '<S54>/Constant8'
                                                    * '<S118>/Constant2'
                                                    * '<S85>/Constant8'
                                                    */
volatile extern real32_T Ctrl_ALTTestMode__RPM;/* Referenced by: '<S46>/Constant1' */
volatile extern boolean_T Ctrl_AngleGenerator__Enable;/* Referenced by: '<S2>/Constant8' */
volatile extern real32_T Ctrl_AngleGenerator__Offset;/* Referenced by: '<S46>/Constant5' */
volatile extern real32_T Ctrl_AngleGenerator__RPM;/* Referenced by: '<S46>/Constant4' */
volatile extern real32_T Ctrl_CurrentReg_Calculated__Bandwith;/* Referenced by: '<S4>/Constant10' */
volatile extern real32_T Ctrl_CurrentReg__CalculationDelay;/* Referenced by: '<S58>/Constant7' */
volatile extern real32_T Ctrl_CurrentReg__DaxisIgain;/* Referenced by: '<S83>/Constant1' */
volatile extern real32_T Ctrl_CurrentReg__DaxisPgain;/* Referenced by: '<S83>/Constant' */
volatile extern real32_T Ctrl_CurrentReg__DecoupleIdIqEnable;/* Referenced by: '<S54>/Constant1' */
volatile extern boolean_T Ctrl_CurrentReg__DecouplePsi;/* Referenced by: '<S54>/Constant5' */
volatile extern real32_T Ctrl_CurrentReg__QaxisIgain;/* Referenced by: '<S84>/Constant1' */
volatile extern real32_T Ctrl_CurrentReg__QaxisPgain;/* Referenced by: '<S84>/Constant' */
volatile extern real32_T Ctrl_FieldWeak__Igain;/* Referenced by: '<S70>/Constant1' */
volatile extern real32_T Ctrl_FieldWeak__LPFtimeConstant;/* Referenced by: '<S28>/TimeConstant' */
volatile extern real32_T Ctrl_FieldWeak__Pgain;/* Referenced by: '<S70>/Constant' */
volatile extern real32_T Ctrl_FieldWeak__TargetModulationFactor;/* Referenced by: '<S65>/Constant2' */
volatile extern int32_T Ctrl_Gen__CurToVelRatio;/* Referenced by:
                                                 * '<S15>/Constant16'
                                                 * '<S286>/Constant3'
                                                 * '<S229>/Constant4'
                                                 * '<S270>/Constant1'
                                                 * '<S306>/Constant5'
                                                 */
volatile extern real32_T Ctrl_Gen__Fcur;/* Referenced by:
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
volatile extern real32_T Ctrl_Gen__MaxRPMcalculations;/* Referenced by: '<S127>/Constant3' */
volatile extern boolean_T Ctrl_IdtoIq_Switch_Enable;/* Referenced by: '<S38>/Constant2' */
volatile extern boolean_T Ctrl_Limits_OverrideCloseLoopRamp;/* Referenced by: '<S6>/Constant4' */
volatile extern real32_T Ctrl_Limits__BridgeTemperature;/* Referenced by: '<S237>/Constant4' */
volatile extern real32_T Ctrl_Limits__BridgeTemperatureIgain;/* Referenced by: '<S237>/Constant1' */
volatile extern real32_T Ctrl_Limits__BridgeTemperaturePgain;/* Referenced by: '<S237>/Constant' */
volatile extern real32_T Ctrl_Limits__CloseLoopRamp;/* Referenced by: '<S6>/Constant5' */
volatile extern real32_T Ctrl_Limits__IdMin;/* Referenced by:
                                             * '<S52>/Constant6'
                                             * '<S53>/Constant6'
                                             */
volatile extern real32_T Ctrl_Limits__IqMax;/* Referenced by: '<S3>/Iq_max' */
volatile extern real32_T Ctrl_Limits__IsMax;/* Referenced by: '<S3>/Is_max' */
volatile extern real32_T Ctrl_Limits__IsMaxSpeedLimit;/* Referenced by: '<S3>/Is_max2' */
volatile extern real32_T Ctrl_Limits__IsMaxStartup;/* Referenced by: '<S3>/Is_max1' */
volatile extern real32_T Ctrl_Limits__MaximumRampLowRpm;/* Referenced by: '<S6>/Constant6' */
volatile extern real32_T Ctrl_Limits__MotorTemperature;/* Referenced by: '<S226>/Constant4' */
volatile extern real32_T Ctrl_Limits__MotorTemperatureIgain;/* Referenced by: '<S226>/Constant1' */
volatile extern real32_T Ctrl_Limits__MotorTemperaturePgain;/* Referenced by: '<S226>/Constant' */
volatile extern real32_T Ctrl_Limits__OpenLoopRampOverrideValue;/* Referenced by: '<S6>/Constant3' */
volatile extern real32_T Ctrl_Limits__OutletTemperature;/* Referenced by: '<S254>/Constant4' */
volatile extern real32_T Ctrl_Limits__OutletTemperatureIgain;/* Referenced by: '<S254>/Constant1' */
volatile extern real32_T Ctrl_Limits__OutletTemperaturePgain;/* Referenced by: '<S254>/Constant' */
volatile extern real32_T Ctrl_Limits__PowerLimitIgain;/* Referenced by: '<S241>/Constant1' */
volatile extern real32_T Ctrl_Limits__PowerLimitPgain;/* Referenced by: '<S241>/Constant' */
volatile extern real32_T Ctrl_Limits__RPMnegative;/* Referenced by: '<S6>/Constant1' */
volatile extern real32_T Ctrl_Limits__RPMpositive;/* Referenced by:
                                                   * '<S6>/Constant13'
                                                   * '<S22>/Constant6'
                                                   */
volatile extern real32_T Ctrl_Limits__RPMramp;/* Referenced by: '<S286>/Constant' */
volatile extern real32_T Ctrl_Limits__UdcMaxToTorque;/* Referenced by: '<S220>/Constant5' */
volatile extern real32_T Ctrl_Limits__UdcMaxToTorqueGain;/* Referenced by: '<S220>/Constant4' */
volatile extern real32_T Ctrl_Measurment__LPF_TimeConst;/* Referenced by: '<S30>/TimeConstant' */
volatile extern real32_T Ctrl_Measurment__Pel_LPF_TimeConst;
                                      /* Referenced by: '<S284>/TimeConstant' */
volatile extern real32_T Ctrl_Mod__DeadTimeComp;/* Referenced by: '<S55>/Constant' */
volatile extern real32_T Ctrl_Mod__MinOffTimeHighSide;
                                 /* Referenced by: '<S20>/MinOffTimeHighSide' */
volatile extern real32_T Ctrl_Mod__MinOffTimeLowSide;
                                  /* Referenced by: '<S20>/MinOffTimeLowSide' */
volatile extern real32_T Ctrl_Mod__MinOffTimeSecondHighSide;
                           /* Referenced by: '<S20>/MinOffTimeSecondHighSide' */
volatile extern real32_T Ctrl_Mod__MinPulseTime;
                                   /* Referenced by: '<S20>/MinimumPulseTime' */
volatile extern real32_T Ctrl_Mod__PWMOffset;/* Referenced by: '<S44>/Gain' */
volatile extern real32_T Ctrl_Motor__IdToIqRatio;/* Referenced by: '<S66>/Constant2' */
volatile extern real32_T Ctrl_Motor__InitialTempRise[2];/* Referenced by: '<S247>/Constant3' */
volatile extern real32_T Ctrl_Motor__IronLossCoef;/* Referenced by: '<S229>/Constant5' */
volatile extern real32_T Ctrl_Motor__Ld;/* Referenced by:
                                         * '<S4>/Constant2'
                                         * '<S4>/Constant7'
                                         * '<S23>/Constant1'
                                         * '<S57>/Constant1'
                                         * '<S126>/Constant5'
                                         */
volatile extern real32_T Ctrl_Motor__Lq;/* Referenced by:
                                         * '<S4>/Constant8'
                                         * '<S23>/Constant'
                                         * '<S57>/Constant2'
                                         * '<S126>/Constant6'
                                         */
volatile extern real32_T Ctrl_Motor__Np;/* Referenced by:
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
volatile extern real32_T Ctrl_Motor__Psi;/* Referenced by:
                                          * '<S57>/Constant11'
                                          * '<S158>/Constant1'
                                          */
volatile extern real32_T Ctrl_Motor__Rs;/* Referenced by:
                                         * '<S4>/Constant1'
                                         * '<S4>/Constant6'
                                         * '<S229>/Constant'
                                         * '<S126>/Constant3'
                                         */
volatile extern real32_T Ctrl_Motor__Rth[2];/* Referenced by: '<S229>/Constant2' */
volatile extern real32_T Ctrl_Motor__Tau[2];/* Referenced by: '<S229>/Constant3' */
volatile extern boolean_T Ctrl_Motor__TempFromRenabled;
                                  /* Referenced by: '<S223>/TempFromRenabled' */
volatile extern real32_T Ctrl_Motor__TempFromRgain;/* Referenced by: '<S269>/Constant1' */
volatile extern real32_T Ctrl_Motor__TempFromRoffset;/* Referenced by: '<S269>/Constant2' */
volatile extern real32_T Ctrl_OpenLoop__Id;/* Referenced by:
                                            * '<S190>/Constant2'
                                            * '<S193>/Constant3'
                                            */
volatile extern real32_T Ctrl_OpenLoop__RampIdRate;/* Referenced by: '<S190>/Constant' */
volatile extern real32_T Ctrl_OpenLoop__RampRPMrate;/* Referenced by:
                                                     * '<S6>/Constant2'
                                                     * '<S192>/Constant'
                                                     */
volatile extern real32_T Ctrl_PowerLimit__BridgeTempHigh;
                   /* Referenced by: '<S242>/Ctrl_PowerLimit__BridgeTempHigh' */
volatile extern real32_T Ctrl_PowerLimit__BridgeTempLow;
                    /* Referenced by: '<S242>/Ctrl_PowerLimit__BridgeTempLow' */
volatile extern real32_T Ctrl_PowerLimit__DisableLimit;
                     /* Referenced by: '<S242>/Ctrl_PowerLimit__DisableLimit' */
volatile extern real32_T Ctrl_PowerLimit__Duration;
                          /* Referenced by: '<S21>/Ctrl_PowerLimit__Duration' */
volatile extern real32_T Ctrl_PowerLimit__DurationHyst;
                      /* Referenced by: '<S21>/Ctrl_PowerLimit__DurationHyst' */
volatile extern real32_T Ctrl_PowerLimit__LowerLimit;
                       /* Referenced by: '<S242>/Ctrl_PowerLimit__LowerLimit' */
volatile extern real32_T Ctrl_PowerLimit__MiddleLimit;
                      /* Referenced by: '<S242>/Ctrl_PowerLimit__MiddleLimit' */
volatile extern real32_T Ctrl_PowerLimit__UpperLimit;
                       /* Referenced by: '<S242>/Ctrl_PowerLimit__UpperLimit' */
volatile extern real32_T Ctrl_Power__CompensationOffset;/* Referenced by: '<S223>/Offset' */
volatile extern real32_T Ctrl_Power__CompensationSlope;/* Referenced by: '<S223>/Slope gain' */
volatile extern real32_T Ctrl_Power__CompensationStart;/* Referenced by: '<S223>/Power in W' */
volatile extern real32_T Ctrl_SensorlessCalculatedGain__ObserverF;/* Referenced by: '<S4>/Constant4' */
volatile extern boolean_T Ctrl_Sensorless__Active;
                                     /* Referenced by: '<S118>/EN_sensorless' */
volatile extern real32_T Ctrl_Sensorless__CalculationDelay;/* Referenced by: '<S22>/Constant1' */
volatile extern boolean_T Ctrl_Sensorless__Enabled;
                                     /* Referenced by: '<S41>/EN_sensorless1' */
volatile extern real32_T Ctrl_Sensorless__EndRPM;
                             /* Referenced by: '<S118>/SenzorlessStateDigram' */
volatile extern real32_T Ctrl_Sensorless__MaxRPM;/* Referenced by: '<S129>/Constant1' */
volatile extern real32_T Ctrl_Sensorless__MinTimeInClose;/* Referenced by:
                                                          * '<S17>/Constant16'
                                                          * '<S118>/SenzorlessStateDigram'
                                                          */
volatile extern real32_T Ctrl_Sensorless__ObserverK1;/* Referenced by: '<S126>/Constant4' */
volatile extern real32_T Ctrl_Sensorless__ObserverK2;/* Referenced by: '<S126>/Constant1' */
volatile extern real32_T Ctrl_Sensorless__OmegaPostLPFtimeConstant;/* Referenced by: '<S32>/TimeConstant' */
volatile extern real32_T Ctrl_Sensorless__PLLBWSpeedSwitch;/* Referenced by: '<S127>/Constant1' */
volatile extern real32_T Ctrl_Sensorless__PLL_Bandwith;/* Referenced by: '<S22>/Constant16' */
volatile extern real32_T Ctrl_Sensorless__PLL_BandwithReduction;/* Referenced by: '<S22>/Constant2' */
volatile extern real32_T Ctrl_Sensorless__TimeInHoldState;/* Referenced by: '<S17>/Constant1' */
volatile extern real32_T Ctrl_Sensorless__TransitionTimeToClose;/* Referenced by: '<S191>/Constant1' */
volatile extern real32_T Ctrl_Sensorless__TransitionTimeToOpen;/* Referenced by: '<S191>/Constant3' */
volatile extern real32_T Ctrl_Sensorless__ValidBEMFratioMax;/* Referenced by: '<S158>/Constant6' */
volatile extern real32_T Ctrl_Sensorless__ValidBEMFratioMin;/* Referenced by: '<S158>/Constant5' */
volatile extern real32_T Ctrl_Sensorless__ValidLPFtimeConstant;/* Referenced by: '<S26>/TimeConstant' */
volatile extern real32_T Ctrl_Sensorless__ValidMaxRPM;/* Referenced by:
                                                       * '<S118>/SenzorlessStateDigram'
                                                       * '<S129>/Constant4'
                                                       */
volatile extern real32_T Ctrl_Sensorless__ValidMinRPM;/* Referenced by: '<S129>/Constant3' */
volatile extern real32_T Ctrl_Sensorless__ValidPLLerrMax;/* Referenced by: '<S161>/Constant' */
volatile extern real32_T Ctrl_Stim1__Amplitude;/* Referenced by: '<S11>/Constant' */
volatile extern int32_T Ctrl_Stim1__GenTarget;/* Referenced by:
                                               * '<S11>/Constant11'
                                               * '<S12>/Constant2'
                                               * '<S61>/Constant2'
                                               * '<S62>/Constant2'
                                               * '<S89>/Constant2'
                                               * '<S90>/Constant2'
                                               */
volatile extern real32_T Ctrl_Stim1__Offset;/* Referenced by: '<S11>/Constant4' */
volatile extern boolean_T Ctrl_Stim1__Override;/* Referenced by: '<Root>/Constant6' */
volatile extern real32_T Ctrl_Stim1__Period;/* Referenced by: '<S11>/Constant1' */
volatile extern boolean_T Ctrl_Stim1__SineOrRect;/* Referenced by: '<S11>/Constant2' */
volatile extern real32_T Ctrl_Stim2__Amplitude;/* Referenced by: '<S11>/Constant8' */
volatile extern int32_T Ctrl_Stim2__GenTarget;/* Referenced by:
                                               * '<S11>/Constant12'
                                               * '<S12>/Constant5'
                                               * '<S61>/Constant5'
                                               * '<S62>/Constant5'
                                               * '<S89>/Constant5'
                                               * '<S90>/Constant5'
                                               */
volatile extern real32_T Ctrl_Stim2__Offset;/* Referenced by: '<S11>/Constant9' */
volatile extern boolean_T Ctrl_Stim2__Override;/* Referenced by: '<Root>/Constant1' */
volatile extern real32_T Ctrl_Stim2__Period;/* Referenced by: '<S11>/Constant7' */
volatile extern boolean_T Ctrl_Stim2__SineOrRect;/* Referenced by: '<S11>/Constant6' */
volatile extern real32_T Ctrl_VelocityReg__Dgain;/* Referenced by: '<S309>/Constant1' */
volatile extern real32_T Ctrl_VelocityReg__DpartLPF;/* Referenced by:
                                                     * '<S308>/Constant'
                                                     * '<S308>/Constant2'
                                                     */
volatile extern real32_T Ctrl_VelocityReg__Igain;/* Referenced by: '<S309>/Constant14' */
volatile extern real32_T Ctrl_VelocityReg__IgainHigh;/* Referenced by: '<S309>/Constant11' */
volatile extern real32_T Ctrl_VelocityReg__Igain_LoadMeas;/* Referenced by: '<S309>/Constant16' */
volatile extern real32_T Ctrl_VelocityReg__Pgain;/* Referenced by: '<S309>/Constant13' */
volatile extern real32_T Ctrl_VelocityReg__PgainHigh;/* Referenced by: '<S309>/Constant12' */
volatile extern real32_T Ctrl_VelocityReg__Pgain_LoadMeas;/* Referenced by: '<S309>/Constant15' */
volatile extern real32_T Ctrl_VelocityReg__SpeedLimFirst;/* Referenced by: '<S309>/Constant2' */
volatile extern real32_T Ctrl_VelocityReg__SpeedLimSecond;/* Referenced by: '<S309>/Constant3' */
volatile extern real32_T Ctrl_VelocityReg__StartIq;/* Referenced by: '<S14>/Start torque' */
volatile extern real32_T Ctrl_WM_AU_Ball_HPF_CornerF;/* Referenced by: '<S24>/Constant' */

/* FLOW2 exported signal list */
volatile extern real32_T Ctrl_ALTTestMode_StatusUdq[2];/* '<S96>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_Calculated__DaxisIgain;/* '<S262>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_Calculated__DaxisPgain;/* '<S261>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_Calculated__QaxisIgain;/* '<S264>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_Calculated__QaxisPgain;/* '<S263>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusIabc[3];/* '<S43>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusId[4];/* '<S69>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusIdqAct[2];/* '<S88>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusIdqRef[2];/* '<S86>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusIqmax;/* '<S80>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusModFactor;/* '<S68>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusRpm;/* '<S60>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusTheta;/* '<S59>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusUabc[3];/* '<S105>/Signal Conversion' */
volatile extern real32_T Ctrl_CurrentReg_StatusUdq[4];/* '<S87>/Signal Conversion' */
volatile extern int32_T Ctrl_Doc_FW__Date;/* '<Root>/FW_date' */
volatile extern int32_T Ctrl_Doc_FW__Version;/* '<Root>/Add' */
volatile extern uint8_T Ctrl_Doc_version[3];/* '<S8>/Signal Conversion' */
volatile extern real32_T Ctrl_FieldWeak_StatusId[2];/* '<S75>/Signal Conversion' */
volatile extern real32_T Ctrl_FieldWeak_StatusUs[3];/* '<S76>/Signal Conversion' */
volatile extern real32_T Ctrl_Gen_Stat__Iq_foc;/* '<S221>/Signal Conversion' */
volatile extern real32_T Ctrl_Gen_Stat__MotorWireTemp;/* '<S234>/Signal Conversion' */
volatile extern real32_T Ctrl_Gen_Stat__RPMref;/* '<S7>/Signal Conversion' */
volatile extern boolean_T Ctrl_Gen_Stat__RunStopIn;/* '<S10>/Signal Conversion' */
volatile extern boolean_T Ctrl_Gen_Stat__RunStopOut;/* '<S9>/Signal Conversion' */
volatile extern real32_T Ctrl_Gen_Stat__TemperatureFromR;/* '<S277>/Signal Conversion' */
volatile extern real32_T Ctrl_Gen_Stat__Udc;/* '<S222>/Signal Conversion' */
volatile extern boolean_T Ctrl_Limits_StatusDisIntegration[5];/* '<S292>/Signal Conversion' */
volatile extern real32_T Ctrl_Limits_StatusIsMax;/* '<S233>/Signal Conversion' */
volatile extern real32_T Ctrl_Limits_StatusLim[3];/* '<S232>/Signal Conversion' */
volatile extern real32_T Ctrl_Limits_StatusRPMramp[2];/* '<S290>/Signal Conversion' */
volatile extern real32_T Ctrl_Limits_StatusRampIn;/* '<S291>/Signal Conversion' */
volatile extern real32_T Ctrl_Limits_StatusUdcMaxToIq;/* '<S225>/Signal Conversion' */
volatile extern real32_T Ctrl_Measurments__IphRms;/* '<S274>/Signal Conversion' */
volatile extern real32_T Ctrl_Measurments__Pel;/* '<S275>/Signal Conversion' */
volatile extern real32_T Ctrl_Measurments__RPM;/* '<S276>/Signal Conversion' */
volatile extern real32_T Ctrl_Measurments__UphRms;/* '<S273>/Signal Conversion' */
volatile extern real32_T Ctrl_Mod_StatusPWM[3];/* '<S42>/Signal Conversion' */
volatile extern real32_T Ctrl_Motor__CopperLoss;/* '<S248>/Signal Conversion' */
volatile extern real32_T Ctrl_Motor__IronLoss;/* '<S249>/Signal Conversion' */
volatile extern real32_T Ctrl_Motor__TotalLoss;/* '<S250>/Signal Conversion' */
volatile extern real32_T Ctrl_OpenLoop_StatusRPMopen;/* '<S187>/Signal Conversion' */
volatile extern int32_T Ctrl_PowerLimit__Counter;/* '<S242>/PowerLimitStateMachine' */
volatile extern real32_T Ctrl_PowerLimit__LimitStatus;/* '<S242>/PowerLimitStateMachine' */
volatile extern int32_T Ctrl_PowerLimit__State;/* '<S242>/PowerLimitStateMachine' */
volatile extern real32_T Ctrl_PowerLimit__StatusActive;/* '<S231>/Signal Conversion' */
volatile extern real32_T Ctrl_PowerLimit__StatusLimitIq;/* '<S223>/Product4' */
volatile extern real32_T Ctrl_Power__CompensatedPower;/* '<S223>/Switch1' */
volatile extern real32_T Ctrl_SensorlessCalculatedGain__K1;/* '<S265>/Signal Conversion' */
volatile extern real32_T Ctrl_SensorlessCalculatedGain__K2;/* '<S266>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_CriteriaBEMF;/* '<S164>/Signal Conversion' */
volatile extern int32_T Ctrl_Sensorless_StatusBEMF_CriteriaFlags;/* '<S165>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_CriteriaPLL;/* '<S163>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_EabEst[2];/* '<S151>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_IabErr[2];/* '<S153>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_IabEst[2];/* '<S152>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_IabIn[2];/* '<S133>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_PLL_Err;/* '<S166>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_RPM[2];/* '<S155>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_Theta[3];/* '<S154>/Signal Conversion' */
volatile extern real32_T Ctrl_Sensorless_StatusBEMF_UabIn[2];/* '<S134>/Signal Conversion' */
volatile extern int32_T Ctrl_Sensorless_Status__LastNonErrorState;/* '<S188>/Signal Conversion' */
volatile extern int32_T Ctrl_Sensorless_Status__LatchedCriteriaFlags;/* '<S189>/Signal Conversion' */
volatile extern int32_T Ctrl_Sensorless_Status__State;/* '<S186>/Signal Conversion' */
volatile extern real32_T Ctrl_Stim_Status[2];/* '<S296>/Signal Conversion' */
volatile extern boolean_T Ctrl_VelocityReg_StatusDissableIntegration;/* '<S304>/Signal Conversion' */
volatile extern real32_T Ctrl_VelocityReg_StatusIq[7];/* '<S303>/Signal Conversion' */
volatile extern real32_T Ctrl_VelocityReg_StatusRPM[3];/* '<S301>/Signal Conversion' */
volatile extern boolean_T Ctrl_VelocityReg_StatusTracking;/* '<S305>/Signal Conversion' */

#endif                             /* RTW_HEADER_emDriveFOC_control_global_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
