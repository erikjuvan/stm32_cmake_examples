/******************************************************************************
 *
 * @file        mat_trace.h
 * @brief       MAT trace communication for project TD-R290
 * @author      Erik Juvan
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

#ifndef _MAT_TRACE_H_
#define _MAT_TRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void mat_trace_write(uint8_t *data, int len);
void mat_trace_process();

#ifdef __cplusplus
}
#endif

#endif // _MAT_TRACE_H_
