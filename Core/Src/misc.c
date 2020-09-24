/*
 * misc.c
 *
 *  Created on: 24 sept. 2020
 *      Author: vince
 */

#include "main.h"
#include "misc.h"

/**
 * @brief Get elapsed time between HAL Tick and a last timestamp in ms
 */
uint32_t misc_getElapsedTimeFromTick(uint32_t lastTimestamp) {
	return HAL_GetTick() - lastTimestamp;
}
