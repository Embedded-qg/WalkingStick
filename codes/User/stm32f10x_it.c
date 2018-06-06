/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_usart1.h"
#include <stdio.h>
#include "bsp_GeneralTim.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "gps.h" 

extern void TimingDelay_Decrement(void);
extern uint8_t direction_flag;
extern _SaveData Save_Data;

////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 0
#endif
//////////////////////////////

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
  printf("\r\n hardfault err\r\n");
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void SysTickDelayTime_Counter(void);
void SysTick_Handler(void)
{
	SysTickDelayTime_Counter();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{


}








/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */

void GENERAL_TIM_INT_FUN(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	if ( TIM_GetITStatus ( GENERAL_TIM, TIM_IT_Update) != RESET )               
	{	
//		TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
		TIM_ICUserValueStructure2.Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( GENERAL_TIM, TIM_FLAG_Update ); 		
	}

//��һ��������ģ�������ز����ж�
	if ( TIM_GetITStatus (GENERAL_TIM, TIM_IT_CC3 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( GENERAL_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC3PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure.Capture_CcrValue = TIM_GetCapture3 (GENERAL_TIM);
			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC3PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
            p_debug("tim2, cc0\r\n");
            			
		}
		TIM_ClearITPendingBit (GENERAL_TIM,TIM_IT_CC3);	    
	}	

//�ڶ���������ģ�������ز����ж�
	else if ( TIM_GetITStatus (GENERAL_TIM, TIM_IT_CC4 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( GENERAL_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC4PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure.Capture_CcrValue = 
			TIM_GetCapture4 (GENERAL_TIM);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC4PolarityConfig(GENERAL_TIM, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
             p_debug("tim2, cc1\r\n");			
		}
		TIM_ClearITPendingBit (GENERAL_TIM,TIM_IT_CC4);	    
	}	
}

void TIM3_IRQHandler(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )               
	{	
//		TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
		TIM_ICUserValueStructure2.Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update ); 		
	}
//printf("tim3\r\n");
//������������ģ�������ز����ж�
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC1 ) != RESET)
	{  	
//printf("cc1\r\n");		
		// ��һ�β���
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture1 (TIM3);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
			p_debug("tim3, cc1\r\n");
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);	    
	}	
//���ĸ�������ģ�������ز����ж�
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC2 ) != RESET)
	{	
		// ��һ�β���
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure2.Capture_CcrValue = TIM_GetCapture2 (TIM3);
			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
			p_debug("tim3, cc2\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC2);	    
	}	

//�����������ģ�������ز����ж�
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC3 ) != RESET)
	{	
		// ��һ�β���
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture3(TIM3);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;
			p_debug("tim3, cc3\r\n");
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC3);	    
	}	
//������������ģ�������ز����ж�
	else if ( TIM_GetITStatus (TIM3, TIM_IT_CC4 ) != RESET)
	{	
		// ��һ�β���
		if ( TIM_ICUserValueStructure2.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure2.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure2.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure2.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure2.Capture_CcrValue = 
			TIM_GetCapture4(TIM3);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure2.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure2.Capture_FinishFlag = 1;	
           p_debug("tim3, cc4\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC4);	    
	}	
}

/**
  * @brief  �ⲿ�ж�0�����ڼ�ⰴ��
  * @param  None
  * @retval None
  */

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{		
		direction_flag=1;												//��������־λ���б��
		EXTI_ClearITPendingBit(EXTI_Line0);     //����жϱ�־λ
	}  
}

/**
  * @brief  ����2�жϣ�����gps
  * @param  None
  * @retval None
  */

void USART2_IRQHandler(void)                	//����2�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART2);						//��ȡ���յ�������
	
	if(Res == '$')
	{
		point1 = 0;	
	}
		

	  USART_RX_BUF[point1++] = Res;

	if(USART_RX_BUF[0] == '$' && USART_RX_BUF[4] == 'M' && USART_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
	{
		if(Res == '\n')									   
		{
			memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //��GPS���ݳ�ʼ��
			memcpy(Save_Data.GPS_Buffer, USART_RX_BUF, point1); 		 //����ȡ�����ݱ��浽GPS������
			Save_Data.isGetData = true;
			point1 = 0;
			memset(USART_RX_BUF, 0, USART_REC_LEN);      						 //�����ڽ����������				
		}	
	}
	
	if(point1 >= USART_REC_LEN)
	{
		point1 = USART_REC_LEN;
	}	 		 
   } 
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
