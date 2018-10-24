#include "shake.h"
/**
  * @brief  ����TIM�������PWMʱ�õ���I/O,����ʹ�õ���TIM2
  * @param  ��
  * @retval ��
  */
static void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*  clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE); 

  /* ���ú������õ������� */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init( GPIOA, &GPIO_InitStructure );
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����TIM�����PWM�źŵ�ģʽ�������ڡ�����
  * @param  ��
  * @retval ��
  */

static void TIM2_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	
	/* ����TIM3CLK ʱ�� */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM2, ENABLE ); 

  /* ������ʱ������ */		  
  TIM_TimeBaseStructure.TIM_Period = 9999;       							  //����ʱ����0������ TIM_Period+1 ��Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    							//����Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
  TIM_OCInitStructure.TIM_Pulse = 9999;				 						  		//���ó�ʼPWM������Ϊ0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

  TIM_OC2Init ( TIM2, &TIM_OCInitStructure );	 									//ʹ��ͨ��2
	

  TIM_OC2PreloadConfig ( TIM2, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 										//ʹ��TIM���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   										//ʹ�ܶ�ʱ��	
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);										//ʹ��update�ж�
		
	NVIC_Config_PWM();																					//�����ж����ȼ�		
	
}

/**
  * @brief  TIM �����Ƴ�ʼ��
  *         ����PWMģʽ��GPIO
  * @param  ��
  * @retval ��
  */
void TIM2_Breathing_Init(void)
{
	TIM2_GPIO_Config();
	TIM2_Mode_Config();	
}


/**
  * @brief  TIM �����Ƴ�ʼ��
  *         ����PWMģʽ��GPIO
  * @param  degree,���������𶯵�Ƶ�ʣ���Ϊ1-4����4����Ƶ�����
  * @retval ��
  */
void AdjustVibrationFrequencyWalking(char degree)
{
	switch(degree)
	{
		case Zero:
			TIM2->CCR2 = 9999;
			break;	
		case First:
			TIM2->CCR2 = 7500;
			break;		
		case Second:
			TIM2->CCR2 = 5000;
			break;
		case Third:
			TIM2->CCR2 = 2500;
			break;
		case Forth:
			TIM2->CCR2 = 0;
			break;
		
//				case Forth:
//			TIM2->CCR2 = 9999;
//			break;	
//		case Third:
//			TIM2->CCR2 = 7500;
//			break;		
//		case Second:
//			TIM2->CCR2 = 5000;
//			break;
//		case First:
//			TIM2->CCR2 = 2500;
//			break;
//		case Zero:
//			TIM2->CCR2 = 0;
//			break;
	}
}
