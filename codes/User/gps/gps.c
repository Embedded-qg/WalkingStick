#include "gps.h"
#include "gprs.h"
u16 point1 = 0;
_SaveData Save_Data;
GPSData SendGPS={0,0};
char USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       //����״̬���	  
void CLR_Buf(void)                           // ���ڻ�������
	
{
		memset(USART_RX_BUF, 0, USART_REC_LEN);      //���
		point1 = 0;                    
}

void clrStruct()
{
		Save_Data.isGetData = false;
		Save_Data.isParseData = false;
		Save_Data.isUsefull = false;
		memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
		memset(Save_Data.UTCTime, 0, UTCTime_Length);
		memset(Save_Data.latitude, 0, latitude_Length);
		memset(Save_Data.N_S, 0, N_S_Length);
		memset(Save_Data.longitude, 0, longitude_Length);
		memset(Save_Data.E_W, 0, E_W_Length);
}


void USART2_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		/* config USART2 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* USART2 GPIO config */
		/* Configure USART2 Tx (PA.02) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
				
		/* Configure USART2 Rx (PA.03) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
		/* USART2 mode config */
		USART_InitStructure.USART_BaudRate = 9600;                //GPSģ��Ĭ��ʹ�ò����ʣ�9600
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		USART_Init(USART2, &USART_InitStructure); 
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART2, ENABLE);

		CLR_Buf();									//��մ��ڻ���
}

void errorLog(int num)
{
	
	while (1)
	{
	  	my_printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer(void)
{
	char *subString;			//���ڷֽ�
	char *subStringNext;
	char i = 0;
	int a,b;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		my_printf("**************\r\n");
		my_printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, 	 subString, subStringNext - subString);break;				//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, 		 	 subString, subStringNext - subString);break;				//��ȡ���ݳɹ���־λ
						case 3:memcpy(Save_Data.latitude,  subString, subStringNext - subString);break;				//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, 			 subString, subStringNext - subString);break;				//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;				//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, 			 subString, subStringNext - subString);break;				//��ȡE/W
						
						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')					//�ж��Ƿ�������ݳɹ�
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}


		}
						a=(int)(atof(Save_Data.longitude)/100);
						b=(int)(atof(Save_Data.latitude)/100);
						SendGPS.lo=(atof(Save_Data.longitude)-100*a)/60+a;
						SendGPS.la=(atof(Save_Data.latitude)-100*b)/60+b;
//						GPRS_Send_GPS(SendGPS.lo, SendGPS.la);	//ʹ��GPRS���͵�ǰλ������
						my_printf("lo f% ,la f%\r\n",SendGPS.lo,SendGPS.la);
	}
}

void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
//		printf("Save_Data.UTCTime = ");
//		printf(Save_Data.UTCTime);
//		printf("\r\n");

//		if(Save_Data.isUsefull)
//		{
//			Save_Data.isUsefull = false;
//			printf("Save_Data.latitude = ");
//			printf(Save_Data.latitude);
//			printf("\r\n");


//			printf("Save_Data.N_S = ");
//			printf(Save_Data.N_S);
//			printf("\r\n");

//			printf("Save_Data.longitude = ");
//			printf(Save_Data.longitude);
//			printf("\r\n");

//			printf("Save_Data.E_W = ");
//			printf(Save_Data.E_W);
//			printf("\r\n");
		}
		else
		{
			my_printf("GPS DATA is not usefull!\r\n");
		}
		
	
}


