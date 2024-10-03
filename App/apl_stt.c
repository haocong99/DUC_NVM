#include "apl.h"
#include "bsp.h"
//#include "displayDriver.h"
//#include "Courier9x13.h"
//#include "Arial16x19.h"
//#include "Victorian_LET16x26.h"
//#include "RGB565/FCB_32x32.h"
//#include "RGB565/RMA.h"
static char str[50];
extern UART_HandleTypeDef huart1;
char entry_idle[]="ENTRY IDLE\n";
char exit_idle[]="EXIT_IDLE\n";
char select_idle[] = "SELECT_IDLE\n";

char entry_setting_1[]="ENTRY_SETTING_1\n";
char exit_setting_1[] ="EXIT_SETTING_1\n";
char INC_setting_1[] = "INC_SETTING_1\n";
char DEC_setting_1[] = "DEC_SETTING_1\n";
char SELECT_setting_1[] ="SELECT_SETTING_1\n";
char HOLD_setting_1[] = "HOLD_SETTING_1\n";

char entry_setting_2[]="ENTRY_SETTING_2\n";
char exit_setting_2[] ="EXIT_SETTING_2\n";
char INC_setting_2[] = "INC_SETTING_2\n";
char DEC_setting_2[] = "DEC_SETTING_2\n";
char SELECT_setting_2[] ="SELECT_SETTING_2\n";
char HOLD_setting_2[] = "HOLD_SETTING_2\n";
char START_setting_2[] = "START_setting_2\n";

