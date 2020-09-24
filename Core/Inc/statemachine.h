/*
 * statemachine.h
 *
 *  Created on: 24 sept. 2020
 *      Author: vince
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_


typedef struct stateMachineHandler_s stateMachineHandler_t;

typedef status_t (* cb_eval)(stateMachineHandler_t * handler);

struct stateMachineHandler_s
{
	cb_eval cb_init;		/* Init state  */
	cb_eval cb_eval;		/* Current state */
};

status_t stateMachine_init(stateMachineHandler_t * handler, cb_eval cb_init);
status_t stateMachine_compute(stateMachineHandler_t * handler);
status_t stateMachine_goTo(stateMachineHandler_t * handler, cb_eval cb_new_eval );
status_t stateMachine_reset(stateMachineHandler_t * handler);

#endif /* INC_STATEMACHINE_H_ */
