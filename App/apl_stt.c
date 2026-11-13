#include "apl.h"
#include "bsp.h"
#include "displayDriver.h"
#include "Times_New_Roman12x16.h"
#include "Times_New_Roman12x6_kytu.h"
#include "Times_New_Roman6x8.h"
#include "Terminal6x8.h"
#include "Times_New_Roman29x26.h"
#include "JQ8400_FL.h"
#include "flash.h"
//#include "RGB565/FCB_32x32.h"
//#include "RGB565/RMA.h"

typedef struct
{
	uint8_t version[10];
	uint8_t max_point;
	uint8_t mode;
	uint8_t sound;
	uint8_t max_sec;
}__attribute__((packed)) Information_t;
 
Information_t Infor_read;
Information_t Infor_write;

#define COLOR_B GREEN
#define COLOR_A RED
static char str[50];
extern UART_HandleTypeDef huart1;
extern JQ8400_Name MP3;
uint8_t max_point_t;
static event_status_t apl_state_handler_IDLE(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_MENU(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_MENU_START(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_MENU_SETTING(apl_status_t *const mobj, event_t const *const e);
//static event_status_t apl_state_handler_MENU_EXIT(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_1(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_2(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_3(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_4(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_STARTING(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_OUTCOME(apl_status_t *const mobj, event_t const *const e);
//static event_status_t apl_state_handler_A(apl_status_t *const mobj, event_t const *const e);
//static event_status_t apl_state_handler_B(apl_status_t *const mobj, event_t const *const e);



void apl_stt_init(apl_status_t *mobj)
{
	  event_t ee;
    ee.sig = ENTRY;
    mobj->active_state = IDLE;
    mobj->mode = 0;
//		mobj->point_num = 0;
		mobj->A_point=0;
		mobj->B_point=0;
		mobj->A_sec = 0;
		mobj->B_sec = 0;
		mobj->select =0;
		mobj->serve=0;
	 flash_read_arr(0x801FC00,(uint8_t *)&Infor_read,sizeof(Infor_read));
		mobj->setting.max_point=Infor_read.max_point;
		mobj->setting.mode=Infor_read.mode;
		mobj->setting.select_setting=0;
		mobj->setting.sound=Infor_read.sound;
		mobj->setting.max_sec=Infor_read.max_sec;
    apl_state_machine(mobj,&ee);
	
}

event_status_t apl_state_machine(apl_status_t *const mobj, event_t const *const e)
{
	switch(mobj->active_state)
	{
		case IDLE:
			return apl_state_handler_IDLE(mobj,e);
		case MENU: 
			return apl_state_handler_MENU(mobj,e);
		case MENU_START:
			return apl_state_handler_MENU_START(mobj,e);
		case MENU_SETTING:
			return apl_state_handler_MENU_SETTING(mobj,e);
//		case MENU_EXIT:
//			return apl_state_handler_MENU_EXIT(mobj,e);
		case SETTING_1:
			return apl_state_handler_SETTING_1(mobj,e);
		case SETTING_2:
			return apl_state_handler_SETTING_2(mobj,e);
		case SETTING_3:
			return apl_state_handler_SETTING_3(mobj,e);
		case SETTING_4:
			return apl_state_handler_SETTING_4(mobj,e);
		case STARTING:
			return apl_state_handler_STARTING(mobj,e);
//		case A:
//			return apl_state_handler_A(mobj,e);
//		case B:
//			return apl_state_handler_B(mobj,e);
		case OUTCOME:
			return apl_state_handler_OUTCOME(mobj,e);
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_IDLE(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
//			mobj->mode=0;
//			mobj->point_num=0;
			mobj->A_point=0;
			mobj->B_point=0;
		displayClear();		
    displayGoTo(3,13);
    displaySetFont(Terminal6x8_BIG);
    displayPrint("PICKELBALL", COLOR(MAGENTA));
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			displayClear();	
//			sprintf(str,"clr_exit_idle\n");
//			memset(str,0,50);
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case SELECT:{
			mobj->active_state = MENU; 
//			memset(str,0,50);
//			sprintf(str,"SELECT_IDLE\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_MENU(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
//			mobj->mode=0;
//			mobj->point_num=0;
//			mobj->A_point=0;
//			mobj->B_point=0;
		if(mobj->select == 0)
		{
			displayClear();		
			displayGoTo(0,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));				
		}
		else if (mobj->select == 1)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(0,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));	
		}
		else if (mobj->select == 2)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(0,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Exit", COLOR(WHITE));	
		}
		  flash_unlock();
			flash_erease(0x801FC00);
			flash_write_arr(0x801FC00,(uint8_t *)&Infor_write,sizeof(Infor_write));
			flash_lock();
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			displayClear();	
//			sprintf(str,"clr_exit_idle\n");
//			memset(str,0,50);
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case DEC:{
		mobj->select +=1;
		if(mobj->select >2)
		{
			mobj->select = 0;
		}	
		if(mobj->select == 0)
		{
			displayClear();		
			displayGoTo(0,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));				
		}
		else if (mobj->select == 1)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(0,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));	
		}
		else if (mobj->select == 2)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(0,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Exit", COLOR(WHITE));	
		}
	
			return EVENT_HANDLED;
		}
		case INC:{
		mobj->select -=1;
		if(mobj->select < 0)
		{
			mobj->select = 2;
		}	
		if(mobj->select == 0)
		{
			displayClear();		
			displayGoTo(0,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));				
		}
		else if (mobj->select == 1)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(0,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Setting", COLOR(WHITE));
			displayGoTo(5,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Exit", COLOR(WHITE));	
		}
		else if (mobj->select == 2)
		{
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Start", COLOR(WHITE));
			displayGoTo(5,12);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Setting", COLOR(WHITE));
			displayGoTo(0,22);
			displaySetFont(Terminal6x8_BIG);
			displayPrint(">Exit", COLOR(WHITE));	
		}
			return EVENT_HANDLED;
		}
		
		case SELECT:
		if(mobj->select == 0)
		{
			mobj->active_state = MENU_START;
			return EVENT_TRANSITION;
		}
		else if (mobj->select == 1)
		{
			mobj->active_state = MENU_SETTING;
			return EVENT_TRANSITION;
		}
		else if (mobj->select == 2)
		{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
			return EVENT_HANDLED;			
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_MENU_START(apl_status_t *const mobj, event_t const *const e)
{
		switch(e->sig)
	{
		// chon doi giao bong: doi A hoac doi B
		case ENTRY:{
			//disp_mode(mobj->mode)
			mobj->A_point=0;
			mobj->B_point=0;
			mobj->A_sec = 0;
			mobj->B_sec = 0;
			max_point_t = mobj->setting.max_point;
			if(mobj->setting.mode==1)
			{
				mobj->serve = team_A_2;
			}
			else if(mobj->setting.mode ==0)
			{
				mobj->serve = team_A_1;
			}
////			displayClear();		
////			displayGoTo(0,2);
////			displaySetFont(Terminal6x8_BIG);
////			displayPrint(">Start A", COLOR(WHITE));
////			displayGoTo(5,12);
////			displaySetFont(Terminal6x8_BIG);
////			displayPrint("Start B", COLOR(WHITE));	
////			displayGoTo(12,12);
////			displaySetFont(Terminal6x8_BIG);
////			displayPrint("Exit", COLOR(WHITE));			
////			return EVENT_HANDLED;
			mobj->active_state = STARTING;
			return EVENT_TRANSITION;
		}
		case EXIT:{
			//disp_clr();
//			displayClear();	
//			memset(str,0,50);
//			sprintf(str,"exit_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
//		case REMOTE_X:
//		{
//			switch(mobj->serve)
//			{
//				case team_B_2:
//					mobj->serve = team_A_2;
//					break;
//				case team_B_1:
//					mobj->serve = team_A_1;
//					break;
//				case team_A_1 :
//					mobj->serve = team_B_1;
//					break;
//				case team_A_2:
//					mobj->serve = team_B_2;
//					break;	
//				default:
//					break;
//			}
//			if((mobj->serve == team_A_2)|(mobj->serve == team_A_1)){
//				displayClear();		
//				displayGoTo(0,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start A", COLOR(WHITE));
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			if((mobj->serve == team_B_2)|(mobj->serve == team_B_1)){
//				// chon B giao bong
//				displayClear();		
//				displayGoTo(5,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start A", COLOR(WHITE));
//				displayGoTo(0,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case REMOTE_Y:
//		{
//			switch(mobj->serve)
//			{
//				case team_B_2:
//					mobj->serve = team_A_2;
//					break;
//				case team_B_1:
//					mobj->serve = team_A_1;
//					break;
//				case team_A_1 :
//					mobj->serve = team_B_1;
//					break;
//				case team_A_2:
//					mobj->serve = team_B_2;
//					break;	
//				default:
//					break;
//			}
//			if((mobj->serve == team_A_2)|(mobj->serve == team_A_1)){
//				displayClear();		
//				displayGoTo(0,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start A", COLOR(WHITE));
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			if((mobj->serve == team_B_2)|(mobj->serve == team_B_1)){
//				// chon B giao bong
//				displayClear();		
//				displayGoTo(5,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start A", COLOR(WHITE));
//				displayGoTo(0,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case INC:
//		{
//			switch(mobj->serve)
//			{
//				case team_B_2:
//					mobj->serve = team_A_2;
//					break;
//				case team_B_1:
//					mobj->serve = team_A_1;
//					break;
//				case team_A_1 :
//					mobj->serve = team_B_1;
//					break;
//				case team_A_2:
//					mobj->serve = team_B_2;
//					break;	
//				default:
//					break;
//			}
//			if((mobj->serve == team_A_2)|(mobj->serve == team_A_1)){
//				displayClear();		
//				displayGoTo(0,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start A", COLOR(WHITE));
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			if((mobj->serve == team_B_2)|(mobj->serve == team_B_1)){
//				// chon B giao bong
//				displayClear();		
//				displayGoTo(5,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start A", COLOR(WHITE));
//				displayGoTo(0,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case DEC:
//		{
//			switch(mobj->serve)
//			{
//				case team_B_2:
//					mobj->serve = team_A_2;
//					break;
//				case team_B_1:
//					mobj->serve = team_A_1;
//					break;
//				case team_A_1 :
//					mobj->serve = team_B_1;
//					break;
//				case team_A_2:
//					mobj->serve = team_B_2;
//					break;	
//				default:
//					break;
//			}
//			if((mobj->serve == team_A_2)|(mobj->serve == team_A_1)){
//				displayClear();		
//				displayGoTo(0,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start A", COLOR(WHITE));
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			if((mobj->serve == team_B_2)|(mobj->serve == team_B_1)){
//				// chon B giao bong
//				displayClear();		
//				displayGoTo(5,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Start A", COLOR(WHITE));
//				displayGoTo(0,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Start B", COLOR(WHITE));			
//				return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case SELECT:
//		{
//			//tran dau bat dau ben mobj->serve giao bong
//			mobj->active_state = STARTING;
//			return EVENT_TRANSITION;
//		}		
//		case START:
//		{
//			mobj->active_state = MENU;
//			return EVENT_TRANSITION;
//		}
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_MENU_SETTING(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:
		{
			if(mobj->setting.select_setting == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));				
			}
			else if (mobj->setting.select_setting == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));	
			}
			else if (mobj->setting.select_setting == 2)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(0,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Sec", COLOR(WHITE));	
			}		
			else if (mobj->setting.select_setting == 3)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Exit", COLOR(WHITE));	
			}		
			return EVENT_HANDLED;
		}
		case EXIT:
			return EVENT_HANDLED;
		case DEC:
		{
			mobj->setting.select_setting +=1;
			if(mobj->setting.select_setting >3)
			{
				mobj->setting.select_setting = 0;
			}	
			if(mobj->setting.select_setting == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));				
			}
			else if (mobj->setting.select_setting == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));	
			}
			else if (mobj->setting.select_setting == 2)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(0,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Sec", COLOR(WHITE));	
			}
			else if (mobj->setting.select_setting == 3)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Exit", COLOR(WHITE));	
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Exit", COLOR(WHITE));
			}
//			else if (mobj->setting.select_setting == 4)
//			{
//				displayClear();		
//				displayGoTo(5,2);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Max Sec", COLOR(WHITE));	
//				displayGoTo(0,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint(">Exit", COLOR(WHITE));
//			}
			return EVENT_HANDLED;
		}
		case INC:
		{
			mobj->setting.select_setting -=1;
			if(mobj->setting.select_setting < 0)
			{
				mobj->setting.select_setting = 3;
			}	
			if(mobj->setting.select_setting == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));				
			}
			else if (mobj->setting.select_setting == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Point", COLOR(WHITE));
				displayGoTo(5,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Sec", COLOR(WHITE));	
			}
			else if (mobj->setting.select_setting == 2)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Mode", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Max Point", COLOR(WHITE));
				displayGoTo(0,22);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Max Sec", COLOR(WHITE));	
			}
			else if (mobj->setting.select_setting == 3)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Exit", COLOR(WHITE));	
