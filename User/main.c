#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "LED.h"
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "PIDCore.h"
#include "NVIC.h"
#include "MPU6050.h"
#include "inv_mpu.h"

#define angle Roll		//小车的平衡用的是翻滚角
#define Zero 1				//机械中值

PID_t Turn_PID = {
	.Kp = 0,
	.Ki = 0,
	.Kd = 0.02,
	.Target = 0,
	
	.OutMax = 50,
	.OutMin = -50,
};

PID_t Speed_PID = {
	.Kp = 0.53,
	.Ki = 0.32 / 200,
	.Kd = 0,
	.Target = 0,
	
	.OutMax = 50,
	.OutMin = -50,
};

PID_t Balance_PID = {
	.Kp = 7.5 * 0.6,
	.Ki = 0,
	.Kd = -0.051 * 0.6,
	.Target = Zero,
	
	.OutMax = 50,
	.OutMin = -50,
};

float Pitch,Roll,Yaw;								//俯仰角默认跟中值一样，翻滚角，偏航角
int16_t L_Encoder,R_Encoder;
int16_t ax,ay,az,gx,gy,gz;							//加速度，陀螺仪角速度

uint8_t KeyNum;
uint8_t Kf = 1;

int main(void)
{
	OLED_Init();
	Encoder1_Init();
	Encoder2_Init();
	Serial3_Init();
	NVIC_GroupInit();
	LED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	
	Motor_Init();			//其中用到了TIM1产生中断,最后初始化
	
	OLED_Printf(0,0,OLED_8X16,"双环控制");
	OLED_Update();
	
	while(1)
	{	
		OLED_Printf(0,16,OLED_6X8,"LEn:%+04d",L_Encoder);
		OLED_Printf(0,24,OLED_6X8,"REn:%+04d",R_Encoder);
		OLED_Printf(0,32,OLED_6X8,"angle:%+05.2f",angle);
		OLED_Printf(0,40,OLED_6X8,"Lout:%+05.2f",Balance_PID.Out);
		OLED_Printf(0,48,OLED_6X8,"Sout:%+05.2f",Speed_PID.Out);
		                               
		OLED_Update();
	}
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t count1 = 0;
	static uint16_t count2 = 0;
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET)//0.1ms进一次
	{
		/*角度环作为内环，速度环为外环*/
		count1++;count2++;
		if(count1>=10)
		{
			count1=0;
			
			/*角度读取*/
			MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);				//读取姿态信息(其中偏航角有飘移是正常现象)
			MPU_Get_Gyroscope(&gx,&gy,&gz);

			/*1.获取实际值*/
			Balance_PID.Actual = angle;
			Turn_PID.Actual = gz;
			
			if(angle > 30 || angle < -30)//角度过大关闭电机
			{
				Kf = 0;
				LED_Control(0);
			}
			
			/*2-4*/
			Vertical_Loop(gx,&Balance_PID);//直立环PID调控
			Turn_Loop(&Turn_PID);					 //转向环PID调控
			
			/*5.执行控制*/
			int16_t PWM_Out = Kf * (Balance_PID.Out + Speed_PID.Out);			
			Motor1_SetPWM(PWM_Out + Turn_PID.Out);
			Motor2_SetPWM(PWM_Out - Turn_PID.Out);
						
		}
		
		if(count2>=200)
		{
			count2 = 0;
			
			/*1.获取实际值*/
			L_Encoder = Encoder1_Get();
			R_Encoder = Encoder2_Get();
			Speed_PID.Actual = (L_Encoder + R_Encoder);
			
			/*2-4*/
			Speed_Loop(&Speed_PID);//速度环PID调控
			
		}

		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}
