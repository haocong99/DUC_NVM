#include "bsp.h"
#include "button.h"
//---------------var button------------


__weak void	btn_pressing_callback(Button_Typedef *ButtonX)
{
}
__weak void btn_press_short_callback(const Button_Typedef *ButtonX )
{
	
}
__weak void btn_release_callback(Button_Typedef *ButtonX)
{

}
__weak void btn_press_timeout_callback(Button_Typedef *ButtonX)
{

}
void button_read_value(Button_Typedef *ButtonX)
{
	//------------------ Loc nhieu ------------------------
	uint8_t sta = HAL_GPIO_ReadPin(ButtonX->GPIOx, ButtonX->GPIO_Pin);
	if(sta != ButtonX->btn_filter)
	{
		ButtonX->btn_filter = sta;
		ButtonX->is_debouncing = 1;
		ButtonX->btn_sm_state = BOUNCE;
		ButtonX->time_deboune = HAL_GetTick();
	}
	//------------------ Tin hieu da xac lap------------------------
	if((ButtonX->btn_sm_state == BOUNCE) && (HAL_GetTick() - ButtonX->time_deboune >= 50))
	{
		ButtonX->btn_current = ButtonX->btn_filter;
		ButtonX->is_debouncing =0;
	}
	//---------------------Xu li nhan nha------------------------
	if(ButtonX->btn_current != ButtonX->btn_last)
	{
		if (ButtonX->GPIO_Pin == DIN_1_PIN || 
		ButtonX->GPIO_Pin == DIN_2_PIN || 
		ButtonX->GPIO_Pin == DIN_3_PIN)
		{
				if(ButtonX->btn_current == 0)//nhan xuong
				{
					ButtonX->is_press_timeout = 1;
					ButtonX->btn_sm_state = PRESSED;
					//btn_pressing_callback(ButtonX);
					ButtonX->time_start_press = HAL_GetTick();
					//return;
				}
				else //nha nut
				{
					if(ButtonX->is_press_timeout && (HAL_GetTick() - ButtonX->time_start_press <1000))
					{
						ButtonX->btn_sm_state = PRESSED_SHORT;
						ButtonX->is_press_timeout = 0;
						//btn_press_short_callback(ButtonX);
						//return;
					}
					ButtonX->is_press_timeout = 0;
					ButtonX->btn_sm_state = NOT_PRESSED;	
					//btn_release_callback(ButtonX);							
				}			
			}	
		if (ButtonX->GPIO_Pin == DIN_5_PIN || 
		ButtonX->GPIO_Pin == DIN_6_PIN || 
		ButtonX->GPIO_Pin == DIN_4_PIN)
		{
				if(ButtonX->btn_current == 1)//nhan xuong
				{
					ButtonX->is_press_timeout = 1;
					ButtonX->btn_sm_state = PRESSED;
					//btn_pressing_callback(ButtonX);
					ButtonX->time_start_press = HAL_GetTick();
					//return;
				}
				else //nha nut
				{
					if(ButtonX->is_press_timeout && (HAL_GetTick() - ButtonX->time_start_press <1000))
					{
						ButtonX->btn_sm_state = PRESSED_SHORT;
						ButtonX->is_press_timeout = 0;
						//btn_press_short_callback(ButtonX);
						return;
					}
					ButtonX->is_press_timeout = 0;
					ButtonX->btn_sm_state = NOT_PRESSED;	
					//btn_release_callback(ButtonX);							
				}			
			}			
			ButtonX->btn_last = ButtonX->btn_current;
		}

		//-------------Xu li nhan giu----------------
		if(ButtonX->is_press_timeout &&(HAL_GetTick() - ButtonX->time_start_press >= 3000))
		{
			ButtonX->btn_sm_state = HOLD;
			//btn_press_timeout_callback(ButtonX);
			ButtonX->is_press_timeout =0;
			return;
		}
}


void button_init(Button_Typedef *ButtonX,GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	ButtonX->GPIOx = GPIOx;
	ButtonX->GPIO_Pin = GPIO_Pin;
	ButtonX->btn_filter = 1;
}
