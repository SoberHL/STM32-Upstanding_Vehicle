#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Timer.h"
#include "Key.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "PIDCore.h"
#include "NVIC.h"
#include <string.h>

uint8_t KeyNum;
uint8_t RxData;

typedef enum{nodata,received} Receive_Flag_Indicator;//是否收到一个数据包
typedef enum{Starting,P,I,D,A,B,Ending} RxState_Indicator;//Rx状态指示标志位

Receive_Flag_Indicator Receive_Flag=nodata;

float A_Target,A_Actual,A_Out; //目标值，实际值，输出值
float B_Target,B_Actual,B_Out; //目标值，实际值，输出值
float Kp,Ki,Kd; //PID要调节的三个参数
/*1.数量级确定:大概是输出范围/输入范围*/
float A_Error0,A_Error1,A_ErrorInt; //本次误差，上次误差，误差积分
float B_Error0,B_Error1,B_ErrorInt; //本次误差，上次误差，误差积分

void TIM3_IRQHandler(void)
{
	static uint16_t count;
		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)//1ms进一次
		{
			count++;
			Key_Tick();
			if(count>=40)
			{
				count=0;

				PID_Tick();//PID调控
			}
			TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		}
}

void USART3_IRQHandler(void)
{
	static RxState_Indicator RxState=Starting;
	
	/*如果收到了数据*/
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		
		uint8_t RxData=USART_ReceiveData(USART3);//读取DR中的数据
		
		/*根据不同的状态指示值执行不同的操作*/
		if(RxState==Starting)//等待包头的状态
		{
			if(RxData=='P')
			{
				RxState=P;//收到包头改变状态为接收中
			}
			if(RxData=='I')
			{
				RxState=I;//收到包头改变状态为接收中
			}
			if(RxData=='D')
			{
				RxState=D;//收到包头改变状态为接收中
			}
			if(RxData=='A')
			{
				RxState=A;//收到包头改变状态为接收中
			}
			if(RxData=='B')
			{
				RxState=B;//收到包头改变状态为接收中
			}

		}
		
		else if(RxState==P)//数据接收中的状态
		{
			if(RxData=='\r')//收到第一个包尾
			{
				RxState=Ending;//改变状态为结束
			}
			else
			{
				Kp=(RxData-'0')/10.0;//读到的数据处理
			}
		}
		
		else if(RxState==I)//数据接收中的状态
		{
			if(RxData=='\r')//收到第一个包尾
			{
				RxState=Ending;//改变状态为结束
			}
			else
			{
				Ki=(RxData-'0')/10.0;//读到的数据处理
			}
		}

		else if(RxState==D)//数据接收中的状态
		{
			if(RxData=='\r')//收到第一个包尾
			{
				RxState=Ending;//改变状态为结束
			}
			else
			{
				Kd=(RxData-'0')/10.0;//读到的数据处理
			}
		}

		else if(RxState==A)//数据接收中的状态
		{
			if(RxData=='\r')//收到第一个包尾
			{
				RxState=Ending;//改变状态为结束
			}
			else
			{
				A_Target=(RxData-'0')*10;//读到的数据处理
			}
		}
		else if(RxState==B)//数据接收中的状态
		{
			if(RxData=='\r')//收到第一个包尾
			{
				RxState=Ending;//改变状态为结束
			}
			else
			{
				B_Target=(RxData-'0')*10;//读到的数据处理
			}
		}

		
		else if(RxState==Ending)//等待包尾状态
		{
			if(RxData=='\n')//收到第二个包尾
			{
				RxState=Starting;//改变状态为开始接收
				Receive_Flag=received;//收到标志位置1
			}
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);//读取RD也可达到清标志位的目的
	}
}


int main(void)
{
	Key_Init();
	OLED_Init();
	Motor_Init();
	Encoder1_Init();
	Encoder2_Init();
	Serial3_Init();
	NVIC_GroupInit();
	LED_Init();
	
	Timer_Init();
	
	OLED_Printf(0,0,OLED_8X16,"速度控制");
	OLED_Update();
	
	while(1)
	{
		OLED_Printf(0,16,OLED_6X8,"Kp:%+4.2f",Kp);
		OLED_Printf(0,24,OLED_6X8,"Ki:%+4.2f",Ki);
		OLED_Printf(0,32,OLED_6X8,"Kd:%+4.2f",Kd);
		
		OLED_Printf(64,16,OLED_6X8,"Tar1:%+04.0f",A_Target);
		OLED_Printf(64,24,OLED_6X8,"Act1:%+04.0f",A_Actual);
		OLED_Printf(64,32,OLED_6X8,"Out1:%+04.0f",A_Out);
		OLED_Printf(64,40,OLED_6X8,"Tar2:%+04.0f",B_Target);
		OLED_Printf(64,48,OLED_6X8,"Act2:%+04.0f",B_Actual);
		OLED_Printf(64,56,OLED_6X8,"Out2:%+04.0f",B_Out);
		                               
		OLED_Update();
		
		Serial_Printf("%f,%f,%f,%f,%f,%f\r\n",A_Target,A_Actual,A_Out,B_Target,B_Actual,B_Out);
	}
}

