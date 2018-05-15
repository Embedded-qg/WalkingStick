/***************************************************************************************
 *	FileName					:	   delay.c
 *	CopyRight					:
 *	ModuleName					:	
 *
 *	CPU							:
 *	RTOS						:
 *
 *	Create Data					:	
 *	Author/Corportation			:	
 *
 *	Abstract Description		:	 ��ʱ����
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	
 *
 ***************************************************************************************/


/**************************************************************
*	Include File Section
**************************************************************/
#include "bsp_delay.h"

/**************************************************************
*	Macro Define Section
**************************************************************/
 
/**************************************************************
*	Struct Define Section
**************************************************************/


/**************************************************************
*	Prototype Declare Section
**************************************************************/


/**************************************************************
*	Global Variable Declare Section
**************************************************************/


/**************************************************************
*	File Static Variable Define Section
**************************************************************/
static uint8_t  fac_us=0;							//us��ʱ������			   
static uint16_t fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
	

/**************************************************************
*	Function Define Section
**************************************************************/

/**
*  @name  : void delayInit()
*	@description : ��ʼ����ʱ����
 *	@param		��none
 *	@return		:none
 *  @notice   : none
 */
 void delayInit()
{
//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //ѡ���ⲿʱ�ӣ�  HCLK/8
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  
	fac_ms=(u16)fac_us*1000;					//����ÿ��ms��Ҫ��systickʱ����
}

/**
 *  @name    void delayUs( uint32_t t )
 *	@description   us��ʱ
 *	@param		t ��ʱtus	
 *	@return		none
 *  @notice   t us<=��2^24��/fac_us
 */
void delayUs( uint32_t t )
{
	uint32_t temp;

	SysTick->LOAD=t*fac_us; 					//ʱ�����		
    SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//������ѭ�����ȴ�ʱ�䵽��  
	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	
}

/**
 *  @name   void delayMs(uint16_t t)
 *	@description    ms��ʱ
 *	@param		t ��ʱtms
 *	@return		none
 *  @notice  �� �����ʱ t<=0xffffff*8*1000/SYSCLK
 */
void delayMs(uint16_t t)
{
	uint32_t temp;		   
	SysTick->LOAD=(u32)t*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  

	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   

	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	 	
}
 
