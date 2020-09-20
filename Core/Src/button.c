/*
 * button.c
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */

#include "main.h"
#include "button.h"

/**
 * @brief Read the state of the button
 * @param button Button name
 * @return GPIO_PinState Button state
 */
GPIO_PinState button_read(button_t button) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;

	switch(button) {
	case BUTTON_RED:
		GPIOx = BUTTON_RED_GPIO_Port;
		GPIO_Pin = BUTTON_RED_Pin;
		break;
	case BUTTON_GREEN:
		GPIOx = BUTTON_GREEN_GPIO_Port;
		GPIO_Pin = BUTTON_GREEN_Pin;
		break;
	case BUTTON_YELLOW:
		GPIOx = BUTTON_YELLOW_GPIO_Port;
		GPIO_Pin = BUTTON_YELLOW_Pin;
		break;
	case BUTTON_BLUE:
		GPIOx = BUTTON_BLUE_GPIO_Port;
		GPIO_Pin = BUTTON_BLUE_Pin;
		break;
	case BUTTON_CONF_UP:
		GPIOx = CONF_UP_GPIO_Port;
		GPIO_Pin = CONF_UP_Pin;
		break;
	case BUTTON_CONF_DOWN:
		GPIOx = CONF_DOWN_GPIO_Port;
		GPIO_Pin = CONF_DOWN_Pin;
		break;
	case BUTTON_CONF_MENU:
		GPIOx = CONF_MENU_GPIO_Port;
		GPIO_Pin = CONF_MENU_Pin;
		break;
	default:
		assert_param(!"Wrong button");
		return GPIO_PIN_RESET;
	}

	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
