/*
 * scoring.h
 *
 *  Created on: Sep 8, 2020
 *      Author: vince
 */

#ifndef INC_SCORING_H_
#define INC_SCORING_H_

#define DIGIT_HUNDREDS	DIGIT_1
#define DIGIT_TENS		DIGIT_2
#define DIGIT_UNITS		DIGIT_3

/**
 * @brief Digit 7-Seg
 */
typedef enum {
	DIGIT_1 = 0, /** Left Digit */
	DIGIT_2, /** Center Digit */
	DIGIT_3, /** Right Digit */
	DIGIT_MAX_SIZE
} scoringDigit_t;

/**
 * @brief List of semgent of a 7-Seg display
 */
typedef enum {
	DIGIT_A = 0,
	DIGIT_B,
	DIGIT_C,
	DIGIT_D,
	DIGIT_E,
	DIGIT_F,
	DIGIT_G,
	DIGIT_SEG_MAX_SIZE
} scoringDigitSeg_t;

/**
 * @brief List of value available to display on 7-seg display
 */
typedef enum {
	DIGIT_VALUE_0 = 0,
	DIGIT_VALUE_1,
	DIGIT_VALUE_2,
	DIGIT_VALUE_3,
	DIGIT_VALUE_4,
	DIGIT_VALUE_5,
	DIGIT_VALUE_6,
	DIGIT_VALUE_7,
	DIGIT_VALUE_8,
	DIGIT_VALUE_9,
	DIGIT_VALUE_CLEAR,
	DIGIT_VALUE_MAX_SIZE	/** Length of the enum */
} scoringDigitValue_t;


void scoring_displayDigit(scoringDigit_t digit, scoringDigitValue_t value);
void scoring_disableAllDigits(void);
scoringDigitValue_t scoring_convertInteger(uint8_t integer);

#endif /* INC_SCORING_H_ */
