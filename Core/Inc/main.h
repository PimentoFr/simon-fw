/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum {
	STATUS_OK = 0,
	STATUS_FAIL
} status_t;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CONF_MENU_Pin GPIO_PIN_14
#define CONF_MENU_GPIO_Port GPIOC
#define CONF_DOWN_Pin GPIO_PIN_15
#define CONF_DOWN_GPIO_Port GPIOC
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOA
#define MEASURE_EN_Pin GPIO_PIN_1
#define MEASURE_EN_GPIO_Port GPIOA
#define DIGIT_3_Pin GPIO_PIN_2
#define DIGIT_3_GPIO_Port GPIOA
#define DIGIT_2_Pin GPIO_PIN_3
#define DIGIT_2_GPIO_Port GPIOA
#define DIGIT_1_Pin GPIO_PIN_4
#define DIGIT_1_GPIO_Port GPIOA
#define BAT_MEASURE_Pin GPIO_PIN_5
#define BAT_MEASURE_GPIO_Port GPIOA
#define DIGIT_A_Pin GPIO_PIN_6
#define DIGIT_A_GPIO_Port GPIOA
#define DIGIT_B_Pin GPIO_PIN_7
#define DIGIT_B_GPIO_Port GPIOA
#define LED_BLUE_Pin GPIO_PIN_0
#define LED_BLUE_GPIO_Port GPIOB
#define BUTTON_RED_Pin GPIO_PIN_1
#define BUTTON_RED_GPIO_Port GPIOB
#define DIGIT_C_Pin GPIO_PIN_8
#define DIGIT_C_GPIO_Port GPIOA
#define DIGIT_D_Pin GPIO_PIN_9
#define DIGIT_D_GPIO_Port GPIOA
#define DIGIT_E_Pin GPIO_PIN_10
#define DIGIT_E_GPIO_Port GPIOA
#define DIGIT_F_Pin GPIO_PIN_11
#define DIGIT_F_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_12
#define LED_RED_GPIO_Port GPIOA
#define LED_YELLOW_Pin GPIO_PIN_15
#define LED_YELLOW_GPIO_Port GPIOA
#define BUTTON_GREEN_Pin GPIO_PIN_3
#define BUTTON_GREEN_GPIO_Port GPIOB
#define BUTTON_YELLOW_Pin GPIO_PIN_4
#define BUTTON_YELLOW_GPIO_Port GPIOB
#define BUTTON_BLUE_Pin GPIO_PIN_5
#define BUTTON_BLUE_GPIO_Port GPIOB
#define CONF_UP_Pin GPIO_PIN_6
#define CONF_UP_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_7
#define LED_GREEN_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#ifndef DEBUG
/* DIGIT_G is used by debugger for SWDIO in debug mode.
 * SO, we need to enable it only on release mode */
#define DIGIT_G_Pin GPIO_PIN_13
#define DIGIT_G_GPIO_Port GPIOA

#endif
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
