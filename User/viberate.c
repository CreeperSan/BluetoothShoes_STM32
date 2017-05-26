
#include "stm32f4xx.h"
#include "viberate.h"
#include "define.h"

void initMassage()
{
	GPIO_InitTypeDef gpioType;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	gpioType.GPIO_Mode = GPIO_Mode_OUT;
	gpioType.GPIO_OType = GPIO_OType_PP;
	gpioType.GPIO_Pin = GPIO_Pin_8;
	gpioType.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioType.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&gpioType);
}

void setMassageState(STATE state)
{
	if(state == true)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	}
}

void initNotification()
{
	GPIO_InitTypeDef gpioType;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	gpioType.GPIO_Mode = GPIO_Mode_OUT;
	gpioType.GPIO_OType = GPIO_OType_PP;
	gpioType.GPIO_Pin = GPIO_Pin_13;
	gpioType.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioType.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&gpioType);
}

void setNotificationState(STATE state)
{
	if(state == true)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_13);
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	}
}
