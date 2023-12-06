/*
 * emDriveTargetSpecific.h
 *
 *  Created on: 18.3.2021
 *      Author: emsiso, aleksander Polutnik
 */

// *************** SIMULINK TARGET ***********************
// this is only dummy header file to have complete generated code and code metrix will work. Do not use this on real target

/*#define FOC_FAST_CODE
#define FOC_SLOW_CODE
#define FOC_RESTORE_CODE*/

// *************** AURIX TARGET ***********************
/*

#define FOC_FAST_CODE       _Pragma ("section code foc_fast_code")
#define FOC_SLOW_CODE       _Pragma ("section code foc_slow_code")
#define FOC_RESTORE_CODE   _Pragma ("section code restore")

// in linker must be defined also next two sections:
// - foc_fast_code, all functions in current loop, shall be defined in coresponding PSRAM. In fast code are also model step1 and step 2 functions, which is not necesary
// - foc_slow_code, most functions in position loop, shall be defined in coresponding PSRAM or FLASH
 
*/

// *************** STM32 TARGET ***********************

#define FOC_FAST_CODE   __attribute__((section (".ccmram")))   // this would be used if there is enough space in ITCM RAM
//#define FOC_FAST_CODE             // now each function separatly needs to be mapped into ITCM RAM in linker file
#define FOC_SLOW_CODE
#define FOC_RESTORE_CODE



