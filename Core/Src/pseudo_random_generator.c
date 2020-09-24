/*
 * pseudo_random_generator.c
 *
 *  Created on: Sep 22, 2020
 *      Author: vince
 */


#include "main.h"
#include "pseudo_random_generator.h"

typedef struct {
	uint32_t seed;
} prngSettings_t;

static prngSettings_t _prngSettings = {0};

status_t pseudoRandomGenerator_init(void) {

	/**
	 * To generate a kind of true random value, we need a pseudo random seed.
	 * For that, we can create it with :
	 * - ADC measurement. Maybe, do 10 ADC measurements of the battery
	 * - Tick : Use the HAL Tick
	 * - Player : Player should start game with a press on a button. The timestamp of the press (from the power up)
	 * 			  could be use to edit this seed
	 */
	_prngSettings.seed = HAL_GetTick() + SysTick->VAL;

	return STATUS_OK;
}
/**
 * @brief Create a pseudo random value
 * @return uint32_t Generated pseudo random value
 */
uint16_t pseudoRandomGenerator_getValue(void) {

	_prngSettings.seed = _prngSettings.seed * 214013 + 2531011;

	return _prngSettings.seed >> 16;
}