//				displayGoTo(5,12);
//				displaySetFont(Terminal6x8_BIG);
//				displayPrint("Exit", COLOR(WHITE));
			}	
			return EVENT_HANDLED;
		}
		case START:
		{
			mobj->active_state = MENU;
			return EVENT_TRANSITION;	
		}		
		case SELECT:
		{
			if(mobj->setting.select_setting == 0)
			{
				mobj->active_state = SETTING_2;
				return EVENT_TRANSITION;
			}
			else if (mobj->setting.select_setting == 1)
			{
				mobj->active_state = SETTING_3;
				return EVENT_TRANSITION;
			}
			else if (mobj->setting.select_setting == 2)
			{
				mobj->active_state = SETTING_4;
				return EVENT_TRANSITION;
			}
//			else if (mobj->setting.select_setting == 3)
//			{
//				mobj->active_state = SETTING_4;
//				return EVENT_TRANSITION;
//			}
			else if (mobj->setting.select_setting == 3)
			{
				mobj->active_state = MENU;
				return EVENT_TRANSITION;
			}
			return EVENT_HANDLED;		
		}
			
	return EVENT_IGNORED;
}
}

//static event_status_t apl_state_handler_MENU_EXIT(apl_status_t *const mobj, event_t const *const e)
//{
//	switch(e->sig)
//	{
//		case ENTRY:
//			return EVENT_HANDLED;
//		case EXIT:
//			return EVENT_HANDLED;
//		case SELECT:
//			mobj->active_state = IDLE;
//			return EVENT_TRANSITION;
//	}
//	return EVENT_IGNORED;
//}


