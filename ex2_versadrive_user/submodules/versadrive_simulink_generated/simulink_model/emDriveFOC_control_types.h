/*
 * File: emDriveFOC_control_types.h
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

#ifndef RTW_HEADER_emDriveFOC_control_types_h_
#define RTW_HEADER_emDriveFOC_control_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_enumSenzorlessState_
#define DEFINED_TYPEDEF_FOR_enumSenzorlessState_

/* enumeration to track active leaf state of emDriveFOC_control/Current_loop/SensorlessTop/Sensorless/SenzorlessStateDigram */
typedef enum {
  enumSenzorlessState_None = 0,        /* Default value */
  enumSenzorlessState_Stop,
  enumSenzorlessState_Align,
  enumSenzorlessState_OpenLoop,
  enumSenzorlessState_TransitionToCloseLoop,
  enumSenzorlessState_CloseLoop,
  enumSenzorlessState_TransitionToOpenLoop,
  enumSenzorlessState_Error,
  enumSenzorlessState_Hold
} enumSenzorlessState;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumSlidingErr_
#define DEFINED_TYPEDEF_FOR_enumSlidingErr_

/* ennumeration used in signal Ctrl_Sensorless_StatusSlidingCriteriaFlags */
typedef enum {
  enumSlidingErr_NoErr = 0,            /* Default value */
  enumSlidingErr_Err_PLL = 1,
  enumSlidingErr_Err_BEMF = 2,
  enumSlidingErr_Err_Omega = 4,
  enumSlidingErr_Err_RPM = 8,
  enumSlidingErr_Err_Overspeed = 16,
  enumSlidingErr_No_SensorLess = 32
} enumSlidingErr;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tyBusFOC_control_
#define DEFINED_TYPEDEF_FOR_tyBusFOC_control_

/* Control input fro complete FOC */
typedef struct {
  /* Requested RPM value in FOC is in velocity mode. */
  real32_T RPM_ref;

  /* This value will be internal limited with parameter Ctrl_Limits__RPMramp */
  real32_T RPM_ramp;

  /* True -> FOC is operational.
     False -> FOC returns voltages 0. But angle and sensorelss is still active and
     motor can start bumpless. */
  boolean_T RunStop;

  /* Set this to true when ASC is active. Will force output PWM to all low side active 100 %. Senszorless calculation will still work and all regulatros will track. */
  boolean_T AscActive;

  /* True -> for start of load and unbalance measurment */
  boolean_T LoadMeasStart;

  /* Number of turs for measuring load and unbalance */
  real32_T LoadNoOfTurns;

  /* how much amplitude /RPM will be increased durring measurment. Unit drum RPM. */
  real32_T LoadRPM_amplitude;
} tyBusFOC_control;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tyBusPos_
#define DEFINED_TYPEDEF_FOR_tyBusPos_

typedef struct {
  /* angle */
  real32_T theta_r;

  /* angle velocity */
  real32_T omega_r;
} tyBusPos;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tyBusSensorsTsc_
#define DEFINED_TYPEDEF_FOR_tyBusSensorsTsc_

/* Input data for FOC algorithm at current sample tiem. */
typedef struct {
  /* Measured pahse currents. Positive current is current flowing into motor.
     Clarke trasformation in FOC will remove DC offset, if summ of all three phase
     currents is not zero. */
  real32_T Is_abc[3];

  /* DC link voltage. Must be accurate, not low pass filtered - delayed and not noisy
     because it is primary used in modulator to calculate PWM dutty cycle. Used also in
     calculation of maximum phase voltage and DC current */
  real32_T Udc;

  /* Bridge temperature deg C */
  real32_T BridgeT;

  /* Sensored angle */
  tyBusPos Pos;

  /* Compressor outlet temperature deg C */
  real32_T OutletT;
} tyBusSensorsTsc;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tyBusActuatorsTsc_
#define DEFINED_TYPEDEF_FOR_tyBusActuatorsTsc_

/* This is main result of FOC algorithm. */
typedef struct {
  /* Motor phase voltage */
  real32_T Us_abc[3];

  /* Calculated PWM dutty cycle for each phase half bridge. Range 0-1, 1 means that
     high side is conductiong 100 %. Internaly limited to 0-1 range. */
  real32_T PWM[3];

  /* If PWM sahll be activated. true -> active.
     This can be also false (PWM not active) even if FOC control input sets RunStop to true,
     because sensorless error has been detected. */
  boolean_T RunStop;
} tyBusActuatorsTsc;

#endif

#ifndef DEFINED_TYPEDEF_FOR_tyBusDataTsc_
#define DEFINED_TYPEDEF_FOR_tyBusDataTsc_

/* FOC outputs to upper layer SW */
typedef struct {
  /* Phase voltage [RMS] */
  real32_T Uph;

  /* Phase current [RMS] */
  real32_T Iph;

  /* Motor electrical power [W] */
  real32_T Pel;

  /* Motor RPM [rpm] */
  real32_T RPM;

  /* State of senzorless state machine. Values same as in enmuSenzorless */
  int32_T SenzorlessState;

  /* Status of all limits influencing maximum phase current. Currently only bridge temperature. 1 = full power, 0 reduced to 0. */
  real32_T StatusLim;
} tyBusDataTsc;

#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM_emDriveFOC_control_T RT_MODEL_emDriveFOC_control_T;

#endif                              /* RTW_HEADER_emDriveFOC_control_types_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
