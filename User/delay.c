
#include "stm32f4xx.h"
#include "uart.h"
#include "delay.h"


static __IO u32 delayTime;


void delay_ms(unsigned int time)
{
	delayUs(time*1000);
}

void delay_us(unsigned int time)
{
	delayUs(time);
}

void initSystemTick(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 * 来自于野火程序 */
	if(SysTick_Config(SystemCoreClock / 1000000)){
		//此处为系统异常执行的代码
		usart1SendString("Error!\n");
		while(1);
	}
}

static void delayUs(__IO u32 mTime)
{
	delayTime = mTime;
	while(delayTime>0);
}

void delayTimeDecrement()
{
	if(delayTime!=0x00){
		delayTime--;
	}
}
































