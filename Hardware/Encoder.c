#include "stm32f10x.h"                  // Device header

/**
   * @brief 旋转编码器1初始化
   * @param 无
   * @retval 无
   */
void Encoder1_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启外设时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//看接在的外设空闲状态是什么电平，就配置成上拉还是下拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	 /*时基单元初始化*/ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟分频，主要决定滤波采样频率
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数器模式选择
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//自动重装值,满量程
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器,只有高级定时器才有
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);

	/*捕获通道配置*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);//自动配置初始值
	
  /*通道1*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择捕获通道
	TIM_ICInitStructure.TIM_ICFilter=0xF;//捕获信号滤波参数
//	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿触发,(这一节表示高低电平极性是否反转),属于重复配置
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	/*通道2*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//选择捕获通道
	TIM_ICInitStructure.TIM_ICFilter=0xF;//捕获信号滤波参数	TIM_ICInit(TIM2,&TIM_ICInitStructure);

	/*配置编码器接口*/
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}

/**
   * @brief 旋转编码器2初始化
   * @param 无
   * @retval 无
   */
void Encoder2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启外设时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//看接在的外设空闲状态是什么电平，就配置成上拉还是下拉
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	 /*时基单元初始化*/ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//指定时钟分频，主要决定滤波采样频率
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数器模式选择
	TIM_TimeBaseInitStructure.TIM_Period=65536-1;//自动重装值,满量程
	TIM_TimeBaseInitStructure.TIM_Prescaler=1-1;//不分频
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//重复计数器,只有高级定时器才有
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

	/*捕获通道配置*/
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);//自动配置初始值
	
  /*通道1*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择捕获通道
	TIM_ICInitStructure.TIM_ICFilter=0xF;//捕获信号滤波参数
//	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿触发,(这一节表示高低电平极性是否反转),属于重复配置
	TIM_ICInit(TIM4,&TIM_ICInitStructure);
	
	/*通道2*/
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//选择捕获通道
	TIM_ICInitStructure.TIM_ICFilter=0xF;//捕获信号滤波参数	TIM_ICInit(TIM4,&TIM_ICInitStructure);

	/*配置编码器接口*/
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Falling,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM4,ENABLE);//启动定时器
}


/**
   * @brief 返回编码器1中Counter的值
   * @param 无
   * @retval Counter的值,范围是所设置的ARR的范围
   */
int16_t Encoder1_Get(void)
{
	int16_t Temp;
	Temp=TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2,0);//每隔一定时间清除Counter
	return Temp;
}

/**
   * @brief 返回编码器2中Counter的值
   * @param 无
   * @retval Counter的值,范围是所设置的ARR的范围
   */
int16_t Encoder2_Get(void)
{
	int16_t Temp;
	Temp=TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4,0);//每隔一定时间清除Counter
	return Temp;
}

