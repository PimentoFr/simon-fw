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
#include "misc.h"

#define REQUESTED_SEQUENCE_MAX_SIZE	(255)
#define AUTOPLAY_ITEM_SHOW_DURATION_MS	(300)	/* Time while note is played */
#define AUTOPLAY_ITEM_DELAY_MS			(200)	/* Time between two notes */
#define PLAY_ITEM_SHOW_DURATION_MS		(300)   /* Time while item will be shown */
typedef enum {
	ITEM_RED = 0,
	ITEM_GREEN,
	ITEM_YELLOW,
	ITEM_BLUE,
	ITEM_EMPTY,
	ITEM_MAX_SIZE
} sequenceItem_t;

typedef struct {
	sequenceItem_t requestedSequence[REQUESTED_SEQUENCE_MAX_SIZE];		/* Requested sequence */
	uint8_t level;														/* Current level */
} levelSettings_t;

typedef struct {
	uint8_t sequenceIndex;					/* Current sequence item */
	uint32_t lastPlaySequenceItemTimestamp;	/* Timestamp (Tick) when the current sequence item is played */
} autoplaySettings_t;

typedef struct {
	uint8_t sequenceIndex;					/* Current sequence item to find */
	sequenceItem_t playerItemProposal;		/* Player item proposal */
	uint32_t lastPlaySequenceItemTimestamp;	/* Timestamp (Tick) when the current sequence item is played */
} playSettings_t;

