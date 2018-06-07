#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_SysTick.h"
#include "timer.h"
#include "string.h"
#include "GA6_module_errors.h"
#include "bsp_usart4.h"
#include <stdio.h>
#include "gprs.h"

#define Buf4_Max 	  100 //����2���泤��
#define STABLE_TIMES  10  //�ȴ�ϵͳ�ϵ����ȶ�

/*************	���س�������	**************/

/*************  ���ر�������	**************/
char Uart4_Buf[Buf4_Max];//����2���ջ���
u8 First_Int = 0;
char error_result[20];

char server_ip_port_cmd[45];


/*************	���غ�������	**************/
void CLR_Buf_4(void);     //�������2���ջ���
u8 Wait_CREG(u8 query_times);    //�ȴ�ģ��ע��ɹ�
u8 Find(char *a);

/*************  �ⲿ�����ͱ�������*****************/

int check_status(void)
{
	int ret;
	char esc_char[2];
	
	esc_char[0] = 0x1B;//�˳��ַ�
	esc_char[1] = '\0';
	
	ret = UART4_Send_AT_Command("AT","OK",5,50);//����ͨ���Ƿ�ɹ�
	if(ret == 0)
	{
		UART4_SendString(esc_char);//��һ����>״̬����ô�÷���һ��ESC�ַ�
		return COMMUNITE_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CPIN?","READY",2,50);//��ѯ���Ƿ����
	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	
	ret = Wait_CREG(3);//��ѯ���Ƿ�ע�ᵽ����
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
    
    ret = UART4_Send_AT_Command("ATE0","OK",2,50);//�رջ��Թ���
	if(ret == 0)
	{
		return EAT0_ERROR;
	}
    
	return 1;
}

/***************************************************************
ע����Ȼ����Է�������ֵ����ȷ����������һ������ָ�����ʧ���ˡ�
****************************************************************/
int send_data_to_server(char *server_IP_and_port,char *content)
{
	u8 ret;
	char end_char[2];
	
	
	end_char[0] = 0x1A;//�����ַ�
	end_char[1] = '\0';
	
	ret = UART4_Send_AT_Command("AT+CIPSTATUS","CONNECT OK",3,50*2);//��ѯ����״̬
	if(ret == 1)//˵����������������״̬
	{
		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);//������������
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;
	}
	else
	{
		ret = UART4_Send_AT_Command("AT+CGATT=1","OK",3,50*7);//��������
		if(ret == 0)
		{
			return AT_CGATT_ERROR;
		}
		
		ret = UART4_Send_AT_Command("AT+CGACT=1,1","OK",3,50*2);//��������
		if(ret == 0)
		{
			return AT_CGATT1_ERROR;
		}

		memset(server_ip_port_cmd,'\0',45);
		strcpy(server_ip_port_cmd,"AT+CIPSTART=\"TCP\",");
		strcat(server_ip_port_cmd,server_IP_and_port);
		
		ret = UART4_Send_AT_Command(server_ip_port_cmd,"CONNECT OK",3,50*2);//���ӷ�����
		if(ret == 0)
		{
			return AT_CIPSTART_ERROR;
		}
		
		delay_ms(50);
		UART1_SendString(Uart4_Buf);//MY �״����ӣ���ʾ���������������ӳɹ���Ϣ
		
		
		ret = UART4_Send_AT_Command("AT+CIPSEND",">",3,50);//��ʼ��������
		if(ret == 0)
		{
			return AT_CIPSEND_ERROR;
		}
		
		UART4_SendString(content);
		ret = UART4_Send_AT_Command_End(end_char,"SEND OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ��������ʱ����ܽϳ�
			
		if(ret == 0)
		{
			return END_CHAR_ERROR;
		}
		
		return 1;

	}
}


/*******************************************************************************
* ������  : USART2_IRQHandler
* ����    : ����1�жϷ������
* ����    : ��
* ����    : �� 
* ˵��    : 
*******************************************************************************/
void UART4_IRQHandler(void)                	
{
	u8 Res=0;
	Res = USART_ReceiveData(UART4);
	Uart4_Buf[First_Int] = Res;  	  //�����յ����ַ����浽������
	First_Int++;                	  //����ָ������ƶ�
	if(First_Int > Buf4_Max)       	  //���������,������ָ��ָ�򻺴���׵�ַ
	{
		First_Int = 0;
	}
} 	

/*******************************************************************************
* ������  : TIM2_IRQHandler
* ����    : ��ʱ��2�ж϶Ϸ�����
* ����    : ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void TIM2_IRQHandler(void)   //TIM2�ж�
{
	static u8 flag =1;

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIM2�����жϱ�־ 
	
		if(flag)
		{
			//LED4_ON(); 
			flag=0;
		}
		else
		{
			//LED4_OFF(); 
			flag=1;
		}
	}	
}

/*******************************************************************************
* ������ : CLR_Buf
* ����   : �������4��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf_4(void)
{
	u16 k;
	for(k=0;k<Buf4_Max;k++)      //��������������
	{
		Uart4_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
u8 Wait_CREG(u8 query_times)
{
	u8 i;
	u8 k;
	u8 j;
	i = 0;
	CLR_Buf_4();
	while(i == 0)        			
	{

		UART4_Send_Command("AT+CREG?");
		delay_ms(100); 
		
		for(k=0;k<Buf4_Max;k++)      			
		{
			if((Uart4_Buf[k] == '+')&&(Uart4_Buf[k+1] == 'C')&&(Uart4_Buf[k+2] == 'R')&&(Uart4_Buf[k+3] == 'E')&&(Uart4_Buf[k+4] == 'G')&&(Uart4_Buf[k+5] == ':'))
			{
					 
				if((Uart4_Buf[k+7] == '1')&&((Uart4_Buf[k+9] == '1')||(Uart4_Buf[k+9] == '5')))
				{
					i = 1;
					return 1;
				}
				
			}
		}
		j++;
		if(j > query_times)
		{
			return 0;
		}
		
	}
	return 0;
}

/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

u8 Find(char *a)
{ 
	if(strstr(Uart4_Buf, a)!=NULL)
	{
		return 1;
	}	
	else
	{
		return 0;
	}
		
}


/*******************************************************************************
* ������ : GPRS_Send_help
* ����   : GPRS_Send_help����
* ����   : 
* ���   : 
* ����   : 
* ע��   : ����2������GA6ģ��ͨ�ţ�����1���ڴ��ڵ��ԣ�
*******************************************************************************/
void GPRS_Send_help(void)
{
	u8  i;
	int  ret;
		
	UART1_SendString("GPRS����help��������.......................\r\n");
	
	//���ھ����Ƿ�ɾ��
	for(i = 0;i < STABLE_TIMES;i++)
	{
		delay_ms(50);
	}
	
	memset(error_result,'\0',20);

	delay_ms(50);
		
	ret = check_status();
	if(ret == 1)
	{
		ret = send_data_to_server("\"47.106.74.67\",10001","help");//�������ݵ�������					
	}
	
	if(ret == 1)
	{
		sprintf(error_result,"�ɹ����͵�server\r\n");
		UART1_SendString(error_result);
		delay_ms(50);
		UART1_SendString("�յ��ظ���\r\n");  //received:��������ݲ��ǻظ�����������
		UART1_SendString(Uart4_Buf);
		UART1_SendString("\r\n");
		
	}
	else //���ݷ���ʧ�ܣ���ʱ��������ʱ��������µ�ʧ�ܣ������ͶϿ����ӣ�Ȼ��Ϳ��Լ�����������
	{
		sprintf(error_result,"������� : %d\r\n",ret);
		UART1_SendString(error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//�ر�����
		
	}
	

}

void GPRS_Send_GPS(float lo, float la)
{
	u8  i;
	int  ret;
	char sendData[30]; //��Ҫ���͵�����
		
	UART1_SendString("GPRS�������깦������.......................\r\n");
	
	for(i = 0;i < STABLE_TIMES;i++)
	{
		delay_ms(50);
	}
	memset(error_result,'\0',20);
	memset(sendData,'\0',30);

	delay_ms(50);
		
	ret = check_status();
	if(ret == 1)
	{
		sprintf(sendData,"lo:%f,la:%f\r\n",lo, la);//��Ҫ���͵�����
		ret = send_data_to_server("\"47.106.74.67\",10001",sendData);//�������ݵ�������					
	}
	
	if(ret == 1)
	{
		sprintf(error_result,"�ɹ����͵�server\r\n");
		UART1_SendString(error_result);
		delay_ms(50);
		UART1_SendString("�յ��ظ���\r\n");  //received:��������ݲ��ǻظ�����������
		UART1_SendString(Uart4_Buf);
		UART1_SendString("\r\n");
		
	}
	else //���ݷ���ʧ�ܣ���ʱ��������ʱ��������µ�ʧ�ܣ������ͶϿ����ӣ�Ȼ��Ϳ��Լ�����������
	{
		sprintf(error_result,"������� : %d\r\n",ret);
		UART1_SendString(error_result);
		UART4_Send_AT_Command("AT+CIPCLOSE","OK",3,150);//�ر�����
		
	}
}

