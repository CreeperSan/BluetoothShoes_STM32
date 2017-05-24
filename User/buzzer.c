
#include "stm32f4xx.h"
#include "buzzer.h"
#include "define.h"

void initBuzzer()
{
	GPIO_InitTypeDef gpioDef;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	gpioDef.GPIO_Mode = GPIO_Mode_OUT;
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_Pin = GPIO_Pin_2;
	gpioDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioDef.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOD,&gpioDef);
	
}

void setBuzzerState(STATE state)
{
	if(state == false)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	}
	else
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	}
}

