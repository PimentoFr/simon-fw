/*
 * button.h
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

/**
 * @brief List of buttons
 */
typedef enum {
	BUTTON_RED = 0,
	BUTTON_GREEN,
	BUTTON_YELLOW,
	BUTTON_BLUE,
	BUTTON_CONF_UP,
	BUTTON_CONF_DOWN,
	BUTTON_CONF_MENU
} button_t;

GPIO_PinState button_read(button_t button);

#endif /* INC_BUTTON_H_ */
