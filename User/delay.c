
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
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 * ������Ұ����� */
	if(SysTick_Config(SystemCoreClock / 1000000)){
		//�˴�Ϊϵͳ�쳣ִ�еĴ���
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
































