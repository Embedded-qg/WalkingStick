#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include "debug.h"
#include "bsp_usart.h"


static void printch(const char ch)   //����ַ�
{  
    Usart3Send(ch);  
}  
 
 
static void printintraw(const int dec)     //���������
{  
    if(dec == 0)  
    {  
        return;  
    }  
    printintraw(dec / 10);  
    Usart3Send((char)(dec % 10 + '0'));  
}  

static void printint(const int dec)     //���������
{  
    if(dec == 0)  
    {  
		Usart3Send('0');  
        return;  
    }  
    printintraw(dec / 10);  
    Usart3Send((char)(dec % 10 + '0'));  
}  
 
 
static void printstr(const char *ptr)        //����ַ���
{  
    while(*ptr)  
    {  
        Usart3Send(*ptr);  
        ptr++;  
    }  
}  
 
 
static void printfloat(const float flt)     //�����������С�����5λ��������
{  
    int tmpint = (int)flt;  
    int tmpflt = (int)(100000 * (flt - tmpint));  
    if(tmpflt % 10 >= 5)  
    {  
        tmpflt = tmpflt / 10 + 1;  
    }  
    else  
    {  
        tmpflt = tmpflt / 10;  
    }  
    printint(tmpint);  
    Usart3Send('.');  
    printint(tmpflt);  
 
}  

 void my_printf(const char *format,...)  
{  
    va_list ap;  
    va_start(ap,format);     //��apָ���һ��ʵ�ʲ����ĵ�ַ
    while(*format)  
    {  
        if(*format != '%')  
        {  
            Usart3Send(*format);  
            format++;  
        }  
        else  
        {  
            format++;  
            switch(*format)  
            {  
                case 'c':  
                {  
                    char valch = va_arg(ap,int);  //��¼��ǰʵ���������ڵ�ַ
                    printch(valch);  
                    format++;  
                    break;  
                }  
                case 'd':  
                {  
                    int valint = va_arg(ap,int);  
                    printint(valint);  
                    format++;  
                    break;  
                }  
                case 's':  
                {  
                    char *valstr = va_arg(ap,char *);  
                    printstr(valstr);  
                    format++;  
                    break;  
                }  
                case 'f':  
                {  
                    float valflt = va_arg(ap,double);  
                    printfloat(valflt);  
                    format++;  
                    break;  
                }  
                default:  
                {  
                    printch(*format);  
                    format++;  
                }  
            }    
        }  
    }
    va_end(ap);         
}  
 
