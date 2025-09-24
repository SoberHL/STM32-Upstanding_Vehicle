#include "stm32f10x.h"                  // Device header

/**
   * @brief
   * @param
   * @retval
   */
void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启外设时钟
	
	TIM_InternalClockConfig(TIM3);//选择时钟源
	
	 /*时基单元初始化*/ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟分频，主要决定滤波采样频率
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数器模式选择
	TIM_TimeBaseInitStructure.TIM_Period=72-1;//自动重装值
	TIM_TimeBaseInitStructure.TIM_Prescaler=1000-1;//分频值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器,只有高级定时器才有
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//由于TimeBaseInit的函数设计，开启中断前手动清除中断标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);//时钟使能
}

//void TIM1_UP_IRQHandler(void)
//{
//		if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)
//		{
//			
//			TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
//		}
//}
