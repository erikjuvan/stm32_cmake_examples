/*
 * File: emDriveFOC_control.h
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

#ifndef RTW_HEADER_emDriveFOC_control_h_
#define RTW_HEADER_emDriveFOC_control_h_
#ifndef emDriveFOC_control_COMMON_INCLUDES_
#define emDriveFOC_control_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* emDriveFOC_control_COMMON_INCLUDES_ */

#include "emDriveFOC_control_types.h"

/* Includes for objects with custom storage classes */
#include "emDriveFOC_control_global.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmCounterLimit
#define rtmCounterLimit(rtm, idx)      ((rtm)->Timing.TaskCounters.cLimit[(idx)])
#endif

#ifndef rtmStepTask
#define rtmStepTask(rtm, idx)          ((rtm)->Timing.TaskCounters.TID[(idx)] == 0)
#endif

#ifndef rtmTaskCounter
#define rtmTaskCounter(rtm, idx)       ((rtm)->Timing.TaskCounters.TID[(idx)])
#endif

/* user code (top of header file) */
#include "emDriveTargetSpecific.h"

/* Block signals and states (default storage) for system '<S54>/StimUd' */
typedef struct {
  volatile real32_T RateTransition_Buffer0;/* '<S89>/Rate Transition' */
  volatile real32_T RateTransition1_Buffer0;/* '<S89>/Rate Transition1' */
} DW_StimUd_emDriveFOC_control_T;

/* Block signals and states (default storage) for system '<S125>/Run.IdRamp' */
typedef struct {
  real32_T Delay_DSTATE;               /* '<S195>/Delay' */
  boolean_T DelayInput1_DSTATE;        /* '<S194>/Delay Input1' */
  boolean_T icLoad;                    /* '<S195>/Delay' */
} DW_RunIdRamp_emDriveFOC_contr_T;

/* Block signals and states (default storage) for system '<S125>/Run.RPM_ramp' */
typedef struct {
  real32_T Delay_DSTATE;               /* '<S215>/Delay' */
  real32_T Memory_PreviousInput;       /* '<S214>/Memory' */
  boolean_T DelayInput1_DSTATE;        /* '<S213>/Delay Input1' */
  boolean_T icLoad;                    /* '<S215>/Delay' */
} DW_RunRPM_ramp_emDriveFOC_con_T;

