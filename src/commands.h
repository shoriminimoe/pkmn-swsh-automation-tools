#ifdef __cplusplus
extern "C" {
#endif

#ifndef _COMMANDS_H_
#define _COMMANDS_H_

typedef enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPIN,
	POSITION,
	X,
	Y,
	A,
	B,
	L,
	R,
	PLUS,
	MINUS,
	HOME,
	NOTHING,
	TRIGGERS
} Buttons_t;

typedef struct {
	Buttons_t button;
	uint16_t duration;
} command; 

typedef enum {
	SYNC_CONTROLLER,
	SYNC_POSITION,
	BREATHE,
	PROCESS,
	CLEANUP,
	DONE
} State_t;

static const command step[] = {
	{ NOTHING,  250 }
};

#endif

#ifdef __cplusplus
}
#endif
