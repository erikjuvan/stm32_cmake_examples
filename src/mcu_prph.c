/**
 * @file     mcu_prph.c
 * @brief    MCU periphery checking module.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 * @addtogroup MCU_PRPH
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include "stm32g0xx_hal.h"
#include "mcu_prph.h"
#include "mcu_prph_cfg.h"
#include "main.h"
#include "drain.h"
#include "lock.h"


////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#define MCU_PRPH_FLAGS_MAX_VAL                    ( UINT32_MAX )
#define MCU_PRPH_SET_ALL_FLAGS()                  { .flags = MCU_PRPH_FLAGS_MAX_VAL }
#define MCU_PRPH_ARE_ALL_FLAGS_SET(x)             ( (bool)( x.flags == MCU_PRPH_FLAGS_MAX_VAL ) )
#define MCU_PRPH_CHK_VAL(val, mask, res)          ( (uint32_t)( (val & mask) == res ) )


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////
/**
 * RCC.
 */
typedef union
{
    struct
    {
        uint32_t cr       : 1;
        uint32_t cfgr     : 1;
        uint32_t pllcfgr  : 1;
        uint32_t ioprstr  : 1;
        uint32_t ahbrstr  : 1;
        uint32_t apbrstr1 : 1;
        uint32_t apbrstr2 : 1;
        uint32_t iopenr   : 1;
        uint32_t ahbenr   : 1;
        uint32_t apbenr1  : 1;
        uint32_t apbenr2  : 1;
    };
    uint32_t flags;
} mcu_prph_rcc_t;

/**
 * SysTick.
 */
typedef union
{
    struct
    {
        uint32_t ctrl : 1;
        uint32_t load : 1;
    };
    uint32_t flags;
} mcu_prph_systick_t;

/**
 * IWDG.
 */
typedef union
{
    struct
    {
        uint32_t pr   : 1;
        uint32_t rlr  : 1;
        uint32_t winr : 1;
    };
    uint32_t flags;
} mcu_prph_iwdg_t;

/**
 * WWDG.
 */
typedef union
{
    struct
    {
        uint32_t cfr  : 1;
    };
    uint32_t flags;
} mcu_prph_wwdg_t;

/**
 * FLASH.
 */
typedef union
{
    struct
    {
        uint32_t optr : 1;
    };
    uint32_t flags;
} mcu_prph_flash_t;

/**
 * GPIO.
 */
typedef union
{
    struct
    {
        uint32_t moder   : 1;
        uint32_t otyper  : 1;
        uint32_t ospeedr : 1;
        uint32_t pupdr   : 1;
        uint32_t lckr    : 1;
        uint32_t afrl    : 1;
        uint32_t afrh    : 1;
    };
    uint32_t flags;
} mcu_prph_gpio_t;

/**
 * GPIO output.
 */
typedef union
{
    struct
    {
        uint32_t odr : 1;
    };
    uint32_t flags;
} mcu_prph_gpio_output_t;

/**
 * TIM.
 */
typedef union
{
    struct
    {
        uint32_t cr1   : 1;
        uint32_t cr2   : 1;
        uint32_t smcr  : 1;
        uint32_t dier  : 1;
        uint32_t ccmr1 : 1;
        uint32_t ccmr2 : 1;
        uint32_t ccer  : 1;
        uint32_t psc   : 1;
        uint32_t arr   : 1;
        uint32_t rcr   : 1;
        uint32_t bdtr  : 1;
        uint32_t dcr   : 1;
        uint32_t dmar  : 1;
        uint32_t or1   : 1;
        uint32_t ccmr3 : 1;
        uint32_t af1   : 1;
        uint32_t af2   : 1;
        uint32_t tisel : 1;
    };
    uint32_t flags;
} mcu_prph_tim_t;

/**
 * EPS periphery.
 */
typedef struct
{
    mcu_prph_gpio_t gpio_power_en;
    mcu_prph_gpio_t gpio;
    mcu_prph_tim_t  tim3;
} mcu_prph_eps_t;

/**
 * Door lock periphery.
 */
typedef struct
{
    mcu_prph_gpio_t        gpio_triac;
    mcu_prph_gpio_output_t gpio_triac_output;
    mcu_prph_gpio_t        gpio_ind; // door locked indicator
    mcu_prph_tim_t         tim15;
} mcu_prph_lock_t;

/**
 * Drain periphery.
 */
typedef struct
{
    mcu_prph_gpio_t        gpio;
    mcu_prph_gpio_output_t gpio_output;
} mcu_prph_drain_t;

/**
 * All safety critical periphery used by SAFE.
 */
