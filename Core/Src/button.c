/*
 * button.c
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */

#include "main.h"
#include "button.h"

/**
 * @brief Duration for a short and long press in milliseconds
 */
#define BUTTON_SHORT_PRESS_DURATION_IN_MS	(25)
#define BUTTON_LONG_PRESS_DURATION_IN_MS	(1000)

typedef struct {
	uint32_t currentTimestamp;
} buttonController_t;

static buttonHandler_t _buttonHandlers[] = {
		{.buttonType = BUTTON_RED, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},		/* BUTTON_RED */
		{.buttonType = BUTTON_GREEN, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},		/* BUTTON_GREEN */
		{.buttonType = BUTTON_YELLOW, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},		/* BUTTON_YELLOW */
		{.buttonType = BUTTON_BLUE, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},		/* BUTTON_BLUE */
		{.buttonType = BUTTON_CONF_UP, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},	/* BUTTON_CONF_UP */
		{.buttonType = BUTTON_CONF_DOWN, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0},	/* BUTTON_CONF_DOWN */
		{.buttonType = BUTTON_CONF_MENU, .currentEvent=BUTTON_NO_PRESS, .startPressTimestamp = 0}	/* BUTTON_CONF_MENU */
};

static buttonController_t _buttonController = {
		.currentTimestamp = 0
};

static buttonHandler_t * _button_getHandler(buttonType_t buttonType);
static void _button_computeButton(buttonType_t buttonType);

/**
 * @brief Initialize the button controller
 */
void button_init(void) {

	return;
}

/**
 * @brief Compute the button controller
 * 		  All buttons will be listening and their current state will be updated
 */
void button_compute(void) {
	_buttonController.currentTimestamp = HAL_GetTick();

	for(uint8_t index = 0; index < BUTTON_MAX_SIZE; index++) {
		_button_computeButton((buttonType_t) index);
	}
}

/**
 * @brief Update the current event of the button according to their state
 * @param buttonType Type of button to update
 */
static void _button_computeButton(buttonType_t buttonType) {
	buttonHandler_t * buttonHandler = _button_getHandler(buttonType);
	uint8_t state = button_read(buttonType);

	if(buttonHandler == NULL) {
		assert_param(!"Pointer NULL");
		return;
	}

	switch(buttonHandler->currentEvent) {
	case BUTTON_NO_PRESS:
		if(state == GPIO_PIN_SET) {
			buttonHandler->currentEvent = BUTTON_BEFORE_SHORT_PRESS;
			buttonHandler->startPressTimestamp = _buttonController.currentTimestamp;
		}
		break;
	case BUTTON_BEFORE_SHORT_PRESS:
		if(state == GPIO_PIN_SET) {
			if((_buttonController.currentTimestamp - buttonHandler->startPressTimestamp) >= BUTTON_SHORT_PRESS_DURATION_IN_MS) {
				buttonHandler->currentEvent = BUTTON_SHORT_PRESS;
			}
		} else {
			buttonHandler->currentEvent = BUTTON_NO_PRESS;
		}
		break;
	case BUTTON_SHORT_PRESS:
		if(state == GPIO_PIN_SET) {
			if((_buttonController.currentTimestamp - buttonHandler->startPressTimestamp) >= BUTTON_LONG_PRESS_DURATION_IN_MS) {
				buttonHandler->currentEvent = BUTTON_LONG_PRESS;
			}
		} else {
			buttonHandler->currentEvent = BUTTON_SHORT_PRESS_AND_RELEASED;
		}
		break;
	case BUTTON_LONG_PRESS:
		if(state == GPIO_PIN_RESET) {
			buttonHandler->currentEvent = BUTTON_LONG_PRESS_AND_RELEASED;
		}
		break;
	case BUTTON_SHORT_PRESS_AND_RELEASED:
	case BUTTON_LONG_PRESS_AND_RELEASED:
		buttonHandler->currentEvent = BUTTON_NO_PRESS;
		break;
	default:
		assert_param(!"Bad Value");
		return;
	}
}


/**
 * @brief get the button corresponding Handler
 * @param buttonType Type of button
 * @return ButtonHandler* Pointer of the corresponding handler.
 */
static buttonHandler_t * _button_getHandler(buttonType_t buttonType) {
	buttonHandler_t * result = NULL;

	if((uint8_t) buttonType >= BUTTON_MAX_SIZE) {
		assert_param(!"Bad value");
		return NULL;
	}

	result = &_buttonHandlers[(uint8_t) buttonType];

	return result;
}

/**
 * @brief Read the state of the button.
 * @param button Button name
 * @return GPIO_PinState Button state
 * @retval GPIO_PIN_SET The button is pressed
 * @retval GPIO_PIN_RESET The button is not pressed
 */
GPIO_PinState button_read(buttonType_t button) {

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

	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) ? GPIO_PIN_RESET : GPIO_PIN_SET;
}


/**
 * @brief Get the current state of the given button
 * @param buttonType Type of button
 * @return Current event fot the given button
 */
buttonEvents_t button_getCurrentState(buttonType_t buttonType) {

	buttonHandler_t * buttonHandler = _button_getHandler(buttonType);
	if(buttonHandler == NULL) {
		assert_param(!"Pointer NULL");
		return BUTTON_NO_PRESS;
	}

	return buttonHandler->currentEvent;
}
