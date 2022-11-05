/**
 * @file     mcu_prph_cfg.h
 * @brief    MCU peripheral configuration header
 * @author   Erik Juvan
 * @date     6.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MCU_PRPH_CFG_API
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

#ifndef _MCU_PRPH_CFG_H_
#define _MCU_PRPH_CFG_H_


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "stm32g0b1xx.h"


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////

/* RCC */
#define MCU_PRPH_RCC_CR_MASK                 0xffffffff
#define MCU_PRPH_RCC_CR_VALUE                0x3000500

#define MCU_PRPH_RCC_CFGR_MASK               0xffffffff // less could be used
#define MCU_PRPH_RCC_CFGR_VALUE              0x12

#define MCU_PRPH_RCC_PLLCFGR_MASK            0xffffffff
#define MCU_PRPH_RCC_PLLCFGR_VALUE           0x72020802

#define MCU_PRPH_RCC_IOPRSTR_MASK            0x0 // not used
#define MCU_PRPH_RCC_IOPRSTR_VALUE           0x0

#define MCU_PRPH_RCC_AHBRSTR_MASK            0x0 // not used
#define MCU_PRPH_RCC_AHBRSTR_VALUE           0x0

#define MCU_PRPH_RCC_APBRSTR1_MASK           0x0 // not used
#define MCU_PRPH_RCC_APBRSTR1_VALUE          0x0

#define MCU_PRPH_RCC_APBRSTR2__MASK          0x0 // not used
#define MCU_PRPH_RCC_APBRSTR2_VALUE          0x0

#define MCU_PRPH_RCC_IOPENR_MASK             (RCC_IOPENR_GPIOAEN_Msk | RCC_IOPENR_GPIOBEN_Msk | RCC_IOPENR_GPIOCEN_Msk)
#define MCU_PRPH_RCC_IOPENR_VALUE            (RCC_IOPENR_GPIOAEN_Msk | RCC_IOPENR_GPIOBEN_Msk | RCC_IOPENR_GPIOCEN_Msk)

#define MCU_PRPH_RCC_AHBENR_MASK             0x0 // not used
#define MCU_PRPH_RCC_AHBENR_VALUE            0x0

#define MCU_PRPH_RCC_APBENR1_MASK            (RCC_APBENR1_WWDGEN_Msk | RCC_APBENR1_TIM3EN_Msk)
#define MCU_PRPH_RCC_APBENR1_VALUE           (RCC_APBENR1_WWDGEN_Msk | RCC_APBENR1_TIM3EN_Msk)

#define MCU_PRPH_RCC_APBENR2_MASK            (RCC_APBENR2_TIM15EN_Msk)
#define MCU_PRPH_RCC_APBENR2_VALUE           (RCC_APBENR2_TIM15EN_Msk)

/* RCC - standby*/
#define MCU_PRPH_RCC_CR_STANDBY_MASK         0xffffffff
#define MCU_PRPH_RCC_CR_STANDBY_VALUE        0xd00

#define MCU_PRPH_RCC_CFGR_STANDBY_MASK       0xffffffff
#define MCU_PRPH_RCC_CFGR_STANDBY_VALUE      0x0

#define MCU_PRPH_RCC_PLLCFGR_STANDBY_MASK    0xffffffff
#define MCU_PRPH_RCC_PLLCFGR_STANDBY_VALUE   0x1000

#define MCU_PRPH_RCC_IOPRSTR_STANDBY_MASK    0x0
#define MCU_PRPH_RCC_IOPRSTR_STANDBY_VALUE   0x0

#define MCU_PRPH_RCC_AHBRSTR_STANDBY_MASK    0x0
#define MCU_PRPH_RCC_AHBRSTR_STANDBY_VALUE   0x0

#define MCU_PRPH_RCC_APBRSTR1_STANDBY_MASK   0x0
#define MCU_PRPH_RCC_APBRSTR1_STANDBY_VALUE  0x0

