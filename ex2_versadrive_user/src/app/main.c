/**
 ******************************************************************************
 * @file   main.c
 * @brief  Main program body
 ******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "peripheral/adc/adc.h"
#include "peripheral/can/can/can.h"
#include "peripheral/comparator/comparator.h"
#include "peripheral/dac/dac.h"
#include "peripheral/dma/dma.h"
#include "peripheral/gpio/gpio.h"
#include "peripheral/opamp/opamp.h"
#include "peripheral/timer/timer.h"
#include "Protocol/CANopen/canopen_cfg.h"
#include "app.h"
#include "build_info.h"
#include "mains_meas.h"
#include "motor_control/motor_control/motor_control.h"
#include "performance_meas.h"
#include "relay.h"
#include "system.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "func_table.h"
#include "trace/trace.h"

// CPR stuff
#include "Communication/serialprocess.h"
#include "Platform/platform.h"
#include "hisense_app_mapping.h"
#include "hisense_app_public.h"

#if defined(DEBUG)
#include "../../Middlewares/Protocol/CANopen/CANopen/canopen.h"
#include "../../Middlewares/Protocol/CANopen/CANopen/co_drv.h"
#include "../../Middlewares/osci/osci/osci.h"
#else
#include "co_canopen.h"
#endif

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

#if (ALT_POWER_TEST == 1)
extern volatile uint8_t Ctrl_ALTTestMode__Enable;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void); /* parasoft-suppress NAMING-34 "Generated function names." */
/* USER CODE BEGIN PFP */
void User_Context_Init_Asm(void); /* parasoft-suppress NAMING-34 "Generated function names." */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
////////////////////////////////////////////////////////////////////////////////
/*!
 * @brief    C context initialization
 *
 * @param[in]    none
 * @return       none
 */
////////////////////////////////////////////////////////////////////////////////
void user_context_init(void)
{
    User_Context_Init_Asm();
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
uint32_t user_app_init(void)
{
    /* USER CODE BEGIN 1 */
    // FLASH_FlushCaches();
    /* Disable prefetch buffer */
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;
    /* Enable flash instruction cache */
    FLASH->ACR |= FLASH_ACR_ICEN;
    /* Enable flash data cache */
    FLASH->ACR |= FLASH_ACR_DCEN;
    /* USER CODE END 1 */

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    // update clock configuration
    SystemCoreClockUpdate();

    // Enable Power Control clock
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
    /* USER CODE END SysInit */

    /* USER CODE BEGIN 2 */

    // Print build info
    const char *build_info = build_info_get_ptr();
    UNUSED(build_info);
    TRACE_INF("MAIN : Build info...\n%s", build_info);

    // CAN communication initialization
#if defined(DEBUG)
    if (co_init() != eCANOPEN_INIT_OK)
    {
        system_set_init_fail(eSYSTEM_INIT_CANOPEN_FAIL);
    }
    TRACE_INF("MAIN : Initializing CAN Open");
#else
    coSimpleInit();
#endif

#if (ALT_POWER_TEST == 1)
    Ctrl_ALTTestMode__Enable = 1;
#endif

    // Oscilloscope initialization
#if defined(DEBUG)
    osci_init();
#endif

    // Performance measurement initialization
    performance_meas_init();

    // Motor control initialization
    if (motor_control_init() != eMOTOR_CONTROL_OK)
    {
        system_set_init_fail(eSYSTEM_INIT_MOTOR_CONTROL_FAIL);
    }

    mains_meas_init();
    mains_meas_register_neg_zero_cross_cbk(relay_proc_zero_cross);

    // CPR and APP stuff
    hisense_app_init();

    // Initialize washing machine state machine and protocol handler
    app_fsm_init();

    // Return back to SAFE code
    return 1UL;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) /* parasoft-suppress NAMING-34 "Generated function names." */
{
    RCC_PeriphCLKInitTypeDef periphClkInit = {0};

    /** Initializes the peripherals clocks
     */
    periphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12 | RCC_PERIPHCLK_FDCAN;
    periphClkInit.FdcanClockSelection  = RCC_FDCANCLKSOURCE_PLL;
    periphClkInit.Adc12ClockSelection  = RCC_ADC12CLKSOURCE_SYSCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&periphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void user_app_loop(void)
{
    app_loop();

#if defined(DEBUG)
    co_hndl();
#endif

    __WFI();
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) /* parasoft-suppress NAMING-34 "Generated function names." */
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
