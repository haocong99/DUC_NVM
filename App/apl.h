#ifndef _APL_STT_H_
#define _APL_STT_H_
#include "stdint.h"
#include "bsp.h"
/* Signals of the application*/
typedef enum{
    INC=0,
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
		MENU,
		MENU_START,
		MENU_SETTING,
		//MENU_EXIT,
    SETTING_1,
    SETTING_2,
		SETTING_3,
		SETTING_4,
    STARTING,
//		A,
//		B,
    OUTCOME,
}apl_state_t;

/*chon ben giao bong*/
typedef enum{
	team_A_1=2,
	team_A_2,
	team_B_1,
	team_B_2
}serve_t;
/*config setting*/
typedef struct{
	uint8_t sound;
	uint8_t mode;
	uint8_t max_point;
	uint8_t max_sec;
	int8_t select_setting;
}setting_t;
/* Main application structure */
typedef struct {
    uint8_t mode;
//    uint8_t point_num;
    int8_t A_point;
    int8_t B_point;	
		uint8_t A_sec;
		uint8_t B_sec;
		int8_t select;
		serve_t serve; // bên giao bong
		setting_t setting;
    apl_state_t active_state;
}apl_status_t;




typedef enum{
    EVENT_HANDLED,  // Event handler s? ki?n du?c x? lý
    EVENT_IGNORED,  // Event Ignore s? ki?n b? b? qua
    EVENT_TRANSITION  // Event transition s? ki?n du?c chuy?n ti?p sang s? ki?n khác
}event_status_t;


void APL_init(void);
void OSL_din_callback(const BSP_DIN_STATUS_t *p_din);
void APL_din_callback(user_event_t const *const e);
void apl_stt_init(apl_status_t *mobj);
event_status_t apl_state_machine(apl_status_t *const mobj, event_t const *const e);
void APL_event_dispatcher(apl_status_t *const mobj,event_t const *const e);

void APL_Manager(void);

#endif
