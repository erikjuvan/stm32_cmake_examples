////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2020
// EMTAS, EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
*@file      canopen.h
*@brief     canopen module setup
*@author    Zdenko Mezgec
*@date      14.12.2020
*@version   V1.3.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup CANOPEN_API
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef CANOPEN_H
#define CANOPEN_H 1

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "co_datatype.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////
/** canopen init  return type.
 * Return type enumeration for co_init
*/
typedef enum
{
    eCANOPEN_INIT_OK             = 0x00, /**< successfully initialized    */
    eCANOPEN_INIT_MUTEX_ERR      = 0x01, /**< failed to initialize mutex  */
    eCANOPEN_INIT_TIMER_ERR      = 0x02, /**< failed to initialize timer  */
    eCANOPEN_INIT_STACK_ERR      = 0x03, /**< failed to initialize stack  */
    eCANOPEN_INIT_EVENT_ERR      = 0x04, /**< failed to initialize events */
    eCANOPEN_INIT_CAN_ERR        = 0x05, /**< failed to initialize can    */
    eCANOPEN_INIT_MASTER_ERR     = 0x06, /**< failed to initialize master */
    eCANOPEN_INIT_TASK_ERR       = 0x07, /**< failed to initialize task   */
} canopen_init_t;

/** sdowrite_callback return type.
 * Return type enumeration for sdo write callback register
*/
typedef enum
{
    eCANOPEN_SDO_REG_OK             = 0x00, /**< successfully registered */
    eCANOPEN_SDO_REG_ERR            = 0x01, /**< can't register desired fnc; out of memory */
} canopen_sdo_reg_t;

/** domain_read_data_callback return type.
 * Return type enumeration for domain read data callback register
*/
typedef enum
{
    eCANOPEN_DOMAIN_READ_REG_OK             = 0x00, /**< successfully registered */
    eCANOPEN_DOMAIN_READ_REG_ERR            = 0x01, /**< can't register desired fnc; out of memory */
} canopen_domain_read_data_reg_t;

/** callback register fnc
 * Typedef for co callback sdo write
*/
typedef void (*co_callback_fnc)(const void *pdata);

/** callback register fnc
 * Typedef for domain read
*/
typedef RET_T (*co_domain_read_data_callback_fnc)(uint16_t index, uint8_t subIndex, void *pData, uint32_t  size);
////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
canopen_init_t      co_init (void);
canopen_sdo_reg_t                co_register_sdowrite_callback      (UNSIGNED16  index, UNSIGNED8   subindex, co_callback_fnc);
canopen_domain_read_data_reg_t   co_register_domainreaddata_callback(UNSIGNED16  index, UNSIGNED8   subindex, co_domain_read_data_callback_fnc);
void                co_setup_domain_size         (CO_DOMAIN_PTR * p_co_domain, uint32_t domain_data_size);
void co_hndl(void);

#endif /* CANOPEN_H */
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
