#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_usart.h"
#include "waveConfig.h"

//#define ONLY_WALKINGSTICK             //���ȵ�������

#define ULTR_NUM        5              //����������
#define AVER_NUM        ULTR_NUM+1            //����������+һλ��λģ��

extern int8_t  IT_TAG;
extern int UltrasonicWave_Distance[];
int getRange(void);
int minusDistance(int distance);
int addDistance(int distance);
void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave(int);
int IsFinishMeasure(void);




#endif /* __UltrasonicWave_H */