typedef struct
{
    mcu_prph_rcc_t       rcc;
    mcu_prph_systick_t   systick;
    mcu_prph_iwdg_t      iwdg;
    mcu_prph_wwdg_t      wwdg;
    mcu_prph_flash_t     flash;
    mcu_prph_tim_t       stl_tim16;
    mcu_prph_eps_t       eps;
    mcu_prph_lock_t      lock;
    mcu_prph_drain_t     drain;
} mcu_prph_t;


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
static mcu_prph_t g_mcu_prph =
{
    .rcc = MCU_PRPH_SET_ALL_FLAGS(),

    .systick = MCU_PRPH_SET_ALL_FLAGS(),

    .iwdg = MCU_PRPH_SET_ALL_FLAGS(),

    .wwdg = MCU_PRPH_SET_ALL_FLAGS(),

	.flash = MCU_PRPH_SET_ALL_FLAGS(),

	.stl_tim16 = MCU_PRPH_SET_ALL_FLAGS(),

    .eps = {
        .gpio_power_en = MCU_PRPH_SET_ALL_FLAGS(),
        .gpio = MCU_PRPH_SET_ALL_FLAGS(),
        .tim3 = MCU_PRPH_SET_ALL_FLAGS(),
    },

    .lock = {
        .gpio_triac = MCU_PRPH_SET_ALL_FLAGS(),
        .gpio_triac_output = MCU_PRPH_SET_ALL_FLAGS(),
        .gpio_ind = MCU_PRPH_SET_ALL_FLAGS(),
        .tim15 = MCU_PRPH_SET_ALL_FLAGS(),
    },

    .drain = {
        .gpio = MCU_PRPH_SET_ALL_FLAGS(),
        .gpio_output = MCU_PRPH_SET_ALL_FLAGS()
    },
};


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
static void mcu_prph_chk_rcc(void);
static void mcu_prph_chk_rcc_standby(void);
static void mcu_prph_chk_systick(void);
static void mcu_prph_chk_systick_standby(void);
static void mcu_prph_chk_iwdg(void);
static void mcu_prph_chk_wwdg(void);
static void mcu_prph_chk_ram_parity(void);
static void mcu_prph_chk_stl_tim16(void);
static void mcu_prph_chk_eps(void);
static void mcu_prph_chk_lock(void);
static void mcu_prph_chk_lock_standby(void);
static void mcu_prph_chk_drain(void);


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Check RCC periphery.
 *
 * @return     None.
 */
static void mcu_prph_chk_rcc(void)
{
    g_mcu_prph.rcc.cr       &= MCU_PRPH_CHK_VAL(RCC->CR       ,MCU_PRPH_RCC_CR_MASK        ,MCU_PRPH_RCC_CR_VALUE      );
    g_mcu_prph.rcc.cfgr     &= MCU_PRPH_CHK_VAL(RCC->CFGR     ,MCU_PRPH_RCC_CFGR_MASK      ,MCU_PRPH_RCC_CFGR_VALUE    );
    g_mcu_prph.rcc.pllcfgr  &= MCU_PRPH_CHK_VAL(RCC->PLLCFGR  ,MCU_PRPH_RCC_PLLCFGR_MASK   ,MCU_PRPH_RCC_PLLCFGR_VALUE );
    g_mcu_prph.rcc.ioprstr  &= MCU_PRPH_CHK_VAL(RCC->IOPRSTR  ,MCU_PRPH_RCC_IOPRSTR_MASK   ,MCU_PRPH_RCC_IOPRSTR_VALUE );
    g_mcu_prph.rcc.ahbrstr  &= MCU_PRPH_CHK_VAL(RCC->AHBRSTR  ,MCU_PRPH_RCC_AHBRSTR_MASK   ,MCU_PRPH_RCC_AHBRSTR_VALUE );
    g_mcu_prph.rcc.apbrstr1 &= MCU_PRPH_CHK_VAL(RCC->APBRSTR1 ,MCU_PRPH_RCC_APBRSTR1_MASK  ,MCU_PRPH_RCC_APBRSTR1_VALUE);
    g_mcu_prph.rcc.apbrstr2 &= MCU_PRPH_CHK_VAL(RCC->APBRSTR2 ,MCU_PRPH_RCC_APBRSTR2__MASK ,MCU_PRPH_RCC_APBRSTR2_VALUE);
    g_mcu_prph.rcc.iopenr   &= MCU_PRPH_CHK_VAL(RCC->IOPENR   ,MCU_PRPH_RCC_IOPENR_MASK    ,MCU_PRPH_RCC_IOPENR_VALUE  );
    g_mcu_prph.rcc.ahbenr   &= MCU_PRPH_CHK_VAL(RCC->AHBENR   ,MCU_PRPH_RCC_AHBENR_MASK    ,MCU_PRPH_RCC_AHBENR_VALUE  );
    g_mcu_prph.rcc.apbenr1  &= MCU_PRPH_CHK_VAL(RCC->APBENR1  ,MCU_PRPH_RCC_APBENR1_MASK   ,MCU_PRPH_RCC_APBENR1_VALUE );
    g_mcu_prph.rcc.apbenr2  &= MCU_PRPH_CHK_VAL(RCC->APBENR2  ,MCU_PRPH_RCC_APBENR2_MASK   ,MCU_PRPH_RCC_APBENR2_VALUE );
}

/**
 * Check RCC periphery while in standby.
 *
 * @return     None.
 */
