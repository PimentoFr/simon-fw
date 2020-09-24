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
	LED_BLUE,
	LED_MAX_SIZE
} ledColor_t;


void led_setEnable(ledColor_t ledColor, GPIO_PinState newState);
#define led_enable(ledColor) led_setEnable(ledColor, GPIO_PIN_SET)
#define led_disable(ledColor) led_setEnable(ledColor, GPIO_PIN_RESET)
void led_disableAll(void);

#endif /* INC_LED_H_ */