/* Block signals and states (default storage) for system '<S125>/Run.PosModeMix' */
typedef struct {
  real32_T UnitDelay_DSTATE;           /* '<S204>/Unit Delay' */
  real32_T UnitDelay1_DSTATE;          /* '<S204>/Unit Delay1' */
  real32_T Delay_DSTATE;               /* '<S201>/Delay' */
  boolean_T DelayInput1_DSTATE;        /* '<S200>/Delay Input1' */
  boolean_T icLoad;                    /* '<S201>/Delay' */
} DW_RunPosModeMix_emDriveFOC_c_T;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_RunPosModeMix_emDriveFOC_c_T RunPosModeMix;/* '<S125>/Run.PosModeMix' */
  DW_RunRPM_ramp_emDriveFOC_con_T RunRPM_ramp;/* '<S125>/Run.RPM_ramp' */
  DW_RunIdRamp_emDriveFOC_contr_T RunIdRamp;/* '<S125>/Run.IdRamp' */
  DW_StimUd_emDriveFOC_control_T StimIq;/* '<S38>/StimIq' */
  DW_StimUd_emDriveFOC_control_T StimId;/* '<S38>/StimId' */
  DW_StimUd_emDriveFOC_control_T StrimUq;/* '<S54>/StrimUq' */
  DW_StimUd_emDriveFOC_control_T StimUd;/* '<S54>/StimUd' */
  tyBusPos BusConversion_InsertedFor_Senzo;
  tyBusPos PosOut;                     /* '<S118>/SenzorlessStateDigram' */
  tyBusPos PosOutOpen;                 /* '<S118>/SenzorlessStateDigram' */
  tyBusPos InitPos_m;                  /* '<S118>/SenzorlessStateDigram' */
  real_T PLL_SlowEnable;               /* '<S118>/SenzorlessStateDigram' */
  real_T UnitDelay7_DSTATE;            /* '<S118>/Unit Delay7' */
  real32_T Switch1_i[5];               /* '<S267>/Switch1' */
  real32_T Delay_DSTATE_du[5];         /* '<S267>/Delay' */
  real32_T Delay_DSTATE_dx[2];         /* '<S251>/Delay' */
  real32_T UnitDelay1_DSTATE_n[2];     /* '<S2>/Unit Delay1' */
  real32_T UnitDelay_DSTATE_b[2];      /* '<S131>/Unit Delay' */
  real32_T UnitDelay_DSTATE_f[2];      /* '<S132>/Unit Delay' */
  real32_T UnitDelay1_DSTATE_m[2];     /* '<S38>/Unit Delay1' */
  real32_T RateTransition2;            /* '<Root>/Rate Transition2' */
  real32_T RateTransition8;            /* '<Root>/Rate Transition8' */
  real32_T RPM_ramp_track_value;       /* '<Root>/Rate Transition3' */
  real32_T omega_r;                    /* '<Root>/Rate Transition3' */
  real32_T IqMax;                      /* '<Root>/Rate Transition3' */
  real32_T TmpRTBAtTemperatureAndPowerLimi;/* '<S3>/Iq_max' */
  real32_T Constant11;                 /* '<S4>/Constant11' */
  real32_T RPMtoOmegaE;                /* '<S4>/RPMtoOmegaE' */
  real32_T OmegaEtoRPM;                /* '<S4>/OmegaEtoRPM' */
  real32_T Switch2;                    /* '<S313>/Switch2' */
  real32_T RPM_out;                    /* '<S6>/Switch2' */
  real32_T Uph;                        /* '<S268>/Gain' */
  real32_T Iph;                        /* '<S268>/Gain1' */
  real32_T RPM;                        /* '<S268>/Product2' */
  real32_T Switch1_h;                  /* '<S283>/Switch1' */
  real32_T Min;                        /* '<S223>/Min' */
  real32_T Switch2_h;                  /* '<S224>/Switch2' */
  real32_T theta_r;                    /* '<S2>/Switch1' */
  real32_T omega_r_c;                  /* '<S2>/Switch1' */
  real32_T DotProduct;                 /* '<S45>/Dot Product' */
  real32_T DotProduct1;                /* '<S45>/Dot Product1' */
  real32_T theta_r_d;                  /* '<S41>/Merge' */
  real32_T omega_r_l;                  /* '<S41>/Merge' */
  real32_T Iq;                         /* '<S41>/Merge' */
  real32_T Id;                         /* '<S41>/Merge' */
  real32_T RPM_h;                      /* '<S41>/Merge' */
  real32_T IdOut;                      /* '<S118>/SenzorlessStateDigram' */
  real32_T TrefOut;                    /* '<S118>/SenzorlessStateDigram' */
  real32_T RPM_Open;                   /* '<S118>/SenzorlessStateDigram' */
  real32_T Sign;                       /* '<S122>/Sign' */
  real32_T RateLimiter;                /* '<S116>/Rate Limiter' */
  real32_T omega_r_b;
  real32_T RPM_g;
  real32_T Q;                          /* '<S53>/Pitagora_calc2' */
  real32_T Product;                    /* '<S46>/Product' */
  real32_T Gain;                       /* '<S46>/Gain' */
  real32_T MinTimeInCloseLoop;         /* '<S17>/Product' */
  real32_T TimeInHold;                 /* '<S17>/Product1' */
  real32_T MinOffTimeHighSideF;        /* '<S20>/Add7' */
  real32_T Ctrl_Mod__MinOffTimeSecondHighS;/* '<S20>/Add9' */
  real32_T MinOffTimeLowSideF;         /* '<S20>/Gain1' */
  real32_T MinPulseTimeF;              /* '<S20>/Gain7' */
  real32_T Constant17;                 /* '<S22>/Constant17' */
  real32_T Gain4;                      /* '<S22>/Gain4' */
  real32_T Delay;                      /* '<S22>/Product1' */
  real32_T K1;                         /* '<S22>/ThetaPLLCalcK2' */
  real32_T K2;                         /* '<S22>/ThetaPLLCalcK2' */
  real32_T K1_g;                       /* '<S22>/ThetaPLLCalcK1' */
  real32_T K2_g;                       /* '<S22>/ThetaPLLCalcK1' */
  real32_T alpha;                      /* '<S32>/MATLAB Function' */
  real32_T alpha_c;                    /* '<S30>/MATLAB Function' */
  real32_T alpha_g;                    /* '<S28>/MATLAB Function' */
  real32_T alpha_b;                    /* '<S26>/MATLAB Function' */
  real32_T Gain2;                      /* '<S3>/Gain2' */
  real32_T RPM_act;                    /* '<S14>/Product' */
  real32_T Product1;                   /* '<S286>/Product1' */
  real32_T Product_h;                  /* '<S286>/Product' */
  real32_T Delay1_DSTATE;              /* '<S307>/Delay1' */
  real32_T Delay_DSTATE;               /* '<S307>/Delay' */
  real32_T UnitDelay1_DSTATE;          /* '<S307>/Unit Delay1' */
  real32_T Delay_DSTATE_a;             /* '<S312>/Delay' */
  real32_T UnitDelay_DSTATE;           /* '<S6>/Unit Delay' */
  real32_T Delay_DSTATE_o;             /* '<S6>/Delay' */
  real32_T Delay_DSTATE_d;             /* '<S288>/Delay' */
  real32_T UnitDelay4_DSTATE;          /* '<S280>/Unit Delay4' */
  real32_T UnitDelay4_DSTATE_o;        /* '<S281>/Unit Delay4' */
  real32_T UnitDelay_DSTATE_l;         /* '<S269>/Unit Delay' */
  real32_T Delay_DSTATE_m;             /* '<S283>/Delay' */
  real32_T Delay_DSTATE_b;             /* '<S229>/Delay' */
  real32_T DelayOneStep_DSTATE;        /* '<S228>/Delay One Step' */
  real32_T Delay_DSTATE_duo;           /* '<S241>/Delay' */
  real32_T Delay_DSTATE_n;             /* '<S254>/Delay' */
  real32_T Delay_DSTATE_h;             /* '<S237>/Delay' */
  real32_T Delay_DSTATE_nd;            /* '<S226>/Delay' */
  real32_T UnitDelay_DSTATE_o;         /* '<S220>/Unit Delay' */
  real32_T UnitDelay2_2_DSTATE;        /* '<S118>/Unit Delay2' */
  real32_T UnitDelay3_DSTATE;          /* '<S122>/Unit Delay3' */
  real32_T DelayOneStep_DSTATE_m;      /* '<S127>/Delay One Step' */
  real32_T Delay_DSTATE_hv;            /* '<S183>/Delay' */
  real32_T Delay_DSTATE_f;             /* '<S168>/Delay' */
  real32_T Delay_DSTATE_oc;            /* '<S143>/Delay' */
  real32_T UnitDelay_DSTATE_fm;        /* '<S144>/Unit Delay' */
  real32_T Delay_DSTATE_oq;            /* '<S136>/Delay' */
  real32_T UnitDelay_DSTATE_c;         /* '<S116>/Unit Delay' */
  real32_T UnitDelay2_DSTATE;          /* '<S38>/Unit Delay2' */
  real32_T UnitDelay5_DSTATE;          /* '<S38>/Unit Delay5' */
  real32_T Delay_DSTATE_e;             /* '<S84>/Delay' */
  real32_T UnitDelay1_DSTATE_i;        /* '<S84>/Unit Delay1' */
  real32_T Delay_DSTATE_g;             /* '<S83>/Delay' */
  real32_T UnitDelay1_DSTATE_b;        /* '<S83>/Unit Delay1' */
  real32_T UnitDelay3_DSTATE_a;        /* '<S53>/Unit Delay3' */
  real32_T Delay_DSTATE_hi;            /* '<S71>/Delay' */
  real32_T Delay_DSTATE_k;             /* '<S70>/Delay' */
  real32_T UnitDelay1_DSTATE_mx;       /* '<S70>/Unit Delay1' */
  real32_T Delay_DSTATE_d3;            /* '<S47>/Delay' */
  volatile real32_T RateTransition_Buffer0;/* '<Root>/Rate Transition' */
  real32_T RPMtoOmegaE_c;              /* '<Root>/Data Store Memory' */
  real32_T OmegaEtoRPM_k;              /* '<Root>/Data Store Memory1' */
  real32_T Tsc;                        /* '<Root>/Data Store Memory3' */
  real32_T ModeOut;                    /* '<S118>/SenzorlessStateDigram' */
  real32_T RPM_sign_latch;             /* '<S118>/SenzorlessStateDigram' */
  real32_T PrevY;                      /* '<S116>/Rate Limiter' */
  int32_T ErrCode;                     /* '<S41>/Merge' */
  int32_T Err;                         /* '<S159>/Add' */
  int32_T PowerLimitDuration;          /* '<S21>/Data Type Conversion' */
  int32_T PowerLimitDurationHysteresis;/* '<S21>/Data Type Conversion1' */
  int32_T UnitDelay_DSTATE_lx;         /* '<S124>/Unit Delay' */
  int32_T UnitDelay1_DSTATE_j;         /* '<S124>/Unit Delay1' */
  uint32_T UnitDelay_DSTATE_e;         /* '<S295>/Unit Delay' */
  uint32_T UnitDelay_DSTATE_k;         /* '<S294>/Unit Delay' */
  uint32_T temporalCounter_i1;         /* '<S118>/SenzorlessStateDigram' */
  enumSenzorlessState State;           /* '<S41>/Merge' */
  enumSenzorlessState SenzorlessState; /* '<S118>/SenzorlessStateDigram' */
  enumSenzorlessState State_p;
  uint8_T is_active_c5_emDriveFOC_control;/* '<S242>/PowerLimitStateMachine' */
  uint8_T is_c5_emDriveFOC_control;    /* '<S242>/PowerLimitStateMachine' */
  uint8_T temporalCounter_i1_i;        /* '<S242>/PowerLimitStateMachine' */
  uint8_T is_c3_emDriveFOC_control;    /* '<S118>/SenzorlessStateDigram' */
  uint8_T is_Run;                      /* '<S118>/SenzorlessStateDigram' */
  boolean_T RateTransition6;           /* '<Root>/Rate Transition6' */
  boolean_T MTPV_active;               /* '<Root>/Rate Transition3' */
  boolean_T RPM_Track;                 /* '<Root>/Rate Transition3' */
  boolean_T pow_lim;                   /* '<S3>/Rate Transition7' */
  boolean_T Udc_lim;                   /* '<S3>/Rate Transition7' */
  boolean_T RateTransition13;          /* '<Root>/Rate Transition13' */
  boolean_T RelationalOperator;        /* '<S223>/Relational Operator' */
  boolean_T Udc_lim_o;                 /* '<S220>/Relational Operator' */
  boolean_T pow_lim_a;                 /* '<S220>/Relational Operator2' */
  boolean_T RPM_Track_b;               /* '<S41>/Merge' */
  boolean_T RPM_Track_a;               /* '<S118>/SenzorlessStateDigram' */
  boolean_T LogicalOperator;           /* '<S129>/Logical Operator' */
  boolean_T RPM_Track_m;
                    /* '<S38>/BusConversion_InsertedFor_FOC_data_at_inport_0' */
  boolean_T RelationalOperator_li;     /* '<S52>/Relational Operator' */
  boolean_T RelationalOperator_le;     /* '<S46>/Relational Operator' */
  boolean_T DelayInput1_DSTATE;        /* '<S278>/Delay Input1' */
  boolean_T UnitDelay2_1_DSTATE;       /* '<S118>/Unit Delay2' */
  boolean_T UnitDelay_DSTATE_e0;       /* '<S129>/Unit Delay' */
  boolean_T icLoad;                    /* '<S307>/Delay1' */
  boolean_T icLoad_c;                  /* '<S307>/Delay' */
  boolean_T icLoad_j;                  /* '<S312>/Delay' */
  boolean_T icLoad_jq;                 /* '<S288>/Delay' */
  boolean_T icLoad_i;                  /* '<S283>/Delay' */
  boolean_T icLoad_jg;                 /* '<S267>/Delay' */
  boolean_T icLoad_f;                  /* '<S229>/Delay' */
  boolean_T icLoad_a;                  /* '<S251>/Delay' */
  boolean_T IdRampFinished;            /* '<S118>/SenzorlessStateDigram' */
  boolean_T icLoad_fe;                 /* '<S183>/Delay' */
  boolean_T icLoad_b;                  /* '<S168>/Delay' */
  boolean_T icLoad_e;                  /* '<S143>/Delay' */
  boolean_T icLoad_d;                  /* '<S136>/Delay' */
  boolean_T icLoad_jc;                 /* '<S84>/Delay' */
  boolean_T icLoad_p;                  /* '<S83>/Delay' */
  boolean_T icLoad_n;                  /* '<S71>/Delay' */
  boolean_T icLoad_o;                  /* '<S70>/Delay' */
  boolean_T icLoad_g;                  /* '<S47>/Delay' */
} DW_emDriveFOC_control_T;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: single([0,42.19,118.82,94.22,123.14,143.46]);
   * Referenced by: '<S85>/Ud Lookup Table'
   */
  real32_T UdLookupTable_tableData[6];

  /* Expression: single([0,562.5,5625,11250,15750,18000]);
   * Referenced by: '<S85>/Ud Lookup Table'
   */
  real32_T UdLookupTable_bp01Data[6];
} ConstP_emDriveFOC_control_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  tyBusFOC_control Control;            /* '<Root>/Control' */
  tyBusSensorsTsc SensorsTsc;          /* '<Root>/SensorsTsc' */
} ExtU_emDriveFOC_control_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  tyBusActuatorsTsc ActuatorsTsc;      /* '<Root>/ActuatorsTsc' */
  tyBusDataTsc DataTsc;                /* '<Root>/DataTsc' */
} ExtY_emDriveFOC_control_T;

