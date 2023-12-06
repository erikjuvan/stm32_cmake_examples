//-----------------------------------------------------------------------------
// PROJECT:            Gorenje [FW update]
//-----------------------------------------------------------------------------
// MODULE NAME:        timer.h
// CREATION DATE:      2019/09/25
// AUTHOR:             Borut Wagner
// LAST CHANGED DATE:  2022/03/04
// LAST CHANGED BY:    Borut Wagner
// REVISION:           1.0
// DESCRIPTION:
//   Millisecond timer implementation API
// ----------------------------------------------------------------------------
// Copyright (C) 2019-2022 by Gorenje d.o.o.
// All rights reserved.
// ----------------------------------------------------------------------------

#ifndef TIMER_H
#define TIMER_H

#include "cfg/project_cfg.h"
#include "types.h"
#include "Platform/proc.h"


typedef struct timer_t
{
    uint8_t   started;        /* Timer is running */
    uint32_t  value;          /* Free running counter value when the timer_start() was called */
} TIMER;


// DESCRIPTION:
//   Initialization of 1 ms timer functionality.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
//void timer_init(void);


// DESCRIPTION:
//   Deinitialization of 1 ms timer functionality.
// INPUTS:
//   --
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_deinit(void);


// DESCRIPTION:
//   Starting of timer (start of time measurement).
// INPUTS:
//   timer - Timer structure, instance of a timer which holds timer information.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
PROC_RAM_FUNCTION_ALWAYS_INFRONT void timer_start(TIMER *timer);


// DESCRIPTION:
//   Stopping the timer, cancelling of time measurement.
// INPUTS:
//   timer - Timer structure, instance of a timer which holds timer information.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_stop(TIMER *timer);


// DESCRIPTION:
//   Stopping of timer, cancelling of time measurement and clearing timer value.
// INPUTS:
//   timer - Timer structure, instance of a timer which holds timer information.
// OUTPUT:
//   --
// RETURNS:
//   --
// ----------------------------------------------------------------------------
void timer_clear(TIMER *timer);


// DESCRIPTION:
//   Measures time elapsed from calling timer_start() till now.
// INPUTS:
//   timer - Timer structure, instance of a timer which holds timer information.
// OUTPUT:
//   --
// RETURNS:
//   Time in milliseconds elapsed from calling timer_start() till now.
// ----------------------------------------------------------------------------
uint32_t timer_elapsed(TIMER *timer);


#endif /* TIMER_H */


/* [] END OF FILE */
