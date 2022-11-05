/**
  ******************************************************************************
  * @file    stm32g0xx_it.h
  * @author  MCD Application Team
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STL_APP_H
#define __STL_APP_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void        STL_NMI_Handler                 (void);
void        STL_HardFault_Handler           (void);
void        STL_SVC_Handler                 (void);
void        STL_PendSV_Handler              (void);
void        STL_SysTick_Handler             (void);
void        STL_TIM16_IRQHandler            (void);
ErrorStatus STL_InitClock_Xcross_Measurement(void);
void        STL_SetIsRunTimeInitialized     (void);
bool        STL_GetIsRunTimeInitialized     (void);

#endif /* __STL_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
