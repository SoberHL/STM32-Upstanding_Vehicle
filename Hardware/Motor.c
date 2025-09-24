#include "stm32f10x.h"  // Device header
#include "PWM.h"

/**
   * @brief 初始化直流电机，包括GPIO口和Timer的PWM模式
   * @param 无 
   * @retval 无
   */
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//方向控制引脚
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	PWM_Init();
}

/**
   * @brief 通过调节RCC间接设定直流电机1的速度
   * @param 速度,范围是 -1000 ~ +1000
   * @retval 无
   */
void Motor1_SetPWM(int16_t PWM)
{
	if(PWM>=0)//判断正反转
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		PWM_SetCompareA(PWM);
	}
	else if(PWM<0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
		PWM_SetCompareA(-PWM);
	}
}

/**
   * @brief 通过调节RCC间接设定直流电机2的速度
   * @param 速度,范围是 -1000 ~ +1000
   * @retval 无
   */
void Motor2_SetPWM(int16_t PWM)
{
	if(PWM>=0)//判断正反转
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
		PWM_SetCompareB(PWM);
	}
	else if(PWM<0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
		PWM_SetCompareB(-PWM);
	}
}
