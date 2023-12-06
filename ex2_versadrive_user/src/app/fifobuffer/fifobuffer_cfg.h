////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2019
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////

/*!
 * @file     fifobuffer_cfg.h
 * @note
 * @author   Marko Rman
 * @date     09.05.2019
 * @brief    fifobuffer header
 */

#ifndef __FIFOBUFFER_CFG_H
#define __FIFOBUFFER_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/* Example how to use wih malloc
----------------------------------------
1. Set define FIFO_USE_MALLOC to 1
2. Create buffer structure
    fifo_t fifo_buffer;
    fifo_status_t fifo_status;
3. Create fifo buffer
    fifo_status = fifo_create(&fifo_buffer, <number of elements>, <size of element in bytes>);
*/
/* Example how to use without malloc
----------------------------------------
1. Set define FIFO_USE_MALLOC to 0
2. Create buffer structure and buffer array
    fifo_t fifo_buffer;
    fifo_status_t fifo_status;
    uint8_t fifo_storage[<number of elements> * <size of element in bytes>];
3. Create fifo buffer
    fifo_status = fifo_create(&fifo_buffer, fifo_storage, <number of elements>, <size of element in
bytes>);
*/
#define FIFO_USE_MALLOC     1

/* How to use Overflow
----------------------------------------
if you set define FIFO_ALLOW_OVERFLOW to 1
if the buffer is full, adding new byte will overwrite oldest data
tail index will also be updated
*/
#define FIFO_ALLOW_OVERFLOW 1

/* How to use Protection
----------------------------------------
Set define FIFO_PROTECT to 1
to use fifo add and get with disabling interrupt
this should be only used if there is multiple entry point for any
of fifo_add or fifo_get functions
*/
#define FIFO_PROTECT        0

/* How to use memcpy
----------------------------------------
Set define FIFO_MEMCPY to 1 to use memcpy function
Set define FIFO_MEMCPY to 0 to use for loop instead
*/
#define FIFO_USE_MEMCPY     1

#endif