static event_status_t apl_state_handler_SETTING_1(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:
		{
			if(mobj->setting.sound == 1)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">ON", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("OFF", COLOR(WHITE));	
			}
			else if(mobj->setting.sound == 0)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("ON", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">OFF", COLOR(WHITE));	
			}
			return EVENT_HANDLED;
		}
		case EXIT:{
			if(Infor_write.sound != mobj->setting.sound)Infor_write.sound = mobj->setting.sound;		
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->setting.sound = ! mobj->setting.sound;
//			mobj->setting.sound -=1;
//			if(mobj->setting.sound < 0)
//			{
//				mobj->setting.sound = 1;
//			}	
			if(mobj->setting.sound == 1)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">ON", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("OFF", COLOR(WHITE));	
			}
			else if(mobj->setting.sound == 0)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("ON", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">OFF", COLOR(WHITE));	
			}
			return EVENT_HANDLED;
		}
		case DEC:{
			mobj->setting.sound = ! mobj->setting.sound;
//			if(mobj->setting.sound > 1)
//			{
//				mobj->setting.sound = 0;
//			}	
			if(mobj->setting.sound == 1)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">ON", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("OFF", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			else if(mobj->setting.sound == 0)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("ON", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">OFF", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			return EVENT_HANDLED;
		}
		case SELECT:{
			mobj->active_state = MENU_SETTING;
//			memset(str,0,50);
//			sprintf(str,"SELECT_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case START:
		{
			mobj->active_state = MENU;
			return EVENT_TRANSITION;	
		}			
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_SETTING_3(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_point);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Point:", COLOR(WHITE));
			if(mobj->setting.max_point >9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));			
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"exit_setting_2\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			max_point_t = mobj->setting.max_point;
			if(Infor_write.max_point != mobj->setting.max_point)Infor_write.max_point = mobj->setting.max_point;
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->setting.max_point +=1;
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_point);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Point:", COLOR(WHITE));
			if(mobj->setting.max_point >9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));	
			return EVENT_HANDLED;
		}
		case DEC:{
			if(mobj->setting.max_point>=1)
			{
			mobj->setting.max_point -= 1;
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_point);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Point:", COLOR(WHITE));
			if(mobj->setting.max_point >9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));	
			return EVENT_HANDLED;
			}
			return EVENT_IGNORED;
		}
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_setting_2\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case START:{
			mobj->active_state = MENU;
			return EVENT_TRANSITION;
		}
		case SELECT:
		{
			mobj->active_state = MENU_SETTING;
			return EVENT_TRANSITION;
		}
			
	}
	return EVENT_IGNORED;	
}

