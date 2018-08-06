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
#include "bsp_usart.h"
#include <string.h>
#include "bsp_SysTick.h"
#include "bsp_angle.h"
#include "bsp_exti.h"
#include "gps.h"
#include "timer.h"
#include "bsp_usart4.h"
#include "gprs.h"
#include "bsp_usart.h"
#include "debug.h"


extern char *pdu_content ;
extern int8_t  MEASURE_FLAG;
uint8_t direction_flag;
extern int UltrasonicWave_Distance[];     

//GPRS��������ʼ������
static void Periph_GPRS_Init()
{
	UART4_Init_Config(115200);//GPRSʹ�ô���4��ʼ��
	SysTick_Init_Config();//�δ�ʱ����ʼ��
	Timer2_Init_Config();	//��ʱ��2��ʼ��
}

void delay_0(u32 time)
{
  while(time--);
}

int main(void)
{
	int ret;
	int angle=0; 
	int i=0;
	int num[AVER_NUM];									         //���泬��������
	
	int sendByBlueTooth[MAX_SEND];		           //���ͱ��������Լ���λ����
	int XX[5]={1,2,3,4,5};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);       //�ж����ȼ�����
	
	MEASURE_FLAG=0;
//	delayInit();												       //��ʼ���δ�ʱ��
    usart3Config();
	USART1_Config(38400);                        //����1��ʼ��,��������
	I2C_GPIO_Config();									         //I2CͨѶ�ĳ�ʼ��
    InitLSM303D();											         //��λģ��ĳ�ʼ��
//	SysTick_Init();                  	         //��શ�ʱ����ʼ��
	WaveConfig();

	USART2_Config();										         //����2������gps
	EXTI_PA0_Config(); 									         //��λ�����������жϳ�ʼ��
	Periph_GPRS_Init();	//GPRS��������ʼ������   

	
	DEBUG("ϵͳ����......");
	my_printf("start debug\r\n");
	printf("test\r\n");
	 while(1)           
	{

//			if(MEASURE_FLAG)
//			{
//					SendGlasses(UltrasonicWave_Distance,ULTR_NUM);
//					MEASURE_FLAG=0;
//			}


//		GPRS_Send_GPS(22.2, 33.3);	//ʹ��GPRS����λ������
//		 GPRS_Send_help();					//ʹ��GPRS��������ź�
//		i++;
//		if(i>100)
//		{
//			i=0;
//		}
//		parseGpsBuffer();									
//		printGpsBuffer();

//			UltrasonicWave(num);								//��ȡ����������
			if(direction_flag)
			{
					angle=getAngle();
					direction_flag=0;
					DEBUG("��ǰ��λΪ��");
					DEBUG("%d",angle);
			}
			
			
			
//			angle=0;
//			printf("%d",angle);
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


