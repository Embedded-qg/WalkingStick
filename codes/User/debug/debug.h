#ifndef DEBUG_H_
#define DEBUG_H_
#include "stm32f10x.h"
#include "bsp_usart.h"

//#define DEBUG  0
#define  DEBUG_ON_OFF  0 

void CloseDebug(void);   //�رյ���
void OnDebug(void);    //�򿪵���
/**
////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 1       //1�򿪵��ԡ�0�ر�
//////////////////////////////
*/

#define  DEBUG_USART3_USART1_ENABLE 0      //ͨ������3��ӡ����1�������͵�����


#define p_debug(restrict,args...)  do{if(DEBUG_ON_OFF){my_printf("\r\ndebug: "restrict"\r\n",##args);}}while(0)
#define p_err(restrict,args...)  do{if(DEBUG_ON_OFF){my_printf("file:%s line:%d: \r\n",__FILE__,__LINE__);my_printf(restrict,##args);}}while(0)

void my_printf(const char *format,...); 

#endif
