#include "bsp.h"
#include "button.h"
#include "apl.h"
#include "string.h"

static char str[30]="";
extern UART_HandleTypeDef huart1;

static GPIO_TypeDef *DIN_PORT_LIST[] = {DIN_1_PORT, DIN_2_PORT, DIN_3_PORT, DIN_4_PORT, DIN_5_PORT, DIN_6_PORT, DIN_7_PORT, DIN_8_PORT};
static uint32_t DIN_PIN_LIST[] = {DIN_1_PIN, DIN_2_PIN, DIN_3_PIN, DIN_4_PIN, DIN_5_PIN, DIN_6_PIN, DIN_7_PIN, DIN_8_PIN};

char button1_short[] = "button1_short\n";
char button2_short[] = "button2_short\n";
char button1_hold[] = "button1_hold\n";
char button2_hold[] = "button2_hold\n";
char button3_short[] = "button3_short\n";
char button4_short[] = "button4_short\n";
char buttonA_short[] = "buttonA_short\n";
char buttonB_short[] = "buttonB_short\n";
char button_increase_short[] = "button_increase\n";
char button_decrease_short[] = "button_decrease\n";
char button1_release[] = "button1_release\n";


/////////////////////////////////LOCAL VARIABLES///////////////////////////////
/** Array holding current status of all DIN input (8 DIN) */
BSP_DIN_STATUS_t g_din_status;

void btn_pressing_callback(Button_Typdef *ButtonX)
{
			APL_din_callback(ButtonX);

}

void btn_press_short_callback(Button_Typdef *ButtonX)
{	
	APL_din_callback(ButtonX);
//	uint32_t i = 0;
//	for (i = 0; i < DIN_CNT; i++) {
//		if(ButtonX == &g_din_status.Button_status[i])
//		{
//			g_din_status.BSP_DIN_short[i]=1;
////			sprintf(str,"button short %d giá tri %d\n ",i,g_din_status.BSP_DIN_short[i]);			
////			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//			APL_din_callback();
//		}

//			switch(ButtonX->button_ID)
//			{
//				case DIN_1_ID:
//				{
//					g_status_event.super.sig = DEC;
//					memset(str,0,50);
//					sprintf(str,"DEC");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//					break;
//				}
//				case DIN_2_ID:
//				{
//					g_status_event.super.sig = SELECT;
//					memset(str,0,50);
//					sprintf(str,"SELECT");	
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);		
//					break;					
//				}
//				case DIN_3_ID:
//				{
//					g_status_event.super.sig = START;
//					memset(str,0,50);
//					sprintf(str,"START");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//					break;					
//				}
//				case DIN_4_ID:
//				{
//					g_status_event.super.sig = REMOTE_INC;
//					memset(str,0,50);
//					sprintf(str,"REMOTE_INC");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//					break;						
//				}
//				case DIN_5_ID:
//				{
//					g_status_event.super.sig = DEC; // xuong
//					memset(str,0,50);
//					sprintf(str,"REMOTE_X");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
//					break;						
//				}
//				case DIN_6_ID:
//				{
//					g_status_event.super.sig = SELECT; // select
//					memset(str,0,50);
//					sprintf(str,"REMOTE_Y");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//					break;
//				}
//				case DIN_7_ID:
//				{
//					g_status_event.super.sig = INC;  // len
//					memset(str,0,50);
//					sprintf(str,"REMOTE_DEC");
//					HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);	
//					break;
//				}
//			}
//		APL_din_callback(ButtonX);
}

void btn_press_timeout_callback(Button_Typdef *ButtonX)
{
//	uint32_t i = 0;
//	for (i = 0; i < DIN_CNT; i++) {
//		if(ButtonX == &g_din_status.Button_status[i])
//		{
//			g_din_status.BSP_DIN_hold[i]=1;
//			HAL_UART_Transmit(&huart1,(uint8_t*) button1_hold, sizeof(button1_short), 1000);
//			APL_din_callback();
//		}
//	}

		APL_din_callback(ButtonX);
}

void btn_release_callback(Button_Typdef *ButtonX)
{
		uint32_t i = 0;
		for (i = 0; i < DIN_CNT; i++) {
		if(ButtonX == &g_din_status.Button_status[i])
		{
			g_din_status.BSP_DIN_short[i]=0;
			g_din_status.BSP_DIN_hold[i]=0;
//			sprintf(str,"button release %d giá tri %d\n ",i,g_din_status.BSP_DIN_short[i]);
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
		}
	}
}



void BSP_din_init(void)
{
	uint32_t i = 0;
	for (i = 0; i < DIN_CNT; i++) {
		g_din_status.BSP_DIN_short[i] = 0;
		g_din_status.BSP_DIN_hold[i] = 0;
		g_din_status.Button_status->button_ID=i;
		button_init(&g_din_status.Button_status[i],DIN_PORT_LIST[i],DIN_PIN_LIST[i]);
	}

}
const BSP_DIN_STATUS_t *BSP_din_get(void)
{
	return &g_din_status;
	
}

void BSP_din_manager(void)
{
	uint32_t i = 0;	
	for (i = 0; i < DIN_CNT; i++) 
	{
		button_handle(&g_din_status.Button_status[i]);
	}
}
