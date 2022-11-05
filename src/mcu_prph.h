/**
 * @file     mcu_prph.h
 * @brief    MCU peripheral check module header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MCU_PRPH_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_PRPH_H_
#define _MCU_PRPH_H_

////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * Module status
 */
typedef enum
{
    eMCU_PRPH_OK             = 0x00,  /**< Evertyhing Ok. */
    eMCU_PRPH_RCC_FAIL       = 0x01,  /**< RCC check failed. */
    eMCU_PRPH_SYSTICK_FAIL   = 0x02,  /**< SysTick check failed. */
    eMCU_PRPH_IWDG_FAIL      = 0x04,  /**< IWDG check failed. */
    eMCU_PRPH_WWDG_FAIL      = 0x08,  /**< WWDG check failed. */
    eMCU_PRPH_RAM_PE_FAIL    = 0x10,  /**< RAM parity check enabled failed. */
    eMCU_PRPH_STL_TIM16_FAIL = 0x20,  /**< TIM16 check failed. */
    eMCU_PRPH_EPS_FAIL       = 0x40,  /**< EPS check failed. */
    eMCU_PRPH_LOCK_FAIL      = 0x80,  /**< LOCK check failed. */
    eMCU_PRPH_DRAIN_FAIL     = 0x100  /**< DRAIN check failed. */
} mcu_prph_status_t;


////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
void mcu_prph_proc(void);
void mcu_prph_proc_standby(void);
mcu_prph_status_t mcu_prph_get_status(void);


#endif // _MCU_PRPH_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
