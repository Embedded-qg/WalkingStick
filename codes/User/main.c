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
extern int HelpFlag;
extern GPSData SendGPS;
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
	char angle=0; 
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

	
	my_printf("ϵͳ����......");
	
	
	 while(1)           
	{
		parseGpsBuffer();									
//		printGpsBuffer();
//			UltrasonicWave(num);								//��ȡ����������
			if(direction_flag)										//�Ѿ����Ƕȵ������
			{
					angle=getAngle();
					direction_flag=0;
					my_printf("��ǰ��λΪ��");
					my_printf("%c",angle);
				SendAngle(angle);										//���ͷ�λ��Ϣ
			}
			if(HelpFlag)
			{
					GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//ʹ��GPRS���͵�ǰλ������
				 GPRS_Send_help();					//ʹ��GPRS��������ź�
					HelpFlag=0;
			}
//		my_printf("running\r\n");
//			UltrasonicWave(num);								//��ȡ����������


	}
}

/*********************************************END OF FILE**********************/


