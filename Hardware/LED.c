#include "stm32f10x.h"                  // Device header

/**
   * @brief 把A1和A2两个端口初始化
   * @param 无
   * @retval 无
   */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);//GPIO配置好后默认低电平，需要手动置高
}

/**
   * @brief 控制插在GPIOA1的LED亮灭
	 * @param 控制位，0为低，1为高
   * @retval 无
   */
void LED_Control(unsigned char i)
{
	if(i==0)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);//置低
	}
	if(i==1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);//置高
	}
}

/**
   * @brief 对A1口的电平取反
   * @param 无
   * @retval 无
   */
void LED_Convert(void)
{
	if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)==0)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	}
	else if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)==1)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}
