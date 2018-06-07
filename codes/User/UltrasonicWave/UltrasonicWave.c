#include "UltrasonicWave.h"
#include "debug.h"

////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#define DEBUG_ON_OFF 1
#endif
//////////////////////////////       

int UltrasonicWave_Distance[ULTR_NUM];      //������ľ���    
int8_t  MEASURE_FLAG = 0;          //��������־
/*
 * ��������dealTIM_ICUserValueStructureData
 * ����  ������׽����ʱ������ת��Ϊ����
 * ����  ��TIM_ICUserValueStructurex ��TIM_ICUserValueStructure1-6
 * ���  ����	
 */
static void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex)
{

	uint32_t time;
	int i;
	// ����ߵ�ƽʱ��ļ�������ֵ
	i = TIM_ICUserValueStructurex.Capture_CCx - 1;
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	UltrasonicWave_Distance[i] = time * 340 / 2  * 100 / TIM_PscCLK ;
	// ��ӡ�ߵ�ƽ����ʱ��
	printf( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n��%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );	
}



/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬���жϣ�������һ��>10us������
 * ����  ��port = TRIG_PORTX ,pin = TRIG_PINX
 * ���  ����	
 */
void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
  delayUs(10);		                      //��ʱ10US����
  GPIO_ResetBits(port,pin);

}

/****************************************************************************
* ��    �ƣ�void UltrasonicWave(void *arg)
* ��    �ܣ�����������߳�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void UltrasonicWave(int measureNum)
{
	int i= 0;	
	for( i = 0; i < ULTR_NUM; i++ )
	{
		if( TIM_ICUserValueStructure[i].Capture_FinishFlag == 1 )  
		{	
			dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[i]);
			TIM_ICUserValueStructure[i].Capture_FinishFlag = 0;
		}		
	}
	
	switch(measureNum)          //��ʼ��࣬����һ��>10us�����壬
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
		case 2: UltrasonicWave_StartMeasure(TRIG_PORT3,TRIG_PIN3); break;
		case 3: UltrasonicWave_StartMeasure(TRIG_PORT4,TRIG_PIN4); break;
		case 4: UltrasonicWave_StartMeasure(TRIG_PORT5,TRIG_PIN5); break;
	}	
}


/******************* (C) 1209 Lab *****END OF FILE************/
