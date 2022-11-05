/**
 * @file     test_switch.c
 * @brief    Enable/disable specific module tests for qualification tests (standards compliance testing).
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "test_switch.h"


#ifdef QUALIFICATION_TEST

test_switch_t g_test_switch = {
    .stl.wdg_test = false,
    .eps.isr_fail = false,
    .lock.isr_fail = false,
    .lock.lock_req_without_ui_start = false,
    .drain.off = false,
};

uint32_t ram_parity_array[4] __attribute__ ((section ("._ram_uninit")));

#endif
