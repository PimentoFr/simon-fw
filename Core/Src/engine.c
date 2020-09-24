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
#include "pseudo_random_generator.h"
#include "statemachine.h"

#define REQUESTED_SEQUENCE_MAX_SIZE	(255)
#define AUTOPLAY_ITEM_SHOW_DURATION_MS	(300)	/* Time while note is played */
#define AUTOPLAY_ITEM_DELAY_MS			(200)	/* Time between two notes */

typedef struct {
	uint8_t requestedSequence[REQUESTED_SEQUENCE_MAX_SIZE];		/* Requested sequence */
	uint8_t level;												/* Current level */
} levelSettings_t;

typedef struct {
	uint8_t sequenceIndex;					/* Current sequence item */
	uint32_t lastPlaySequenceItemTimestamp;	/* Timestamp (Tick) when the current sequence item is played */
} autoplaySettings_t;

typedef struct {
	levelSettings_t levelSettings;						/* Current level settings*/
	autoplaySettings_t autoplaySettings;				/* Autoplay settings */
} engineSettings_t;

static engineSettings_t _engineSettings = {0};
static stateMachineHandler_t _engineSM = {0};


/* ================================
 *  State machine evaluation callbacks
 *  =============================== */
/* Start up */
static status_t _engine_startUp(stateMachineHandler_t * handler);
/* Game */
static status_t _engine_gameEnter(stateMachineHandler_t * handler);
static status_t _engine_gameGenerator(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplay(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplayWaitEndItem(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplayWaitNextItem(stateMachineHandler_t * handler);
static status_t _engine_gamePlay(stateMachineHandler_t * handler);
static status_t _engine_gameLevelSuccess(stateMachineHandler_t * handler);
static status_t _engine_gameLevelFailed(stateMachineHandler_t * handler);
/* Menu */
static status_t _engine_menuEnter(stateMachineHandler_t * handler);


status_t engine_init(void) {


	button_init();

	if(stateMachine_init(&_engineSM, _engine_startUp ) != STATUS_OK) {
		assert_param(!"Can not init stateMachine");
		return STATUS_FAIL;
	}

	return STATUS_OK;
}

void engine_compute(void) {

	while(1) {
		/* Compute button controller and update button events */
		button_compute();
		/* Compute the engine state machine */
		stateMachine_compute(&_engineSM);
	}
}

static status_t _engine_startUp(stateMachineHandler_t * handler) {

	_engineSettings.autoplaySettings.sequenceIndex = 0;
	_engineSettings.levelSettings.level = 0;

	return stateMachine_goTo(handler, _engine_gameEnter);
}

/**
 * ==============================================
 * 		GAME EVALUATED CALLBACKS
 * ==============================================
 */

static status_t _engine_gameEnter(stateMachineHandler_t * handler) {

	/* Set default level */
	_engineSettings.levelSettings.level = 10;

	if(stateMachine_goTo(handler, _engine_gameGenerator) != STATUS_OK) {
		return STATUS_FAIL;
	}

	return STATUS_OK;
}

static status_t _engine_gameGenerator(stateMachineHandler_t * handler) {

	for(uint8_t sequenceIndex = 0; sequenceIndex < _engineSettings.levelSettings.level; sequenceIndex++) {
		uint32_t randomValue = pseudoRandomGenerator_getValue();
		_engineSettings.levelSettings.requestedSequence[sequenceIndex] = randomValue % 4;
	}

	/* Re-initialize autoplaySettings structure */
	_engineSettings.autoplaySettings.sequenceIndex = 0;
	_engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp = HAL_GetTick();

	return stateMachine_goTo(handler, _engine_gameAutoplay);
}

static status_t _engine_gameAutoplay(stateMachineHandler_t * handler) {

	uint8_t itemSequence = _engineSettings.levelSettings.requestedSequence[_engineSettings.autoplaySettings.sequenceIndex];

	if(itemSequence >= LED_MAX_SIZE) {
		assert_param(!"Out of range");
		return STATUS_FAIL;
	}

	led_disableAll();

	/* Show the sequence item */
	led_enable((ledColor_t) itemSequence);
	//TODO: Play the tone

	/* Update the new timestamp */
	_engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp = HAL_GetTick();

	return stateMachine_goTo(handler, _engine_gameAutoplayWaitEndItem);
}

static status_t _engine_gameAutoplayWaitEndItem(stateMachineHandler_t * handler) {

	uint32_t timeElapsed = HAL_GetTick() - _engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp;
	if(timeElapsed >= AUTOPLAY_ITEM_SHOW_DURATION_MS ) {

		/* Stop showing item sequence */
		uint8_t itemSequence = _engineSettings.levelSettings.requestedSequence[_engineSettings.autoplaySettings.sequenceIndex];
		if(itemSequence >= LED_MAX_SIZE) {
			assert_param(!"Out of range");
			return STATUS_FAIL;
		}
		led_disable((ledColor_t) itemSequence);

		/* Wait the next item */
		return stateMachine_goTo(handler, _engine_gameAutoplayWaitNextItem);
	}
	return STATUS_OK;
}

static status_t _engine_gameAutoplayWaitNextItem(stateMachineHandler_t * handler) {
	uint32_t timeElapsed = HAL_GetTick() - _engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp;
	if(timeElapsed >= (AUTOPLAY_ITEM_SHOW_DURATION_MS + AUTOPLAY_ITEM_DELAY_MS)) {

		/* Prepare the next item to show */
		_engineSettings.autoplaySettings.sequenceIndex ++;

		/* If there is no more item to show */
		if(_engineSettings.autoplaySettings.sequenceIndex >= _engineSettings.levelSettings.level ) {
			return stateMachine_goTo(handler, _engine_gamePlay);
		} else {
			return stateMachine_goTo(handler, _engine_gameAutoplay);
		}
	}
	return STATUS_OK;
}

static status_t _engine_gamePlay(stateMachineHandler_t * handler) {

	HAL_Delay(5000);
	return stateMachine_goTo(handler, _engine_gameEnter);
}

static status_t _engine_gameLevelSuccess(stateMachineHandler_t * handler) {

	return STATUS_OK;
}


static status_t _engine_gameLevelFailed(stateMachineHandler_t * handler) {

	return STATUS_OK;
}


/**
 * ====================================================
 *     MENU EVALUATED CALLBACKS
 * ====================================================
 */

static status_t _engine_menuEnter(stateMachineHandler_t * handler) {

	return stateMachine_goTo(handler, _engine_gameEnter);
}


