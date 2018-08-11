
#include "waveConfig.h"
#include "UltrasonicWave.h"
// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure[ULTR_NUM];

// �ж����ȼ�����
static void GENERAL1_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL1_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL2_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL2_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL1_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���벶��ͨ�� GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH1_PORT, &GPIO_InitStructure);	
	
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH2_PORT, &GPIO_InitStructure);	

  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH3_PORT, &GPIO_InitStructure);	
	
  RCC_APB2PeriphClockCmd(GENERAL1_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL1_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL1_TIM_CH4_PORT, &GPIO_InitStructure);	
}


static void GENERAL2_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���벶��ͨ�� GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(GENERAL2_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL2_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL2_TIM_CH3_PORT, &GPIO_InitStructure);	
	
//  RCC_APB2PeriphClockCmd(GENERAL2_TIM_CH4_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Pin =  GENERAL2_TIM_CH4_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GENERAL2_TIM_CH4_PORT, &GPIO_InitStructure);	
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void GENERAL1_TIM_Mode_Config(void)
{
		TIM_ICInitTypeDef TIM_ICInitStructure;
	 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL1_TIM_APBxClock_FUN(GENERAL1_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL1_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL1_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL1_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_1;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_2;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_3;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = GENERAL1_TIM_CHANNEL_4;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL1_TIM, &TIM_ICInitStructure);	
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(GENERAL1_TIM, TIM_FLAG_Update|GENERAL1_TIM_IT_CC1|GENERAL1_TIM_IT_CC2|GENERAL1_TIM_IT_CC3|GENERAL1_TIM_IT_CC4);	
  // �������ºͲ����ж�  
	TIM_ITConfig (GENERAL1_TIM, TIM_IT_Update | GENERAL1_TIM_IT_CC1|GENERAL1_TIM_IT_CC2|GENERAL1_TIM_IT_CC3|GENERAL1_TIM_IT_CC4, ENABLE );
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL1_TIM, ENABLE);
}


static void GENERAL2_TIM_Mode_Config(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL2_TIM_APBxClock_FUN(GENERAL2_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL2_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL2_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL2_TIM, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL2_TIM_CHANNEL_3;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
//	TIM_ICInit(GENERAL2_TIM, &TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_Channel = GENERAL2_TIM_CHANNEL_4;    // ���벶���źŵļ�������
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL2_TIM, &TIM_ICInitStructure);	
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(GENERAL2_TIM, TIM_FLAG_Update|GENERAL2_TIM_IT_CC3);	
  // �������ºͲ����ж�  
	TIM_ITConfig (GENERAL2_TIM, TIM_IT_Update|GENERAL2_TIM_IT_CC3, ENABLE );
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL2_TIM, ENABLE);
}

void GENERAL_TIM_Init(void)
{
	GENERAL1_TIM_GPIO_Config();
	GENERAL1_TIM_NVIC_Config();
	GENERAL1_TIM_Mode_Config();		
	
	GENERAL2_TIM_GPIO_Config();
	GENERAL2_TIM_NVIC_Config();
	GENERAL2_TIM_Mode_Config();		
}

/*********************************************END OF FILE**********************/


void TIM5_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM5, //TIM5
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx����					 
}



/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 * ����  ����
 * ���  ����	
 */
static void UltrasonicWave_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN1;					
	GPIO_Init(TRIG_PORT1, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN2;					
	GPIO_Init(TRIG_PORT2, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN3;					
	GPIO_Init(TRIG_PORT3, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN4;					
	GPIO_Init(TRIG_PORT4, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN5;					
	GPIO_Init(TRIG_PORT5, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN6;					
	GPIO_Init(TRIG_PORT6, &GPIO_InitStructure);			
}

//��๦�ܳ�ʼ��
void WaveConfig()
{
	UltrasonicWave_Configuration();			//��ʼ������
	GENERAL_TIM_Init();	
	TIM5_Init();
}
