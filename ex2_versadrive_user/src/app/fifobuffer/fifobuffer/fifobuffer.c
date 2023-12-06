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
 * @addtogroup FIFOBUFFER
 * @{ <!-- BEGIN GROUP -->
 *
 * All functions that are used inside module and not relevant to user.
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "fifobuffer.h"
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup FIFOBUFFER_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////

#if (FIFO_USE_MALLOC == 1)
////////////////////////////////////////////////////////////////////////////////
/**
 *  Create FIFO instance that can hold \p number_of_elements many elements
 *  where \p size_of_element bytes are reserved per element and store the resultant
 *  instance to \p buffer.
 *
 * @param[in,out]   buffer                  Pointer to FIFO instance.
 * @param[in]       number_of_elements      Number of elements FIFO can hold.
 * @param[in]       size_of_element         Size [byte] of element.
 * @return          #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t
fifo_create(fifo_t *const buffer, uint16_t number_of_elements, uint16_t size_of_element)
#else
////////////////////////////////////////////////////////////////////////////////
/**
 *  Create FIFO instance with storage at \p storage that can hold \p
 *  number_of_elements many elements where \p size_of_element bytes are reserved
 *  per element and store the resultant instance to \p buffer.
 *
 * @note
 *  \p storage must be at least \p number_of_elements * \p size_of_element
 *  large [byte].
 *
 * @param[in,out]   buffer                  Pointer to FIFO instance.
 * @param[in]       storage                 Place where elements are stored.
 * @param[in]       number_of_elements      Number of elements FIFO can hold.
 * @param[in]       size_of_element         Size [byte] of element.
 * @return          #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_create(
    fifo_t *const buffer, const uint8_t *const storage, uint16_t number_of_elements,
    uint16_t size_of_element
)
#endif
{
    fifo_status_t status = eFIFO_OK;

    // Check for valid buffer pointer
    if (buffer == NULL)
    {
        status = eFIFO_POINTER_ERROR;
    }
    else if ((number_of_elements * size_of_element) == 0)
    {
        status = eFIFO_WRONG_LENGTH;
    }
    else
    {
#if (FIFO_USE_MALLOC == 1)
        // Allocate memory needed for buffer
        buffer->p_data = malloc(
            number_of_elements * size_of_element
        ); /* parasoft-suppress MISRAC2012-DIR_4_12-a "Used only in creation/desctruction of FIFO
              buffer" */

        // Check for valid buffer pointer
        if (buffer->p_data == NULL)
        {
            status = eFIFO_MALLOC_ERROR;
        }
        else
        {
#else
        if (storage == NULL)
        {
            status = eFIFO_POINTER_ERROR;
        }
        else
        {
            buffer->p_data = (uint8_t *)storage;
#endif
            // Initialize buffer input index
            buffer->head = 0;

            // Initialize buffer output index
            buffer->tail = 0;

            // Initialize buffer size
            buffer->number_of_elements = number_of_elements;

            // Initialize element size
            buffer->size_of_element = size_of_element;

            // Initialite buffer fill status
            buffer->b_is_full = 0;

            // Initialize buffer status
            buffer->b_is_created = 1;

            status = eFIFO_OK;
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Destroy FIFO instance \p buffer by freeing its storage.
 *
 * @param[in,out]   buffer  Pointer to FIFO instance.
 * @return          void.
 */
////////////////////////////////////////////////////////////////////////////////
void fifo_destroy(fifo_t *const buffer)
{
    if (buffer != NULL)
    {
#if (FIFO_USE_MALLOC == 1)
        free(buffer->p_data); /* parasoft-suppress MISRAC2012-DIR_4_12-a "Used only in
                                 creation/desctruction of FIFO buffer" */
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Add \p data to FIFO instance \p buffer where number of bytes to add is based
 *  on FIFO creation parameters.
 *
 * @param[in,out]   buffer      Pointer to FIFO instance.
 * @param[in]       data        Pointer to data to add.
 * @return          #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_add(fifo_t *const buffer, const void *const data)
{
    fifo_status_t status = eFIFO_OK;
    uint16_t      next_head;
#if (FIFO_ALLOW_OVERFLOW == 1)
    uint16_t next_tail;
#endif

#if (FIFO_USE_MEMCPY == 0)
    uint8_t *p_data;
    uint16_t i;
#endif

    // Check for valid buffer pointer
    if ((buffer == NULL) || (data == NULL))
    {
        status = eFIFO_POINTER_ERROR;
    }
    else if (buffer->b_is_created != 1)
    {
        status = eFIFO_NOT_CREATED;
    }
    else
    {
#if (FIFO_PROTECT == 1)
        __disable_irq();
#endif
        // next head is where head will point after adding element
        next_head = buffer->head + 1;
        if (next_head >= buffer->number_of_elements)
        {
            next_head = 0;
        }

        // check if buffer is full
        if ((buffer->head == buffer->tail) && (buffer->b_is_full == 1))
        {
            status = eFIFO_FULL;
        }
        else
        {
            // copy data to buffer
#if (FIFO_USE_MEMCPY == 1)
            memcpy(
                &buffer->p_data[buffer->head * buffer->size_of_element], data,
                buffer->size_of_element
            );
#else
            p_data              = &buffer->p_data[buffer->head * buffer->size_of_element];
            uint8_t *p_new_data = (uint8_t *)data;

            for (i = 0; i < buffer->size_of_element; i++)
            {
                *p_data++ = *(uint8_t *)p_new_data++;
            }
#endif

            // increment head
            buffer->head = next_head;

            // increment tail or set full
            if (buffer->head == buffer->tail)
            {
#if (FIFO_ALLOW_OVERFLOW == 1)
                // increment tail
                next_tail = buffer->tail + 1;
                if (next_tail >= buffer->number_of_elements)
                {
                    next_tail = 0;
                }
                buffer->tail = next_tail;
#else
                buffer->b_is_full = 1;
#endif
            }
        }

#if (FIFO_PROTECT == 1)
        __enable_irq();
#endif
    }
    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Get bytes from FIFO instance \p buffer and store them to \p data where number
 *  of bytes to retrieve is based on FIFO creation parameters.
 *
 * @param[in,out]   buffer      Pointer to FIFO instance.
 * @param[out]      data        Pointer to where retrieved data is stored.
 * @return          #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_get(fifo_t *const buffer, void *const data)
{
    fifo_status_t status = eFIFO_OK;
    uint16_t      next_tail;
#if (FIFO_USE_MEMCPY == 0)
    uint8_t *p_data;
    uint16_t i;
#endif

    // Check for valid buffer and data pointer
    if ((buffer == NULL) || (data == NULL))
    {
        status = eFIFO_POINTER_ERROR;
    }
    else if (buffer->b_is_created != 1)
    {
        status = eFIFO_NOT_CREATED;
    }
    else
    {
#if (FIFO_PROTECT == 1)
        __disable_irq();
#endif

        // Is buffer empty?
        if ((buffer->tail == buffer->head) && (buffer->b_is_full == 0))
        {
            status = eFIFO_EMPTY;
        }
        else
        {
            // copy data from buffer
#if (FIFO_USE_MEMCPY == 1)
            memcpy(
                data, &buffer->p_data[buffer->tail * buffer->size_of_element],
                buffer->size_of_element
            );
#else
            p_data              = &buffer->p_data[buffer->tail * buffer->size_of_element];
            uint8_t *p_new_data = (uint8_t *)data;

            for (i = 0; i < buffer->size_of_element; i++)
            {
                *(uint8_t *)p_new_data++ = *p_data++;
            }
#endif
            // Increment tail
            next_tail = buffer->tail + 1;
            if (next_tail >= buffer->number_of_elements)
            {
                next_tail = 0;
            }
            buffer->tail = next_tail;

            // Set to not full
            buffer->b_is_full = 0;

            status = eFIFO_OK;
        }

#if (FIFO_PROTECT == 1)
        __enable_irq();
#endif
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Clear FIFO instance \p buffer storage.
 *
 * @param[in,out]   buffer      Pointer to FIFO instance.
 * @return          #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_clear(fifo_t *const buffer)
{
    fifo_status_t status = eFIFO_OK;

    // Check for valid buffer pointer
    if (buffer == NULL)
    {
        status = eFIFO_POINTER_ERROR;
    }
    else
    {
#if (FIFO_PROTECT == 1)
        __disable_irq();
#endif

        // reset all indexes, pointers and data count
        buffer->head      = 0;
        buffer->tail      = 0;
        buffer->b_is_full = 0;

#if (FIFO_PROTECT == 1)
        __enable_irq();
#endif
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Get number of used bytes in FIFO instance \p buffer.
 *
 * @param[in]   buffer      Pointer to FIFO instance.
 * @param[out]  value       Pointer to where value (used bytes) gets written.
 * @return      #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_count(const fifo_t *const buffer, uint16_t *const value)
{
    fifo_status_t status = eFIFO_OK;
    int32_t       temp_count;

    // invalid pointer
    if ((buffer == NULL) || (value == NULL))
    {
        status = eFIFO_POINTER_ERROR;
    }
    else
    {
#if (FIFO_PROTECT == 1)
        __disable_irq();
#endif

        temp_count = (buffer->head - buffer->tail);

        if (temp_count < 0)
        {
            temp_count += buffer->number_of_elements;
        }

        if (temp_count == 0)
        {
            if (buffer->b_is_full == 1)
            {
                temp_count = buffer->number_of_elements;
            }
        }

        *value = (uint16_t)temp_count;

#if (FIFO_PROTECT == 1)
        __enable_irq();
#endif
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 *  Get number of free bytes in FIFO instance \p buffer.
 *
 * @param[in]   buffer      Pointer to FIFO instance.
 * @param[out]  value       Pointer to where value (free bytes) gets written.
 * @return      #fifo_status_t.
 */
////////////////////////////////////////////////////////////////////////////////
fifo_status_t fifo_space(const fifo_t *const buffer, uint16_t *const value)
{
    fifo_status_t status = eFIFO_OK;
    int32_t       temp_count;

    // invalid pointer
    if ((buffer == NULL) || (value == NULL))
    {
        status = eFIFO_POINTER_ERROR;
    }
    else
    {
#if (FIFO_PROTECT == 1)
        __disable_irq();
#endif

        temp_count = (buffer->head - buffer->tail);

        if (temp_count < 0)
        {
            temp_count += buffer->number_of_elements;
        }

        if (temp_count == 0)
        {
            if (buffer->b_is_full == 1)
            {
                temp_count = buffer->number_of_elements;
            }
        }

        *value = (buffer->number_of_elements - temp_count);

#if (FIFO_PROTECT == 1)
        __enable_irq();
#endif
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
