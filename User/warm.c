
#include "stm32f4xx.h"
#include "define.h"
#include "warm.h"

void initWarm()
{
	GPIO_InitTypeDef gpioDef;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	gpioDef.GPIO_Mode = GPIO_Mode_OUT;
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_Pin = GPIO_Pin_12;
	gpioDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioDef.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&gpioDef);
}

void setWarmState(STATE state)
{
	if(state)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	}
}




