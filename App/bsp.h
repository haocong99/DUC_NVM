#ifndef _BSP_H_
#define _BSP_H_
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "button.h"
// DIN:
#define DIN_CNT								6
#define DIN_1_ID							0
#define DIN_2_ID							1
#define DIN_3_ID							2
#define DIN_4_ID							3
#define DIN_5_ID							4
#define DIN_6_ID							5
#define DIN_ID_MAX						6

/////////////////////////////////LOCAL CONSTANTS///////////////////////////////
// DIN Pin definitions:
#define DIN_1_PORT					GPIOB
#define DIN_1_PIN					GPIO_PIN_13

#define DIN_2_PORT					GPIOB
#define DIN_2_PIN					GPIO_PIN_14

#define DIN_3_PORT					GPIOB
#define DIN_3_PIN					GPIO_PIN_15

#define DIN_4_PORT					GPIOB
#define DIN_4_PIN					GPIO_PIN_3

#define DIN_5_PORT					GPIOB
#define DIN_5_PIN					GPIO_PIN_4

#define DIN_6_PORT					GPIOB
#define DIN_6_PIN					GPIO_PIN_5


/*Generic event structure */
typedef struct{
    uint8_t sig;
}event_t;

/* For user generated events */
typedef struct{
    event_t super;
}user_event_t;

typedef struct {
//	uint8_t BSP_DIN_short[6];
//	uint8_t BSP_DIN_hold[6];
	Button_Typedef Button_status[6];
} BSP_DIN_STATUS_t;


void BSP_din_init(void);
const BSP_DIN_STATUS_t *BSP_din_get(void);
const user_event_t *APL_event_get(void);
void BSP_din_manager(void);
void button_handler_pressed_short(const Button_Typedef *ButtonX);
void button_handler_hold(const Button_Typedef *ButtonX);

#endif
