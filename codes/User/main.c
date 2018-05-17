/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2014-08-xx
  * @brief   gpsģ����ʾ���Գ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-STM32 ������
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
	*/
#include "UltrasonicWave.h"
#include "bsp_delay.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "gps_config.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_usart3.h"
#include "sim900a.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
uint8_t direction_flag;

/*
 * ����GPSģ�鹦��
 * 
 */


int main(void)
{
	
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									//���泬��������
	nmeaINFO GpsInfo;     						  //GPS�ṹ��
	nmeaTIME beiJingTime;  							//����ʱ��
	char sendData[30]; 									//gprs��Ҫ���͵�����
	int sendByBlueTooth[MAX_SEND];		  //���ͱ��������Լ���λ����
	delayInit();
	EXTI_PA0_Config(); 									//��λ�����������жϳ�ʼ��
	NVIC_Configuration();								//�����ⲿ�ж�0���ȼ�
	UltrasonicWave_Configuration();			//��ʼ������
	USART1_Config(9600);               //����1��ʼ��,���ڵ���
	I2C_GPIO_Config();									//I2CͨѶ�ĳ�ʼ��

  InitLSM303D();											//��λģ��ĳ�ʼ��
	SysTick_Init();                  	  //��શ�ʱ����ʼ��
	GENERAL_TIM_Init();									//��ʼ����ʱ��
	USART3_Config(9600);             	 	//����3��ʼ��������gprs
	GPS_Config();                    	 	//����2��ʼ��������gps
		gprs_init("47.106.74.67","10001");
	
//	GPS_GPRS(GpsInfo,beiJingTime,sendData);  //gps��Ϣ�ķ���
	printf("Hello\r\n");
	 while(1)           
	{
//		i++;
//		if(i>100)
//		{
//			GPS_GPRS(GpsInfo,beiJingTime,sendData);
//			i=0;
//		}
//		printf("Hello\r\n");
			GPS_GPRS(GpsInfo,beiJingTime,sendData);  //gps��Ϣ�ķ���
//			UltrasonicWave(num);								//��ȡ����������
//			if(direction_flag)
//			{
//					angle=getAngle();
//					direction_flag=0;
//			}
//			printf("%d\r\n",angle);
//			angle=0;
//			num[AVER_NUM-1]=angle;
//			for(i=0;i<AVER_NUM;i++)				//num�����һ�������������ͷ�λ��Ϣ
//			{
//				printf("%d",num[i]);
//			}
			delayMs(600);
	}
}

/*********************************************END OF FILE**********************/


