/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-25
  * @brief   ���ܹ���������
  ******************************************************************************
	*/
#include "UltrasonicWave.h"
#include "bsp_delay.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
#include "gps.h"
#include "timer.h"
#include "bsp_usart4.h"
#include "gprs.h"

#define _DEBUG_ 1										//���ڵ���
#if _DEBUG_
    #define DEBUG(x) printf(x)
#else
    #define DEBUG(x) 
#endif

extern char *pdu_content ;
uint8_t direction_flag;

//GPRS��������ʼ������
static void Periph_GPRS_Init()
{
	UART4_Init_Config(115200);//GPRSʹ�ô���4��ʼ��
	SysTick_Init_Config();//�δ�ʱ����ʼ��
	Timer2_Init_Config();	//��ʱ��2��ʼ��
}

int main(void)
{
	int ret;
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									//���泬��������
	
	int sendByBlueTooth[MAX_SEND];		  //���ͱ��������Լ���λ����
	
//	delayInit();												//��ʼ���δ�ʱ��
	
	EXTI_PA0_Config(); 									//��λ�����������жϳ�ʼ��
//	NVIC_Configuration();								//�����ⲿ�ж�0���ȼ�
	
	UltrasonicWave_Configuration();			//��ʼ������
	
	USART1_Config(38400);               //����1��ʼ��,��������
	
	I2C_GPIO_Config();									//I2CͨѶ�ĳ�ʼ��
	
  InitLSM303D();											//��λģ��ĳ�ʼ��
	
//	SysTick_Init();                  	  //��શ�ʱ����ʼ��
	 
	GENERAL_TIM_Init();									//��ʼ����ʱ��
	
	USART2_Config();										//����2������gps
	
	Periph_GPRS_Init();	//GPRS��������ʼ������

	send_pdu_message(pdu_content);     //����pdu����

	 while(1)           
	{

//		printf("ϵͳ����......");
//		GPRS_Send_help();	//ʹ��GPRS��������ź�
//		GPRS_Send_GPS(22.2, 33.3);	//ʹ��GPRS����λ������
		
//		i++;
//		if(i>100)
//		{
//			i=0;
//		}
//		parseGpsBuffer();									
//		printGpsBuffer();
//			UltrasonicWave(num);								//��ȡ����������
//			if(direction_flag)
//			{
//					angle=getAngle();
//					direction_flag=0;
//					printf("%d",angle);
//			}
//			angle=0;
//			num[AVER_NUM-1]=angle;
//			for(i=0;i<AVER_NUM;i++)				//num�����һ�������������ͷ�λ��Ϣ
//			{
//				printf("%d",num[i]);
//			}
//			delayMs(600);            //����1�������ʹgprs��ס����֪��Ϊʲô
////			GPS_GPRS(GpsInfo,beiJingTime,sendData);
//			i=0;
//		}

//			UltrasonicWave(num);								//��ȡ����������
//			delayMs(500);            //����1�������ʹgprs��ס����֪��Ϊʲô

	}
}

/*********************************************END OF FILE**********************/


