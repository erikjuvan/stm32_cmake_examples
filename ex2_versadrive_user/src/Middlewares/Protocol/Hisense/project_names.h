//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        project_names.h
// CREATION DATE:      2020/10/20
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/02/03
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Definitions project names
// ----------------------------------------------------------------------------
// Copyright (C) 2020-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PROJECT_NAMES_H
#define PROJECT_NAMES_H

/* ************************************************************************* */

/* No special project, proof of concept                                      */
#define PROJECT_NAME_NONE                                                     0

/* ************************************************************************* */

/* H-PIND-18 IRIS2 project, master electronics                               */
#define PROJECT_NAME_H_PIND_18_MASTER                                         1

/* H-PIND-18 IRIS2 project, slave electronics                                */
#define PROJECT_NAME_H_PIND_18_SLAVE                                          2

/* ************************************************************************* */

/* H-USA-21 IconLedPlus project, User Interface electronics                  */
#define PROJECT_NAME_H_USA_21_ILP_UI                                         10

/* H-USA-21 IconLedPlus project, Power Board electronics                     */
#define PROJECT_NAME_H_USA_21_ILP_PB                                         11

/* H-USA-21 IconLed project, User Interface electronics                      */
#define PROJECT_NAME_H_USA_21_IL_UI                                          15

/* H-USA-21 IconLed project, Power Board electronics                         */
#define PROJECT_NAME_H_USA_21_IL_PB                                          16

/* ************************************************************************* */

/* HV_BIO-2_60-21 project, User Interface electronics                        */
#define PROJECT_NAME_HV_BIO_2_60_21_UI                                       20

/* HV_BIO-2_60-21 project, Power Board electronics (no MCU, just placeholder)*/
#define PROJECT_NAME_HV_BIO_2_60_21_PB                                       21

/* ************************************************************************* */

/* H-BIO_45_60-14 project, User Interface electronics                        */
#define PROJECT_NAME_H_BIO_45_60_14_UI                                       30

/* H-BIO_45_60-14 project, TFT electronics                                   */
#define PROJECT_NAME_H_BIO_45_60_14_TFT                                      31

/* H-BIO_45_60-14 project, Power Board electronics                           */
#define PROJECT_NAME_H_BIO_45_60_14_PB                                       32

/* ************************************************************************* */

/* WM Asko professional project, User Interface electronics                  */
#define PROJECT_NAME_WM_ASKO_PROF_UI                                         40

/* WM Asko professional project, Power Board electronics, main MCU           */
#define PROJECT_NAME_WM_ASKO_PROF_PB_MAIN_MCU                                41

/* WM Asko professional project, Power Board electronics, power MCU          */
#define PROJECT_NAME_WM_ASKO_PROF_PB_POWER_MCU                               42

/* ************************************************************************* */

/* HIH PIND project, slave electronics                                       */
#define PROJECT_NAME_HIH_PIND_SLAVE                                          50

/* ************************************************************************* */

/* DW60.3 project, User Interface electronics (Top)                          */
#define PROJECT_NAME_DW60_3_UI_ST_MCU (aFOTA upgradable)                     60
/* DW60.3 project, User Interface electronics                                */
#define PROJECT_NAME_DW60_3_UI_CYPRESS_MCU (NOT upgradable via aFOTA)        61
/* DW60.3 project, GCU-P electronics, Main microcontroller                   */
#define PROJECT_NAME_DW60_3_GCUP_MAIN                                        62
/* DW60.3 project, GCU-P electronics, BLDC motor control microcontroller     */
#define PROJECT_NAME_DW60_3_GCUP_BLDC                                        63

/* DW60.2 project, User Interface electronics (Top)                          */
#define PROJECT_NAME_DW60_2_UI                                               64
/* DW60.2 project, GCU-P electronics, Main microcontroller                   */
#define PROJECT_NAME_DW60_2_GCUP_MAIN                                        65
/* DW60.2 project, GCU-P electronics, BLDC motor control microcontroller     */
#define PROJECT_NAME_DW60_2_BLDC                                             66

/* DW60.1 project, User Interface electronics (Front)                        */
#define PROJECT_NAME_DW60_1_UI                                               67
/* DW60.1 project, GCU-P electronics, Main microcontroller                   */
#define PROJECT_NAME_DW60_1_GCUP_MAIN                                        68
/* DW60.1 project, GCU-P electronics, BLDC motor control microcontroller     */
#define PROJECT_NAME_DW60_1_BLDC                                             69

/* ************************************************************************* */

/* WM22 (W-WaveEn-22) project, Power Control Unit                            */
#define PROJECT_NAME_WM22_PCU                                                70

/* WM22 (W-WaveEn-22) project, Motor Control Unit                            */
#define PROJECT_NAME_WM22_MCU                                                71

/* WM22 (W-WaveEn-22) project, User Interface electronics                    */
#define PROJECT_NAME_WM22_UI                                                 72

/* ************************************************************************* */

/* DW50 project, GCU-L electronics                                            */
#define PROJECT_NAME_DW50_GCUL                                               80

/* DW50 project, GCU-P electronics, Main microcontroller (N/A)               */
#define PROJECT_NAME_DW50_GCUP_MAIN                                          81

/* DW50 project, GCU-P electronics, BLDC motor control microcontroller (N/A) */
#define PROJECT_NAME_DW50_GCUP_BLDC                                          82

/* DW50.1 project, UI electronics (Front)                                    */
#define PROJECT_NAME_DW50_1_F_UI                                             83

/* DW50.1 project, UI electronics (Top)                                      */
#define PROJECT_NAME_DW50_1_T_UI                                             84

/* DW50.2 project, UI electronics (Front)                                    */
#define PROJECT_NAME_DW50_2_F_UI                                             85

/* DW50.2 project, UI electronics (Top)                                      */
#define PROJECT_NAME_DW50_2_T_UI                                             86

/* ************************************************************************* */

/* Enter new project here                                                    90
                                                                             91
                                                                             92
                                                                            ...
                                                                             */


/* ************************************************************************* */

#endif /* PROJECT_NAMES_H */

/* [] END OF FILE */