#define MCU_PRPH_RCC_APBRSTR2_STANDBY_MASK   0x0
#define MCU_PRPH_RCC_APBRSTR2_STANDBY_VALUE  0x0

#define MCU_PRPH_RCC_IOPENR_STANDBY_MASK     (RCC_IOPENR_GPIOAEN_Msk | RCC_IOPENR_GPIOBEN_Msk | RCC_IOPENR_GPIOCEN_Msk)
#define MCU_PRPH_RCC_IOPENR_STANDBY_VALUE    (RCC_IOPENR_GPIOAEN_Msk | RCC_IOPENR_GPIOBEN_Msk | RCC_IOPENR_GPIOCEN_Msk)

#define MCU_PRPH_RCC_AHBENR_STANDBY_MASK     0x0
#define MCU_PRPH_RCC_AHBENR_STANDBY_VALUE    0x0

#define MCU_PRPH_RCC_APBENR1_STANDBY_MASK    (RCC_APBENR1_WWDGEN_Msk | RCC_APBENR1_TIM3EN_Msk)
#define MCU_PRPH_RCC_APBENR1_STANDBY_VALUE   (RCC_APBENR1_WWDGEN_Msk | RCC_APBENR1_TIM3EN_Msk)

#define MCU_PRPH_RCC_APBENR2_STANDBY_MASK    (RCC_APBENR2_TIM15EN_Msk)
#define MCU_PRPH_RCC_APBENR2_STANDBY_VALUE   (0x0)

/* SysTick */
#define MCU_PRPH_SYSTICK_CTRL_MASK           0x7
#define MCU_PRPH_SYSTICK_CTRL_VALUE          0x7

#define MCU_PRPH_SYSTICK_LOAD_MASK           0xffffffff
#define MCU_PRPH_SYSTICK_LOAD_VALUE          0x7cff

/* SysTick - standby */
#define MCU_PRPH_SYSTICK_CTRL_STANDBY_MASK   0x7
#define MCU_PRPH_SYSTICK_CTRL_STANDBY_VALUE  0x7

#define MCU_PRPH_SYSTICK_LOAD_STANDBY_MASK   0xffffffff
#define MCU_PRPH_SYSTICK_LOAD_STANDBY_VALUE  0x1f3f

/* IWDG */
#define MCU_PRPH_IWDG_PR_MASK                IWDG_PR_PR_Msk
#define MCU_PRPH_IWDG_PR_VALUE               0x6

#define MCU_PRPH_IWDG_RLR_MASK               IWDG_RLR_RL_Msk
#define MCU_PRPH_IWDG_RLR_VALUE              0x64

#define MCU_PRPH_IWDG_WINR_MASK              IWDG_WINR_WIN_Msk
#define MCU_PRPH_IWDG_WINR_VALUE             0x64


/* WWDG */
#define MCU_PRPH_WWDG_CFR_MASK               (WWDG_CFR_W_Msk | WWDG_CFR_WDGTB_Msk | WWDG_CFR_EWI_Msk)
#define MCU_PRPH_WWDG_CFR_VALUE              0x327f

/* WWDG - standby */
#define MCU_PRPH_WWDG_CFR_STANDBY_MASK       (WWDG_CFR_W_Msk | WWDG_CFR_WDGTB_Msk | WWDG_CFR_EWI_Msk)
#define MCU_PRPH_WWDG_CFR_STANDBY_VALUE      0x227f

/* RAM PARITY */
#define MCU_PRPH_FLASH_OPTR_RAM_PARITY_MASK  FLASH_OPTR_RAM_PARITY_CHECK_Msk
#define MCU_PRPH_FLASH_OPTR_RAM_PARITY_VALUE 0x0


/* TIM16 - STL */
#define MCU_PRPH_TIM16_CR1_MASK              0xffffffff
#define MCU_PRPH_TIM16_CR1_VALUE             0x1

#define MCU_PRPH_TIM16_CR2_MASK              0xffffffff
#define MCU_PRPH_TIM16_CR2_VALUE             0x0

