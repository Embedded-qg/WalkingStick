/**
  ******************************************************************************
  * @file    sim900a.c
  * @author  Shengqiang.Zhang
  * @version 
  * @date    2017-03-28
  * @brief   SIM900A GPRS操作
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
#include <stdarg.h>
#include "bsp_usart3.h"
#include "sim900a.h"
#include <string.h>
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_usart3.h"
char ime[16];//IMEI串号
char ccid[20];//CCID号码
char csq;//CSQ


/**
 * @brief  sim900a_cmd  发送命令
 * @param  cmd：命令字符串指针，reply：希望得到的命令响应，waittime：命令等待的时间
 * @retval 命令是否得到正常响应，正常：SIM900A_TRUE，不正常：SIM900A_FALSE
 */
uint8_t sim900a_cmd(char *cmd, char *reply,uint32_t waittime )
{    
    SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据

    SIM900A_TX(cmd);                    //发送命令

    if(reply == 0)                      //不需要接收数据
        return SIM900A_TRUE;
    
    SIM900A_DELAY(waittime);                 //延时
    
    return sim900a_cmd_check(reply);    //对接收数据进行处理
}


/**
 * @brief  sim900a_cmd_check  检测命令的响应
 * @param  reply命令响应的字符串指针
 * @retval 检测到匹配的响应SIM900A_TRUE，没检测到匹配的响应SIM900A_FALSE
 */
sim900a_res_e sim900a_cmd_check(char *reply)
{
	char *redata;

	redata = SIM900A_RX();   //接收数据 
	
	if(strstr(redata,reply) != NULL) 
	{
		SIM900A_CLEAN_RX();
		return SIM900A_TRUE;        //查找到匹配的响应内容
	}
	else
	{
		SIM900A_CLEAN_RX();
		return SIM900A_FALSE;       
	}
}



/**
 * @brief  sim900a_waitask  等待数据应答 
 * @param  waitask_hook，检测事件勾子函数，若产生事件，退出本函数。
 * @retval 接收到的应答数据指针
 */
char * sim900a_waitask(uint8_t waitask_hook(void))      //等待有数据应答
{
    uint8_t len;
    char *redata;
    do{
        redata = SIM900A_RX_LEN(len);   //接收数据
        if(waitask_hook!=0)
        {
            if(waitask_hook()==SIM900A_TRUE)           //返回 SIM900A_TRUE 表示检测到事件，需要退出
            {
                redata = 0;
                return redata;               
            }
        }
    }while(len==0);                 //接收数据为0时一直等待
    
    
    SIM900A_DELAY(20);              //延时，确保能接收到全部数据（115200波特率下，每ms能接收11.52个字节）
    return redata;
}



/**
 * @brief  sim900a_cnum 
 * @param  num:存储本机号码的指针
 * @retval 返回SIM900A_FALSE表示获取失败，返回SIM900A_TRUE表示成功
 */
uint8_t sim900a_cnum(char *num)
{
    char *redata;
    uint8_t len;
    
    if(sim900a_cmd("AT+CNUM\r","OK", 1000) != SIM900A_TRUE)
    {
        return SIM900A_FALSE;
    }
    
    redata = SIM900A_RX();  //接收数据
    
    if(len == 0)
    {
        return SIM900A_FALSE;
    }
    
    //第一个逗号后面的数据为:"本机号码"
    while(*redata != ',')
    {
        len--;
        if(len==0)
        {
            return SIM900A_FALSE;
        }
        redata++;
    }
    redata+=2;
    
    while(*redata != '"')
    {
        *num++ = *redata++;
    }
    *num = 0;               //字符串结尾需要清0
    return SIM900A_TRUE;
}

void get_CCID(char num[20])
{
	u8 	i;
	char str[21];
	char redata[256];
	SIM900A_CLEAN_RX();
  sim900a_tx_printf("AT+CCID\r");
	SIM900A_DELAY(100);
	strcpy(redata, SIM900A_RX());
	SIM900A_CLEAN_RX();
	for (i = 0; i < 20; i++)
	{
		num[i] = redata[i+10];
		str[i] = num[i];
	}
	str[20] = '\0';
	DEBUG_GPRS("CCID:%s\r\n",str);
}

