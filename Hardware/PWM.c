#include "stm32f10x.h"                  // Device header

/**
   * @brief 初始化PWM，启用了Timer2的输出比较功能
   * @param 无
   * @retval 无
   */
void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启外设时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*两个PWM波输出引脚*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_InternalClockConfig(TIM1);//选择时钟源
	
	 /*时基单元初始化*/ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟分频，主要决定滤波采样频率
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数器模式选择
	TIM_TimeBaseInitStructure.TIM_Period=100-1;//自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler=72-1;//分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器,只有高级定时器才有
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);//由于TimeBaseInit的函数设计，开启中断前手动清除中断标志位
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;//设置CCR
	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;//设置CCR
	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	/*中断配置*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1,ENABLE);//时钟使能
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//设置TIM1的PWM主输出为使能
	
}

/**
   * @brief 更改OC1的CCR
   * @param 你想改的值
   * @retval 无
   */
void PWM_SetCompareA(uint16_t Compare)
{
	TIM_SetCompare4(TIM1,Compare);
}

/**
   * @brief 更改OC1的CCR
   * @param 你想改的值
   * @retval 无
   */
void PWM_SetCompareB(uint16_t Compare)
{
	TIM_SetCompare1(TIM1,Compare);
}
