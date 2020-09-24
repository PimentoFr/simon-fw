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
	BUTTON_BLUE,
	BUTTON_YELLOW,
	BUTTON_CONF_UP,
	BUTTON_CONF_DOWN,
	BUTTON_CONF_MENU,
	BUTTON_MAX_SIZE
} buttonType_t;

typedef enum {
	BUTTON_NO_PRESS = 0,
	BUTTON_BEFORE_SHORT_PRESS, 			/* Pressed during less than 100 ms */
	BUTTON_SHORT_PRESS,		 			/* Pressed between 100 ms and 1s */
	BUTTON_SHORT_PRESS_AND_RELEASED, 	/* Button is released while it is in short press */
	BUTTON_LONG_PRESS,					/* Pressed over than 1s */
	BUTTON_LONG_PRESS_AND_RELEASED	 	/* Button is released while it is in long press */
} buttonEvents_t;

typedef struct {
	buttonType_t buttonType;
	buttonEvents_t currentEvent;	/* Current event */
	uint32_t startPressTimestamp;	/* Start time of the button press */
} buttonHandler_t;

GPIO_PinState button_read(buttonType_t button);
void button_init(void);
void button_compute(void);
buttonEvents_t button_getCurrentState(buttonType_t buttonType);

#endif /* INC_BUTTON_H_ */