static event_status_t apl_state_handler_IDLE(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_1(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_SETTING_2(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_STARTING(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_OUTCOME(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_A(apl_status_t *const mobj, event_t const *const e);
static event_status_t apl_state_handler_B(apl_status_t *const mobj, event_t const *const e);

void apl_stt_init(apl_status_t *mobj)
{
	  event_t ee;
    ee.sig = ENTRY;
    mobj->active_state = IDLE;
    mobj->mode = 0;
		mobj->point_num = 0;
		mobj->A_point=0;
		mobj->B_point=0;
    apl_state_machine(mobj,&ee);
}

event_status_t apl_state_machine(apl_status_t *const mobj, event_t const *const e)
{
	switch(mobj->active_state)
	{
		case IDLE:
			return apl_state_handler_IDLE(mobj,e);
		case SETTING_1:
			return apl_state_handler_SETTING_1(mobj,e);
		case SETTING_2:
			return apl_state_handler_SETTING_2(mobj,e);
		case STARTING:
			return apl_state_handler_STARTING(mobj,e);
		case A:
			return apl_state_handler_A(mobj,e);
		case B:
			return apl_state_handler_B(mobj,e);
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
			//disp_msg("Play_NOW");
			memset(str,0,50);
			sprintf(str,"Play_now\n");
//		displayClear();		
//    displayGoTo(0,0);
//    displaySetFont(DEFAULT_BIG);
//    displayPrint("PlayNow", COLOR(MAGENTA));
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
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
			mobj->active_state = SETTING_1; 
//			memset(str,0,50);
//			sprintf(str,"SELECT_IDLE\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
	}
	return EVENT_IGNORED;
}

static event_status_t apl_state_handler_SETTING_1(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			//disp_mode(mobj->mode)
//		displayGoTo(0,0);
//    displaySetFont(DEFAULT_BIG);
//			displayPrint("Mode:don", COLOR(MAGENTA));
			memset(str,0,50);
			if( mobj->mode ==0)
			{
				sprintf(str,"mode:don\n");
				
			}
			if( mobj->mode ==1)
			{
			sprintf(str,"mode:doi\n");
			}	
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"exit_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->mode = ! mobj->mode;
			//disp_mode(mobj->mode)
			memset(str,0,50);
			if( mobj->mode ==0)
			{
				sprintf(str,"mode:don\n");
			}
			if( mobj->mode ==1)
			{
			sprintf(str,"mode:doi\n");
			}			
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case DEC:{
			mobj->mode = ! mobj->mode;
			//disp_mode(mobj->mode)
			memset(str,0,50);
			if( mobj->mode ==0)
			{
			sprintf(str,"mode:don\n");
			}
			if( mobj->mode ==1)
			{
			sprintf(str,"mode:doi\n");
			}
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case SELECT:{
			mobj->active_state = SETTING_2;
//			memset(str,0,50);
//			sprintf(str,"SELECT_setting_1\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
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

static event_status_t apl_state_handler_SETTING_2(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			//disp_mode(mobj->mode)
			memset(str,0,50);
			sprintf(str,"chon diem max:%d\n",mobj->point_num);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"exit_setting_2\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case INC:{
			mobj->point_num +=1;
			//disp_mode(mobj->point_num)
			memset(str,0,50);
			sprintf(str,"chon diem max:%d\n",mobj->point_num);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case DEC:{
			if(mobj->point_num>=1)
			{
			mobj->point_num -= 1;
			//disp_mode(mobj->point_num)
				memset(str,0,50);
			sprintf(str,"chon diem max:%d\n",mobj->point_num);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
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
			if(mobj->point_num>=1)
			{
				mobj->active_state = STARTING;
				memset(str,0,50);
			sprintf(str,"Tran dau bat dau\n");
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				return EVENT_TRANSITION;
			}
			return EVENT_IGNORED;
		}
	}
	return EVENT_IGNORED;	
}

static event_status_t apl_state_handler_STARTING(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			//disp_point(mobj->A_point)
			//disp_point(mobj->B_point)
			memset(str,0,50);
			sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"EXIT_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case REMOTE_X:{
			event_status_t apl_state_handler_A(apl_status_t *const mobj, event_t const *const e);
			mobj->active_state = A;
//			memset(str,0,50);
//			sprintf(str,"REMOTE_X_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case REMOTE_Y:{
			mobj->active_state = B;
//			memset(str,0,50);
//			sprintf(str,"REMOTE_Y_STARTING\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
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
			sprintf(str,"A WIN WIN WIN\n");
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			memset(str,0,50);
			sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
			}
			else if(mobj->A_point < mobj->B_point)
			{
			memset(str,0,50);
			sprintf(str,"B WIN WIN WIN\n");
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			memset(str,0,50);
			sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
			}
		}
		case EXIT:{
			//disp_clr();
			memset(str,0,50);
			sprintf(str,"THANK!!!\n");
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}		
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_OUTCOME\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
	}	
	return EVENT_IGNORED;		
}
static event_status_t apl_state_handler_A(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			//disp_point(Apoint,Bpoint)
//			memset(str,0,50);
//			sprintf(str,"ENTRY_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				memset(str,0,50);
				sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"EXIT_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case REMOTE_INC:{
			if(++mobj->A_point<mobj->point_num)
			{
				memset(str,0,50);
//				sprintf(str,"REMOTE_INC_A\n");
//				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				return EVENT_HANDLED;			
			}
			else{
				mobj->active_state = OUTCOME;
				return EVENT_TRANSITION;
			}
		}
		case REMOTE_DEC:{
			if(mobj->A_point >0)
			{
			mobj->A_point -=1;
				memset(str,0,50);
//			sprintf(str,"REMOTE_DEC_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
			}
			return EVENT_IGNORED;
		}
		case REMOTE_X:{
			mobj->active_state = A;
//			memset(str,0,50);
//			sprintf(str,"REMOTE_X_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case REMOTE_Y:{
			mobj->active_state = B;
			return EVENT_TRANSITION;
		}
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
	}
}
	return EVENT_IGNORED;	
}

static event_status_t apl_state_handler_B(apl_status_t *const mobj, event_t const *const e)
{
	switch(e->sig)
	{
		case ENTRY:{
			//disp_point(Apoint,Bpoint)
//			memset(str,0,50);
//			sprintf(str,"ENTRY_B\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case EXIT:{
			//disp_clr();
//			memset(str,0,50);
//			sprintf(str,"EXIT_B\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
		}
		case REMOTE_INC:{
			if(++mobj->B_point<mobj->point_num)
			{
//				memset(str,0,50);
//				sprintf(str,"REMOTE_INC_B\n");
//				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
				HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
				return EVENT_HANDLED;
			}
			else{
				mobj->active_state = OUTCOME;
				return EVENT_TRANSITION;
			}
		}
		case REMOTE_DEC:{
			if(mobj->B_point >0)
			{
			mobj->B_point -=1;
//				memset(str,0,50);
//			sprintf(str,"REMOTE_DEC_B\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			sprintf(str,"A:%d vs B:%d\n",mobj->A_point,mobj->B_point);
			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_HANDLED;
			}
			return EVENT_IGNORED;
		}
		case REMOTE_Y:{
			mobj->active_state = B;
//			memset(str,0,50);
//			sprintf(str,"REMOTE_Y_B\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
		}
		case REMOTE_X:{
			mobj->active_state = A;
			return EVENT_TRANSITION;
		}
		case START_HOLD:{
			mobj->active_state = IDLE;
//			memset(str,0,50);
//			sprintf(str,"HOLD_A\n");
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			return EVENT_TRANSITION;
	}
}
	return EVENT_IGNORED;	
}
