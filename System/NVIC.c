#include "stm32f10x.h"                  // Device header

void NVIC_GroupInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