static void mcu_prph_chk_rcc_standby(void)
{
    g_mcu_prph.rcc.cr       &= MCU_PRPH_CHK_VAL(RCC->CR       ,MCU_PRPH_RCC_CR_STANDBY_MASK        ,MCU_PRPH_RCC_CR_STANDBY_VALUE      );
    g_mcu_prph.rcc.cfgr     &= MCU_PRPH_CHK_VAL(RCC->CFGR     ,MCU_PRPH_RCC_CFGR_STANDBY_MASK      ,MCU_PRPH_RCC_CFGR_STANDBY_VALUE    );
    g_mcu_prph.rcc.pllcfgr  &= MCU_PRPH_CHK_VAL(RCC->PLLCFGR  ,MCU_PRPH_RCC_PLLCFGR_STANDBY_MASK   ,MCU_PRPH_RCC_PLLCFGR_STANDBY_VALUE );
    g_mcu_prph.rcc.ioprstr  &= MCU_PRPH_CHK_VAL(RCC->IOPRSTR  ,MCU_PRPH_RCC_IOPRSTR_STANDBY_MASK   ,MCU_PRPH_RCC_IOPRSTR_STANDBY_VALUE );
    g_mcu_prph.rcc.ahbrstr  &= MCU_PRPH_CHK_VAL(RCC->AHBRSTR  ,MCU_PRPH_RCC_AHBRSTR_STANDBY_MASK   ,MCU_PRPH_RCC_AHBRSTR_STANDBY_VALUE );
    g_mcu_prph.rcc.apbrstr1 &= MCU_PRPH_CHK_VAL(RCC->APBRSTR1 ,MCU_PRPH_RCC_APBRSTR1_STANDBY_MASK  ,MCU_PRPH_RCC_APBRSTR1_STANDBY_VALUE);
    g_mcu_prph.rcc.apbrstr2 &= MCU_PRPH_CHK_VAL(RCC->APBRSTR2 ,MCU_PRPH_RCC_APBRSTR2_STANDBY_MASK  ,MCU_PRPH_RCC_APBRSTR2_STANDBY_VALUE);
    g_mcu_prph.rcc.iopenr   &= MCU_PRPH_CHK_VAL(RCC->IOPENR   ,MCU_PRPH_RCC_IOPENR_STANDBY_MASK    ,MCU_PRPH_RCC_IOPENR_STANDBY_VALUE  );
    g_mcu_prph.rcc.ahbenr   &= MCU_PRPH_CHK_VAL(RCC->AHBENR   ,MCU_PRPH_RCC_AHBENR_STANDBY_MASK    ,MCU_PRPH_RCC_AHBENR_STANDBY_VALUE  );
    g_mcu_prph.rcc.apbenr1  &= MCU_PRPH_CHK_VAL(RCC->APBENR1  ,MCU_PRPH_RCC_APBENR1_STANDBY_MASK   ,MCU_PRPH_RCC_APBENR1_STANDBY_VALUE );
    g_mcu_prph.rcc.apbenr2  &= MCU_PRPH_CHK_VAL(RCC->APBENR2  ,MCU_PRPH_RCC_APBENR2_STANDBY_MASK   ,MCU_PRPH_RCC_APBENR2_STANDBY_VALUE );
}

/**
 * Check SysTick periphery.
 *
 * @return     None.
 */
static void mcu_prph_chk_systick(void)
{
    g_mcu_prph.systick.ctrl &= MCU_PRPH_CHK_VAL(SysTick->CTRL, MCU_PRPH_SYSTICK_CTRL_MASK, MCU_PRPH_SYSTICK_CTRL_VALUE);
    g_mcu_prph.systick.load &= MCU_PRPH_CHK_VAL(SysTick->LOAD, MCU_PRPH_SYSTICK_LOAD_MASK, MCU_PRPH_SYSTICK_LOAD_VALUE);
}

/**
 * Check SysTick periphery while in standby.
 *
 * @return     None.
 */
static void mcu_prph_chk_systick_standby(void)
{
    g_mcu_prph.systick.ctrl &= MCU_PRPH_CHK_VAL(SysTick->CTRL, MCU_PRPH_SYSTICK_CTRL_STANDBY_MASK, MCU_PRPH_SYSTICK_CTRL_STANDBY_VALUE);
    g_mcu_prph.systick.load &= MCU_PRPH_CHK_VAL(SysTick->LOAD, MCU_PRPH_SYSTICK_LOAD_STANDBY_MASK, MCU_PRPH_SYSTICK_LOAD_STANDBY_VALUE);
}

/**
 * Check IWDG periphery.
 *
 * @return     None.
 */
static void mcu_prph_chk_iwdg(void)
{
    g_mcu_prph.iwdg.pr   &= MCU_PRPH_CHK_VAL(IWDG->PR   ,MCU_PRPH_IWDG_PR_MASK   ,MCU_PRPH_IWDG_PR_VALUE  );
    g_mcu_prph.iwdg.rlr  &= MCU_PRPH_CHK_VAL(IWDG->RLR  ,MCU_PRPH_IWDG_RLR_MASK  ,MCU_PRPH_IWDG_RLR_VALUE );
    g_mcu_prph.iwdg.winr &= MCU_PRPH_CHK_VAL(IWDG->WINR ,MCU_PRPH_IWDG_WINR_MASK ,MCU_PRPH_IWDG_WINR_VALUE);
}

/**
 * Check WWDG periphery.
 *
 * @return     None.
 */
static void mcu_prph_chk_wwdg(void)
{
    g_mcu_prph.wwdg.cfr &= MCU_PRPH_CHK_VAL(WWDG->CFR ,MCU_PRPH_WWDG_CFR_MASK ,MCU_PRPH_WWDG_CFR_VALUE);
}

