#include "bsp.h"
#include "button.h"
#include "apl.h"
#include "string.h"

static char str[30]="";
extern UART_HandleTypeDef huart1;
/////////////////////////////////LOCAL CONSTANTS///////////////////////////////
// DIN Pin definitions:
#define DIN_1_PORT					GPIOB
#define DIN_1_PIN					GPIO_PIN_14

#define DIN_2_PORT					GPIOB
#define DIN_2_PIN					GPIO_PIN_15

#define DIN_3_PORT					GPIOB
#define DIN_3_PIN					GPIO_PIN_12

#define DIN_4_PORT					GPIOB
#define DIN_4_PIN					GPIO_PIN_13

#define DIN_5_PORT					GPIOB
#define DIN_5_PIN					GPIO_PIN_6

#define DIN_6_PORT					GPIOB
#define DIN_6_PIN					GPIO_PIN_5

#define DIN_7_PORT					GPIOB
#define DIN_7_PIN					GPIO_PIN_4

#define DIN_8_PORT					GPIOB
#define DIN_8_PIN					GPIO_PIN_3
static GPIO_TypeDef *DIN_PORT_LIST[] = {DIN_1_PORT, DIN_2_PORT, DIN_3_PORT, DIN_4_PORT, DIN_5_PORT, DIN_6_PORT, DIN_7_PORT, DIN_8_PORT};
static uint32_t DIN_PIN_LIST[] = {DIN_1_PIN, DIN_2_PIN, DIN_3_PIN, DIN_4_PIN, DIN_5_PIN, DIN_6_PIN, DIN_7_PIN, DIN_8_PIN};
static Button_Typdef button[8];
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
//uint8_t BSP_DIN_short[7];
//----------------var led-----------
//Button_Typdef button1;
//Button_Typdef button2;
//Button_Typdef button3;
//Button_Typdef button4;
//Button_Typdef button_increase ;
//Button_Typdef button_decrement;
//Button_Typdef button_A;
//Button_Typdef button_B;

void btn_pressing_callback(Button_Typdef *ButtonX)
{	
	uint32_t i = 0;
	for (i = 0; i < DIN_CNT; i++) {
		if(ButtonX == &button[i])
		{
			g_din_status.BSP_DIN_short[i]=1;
//			sprintf(str,"button short %d giá tri %d\n ",i,g_din_status.BSP_DIN_short[i]);			
//			HAL_UART_Transmit(&huart1,(uint8_t*) str, sizeof(str), 1000);
			APL_din_callback();
		}
	}
}

void btn_press_timeout_callback(Button_Typdef *ButtonX)
{
	uint32_t i = 0;
	for (i = 0; i < DIN_CNT; i++) {
		if(ButtonX == &button[i])
		{
			g_din_status.BSP_DIN_hold[i]=1;
			HAL_UART_Transmit(&huart1,(uint8_t*) button1_hold, sizeof(button1_short), 1000);
			APL_din_callback();
		}
	}
}

void btn_release_callback(Button_Typdef *ButtonX)
{
		uint32_t i = 0;
		for (i = 0; i < DIN_CNT; i++) {
		if(ButtonX == &button[i])
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
		button_init(&button[i],DIN_PORT_LIST[i],DIN_PIN_LIST[i]);
	}

}
const BSP_DIN_STATUS_t *BSP_din_get(void)
{
	return &g_din_status;
	
}

void BSP_din_manager(void)
{
	uint32_t i = 0;	
	for (i = 0; i < DIN_CNT; i++) {
		button_handle(&button[i]);
	}
}
