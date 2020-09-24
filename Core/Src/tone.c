/*
 * tone.c
 *
 *  Created on: Sep 24, 2020
 *      Author: vince
 */


#include "main.h"
#include "tone.h"

typedef struct {
	uint8_t toneEnabled;
	tone_t currentTonePlayed;
	TIM_HandleTypeDef * timerHandler;
} toneSettings_t;

static toneSettings_t _toneSettings = {0};
extern TIM_HandleTypeDef htim2;

status_t tone_init(void) {

	_toneSettings.toneEnabled = GPIO_PIN_RESET;
	_toneSettings.currentTonePlayed = TONE_NONE;
	_toneSettings.timerHandler = &htim2;

	return STATUS_OK;
}

status_t tone_stop(void) {

	_toneSettings.toneEnabled = GPIO_PIN_RESET;

	return STATUS_OK;
}

status_t tone_start(tone_t tone) {

	switch(tone) {
	case TONE_DO:
		break;
	case TONE_RE:
		break;
	case TONE_FA:
		break;
	case TONE_LA:
		break;
	case TONE_NONE:
	default:
		assert_param(!"Bad value");
		return STATUS_FAIL;
	}

	if(_toneSettings.toneEnabled == GPIO_PIN_SET) {
		if(tone_stop() != STATUS_OK) {
			return STATUS_FAIL;
		}
	}

	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 3999;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 2;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		assert_param(!"Can not init pwm");
		return STATUS_FAIL;
	}

	//HAL_StatusTy peDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)


	return STATUS_OK;
}