#define MCU_PRPH_TIM16_DIER_MASK             0xffffffff
#define MCU_PRPH_TIM16_DIER_VALUE            0x2

#define MCU_PRPH_TIM16_CCMR1_MASK            0xffffffff
#define MCU_PRPH_TIM16_CCMR1_VALUE           0xd

#define MCU_PRPH_TIM16_CCER_MASK             0xffffffff
#define MCU_PRPH_TIM16_CCER_VALUE            0x1

#define MCU_PRPH_TIM16_PSC_MASK              0xffffffff
#define MCU_PRPH_TIM16_PSC_VALUE             0x0

#define MCU_PRPH_TIM16_ARR_MASK              0xffffffff
#define MCU_PRPH_TIM16_ARR_VALUE             0xffff

#define MCU_PRPH_TIM16_RCR_MASK              0xffffffff
#define MCU_PRPH_TIM16_RCR_VALUE             0x0

#define MCU_PRPH_TIM16_BDTR_MASK             0xffffffff
#define MCU_PRPH_TIM16_BDTR_VALUE            0x0

#define MCU_PRPH_TIM16_DCR_MASK              0xffffffff
#define MCU_PRPH_TIM16_DCR_VALUE             0x0

#define MCU_PRPH_TIM16_DMAR_MASK             0xffffffff
#define MCU_PRPH_TIM16_DMAR_VALUE            0x1

#define MCU_PRPH_TIM16_AF1_MASK              0xffffffff
#define MCU_PRPH_TIM16_AF1_VALUE             0x1

#define MCU_PRPH_TIM16_TISEL_MASK            0xffffffff
#define MCU_PRPH_TIM16_TISEL_VALUE           0x1

/* EPS */
// GPIO POWER EN
#define MCU_PRPH_EPS_POWER_EN_MODER_MASK     GPIO_MODER_MODE2_Msk
#define MCU_PRPH_EPS_POWER_EN_MODER_VALUE    GPIO_MODER_MODE2_0

#define MCU_PRPH_EPS_POWER_EN_OTYPER_MASK    GPIO_OTYPER_OT2_Msk
#define MCU_PRPH_EPS_POWER_EN_OTYPER_VALUE   0x0

#define MCU_PRPH_EPS_POWER_EN_OSPEEDR_MASK   GPIO_OSPEEDR_OSPEED2_Msk
#define MCU_PRPH_EPS_POWER_EN_OSPEEDR_VALUE  0x0

#define MCU_PRPH_EPS_POWER_EN_PUPDR_MASK     GPIO_PUPDR_PUPD2_Msk
#define MCU_PRPH_EPS_POWER_EN_PUPDR_VALUE    0x0

#define MCU_PRPH_EPS_POWER_EN_AFRL_MASK      GPIO_AFRL_AFSEL2_Msk
#define MCU_PRPH_EPS_POWER_EN_AFRL_VALUE     0x0

// GPIO
#define MCU_PRPH_EPS_MODER_MASK              GPIO_MODER_MODE7_Msk
#define MCU_PRPH_EPS_MODER_VALUE             GPIO_MODER_MODE7_1

#define MCU_PRPH_EPS_OTYPER_MASK             GPIO_OTYPER_OT7_Msk
#define MCU_PRPH_EPS_OTYPER_VALUE            0x0

#define MCU_PRPH_EPS_OSPEEDR_MASK            GPIO_OSPEEDR_OSPEED7_Msk
#define MCU_PRPH_EPS_OSPEEDR_VALUE           0x0

#define MCU_PRPH_EPS_PUPDR_MASK              GPIO_PUPDR_PUPD7_Msk
#define MCU_PRPH_EPS_PUPDR_VALUE             0x0

#define MCU_PRPH_EPS_AFRL_MASK               GPIO_AFRL_AFSEL7_Msk
#define MCU_PRPH_EPS_AFRL_VALUE              (GPIO_AFRL_AFSEL7_0)

