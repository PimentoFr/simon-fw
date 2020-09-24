/*
 * tone.h
 *
 *  Created on: Sep 24, 2020
 *      Author: vince
 */

#ifndef INC_TONE_H_
#define INC_TONE_H_

typedef enum {
	TONE_DO = 0,
	TONE_RE,
	TONE_FA,
	TONE_LA,
	TONE_NONE,
	TONE_MAX_SIZE
} tone_t;

status_t tone_init(void);
status_t tone_stop(void);
status_t tone_start(tone_t tone);

#endif /* INC_TONE_H_ */
