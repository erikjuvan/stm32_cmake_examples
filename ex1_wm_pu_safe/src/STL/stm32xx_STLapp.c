/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "main.h"
#include "stm32xx_STLparam.h"
#include "stm32xx_STLlib.h"
#include "stm32xx_STLapp.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
   static volatile uint16_t tmpCC1_last;  /* keep last TIM16/CCR1 captured value */
   static bool     gb_IsInit = false;     /* STL RunTime initialization status.  */
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void STL_NMI_Handler(void)
{
#if defined (STL_VERBOSE)
  if (__HAL_RCC_GET_IT(RCC_IT_CSS))
  {
    while(__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TC) == 0)
    {   /* Wait previous transmission completion */
    }
    /* Re-configure USART baud rate to have 115200 bds with HSI clock (8MHz) */
    USART_Configuration();
    printf("\n\r Clock Source failure (Clock Security System)\n\r");
  }
  else
  {
    printf("\n\r NMI Exception\n\r");
  }
#endif /* STL_VERBOSE */

}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void STL_HardFault_Handler(void)
{
#if defined (STL_VERBOSE)
  printf("\n\r Hard fault Exception \n\r");
#endif /* STL_VERBOSE */

  FailSafePOR();
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void STL_SVC_Handler(void)
{
#if defined (STL_VERBOSE)
  printf("\n\r SVC Exception \n\r");
#endif /* STL_VERBOSE */

  FailSafePOR();  
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void STL_PendSV_Handler(void)
{
#if defined (STL_VERBOSE)
  printf("\n\r PendSVC Exception \n\r");
#endif /* STL_VERBOSE */

  FailSafePOR();  
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void STL_SysTick_Handler(void)
{
  HAL_IncTick();
     
   /* Verify TickCounter integrity */
  if ((TickCounter ^ TickCounterInv) == 0xFFFFFFFFuL)
  {
    TickCounter++;
    TickCounterInv = ~TickCounter;

    if (TickCounter >= SYSTICK_10ms_TB)
    {
      uint32_t RamTestResult;
   
      #if defined STL_EVAL_MODE
        /* Toggle LED_VLM for debug purposes */
        BSP_LED_Toggle(LED_VLM);
      #endif  /* STL_EVAL_MODE */

      /* Reset timebase counter */
      TickCounter = 0u;
      TickCounterInv = 0xFFFFFFFFuL;

      /* Set Flag read in main loop */
      TimeBaseFlag = 0xAAAAAAAAuL;
      TimeBaseFlagInv = 0x55555555uL;

      ISRCtrlFlowCnt += RAM_MARCHC_ISR_CALLER;
			  __disable_irq();
      RamTestResult = STL_TranspMarch();
			  __enable_irq();
      ISRCtrlFlowCntInv -= RAM_MARCHC_ISR_CALLER;

      switch ( RamTestResult )
      {
        case TEST_RUNNING:
          break;
        case TEST_OK:
          #ifdef STL_VERBOSE_2
          /* avoid any long string output here in the interrupt, '#' marks ram test completed ok */
            #ifndef __GNUC__
              putchar((int16_t)'#');
            #else
              __io_putchar((int16_t)'#');
            #endif /* __GNUC__ */
          #endif  /* STL_VERBOSE */
          #ifdef STL_EVAL_MODE
            /* Toggle LED_VLM for debug purposes */
            BSP_LED_Toggle(LED_VLM);
          #endif /* STL_EVAL_MODE */
          #if defined(STL_EVAL_LCD)
            ++MyRAMCounter;
          #endif /* STL_EVAL_LCD */
          break;
        case TEST_FAILURE:
        case CLASS_B_DATA_FAIL:
        default:
          #ifdef STL_VERBOSE
            printf("\n\r >>>>>>>>>>>>>>>>>>>  RAM Error (March C- Run-time check)\n\r");
          #endif  /* STL_VERBOSE_2 */
          FailSafePOR();
          break;
      } /* End of the switch */

      /* Do we reached the end of RAM test? */
      /* Verify 1st ISRCtrlFlowCnt integrity */
      if ((ISRCtrlFlowCnt ^ ISRCtrlFlowCntInv) == 0xFFFFFFFFuL)
      {
        if (RamTestResult == TEST_OK)
        {
  /* ==============================================================================*/
  /* MISRA violation of rule 17.4 - pointer arithmetic is used to check RAM test control flow */
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
		#pragma diag_suppress=Pm088
	#endif /* __IAR_SYSTEMS_ICC__ */
          if (ISRCtrlFlowCnt != RAM_TEST_COMPLETED)
	#ifdef __IAR_SYSTEMS_ICC__  /* IAR Compiler */
		#pragma diag_default=Pm088
	#endif /* __IAR_SYSTEMS_ICC__ */
  /* ==============================================================================*/
          {
          #ifdef STL_VERBOSE
            printf("\n\r Control Flow error (RAM test) \n\r");
          #endif  /* STL_VERBOSE */
          FailSafePOR();
          }
          else  /* Full RAM was scanned */
          {
            ISRCtrlFlowCnt = 0u;
            ISRCtrlFlowCntInv = 0xFFFFFFFFuL;
          }
        } /* End of RAM completed if*/
      } /* End of control flow monitoring */
      else
      {
      #ifdef STL_VERBOSE
        printf("\n\r Control Flow error in ISR \n\r");
      #endif  /* STL_VERBOSE */
      FailSafePOR();
      }
      #if defined STL_EVAL_MODE
        /* Toggle LED_VLM for debug purposes */
        BSP_LED_Toggle(LED_VLM);
      #endif  /* STL_EVAL_MODE */
    } /* End of the 10 ms timebase interrupt */
  } 
}

/******************************************************************************/
/**
  * @brief  This function handles TIM16 global interrupt request.
  * @param  : None
  * @retval : None
  */
void STL_TIM16_IRQHandler(void)
{
  uint16_t tmpCC1_last_cpy;
   
  if ((TIM16->SR & TIM_SR_CC1IF) != 0u )
  {
    /* store previous captured value */
    tmpCC1_last_cpy = tmpCC1_last; 
    /* get currently captured value */
    tmpCC1_last = (uint16_t)(TIM16->CCR1);
    /* The CC1IF flag is already cleared here be reading CCR1 register */

    /* overight results only in case the data is required */
    if (LSIPeriodFlag == 0u)
    {
      /* take correct measurement only */
      if ((TIM16->SR & TIM_SR_CC1OF) == 0u)
      {
        /* Compute period length */
        PeriodValue = ((uint32_t)(tmpCC1_last) - (uint32_t)(tmpCC1_last_cpy)) & 0xFFFFuL;
        PeriodValueInv = ~PeriodValue;
      
        /* Set Flag tested at main loop */
        LSIPeriodFlag = 0xAAAAAAAAuL;
        LSIPeriodFlagInv = 0x55555555uL;
      }
      else
      {
        /* ignore computation in case of IC overflow */
        TIM16->SR &= (uint16_t)(~TIM_SR_CC1OF);
      }
    }
    /* ignore computation in case data is not required */
  }
}

/******************************************************************************/
/**
  * @brief Configure TIM16 to measure LSI period
  * @param  : None
  * @retval : ErrorStatus = (ERROR, SUCCESS)
  */
ErrorStatus STL_InitClock_Xcross_Measurement(void)
{
  ErrorStatus result = SUCCESS;
  TIM_HandleTypeDef  tim_capture_handle;
  TIM_IC_InitTypeDef tim_input_config;
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  __PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /* Configue LSI as RTC clock soucre */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    result = ERROR;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    result = ERROR;
  }
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
  
  /* TIM16 Peripheral clock enable */
   __TIM16_CLK_ENABLE();
  
  /* Configure the NVIC for TIM16 */
  HAL_NVIC_SetPriority(TIM16_FDCAN_IT0_IRQn, 0, 0u);
  
  /* Enable the TIM16 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM16_FDCAN_IT0_IRQn);
  
  /* TIM16 configuration: Input Capture mode ---------------------
  The Rising edge is used as active edge, ICC input divided by 8
  The TIM16 CCR1 is used to compute the frequency value. 
  ------------------------------------------------------------ */
  tim_capture_handle.Instance = TIM16;
  tim_capture_handle.Init.Prescaler         = 0u; 
  tim_capture_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;  
  tim_capture_handle.Init.Period            = 0xFFFFFFFFul; 
  tim_capture_handle.Init.ClockDivision     = 0u;     
  tim_capture_handle.Init.RepetitionCounter = 0u; 
  tim_capture_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; 
  
  
  /* define internal HAL driver status here as handle structure is defined locally */
  __HAL_RESET_HANDLE_STATE(&tim_capture_handle);
  if(HAL_TIM_IC_Init(&tim_capture_handle) != HAL_OK)
  {
    /* Initialization Error */
    result = ERROR;
  }

  /* Configure the TIM16 Input Capture of channel 1 */
  tim_input_config.ICPolarity  = TIM_ICPOLARITY_RISING;
  tim_input_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
  tim_input_config.ICPrescaler = TIM_ICPSC_DIV8;
  tim_input_config.ICFilter    = 0u;
  if(HAL_TIM_IC_ConfigChannel(&tim_capture_handle, &tim_input_config, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    result = ERROR;
  }
  
  
  HAL_TIMEx_TISelection(&tim_capture_handle, TIM_TIM16_TI1_LSI, TIM_CHANNEL_1);
  
  /* Reset the flags */
  tim_capture_handle.Instance->SR = 0u;
  LSIPeriodFlag = 0u;
  
  /* Start the TIM Input Capture measurement in interrupt mode */
  if(HAL_TIM_IC_Start_IT(&tim_capture_handle, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Initialization Error */
    result = ERROR;
  }
  return(result);
}

/******************************************************************************/
/**
  * @brief Set STL RunTime initialized flag to true.
  * @param  : None
  * @retval : None
  */
void STL_SetIsRunTimeInitialized(void)
{
    gb_IsInit = true;
}

/******************************************************************************/
/**
  * @brief Get RunTime initialized flag value.
  * @param  : None
  * @retval : bool = (true, false)
  */
bool STL_GetIsRunTimeInitialized(void)
{
    return gb_IsInit;
}
