/*
 * pseudo_random_generator.c
 *
 *  Created on: Sep 22, 2020
 *      Author: vince
 */


#include "main.h"
#include "pseudo_random_generator.h"


/**
 * @brief Create a pseudo random value
 * @return uint32_t Generated pseudo random value
 */
uint32_t pseudoRandomGenerator_getValue(void) {
	static uint32_t seed = 0;
	/**
	 * To generate a kind of true random value, we need a pseudo random seed.
	 * For that, we can create it with :
	 * - ADC measurement. Maybe, do 10 ADC measurements of the battery
	 * - Tick : Use the HAL Tick
	 */
	seed = seed + HAL_GetTick();

	seed = seed + 271828891;
	seed = seed * 3141517;

	seed = ((seed & 0xFF) << 24) | (seed >> 8);

	return seed;
}
