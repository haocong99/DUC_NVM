#ifndef _BSP_H_
#define _BSP_H_
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "button.h"
// DIN:
#define DIN_CNT								8
#define DIN_1_ID							0
#define DIN_2_ID							1
#define DIN_3_ID							2
#define DIN_4_ID							3
#define DIN_5_ID							4
#define DIN_6_ID							5
#define DIN_7_ID							6
#define DIN_8_ID							7
#define DIN_ID_MAX						7

typedef struct {
	uint8_t BSP_DIN_short[8];
	uint8_t BSP_DIN_hold[8];
} BSP_DIN_STATUS_t;

void BSP_din_init(void);
const BSP_DIN_STATUS_t *BSP_din_get(void);
void BSP_din_manager(void);

#endif
