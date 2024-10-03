#include "JQ8400_FL.h"
uint8_t SendFrame[8] = {0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define HBYTE(X) ((X & 0xFF00) >> 8) 
#define LBYTE(X) (X & 0xFF) 

////******************************** LOW Level Functions ****************************//

static void JQ8400_SendUART(JQ8400_Name* MP3,uint8_t size)
{
	HAL_UART_Transmit(MP3->JQ8400_UART, (uint8_t *)&MP3->SendBuff, size, 1000);
}
static void JQ8400_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}

static uint8_t JQ8400_GetChecksum (uint8_t *thebuf, int datasize)
{
	uint16_t sum = 0;
	for (int i=0; i<datasize; i++) 
	{
		sum += thebuf[i];
	}
	return sum;
}


static void JQ8400_SendCmd(JQ8400_Name* MP3,uint8_t size)
{
	JQ8400_SendUART(MP3,size);
	JQ8400_Delay(100);
}

////******************************** High Level Functions ****************************//
void JQ8400_Init(JQ8400_Name* MP3, UART_HandleTypeDef* UART)
{
	for(int i = 0; i<8; i++)
	{
		MP3->SendBuff[i] = SendFrame[i];
	}
	MP3->JQ8400_UART = UART;
}

void JQ8400_SelectTrack(JQ8400_Name* MP3, uint16_t num)
{
	MP3->SendBuff[1] = JQ8400_SELECT_BUT_NOT_PLAY;
	MP3->SendBuff[2] = 0x02;
	MP3->SendBuff[3] = HBYTE(num);
	MP3->SendBuff[4] = LBYTE(num);
	MP3->SendBuff[5] = JQ8400_GetChecksum (MP3->SendBuff,5);
	JQ8400_SendCmd(MP3,6);	
}

//void DFPLAYER_Next(DFPLAYER_Name* MP3) 
//{
//	DFPLAYER_SendCmd(MP3, DFP_NEXT,00,00);
//}

//void DFPLAYER_Prev(DFPLAYER_Name* MP3)
//{
//	DFPLAYER_SendCmd(MP3, DFP_PREV,00,00);
//}

void JQ8400_SetVolume(JQ8400_Name* MP3, uint8_t volume)
{
	MP3->SendBuff[1] = JQ8400_VOLUME_CONTROL;
	MP3->SendBuff[2] = 0x01;
	MP3->SendBuff[3] = volume;
	MP3->SendBuff[4] = JQ8400_GetChecksum (MP3->SendBuff,4);
	JQ8400_SendCmd(MP3,5);
}

void JQ8400_PlaySong(JQ8400_Name* MP3,uint8_t number)
{
	MP3->SendBuff[1] = JQ8400_SPECIFY_SONG;
	MP3->SendBuff[2] = 0x02;
	MP3->SendBuff[3] = HBYTE(number);
	MP3->SendBuff[4] = LBYTE(number);
	MP3->SendBuff[5] = JQ8400_GetChecksum (MP3->SendBuff,5);
	JQ8400_SendCmd(MP3,6);
}

void JQ8400_Pause(JQ8400_Name* MP3)
{
	MP3->SendBuff[1] = JQ8400_PAUSE;
	MP3->SendBuff[2] = 0x00;
	MP3->SendBuff[3] = JQ8400_GetChecksum (MP3->SendBuff,3);
	JQ8400_SendCmd(MP3,4); 	
}

//void DFPLAYER_Stop(DFPLAYER_Name* MP3)
//{
//	DFPLAYER_SendCmd (MP3, DFP_STOP, 00, 00);
//}

//void DFPLAYER_RandomPlay(DFPLAYER_Name* MP3)
//{
//	DFPLAYER_SendCmd (MP3, DFP_RANDOM,0,0);
//}

//void DFPLAYER_PlayFileInFolder(DFPLAYER_Name* MP3, uint8_t folder, uint32_t num) 
//{
//	DFPLAYER_SendCmd (MP3, DFP_PLAYFILEINFOLDER, folder, num);
//}
int fileNums(JQ8400_Name* MP3) {
//  unsigned char commands[4] = {0xAA,0x0C,0x00,0xB6};
//  SoftSerial.write(commands,4);
	MP3->SendBuff[1] = JQ8400_TOTAL_AUDIO;
	MP3->SendBuff[2] = 0x00;
	MP3->SendBuff[3] = JQ8400_GetChecksum (MP3->SendBuff,3);
	JQ8400_SendCmd(MP3,4); 

	
//  int rByte = 0;
//  uint8_t rcvData[6];
//  while(rByte < 6) {
//      uint8_t incomingByte = SoftSerial.read();
//      rcvData[rByte] = incomingByte ;
//      rByte ++;
//    }
//  }
//  int fileNum = rcvData[3] * 256 + rcvData[4];
  return 3;
}