// Timer
#define MCU_PRPH_TIM3_CR1_MASK               0xffffffff
#define MCU_PRPH_TIM3_CR1_VALUE              0x5

#define MCU_PRPH_TIM3_CR2_MASK               0xffffffff
#define MCU_PRPH_TIM3_CR2_VALUE              0x0

#define MCU_PRPH_TIM3_SMCR_MASK              0xffffffff
#define MCU_PRPH_TIM3_SMCR_VALUE             0x64

#define MCU_PRPH_TIM3_DIER_MASK              0xffffffff
#define MCU_PRPH_TIM3_DIER_VALUE             0x5

#define MCU_PRPH_TIM3_CCMR1_MASK             0xffffffff
#define MCU_PRPH_TIM3_CCMR1_VALUE            0x102

#define MCU_PRPH_TIM3_CCMR2_MASK             0xffffffff
#define MCU_PRPH_TIM3_CCMR2_VALUE            0x0

#define MCU_PRPH_TIM3_CCER_MASK              0xffffffff
#define MCU_PRPH_TIM3_CCER_VALUE             0x13

#define MCU_PRPH_TIM3_PSC_MASK               0xffffffff
#define MCU_PRPH_TIM3_PSC_VALUE              0x1

#define MCU_PRPH_TIM3_ARR_MASK               0xffffffff
#define MCU_PRPH_TIM3_ARR_VALUE              0xffff

#define MCU_PRPH_TIM3_DCR_MASK               0xffffffff
#define MCU_PRPH_TIM3_DCR_VALUE              0x0

#define MCU_PRPH_TIM3_DMAR_MASK              0xffffffff
#define MCU_PRPH_TIM3_DMAR_VALUE             0x5

#define MCU_PRPH_TIM3_OR1_MASK               0xffffffff
#define MCU_PRPH_TIM3_OR1_VALUE              0x0

#define MCU_PRPH_TIM3_AF1_MASK               0xffffffff
#define MCU_PRPH_TIM3_AF1_VALUE              0x0

#define MCU_PRPH_TIM3_TISEL_MASK             0xffffffff
#define MCU_PRPH_TIM3_TISEL_VALUE            0x0


/* Lock */
// GPIO triac
#define MCU_PRPH_LOCK_TRIAC_MODER_MASK       GPIO_MODER_MODE12_Msk
#define MCU_PRPH_LOCK_TRIAC_MODER_VALUE      GPIO_MODER_MODE12_0

#define MCU_PRPH_LOCK_TRIAC_OTYPER_MASK      GPIO_OTYPER_OT12_Msk
#define MCU_PRPH_LOCK_TRIAC_OTYPER_VALUE     0x0

#define MCU_PRPH_LOCK_TRIAC_OSPEEDR_MASK     GPIO_OSPEEDR_OSPEED12_Msk
#define MCU_PRPH_LOCK_TRIAC_OSPEEDR_VALUE    0x0

#define MCU_PRPH_LOCK_TRIAC_PUPDR_MASK       GPIO_PUPDR_PUPD12_Msk
#define MCU_PRPH_LOCK_TRIAC_PUPDR_VALUE      0x0

#define MCU_PRPH_LOCK_TRIAC_AFRH_MASK        GPIO_AFRH_AFSEL12_Msk
#define MCU_PRPH_LOCK_TRIAC_AFRH_VALUE       0x0

// GPIO indicator
#define MCU_PRPH_LOCK_IND_MODER_MASK         GPIO_MODER_MODE15_Msk
#define MCU_PRPH_LOCK_IND_MODER_VALUE        GPIO_MODER_MODE15_1

#define MCU_PRPH_LOCK_IND_OTYPER_MASK        GPIO_OTYPER_OT15_Msk
#define MCU_PRPH_LOCK_IND_OTYPER_VALUE       0x0