/**
 * Check WWDG periphery while in standby.
 *
 * @return     None.
 */
static void mcu_prph_chk_wwdg_standby(void)
{
    g_mcu_prph.wwdg.cfr &= MCU_PRPH_CHK_VAL(WWDG->CFR ,MCU_PRPH_WWDG_CFR_STANDBY_MASK ,MCU_PRPH_WWDG_CFR_STANDBY_VALUE);
}

/**
 * Check if RAM Parity Check is enabled.
 *
 * @return     None.
 */
static void mcu_prph_chk_ram_parity(void)
{
    g_mcu_prph.flash.optr &= MCU_PRPH_CHK_VAL(FLASH->OPTR ,MCU_PRPH_FLASH_OPTR_RAM_PARITY_MASK ,MCU_PRPH_FLASH_OPTR_RAM_PARITY_VALUE);
}

/**
 * Check periphery used by TIM16 used in STL library.
 *
 * @return     None.
 */
static void mcu_prph_chk_stl_tim16(void)
{
    g_mcu_prph.stl_tim16.cr1   &= MCU_PRPH_CHK_VAL(TIM16->CR1   ,MCU_PRPH_TIM16_CR1_MASK   ,MCU_PRPH_TIM16_CR1_VALUE  );
    g_mcu_prph.stl_tim16.cr2   &= MCU_PRPH_CHK_VAL(TIM16->CR2   ,MCU_PRPH_TIM16_CR2_MASK   ,MCU_PRPH_TIM16_CR2_VALUE  );
    g_mcu_prph.stl_tim16.dier  &= MCU_PRPH_CHK_VAL(TIM16->DIER  ,MCU_PRPH_TIM16_DIER_MASK  ,MCU_PRPH_TIM16_DIER_VALUE );
    g_mcu_prph.stl_tim16.ccmr1 &= MCU_PRPH_CHK_VAL(TIM16->CCMR1 ,MCU_PRPH_TIM16_CCMR1_MASK ,MCU_PRPH_TIM16_CCMR1_VALUE);
    g_mcu_prph.stl_tim16.ccer  &= MCU_PRPH_CHK_VAL(TIM16->CCER  ,MCU_PRPH_TIM16_CCER_MASK  ,MCU_PRPH_TIM16_CCER_VALUE );
    g_mcu_prph.stl_tim16.psc   &= MCU_PRPH_CHK_VAL(TIM16->PSC   ,MCU_PRPH_TIM16_PSC_MASK   ,MCU_PRPH_TIM16_PSC_VALUE  );
    g_mcu_prph.stl_tim16.arr   &= MCU_PRPH_CHK_VAL(TIM16->ARR   ,MCU_PRPH_TIM16_ARR_MASK   ,MCU_PRPH_TIM16_ARR_VALUE  );
    g_mcu_prph.stl_tim16.rcr   &= MCU_PRPH_CHK_VAL(TIM16->RCR   ,MCU_PRPH_TIM16_RCR_MASK   ,MCU_PRPH_TIM16_RCR_VALUE  );
    g_mcu_prph.stl_tim16.bdtr  &= MCU_PRPH_CHK_VAL(TIM16->BDTR  ,MCU_PRPH_TIM16_BDTR_MASK  ,MCU_PRPH_TIM16_BDTR_VALUE );
    g_mcu_prph.stl_tim16.dcr   &= MCU_PRPH_CHK_VAL(TIM16->DCR   ,MCU_PRPH_TIM16_DCR_MASK   ,MCU_PRPH_TIM16_DCR_VALUE  );
    g_mcu_prph.stl_tim16.dmar  &= MCU_PRPH_CHK_VAL(TIM16->DMAR  ,MCU_PRPH_TIM16_DMAR_MASK  ,MCU_PRPH_TIM16_DMAR_VALUE );
    g_mcu_prph.stl_tim16.af1   &= MCU_PRPH_CHK_VAL(TIM16->AF1   ,MCU_PRPH_TIM16_AF1_MASK   ,MCU_PRPH_TIM16_AF1_VALUE  );
    g_mcu_prph.stl_tim16.tisel &= MCU_PRPH_CHK_VAL(TIM16->TISEL ,MCU_PRPH_TIM16_TISEL_MASK ,MCU_PRPH_TIM16_TISEL_VALUE);
}

/**
 * Check periphery used by EPS.
 *
 * @return     None.
 */
