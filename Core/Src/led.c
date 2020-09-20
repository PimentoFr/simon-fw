/*
 * led.c
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */


#include "main.h"
#include "led.h"

typedef struct {
	GPIO_PinState red;
	GPIO_PinState yellow;
	GPIO_PinState green;
	GPIO_PinState blue;
} sLedState_t;


sLedState_t _sLedState = {
		.red = GPIO_PIN_RESET,
		.yellow = GPIO_PIN_RESET,
		.blue = GPIO_PIN_RESET,
		.green = GPIO_PIN_RESET
};

/**
 * @brief Set the enable state the colored led
 * @param ledColor Color of the led
 * @param newState New state of the led
 */
void led_setEnable(ledColor_t ledColor, GPIO_PinState newState) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;
	GPIO_PinState * currentState = NULL;
	switch(ledColor) {
	case LED_RED:
		GPIOx = LED_RED_GPIO_Port;
		GPIO_Pin = LED_RED_Pin;
		currentState = &_sLedState.red;
		break;
	case LED_GREEN:
		GPIOx = LED_GREEN_GPIO_Port;
		GPIO_Pin = LED_GREEN_Pin;
		currentState = &_sLedState.green;
		break;
	case LED_YELLOW:
		GPIOx = LED_YELLOW_GPIO_Port;
		GPIO_Pin = LED_YELLOW_Pin;
		currentState = &_sLedState.yellow;
		break;
	case LED_BLUE:
		GPIOx = LED_BLUE_GPIO_Port;
		GPIO_Pin = LED_BLUE_Pin;
		currentState = &_sLedState.blue;
		break;
	default:
		assert_param(!"Wrong color");
		return;
	}

	if(*currentState == newState) {
		return;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, newState);
	*currentState = newState;
}
