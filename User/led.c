
#include "stm32f4xx.h"
#include "led.h"
#include "define.h"

void initLed()
{
	GPIO_InitTypeDef gpioDef;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	gpioDef.GPIO_Mode = GPIO_Mode_OUT;
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_Pin = GPIO_Pin_10;
	gpioDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpioDef.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&gpioDef);
}

void setLedState(STATE state)
{
	if(state == true)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_10);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	}
}



