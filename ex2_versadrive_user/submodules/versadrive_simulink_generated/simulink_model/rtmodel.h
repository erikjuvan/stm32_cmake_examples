/*
 * File: rtmodel.h
 *
 * Code generated for Simulink model 'emDriveFOC_control'.
 *
 * Model version                  : 10.170
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Tue Dec 27 09:55:42 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Infineon->TriCore
 * Code generation objective: Execution efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_rtmodel_h_
#define RTW_HEADER_rtmodel_h_
#include "emDriveFOC_control.h"

/* Macros generated for backwards compatibility  */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((void) 0)
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((void*) 0)
#endif

/* Model wrapper function */
/* Use this function only if you need to maintain compatibility with an existing static main program. */
extern void emDriveFOC_control_step(int_T tid);

#endif                                 /* RTW_HEADER_rtmodel_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
