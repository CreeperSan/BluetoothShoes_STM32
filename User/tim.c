
#include "stm32f4xx.h"
#include "tim.h"

void initTimTwinkle()//这个是于闪烁的定时器(车灯闪烁、蜂鸣器间断鸣起)
{
	NVIC_InitTypeDef nvicDef;
	TIM_TimeBaseInitTypeDef timDef;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvicDef.NVIC_IRQChannel = TIM6_DAC_IRQn;
	nvicDef.NVIC_IRQChannelCmd = ENABLE;
	nvicDef.NVIC_IRQChannelPreemptionPriority = 2;
	nvicDef.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicDef);
	
	timDef.TIM_Period = 100;//周期
	timDef.TIM_Prescaler = 44999;//分频系数
	TIM_TimeBaseInit(TIM6,&timDef);
	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}

































