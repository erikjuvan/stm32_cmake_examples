/**
 * @file     eps.h
 * @brief    Electronic Pressure Sensors (EPS) module, also known as water level sensor module
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup EPS_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _EPS_H_
#define _EPS_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <stdbool.h>
#include "eps_common.h"
#include "lock.h"
#include "drain.h"
#include "valve.h"


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Initialization status
 */
typedef enum
{
    eEPS_INIT_OK          = 0x00,
    eEPS_INIT_TIMER_FAIL  = 0x01,
    eEPS_INIT_GPIO_FAIL   = 0x02,
} eps_init_status_t;

/**
 * Timer division configuration (in standby the clock gets divided by 4)
 */
typedef enum
{
    EPS_TIMER_FREQ_NODIV,
    EPS_TIMER_FREQ_DIV4
} eps_timer_freq_div_t;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
eps_init_status_t eps_init        (void);
void              eps_proc        (lock_state_t lock, drain_state_t drain, valve_state_t valve);
void              eps_timer_isr   (void);
void              eps_power_up    (void);
void              eps_power_down  (void);
eps_level_t       eps_get_level   (void);
uint32_t          eps_get_period  (void);
int32_t           eps_get_sig_um  (void);
bool              eps_is_level_empty(void);
uint32_t          eps_get_isr_cnt (void);
bool              eps_is_error    (void);
bool              eps_is_plausible(void);
bool              eps_is_safe     (void);
void              eps_set_timer_mode(eps_timer_freq_div_t div);
bool              eps_calibration_timer_freq_set(uint32_t measured_timer_freq);
void              eps_calibration_timer_freq_reset(void);
bool              eps_calibration_eps_freq_offset_set(int32_t measured_eps_freq_offset_mHz);
void              eps_calibration_eps_freq_offset_reset(void);

#ifdef __cplusplus
extern "C" {
#endif

#endif // _EPS_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