static event_status_t apl_state_handler_SETTING_4(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_sec);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Sec:", COLOR(WHITE));
			if(mobj->setting.max_sec>9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));			
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"exit_setting_2\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			if(Infor_write.max_sec != mobj->setting.max_sec)Infor_write.max_sec = mobj->setting.max_sec;
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->setting.max_sec +=1;
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_sec);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Sec:", COLOR(WHITE));
			if(mobj->setting.max_sec >9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));	
			return EVENT_HANDLED;
		}
		case DEC:{
			if(mobj->setting.max_sec>=1)
			{
			mobj->setting.max_sec -= 1;
			memset(str,0,50);
			sprintf(str,"%d",mobj->setting.max_sec);
			displayClear();		
			displayGoTo(5,2);
			displaySetFont(Terminal6x8_BIG);
			displayPrint("Max Sec:", COLOR(WHITE));
			if(mobj->setting.max_sec >9)displayGoTo(20,10);
			else displayGoTo(25,10);
			displaySetFont(Times_New_Roman29x26_BIG);
			displayPrint(str, COLOR(WHITE));	
			return EVENT_HANDLED;
			}
			return EVENT_IGNORED;
		}
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_setting_2\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case START:{
			mobj->active_state = MENU;
			return EVENT_TRANSITION;
		}
		case SELECT:
		{
			mobj->active_state = MENU_SETTING;
			return EVENT_TRANSITION;
		}
	}
	return EVENT_IGNORED;	
}