static void mcu_prph_chk_eps(void)
{
    // gpio power enable
    g_mcu_prph.eps.gpio_power_en.moder   &= MCU_PRPH_CHK_VAL(EPS_POWER_EN_PORT->MODER   ,MCU_PRPH_EPS_POWER_EN_MODER_MASK   ,MCU_PRPH_EPS_POWER_EN_MODER_VALUE  );
    g_mcu_prph.eps.gpio_power_en.otyper  &= MCU_PRPH_CHK_VAL(EPS_POWER_EN_PORT->OTYPER  ,MCU_PRPH_EPS_POWER_EN_OTYPER_MASK  ,MCU_PRPH_EPS_POWER_EN_OTYPER_VALUE );
    g_mcu_prph.eps.gpio_power_en.ospeedr &= MCU_PRPH_CHK_VAL(EPS_POWER_EN_PORT->OSPEEDR ,MCU_PRPH_EPS_POWER_EN_OSPEEDR_MASK ,MCU_PRPH_EPS_POWER_EN_OSPEEDR_VALUE);
    g_mcu_prph.eps.gpio_power_en.pupdr   &= MCU_PRPH_CHK_VAL(EPS_POWER_EN_PORT->PUPDR   ,MCU_PRPH_EPS_POWER_EN_PUPDR_MASK   ,MCU_PRPH_EPS_POWER_EN_PUPDR_VALUE  );
    g_mcu_prph.eps.gpio_power_en.afrl    &= MCU_PRPH_CHK_VAL(EPS_POWER_EN_PORT->AFR[0]  ,MCU_PRPH_EPS_POWER_EN_AFRL_MASK    ,MCU_PRPH_EPS_POWER_EN_AFRL_VALUE   );

    // gpio
    g_mcu_prph.eps.gpio.moder   &= MCU_PRPH_CHK_VAL(EPS_PORT->MODER   ,MCU_PRPH_EPS_MODER_MASK   ,MCU_PRPH_EPS_MODER_VALUE  );
    g_mcu_prph.eps.gpio.otyper  &= MCU_PRPH_CHK_VAL(EPS_PORT->OTYPER  ,MCU_PRPH_EPS_OTYPER_MASK  ,MCU_PRPH_EPS_OTYPER_VALUE );
    g_mcu_prph.eps.gpio.ospeedr &= MCU_PRPH_CHK_VAL(EPS_PORT->OSPEEDR ,MCU_PRPH_EPS_OSPEEDR_MASK ,MCU_PRPH_EPS_OSPEEDR_VALUE);
    g_mcu_prph.eps.gpio.pupdr   &= MCU_PRPH_CHK_VAL(EPS_PORT->PUPDR   ,MCU_PRPH_EPS_PUPDR_MASK   ,MCU_PRPH_EPS_PUPDR_VALUE  );
    g_mcu_prph.eps.gpio.afrl    &= MCU_PRPH_CHK_VAL(EPS_PORT->AFR[0]  ,MCU_PRPH_EPS_AFRL_MASK    ,MCU_PRPH_EPS_AFRL_VALUE   );

    // tim3
    g_mcu_prph.eps.tim3.cr1   &= MCU_PRPH_CHK_VAL(TIM3->CR1   ,MCU_PRPH_TIM3_CR1_MASK   ,MCU_PRPH_TIM3_CR1_VALUE  );
    g_mcu_prph.eps.tim3.cr2   &= MCU_PRPH_CHK_VAL(TIM3->CR2   ,MCU_PRPH_TIM3_CR2_MASK   ,MCU_PRPH_TIM3_CR2_VALUE  );
    g_mcu_prph.eps.tim3.smcr  &= MCU_PRPH_CHK_VAL(TIM3->SMCR  ,MCU_PRPH_TIM3_SMCR_MASK  ,MCU_PRPH_TIM3_SMCR_VALUE );
    g_mcu_prph.eps.tim3.dier  &= MCU_PRPH_CHK_VAL(TIM3->DIER  ,MCU_PRPH_TIM3_DIER_MASK  ,MCU_PRPH_TIM3_DIER_VALUE );
    g_mcu_prph.eps.tim3.ccmr1 &= MCU_PRPH_CHK_VAL(TIM3->CCMR1 ,MCU_PRPH_TIM3_CCMR1_MASK ,MCU_PRPH_TIM3_CCMR1_VALUE);
    g_mcu_prph.eps.tim3.ccmr2 &= MCU_PRPH_CHK_VAL(TIM3->CCMR2 ,MCU_PRPH_TIM3_CCMR2_MASK ,MCU_PRPH_TIM3_CCMR2_VALUE);
    g_mcu_prph.eps.tim3.ccer  &= MCU_PRPH_CHK_VAL(TIM3->CCER  ,MCU_PRPH_TIM3_CCER_MASK  ,MCU_PRPH_TIM3_CCER_VALUE );
    g_mcu_prph.eps.tim3.psc   &= MCU_PRPH_CHK_VAL(TIM3->PSC   ,MCU_PRPH_TIM3_PSC_MASK   ,MCU_PRPH_TIM3_PSC_VALUE  );
    g_mcu_prph.eps.tim3.arr   &= MCU_PRPH_CHK_VAL(TIM3->ARR   ,MCU_PRPH_TIM3_ARR_MASK   ,MCU_PRPH_TIM3_ARR_VALUE  );
    g_mcu_prph.eps.tim3.dcr   &= MCU_PRPH_CHK_VAL(TIM3->DCR   ,MCU_PRPH_TIM3_DCR_MASK   ,MCU_PRPH_TIM3_DCR_VALUE  );
    g_mcu_prph.eps.tim3.dmar  &= MCU_PRPH_CHK_VAL(TIM3->DMAR  ,MCU_PRPH_TIM3_DMAR_MASK  ,MCU_PRPH_TIM3_DMAR_VALUE );
    g_mcu_prph.eps.tim3.or1   &= MCU_PRPH_CHK_VAL(TIM3->OR1   ,MCU_PRPH_TIM3_OR1_MASK   ,MCU_PRPH_TIM3_OR1_VALUE  );
    g_mcu_prph.eps.tim3.af1   &= MCU_PRPH_CHK_VAL(TIM3->AF1   ,MCU_PRPH_TIM3_AF1_MASK   ,MCU_PRPH_TIM3_AF1_VALUE  );
    g_mcu_prph.eps.tim3.tisel &= MCU_PRPH_CHK_VAL(TIM3->TISEL ,MCU_PRPH_TIM3_TISEL_MASK ,MCU_PRPH_TIM3_TISEL_VALUE);
}

