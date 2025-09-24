#pragma once		//防重复包含

typedef struct {
	float Target;
	float Actual;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	
	float OutMax;
	float OutMin;
}PID_t;

//void PID_Tick(PID_t *p);

void Vertical_Loop(float Gyro,PID_t *p);
void Speed_Loop(PID_t *p);
void Turn_Loop(PID_t *p);
