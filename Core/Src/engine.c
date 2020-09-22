/*
 * engine.c
 *
 *  Created on: 21 sept. 2020
 *      Author: vince
 */

#include "main.h"
#include "engine.h"
#include "button.h"
#include "led.h"

static void _engine_testButtonCompute(void);

void engine_init(void) {

	button_init();

	return;
}

void engine_compute(void) {

	while(1) {
		button_compute();

		_engine_testButtonCompute();
	}
}


static void _engine_testButtonCompute(void) {

	/* Button red */
	if(button_getCurrentState(BUTTON_RED) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		led_enable(LED_RED);
		HAL_Delay(1000);
	}
	else if(button_getCurrentState(BUTTON_RED) == BUTTON_LONG_PRESS_AND_RELEASED)
	{
		led_enable(LED_RED);
		HAL_Delay(5000);
	}
	led_disable(LED_RED);

	/* Button green */
	if(button_getCurrentState(BUTTON_GREEN) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		led_enable(LED_GREEN);
		HAL_Delay(1000);
	}
	else if(button_getCurrentState(BUTTON_GREEN) == BUTTON_LONG_PRESS_AND_RELEASED)
	{
		led_enable(LED_GREEN);
		HAL_Delay(5000);
	}
	led_disable(LED_GREEN);

	/* Button Blue */
	if(button_getCurrentState(BUTTON_BLUE) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		led_enable(LED_BLUE);
		HAL_Delay(1000);
	}
	else if(button_getCurrentState(BUTTON_BLUE) == BUTTON_LONG_PRESS_AND_RELEASED)
	{
		led_enable(LED_BLUE);
		HAL_Delay(5000);
	}
	led_disable(LED_BLUE);

	/* Buton Yellow */
	if(button_getCurrentState(BUTTON_YELLOW) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		led_enable(LED_YELLOW);
		HAL_Delay(1000);
	}
	else if(button_getCurrentState(BUTTON_YELLOW) == BUTTON_LONG_PRESS_AND_RELEASED)
	{
		led_enable(BUTTON_YELLOW);
		HAL_Delay(5000);
	}
	led_disable(BUTTON_YELLOW);
}
