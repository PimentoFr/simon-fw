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

#define REQUESTED_SEQUENCE_MAX_SIZE	(255)
#define AUTOPLAY_ITEM_SHOW_DURATION_MS	(300)	/* Time while note is played */
#define AUTOPLAY_ITEM_DELAY_MS			(200)	/* Time between two notes */

typedef enum {
	ENGINE_STARTUP = 0,
	ENGINE_GAME,				/* Engine main state which will handle the game process */
	ENGINE_MENU
} engineMachineStates_t;

typedef enum {
	GAME_ENTER = 0,
	GAME_GENERATOR, 			/* Generate the new level meaning create a new button sequence */
	GAME_AUTOPLAY,				/* Simon will play this sequence to present it to player */
	GAME_AUTOPLAY_WAIT_END_ITEM,			/* Wait until the item sequence is showing */
	GAME_AUTOPLAY_WAIT_NEXT_ITEM,			/* Wait the next item sequence */
	GAME_PLAY,					/* User will try to reproduce the requested sequence */
	GAME_LEVEL_SUCCESS,			/* User reproduce will success the requested sequence */
	GAME_LEVEL_FAILED			/* User fails to reproduce the requested sequence */
} engineGameMachineStates_t;

typedef struct {
	uint8_t requestedSequence[REQUESTED_SEQUENCE_MAX_SIZE];		/* Requested sequence */
	uint8_t level;												/* Current level */
} levelSettings_t;

typedef struct {
	uint8_t sequenceIndex;					/* Current sequence item */
	uint32_t lastPlaySequenceItemTimestamp;	/* Timestamp (Tick) when the current sequence item is played */
} autoplaySettings_t;

typedef struct {
	engineMachineStates_t currentState;
	union {
		engineGameMachineStates_t game;					/* Current sub state for the game process */
	} currentSubState;

	levelSettings_t levelSettings;						/* Current level settings*/
	autoplaySettings_t autoplaySettings;				/* Autoplay settings */
} engineSettings_t;




static engineSettings_t _engineSettings = {0};

static void _engine_stateMachine(void);
static status_t _engine_goToState(engineMachineStates_t state);


static void _engineGame_compute(void);
static status_t _engineGame_goToState(engineGameMachineStates_t gameState);
static status_t _engineGame_enter(void);
static status_t _engineGame_generateLevel(void);
static status_t _engineGame_autoplay(void);
static status_t _engineGame_autoplayWaitNextItem(void);
static status_t _engineGame_autoplayWaitEndItem(void);

static void _engine_menuCompute(void);


void engine_init(void) {

	_engineSettings.currentState = ENGINE_STARTUP;

	button_init();

	return;
}

void engine_compute(void) {

	while(1) {
		/* Compute button controller and update button events */
		button_compute();
		/* Compute the engine state machine */
		_engine_stateMachine();
	}
}

static void _engine_stateMachine(void) {
	switch(_engineSettings.currentState) {
	case ENGINE_STARTUP:
		_engine_goToState(ENGINE_GAME);
		_engineGame_goToState(GAME_ENTER);
		break;
	case ENGINE_GAME:
		_engineGame_compute();
		break;
	case ENGINE_MENU:
		_engine_menuCompute();
		break;
	default:
		assert_param(!"Bad state");
		return;

	}
}

/**
 * @brief Change the current state of the engine state machine
 * @param state New state of the engine state machine

 */
static status_t _engine_goToState(engineMachineStates_t state) {
	_engineSettings.currentState = state;
	return STATUS_OK;
}

/**
 * ===============================================================
 * 				GAME CONTROLLER
 * ===============================================================
 */

/**
 * @brief Change the substate of the game process
 */
static status_t _engineGame_goToState(engineGameMachineStates_t gameState) {
	if(_engineSettings.currentState != ENGINE_GAME) {
		assert_param(!"Bad state");
		return STATUS_FAIL;
	}

	_engineSettings.currentSubState.game = gameState;

	return STATUS_OK;
}

static void _engineGame_compute(void) {
	switch(_engineSettings.currentSubState.game) {
	case GAME_ENTER:
		if(_engineGame_enter() != STATUS_OK) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_GENERATOR:
		if(_engineGame_generateLevel() != STATUS_OK) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_AUTOPLAY:
		if(_engineGame_autoplay() != STATUS_OK) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_AUTOPLAY_WAIT_END_ITEM:
		if(_engineGame_autoplayWaitEndItem() != STATUS_OK) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_AUTOPLAY_WAIT_NEXT_ITEM:
		if(_engineGame_autoplayWaitNextItem() != STATUS_OK) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_PLAY:
		HAL_Delay(5000);
		if(_engineGame_goToState(GAME_ENTER) != STATUS_FAIL) {
			assert_param(!"Bad value");
			return;
		}
		break;
	case GAME_LEVEL_SUCCESS:
		break;
	case GAME_LEVEL_FAILED:
		break;
	default:
		break;
	}


	if(button_getCurrentState(BUTTON_CONF_MENU) == BUTTON_SHORT_PRESS_AND_RELEASED) {
		_engine_goToState(ENGINE_MENU);
	}
}

static status_t _engineGame_enter(void) {

	/* Set default level */
	_engineSettings.levelSettings.level = 10;

	if(_engineGame_goToState(GAME_GENERATOR) != STATUS_OK) {
		return STATUS_FAIL;
	}
	return STATUS_OK;
}

static status_t _engineGame_generateLevel(void) {

	for(uint8_t sequenceIndex = 0; sequenceIndex < _engineSettings.levelSettings.level; sequenceIndex++) {
		uint32_t randomValue = pseudoRandomGenerator_getValue();
		_engineSettings.levelSettings.requestedSequence[sequenceIndex] = randomValue % 4;
	}

	/* Re-initialize autoplaySettings structure */
	_engineSettings.autoplaySettings.sequenceIndex = 0;
	_engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp = HAL_GetTick();

	if(_engineGame_goToState(GAME_AUTOPLAY) != STATUS_OK) {
		return STATUS_FAIL;
	}

	return STATUS_OK;
}

static status_t _engineGame_autoplay(void) {

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

	return _engineGame_goToState(GAME_AUTOPLAY_WAIT_END_ITEM);
}

static status_t _engineGame_autoplayWaitEndItem(void) {

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
		if(_engineGame_goToState(GAME_AUTOPLAY_WAIT_NEXT_ITEM) != STATUS_OK) {
			return STATUS_FAIL;
		}
	}

	return STATUS_OK;
}

static status_t _engineGame_autoplayWaitNextItem(void) {
	uint32_t timeElapsed = HAL_GetTick() - _engineSettings.autoplaySettings.lastPlaySequenceItemTimestamp;
	if(timeElapsed >= (AUTOPLAY_ITEM_SHOW_DURATION_MS + AUTOPLAY_ITEM_DELAY_MS)) {

		/* Prepare the next item to show */
		_engineSettings.autoplaySettings.sequenceIndex ++;

		/* If there is no more item to show */
		if(_engineSettings.autoplaySettings.sequenceIndex >= _engineSettings.levelSettings.level ) {
			if(_engineGame_goToState(GAME_PLAY) != STATUS_OK) {
				return STATUS_FAIL;
			}
		} else {
			if(_engineGame_goToState(GAME_AUTOPLAY) != STATUS_OK) {
				return STATUS_FAIL;
			}
		}
	}

	return STATUS_OK;
}

static void _engine_menuCompute(void) {
	_engine_goToState(ENGINE_GAME);
}