void get_CSQ(char *num)
{
	char redata[256];
	SIM900A_CLEAN_RX();
    sim900a_tx_printf("AT+CSQ\r");
	SIM900A_DELAY(100);
	strcpy(redata, SIM900A_RX());
	SIM900A_CLEAN_RX();

	*num = redata[15] - '0';
	if (redata[16] == ',')
		*num = redata[15] - '0';
	else if (redata[17] == ',')
		*num = (redata[15] - '0') * 10 + (redata[16] - '0');
	else
		*num = 99;
	DEBUG_GPRS("CSQ:%d\r\n",*num);
}


/**
 * @brief  sim900a_gprs_init GPRS功能初始化
 * @param  无
 * @retval 无
 */
void sim900a_gprs_init(void)
{
    sim900a_tx_printf("AT+CGCLASS=\"B\"\r");                       //设置移动台类别为"B"
//	sim900a_tx_printf("AT+CGCLASS?\r");
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGDCONT=1,\"IP\",\"CMNET\"\r");          //PDP上下文标识1，互联网协议，接入点名称：CMNET
//	sim900a_tx_printf("AT+CGDCONT?\r");
    SIM900A_DELAY(100);
    sim900a_tx_printf("AT+CGATT=1\r");                             //附着 GPRS 网络
//	sim900a_tx_printf("AT+CGATT?\r");
    SIM900A_DELAY(100);
//	sim900a_tx_printf("AT+CIPCSGP?\r");
//	SIM900A_DELAY(100);
//	printf(SIM900A_RX());
		SIM900A_CLEAN_RX();
}




/**
 * @brief  sim900a_gprs_tcp_link 建立TCP连接
 * @param  无
 * @retval 无
 */
sim900a_res_e sim900a_gprs_tcp_link(char * serverip,char * serverport)
{
	SIM900A_CLEAN_RX();
  sim900a_tx_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%s\"\r",serverip,serverport);
	SIM900A_DELAY(1000);
	if (sim900a_cmd_check("CONNECT OK") == SIM900A_TRUE)//连接成功
	{
		SIM900A_CLEAN_RX();
		return SIM900A_TRUE;		
	}
	else
	{
		SIM900A_CLEAN_RX();
		return SIM900A_FALSE;
	}
}

/**
 * @brief  sim900a_gprs_status 判断网络连接状态 
 * @param  无
 * @retval 返回1表示已连接  返回0表示未连接
 */
sim900a_res_e sim900a_gprs_status(void)
{
	char redata[256];
	char *p;
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+CIPSTATUS\r");
	SIM900A_DELAY(500);
  strcpy(redata, SIM900A_RX());   //接收数据 
	SIM900A_CLEAN_RX();
	DEBUG_GPRS("\n注册结果是：%s\n\n\n\n",redata);
	DEBUG_GPRS("--------------\r\n");
	p = strstr(redata,"STATE");//返回STATE后的字符串
	if (p)
		DEBUG_GPRS(p);
	else
	{
		DEBUG_GPRS(redata);
		SIM900A_SEND_ENDCHAR();//没有返回  可能是处于发送但未完成的状态
	}
	
	if(strstr(redata,"STATE: CONNECT OK") != NULL)//成功
		return SIM900A_TRUE;
	else
		return SIM900A_FALSE;
}

/**
 * @brief  sim900a_gprs_send 通过GPRS发送数据
 * @param  str:数据指针
 * @retval 无
 */
sim900a_res_e sim900a_gprs_send(char * str)
{
	int i = 50;
	SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
  sim900a_tx_printf("AT+CIPSEND\r");
	SIM900A_DELAY(300);                 //延时
	DEBUG_GPRS("发送中\n");
  if (sim900a_cmd_check(">") == SIM900A_TRUE)
	{
		sim900a_tx_printf(str,i);
		SIM900A_DELAY(100);
		SIM900A_SEND_ENDCHAR();
		SIM900A_DELAY(1264);
		SIM900A_DELAY(1264);		
		return sim900a_cmd_check("SEND OK");
	}
	else
	{
		SIM900A_SEND_ENDCHAR();
		//SIM900A_DELAY(100);
		//SIM900A_CLEAN_RX();
		return SIM900A_FALSE;
	}
}
/**
 * @brief  sim900a_gprs_send 通过GPRS发送数据
 * @param  str:数据指针
 * @retval 无
 */