/* Real-time Model Data Structure */
struct tag_RTM_emDriveFOC_control_T {
  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    struct {
      uint16_T TID[3];
      uint16_T cLimit[3];
    } TaskCounters;
  } Timing;
};

/* Block signals and states (default storage) */
extern DW_emDriveFOC_control_T emDriveFOC_control_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_emDriveFOC_control_T emDriveFOC_control_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_emDriveFOC_control_T emDriveFOC_control_Y;

/* Constant parameters (default storage) */
extern const ConstP_emDriveFOC_control_T emDriveFOC_control_ConstP;

/* Model entry point functions */
FOC_SLOW_CODE
  extern void emDriveFOC_control_initialize(void);
FOC_RESTORE_CODE
  FOC_FAST_CODE
  extern void emDriveFOC_control_step0(void);
FOC_RESTORE_CODE
//  FOC_FAST_CODE
  extern void emDriveFOC_control_step1(void);
//FOC_RESTORE_CODE
//  FOC_FAST_CODE
  extern void emDriveFOC_control_step2(void);
//FOC_RESTORE_CODE
  /* Real-time Model object */
  extern RT_MODEL_emDriveFOC_control_T *const emDriveFOC_control_M;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S22>/Display' : Unused code path elimination
 * Block '<S22>/Display1' : Unused code path elimination
 * Block '<S22>/Display2' : Unused code path elimination
 * Block '<S22>/Display3' : Unused code path elimination
 * Block '<S77>/Data Type Duplicate' : Unused code path elimination
 * Block '<S77>/Data Type Propagation' : Unused code path elimination
 * Block '<S78>/Data Type Duplicate' : Unused code path elimination
 * Block '<S78>/Data Type Propagation' : Unused code path elimination
 * Block '<S65>/FieldWeakening' : Unused code path elimination
 * Block '<S72>/Gain' : Unused code path elimination
 * Block '<S72>/Relational Operator1' : Unused code path elimination
 * Block '<S67>/Data Type Duplicate' : Unused code path elimination
 * Block '<S67>/Data Type Propagation' : Unused code path elimination
 * Block '<S82>/Data Type Duplicate' : Unused code path elimination
 * Block '<S82>/Data Type Propagation' : Unused code path elimination
 * Block '<S92>/Data Type Duplicate' : Unused code path elimination
 * Block '<S92>/Data Type Propagation' : Unused code path elimination
 * Block '<S93>/Data Type Duplicate' : Unused code path elimination
 * Block '<S93>/Data Type Propagation' : Unused code path elimination
 * Block '<S94>/Data Type Duplicate' : Unused code path elimination
 * Block '<S94>/Data Type Propagation' : Unused code path elimination
 * Block '<S95>/Data Type Duplicate' : Unused code path elimination
 * Block '<S95>/Data Type Propagation' : Unused code path elimination
 * Block '<S85>/Scope1' : Unused code path elimination
 * Block '<S85>/Scope2' : Unused code path elimination
 * Block '<S54>/Current_regulators_U' : Unused code path elimination
 * Block '<S38>/FOC' : Unused code path elimination
 * Block '<S58>/DeadTime2' : Unused code path elimination
 * Block '<S127>/Scope1' : Unused code path elimination
 * Block '<S145>/Data Type Duplicate' : Unused code path elimination
 * Block '<S145>/Data Type Propagation' : Unused code path elimination
 * Block '<S128>/Gain3' : Unused code path elimination
 * Block '<S128>/Senzorless' : Unused code path elimination
 * Block '<S129>/SensorlessValid' : Unused code path elimination
 * Block '<S124>/SensorlessMode' : Unused code path elimination
 * Block '<S197>/Data Type Duplicate' : Unused code path elimination
 * Block '<S197>/Data Type Propagation' : Unused code path elimination
 * Block '<S206>/Data Type Duplicate' : Unused code path elimination
 * Block '<S206>/Data Type Propagation' : Unused code path elimination
 * Block '<S219>/Data Type Duplicate' : Unused code path elimination
 * Block '<S219>/Data Type Propagation' : Unused code path elimination
 * Block '<S44>/PWM' : Unused code path elimination
 * Block '<S220>/Overvoltage' : Unused code path elimination
 * Block '<S220>/Overvoltage1' : Unused code path elimination
 * Block '<S224>/Data Type Duplicate' : Unused code path elimination
 * Block '<S224>/Data Type Propagation' : Unused code path elimination
 * Block '<S235>/Data Type Duplicate' : Unused code path elimination
 * Block '<S235>/Data Type Propagation' : Unused code path elimination
 * Block '<S236>/Data Type Duplicate' : Unused code path elimination
 * Block '<S236>/Data Type Propagation' : Unused code path elimination
 * Block '<S239>/Data Type Duplicate' : Unused code path elimination
 * Block '<S239>/Data Type Propagation' : Unused code path elimination
 * Block '<S240>/Data Type Duplicate' : Unused code path elimination
 * Block '<S240>/Data Type Propagation' : Unused code path elimination
 * Block '<S243>/Data Type Duplicate' : Unused code path elimination
 * Block '<S243>/Data Type Propagation' : Unused code path elimination
 * Block '<S244>/Data Type Duplicate' : Unused code path elimination
 * Block '<S244>/Data Type Propagation' : Unused code path elimination
 * Block '<S241>/Scope' : Unused code path elimination
 * Block '<S242>/TemperatureLimit' : Unused code path elimination
 * Block '<S228>/TemperatureLimit' : Unused code path elimination
 * Block '<S256>/Data Type Duplicate' : Unused code path elimination
 * Block '<S256>/Data Type Propagation' : Unused code path elimination
 * Block '<S257>/Data Type Duplicate' : Unused code path elimination
 * Block '<S257>/Data Type Propagation' : Unused code path elimination
 * Block '<S223>/Power Limit' : Unused code path elimination
 * Block '<S223>/Temperature limit' : Unused code path elimination
 * Block '<S4>/Display' : Unused code path elimination
 * Block '<S4>/Display1' : Unused code path elimination
 * Block '<S4>/Display2' : Unused code path elimination
 * Block '<S4>/Display3' : Unused code path elimination
 * Block '<S4>/Display4' : Unused code path elimination
 * Block '<S4>/Display5' : Unused code path elimination
 * Block '<S268>/Measurments' : Unused code path elimination
 * Block '<S6>/Ramp' : Unused code path elimination
 * Block '<S293>/Data Type Duplicate' : Unused code path elimination
 * Block '<S293>/Data Type Propagation' : Unused code path elimination
 * Block '<S289>/Data Type Duplicate' : Unused code path elimination
 * Block '<S289>/Data Type Propagation' : Unused code path elimination
 * Block '<S297>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S297>/FixPt Data Type Propagation' : Unused code path elimination
 * Block '<S298>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S298>/FixPt Data Type Propagation' : Unused code path elimination
 * Block '<S14>/Scope' : Unused code path elimination
 * Block '<S14>/VelocityReg' : Unused code path elimination
 * Block '<S313>/Data Type Duplicate' : Unused code path elimination
 * Block '<S313>/Data Type Propagation' : Unused code path elimination
 * Block '<S314>/Data Type Duplicate' : Unused code path elimination
 * Block '<S314>/Data Type Propagation' : Unused code path elimination
 * Block '<S22>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S22>/Rate Transition7' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S37>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S38>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S38>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S38>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S41>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S41>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S124>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S124>/Data Type Conversion3' : Eliminate redundant data type conversion
 * Block '<S3>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition12' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition2' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition3' : Eliminated since input and output rates are identical
 * Block '<S3>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition11' : Eliminated since input and output rates are identical
 * Block '<S4>/Rate Transition12' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition13' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition4' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition5' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition6' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition7' : Eliminated since input and output rates are identical
 * Block '<S5>/Rate Transition8' : Eliminated since input and output rates are identical
 * Block '<S294>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S297>/FixPt Gateway In' : Eliminate redundant data type conversion
 * Block '<S295>/Data Type Conversion2' : Eliminate redundant data type conversion
 * Block '<S298>/FixPt Gateway In' : Eliminate redundant data type conversion
 * Block '<S12>/Rate Transition' : Eliminated since input and output rates are identical
 * Block '<S12>/Rate Transition1' : Eliminated since input and output rates are identical
 * Block '<S14>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S14>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S158>/Constant7' : Unused code path elimination
 * Block '<S161>/Constant8' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'emDriveFOC_control'
 * '<S1>'   : 'emDriveFOC_control/ConstInit'
 * '<S2>'   : 'emDriveFOC_control/Current_loop'
 * '<S3>'   : 'emDriveFOC_control/Derating'
 * '<S4>'   : 'emDriveFOC_control/InitGlobals'
 * '<S5>'   : 'emDriveFOC_control/Measurement1'
 * '<S6>'   : 'emDriveFOC_control/RPM_ramp_and_limit'
 * '<S7>'   : 'emDriveFOC_control/SignalExport'
 * '<S8>'   : 'emDriveFOC_control/SignalExport1'
 * '<S9>'   : 'emDriveFOC_control/SignalExport3'
 * '<S10>'  : 'emDriveFOC_control/SignalExport4'
 * '<S11>'  : 'emDriveFOC_control/StimGenerator'
 * '<S12>'  : 'emDriveFOC_control/StimRPM'
 * '<S13>'  : 'emDriveFOC_control/StopIfErorr'
 * '<S14>'  : 'emDriveFOC_control/Velocity_loop'
 * '<S15>'  : 'emDriveFOC_control/ConstInit/AustalianBall'
 * '<S16>'  : 'emDriveFOC_control/ConstInit/BEMF_ValidConst'
 * '<S17>'  : 'emDriveFOC_control/ConstInit/CloseLoopConst'
 * '<S18>'  : 'emDriveFOC_control/ConstInit/FieldWeakConst'
 * '<S19>'  : 'emDriveFOC_control/ConstInit/MeasurmentConst'
 * '<S20>'  : 'emDriveFOC_control/ConstInit/ModulatorConst'
 * '<S21>'  : 'emDriveFOC_control/ConstInit/PowerLimitConst'
 * '<S22>'  : 'emDriveFOC_control/ConstInit/QadraturePLL_BEMF_const'
 * '<S23>'  : 'emDriveFOC_control/ConstInit/TtoIq_Const'
 * '<S24>'  : 'emDriveFOC_control/ConstInit/AustalianBall/HPF_coef'
 * '<S25>'  : 'emDriveFOC_control/ConstInit/AustalianBall/HPF_coef/MATLAB Function'
 * '<S26>'  : 'emDriveFOC_control/ConstInit/BEMF_ValidConst/LPFalphaCriteria1'
 * '<S27>'  : 'emDriveFOC_control/ConstInit/BEMF_ValidConst/LPFalphaCriteria1/MATLAB Function'
 * '<S28>'  : 'emDriveFOC_control/ConstInit/FieldWeakConst/LPFalphaFieldWeak1'
 * '<S29>'  : 'emDriveFOC_control/ConstInit/FieldWeakConst/LPFalphaFieldWeak1/MATLAB Function'
 * '<S30>'  : 'emDriveFOC_control/ConstInit/MeasurmentConst/LPFalphaMeasurment'
 * '<S31>'  : 'emDriveFOC_control/ConstInit/MeasurmentConst/LPFalphaMeasurment/MATLAB Function'
 * '<S32>'  : 'emDriveFOC_control/ConstInit/QadraturePLL_BEMF_const/LPFalphaOmega'
 * '<S33>'  : 'emDriveFOC_control/ConstInit/QadraturePLL_BEMF_const/ThetaPLLCalcK1'
 * '<S34>'  : 'emDriveFOC_control/ConstInit/QadraturePLL_BEMF_const/ThetaPLLCalcK2'
 * '<S35>'  : 'emDriveFOC_control/ConstInit/QadraturePLL_BEMF_const/LPFalphaOmega/MATLAB Function'
 * '<S36>'  : 'emDriveFOC_control/ConstInit/TtoIq_Const/CalcualteIqC'
 * '<S37>'  : 'emDriveFOC_control/Current_loop/AngleGen'
 * '<S38>'  : 'emDriveFOC_control/Current_loop/FOC'
 * '<S39>'  : 'emDriveFOC_control/Current_loop/PreventZero'
 * '<S40>'  : 'emDriveFOC_control/Current_loop/PreventZero1'
 * '<S41>'  : 'emDriveFOC_control/Current_loop/SensorlessTop'
 * '<S42>'  : 'emDriveFOC_control/Current_loop/SignalExport'
 * '<S43>'  : 'emDriveFOC_control/Current_loop/SignalExport1'
 * '<S44>'  : 'emDriveFOC_control/Current_loop/Uabc_to_PWM'
 * '<S45>'  : 'emDriveFOC_control/Current_loop/abc-to-alphabeta'
 * '<S46>'  : 'emDriveFOC_control/Current_loop/AngleGen/AngleGenConst'
 * '<S47>'  : 'emDriveFOC_control/Current_loop/AngleGen/IntegratorTheta'
 * '<S48>'  : 'emDriveFOC_control/Current_loop/AngleGen/ThetaAdd'
 * '<S49>'  : 'emDriveFOC_control/Current_loop/AngleGen/IntegratorTheta/ThetaAdd'
 * '<S50>'  : 'emDriveFOC_control/Current_loop/AngleGen/IntegratorTheta/ThetaAdd/ThetaAdd'
 * '<S51>'  : 'emDriveFOC_control/Current_loop/AngleGen/ThetaAdd/ThetaAdd'
 * '<S52>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id'
 * '<S53>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Iq'
 * '<S54>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators'
 * '<S55>'  : 'emDriveFOC_control/Current_loop/FOC/DeadTimeComp'
 * '<S56>'  : 'emDriveFOC_control/Current_loop/FOC/FW transform'
 * '<S57>'  : 'emDriveFOC_control/Current_loop/FOC/IQ decoupling'
 * '<S58>'  : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1'
 * '<S59>'  : 'emDriveFOC_control/Current_loop/FOC/SignalExport1'
 * '<S60>'  : 'emDriveFOC_control/Current_loop/FOC/SignalExport2'
 * '<S61>'  : 'emDriveFOC_control/Current_loop/FOC/StimId'
 * '<S62>'  : 'emDriveFOC_control/Current_loop/FOC/StimIq'
 * '<S63>'  : 'emDriveFOC_control/Current_loop/FOC/alphabeta-to-dq'
 * '<S64>'  : 'emDriveFOC_control/Current_loop/FOC/dq-to-alphabeta'
 * '<S65>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening'
 * '<S66>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/MTPA'
 * '<S67>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/Saturation Dynamic'
 * '<S68>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/SignalExport'
 * '<S69>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/SignalExport1'
 * '<S70>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/ Field weakening PI'
 * '<S71>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/LPFFieldWeak'
 * '<S72>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/ModulationFactor_Warning'
 * '<S73>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/Pitagora_calc'
 * '<S74>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/SafeDivide2'
 * '<S75>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/SignalExport'
 * '<S76>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/SignalExport1'
 * '<S77>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/ Field weakening PI/Saturation Dynamic'
 * '<S78>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Id/FieldWeakening/ Field weakening PI/Saturation Dynamic1'
 * '<S79>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Iq/Pitagora_calc2'
 * '<S80>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Iq/SignalExport4'
 * '<S81>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Iq/SymetricLimiter'
 * '<S82>'  : 'emDriveFOC_control/Current_loop/FOC/Calculate_Iq/SymetricLimiter/Saturation Dynamic'
 * '<S83>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator D'
 * '<S84>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator Q'
 * '<S85>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ALTTestMode'
 * '<S86>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/SignalExport'
 * '<S87>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/SignalExport1'
 * '<S88>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/SignalExport2'
 * '<S89>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/StimUd'
 * '<S90>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/StrimUq'
 * '<S91>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ToSymetricLimUq1'
 * '<S92>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator D/Saturation Dynamic'
 * '<S93>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator D/Saturation Dynamic1'
 * '<S94>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator Q/Saturation Dynamic'
 * '<S95>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ PI regulator Q/Saturation Dynamic1'
 * '<S96>'  : 'emDriveFOC_control/Current_loop/FOC/CurrentRegulators/ALTTestMode/SignalExport3'
 * '<S97>'  : 'emDriveFOC_control/Current_loop/FOC/DeadTimeComp/abc-to-alphabeta2'
 * '<S98>'  : 'emDriveFOC_control/Current_loop/FOC/DeadTimeComp/alphabeta-to-abc'
 * '<S99>'  : 'emDriveFOC_control/Current_loop/FOC/DeadTimeComp/alphabeta-to-abc1'
 * '<S100>' : 'emDriveFOC_control/Current_loop/FOC/FW transform/alphabeta-to-dq'
 * '<S101>' : 'emDriveFOC_control/Current_loop/FOC/FW transform/alphabeta-to-dq/vector mixer'
 * '<S102>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/CompDelayFOC'
 * '<S103>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/SVPWM'
 * '<S104>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/Saturation'
 * '<S105>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/SignalExport'
 * '<S106>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/abc-to-alphabeta1'
 * '<S107>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/alphabeta-to-abc'
 * '<S108>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/dq-to-alphabeta'
 * '<S109>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/CompDelayFOC/ThetaAdd'
 * '<S110>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/CompDelayFOC/ThetaAdd/ThetaAdd'
 * '<S111>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/Saturation/MinPulseTime'
 * '<S112>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/Saturation/Saturate'
 * '<S113>' : 'emDriveFOC_control/Current_loop/FOC/ReverseTransform1/dq-to-alphabeta/vector mixer'
 * '<S114>' : 'emDriveFOC_control/Current_loop/FOC/alphabeta-to-dq/vector mixer'
 * '<S115>' : 'emDriveFOC_control/Current_loop/FOC/dq-to-alphabeta/vector mixer'
 * '<S116>' : 'emDriveFOC_control/Current_loop/SensorlessTop/CalculateRefSign'
 * '<S117>' : 'emDriveFOC_control/Current_loop/SensorlessTop/EmptyPosCtrl'
 * '<S118>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless'
 * '<S119>' : 'emDriveFOC_control/Current_loop/SensorlessTop/SensorlessDummy'
 * '<S120>' : 'emDriveFOC_control/Current_loop/SensorlessTop/EmptyPosCtrl/Enumerated Constant'
 * '<S121>' : 'emDriveFOC_control/Current_loop/SensorlessTop/EmptyPosCtrl/Enumerated Constant2'
 * '<S122>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode'
 * '<S123>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/HFI_PosCtrl'
 * '<S124>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope'
 * '<S125>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram'
 * '<S126>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer'
 * '<S127>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation'
 * '<S128>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals'
 * '<S129>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid'
 * '<S130>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/BEMF_Observer'
 * '<S131>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/Discrete  Integrator1'
 * '<S132>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/Discrete  Integrator2'
 * '<S133>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/SignalExport'
 * '<S134>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/EBEMF_observer/SignalExport1'
 * '<S135>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/CompensateDelay'
 * '<S136>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/LPF_omega'
 * '<S137>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaAdd'
 * '<S138>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL'
 * '<S139>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaSub'
 * '<S140>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/CompensateDelay/ThetaAdd'
 * '<S141>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/CompensateDelay/ThetaAdd/ThetaAdd'
 * '<S142>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaAdd/ThetaAdd'
 * '<S143>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/Discrete  Integrator1'
 * '<S144>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/Discrete  Integrator2'
 * '<S145>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/Saturation Dynamic'
 * '<S146>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/ThetaSub'
 * '<S147>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/Discrete  Integrator1/ThetaAdd'
 * '<S148>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/Discrete  Integrator1/ThetaAdd/ThetaAdd'
 * '<S149>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaPLL/ThetaSub/ThetaSub'
 * '<S150>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Quadrature_PLL_LPF_compensation/ThetaSub/ThetaSub'
 * '<S151>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/SignalExport1'
 * '<S152>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/SignalExport2'
 * '<S153>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/SignalExport3'
 * '<S154>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/SignalExport4'
 * '<S155>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/SignalExport5'
 * '<S156>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/ThetaSub'
 * '<S157>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Scope_and_signals/ThetaSub/ThetaSub'
 * '<S158>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/BEMF_criteria'
 * '<S159>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask'
 * '<S160>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Omega_criteria'
 * '<S161>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/PLL_criteria'
 * '<S162>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/RPM_criteria'
 * '<S163>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/SignalExport1'
 * '<S164>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/SignalExport2'
 * '<S165>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/SignalExport3'
 * '<S166>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/SignalExport4'
 * '<S167>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/BEMF_criteria/Compare To Constant1'
 * '<S168>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/BEMF_criteria/LPF_BackEMF'
 * '<S169>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/BEMF_criteria/Pitagora_calc'
 * '<S170>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant'
 * '<S171>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant1'
 * '<S172>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant10'
 * '<S173>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant11'
 * '<S174>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant2'
 * '<S175>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant3'
 * '<S176>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant4'
 * '<S177>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant5'
 * '<S178>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant6'
 * '<S179>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant7'
 * '<S180>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant8'
 * '<S181>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/Error_bitmask/Enumerated Constant9'
 * '<S182>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/PLL_criteria/Compare To Constant'
 * '<S183>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/CloseLoopMode/Sensorless_valid/PLL_criteria/LPF_PLLerr'
 * '<S184>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/Enumerated Constant'
 * '<S185>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/Enumerated Constant1'
 * '<S186>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/SignalExport1'
 * '<S187>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/SignalExport2'
 * '<S188>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/SignalExport4'
 * '<S189>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/MuxAndScope/SignalExport5'
 * '<S190>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.IdRamp'
 * '<S191>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix'
 * '<S192>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp'
 * '<S193>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RefValueMix'
 * '<S194>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.IdRamp/Detect Rise Positive'
 * '<S195>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.IdRamp/Rate Limiter Dynamic with track'
 * '<S196>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.IdRamp/Detect Rise Positive/Positive'
 * '<S197>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.IdRamp/Rate Limiter Dynamic with track/Saturation Dynamic'
 * '<S198>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Compare To Constant'
 * '<S199>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Compare To Constant1'
 * '<S200>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Detect Rise Positive'
 * '<S201>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Rate Limiter Dynamic with track'
 * '<S202>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/ThetaAdd'
 * '<S203>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/ThetaSub'
 * '<S204>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Unwrap'
 * '<S205>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Detect Rise Positive/Positive'
 * '<S206>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Rate Limiter Dynamic with track/Saturation Dynamic'
 * '<S207>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/ThetaAdd/ThetaAdd'
 * '<S208>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/ThetaSub/ThetaSub'
 * '<S209>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Unwrap/ThetaAdd'
 * '<S210>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Unwrap/ThetaSub'
 * '<S211>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Unwrap/ThetaAdd/ThetaAdd'
 * '<S212>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.PosModeMix/Unwrap/ThetaSub/ThetaSub'
 * '<S213>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/Detect Rise Positive'
 * '<S214>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/OmegaToTheta'
 * '<S215>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/Rate Limiter Dynamic with track'
 * '<S216>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/Detect Rise Positive/Positive'
 * '<S217>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/OmegaToTheta/ThetaAdd'
 * '<S218>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/OmegaToTheta/ThetaAdd/ThetaAdd'
 * '<S219>' : 'emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram/Run.RPM_ramp/Rate Limiter Dynamic with track/Saturation Dynamic'
 * '<S220>' : 'emDriveFOC_control/Derating/Overvoltage'
 * '<S221>' : 'emDriveFOC_control/Derating/SignalExport1'
 * '<S222>' : 'emDriveFOC_control/Derating/SignalExport2'
 * '<S223>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit'
 * '<S224>' : 'emDriveFOC_control/Derating/Overvoltage/Saturation Dynamic'
 * '<S225>' : 'emDriveFOC_control/Derating/Overvoltage/SignalExport1'
 * '<S226>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ PI regMotorTemp'
 * '<S227>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/BridgeTemperature'
 * '<S228>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit'
 * '<S229>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel'
 * '<S230>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/OutletTemperature'
 * '<S231>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/SignalExport1'
 * '<S232>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/SignalExport2'
 * '<S233>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/SignalExport3'
 * '<S234>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/SignalExport4'
 * '<S235>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ PI regMotorTemp/Saturation Dynamic'
 * '<S236>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ PI regMotorTemp/Saturation Dynamic1'
 * '<S237>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/BridgeTemperature/ PIregBridgeTemp'
 * '<S238>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/BridgeTemperature/PreventMinimalLimiting'
 * '<S239>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/BridgeTemperature/ PIregBridgeTemp/Saturation Dynamic'
 * '<S240>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/BridgeTemperature/ PIregBridgeTemp/Saturation Dynamic1'
 * '<S241>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/ElectricalPowerLimit'
 * '<S242>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow'
 * '<S243>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/ElectricalPowerLimit/Saturation Dynamic'
 * '<S244>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/ElectricalPowerLimit/Saturation Dynamic1'
 * '<S245>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/ElectricalPowerLimit/PowerLimitDecisionStateflow/PowerLimitStateMachine'
 * '<S246>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/CopperResistance'
 * '<S247>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/PowerToTemperatureMotor'
 * '<S248>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/SignalExport1'
 * '<S249>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/SignalExport2'
 * '<S250>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/SignalExport3'
 * '<S251>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/PowerToTemperatureMotor/ThermalModelVariableP'
 * '<S252>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/PowerToTemperatureMotor/ThermalTimeConstant'
 * '<S253>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/MotorTempThermalModel/PowerToTemperatureMotor/ThermalModelVariableP/P_variable_to_Temp'
 * '<S254>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/OutletTemperature/ PIregOutletTemp'
 * '<S255>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/OutletTemperature/PreventMinimalLimiting'
 * '<S256>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/OutletTemperature/ PIregOutletTemp/Saturation Dynamic'
 * '<S257>' : 'emDriveFOC_control/Derating/TemperatureAndPowerLimit/OutletTemperature/ PIregOutletTemp/Saturation Dynamic1'
 * '<S258>' : 'emDriveFOC_control/InitGlobals/CalculatedDgains'
 * '<S259>' : 'emDriveFOC_control/InitGlobals/CalculatedQgains'
 * '<S260>' : 'emDriveFOC_control/InitGlobals/EBMFObserverGains'
 * '<S261>' : 'emDriveFOC_control/InitGlobals/SignalExport'
 * '<S262>' : 'emDriveFOC_control/InitGlobals/SignalExport1'
 * '<S263>' : 'emDriveFOC_control/InitGlobals/SignalExport2'
 * '<S264>' : 'emDriveFOC_control/InitGlobals/SignalExport3'
 * '<S265>' : 'emDriveFOC_control/InitGlobals/SignalExport4'
 * '<S266>' : 'emDriveFOC_control/InitGlobals/SignalExport5'
 * '<S267>' : 'emDriveFOC_control/Measurement1/LPF_Measurment'
 * '<S268>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel'
 * '<S269>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR'
 * '<S270>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pel_LPF'
 * '<S271>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pitagora_calc'
 * '<S272>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pitagora_calc1'
 * '<S273>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/SignalExport1'
 * '<S274>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/SignalExport2'
 * '<S275>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/SignalExport3'
 * '<S276>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/SignalExport4'
 * '<S277>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/SignalExport5'
 * '<S278>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR/Detect Fall Nonpositive'
 * '<S279>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR/Enumerated Constant'
 * '<S280>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR/UdIntegrator'
 * '<S281>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR/UdIntegrator1'
 * '<S282>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/MeasureMotorTempFromR/Detect Fall Nonpositive/Nonpositive'
 * '<S283>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pel_LPF/LPF_Measurment'
 * '<S284>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pel_LPF/LPFalphaMeasurment1'
 * '<S285>' : 'emDriveFOC_control/Measurement1/MeasurmentsVel/Pel_LPF/LPFalphaMeasurment1/MATLAB Function'
 * '<S286>' : 'emDriveFOC_control/RPM_ramp_and_limit/Const'
 * '<S287>' : 'emDriveFOC_control/RPM_ramp_and_limit/RampLimit'
 * '<S288>' : 'emDriveFOC_control/RPM_ramp_and_limit/Rate Limiter Dynamic with track'
 * '<S289>' : 'emDriveFOC_control/RPM_ramp_and_limit/Saturation Dynamic'
 * '<S290>' : 'emDriveFOC_control/RPM_ramp_and_limit/SignalExport'
 * '<S291>' : 'emDriveFOC_control/RPM_ramp_and_limit/SignalExport1'
 * '<S292>' : 'emDriveFOC_control/RPM_ramp_and_limit/RampLimit/SignalExport1'
 * '<S293>' : 'emDriveFOC_control/RPM_ramp_and_limit/Rate Limiter Dynamic with track/Saturation Dynamic'
 * '<S294>' : 'emDriveFOC_control/StimGenerator/PulseGenerator'
 * '<S295>' : 'emDriveFOC_control/StimGenerator/PulseGenerator1'
 * '<S296>' : 'emDriveFOC_control/StimGenerator/SignalExport'
 * '<S297>' : 'emDriveFOC_control/StimGenerator/PulseGenerator/Increment Stored Integer'
 * '<S298>' : 'emDriveFOC_control/StimGenerator/PulseGenerator1/Increment Stored Integer'
 * '<S299>' : 'emDriveFOC_control/StopIfErorr/Enumerated Constant'
 * '<S300>' : 'emDriveFOC_control/Velocity_loop/CalculateLimit'
 * '<S301>' : 'emDriveFOC_control/Velocity_loop/SignalExport1'
 * '<S302>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator'
 * '<S303>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/SignalExport'
 * '<S304>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/SignalExport1'
 * '<S305>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/SignalExport2'
 * '<S306>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Vel_PID_param'
 * '<S307>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Velocity PID'
 * '<S308>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Vel_PID_param/LPF_coef1'
 * '<S309>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Vel_PID_param/SpeedParamScheduler'
 * '<S310>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Vel_PID_param/LPF_coef1/MATLAB Function1'
 * '<S311>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Vel_PID_param/SpeedParamScheduler/MATLAB Function'
 * '<S312>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Velocity PID/LPF'
 * '<S313>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Velocity PID/Saturation Dynamic'
 * '<S314>' : 'emDriveFOC_control/Velocity_loop/Velocity_regulator/Velocity PID/Saturation Dynamic1'
 */
#endif                                 /* RTW_HEADER_emDriveFOC_control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
