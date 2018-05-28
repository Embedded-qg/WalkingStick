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

#define _DEBUG_ 1										//���ڵ���
#if _DEBUG_
    #define DEBUG(x) printf(x)
#else
    #define DEBUG(x) 
#endif


uint8_t direction_flag;


int main(void)
{
	
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									//���泬��������
	
	int sendByBlueTooth[MAX_SEND];		  //���ͱ��������Լ���λ����
	
	delayInit();												//��ʼ���δ�ʱ��
	
	EXTI_PA0_Config(); 									//��λ�����������жϳ�ʼ��
	NVIC_Configuration();								//�����ⲿ�ж�0���ȼ�
	
	UltrasonicWave_Configuration();			//��ʼ������
	
	USART1_Config(38400);               //����1��ʼ��,���ڵ�
	
	I2C_GPIO_Config();									//I2CͨѶ�ĳ�ʼ��
	
  InitLSM303D();											//��λģ��ĳ�ʼ��
	
	SysTick_Init();                  	  //��શ�ʱ����ʼ��
	
	GENERAL_TIM_Init();									//��ʼ����ʱ��
	
	USART2_Config();										//����2������gps
	
	 while(1)           
	{
//		i++;
//		if(i>100)
//		{
//			i=0;
//		}
		parseGpsBuffer();									
		printGpsBuffer();
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

			UltrasonicWave(num);								//��ȡ����������
			delayMs(500);            //����1�������ʹgprs��ס����֪��Ϊʲô
	}
}

/*********************************************END OF FILE**********************/


