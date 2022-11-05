/**
 * @file     watchdog.c
 * @brief    Watchdog module
 * @author   Erik Juvan
 * @date     21.09.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 *
 * @addtogroup WATCHDOG
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "stm32g0xx_hal.h"
#include "stm32xx_STLparam.h"
#include "trace.h"


////////////////////////////////////////////////////////////////////////////////
// External variables
////////////////////////////////////////////////////////////////////////////////
extern IWDG_HandleTypeDef IwdgHandle;   /**< Independent watchdog (IWDG) handle. */
extern WWDG_HandleTypeDef WwdgHandle;   /**< Window watchdog (WWDG) handle. */


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * Watchdog (IWDG and WWDG) initialization routine.
 *
 * @return       None.
 */
void watchdog_init(void)
{
    HAL_StatusTypeDef ret;
#ifdef USE_INDEPENDENT_WDOG
    // (1 / 32kHz) * 256 * 100 = 800 ms
    IwdgHandle.Instance = IWDG;
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
    IwdgHandle.Init.Reload = 100U;
    IwdgHandle.Init.Window = 100U;
    ret = HAL_IWDG_Init(&IwdgHandle);
    if (ret != HAL_OK)
    {
        TRACE_FAT("SMN ::: IWDG init error %d", ret);
        FailSafePOR();
    }
#endif

#ifdef USE_WINDOW_WDOG
    // (1/PCLK=32MHz) * 4096 * 64 * 64 = 524 ms
    __WWDG_CLK_ENABLE();
    WwdgHandle.Instance = WWDG;
    WwdgHandle.Init.Prescaler = WWDG_PRESCALER_64;
    WwdgHandle.Init.Counter = 127U;
    WwdgHandle.Init.Window = 127U;
    WwdgHandle.Init.EWIMode = WWDG_EWI_ENABLE;
    ret = HAL_WWDG_Init(&WwdgHandle);
    if (ret != HAL_OK)
    {
        TRACE_FAT("SMN ::: WWDG init error %d", ret);
        FailSafePOR();
    }

    /* WWDG interrupt Init */
    HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
#endif
}

/**
 * WWDG re-initialization routine for standby mode.
 * Since in standby clock gets decreased from 32 -> 8 MHz (divided by 4), clock that ticks WWDG also slows down.
 * For this reason if code were to hang, IWDG would fire before WWDG, so we need to lower the prescaler by the
 * same ration (4).
 *
 * @return       None.
 */
void watchdog_reinit_for_standby(void)
{
#ifdef USE_WINDOW_WDOG
    // (1/PCLK=8MHz) * 4096 * 16 * 64 = 524 ms
    WwdgHandle.Init.Prescaler = WWDG_PRESCALER_16;
    if (HAL_WWDG_Init(&WwdgHandle) != HAL_OK)
    {
        TRACE_FAT("SMN ::: Standby WWDG reinit error");
        FailSafePOR();
    }
#endif
}

/**
 * WWDG re-initialization routine for run mode.
 *
 * @return       None.
 */
void watchdog_reinit_for_run(void)
{
#ifdef USE_WINDOW_WDOG
    // (1/PCLK=32MHz) * 4096 * 16 * 64 = 524 ms
    WwdgHandle.Init.Prescaler = WWDG_PRESCALER_64;
    if(HAL_WWDG_Init(&WwdgHandle) != HAL_OK)
    {
        TRACE_FAT("SMN ::: Run WWDG reinit error");
        FailSafePOR();
    }
#endif
}

/**
 * Update IWDG and WWDG
 *
 * @return       None.
 */
void watchdog_update(void)
{
#ifdef USE_INDEPENDENT_WDOG
    HAL_IWDG_Refresh(&IwdgHandle);
#endif
#ifdef USE_WINDOW_WDOG
    HAL_WWDG_Refresh(&WwdgHandle);
#endif
}


