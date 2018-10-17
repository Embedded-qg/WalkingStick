/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-15
  * @brief   I/O���ж�Ӧ��bsp
  ******************************************************************************
  */
  
#include "bsp_exti.h"

 /**
  * @brief  �ⲿ�ж�0�жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration_PA0(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;  //�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ���� PA0 Ϊ���жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_PA0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration_PA0();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	          // ��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�Ͻ����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
/*********************************************END OF FILE**********************/
