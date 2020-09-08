/*
 * led.h
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */

#ifndef INC_LED_H_
#define INC_LED_H_

/**
 * @brief List of led colors
 */
typedef enum {
	LED_RED = 0,
	LED_GREEN,
	LED_YELLOW,
	LED_BLUE
} ledColor_t;


void led_enable(ledColor_t ledColor);
void led_disable(ledColor_t ledColor);


#endif /* INC_LED_H_ */
