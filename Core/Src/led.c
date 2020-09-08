/*
 * led.c
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */


#include "main.h"
#include "led.h"

/**
 * @brief Enable the colored led
 * @param ledColor Color of the led
 */
void led_enable(ledColor_t ledColor) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;

	switch(ledColor) {
	case LED_RED:
		GPIOx = LED_RED_GPIO_Port;
		GPIO_Pin = LED_RED_Pin;
		break;
	case LED_GREEN:
		GPIOx = LED_GREEN_GPIO_Port;
		GPIO_Pin = LED_GREEN_Pin;
		break;
	case LED_YELLOW:
		GPIOx = LED_YELLOW_GPIO_Port;
		GPIO_Pin = LED_YELLOW_Pin;
		break;
	case LED_BLUE:
		GPIOx = LED_BLUE_GPIO_Port;
		GPIO_Pin = LED_BLUE_Pin;
		break;
	default:
		assert_param(!"Wrong color");
		return;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/**
 * @brief Disable the colored led
 * @param ledColor Color of the led
 */
void led_disable(ledColor_t ledColor) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;

	switch(ledColor) {
	case LED_RED:
		GPIOx = LED_RED_GPIO_Port;
		GPIO_Pin = LED_RED_Pin;
		break;
	case LED_GREEN:
		GPIOx = LED_GREEN_GPIO_Port;
		GPIO_Pin = LED_GREEN_Pin;
		break;
	case LED_YELLOW:
		GPIOx = LED_YELLOW_GPIO_Port;
		GPIO_Pin = LED_YELLOW_Pin;
		break;
	case LED_BLUE:
		GPIOx = LED_BLUE_GPIO_Port;
		GPIO_Pin = LED_BLUE_Pin;
		break;
	default:
		assert_param(!"Wrong color");
		return;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