sim900a_res_e sim900a_gprs_send_photo(char * str,u32 len)
{
	int count = 0;
	int i,j = 0;
	SIM900A_CLEAN_RX();                 //清空了接收缓冲区数据
  sim900a_tx_printf("AT+CIPSEND\r");
	SIM900A_DELAY(100);                 //延时
	DEBUG_GPRS("发送中\n");
	SIM900A_DELAY(100);
  if (sim900a_cmd_check(">") == SIM900A_TRUE)
	{
		for(i = 0;i < len;i++)
		{
			  if(*str == 0x1a || *str == 0x1b)
				{
					USART_SendData(USART3,*str + 2);
					while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
					USART_SendData(USART3,*str + 2);
					while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
					USART_SendData(USART3,'\0');
					j++;
				}
				else if(*str == 0x1c || *str == 0x1d)
				{
					USART_SendData(USART3,*str);
					while( USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
					USART_SendData(USART3,'\0');
				}
				else				USART_SendData(USART3,*str);
			  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
				str++;
		}
		DEBUG_GPRS("j = %d\r\n",j);
		SIM900A_DELAY(100);
		SIM900A_SEND_ENDCHAR();
		SIM900A_DELAY(100);
		SIM900A_CLEAN_RX();//清空了接收缓冲区数据
		while(1)
		{
			if(strstr(SIM900A_RX(),"OK") != NULL || strstr(SIM900A_RX(),"ERROR") != NULL) 
				break;
			else
			{
				if(count < 1000)//10s超时
				{
					count++;
					Delay_ms(10);
				}
				else
				{
					printf("获取返回信息失败\r\n");
					return SIM900A_FALSE;
				}
			}
				
		}
		
		//SIM900A_DELAY(1464);		
		return sim900a_cmd_check("SEND OK");
	}
	else
	{
		SIM900A_SEND_ENDCHAR();
		//SIM900A_DELAY(100);
		//SIM900A_CLEAN_RX();
		return SIM900A_FALSE;
	}
}

/**
 * @brief  sim900a_gprs_link_close 断开网络连接
 * @param  无
 * @retval 无
 */
void sim900a_gprs_link_close(void) 
{
	SIM900A_CLEAN_RX();
  sim900a_tx_printf("AT+CIPCLOSE=1\r");
	SIM900A_DELAY(300);
	DEBUG_GPRS("关闭tcp连接结果：%s",SIM900A_RX());
}

/**
 * @brief  sim900a_gprs_checkIP 查IP
 * @param  无
 * @retval 无
 */
char* sim900a_gprs_checkIP(void) 
{
	char redata[256];
	char *p = redata;
	SIM900A_CLEAN_RX();
    sim900a_tx_printf("AT+CIFSR\r");
	SIM900A_DELAY(100);
	strcpy(redata, SIM900A_RX());
	SIM900A_CLEAN_RX();
	p += 11;
	return p;
}

void get_IMEI(char num[15])
{
	u8 	i;
	char str[16];
	char redata[256];
	SIM900A_CLEAN_RX();
  sim900a_tx_printf("AT+GSN\r");
	SIM900A_DELAY(100);
	strcpy(redata, SIM900A_RX());
	SIM900A_CLEAN_RX();
	for (i = 0; i < 15; i++)
	{
		num[i] = redata[i+9];
		str[i] = num[i];
	}
	str[15] = '\0';
	DEBUG_GPRS("IMEI:%s\r\n",str);
}



/**
 * @brief  gprs_reg_status 判断网络注册状态 
 * @param  无
 * @retval 
 */
sim900a_res_e gprs_reg_status(void)
{
	char redata[256];
	int count = 0;
	
	while(1)
	{
		SIM900A_CLEAN_RX();
		sim900a_tx_printf("AT+CGREG?\r");
		SIM900A_DELAY(2000);
		strcpy(redata, SIM900A_RX());   //接收数据
		DEBUG_GPRS("reg:%s\r\n",redata);

		if (redata[22] == '1')
		{
			printf("已注册本地网络\r\n");
			return SIM900A_TRUE;
		}
			else if  (redata[22] == '5')
		{
			printf("已注册漫游网络\r\n");
			return SIM900A_TRUE;
		}
		else
		{	
			printf("未注册 ERR:%c\r\n",redata[22]);     //手机卡问题
			if(count >10)
				return SIM900A_FALSE;
		}
		count++;
	}

}

/**
 * @brief  gprs_init 初始化gprs
 * @param  无
 * @retval 
 */
sim900a_res_e gprs_init(char* ip,char* port)
{
	u32 count=0;
	u32 count2=0;
	while(sim900a_cmd("AT\r","OK",1000) != 1)//检测模块响应是否正常
	{
		count++;
		if(count >= 10)//尝试重新连接超过5遍
		{
			printf("模块响应测试错误！\r\n");
			return SIM900A_FALSE;
		}
		else
		{
			printf("模块响应测试错误！正在重新连接中(%d)\r\n",count);
		}
			
	}
	printf("GPRS模块响应正常\r\n");
	//以下设置只需设置一次  就不要再设置了  掉电仍然保持
	
	if (SIM900A_FALSE == gprs_reg_status())//注册失败
		sim900a_gprs_init();

	if (sim900a_gprs_status() == 0)
	{
		if (1 == sim900a_gprs_tcp_link(ip, port))
		{
			printf("TCP建立连接成功！GPRS网络工作正常\r\n\r\n");
			return SIM900A_TRUE;
		}
		else
		{
			printf("TCP建立连接失败！GPRS网络工作异常(%d)\r\n\r\n",count2);
			return SIM900A_FALSE;				
		}	
	}
	else
	{
		printf("GPRS网络工作正常\r\n");
		return SIM900A_TRUE;
	}
}


/**
 * @brief  gps_to_place 将经纬度转化为实际地址
 * @param  无
 * @retval 
 */

sim900a_res_e gps_to_place(float longitude,float latitude)
{
	char lo[20]={0};
	char la[20]={0};
	char url[200]={0};//提交地址
	char key[]={"SzvU0ErKlhLnxdGZWWBFrif1q7AQ3Rbh"};//密钥
	int count = 0;
	char redata[30];
	
	sprintf(lo,"%f",longitude);//将浮点型经度格式化为字符串
	sprintf(la,"%f",latitude);//将浮点型纬度格式化为字符串
	//sprintf(url,"%s%s%s%s%s\"\r","AT+HTTPPARA=\"URL\",\"http://120.78.193.178:8080/smart_glasses/getpoint?lo=",lo,"&","la=",la);//格式化字符串，转化为提交地址
	sprintf(url,"%s\"\r","AT+HTTPPARA=\"URL\",\"http://120.78.193.178:8080/smart_glasses/getpoint?lo=113.39723&la=23.0374");//格式化字符串，转化为提交地址
	DEBUG_GPRS("url:%s\r\n",url);//查看一下输出的字符串是否符合预期要求
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r");//设置接入点为CMNET
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果		

	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=3,1,\"PHONENUM\",\"12345678900\"\r");//设置手机号码
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=4,1\r");//查询参数设置
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	

	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=5,1\r");//将本次的参数设置保存到RAM，下次就不用设置了，直接打开就行
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=1,1\r");//打开承载
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	

	sim900a_tx_printf("AT+SAPBR=2,1\r");//打开承载
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	

	sim900a_tx_printf("AT+HTTPINIT\r");//初始化HTTP
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+HTTPPARA=\"CID\",\"1\"\r");//设置承载上下文标识，和上面一致
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	
	
	SIM900A_CLEAN_RX();
	sim900a_tx_printf(url);//设置你要访问的那个网站
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果	

	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+HTTPACTION=0\r");//激活HTTP请求，0表示get方式、1表示post方式、2表示head
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果		
	
	while(1)
	{
		strcpy(redata,SIM900A_RX());
		if(strstr(redata,"0,200") != NULL) 
			break;
		else
		{
			if(count < 60)//一分钟超时
			{
				SIM900A_DELAY(1000);
				count++;
				printf("正在等待返回信息\r\n");
			}
			else
			{
				printf("获取返回信息失败\r\n");
				return SIM900A_FALSE;
			}
		}
			
	}
	
	SIM900A_DELAY(2000);
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+HTTPREAD=1,1000\r");//读取HTTP相应数据。1表示从第一个字节开始读，读100个字节
	SIM900A_DELAY(2000);
	printf("%s\n",SIM900A_RX());//输出结果		


	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+HTTPTERM\r");//终止HTTP服务
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//输出结果		
 
	SIM900A_CLEAN_RX();
	sim900a_tx_printf("AT+SAPBR=0,1\r");//设置接入点为CMNET
	SIM900A_DELAY(500);
	DEBUG_GPRS("输出结果%s",SIM900A_RX());//关闭承载
	
	
	return SIM900A_TRUE;
}