typedef struct {
	levelSettings_t levelSettings;						/* Current level settings*/
	autoplaySettings_t autoplaySettings;				/* Autoplay settings */
	playSettings_t playSettings;						/* Play settings */
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
static status_t _engine_gameGeneratorAddItem(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplayInit(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplay(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplayWaitEndItem(stateMachineHandler_t * handler);
static status_t _engine_gameAutoplayWaitNextItem(stateMachineHandler_t * handler);
static status_t _engine_gamePlayInit(stateMachineHandler_t * handler);
static status_t _engine_gamePlay(stateMachineHandler_t * handler);
static status_t _engine_gamePlayStartShowItem(stateMachineHandler_t * handler);
static status_t _engine_gamePlayStopShowItem(stateMachineHandler_t * handler);
static status_t _engine_gamePlayCheckProposal(stateMachineHandler_t * handler);
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
	_engineSettings.levelSettings.level = 4;

	HAL_Delay(3000);
	if(stateMachine_goTo(handler, _engine_gameGenerator) != STATUS_OK) {
		return STATUS_FAIL;
	}

	return STATUS_OK;
}

static status_t _engine_gameGenerator(stateMachineHandler_t * handler) {

	for(uint8_t sequenceIndex = 0; sequenceIndex < _engineSettings.levelSettings.level; sequenceIndex++) {
		uint16_t randomValue = pseudoRandomGenerator_getValue();
		_engineSettings.levelSettings.requestedSequence[sequenceIndex] = (sequenceItem_t) randomValue % 4;
	}

	return stateMachine_goTo(handler, _engine_gameAutoplayInit);
}

static status_t _engine_gameGeneratorAddItem(stateMachineHandler_t * handler) {
	uint8_t sequenceIndex = _engineSettings.levelSettings.level - 1;
	uint16_t randomValue = pseudoRandomGenerator_getValue();
	_engineSettings.levelSettings.requestedSequence[sequenceIndex] = (sequenceItem_t) randomValue % 4;

	return stateMachine_goTo(handler, _engine_gameAutoplayInit);
}

static status_t _engine_gameAutoplayInit(stateMachineHandler_t * handler) {

	/* Re-initialize autoplaySettings structure */
	_engineSettings.autoplaySettings.sequenceIndex = 0;
	_engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp = HAL_GetTick();

	return stateMachine_goTo(handler, _engine_gameAutoplay);
}

static status_t _engine_gameAutoplay(stateMachineHandler_t * handler) {

	sequenceItem_t itemSequence = _engineSettings.levelSettings.requestedSequence[_engineSettings.autoplaySettings.sequenceIndex];

	if(itemSequence >= ITEM_EMPTY) {
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

	if(misc_getElapsedTimeFromTick(_engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp) >= AUTOPLAY_ITEM_SHOW_DURATION_MS ) {

		/* Stop showing item sequence */
		sequenceItem_t itemSequence = _engineSettings.levelSettings.requestedSequence[_engineSettings.autoplaySettings.sequenceIndex];
		if(itemSequence >= ITEM_EMPTY) {
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
			return stateMachine_goTo(handler, _engine_gamePlayInit);
		} else {
			return stateMachine_goTo(handler, _engine_gameAutoplay);
		}
	}
	return STATUS_OK;
}

static status_t _engine_gamePlayInit(stateMachineHandler_t * handler) {

	_engineSettings.playSettings.sequenceIndex = 0;

	return stateMachine_goTo(handler, _engine_gamePlay);
}

static status_t _engine_gamePlay(stateMachineHandler_t * handler) {

	_engineSettings.playSettings.playerItemProposal = ITEM_EMPTY;

	if(button_getCurrentState(BUTTON_RED) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		_engineSettings.playSettings.playerItemProposal = ITEM_RED;
	} else if(button_getCurrentState(BUTTON_GREEN) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		_engineSettings.playSettings.playerItemProposal = ITEM_GREEN;
	} else if(button_getCurrentState(BUTTON_YELLOW) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		_engineSettings.playSettings.playerItemProposal = ITEM_YELLOW;
	} else if(button_getCurrentState(BUTTON_BLUE) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		_engineSettings.playSettings.playerItemProposal = ITEM_BLUE;
	}

	/* Player pressed a color button */
	if(_engineSettings.playSettings.playerItemProposal != ITEM_EMPTY) {
		return stateMachine_goTo(handler, _engine_gamePlayStartShowItem);
	}

	return STATUS_OK;
}

static status_t _engine_gamePlayStartShowItem(stateMachineHandler_t * handler) {

	led_disableAll();

	if(_engineSettings.playSettings.playerItemProposal >= ITEM_EMPTY) {
		assert_param(!"Out range");
		return STATUS_FAIL;
	}

	/* Show the selected led */
	led_enable((ledColor_t) _engineSettings.playSettings.playerItemProposal);
	//TODO: Play tone

	/* Set the new start item showing timestamp */
	_engineSettings.playSettings.lastPlaySequenceItemTimestamp = HAL_GetTick();
	return stateMachine_goTo(handler, _engine_gamePlayStopShowItem);
}

static status_t _engine_gamePlayStopShowItem(stateMachineHandler_t * handler) {
	if(misc_getElapsedTimeFromTick(_engineSettings.playSettings.lastPlaySequenceItemTimestamp) >= PLAY_ITEM_SHOW_DURATION_MS) {
		if(_engineSettings.playSettings.playerItemProposal >= ITEM_EMPTY) {
			assert_param(!"Out range");
			return STATUS_FAIL;
		}

		/* Show the selected led */
		led_disable((ledColor_t) _engineSettings.playSettings.playerItemProposal);
		//TODO: Stop tone

		return stateMachine_goTo(handler, _engine_gamePlayCheckProposal);
	}
	return STATUS_OK;
}

static status_t _engine_gamePlayCheckProposal(stateMachineHandler_t * handler) {
	sequenceItem_t itemToFound = _engineSettings.levelSettings.requestedSequence[_engineSettings.playSettings.sequenceIndex];

	if(itemToFound == _engineSettings.playSettings.playerItemProposal) {
		_engineSettings.playSettings.sequenceIndex ++;

		/* Check if the level is finished*/
		if(_engineSettings.playSettings.sequenceIndex >= _engineSettings.levelSettings.level) {
			return stateMachine_goTo(handler, _engine_gameLevelSuccess);
		} else {
			return stateMachine_goTo(handler, _engine_gamePlay);
		}
	} else {
		/* Player fails the level */
		return stateMachine_goTo(handler, _engine_gameLevelFailed);
	}
	return STATUS_OK;
}

static status_t _engine_gameLevelSuccess(stateMachineHandler_t * handler) {

	if(_engineSettings.levelSettings.level != UINT8_MAX) {
		_engineSettings.levelSettings.level ++;
	}

	HAL_Delay(500);

	return stateMachine_goTo(handler, _engine_gameGeneratorAddItem);
}


static status_t _engine_gameLevelFailed(stateMachineHandler_t * handler) {

	led_disableAll();

	for(uint8_t i = 0; i < 20; i++) {
		led_enable(LED_RED);
		HAL_Delay(100);
		led_disable(LED_RED);
		HAL_Delay(100);
	}

	return stateMachine_goTo(handler, _engine_gameEnter);
}


/**
 * ====================================================
 *     MENU EVALUATED CALLBACKS
 * ====================================================
 */

static status_t _engine_menuEnter(stateMachineHandler_t * handler) {

	return stateMachine_goTo(handler, _engine_gameEnter);
}