#define MCU_PRPH_LOCK_IND_OSPEEDR_MASK       GPIO_OSPEEDR_OSPEED15_Msk
#define MCU_PRPH_LOCK_IND_OSPEEDR_VALUE      0x0

#define MCU_PRPH_LOCK_IND_PUPDR_MASK         GPIO_PUPDR_PUPD15_Msk
#define MCU_PRPH_LOCK_IND_PUPDR_VALUE        0x0

#define MCU_PRPH_LOCK_IND_AFRH_MASK          GPIO_AFRH_AFSEL15_Msk
#define MCU_PRPH_LOCK_IND_AFRH_VALUE         (GPIO_AFRH_AFSEL15_0 | GPIO_AFRH_AFSEL15_2)

// Timer
#define MCU_PRPH_TIM15_CR1_MASK              0xffffffff
#define MCU_PRPH_TIM15_CR1_VALUE             0x5

#define MCU_PRPH_TIM15_CR2_MASK              0xffffffff
#define MCU_PRPH_TIM15_CR2_VALUE             0x0

#define MCU_PRPH_TIM15_SMCR_MASK             0xffffffff
#define MCU_PRPH_TIM15_SMCR_VALUE            0x64

#define MCU_PRPH_TIM15_DIER_MASK             0xffffffff
#define MCU_PRPH_TIM15_DIER_VALUE            0x5

#define MCU_PRPH_TIM15_CCMR1_MASK            0xffffffff
#define MCU_PRPH_TIM15_CCMR1_VALUE           0x100

#define MCU_PRPH_TIM15_CCER_MASK             0xffffffff
#define MCU_PRPH_TIM15_CCER_VALUE            0x10

#define MCU_PRPH_TIM15_PSC_MASK              0xffffffff
#define MCU_PRPH_TIM15_PSC_VALUE             0x1f

#define MCU_PRPH_TIM15_ARR_MASK              0xffffffff
#define MCU_PRPH_TIM15_ARR_VALUE             0x61a8 // 25000

#define MCU_PRPH_TIM15_RCR_MASK              0xffffffff
#define MCU_PRPH_TIM15_RCR_VALUE             0x0

#define MCU_PRPH_TIM15_BDTR_MASK             0xffffffff
#define MCU_PRPH_TIM15_BDTR_VALUE            0x0

#define MCU_PRPH_TIM15_DCR_MASK              0xffffffff
#define MCU_PRPH_TIM15_DCR_VALUE             0x0

#define MCU_PRPH_TIM15_DMAR_MASK             0xffffffff
#define MCU_PRPH_TIM15_DMAR_VALUE            0x5

#define MCU_PRPH_TIM15_AF1_MASK              0xffffffff
#define MCU_PRPH_TIM15_AF1_VALUE             0x1

#define MCU_PRPH_TIM15_TISEL_MASK            0xffffffff
#define MCU_PRPH_TIM15_TISEL_VALUE           0x0


/* Drain */
// GPIO
#define MCU_PRPH_DRAIN_MODER_MASK            GPIO_MODER_MODE11_Msk
#define MCU_PRPH_DRAIN_MODER_VALUE           GPIO_MODER_MODE11_0

#define MCU_PRPH_DRAIN_OTYPER_MASK           GPIO_OTYPER_OT11_Msk
#define MCU_PRPH_DRAIN_OTYPER_VALUE          0x0

#define MCU_PRPH_DRAIN_OSPEEDR_MASK          GPIO_OSPEEDR_OSPEED11_Msk
#define MCU_PRPH_DRAIN_OSPEEDR_VALUE         0x0

#define MCU_PRPH_DRAIN_PUPDR_MASK            GPIO_PUPDR_PUPD11_Msk
#define MCU_PRPH_DRAIN_PUPDR_VALUE           0x0

#define MCU_PRPH_DRAIN_AFRH_MASK             GPIO_AFRH_AFSEL11_Msk
#define MCU_PRPH_DRAIN_AFRH_VALUE            0x0

#endif // _MCU_PRPH_CFG_H_

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
