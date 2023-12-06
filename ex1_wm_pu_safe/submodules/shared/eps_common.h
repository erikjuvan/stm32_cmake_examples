#ifndef _EPS_COMMON_H_
#define _EPS_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////

#define EPS_SIGNAL_MIN_MM   -80 /**< Min valid EPS level: -80mm */
#define EPS_SIGNAL_MAX_MM   370 /**< Max valid EPS level: 370mm */

/**
 * Timer frequency measurement (FCT test)
 *
 * Equation for the result of MCU timer freq measurement with respect to freq. gen. error: MCU_freq = f_mcu_actual * 1/f_gen_actual * f_gen_desired ->
 * -> e.g. 0.1% fgen error: Timer_freq = 99Hz * 1/100.1Hz * 100Hz = 98.9Hz. So we would measure our frequency as 98.9Hz, which is much better then thinking it was 100Hz.
 *
 * Min/Max allowable freq we can set (taking into account: max +-1% error, function can be called with any value in min/max range, and the actual
 * MCU freq can be as bad as specified in datasheet (15.88 < f < 16.08 MHz), we have to solve for equation: f_eps_calc = f_eps * f_mcu_calib / f_mcu_actual
 * min limit: f_mcu_calib_min = f_mcu_highest_possible_datasheet * 0.99 (-1%) -> f_mcu_calib_min = 16.08 * 0.99 -> f_mcu_calib_min = 15.92MHz
 * max limit: f_mcu_calib_max = f_mcu_lowest_possible_datasheet  * 1.01 (+1%) -> f_mcu_calib_max = 15.88 * 1.01 -> f_mcu_calib_max = 16.04MHz
 * One way to think about it in plain language is, if MCU freq is 15.88 MHz, what can we "think" our freq is to make a 1% error? It is simply 15.88 * 1.01.
 */
#define EPS_TIMER_DEFAULT_FREQ  16000000U /**< 16MHz */
#define EPS_TIMER_MIN_FREQ      15920000U /**< Minimum freq we can "think" we have. 16.08 * 0.99 -> f_mcu_calib_min = 15.92MHz. Note it is actually higher than min datasheet value, since we have to limit the worst case calibration outcome to 1%. */
#define EPS_TIMER_MAX_FREQ      16040000U /**< Maximum freq we can "think" we have. 15.88 * 1.01 -> f_mcu_calib_max = 16.04MHz. Note it is actually lower  than max datasheet value, since we have to limit the worst case calibration outcome to 1%. */

/**
 * EPS calibration (Production test)
 *
 * diff = nominal_value - measured_value -> e.g. diff = 625 - 622 = 3Hz -> eps_level = 622 + 3 = 625Hz
 *
 * SAFE limit of calibration offset is (NOTE: Equation for water level in mm is h=a-b*freq, so with falling freq the level is rising):
 * - f_cal_offset_max = abs((f_opening - f_eps_high) + f_some_margin + f_worst_case_total_tolerance(eps+timer));
 * - f_cal_offset_min = -f_worst_case_total_tolerance; Since there is no spill hazard when thinking there is more water than there actually is. Heater is not part of SAFE so
 * the min limit does not protect against destroying the heater.
 *
 * example 1: wst_cs_tol = +-40mm, f_op = 100mm, f_eps_high = 50 mm, margin = 0mm. This means that max_cal_offset can be (100 - 50 - 0) - 40 = 50 - 40 = 10mm. Anything more and having the "right" (meaning wrong)
 * level of water while calibrating can lead to a spill. E.g. if we allowed 15mm (which is still less than tolerance/2), then you could have 40 + 15 mm error = 55mm, so with water level 103mm - 55 = 48mm you would think EPS_LOW.
 * Another way to think about it is, you can have any ammount of water while calibrating, so the mistake you can make is = worst_case_tolerance + calibration_value. So if w_c_t = 18 mm, and you allow for the
 * calibration value to be 10mm, than the worst case mistake can be 18 + 10 = 28mm. So you have to solve for the equation (w_c_t + cal_val < f_at_open - f_eps_high - margin). ->
 * -> cal_val <  f_at_open - f_eps_high - margin - w_c_t
 * For our specific case the approximate values are: cal_val < 120 mm - 95mm - 5mm - 18mm = 2 mm. Parts of the equation that are fixed is the tolerance of EPS + timer, and level of door opening. There isn't too much
 * room for margin. Leaving us only with eps_high level which probably won't be changed just to allow for more calibration range. If we assume that timer has been calibrated in FCT than 2mm is almost good enough
 * to take care of EPS tolerance which is 1Hz (2.5mm). Also in that case the tolerance would drop significantly since the part of timer is much larger (+-1% - 6.25Hz). But that would assume that FCT was calibrated
 * perfectly which we probably can't assume. We probably have to assume the potential error added during FCT also. Also should we take into account the temperature threshold. But that would already push us over the
 * limit, since that would mean timer +-2Hz -> 2 * 6.25 * 2,5 = 31,25mm.
 */
#define EPS_MAX_CALIBRATION_OFFSET_FREQ_MILLIHZ  1000    /**< Summary: 25mm - 18mm - 5mm = 2mm -> / 2,5 = ~1Hz.  See long explanation above. */
#define EPS_MIN_CALIBRATION_OFFSET_FREQ_MILLIHZ  -7000   /**< 1% freq from MCU timer = 6.25Hz + 1Hz from EPS at 0mm water level ~= 7Hz */


////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 * Possible states of eps
 */
typedef enum
{
    eEPS_HIGH = 0,  /**< Water level above door edge (SAFE STATE, that's why it's zero) */
    eEPS_LOW  = 1,  /**< Water level below door edge */
} eps_level_t;


#ifdef __cplusplus
}
#endif

#endif