static event_status_t apl_state_handler_SETTING_2(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:
		{
			if(mobj->setting.mode == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Single", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Double", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			else if(mobj->setting.mode == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Single", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Double", COLOR(WHITE));
				return EVENT_HANDLED;
			}
			return EVENT_HANDLED;
		}
		case EXIT:{
			if(Infor_write.mode != mobj->setting.mode)Infor_write.mode = mobj->setting.mode;
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->setting.mode = ! mobj->setting.mode;
			if(mobj->setting.mode == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Single", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Double", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			else if(mobj->setting.mode == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Single", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Double", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			return EVENT_HANDLED;
		}
		case DEC:{
			mobj->setting.mode = ! mobj->setting.mode;
			if(mobj->setting.mode == 0)
			{
				displayClear();		
				displayGoTo(0,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Single", COLOR(WHITE));
				displayGoTo(5,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Double", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			else if(mobj->setting.mode == 1)
			{
				displayClear();		
				displayGoTo(5,2);
				displaySetFont(Terminal6x8_BIG);
				displayPrint("Single", COLOR(WHITE));
				displayGoTo(0,12);
				displaySetFont(Terminal6x8_BIG);
				displayPrint(">Double", COLOR(WHITE));	
				return EVENT_HANDLED;
			}
			return EVENT_HANDLED;
		}
		case SELECT:{
			mobj->active_state = MENU_SETTING;
//			memset(str,0,50);
//			sprintf(str,"SELECT_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case START:
			mobj->active_state = MENU;
			return EVENT_TRANSITION;	
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
	}
	return EVENT_IGNORED;	
}


static event_status_t apl_state_handler_STARTING(apl_status_t *const mobj, event_t const *const e)
{
	char str_A[4]="";
	char str_B[4]="";
	char str_sec_A[4]="";
	char str_sec_B[4]="";

	switch(e->sig)
	{
		case ENTRY:{
			//disp_point(mobj->A_point)
			//disp_point(mobj->B_point)	
			memset(str_A,0,4);
			memset(str_B,0,4);
			memset(str_sec_A,0,4);
			memset(str_sec_B,0,4);

//			mobj->A_point=0;
//			mobj->B_point=0;
			if(mobj->A_point <10)
			{
				sprintf(str_A,"0%d",mobj->A_point);
			}
			else
			{
				sprintf(str_A,"%d",mobj->A_point);
			}
			if(mobj->B_point <10)
			{
				sprintf(str_B,"0%d",mobj->B_point);
			}
			else
			{
				sprintf(str_B,"%d",mobj->B_point);
			}	
			sprintf(str_sec_A,"%d",mobj->A_sec);
			sprintf(str_sec_B,"%d",mobj->B_sec);
			displayClear();		
			displayGoTo(5,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_A, COLOR(COLOR_A));
			displayGoTo(38,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_B, COLOR(COLOR_B));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));		
			displayGoTo(19,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_A, COLOR(WHITE));
			if(mobj->B_sec>9)displayGoTo(34,1);
			else displayGoTo(40,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_B, COLOR(WHITE));	
			if(mobj->setting.mode == 1)
			{
			if(mobj->serve == team_A_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_2);
				}
				
			}	
			else if(mobj->serve == team_A_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_1);
				}
			}	
			else if(mobj->serve == team_B_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_1-2);
				}
			}
			else if(mobj->serve == team_B_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_2-2);
				}
			}
			}
			else
			{
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
					}
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
					}				
				}
			}

			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"EXIT_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case REMOTE_X:
		{
			if(mobj->setting.mode==1)
			{
				if(mobj->serve == team_B_2)
				{
					mobj->serve = team_A_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				if(mobj->serve == team_A_1)
				{
					mobj->serve = team_A_2;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				if(mobj->serve == team_A_2)
				{
					mobj->serve = team_B_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;				
				}
				if(mobj->serve == team_B_1)
				{
					mobj->serve = team_B_2;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;				
				}
			}
			else {
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					mobj->serve = team_A_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					mobj->serve = team_B_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}
			
			return EVENT_IGNORED;
		}
		case REMOTE_Y:{
//			mobj->active_state = B;
//			memset(str,0,50);
//			sprintf(str,"REMOTE_Y_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case INC:
		{

			if((mobj->serve == team_A_1) ||(mobj->serve== team_A_2))
			{
				mobj->A_point +=1;
				if((mobj->A_point >=(max_point_t - 1))&&(mobj->A_point == mobj->B_point))
				{
					max_point_t +=1;
				}
				if(mobj->A_point > max_point_t)
				{
					mobj->A_sec +=1;
					max_point_t = mobj->setting.max_point;	
					mobj->serve = team_A_2	;				
					if(mobj->A_sec >= mobj->setting.max_sec)
					{
						mobj->active_state=OUTCOME;
						return EVENT_TRANSITION;
					}
					mobj->A_point=0;
					mobj->B_point=0;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			else if((mobj->serve == team_B_1) ||(mobj->serve== team_B_2))
			{
				mobj->B_point +=1;
				if((mobj->B_point >=(max_point_t - 1))&&(mobj->A_point == mobj->B_point))
				{
					max_point_t +=1;
				}
				if(mobj->B_point > max_point_t)
				{
					mobj->B_sec +=1;
					max_point_t = mobj->setting.max_point;
					mobj->serve = team_B_2;
					if(mobj->B_sec >= mobj->setting.max_sec)
					{
						mobj->active_state=OUTCOME;
						return EVENT_TRANSITION;
					}
					mobj->B_point=0;
					mobj->A_point=0;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			memset(str_A,0,4);
			memset(str_B,0,4);
			memset(str_sec_A,0,4);
			memset(str_sec_B,0,4);
			if(mobj->A_point <10)
			{
				sprintf(str_A,"0%d",mobj->A_point);
			}
			else
			{
				sprintf(str_A,"%d",mobj->A_point);
			}
			if(mobj->B_point <10)
			{
				sprintf(str_B,"0%d",mobj->B_point);
			}
			else
			{
				sprintf(str_B,"%d",mobj->B_point);
			}	
			sprintf(str_sec_A,"%d",mobj->A_sec);
			sprintf(str_sec_B,"%d",mobj->B_sec);
			displayClear();		
			displayGoTo(5,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_A, COLOR(COLOR_A));
			displayGoTo(38,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_B, COLOR(COLOR_B));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
			displayGoTo(19,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_A, COLOR(WHITE));
			if(mobj->B_sec>9)displayGoTo(34,1);
			else displayGoTo(40,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_B, COLOR(WHITE));
			if(mobj->setting.mode==1)
			{
			if(mobj->serve == team_A_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_2);
				}
				
			}	
			else if(mobj->serve == team_A_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_1);
				}
			}	
			else if(mobj->serve == team_B_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_1-2);
				}
			}
			else if(mobj->serve == team_B_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_2-2);
				}
			}		
			return EVENT_HANDLED;
		}
			else{
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
					}
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
					}				
				}
			return EVENT_HANDLED;				
			}
			return EVENT_IGNORED ;
		}
		case DEC:
		{
			if((mobj->serve == team_A_1) ||(mobj->serve== team_A_2))
			{
				if(mobj->A_point == max_point_t-2)
				{
					max_point_t -=1;
				}
				//if(mobj->A_point > 0)
				mobj->A_point -=1;

				if(mobj->A_point < 0)
				{
					mobj->A_point =0;
					if(mobj->A_sec >0)mobj->A_sec -=1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			if((mobj->serve == team_B_1) ||(mobj->serve== team_B_2))
			{
				if(mobj->A_point == max_point_t-2)
				{
					max_point_t -=1;
				}
				//if(mobj->B_point > 0)
				mobj->B_point -=1;
				if(mobj->B_point < 0)
				{
					mobj->B_point =0;
					if(mobj->B_sec >0)mobj->B_sec -=1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			memset(str_A,0,4);
			memset(str_B,0,4);
			memset(str_sec_A,0,4);
			memset(str_sec_B,0,4);
			if(mobj->A_point <10)
			{
				sprintf(str_A,"0%d",mobj->A_point);
			}
			else
			{
				sprintf(str_A,"%d",mobj->A_point);
			}
			if(mobj->B_point <10)
			{
				sprintf(str_B,"0%d",mobj->B_point);
			}
			else
			{
				sprintf(str_B,"%d",mobj->B_point);
			}	
			sprintf(str_sec_A,"%d",mobj->A_sec);
			sprintf(str_sec_B,"%d",mobj->B_sec);
			displayClear();		
			displayGoTo(5,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_A, COLOR(COLOR_A));
			displayGoTo(38,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_B, COLOR(COLOR_B));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
			displayGoTo(19,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_A, COLOR(WHITE));
			if(mobj->B_sec>9)displayGoTo(34,1);
			else displayGoTo(40,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_B, COLOR(WHITE));
			if(mobj->setting.mode==1)
			{
			if(mobj->serve == team_A_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_2);
				}
			}	
			else if(mobj->serve == team_A_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_1);
				}
			}	
			else if(mobj->serve == team_B_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_1-2);
				}
			}
			else if(mobj->serve == team_B_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_2-2);
				}
			}	
			return EVENT_HANDLED;
		}
			else{
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
					}
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
					}				
				}
				return EVENT_HANDLED;				
			}
			return EVENT_IGNORED;
		}
		case REMOTE_INC:
		{

			if((mobj->serve == team_A_1) ||(mobj->serve== team_A_2))
			{
				mobj->A_point +=1;
				if((mobj->A_point >=(max_point_t - 1))&&(mobj->A_point == mobj->B_point))
				{
					max_point_t +=1;
				}
				if(mobj->A_point > max_point_t)
				{
					mobj->A_sec +=1;
					max_point_t = mobj->setting.max_point;	
					mobj->serve = team_A_2	;				
					if(mobj->A_sec >= mobj->setting.max_sec)
					{
						mobj->active_state=OUTCOME;
						return EVENT_TRANSITION;
					}
					mobj->A_point=0;
					mobj->B_point=0;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			else if((mobj->serve == team_B_1) ||(mobj->serve== team_B_2))
			{
				mobj->B_point +=1;
				if((mobj->B_point >=(max_point_t - 1))&&(mobj->A_point == mobj->B_point))
				{
					max_point_t +=1;
				}
				if(mobj->B_point > max_point_t)
				{
					mobj->B_sec +=1;
					max_point_t = mobj->setting.max_point;
					mobj->serve = team_B_2;
					if(mobj->B_sec >= mobj->setting.max_sec)
					{
						mobj->active_state=OUTCOME;
						return EVENT_TRANSITION;
					}
					mobj->B_point=0;
					mobj->A_point=0;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			memset(str_A,0,4);
			memset(str_B,0,4);
			memset(str_sec_A,0,4);
			memset(str_sec_B,0,4);
			if(mobj->A_point <10)
			{
				sprintf(str_A,"0%d",mobj->A_point);
			}
			else
			{
				sprintf(str_A,"%d",mobj->A_point);
			}
			if(mobj->B_point <10)
			{
				sprintf(str_B,"0%d",mobj->B_point);
			}
			else
			{
				sprintf(str_B,"%d",mobj->B_point);
			}	
			sprintf(str_sec_A,"%d",mobj->A_sec);
			sprintf(str_sec_B,"%d",mobj->B_sec);
			displayClear();		
			displayGoTo(5,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_A, COLOR(COLOR_A));
			displayGoTo(38,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_B, COLOR(COLOR_B));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
			displayGoTo(19,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_A, COLOR(WHITE));
			if(mobj->B_sec>9)displayGoTo(34,1);
			else displayGoTo(40,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_B, COLOR(WHITE));
			if(mobj->setting.mode==1)
			{
			if(mobj->serve == team_A_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_2);
				}
				
			}	
			else if(mobj->serve == team_A_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_1);
				}
			}	
			else if(mobj->serve == team_B_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_1-2);
				}
			}
			else if(mobj->serve == team_B_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_2-2);
				}
			}		
			return EVENT_HANDLED;
		}
			else{
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
					}
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
					}				
				}
			return EVENT_HANDLED;				
			}
			return EVENT_IGNORED ;
		}
		case REMOTE_DEC:
		{
			if((mobj->serve == team_A_1) ||(mobj->serve== team_A_2))
			{
				if(mobj->A_point == max_point_t-2)
				{
					max_point_t -=1;
				}
				if(mobj->A_point > 0)mobj->A_point -=1;

				if(mobj->A_point == 0)
				{
					mobj->A_point =0;
					if(mobj->A_sec >0)mobj->A_sec -=1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			if((mobj->serve == team_B_1) ||(mobj->serve== team_B_2))
			{
				if(mobj->A_point == max_point_t-2)
				{
					max_point_t -=1;
				}
				if(mobj->B_point > 0)mobj->B_point -=1;
				if(mobj->B_point == 0)
				{
					mobj->B_point =0;
					if(mobj->B_sec >0)mobj->B_sec -=1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}	
			memset(str_A,0,4);
			memset(str_B,0,4);
			memset(str_sec_A,0,4);
			memset(str_sec_B,0,4);
			if(mobj->A_point <10)
			{
				sprintf(str_A,"0%d",mobj->A_point);
			}
			else
			{
				sprintf(str_A,"%d",mobj->A_point);
			}
			if(mobj->B_point <10)
			{
				sprintf(str_B,"0%d",mobj->B_point);
			}
			else
			{
				sprintf(str_B,"%d",mobj->B_point);
			}	
			sprintf(str_sec_A,"%d",mobj->A_sec);
			sprintf(str_sec_B,"%d",mobj->B_sec);
			displayClear();		
			displayGoTo(5,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_A, COLOR(COLOR_A));
			displayGoTo(38,14);
			displaySetFont(Times_New_Roman12x16_BIG);
			displayPrint(str_B, COLOR(COLOR_B));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
			displayGoTo(19,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_A, COLOR(WHITE));
			if(mobj->B_sec>9)displayGoTo(34,1);
			else displayGoTo(40,1);
			displaySetFont(Times_New_Roman6x8_BIG);
			displayPrint(str_sec_B, COLOR(WHITE));
			if(mobj->setting.mode==1)
			{
			if(mobj->serve == team_A_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_2);
				}
			}	
			else if(mobj->serve == team_A_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_A_1);
				}
			}	
			else if(mobj->serve == team_B_1)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(".,", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_1-2);
				}
			}
			else if(mobj->serve == team_B_2)
			{
				displayGoTo(5,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint(",,", COLOR(COLOR_A));	
				displayGoTo(44,6);
				displaySetFont(Times_New_Roman10x5_BIG);
				displayPrint("..", COLOR(COLOR_B));
				if(mobj->setting.sound==1)
				{
					JQ8400_PlaySong(&MP3,mobj->B_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,mobj->A_point+1);
					HAL_Delay(1000);
					JQ8400_PlaySong(&MP3,team_B_2-2);
				}
			}	
			return EVENT_HANDLED;
		}
			else{
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
					}
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					displayGoTo(6,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(".", COLOR(COLOR_A));	
					displayGoTo(50,6);
					displaySetFont(Times_New_Roman10x5_BIG);
					displayPrint(",,", COLOR(COLOR_B));
					if(mobj->setting.sound==1)
					{
						JQ8400_PlaySong(&MP3,mobj->A_point+1);
						HAL_Delay(1000);
						JQ8400_PlaySong(&MP3,mobj->B_point+1);
					}				
				}
				return EVENT_HANDLED;				
			}
			return EVENT_IGNORED;
		}
		case SELECT:
		{
			if(mobj->setting.mode==1)
			{
				if(mobj->serve == team_B_2)
				{
					mobj->serve = team_A_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				if(mobj->serve == team_A_1)
				{
					mobj->serve = team_A_2;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				if(mobj->serve == team_A_2)
				{
					mobj->serve = team_B_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;				
				}
				if(mobj->serve == team_B_1)
				{
					mobj->serve = team_B_2;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;				
				}
			}
			else {
				if((mobj->serve == team_B_2)||(mobj->serve == team_B_1))
				{
					mobj->serve = team_A_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
				else if((mobj->serve == team_A_2)||(mobj->serve == team_A_1))
				{
					mobj->serve = team_B_1;
					mobj->active_state = STARTING;
					return EVENT_TRANSITION;
				}
			}
			
			return EVENT_IGNORED;
		}
		case START_HOLD:{
			mobj->active_state = IDLE;
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"HOLD_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		
	}
		return EVENT_IGNORED;	
}

static event_status_t apl_state_handler_OUTCOME(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			if(mobj->A_point > mobj->B_point)
			{
				memset(str,0,50);
				sprintf(str,"A WIN !!!");
				displayClear();	
				displayGoTo(5,10);
				displaySetFont(DEFAULT_BIG);
				displayPrint( str, COLOR(MAGENTA));				
				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//				memset(str,0,50);
//				sprintf(str,"A:%d vs B:%d",mobj->A_point,mobj->B_point);
//				displayClear();	
//				displayGoTo(0,0);
//				displaySetFont(DEFAULT_BIG);
//				displayPrint( str, COLOR(MAGENTA));
//				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				return EVENT_HANDLED;
			}
			else if(mobj->A_point < mobj->B_point)
			{
				memset(str,0,50);
				sprintf(str,"B WIN !!!");
				displayClear();	
				displayGoTo(5,10);
				displaySetFont(DEFAULT_BIG);
				displayPrint( str, COLOR(MAGENTA));
				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//				memset(str,0,50);
//				sprintf(str,"A:%d vs B:%d",mobj->A_point,mobj->B_point);
//				displayClear();	
//				displayGoTo(0,0);
//				displaySetFont(DEFAULT_BIG);
//				displayPrint( str, COLOR(MAGENTA));
//				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				return EVENT_HANDLED;
			}
		}
		case EXIT:{
			return EVENT_HANDLED;
		}		
		case START_HOLD:{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
		case SELECT:{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
		case START:{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
		case REMOTE_X:{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
		case REMOTE_Y:{
			mobj->active_state = IDLE;
			return EVENT_TRANSITION;
		}
	}	
	return EVENT_IGNORED;		
}
//static event_status_t apl_state_handler_A(apl_status_t *const mobj, event_t const *const e)
//{
//	switch(e->sig)
//	{
//		case ENTRY:{
//			char str_A[4]="";
//			char str_B[4]="";
//			char str_sec_A[4]="";
//			char str_sec_B[4]="";
//			
//			memset(str_A,0,4);
//			memset(str_B,0,4);
//			memset(str_sec_A,0,4);
//			memset(str_sec_B,0,4);
//			if(mobj->A_point <10)
//			{
//				sprintf(str_A,"0%d",mobj->A_point);
//			}
//			else
//			{
//				sprintf(str_A,"%d",mobj->A_point);
//			}
//			if(mobj->B_point <10)
//			{
//				sprintf(str_B,"0%d",mobj->B_point);
//			}
//			else
//			{
//				sprintf(str_B,"%d",mobj->B_point);
//			}	
//			displayClear();		
//			displayGoTo(5,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_A, COLOR(RED));
//			displayGoTo(38,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_B, COLOR(BLUE));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
//			if(mobj->serve == team_A_2)
//			{
//				displayGoTo(5,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(RED));	
//				displayGoTo(44,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(BLUE));
//			}				
//			else if(mobj->serve == team_B_2)
//			{
//				displayGoTo(5,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(RED));	
//				displayGoTo(44,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(BLUE));
//			}	
//			displayGoTo(19,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_A, COLOR(WHITE));
//			displayGoTo(34,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_A, COLOR(WHITE));	

//			return EVENT_HANDLED;
//		}
//		case EXIT:{
//			//disp_clr();
////			memset(str,0,50);
////			sprintf(str,"EXIT_A\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_HANDLED;
//		}
//		case REMOTE_INC:{
//			char str_A[4]="";
//			char str_B[4]="";
//			char str_sec_A[4]="";
//			char str_sec_B[4]="";
//			
//			memset(str_A,0,4);
//			memset(str_B,0,4);
//			memset(str_sec_A,0,4);
//			memset(str_sec_B,0,4);
//			if(++mobj->A_point < mobj->setting.max_point)
//			{
////				memset(str,0,50);
//////				sprintf(str,"REMOTE_INC_A\n");
//////				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
////				sprintf(str,"A:%d vs B:%d",mobj->A_point,mobj->B_point);
////				displayClear();	
////				displayGoTo(0,0);
////				displaySetFont(DEFAULT_BIG);
////				displayPrint( str, COLOR(MAGENTA));

//				if(mobj->A_point <10)
//				{
//					sprintf(str_A,"0%d",mobj->A_point);
//				}
//				else
//				{
//					sprintf(str_A,"%d",mobj->A_point);
//				}
//				if(mobj->B_point <10)
//				{
//					sprintf(str_B,"0%d",mobj->B_point);
//				}
//				else
//				{
//					sprintf(str_B,"%d",mobj->B_point);
//				}					
//			}
//			else{
////				mobj->active_state = OUTCOME;
////				return EVENT_TRANSITION;
//				mobj->A_sec += 1;
//				mobj->A_point=0;
//				mobj->B_point=0;
//				if(mobj->A_sec >= mobj->setting.max_sec)
//				{
//					mobj->active_state = OUTCOME;
//					return EVENT_TRANSITION;
//				}
//				return EVENT_HANDLED;
//			}
//			displayClear();		
//			displayGoTo(5,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_A, COLOR(RED));
//			displayGoTo(38,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_B, COLOR(BLUE));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
//			if(mobj->serve == team_A_2)
//			{
//				displayGoTo(5,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(RED));	
//				displayGoTo(44,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(BLUE));
//			}				
//			else if(mobj->serve == team_B_2)
//			{
//				displayGoTo(5,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(RED));	
//				displayGoTo(44,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(BLUE));
//			}	
//			displayGoTo(19,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_A, COLOR(WHITE));
//			displayGoTo(34,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_B, COLOR(WHITE));	
//				JQ8400_PlaySong(&MP3,mobj->A_point);
//				JQ8400_PlaySong(&MP3,mobj->B_point+10);
////				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//				return EVENT_HANDLED;			
//		}
//		case REMOTE_DEC:{
//			if(mobj->A_point >0)
//			{
//			mobj->A_point -=1;
//			char str_A[4]="";
//			char str_B[4]="";
//			char str_sec_A[4]="";
//			char str_sec_B[4]="";
//			
//			memset(str_A,0,4);
//			memset(str_B,0,4);
//			memset(str_sec_A,0,4);
//			memset(str_sec_B,0,4);
//			if(mobj->A_point <10)
//			{
//				sprintf(str_A,"0%d",mobj->A_point);
//			}
//			else
//			{
//				sprintf(str_A,"%d",mobj->A_point);
//			}
//			if(mobj->B_point <10)
//			{
//				sprintf(str_B,"0%d",mobj->B_point);
//			}
//			else
//			{
//				sprintf(str_B,"%d",mobj->B_point);
//			}		
//			displayClear();		
//			displayGoTo(5,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_A, COLOR(RED));
//			displayGoTo(38,14);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_B, COLOR(BLUE));
//			displayGoTo(27,21);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));
//			if(mobj->serve == team_A_2)
//			{
//				displayGoTo(5,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(RED));	
//				displayGoTo(44,5);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(BLUE));
//			}				
//			else if(mobj->serve == team_B_2)
//			{
//				displayGoTo(5,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(",,", COLOR(RED));	
//				displayGoTo(44,6);
//				displaySetFont(Times_New_Roman10x5_BIG);
//				displayPrint(".", COLOR(BLUE));
//			}	
//			displayGoTo(19,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_A, COLOR(WHITE));
//			displayGoTo(34,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint(str_sec_A, COLOR(WHITE));	
//				JQ8400_PlaySong(&MP3,mobj->A_point);
//				JQ8400_PlaySong(&MP3,mobj->B_point+10);			
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case REMOTE_X:{
//			mobj->active_state = A;
////			memset(str,0,50);
////			sprintf(str,"REMOTE_X_A\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_TRANSITION;
//		}
//		case REMOTE_Y:{
//			mobj->active_state = B;
//			return EVENT_TRANSITION;
//		}
//		case START_HOLD:{
//			mobj->active_state = IDLE;
////			memset(str,0,50);
////			sprintf(str,"HOLD_A\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_TRANSITION;
//	}
//}
//	return EVENT_IGNORED;	
//}

//static event_status_t apl_state_handler_B(apl_status_t *const mobj, event_t const *const e)
//{
//	switch(e->sig)
//	{
//		case ENTRY:{
//			//disp_point(Apoint,Bpoint)
////			memset(str,0,50);
////			sprintf(str,"ENTRY_B\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_HANDLED;
//		}
//		case EXIT:{
//			//disp_clr();
////			memset(str,0,50);
////			sprintf(str,"EXIT_B\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_HANDLED;
//		}
//		case REMOTE_INC:{
//			if(++mobj->B_point<mobj->setting.max_point)
//			{
////				memset(str,0,50);
////				sprintf(str,"REMOTE_INC_B\n");
////				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			char str_A[4]="";
//			char str_B[4]="";
//			char str_sec_A[4]="";
//			char str_sec_B[4]="";
//			
//			memset(str_A,0,4);
//			memset(str_B,0,4);
//			memset(str_sec_A,0,4);
//			memset(str_sec_B,0,4);
//			if(mobj->A_point <10)
//			{
//				sprintf(str_A,"0%d",mobj->A_point);
//			}
//			else
//			{
//				sprintf(str_A,"%d",mobj->A_point);
//			}
//			if(mobj->B_point <10)
//			{
//				sprintf(str_B,"0%d",mobj->B_point);
//			}
//			else
//			{
//				sprintf(str_B,"%d",mobj->B_point);
//			}		
//			displayClear();		
//			displayGoTo(5,5);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_A, COLOR(RED));
//			displayGoTo(38,5);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_B, COLOR(BLUE));
//			displayGoTo(27,11);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));	
//			displayGoTo(8,24);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("..", COLOR(RED));		
//			displayGoTo(41,24);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("..", COLOR(BLUE));
//			displayGoTo(26,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint("0", COLOR(WHITE));
//			displayGoTo(33,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint("0", COLOR(WHITE));	
//				JQ8400_PlaySong(&MP3,mobj->A_point);
//				JQ8400_PlaySong(&MP3,mobj->B_point+10);
//				return EVENT_HANDLED;
//			}
//			else{
//				mobj->active_state = OUTCOME;
//				return EVENT_TRANSITION;
//			}
//		}
//		case REMOTE_DEC:{
//			if(mobj->B_point >0)
//			{
//			mobj->B_point -=1;
//			char str_A[4]="";
//			char str_B[4]="";
//			char str_sec_A[4]="";
//			char str_sec_B[4]="";
//			
//			memset(str_A,0,4);
//			memset(str_B,0,4);
//			memset(str_sec_A,0,4);
//			memset(str_sec_B,0,4);
//			if(mobj->A_point <10)
//			{
//				sprintf(str_A,"0%d",mobj->A_point);
//			}
//			else
//			{
//				sprintf(str_A,"%d",mobj->A_point);
//			}
//			if(mobj->B_point <10)
//			{
//				sprintf(str_B,"0%d",mobj->B_point);
//			}
//			else
//			{
//				sprintf(str_B,"%d",mobj->B_point);
//			}		
//			displayClear();		
//			displayGoTo(5,5);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_A, COLOR(RED));
//			displayGoTo(38,5);
//			displaySetFont(Times_New_Roman12x16_BIG);
//			displayPrint(str_B, COLOR(BLUE));
//			displayGoTo(27,11);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("-", COLOR(WHITE));	
//			displayGoTo(8,24);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("..", COLOR(RED));		
//			displayGoTo(41,24);
//			displaySetFont(Times_New_Roman10x5_BIG);
//			displayPrint("..", COLOR(BLUE));
//			displayGoTo(26,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint("0", COLOR(WHITE));
//			displayGoTo(33,1);
//			displaySetFont(Times_New_Roman6x8_BIG);
//			displayPrint("0", COLOR(WHITE));	
//				JQ8400_PlaySong(&MP3,mobj->A_point);
//				JQ8400_PlaySong(&MP3,mobj->B_point+10);
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_HANDLED;
//			}
//			return EVENT_IGNORED;
//		}
//		case REMOTE_Y:{
//			mobj->active_state = B;
////			memset(str,0,50);
////			sprintf(str,"REMOTE_Y_B\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_TRANSITION;
//		}
//		case REMOTE_X:{
//			mobj->active_state = A;
//			return EVENT_TRANSITION;
//		}
//		case START_HOLD:{
//			mobj->active_state = IDLE;
////			memset(str,0,50);
////			sprintf(str,"HOLD_A\n");
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			return EVENT_TRANSITION;
//	}
//}
//	return EVENT_IGNORED;	
//}
