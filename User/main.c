
#include "stm32f4xx.h"
#include "define.h"
#include "humiture.h"
#include "delay.h"
#include "uart.h"
#include "tim.h"

unsigned char dat_humiture[5];		//温湿度数据

int main()
{
	initTimTwinkle();
	initUSART1();
	initHumitureClock();//初始化温湿度时钟
	initHumitureAsOutput();//初始化温湿度控制
	initSystemTick();//初始化系统滴答定时器 (用于延时)
	usart1SendString("System Start!");
	while(1)
	{
		readHumiture(dat_humiture);
	}
}


//以下为中断服务函数

void USART1_IRQHandler(void)
{
	uint8_t getChar;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
		getChar = USART_ReceiveData(USART1);
		usart1SendByte(getChar);
		if(getChar == 0x30){
			usart1SendString("湿度");
			usart1SendByte(0x30+(dat_humiture[0]%1000/100));
			usart1SendByte(0x30+(dat_humiture[0]%100/10));
			usart1SendByte(0x30+(dat_humiture[0]%10));
			usart1SendByte('.');
			usart1SendByte(0x30+(dat_humiture[1]%1000/100));
			usart1SendByte(0x30+(dat_humiture[1]%100/10));
			usart1SendByte(0x30+(dat_humiture[1]%10));
			usart1SendString("温度");
			usart1SendByte(0x30+(dat_humiture[2]%1000/100));
			usart1SendByte(0x30+(dat_humiture[2]%100/10));
			usart1SendByte(0x30+(dat_humiture[2]%10));
			usart1SendByte('.');
			usart1SendByte(0x30+(dat_humiture[3]%1000/100));
			usart1SendByte(0x30+(dat_humiture[3]%100/10));
			usart1SendByte(0x30+(dat_humiture[3]%10));
			usart1SendByte('\n');
		}
	}
}

//----- 闪烁定时器中断
void TIM6_DAC_IRQHandler(){
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)){
		
		TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	}
	
}

void SysTick_Handler()
{
	delayTimeDecrement();
}
