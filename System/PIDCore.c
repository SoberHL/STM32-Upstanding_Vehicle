#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "Motor.h"
#include "LED.h"
#include "PIDCore.h"
#include <math.h>

#define EPSLION 0.001

//void PID_Tick(PID_t *p)
//{
//	/*2.计算误差，并整理得到上次误差*/
//	p->Error1=p->Error0;							//得到上次误差
//	p->Error0=p->Target-p->Actual;		//得到本次误差
//	
//	if(p->Ki > EPSLION)								//Ki非零才引入积分项
//	{
//		p->ErrorInt+=p->Error0;					//误差积分
//	}
//	else
//	{
//		p->ErrorInt=0;
//	}

//	/*3.PID计算*/
//	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0-p->Error1);
//	
//	/*4.输出限幅*//*注意Out的范围*/
//	if(p->Out > p->OutMax ){p->Out = p->OutMax ;}
//	if(p->Out < p->OutMin ){p->Out = p->OutMin ;}
//}

void Vertical_Loop(float Gyro,PID_t *p)
{
	p->Error0 = p->Target - p->Actual;
	
	p->Out = p->Kp * p->Error0 + p->Kd * Gyro;
	
	if(p->Out > p->OutMax ){p->Out = p->OutMax ;}
	if(p->Out < p->OutMin ){p->Out = p->OutMin ;}
}

void Speed_Loop(PID_t *p)
{
	static int16_t Encoder_Least,Encoder,Encoder_Int;
	Encoder_Least = p->Actual;
	Encoder *= 0.6;							//一阶低通滤波
	Encoder += Encoder_Least * 0.4;
	Encoder_Int += Encoder;
	float Error = Encoder - p->Target;
	
	if(fabs(Error) <= 6) {Error = 0;Encoder_Int = 0;}//误差过小时置零速度环
	
	p->Out = p->Kp * Error + p->Ki * Encoder_Int;
	
	/*4.输出限幅和积分限幅*//*注意Out的范围*/
	if(Encoder_Int >  70){Encoder_Int =  70;}
	if(Encoder_Int < -70){Encoder_Int = -70;}
	
	if(p->Out > p->OutMax ){p->Out = p->OutMax ;}
	if(p->Out < p->OutMin ){p->Out = p->OutMin ;}

}

void Turn_Loop(PID_t *p)
{
	if(p->Target < EPSLION)
	{
		p->Out = p->Kd * p->Actual;
	}
	else 
	{
		;
	}
}
