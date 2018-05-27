#ifndef __GPS_H
#define __GPS_H
#include "stdio.h"	
#include "string.h"
#include "stm32f10x.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
 	
extern char  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 point1;



#define false 0
#define true 1

//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;											//�Ƿ��ȡ��GPS����
	char isParseData;										//�Ƿ�������
	char UTCTime[UTCTime_Length];				//UTCʱ��
	char latitude[latitude_Length];			//γ��
	char N_S[N_S_Length];								//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];								//E/W
	char isUsefull;											//��λ��Ϣ�Ƿ���Ч
} _SaveData;




void USART2_Config(void);
extern char rxdatabufer;
extern u16 point1;
extern _SaveData Save_Data;

void CLR_Buf(void);
u8 Hand(char *a);
void clrStruct(void);
void parseGpsBuffer(void);
void printGpsBuffer(void);
#endif