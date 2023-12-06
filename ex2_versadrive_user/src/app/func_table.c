/*
 * function_table.c
 *
 *  Created on: 28 Oct 2021
 *      Author: dusans
 */

#include "func_table.h"
#include "main.h"
#include "relay.h"
#include "trace/trace.h"

// pointer to struct with pointers to safe functions to be called by this app
volatile const safe_func_table_t *const pg_safe_func_table =
    (volatile const safe_func_table_t *const)(&_MEMPART_FLASH_SAFE_FNC_TABLE_ADDR);

// struct with pointers to user functions to called called by safe app
volatile const user_func_table_t g_user_func_table __attribute__((section(".user_ft"))) = {
    // Init
    .fp_User_Context_Init = user_context_init,
    .fp_User_App_Init     = user_app_init,

    // Main loop function
    .fp_User_App_Loop = user_app_loop,

    // Black channel
    .fp_User_BlackCh_Send_Frame = NULL,

    .fp_User_trace_print = TRACE_Print,
};