/**
 * Check periphery used by door lock.
 *
 * @return     None.
 */
static void mcu_prph_chk_lock(void)
{
    // gpio_triac
    g_mcu_prph.lock.gpio_triac.moder   &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->MODER   ,MCU_PRPH_LOCK_TRIAC_MODER_MASK   ,MCU_PRPH_LOCK_TRIAC_MODER_VALUE  );
    g_mcu_prph.lock.gpio_triac.otyper  &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->OTYPER  ,MCU_PRPH_LOCK_TRIAC_OTYPER_MASK  ,MCU_PRPH_LOCK_TRIAC_OTYPER_VALUE );
    g_mcu_prph.lock.gpio_triac.ospeedr &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->OSPEEDR ,MCU_PRPH_LOCK_TRIAC_OSPEEDR_MASK ,MCU_PRPH_LOCK_TRIAC_OSPEEDR_VALUE);
    g_mcu_prph.lock.gpio_triac.pupdr   &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->PUPDR   ,MCU_PRPH_LOCK_TRIAC_PUPDR_MASK   ,MCU_PRPH_LOCK_TRIAC_PUPDR_VALUE  );
    g_mcu_prph.lock.gpio_triac.afrh    &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->AFR[1]  ,MCU_PRPH_LOCK_TRIAC_AFRH_MASK    ,MCU_PRPH_LOCK_TRIAC_AFRH_VALUE   );

    // gpio_triac_output
    __disable_irq(); // disable IRQ to make sure value doesn't change from ZeroCross IRQ, while reading state and GPIO
    lock_triac_t triac_state = lock_get_triac();
    uint32_t triac_odr = LOCK_TRIAC_PORT->ODR;
    __enable_irq();
    uint32_t triac_gpio_odr_value = 0;
    if (triac_state == eLOCK_TRIAC_ON)
    {
        triac_gpio_odr_value = LOCK_TRIAC_PIN;
    }
    g_mcu_prph.lock.gpio_triac_output.odr &= MCU_PRPH_CHK_VAL(triac_odr, LOCK_TRIAC_PIN, triac_gpio_odr_value);

    // gpio_ind
    g_mcu_prph.lock.gpio_ind.moder   &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->MODER   ,MCU_PRPH_LOCK_IND_MODER_MASK   ,MCU_PRPH_LOCK_IND_MODER_VALUE  );
    g_mcu_prph.lock.gpio_ind.otyper  &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->OTYPER  ,MCU_PRPH_LOCK_IND_OTYPER_MASK  ,MCU_PRPH_LOCK_IND_OTYPER_VALUE );
    g_mcu_prph.lock.gpio_ind.ospeedr &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->OSPEEDR ,MCU_PRPH_LOCK_IND_OSPEEDR_MASK ,MCU_PRPH_LOCK_IND_OSPEEDR_VALUE);
    g_mcu_prph.lock.gpio_ind.pupdr   &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->PUPDR   ,MCU_PRPH_LOCK_IND_PUPDR_MASK   ,MCU_PRPH_LOCK_IND_PUPDR_VALUE  );
    g_mcu_prph.lock.gpio_ind.afrh    &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->AFR[1]  ,MCU_PRPH_LOCK_IND_AFRH_MASK    ,MCU_PRPH_LOCK_IND_AFRH_VALUE   );

    // tim15
    g_mcu_prph.lock.tim15.cr1   &= MCU_PRPH_CHK_VAL(TIM15->CR1   ,MCU_PRPH_TIM15_CR1_MASK   ,MCU_PRPH_TIM15_CR1_VALUE  );
    g_mcu_prph.lock.tim15.cr2   &= MCU_PRPH_CHK_VAL(TIM15->CR2   ,MCU_PRPH_TIM15_CR2_MASK   ,MCU_PRPH_TIM15_CR2_VALUE  );
    g_mcu_prph.lock.tim15.smcr  &= MCU_PRPH_CHK_VAL(TIM15->SMCR  ,MCU_PRPH_TIM15_SMCR_MASK  ,MCU_PRPH_TIM15_SMCR_VALUE );
    g_mcu_prph.lock.tim15.dier  &= MCU_PRPH_CHK_VAL(TIM15->DIER  ,MCU_PRPH_TIM15_DIER_MASK  ,MCU_PRPH_TIM15_DIER_VALUE );
    g_mcu_prph.lock.tim15.ccmr1 &= MCU_PRPH_CHK_VAL(TIM15->CCMR1 ,MCU_PRPH_TIM15_CCMR1_MASK ,MCU_PRPH_TIM15_CCMR1_VALUE);
    g_mcu_prph.lock.tim15.ccer  &= MCU_PRPH_CHK_VAL(TIM15->CCER  ,MCU_PRPH_TIM15_CCER_MASK  ,MCU_PRPH_TIM15_CCER_VALUE );
    g_mcu_prph.lock.tim15.psc   &= MCU_PRPH_CHK_VAL(TIM15->PSC   ,MCU_PRPH_TIM15_PSC_MASK   ,MCU_PRPH_TIM15_PSC_VALUE  );
    g_mcu_prph.lock.tim15.arr   &= MCU_PRPH_CHK_VAL(TIM15->ARR   ,MCU_PRPH_TIM15_ARR_MASK   ,MCU_PRPH_TIM15_ARR_VALUE  );
    g_mcu_prph.lock.tim15.rcr   &= MCU_PRPH_CHK_VAL(TIM15->RCR   ,MCU_PRPH_TIM15_RCR_MASK   ,MCU_PRPH_TIM15_RCR_VALUE  );
    g_mcu_prph.lock.tim15.bdtr  &= MCU_PRPH_CHK_VAL(TIM15->BDTR  ,MCU_PRPH_TIM15_BDTR_MASK  ,MCU_PRPH_TIM15_BDTR_VALUE );
    g_mcu_prph.lock.tim15.dcr   &= MCU_PRPH_CHK_VAL(TIM15->DCR   ,MCU_PRPH_TIM15_DCR_MASK   ,MCU_PRPH_TIM15_DCR_VALUE  );
    g_mcu_prph.lock.tim15.dmar  &= MCU_PRPH_CHK_VAL(TIM15->DMAR  ,MCU_PRPH_TIM15_DMAR_MASK  ,MCU_PRPH_TIM15_DMAR_VALUE );
    g_mcu_prph.lock.tim15.af1   &= MCU_PRPH_CHK_VAL(TIM15->AF1   ,MCU_PRPH_TIM15_AF1_MASK   ,MCU_PRPH_TIM15_AF1_VALUE  );
    g_mcu_prph.lock.tim15.tisel &= MCU_PRPH_CHK_VAL(TIM15->TISEL ,MCU_PRPH_TIM15_TISEL_MASK ,MCU_PRPH_TIM15_TISEL_VALUE);
}

