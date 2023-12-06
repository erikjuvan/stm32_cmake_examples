////////////////////////////////////////////////////////////////////////////////
// COPYRIGHT (c) 2021
// EMSISO
// All rights reserved.
////////////////////////////////////////////////////////////////////////////////
/**
* @file     test_switch_common.h
* @brief    Test Switches type definitions for USER and SAFE.
* @author   Dusan Slavinec
* @date     14.10.2021
* @version  V1.0.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
* @addtogroup TEST_SWITCH_COMMON
* @{ <!-- BEGIN GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_SWITCH_COMMON_H
#define __TEST_SWITCH_COMMON_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Status of setting test switches.
 */
typedef enum
{
     eTEST_SWITCH_NOT_COMPILED = 0
    ,eTEST_SWITCH_SET
} test_switch_status_t;

/* parasoft-begin-suppress MISRAC2012-RULE_6_1-a "False positive, bit field is uint32_t" */
/**
 * Structure holding functionality test switches.
 */
typedef struct
{
    union
    {
        struct
        {
            uint32_t  stl_startup : 1
                     ,stl_runtime : 1
                     ,safe_loop   : 1
                     ,blkch_rxcrc : 1
                     ,blkch_seq   : 1
                     ,mu_app      : 1;
        };
        uint32_t flags;
    } test;

    union
    {
        struct
        {
            uint32_t  reset        : 1
                     ,switch_to_lp : 1
                     ,switch_to_np : 1;
        };
        uint32_t all;
    } cmd;
} test_switch_t;
/* parasoft-end-suppress MISRAC2012-RULE_6_1-a "False positive, bit field is uint32_t" */

#endif // __TEST_SWITCH_COMMON_H
////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////
