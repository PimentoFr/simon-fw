/*
 * scoring.c
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */


#include "main.h"
#include "scoring.h"

/**
 * @brief Max number of segment on a 7-seg display
 */
#define DIGIT_DISPLAY_MAX_SEG	7



static void _scoring_setDigitSegment(scoringDigitSeg_t segment, GPIO_PinState gpioState);
static void _scoring_setEnableDigit(scoringDigit_t digit, GPIO_PinState state);

#define scoring_enableDigitSegment(segment)	 _scoring_setDigitSegment(segment, GPIO_PIN_SET)
#define scoring_disableDigitSegment(segment) _scoring_setDigitSegment(segment, GPIO_PIN_RESET)

/**
 * @brief 7-segment display value mapping
 */
uint8_t digitValueSegments[] = {
	/**
	 * Segment of 7-seg display
	 *    -GFEDCBA		 */
		0b00111111, /** DIGIT_VALUE_0, */
		0b00000110, /** DIGIT_VALUE_1, */
		0b01011011, /** DIGIT_VALUE_2, */
		0b01001111, /** DIGIT_VALUE_3, */
		0b01100110, /** DIGIT_VALUE_4, */
		0b01101101, /** DIGIT_VALUE_5, */
		0b01111101, /** DIGIT_VALUE_6, */
		0b00000111, /** DIGIT_VALUE_7, */
		0b01111111, /** DIGIT_VALUE_8, */
		0b01101111, /** DIGIT_VALUE_9, */
		0b00000000  /** DIGIT_VALUE_CLEAR */
};

/**
 * @brief Display a digit
 */
void scoring_displayDigit(scoringDigit_t digit, scoringDigitValue_t value) {

	uint8_t segmentsNewState = digitValueSegments[DIGIT_VALUE_CLEAR];
	uint8_t mask = 0x01;
	uint8_t nbSegment = 0;

	if(value >= DIGIT_VALUE_MAX_SIZE) {
		assert_param(!"Invalid Value");
		return;
	}

	segmentsNewState = digitValueSegments[value];

	/* Clear all digits */
	scoring_disableAllDigits();

	/* Switch on the cathode of the 7-seg digit */
	_scoring_setEnableDigit(digit, GPIO_PIN_SET);

	/* Drive 7-seg segments*/
	for(nbSegment = 0; nbSegment < DIGIT_DISPLAY_MAX_SEG; nbSegment ++) {

		uint8_t segmentNewStateMasked = segmentsNewState & mask;

		if(segmentNewStateMasked > 0) {
			scoring_enableDigitSegment((scoringDigitSeg_t) nbSegment);
		} else {
			scoring_disableDigitSegment((scoringDigitSeg_t) nbSegment);
		}

		mask = mask << 1;
	}
}

/**
 * @brief Set state to a segment
 */
static void _scoring_setDigitSegment(scoringDigitSeg_t segment, GPIO_PinState gpioState) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;

	switch(segment) {
	case DIGIT_A:
		GPIOx = DIGIT_A_GPIO_Port;
		GPIO_Pin = DIGIT_A_Pin;
		break;
	case DIGIT_B:
		GPIOx = DIGIT_B_GPIO_Port;
		GPIO_Pin = DIGIT_B_Pin;
		break;
	case DIGIT_C:
		GPIOx = DIGIT_C_GPIO_Port;
		GPIO_Pin = DIGIT_C_Pin;
		break;
	case DIGIT_D:
		GPIOx = DIGIT_D_GPIO_Port;
		GPIO_Pin = DIGIT_D_Pin;
		break;
	case DIGIT_E:
		GPIOx = DIGIT_E_GPIO_Port;
		GPIO_Pin = DIGIT_E_Pin;
		break;
	case DIGIT_F:
		GPIOx = DIGIT_F_GPIO_Port;
		GPIO_Pin = DIGIT_F_Pin;
		break;
	case DIGIT_G:
#ifdef DEBUG
		return;
#else
		GPIOx = DIGIT_G_GPIO_Port;
		GPIO_Pin = DIGIT_G_Pin;
#endif
		break;
	default:
		assert_param(!"Wrong segment");
		return;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, gpioState);
}

/**
 * @brief Enable a 7-digit display
 * @param digit Digit to enable/disable
 * @param state State of the digit
 */
static void _scoring_setEnableDigit(scoringDigit_t digit, GPIO_PinState state) {

	GPIO_TypeDef *GPIOx = NULL;
	uint16_t GPIO_Pin = 0;
	switch(digit) {
	case DIGIT_1:
		GPIOx = DIGIT_1_GPIO_Port;
		GPIO_Pin = DIGIT_1_Pin;
		break;
	case DIGIT_2:
		GPIOx = DIGIT_2_GPIO_Port;
		GPIO_Pin = DIGIT_2_Pin;
		break;
	case DIGIT_3:
		GPIOx = DIGIT_3_GPIO_Port;
		GPIO_Pin = DIGIT_3_Pin;
		break;
	default:
		assert_param(!"Wrong digit");
		return;
	}

	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, state);
}

/**
 * @brief Disable all digits
 */
void scoring_disableAllDigits(void) {
	_scoring_setEnableDigit(DIGIT_1, GPIO_PIN_RESET);
	_scoring_setEnableDigit(DIGIT_2, GPIO_PIN_RESET);
	_scoring_setEnableDigit(DIGIT_3, GPIO_PIN_RESET);
}

/**
 * @brief Convert an integer (from 0 to 9) in scoringDigitValue_t
 */

scoringDigitValue_t scoring_convertInteger(uint8_t integer) {
	if(integer > (uint8_t) DIGIT_VALUE_9) {
		assert_param(!"Wrong value");
		return DIGIT_VALUE_CLEAR;
	}

	return (scoringDigitValue_t) integer;
}

