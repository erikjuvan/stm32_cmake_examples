/******************************************************************************
 *
 * @file        mat.h
 * @brief       MAT communication for project TD-R290
 * @author      Blaz Baskovc
 * @author      Erik Juvan
 * @copyright   Copyright Gorenje d.d.
 *
 *****************************************************************************/

#ifndef _MAT_H_
#define _MAT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>

/* Public Function Prototypes ---------------------------------------------- */
bool     mat_handle_req(uint8_t *data, uint16_t length);
uint8_t *mat_reponse_buffer_get();
uint16_t mat_reponse_buffer_length();
uint16_t mat_response_buffer_append(void *data, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* _MAT_H_ */

/* EOF --------------------------------------------------------------------- */
