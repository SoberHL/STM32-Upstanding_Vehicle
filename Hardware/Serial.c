#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_ReceiveFlag;

/**
   * @brief 串口发送初始化
   * @param 无
   * @retval 无
   */
void Serial3_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/*GPIO控制*/
	/*PB10*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*PB11*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*USART配置*/
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600; //波特率
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //流控
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx; //同时启用发送接收
	USART_InitStructure.USART_Parity=USART_Parity_No; //无校验
	USART_InitStructure.USART_StopBits=1;//停止位长度
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//8位数据模式
	USART_Init(USART3,&USART_InitStructure);
	
	/*串口接收：启用中断模块*/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//RXNE置1时申请中断
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3,ENABLE);//开启USART
}

/**
   * @brief 串口发送一个字节数据
   * @param 要发送的数据
   * @retval 无
   */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3,Byte);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);//检查TXE标志位确保发送完成
}

/**
   * @brief 串口发送普通数组
   * @param 数组名
	 * @param 数组元素个数
   * @retval 无
   */
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Array[i]);
	}
}

/**
   * @brief 串口发送字符串
   * @param 你要发送的字符串
   * @retval 无
   */
void Serial_SendString(char *string)
{
	uint16_t i;
	for(i=0;string[i]!='\0';i++)
	{
		Serial_SendByte(string[i]);
	}
}

uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t Ret=1;
	
	while(y)
	{
		Ret*=x;
		y--;
	}
	return Ret;
}

/**
   * @brief 串口以字符形式发送一串数字
   * @param 你要发送的数字
   * @param 数字的长度
   * @retval 无
   */
void Serial_SendNum(uint32_t Num,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
		Serial_SendByte(Num/Serial_Pow(10,Length-i-1)%10+'0');
	}
}

//printf函数重定向到串口
int fputc(int ch,FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(char *format, ...)
{
	char String[100];				//定义字符数组
	va_list arg;					//定义可变参数列表数据类型的变量arg
	va_start(arg, format);			//从format开始，接收参数列表到arg变量
	vsprintf(String, format, arg);	//使用vsprintf打印格式化字符串和参数列表到字符数组中
	va_end(arg);					//结束变量arg
	Serial_SendString(String);		//串口发送字符数组（字符串）
}

/*
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//读取RD也可达到清标志位的目的
	}
}
*/
