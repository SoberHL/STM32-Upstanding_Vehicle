#include "stm32f10x.h"                  // Device header

/**
   * @brief AD转换器初始化
   * @param 无
   * @retval 无
   */
void RP_Init(void)
{
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//开启ADC2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//配置ADC时钟分频
	
	/*设置GPIO口*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入模式
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*设置为规则通道*/
	ADC_RegularChannelConfig(ADC2,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	
	/*配置ADC模式*/
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent; //独立扫描
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right; //右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None; //软件作为触发源
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE; //单次扫描/连续扫描
	ADC_InitStructure.ADC_NbrOfChannel=1; //通道数目
	ADC_InitStructure.ADC_ScanConvMode=DISABLE; //非扫描模式
	ADC_Init(ADC2,&ADC_InitStructure);
	
	ADC_Cmd(ADC2,ENABLE);//启动ADC2
	
	/*启动后校准*/
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2)==SET);//等待就绪完成
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2)==SET);//等待校准完成
}

/**
   * @brief 获取特定通道AD的转换值
   * @param 你想要读取的对应通道数,范围是0-3
   * @retval AD的转换值,范围是0-4095
   */
uint16_t RP_GetValue(uint8_t n)
{
	if(n==1)
	{
		ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_55Cycles5);//选择1个通道以填充列表
	}
	else if(n==2)
	{
		ADC_RegularChannelConfig(ADC2,ADC_Channel_3,1,ADC_SampleTime_55Cycles5);//选择1个通道以填充列表
	}
	else if(n==3)
	{
		ADC_RegularChannelConfig(ADC2,ADC_Channel_4,1,ADC_SampleTime_55Cycles5);//选择1个通道以填充列表
	}
	else if(n==4)
	{
		ADC_RegularChannelConfig(ADC2,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);//选择1个通道以填充列表
	}

	
	ADC_SoftwareStartConvCmd(ADC2,ENABLE);//软件读取启动转换
	while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==RESET);//等待转换完成
	return ADC_GetConversionValue(ADC2);
}
