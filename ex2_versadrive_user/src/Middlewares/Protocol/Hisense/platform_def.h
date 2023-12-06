//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        platform_def.h
// CREATION DATE:      2019/12/19
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/06/24
// LAST CHANGED BY:    Borut Wagner (Blaz Plaskan - merge)
// REVISION:           1.0
// DESCRIPTION:
//   Definitions of supported platforms
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2021 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PLATFORM_DEF_H
#define PLATFORM_DEF_H

/* Cypress CY8C5888: */
#define PLATFORM_CYPRESS_PSOC5                  1

/* Cypress CY8C4127 (CYDEV_CHIP_MEMBER_4V) and Cypress CY8C4148 (CYDEV_CHIP_MEMBER_4AB): */
#define PLATFORM_CYPRESS_PSOC4                  2

/* STMicroelectronics STM8S207: */
#define PLATFORM_STM8_STM8S207                  3

/* Cypress MB9AF114MA: */
#define PLATFORM_CYPRESS_MB9AF114MA             4

/* Cypress MB9BF121J: */
#define PLATFORM_CYPRESS_MB9BF121J              5

/* NXP LPC54607J512: */
#define PLATFORM_NXP_LPC546XX                   6

/* Cypress CY8C4149 (Eclipse IDE for Modustoolbox): */
#define PLATFORM_CYPRESS_PSOC4100SMAX           7

/* STMicroelectronics STM32G0xx family: */
#define PLATFORM_STM_STM32G0xx                  8

/* STMicroelectronics STM32F4xx family: */
#define PLATFORM_STM_STM32F4xx                  9

/* ST STM32G4xx: */
#define PLATFORM_STM32_STM32GXX                 10

/* Define other platforms here */

/*
PLATFORM_<n> is a search token.
Increase PLATFORM_<n> to PLATFORM_<n+1> on each platform implementation.
This indicates that implementation for new platform is finished.
*/
#define PLATFORM_11                             11

/* Placeholder and templates for new platforms: */
#define PLATFORM_template                     255


#endif /* PLATFORM_DEF_H */

/* [] END OF FILE */
