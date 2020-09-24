/*
 * statemachine.c
 *
 *  Created on: 24 sept. 2020
 *      Author: vince
 */


#include "main.h"
#include "statemachine.h"

/**
 * @brief Initialize the state machine
 * @param handler Handler of the state machine
 * @param cb_init Callback for the initialization of the state machine
 * @return status_t
 */
status_t stateMachine_init(stateMachineHandler_t * handler, cb_eval cb_init)
{
	if(handler == NULL || cb_init == NULL) {
		return STATUS_FAIL;
	}

	handler->cb_init = cb_init;
	handler->cb_eval = cb_init;

	return STATUS_OK;
}

/**
 * @brief Compute the state machine from its handler
 * @param handler Handler of the state machine
 * @return status_t
 */
status_t stateMachine_compute(stateMachineHandler_t * handler) {

	if(handler == NULL || handler->cb_eval == NULL) {
		return STATUS_FAIL;
	}

	return handler->cb_eval(handler);
}

/**
 * @brief Change the evaluated callback by a new one
 * @param handler Handler of the state machine
 * @param cb_new_eval New callback to be evaluated
 * @return status_t
 */
status_t stateMachine_goTo(stateMachineHandler_t * handler, cb_eval cb_new_eval )
{
	if(handler == NULL || cb_new_eval == NULL) {
		return STATUS_FAIL;
	}

	handler->cb_eval = cb_new_eval;

	return STATUS_OK;
}

/**
 * @brief Reset the state machine
 * @param handler Handler of state machine
 * @return status_t
 */
status_t stateMachine_reset(stateMachineHandler_t * handler) {
	if(handler == NULL) {
		return STATUS_FAIL;
	}

	if(handler->cb_init != NULL) {
		handler->cb_eval = handler->cb_init;
	}

	return STATUS_OK;
}
