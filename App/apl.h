#ifndef _APL_STT_H_
#define _APL_STT_H_
#include "stdint.h"
#include "bsp.h"
/* Signals of the application*/
typedef enum{
    INC,
    DEC,
    START,
    START_HOLD,
    SELECT,
	  REMOTE_INC,
		REMOTE_DEC,
		REMOTE_X,
		REMOTE_Y,
/* Internal activity signals */
    ENTRY,
    EXIT
}apl_signal_t;

/* Various States of the application*/
typedef enum{
    IDLE,
    SETTING_1,
    SETTING_2,
    STARTING,
		A,
		B,
    OUTCOME
}apl_state_t;

/* Main application structure */
typedef struct {
    uint8_t mode;
    uint8_t point_num;
    uint8_t A_point;
    uint8_t B_point;	
    apl_state_t active_state;
}apl_status_t;

/*Generic event structure */
typedef struct{
    uint8_t sig;
}event_t;

/* For user generated events */
typedef struct{
    event_t super;
}user_event_t;

typedef enum{
    EVENT_HANDLED,  // Event handler s? ki?n du?c x? lý
    EVENT_IGNORED,  // Event Ignore s? ki?n b? b? qua
    EVENT_TRANSITION  // Event transition s? ki?n du?c chuy?n ti?p sang s? ki?n khác
}event_status_t;


void APL_init(void);
void OSL_din_callback(const BSP_DIN_STATUS_t *p_din);
void APL_din_callback(void);
void apl_stt_init(apl_status_t *mobj);
event_status_t apl_state_machine(apl_status_t *const mobj, event_t const *const e);
void APL_event_dispatcher(apl_status_t *const mobj,event_t const *const e);
const user_event_t *APL_event_get(void);
void APL_Manager(void);

#endif