/**
 * Check periphery used by door lock in standby. It is different from normal since it doesn't check TIM periphery since it's
 * clock is turned off in standby.
 *
 * @return     None.
 */
static void mcu_prph_chk_lock_standby(void)
{
    // gpio_triac
    g_mcu_prph.lock.gpio_triac.moder   &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->MODER   ,MCU_PRPH_LOCK_TRIAC_MODER_MASK   ,MCU_PRPH_LOCK_TRIAC_MODER_VALUE  );
    g_mcu_prph.lock.gpio_triac.otyper  &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->OTYPER  ,MCU_PRPH_LOCK_TRIAC_OTYPER_MASK  ,MCU_PRPH_LOCK_TRIAC_OTYPER_VALUE );
    g_mcu_prph.lock.gpio_triac.ospeedr &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->OSPEEDR ,MCU_PRPH_LOCK_TRIAC_OSPEEDR_MASK ,MCU_PRPH_LOCK_TRIAC_OSPEEDR_VALUE);
    g_mcu_prph.lock.gpio_triac.pupdr   &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->PUPDR   ,MCU_PRPH_LOCK_TRIAC_PUPDR_MASK   ,MCU_PRPH_LOCK_TRIAC_PUPDR_VALUE  );
    g_mcu_prph.lock.gpio_triac.afrh    &= MCU_PRPH_CHK_VAL(LOCK_TRIAC_PORT->AFR[1]  ,MCU_PRPH_LOCK_TRIAC_AFRH_MASK    ,MCU_PRPH_LOCK_TRIAC_AFRH_VALUE   );

    // gpio_triac_output
    __disable_irq(); // disable IRQ to make sure value doesn't change from ZeroCross IRQ, while reading state and GPIO
    lock_triac_t triac_state = lock_get_triac();
    uint32_t triac_odr = LOCK_TRIAC_PORT->ODR;
    __enable_irq();
    uint32_t triac_gpio_odr_value = 0;
    if (triac_state == eLOCK_TRIAC_ON)
    {
        triac_gpio_odr_value = LOCK_TRIAC_PIN;
    }
    g_mcu_prph.lock.gpio_triac_output.odr &= MCU_PRPH_CHK_VAL(triac_odr, LOCK_TRIAC_PIN, triac_gpio_odr_value);

    // gpio_ind
    g_mcu_prph.lock.gpio_ind.moder   &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->MODER   ,MCU_PRPH_LOCK_IND_MODER_MASK   ,MCU_PRPH_LOCK_IND_MODER_VALUE  );
    g_mcu_prph.lock.gpio_ind.otyper  &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->OTYPER  ,MCU_PRPH_LOCK_IND_OTYPER_MASK  ,MCU_PRPH_LOCK_IND_OTYPER_VALUE );
    g_mcu_prph.lock.gpio_ind.ospeedr &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->OSPEEDR ,MCU_PRPH_LOCK_IND_OSPEEDR_MASK ,MCU_PRPH_LOCK_IND_OSPEEDR_VALUE);
    g_mcu_prph.lock.gpio_ind.pupdr   &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->PUPDR   ,MCU_PRPH_LOCK_IND_PUPDR_MASK   ,MCU_PRPH_LOCK_IND_PUPDR_VALUE  );
    g_mcu_prph.lock.gpio_ind.afrh    &= MCU_PRPH_CHK_VAL(LOCK_IND_PORT->AFR[1]  ,MCU_PRPH_LOCK_IND_AFRH_MASK    ,MCU_PRPH_LOCK_IND_AFRH_VALUE   );
}


