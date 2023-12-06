/*
 * File: rtmodel.c
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

#include "rtmodel.h"

/* Use this function only if you need to maintain compatibility with an existing static main program. */
FOC_FAST_CODE
  void emDriveFOC_control_step(int_T tid)
{
  switch (tid) {
   case 0 :
    emDriveFOC_control_step0();
    break;

   case 1 :
    emDriveFOC_control_step1();
    break;

   case 2 :
    emDriveFOC_control_step2();
    break;

   default :
    /* do nothing */
    break;
  }
}

FOC_RESTORE_CODE
  /*
   * File trailer for generated code.
   *
   * [EOF]
   */
