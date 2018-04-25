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
//	
//	int angle;
//	int num[AVER_NUM];
//	delayInit();
//		EXTI_PA0_Config(); //��λ�İ���
//	UltrasonicWave_Configuration();
////	SysTick_Init();                   //��શ�ʱ����ʼ��
//	USART1_Config(38400);               //����1��ʼ��,���ڵ���
////	NVIC_Configuration();
////	USART3_Config(9600);              //����3��ʼ��������gprs
////	GPS_Config();                     //����2��ʼ��������gps
////	gprs_init("120.78.193.178","10000");
//	GENERAL_TIM_Init();
//////	GPS_GPRS();
//	  while(1)           
//	{
//		UltrasonicWave(num);
//	//	printf("S00000TES11114TE");
//		delayMs(60);
//	}
//  I2C_GPIO_Config();
//  InitLSM303D();
//  while(1)
//  {
//		if(direction_flag)
//		{
//				getAngle();
//				direction_flag=0;;
//		}
//		printf("�����\n");
//		printf("direction_flag = %d\n",direction_flag);		
//		delayMs(1000);
//						
//  }


	int a,b;
	nmeaINFO GpsInfo;      //GPS�ṹ��
	nmeaTIME beiJingTime;  //����ʱ��
	char sendData[30]; //��Ҫ���͵�����
	
	SysTick_Init();        //��શ�ʱ����ʼ��
	USART1_Config(115200); //����1��ʼ��,���ڵ���
	USART3_Config(9600); //����3��ʼ��������gprs
	GPS_Config();          //����2��ʼ��������gps
	gprs_init("47.106.74.67","10001");
	
	while(1)
	{
		GpsInfo = Get_GPS_Data();//GPS����
		GMTconvert(&GpsInfo.utc,&beiJingTime,8,1);//�Խ�����ʱ�����ת����ת���ɱ���ʱ��
		DEBUG_GPS("γ�ȣ�%lf,����%lf\r\n\r\n",GpsInfo.lat,GpsInfo.lon);
		a=(int)(GpsInfo.lon/100);
		b=(int)(GpsInfo.lat/100);
		GpsInfo.lon=(GpsInfo.lon-100*a)/60+a;
		GpsInfo.lat=(GpsInfo.lat-100*b)/60+b;
		sprintf(sendData,"lo:%f,la:%f\r\n",GpsInfo.lon,GpsInfo.lat);//��Ҫ���͵�����
		DEBUG_GPS("ʱ�䣺%d-%d-%d %d:%d:%d\r\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
		DEBUG_GPS("γ�ȣ�%lf,����%lf\r\n\r\n",GpsInfo.lat,GpsInfo.lon);

		/*����TCP����*/
		if(sim900a_gprs_send(sendData) == 1)//��������
		{
				printf("���ͳɹ���\r\n");
				SIM900A_CLEAN_RX(); //������ڻ�����
				Delay_ms(2000);
				printf("����λ��Ϊ%s\r\n",SIM900A_RX());//������		
		}
		else
		{
			printf("����ʧ�ܣ�\r\n\r\n");
			gprs_init("47.106.74.67","10001");
		}	
	}

}

/*********************************************END OF FILE**********************/


