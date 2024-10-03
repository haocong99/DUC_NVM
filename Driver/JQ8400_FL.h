#ifndef __JQ8400_FL_H
#define __JQ8400_FL_H
#include "stm32f1xx_hal.h"

typedef struct
{
	UART_HandleTypeDef* JQ8400_UART;
	uint8_t SendBuff[8];
	uint16_t Checksum;
}JQ8400_Name;

#define JQ8400_STATUS 							0X01
#define JQ8400_PLAY 								0X02
#define JQ8400_PAUSE 								0X03
#define JQ8400_STOP 								0X04
#define JQ8400_LASTSONG 						0X05
#define JQ8400_NEXTSONG 						0X06
#define JQ8400_SPECIFY_SONG					0X07
#define JQ8400_SPECIFY_DRIVER				0X08
#define JQ8400_TOTAL_AUDIO					0x0C

#define JQ8400_VOLUME_CONTROL				0X13
#define JQ8400_VOLUME_INCREASE			0X14
#define JQ8400_VOLUME_DECREASE			0X15
#define JQ8400_MODE_LOOP						0X18
#define JQ8400_NUMBER_LOOP					0X19
#define JQ8400_EQ_SETTING						0X1A
#define JQ8400_SELECT_BUT_NOT_PLAY	0X1F





#define DFP_PLAYTRACK 				0X12
#define DFP_NEXT 							0X01
#define DFP_PREV		  				0X02
#define DFP_SETVOLUME 				0X06
#define DFP_PLAY		 				 	0X0D
#define DFP_PAUSE		 				  0X0E
#define DFP_STOP					  	0X16
#define DFP_RANDOM 						0X18
#define DFP_PLAYFILEINFOLDER  0X0F

void JQ8400_Init(JQ8400_Name* MP3, UART_HandleTypeDef* UART);
void JQ8400_SelectTrack(JQ8400_Name* MP3, uint16_t num);
//void DFPLAYER_Next(JQ8400_Name* MP3);
//void DFPLAYER_Prev(JQ8400_Name* MP3);
void JQ8400_SetVolume(JQ8400_Name* MP3, uint8_t volume);
void JQ8400_PlaySong(JQ8400_Name* MP3,uint8_t number);
void JQ8400_Pause(JQ8400_Name* MP3);
//void DFPLAYER_Stop(JQ8400_Name* MP3);
//void DFPLAYER_RandomPlay(JQ8400_Name* MP3);
void JQ8400_PlayFileInFolder(JQ8400_Name* MP3, uint8_t folder, uint32_t num);
int fileNums(JQ8400_Name* MP3);


#endif
