#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"


/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM5

#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM_PERIOD            0XFFFF
#define            GENERAL_TIM_PSC              (72-1)
#define            TIM_PscCLK 					 (72000000/(GENERAL_TIM_PSC+1))
// �ж���غ궨��
#define            GENERAL_TIM_IT_CCx            TIM_IT_CC2
#define            GENERAL_TIM_IRQ               TIM4_IRQn
#define            GENERAL_TIM_INT_FUN           TIM4_IRQHandler

// ��ȡ����Ĵ���ֵ�����궨��
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture2
// �����źż��Ժ����궨��
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC2PolarityConfig

// ��������ʼ����
#define            GENERAL_TIM_STRAT_ICPolarity                TIM_ICPolarity_Rising
// �����Ľ�������
#define            GENERAL_TIM_END_ICPolarity                  TIM_ICPolarity_Falling



// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	int16_t  Capture_CcrValue;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // �Զ���װ�ؼĴ������±�־ 
	uint8_t   Capture_CCx;          //��Ӧͨ����־
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure[];

/**************************��������********************************/
void GENERAL_TIM_Init(void);





#define	TRIG_PORT1     			  GPIOC		//TRIG       
#define	TRIG_PIN1      			  GPIO_Pin_0   //TRIG       

#define	TRIG_PORT2     			  GPIOC		//TRIG       
#define	TRIG_PIN2      			  GPIO_Pin_1   //TRIG     

#define	TRIG_PORT3     			  GPIOC		//TRIG       
#define	TRIG_PIN3      			  GPIO_Pin_2   //TRIG       

#define	TRIG_PORT4     			  GPIOC		//TRIG       
#define	TRIG_PIN4      			  GPIO_Pin_3   //TRIG     

#define	TRIG_PORT5     			  GPIOC		//TRIG       
#define	TRIG_PIN5     			  GPIO_Pin_4   //TRIG 

#define	TRIG_PORT6     			  GPIOC		//TRIG       
#define	TRIG_PIN6      			  GPIO_Pin_5   //TRIG 


#define RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOC

#endif	/* __BSP_GENERALTIME_H */


