/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Output_MCU_LAT_Pin GPIO_PIN_0
#define Output_MCU_LAT_GPIO_Port GPIOA
#define Output_MCU_E_Pin GPIO_PIN_1
#define Output_MCU_E_GPIO_Port GPIOA
#define Output_MCU_B_Pin GPIO_PIN_2
#define Output_MCU_B_GPIO_Port GPIOA
#define Output_MCU_B1_Pin GPIO_PIN_3
#define Output_MCU_B1_GPIO_Port GPIOA
#define Output_MCU_B2_Pin GPIO_PIN_4
#define Output_MCU_B2_GPIO_Port GPIOA
#define Output_MCU_A_Pin GPIO_PIN_5
#define Output_MCU_A_GPIO_Port GPIOA
#define Output_MCU_C_Pin GPIO_PIN_6
#define Output_MCU_C_GPIO_Port GPIOA
#define Output_MCU_CLK_Pin GPIO_PIN_7
#define Output_MCU_CLK_GPIO_Port GPIOA
#define Output_MCU_OE_Pin GPIO_PIN_1
#define Output_MCU_OE_GPIO_Port GPIOB
#define Output_MCU_G1_Pin GPIO_PIN_10
#define Output_MCU_G1_GPIO_Port GPIOB
#define Output_MCU_G2_Pin GPIO_PIN_11
#define Output_MCU_G2_GPIO_Port GPIOB
#define Input_BT3_Pin GPIO_PIN_12
#define Input_BT3_GPIO_Port GPIOB
#define Input_BT4_Pin GPIO_PIN_13
#define Input_BT4_GPIO_Port GPIOB
#define Input_BT1_Pin GPIO_PIN_14
#define Input_BT1_GPIO_Port GPIOB
#define Input_BT2_Pin GPIO_PIN_15
#define Input_BT2_GPIO_Port GPIOB
#define Input_DATA4_Pin GPIO_PIN_3
#define Input_DATA4_GPIO_Port GPIOB
#define Input_DATA3_Pin GPIO_PIN_4
#define Input_DATA3_GPIO_Port GPIOB
#define Input_DATA2_Pin GPIO_PIN_5
#define Input_DATA2_GPIO_Port GPIOB
#define Input_DATA1_Pin GPIO_PIN_6
#define Input_DATA1_GPIO_Port GPIOB
#define Output_MCU_R2_Pin GPIO_PIN_8
#define Output_MCU_R2_GPIO_Port GPIOB
#define Output_MCU_R1_Pin GPIO_PIN_9
#define Output_MCU_R1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