/**
 * Check periphery used by drain pump.
 *
 * @return     None.
 */
static void mcu_prph_chk_drain(void)
{
    // gpio
    g_mcu_prph.drain.gpio.moder   &= MCU_PRPH_CHK_VAL(DRAIN_PORT->MODER   ,MCU_PRPH_DRAIN_MODER_MASK   ,MCU_PRPH_DRAIN_MODER_VALUE  );
    g_mcu_prph.drain.gpio.otyper  &= MCU_PRPH_CHK_VAL(DRAIN_PORT->OTYPER  ,MCU_PRPH_DRAIN_OTYPER_MASK  ,MCU_PRPH_DRAIN_OTYPER_VALUE );
    g_mcu_prph.drain.gpio.ospeedr &= MCU_PRPH_CHK_VAL(DRAIN_PORT->OSPEEDR ,MCU_PRPH_DRAIN_OSPEEDR_MASK ,MCU_PRPH_DRAIN_OSPEEDR_VALUE);
    g_mcu_prph.drain.gpio.pupdr   &= MCU_PRPH_CHK_VAL(DRAIN_PORT->PUPDR   ,MCU_PRPH_DRAIN_PUPDR_MASK   ,MCU_PRPH_DRAIN_PUPDR_VALUE  );
    g_mcu_prph.drain.gpio.afrh    &= MCU_PRPH_CHK_VAL(DRAIN_PORT->AFR[1]  ,MCU_PRPH_DRAIN_AFRH_MASK    ,MCU_PRPH_DRAIN_AFRH_VALUE   );

    // gpio_output
    __disable_irq(); // disable IRQ to make sure value doesn't change from ZeroCross IRQ, while reading state and GPIO
    drain_state_t drain_state = drain_get_state();
    uint32_t gpioc_odr = GPIOC->ODR;
    __enable_irq();
    uint32_t drain_gpio_odr_value = 0;
    if (drain_state == eDRAIN_ON)
    {
        drain_gpio_odr_value = DRAIN_PIN;
    }
    g_mcu_prph.drain.gpio_output.odr &= MCU_PRPH_CHK_VAL(gpioc_odr, DRAIN_PIN, drain_gpio_odr_value);
}

/**
 * Periphery check run/proc routine.
 *
 * @return     None.
 */
void mcu_prph_proc(void)
{
    mcu_prph_chk_rcc();
    mcu_prph_chk_systick();
    mcu_prph_chk_iwdg();
    mcu_prph_chk_wwdg();
    mcu_prph_chk_ram_parity();
    mcu_prph_chk_stl_tim16();
    mcu_prph_chk_eps();
    mcu_prph_chk_lock();
    mcu_prph_chk_drain();
}

/**
 * Standby periphery check run/proc routine.
 *
 * @return     None.
 */
void mcu_prph_proc_standby(void)
{
    // Standby specific
    mcu_prph_chk_rcc_standby();
    mcu_prph_chk_systick_standby();
    mcu_prph_chk_lock_standby();
    mcu_prph_chk_wwdg_standby();

    // Same as in normal operation
    mcu_prph_chk_iwdg();
    mcu_prph_chk_ram_parity();
    mcu_prph_chk_stl_tim16();
    mcu_prph_chk_eps();
    mcu_prph_chk_drain();
}

/**
 * Get status of periphery check.
 *
 * @return    Status of periphery check.
 */
mcu_prph_status_t mcu_prph_get_status(void)
{
    mcu_prph_status_t status = eMCU_PRPH_OK;

    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.rcc) != true)
    {
        status |= eMCU_PRPH_RCC_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.systick) != true)
    {
        status |= eMCU_PRPH_SYSTICK_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.iwdg) != true)
    {
        status |= eMCU_PRPH_IWDG_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.wwdg) != true)
    {
        status |= eMCU_PRPH_WWDG_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.flash) != true)
    {
        status |= eMCU_PRPH_RAM_PE_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.stl_tim16) != true)
    {
    	status |= eMCU_PRPH_STL_TIM16_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.eps.gpio_power_en) != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.eps.gpio)          != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.eps.tim3)          != true)
    {
        status |= eMCU_PRPH_EPS_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.lock.gpio_ind)          != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.lock.gpio_triac)        != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.lock.gpio_triac_output) != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.lock.tim15)             != true)
    {
        status |= eMCU_PRPH_LOCK_FAIL;
    }
    if (MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.drain.gpio)        != true ||
        MCU_PRPH_ARE_ALL_FLAGS_SET(g_mcu_prph.drain.gpio_output) != true)
    {
        status |= eMCU_PRPH_DRAIN_FAIL;
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
