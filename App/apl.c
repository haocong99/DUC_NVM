#include "apl.h"
#include "bsp.h"
#include "string.h"
#include "displayDriver.h"


static char str[50]="";
extern UART_HandleTypeDef huart1;
extern BSP_DIN_STATUS_t g_din_status;
/*Main application object */
static apl_status_t status;
//static	uint8_t chang_flg =0;

static user_event_t g_status_event;

const user_event_t *APL_event_get(void)
{
	return &g_status_event;
}

void APL_init(void)
{
	apl_stt_init(&status);
	displayInit();
}


void OSL_din_callback(const BSP_DIN_STATUS_t *p_din)
{
//	user_event_t *p_event = &g_status_event;
//	if(p_din->BSP_DIN_short[0]==1)
//	{
//		g_status_event.super.sig = DEC;
//		memset(str,0,50);
//		sprintf(str,"DEC");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//	}
//	if(p_din->BSP_DIN_short[1]==1)
//	{
//		g_status_event.super.sig = SELECT;
//		memset(str,0,50);
//		sprintf(str,"SELECT");
//		
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//	}
//	if(p_din->BSP_DIN_short[2]==1)
//	{
//		g_status_event.super.sig = START;
//		memset(str,0,50);
//		sprintf(str,"START");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//	}
//	if(p_din->BSP_DIN_short[3]==1)
//	{
//		g_status_event.super.sig = INC;
//		memset(str,0,50);
//		sprintf(str,"INC");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//	}
//	if(p_din->BSP_DIN_short[4]==1)
//	{
//		g_status_event.super.sig = REMOTE_INC;
//		memset(str,0,50);
//		sprintf(str,"REMOTE_INC");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);		
//	}
//		if(p_din->BSP_DIN_short[5]==1)
//	{
//		g_status_event.super.sig = DEC; // xuong
//		memset(str,0,50);
//		sprintf(str,"REMOTE_X");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);		
//	}
//		if(p_din->BSP_DIN_short[6]==1)
//	{
//		g_status_event.super.sig = SELECT; // select
//		memset(str,0,50);
//		sprintf(str,"REMOTE_Y");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//	}
//		if(p_din->BSP_DIN_short[7]==1)
//	{
//		g_status_event.super.sig = INC;  // len
//		memset(str,0,50);
//		sprintf(str,"REMOTE_DEC");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//	}
//	if(p_din->BSP_DIN_hold[2]==1)
//	{
//		g_status_event.super.sig = START_HOLD;
//		memset(str,0,50);
//		sprintf(str,"START_HOLD");
//		HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//	}
}



void APL_din_callback(const Button_Typdef *ButtonX)
{
		const BSP_DIN_STATUS_t *p_din = BSP_din_get();
//		uint32_t i = 0;
//		for (i = 0; i < DIN_CNT; i++) {
//			if(p_din->Button_status[i].btn_sm_state == PRESSED)
//			{
////			sprintf(str,"apl_din_callback %d giá tri %d\n ",i,g_din_status.BSP_DIN_short[i]);
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			//HAL_Delay(100);
		if(ButtonX->btn_sm_state == PRESSED)
		{
			switch(ButtonX->button_ID)
			{
				case DIN_1_ID:
				{
					g_status_event.super.sig = DEC;
					memset(str,0,50);
					sprintf(str,"DEC");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;
				}
				case DIN_2_ID:
				{
					g_status_event.super.sig = SELECT;
					memset(str,0,50);
					sprintf(str,"SELECT");	
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);		
					break;					
				}
				case DIN_3_ID:
				{
					g_status_event.super.sig = START;
					memset(str,0,50);
					sprintf(str,"START");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;					
				}
				case DIN_4_ID:
				{
					g_status_event.super.sig = REMOTE_INC;
					memset(str,0,50);
					sprintf(str,"REMOTE_INC");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;						
				}
				case DIN_5_ID:
				{
					g_status_event.super.sig = DEC; // xuong
					memset(str,0,50);
					sprintf(str,"REMOTE_X");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;						
				}
				case DIN_6_ID:
				{
					g_status_event.super.sig = SELECT; // select
					memset(str,0,50);
					sprintf(str,"REMOTE_Y");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;
				}
				case DIN_7_ID:
				{
					g_status_event.super.sig = INC;  // len
					memset(str,0,50);
					sprintf(str,"REMOTE_DEC");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;
				}
			}	
}
		else if (ButtonX->btn_sm_state == HOLD)
		{
			switch(ButtonX->button_ID)
			{
				case DIN_1_ID:
				{
//					g_status_event.super.sig = DEC;
					memset(str,0,50);
					sprintf(str,"DEC_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;
				}
				case DIN_2_ID:
				{
//					g_status_event.super.sig = SELECT;
					g_status_event.super.sig= START_HOLD;
					memset(str,0,50);
					sprintf(str,"SELECT_HOLD");	
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);		
					break;					
				}
				case DIN_3_ID:
				{
//					g_status_event.super.sig = START;
					memset(str,0,50);
					sprintf(str,"START_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;					
				}
				case DIN_4_ID:
				{
//					g_status_event.super.sig = REMOTE_INC;
					memset(str,0,50);
					sprintf(str,"REMOTE_INC_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;						
				}
				case DIN_5_ID:
				{
	//				g_status_event.super.sig = DEC; // xuong
					memset(str,0,50);
					sprintf(str,"REMOTE_X_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
					break;						
				}
				case DIN_6_ID:
				{
	//				g_status_event.super.sig = SELECT; // select
					memset(str,0,50);
					sprintf(str,"REMOTE_Y_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;
				}
				case DIN_7_ID:
				{
//					g_status_event.super.sig = INC;  // len
					memset(str,0,50);
					sprintf(str,"REMOTE_DEC_HOLD");
					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
					break;
				}
			}	
		}
//		OSL_din_callback(&g_din_status);
		APL_event_dispatcher(&status,&g_status_event.super);
}
void APL_event_dispatcher(apl_status_t *const mobj,event_t const *const e)
{
	event_status_t status;
  apl_state_t source, target;
	source = mobj->active_state;
	status = apl_state_machine(mobj,e);
	if(status == EVENT_TRANSITION)
	{
		target = mobj->active_state;
		event_t ee;
		//1. run the exit action for the source state
    ee.sig = EXIT;
    mobj->active_state = source;
    apl_state_machine(mobj,&ee);

    //2. run the entry action for the target state
    ee.sig = ENTRY;
    mobj->active_state = target;
    apl_state_machine(mobj,&ee);
	}
	
}

void APL_Manager(void)
{
	const user_event_t *p_event_t = APL_event_get();
	BSP_din_manager();

}
