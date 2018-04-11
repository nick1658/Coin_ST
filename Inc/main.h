/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
 
#undef __NVIC_PRIO_BITS
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Nand_D2_Pin GPIO_PIN_2
#define Nand_D2_GPIO_Port GPIOE
#define Nand_D3_Pin GPIO_PIN_3
#define Nand_D3_GPIO_Port GPIOE
#define Nand_D7_Pin GPIO_PIN_4
#define Nand_D7_GPIO_Port GPIOE
#define Nand_D6_Pin GPIO_PIN_5
#define Nand_D6_GPIO_Port GPIOE
#define Nand_D5_Pin GPIO_PIN_6
#define Nand_D5_GPIO_Port GPIOE
#define Nand_D4_Pin GPIO_PIN_13
#define Nand_D4_GPIO_Port GPIOC
#define User_Led_Pin GPIO_PIN_14
#define User_Led_GPIO_Port GPIOC
#define ADC_IN2_Pin GPIO_PIN_4
#define ADC_IN2_GPIO_Port GPIOA
#define ADC_IN1_Pin GPIO_PIN_5
#define ADC_IN1_GPIO_Port GPIOA
#define ADC_IN0_Pin GPIO_PIN_6
#define ADC_IN0_GPIO_Port GPIOA
#define IN7_Pin GPIO_PIN_8
#define IN7_GPIO_Port GPIOE
#define IN7_EXTI_IRQn EXTI9_5_IRQn
#define IN6_Pin GPIO_PIN_9
#define IN6_GPIO_Port GPIOE
#define IN6_EXTI_IRQn EXTI9_5_IRQn
#define IN5_Pin GPIO_PIN_10
#define IN5_GPIO_Port GPIOE
#define IN5_EXTI_IRQn EXTI15_10_IRQn
#define IN4_Pin GPIO_PIN_11
#define IN4_GPIO_Port GPIOE
#define IN4_EXTI_IRQn EXTI15_10_IRQn
#define IN3_Pin GPIO_PIN_12
#define IN3_GPIO_Port GPIOE
#define IN3_EXTI_IRQn EXTI15_10_IRQn
#define IN2_Pin GPIO_PIN_13
#define IN2_GPIO_Port GPIOE
#define IN2_EXTI_IRQn EXTI15_10_IRQn
#define IN1_Pin GPIO_PIN_14
#define IN1_GPIO_Port GPIOE
#define IN1_EXTI_IRQn EXTI15_10_IRQn
#define IN0_Pin GPIO_PIN_15
#define IN0_GPIO_Port GPIOE
#define IN0_EXTI_IRQn EXTI15_10_IRQn
#define OUT7_Pin GPIO_PIN_10
#define OUT7_GPIO_Port GPIOB
#define OUT6_Pin GPIO_PIN_14
#define OUT6_GPIO_Port GPIOB
#define OUT5_Pin GPIO_PIN_15
#define OUT5_GPIO_Port GPIOB
#define OUT4_Pin GPIO_PIN_13
#define OUT4_GPIO_Port GPIOD
#define OUT3_Pin GPIO_PIN_14
#define OUT3_GPIO_Port GPIOD
#define OUT2_Pin GPIO_PIN_15
#define OUT2_GPIO_Port GPIOD
#define OUT1_Pin GPIO_PIN_6
#define OUT1_GPIO_Port GPIOC
#define OUT0_Pin GPIO_PIN_7
#define OUT0_GPIO_Port GPIOC
#define LAN8720_RST_Pin GPIO_PIN_8
#define LAN8720_RST_GPIO_Port GPIOA
#define Uart1TX_Pin GPIO_PIN_9
#define Uart1TX_GPIO_Port GPIOA
#define Uart1RX_Pin GPIO_PIN_10
#define Uart1RX_GPIO_Port GPIOA
#define Nand_WP_Pin GPIO_PIN_10
#define Nand_WP_GPIO_Port GPIOC
#define Nand_WE_Pin GPIO_PIN_11
#define Nand_WE_GPIO_Port GPIOC
#define Nand_ALE_Pin GPIO_PIN_12
#define Nand_ALE_GPIO_Port GPIOC
#define Nand_CLE_Pin GPIO_PIN_0
#define Nand_CLE_GPIO_Port GPIOD
#define Nand_CE_Pin GPIO_PIN_1
#define Nand_CE_GPIO_Port GPIOD
#define Nand_RE_Pin GPIO_PIN_2
#define Nand_RE_GPIO_Port GPIOD
#define Nand_RB_Pin GPIO_PIN_3
#define Nand_RB_GPIO_Port GPIOD
#define SPI2_CS_Pin GPIO_PIN_7
#define SPI2_CS_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_8
#define SPI1_CS_GPIO_Port GPIOB
#define SPI1_WP_Pin GPIO_PIN_9
#define SPI1_WP_GPIO_Port GPIOB
#define Nand_D0_Pin GPIO_PIN_0
#define Nand_D0_GPIO_Port GPIOE
#define Nand_D1_Pin GPIO_PIN_1
#define Nand_D1_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
//#include "user_app.h"
/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
