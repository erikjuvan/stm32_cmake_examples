////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2019
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
 * @file     fifobuffer.c
 * @brief    FIFO buffer functions.
 * @author   Marko Rman
 * @date     09.05.2019
 * @version  V1.6.0
 */
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup FIFOBUFFER_API
 * @{ <!-- BEGIN GROUP -->
 *
 * Following module is part of API which is available to the user of FIFOBUFFER Module.
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef __FIFOBUFFER_H
#define __FIFOBUFFER_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "../fifobuffer_cfg.h"
#include <stdint.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 *  FIFO buffer structure.
 */
typedef struct
{
    uint16_t head;               /**< Index in FIFO where next data gets stored to. */
    uint16_t tail;               /**< Index in FIFO where next data gets read from. */
    uint16_t number_of_elements; /**< Number of elements FIFO can hold. */
    uint16_t size_of_element;    /**< Size [byte] of each element in FIFO. */
    uint8_t *p_data;             /**< Pointer to stored data. */
    uint8_t  b_is_full;          /**< Indicates if FIFO is full or not. */
    uint8_t  b_is_created;       /**< Indicates if FIFO is created and can be used. */
} fifo_t;

/**
 *  Return status enumeration.
 */
typedef enum
{
    eFIFO_OK = 0,        /**< Successful completion. */
    eFIFO_FULL,          /**< FIFO is full. */
    eFIFO_EMPTY,         /**< FIFO is empty. */
    eFIFO_WRONG_LENGTH,  /**< Either \p number_of_elements of \p size_of_element is 0. */
    eFIFO_POINTER_ERROR, /**< Pointer has NULL value. */
    eFIFO_MALLOC_ERROR,  /**< Calling malloc() returned error. */
    eFIFO_NOT_CREATED,   /**< FIFO has not yet been created with fifo_create(). */
} fifo_status_t;

////////////////////////////////////////////////////////////////////////////////
// Function Prototypes
////////////////////////////////////////////////////////////////////////////////
#if (FIFO_USE_MALLOC == 1)
fifo_status_t
fifo_create(fifo_t *const buffer, uint16_t number_of_elements, uint16_t size_of_element);
#else
fifo_status_t fifo_create(
    fifo_t *const buffer, const uint8_t *const storage, uint16_t number_of_elements,
    uint16_t size_of_element
);
#endif
void          fifo_destroy(fifo_t *const buffer);
fifo_status_t fifo_add(fifo_t *const buffer, const void *const data);
fifo_status_t fifo_get(fifo_t *const buffer, void *const data);
fifo_status_t fifo_clear(fifo_t *const buffer);
fifo_status_t fifo_count(const fifo_t *const buffer, uint16_t *const value);
fifo_status_t fifo_space(const fifo_t *const buffer, uint16_t *const value);

#endif // __FIFOBUFFER_H
////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
