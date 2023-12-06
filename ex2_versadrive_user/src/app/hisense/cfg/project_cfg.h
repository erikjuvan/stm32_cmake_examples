//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        project_cfg.h
// CREATION DATE:      2019/12/19
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2020/10/29
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Configuration of project functionality
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2020 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef PROJECT_CFG_H
#define PROJECT_CFG_H

#include "project_cfg_common.h"

/* ********************************************************************************************************
 */

/* Functionality enabling */

#if (NUMBER_OF_WIFI_INSTANCES > 0)
#define HISENSE_PROTOCOL_USE 1
#else
#define HISENSE_PROTOCOL_USE 0
#endif

#if ((NUMBER_OF_FORWARD_PORTS > 0) && (HISENSE_PROTOCOL_USE == 1))
#define FORWARDING_USE 1
#else
#define FORWARDING_USE 0
#endif

#define REPLY_ADDRESSMISMATCH_MESSAGES \
    1 /* Set to 1, if only one device is present on the communication bus. */
#define APPLICATION_DEBUG_USE    0
#define LCD_USE                  0
#define IOCONTROL_USE            0

#define PROJECT_TYPE             PROJECT_TYPE_APPLICATION
#define DOUBLE_BUFFERING         0
#define EXECUTING_FROM_RAM       DOUBLE_BUFFERING

#define MASTER_FUNCTIONALITY_USE 1

/* ********************************************************************************************************
 */

#endif /* PROJECT_CFG_H */
/* [] END OF FILE */
