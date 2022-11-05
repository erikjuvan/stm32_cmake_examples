/**
 * @file     main.c
 * @brief    SAFE main.
 * @author   Erik Juvan
 * @date     14.07.2022
 * @version  V1.0.0
 */

////////////////////////////////////////////////////////////////////////////////
/**
 *
 * @addtogroup MAIN
 * @{ <!-- BEGIN GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "func_table.h"
#include "stm32xx_STLlib.h"
#include "stm32xx_STLapp.h"
#include "project_conf.h"
#include "trace.h"
#include "apptime.h"
#include "pu_safe.h"
#include "safe_info.h"
#include "watchdog.h"

#ifdef QUALIFICATION_TEST
#include "test_switch.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////////////
#define SAFE_APP_LOOP_PERIOD_MS  10     /**< Safe app loop period. */

////////////////////////////////////////////////////////////////////////////////
// Public variables
////////////////////////////////////////////////////////////////////////////////
bool g_user_app_init_is_done = false;  /**< User app initialization done flag. */
uint8_t gu8_enable_user_irq = 0;       /**< Flag to enable/disable user app. */


////////////////////////////////////////////////////////////////////////////////
// Local variables
////////////////////////////////////////////////////////////////////////////////
static apptime_t safe_app_loop_ts = 0; /**< Safe loop timestamp. */


////////////////////////////////////////////////////////////////////////////////
// Function declaration
////////////////////////////////////////////////////////////////////////////////
void SystemClock_Config  (void);
void USART_Configuration (void);


////////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////////
/**
 * SAFE initialization.
 *
 * @return    None.
 */
static void Safe_App_Init(void)
{
#if defined(STL_VERBOSE) || defined(CONF_TRACE_ENABLE)
    // GotoCompilerStartUp calls Startup_Copy_Handler which clears UartHandle in ram so we have to reconfigure it
    USART_Configuration();

    TRACE_Init();
#endif

    // Check if SRAM parity is enabled
    if (FLASH->OPTR & FLASH_OPTR_RAM_PARITY_CHECK) // 1 - disabled
    {
        TRACE_FAT("SMN ::: SRAM parity check MUST be enabled but it is not!");
        FailSafePOR();
    }

    // Initialize safe info struct with constants not known at compile time (e.g. CRC)
    safe_info_init();

    // Print version to TRACE
    TRACE_INF("SMN ::: SAFE version: %u.%u.%u  CRC: %X",
        g_safe_info.version.major, g_safe_info.version.minor, g_safe_info.version.patch, g_safe_info.crc32);

    pu_safe_init();
}

/**
 * SAFE loop.
 *
 * @return    None.
 */
static void Safe_App_Loop(void)
{
#ifdef QUALIFICATION_TEST
    if (g_test_switch.stl.wdg_test == true)
    {
        // wait for IWDG timer to reset MCU
        while (1) {}
    }
#endif

    if (apptime_is_elapsed(safe_app_loop_ts, SAFE_APP_LOOP_PERIOD_MS))
    {
        // Update watchdog counters so in case FailSafePOR happens in STL_DoRunTimeChecks USER definitely has enough time to save to FLASH
        watchdog_update();

        // Perform STL checks
        STL_DoRunTimeChecks();

        safe_app_loop_ts = apptime_get_ms();
        pu_safe_proc();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Public functions
////////////////////////////////////////////////////////////////////////////////
/**
 * SAFE main function.
 *
 * @return    None.
 */
void Safe_Main(void)
{
    __enable_irq();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    StartUpClock_Config();

    // This is where the main self-test routines are initialized
    STL_InitRunTimeChecks();

    STL_SetIsRunTimeInitialized();

    // WDT are initialized in STL_InitRunTimeChecks so we have to call this after that
    watchdog_init();

    // Initialize safe modules
    Safe_App_Init();

    // Initialize USER APP context - done in USER APP startup_xxx.s file
    pg_user_func_table->fp_context_init();

    // user initialization needs interrupts to succeed
    gu8_enable_user_irq = 1;

    // Initialize User App - the usual code in main prior to infinite loop
    pg_user_func_table->fp_app_init();

    g_user_app_init_is_done = 1;

    /* Infinite loop */
    while (1)
    {
        Safe_App_Loop();

        // call USER app loop functions
        pg_user_func_table->fp_app_loop();
    }
}

/**
 * System wide runtime clock configuration.
 *
 * @return     None.
 */
void StartUpClock_Config(void)
{
  HAL_StatusTypeDef ret;
  RCC_OscInitTypeDef RCC_OscInitStruct   = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct   = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv              = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState            = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN            = 8;
  RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV4;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK)
  {
      TRACE_FAT("SMN ::: HAL_RCC_OscConfig error %d", ret);
      FailSafePOR();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType       =  RCC_CLOCKTYPE_HCLK
                                      |RCC_CLOCKTYPE_SYSCLK
                                      |RCC_CLOCKTYPE_PCLK1  ;
  RCC_ClkInitStruct.SYSCLKSource    = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider   = RCC_SYSCLK_DIV1       ;
  RCC_ClkInitStruct.APB1CLKDivider  = RCC_HCLK_DIV1         ;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
  if (ret != HAL_OK)
  {
      TRACE_FAT("SMN ::: HAL_RCC_ClockConfig error %d", ret);
      FailSafePOR();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
							  |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
							  |RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_HSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_HSI;
  ret = HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
  if (ret != HAL_OK)
  {
      TRACE_FAT("SMN ::: HAL_RCCEx_PeriphCLKConfig error %d", ret);
      FailSafePOR();
  }
}

/**
 * Standby clock configuration.
 *
 * @return     None.
 */
void StandbyClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    // If you try to reconfigure clock and call HAL_RCC_OscConfig with RCC_PLL_OFF there is an error.
    // This solves it but I don't know if it is the correct way
    HAL_RCC_DeInit();

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        TRACE_FAT("SMN ::: StandbyClock_Config HAL_RCC_OscConfig error");
        FailSafePOR();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        TRACE_FAT("SMN ::: StandbyClock_Config HAL_RCC_ClockConfig error");
        FailSafePOR();
    }
}

/**
 * Print stack dump to aid in debugging
 *
 * @return     None.
 */
void print_stackdump(void)
{
#ifdef LOG_VIA_UART
    uint32_t addr = 0;
    uint32_t *sp;

    asm volatile ("mov %0, sp\n\t"
        : "=r" ( addr)
    );
    sp = (uint32_t*) addr;

    TRACE_INF("\r\nStack dump...\r\nsp: 0x%X", (unsigned int) sp);
    while ((uint32_t) sp < (uint32_t) 0x20008000)
    {
        printf("0x%X\r\n", (unsigned int) *sp);
        sp++;
    }
    TRACE_INF("Stack dump end");
#endif
}


////////////////////////////////////////////////////////////////////////////////
/**
 * @} <!-- END GROUP -->
 */
////////////////////////////////////////////////////////////////////////////////
