/*
 * emDriveFOC_control_specific.h
 *
 *  Created on: 16.5.2021
 *      Author: emsiso, aleksander Polutnik
 */

// *************** SIMULINK TARGET ***********************
// this is only dummy header file to have complete generated code and code metrix will work. Do not use this on real target

/*#define emDriveSin          sin
#define emDriveCos          cos
#define emDriveSqrt         sqrt
#define emDriveAtan2        atan2*/


// *************** AURIX TARGET ***********************
/*
#include "dsplib.h"
float32 Ifx_atan2F32_2param (float32 y,float32 x)
{
    return(Ifx_atan2F32 (y,x, IFX_MODE_OPTIMIZED_C));
}

#define emDriveSin          Ifx_sinF32_fast
#define emDriveCos          Ifx_cosF32_fast
#define emDriveSqrt         Ifx_sqrtF32_fast            // shall be optimised to call directly FPU instruction
#define emDriveAtan2        Ifx_atan2F32_2param
//#define emDriveAtan2        atan2f

#pragma section neardata foc_near_data     // put new section foc_near_data in coresponding DSRAM
#pragma section nearbss  foc_near_bss      // put new section foc_near_bss in coresponding DSRAM 
#pragma section fardata foc_far_data       // put new section foc_far_data in coresponding DSRAM 
#pragma section farbss  foc_far_bss        // put new section foc_far_bss in coresponding DSRAM 
#pragma default_near_size   4096            // this shall fit all objects. Check size of emDriveFOC_control_DW
*/

// *************** STM32 TARGET ***********************

#include <math.h>

#define emDriveSin          sinf				// all functions shall be replaced with CMSIS versions
#define emDriveCos          cosf
#define emDriveSqrt         sqrtf
#define emDriveAtan2        atan2f

