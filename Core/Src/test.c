/*
 * test.c
 *
 *  Created on: 10 sept. 2020
 *      Author: vince
 */


#include "main.h"
#include "test.h"
#include "button.h"
#include "led.h"
#include "scoring.h"


#define TEST_7SEG_INCREMENT_DELAY_DEFAULT_MS	(500)
#define TEST_7SEG_COUNTER_MAX_VALUE				(1000)
#define TEST_7SEG_DELAY_UPDATE_DISPLAY_MS		(1)

typedef struct {
	uint32_t lastTickIncrementCounter;			/** Last tick in ms when the counter was incremented */
	uint32_t delayIncrement;					/** Delay in ms between two increments */
	uint16_t counterValue;						/** Counter value to show on 7-seg display */
	uint32_t lastTickUpdateDisplay;				/** Last tick in ms when the display was edited */
	scoringDigit_t currentDigitDisplayDriven;	/** Current Digit driven*/
} testHWSettings_t;

testHWSettings_t _testHWSettings = {0};

static void _test_computeButtonLed(void);
static void _test_compute7SegCounter(void);
static void _test_computeMenuButton(void);
/**
 * @brief Initialize the testHWSettings structure
 */
void test_initTestHW(void) {

	_testHWSettings.lastTickIncrementCounter = 0;
	_testHWSettings.delayIncrement = TEST_7SEG_INCREMENT_DELAY_DEFAULT_MS;
	_testHWSettings.counterValue = 0;
	_testHWSettings.lastTickUpdateDisplay = 0;
	_testHWSettings.currentDigitDisplayDriven = DIGIT_HUNDREDS;

}

/**
 * @brief Compute HW test
 * 		  Once pressed color button, the corresponding LED will be lighted.
 * 		  Otherwise, the LED will be off
 *
 *		  Moreover, a counter will be periodically incremented and shown on the 7 segment display.
 *		  CONF_MENU, CONF_UP, CONF_DOWN permit to reset counter value to 0, 333, 666 respectively
 *
 */
void test_computeTestHW(void) {

	while(1) {
		/* Test Colored leds/buttons */
		_test_computeButtonLed();
		/* Test 7 seg display with a counter */
		_test_compute7SegCounter();
		/* Test menu buttons */
		_test_computeMenuButton();
	}
}


/**
 * @brief Compute test HW for colored led and button.
 * 		  On press on a colored button, the corresponding LED will switch on.
 * 		  Otherwise, it will switch off
 */
static void _test_computeButtonLed(void) {
	/* BUTTON_RED */
	led_setEnable(LED_RED, (button_read(BUTTON_RED) == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	/* BUTTON_GREEN */
	led_setEnable(LED_GREEN, (button_read(BUTTON_GREEN) == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	/* BUTTON_YELLOW */
	led_setEnable(LED_YELLOW, (button_read(BUTTON_YELLOW) == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	/* BUTTON_BLUE */
	led_setEnable(LED_BLUE, (button_read(BUTTON_BLUE) == GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Compute test HW of 7 Seg display
 */
static void _test_compute7SegCounter(void) {
	uint32_t newTick = HAL_GetTick();
	uint8_t hundreds, tens, units;

	if((newTick - _testHWSettings.lastTickIncrementCounter) >= _testHWSettings.delayIncrement ) {
		_testHWSettings.lastTickIncrementCounter = newTick;
		_testHWSettings.counterValue ++;
		if(_testHWSettings.counterValue >= TEST_7SEG_COUNTER_MAX_VALUE) {
			_testHWSettings.counterValue = 0;
		}
	}

	if((newTick - _testHWSettings.lastTickUpdateDisplay) >= TEST_7SEG_DELAY_UPDATE_DISPLAY_MS) {
		_testHWSettings.lastTickUpdateDisplay = newTick;

		/* Change the current digit display which is driven */
		_testHWSettings.currentDigitDisplayDriven ++;
		if(_testHWSettings.currentDigitDisplayDriven >= DIGIT_MAX_SIZE) {
			_testHWSettings.currentDigitDisplayDriven = DIGIT_1;
		}

		/* Display the value of the 7-seg screen */
		uint16_t counterValue = _testHWSettings.counterValue;
		hundreds = counterValue / 100;
		counterValue -= hundreds * 100;
		tens = counterValue / 10;
		counterValue -= tens * 10;
		units = counterValue;

		/* Convert integer value into scoringDigitValue to be send to scoring methods */
		scoringDigitValue_t hundredsValue = scoring_convertInteger(hundreds);
		scoringDigitValue_t tensValue = scoring_convertInteger(tens);
		scoringDigitValue_t unitsValue = scoring_convertInteger(units);

		scoringDigitValue_t * valueToDisplay = NULL;

		switch(_testHWSettings.currentDigitDisplayDriven) {
		case DIGIT_1:
			valueToDisplay = &hundredsValue;
			break;
		case DIGIT_2:
			valueToDisplay = &tensValue;
			break;
		case DIGIT_3:
			valueToDisplay = &unitsValue;
			break;
		default:
			assert_param(!"Wrong value");
			return;
		}

		/* Display the digit on the driven 7-seg digit */
		scoring_displayDigit(_testHWSettings.currentDigitDisplayDriven, *valueToDisplay);
	}
}

/**
 * Compute test of menu buttons
 * On a press of CONF_MENU, the value of the counter will be set to 0
 * On a press of CONF_UP, the value of the counter will be set to 333
 * On a press of CONF_DOWN, the value of the counter will be set to 666
 */
static void _test_computeMenuButton(void) {

	if(button_read(BUTTON_CONF_MENU) == GPIO_PIN_SET) {
		_testHWSettings.counterValue = 0;
	}
	else if(button_read(BUTTON_CONF_DOWN) == GPIO_PIN_SET) {
		_testHWSettings.counterValue = 333;
	}
	else if(button_read(BUTTON_CONF_UP) == GPIO_PIN_SET) {
		_testHWSettings.counterValue = 666;
	}
}
