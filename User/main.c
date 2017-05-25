
#include "stm32f4xx.h"
#include "define.h"
#include "humiture.h"
#include "delay.h"
#include "uart.h"
#include "tim.h"
#include "led.h"
#include "viberate.h"
#include "buzzer.h"
#include "adc.h"
#include "warm.h"

void sendTempData(void);

unsigned char dat_humiture[5];		//温湿度数据
extern __IO uint16_t ADC_ConvertedValue;//AD的原始数值
float ADC_Vol; 						//转换后的AD

unsigned char count = 0;
unsigned char const countMax = 32;
bool isBuzzer = false;
bool isNotification = false;
bool isNotificating = false;
unsigned char notificationViberaLeft = 0;
unsigned char notificationFrequency = 8;
bool isMassage = false;
unsigned char massageFrequency = 0;
bool isLed = false;
bool isLedNotification = false;
bool isLedNotificating = false;
unsigned char ledNotificationLeft = 0;
bool isNeedTemp = true;

int main()
{
	initTimTwinkle();//初始化闪烁定时器
	initUSART1();//初始化串口
	initSystemTick();//初始化系统滴答定时器 (用于延时)
	initLed();//初始化LED
	initMassage();
	initNotification();
	setLedState(false);
	initBuzzer();
	Rheostat_Init();
	initHumitureClock();//初始化温湿度时钟
	initHumitureAsOutput();//初始化温湿度控制
	initWarm();
	usart1SendString("System Start!");
	while(1)
	{
		//温湿度数据采集
		if(count>16)
		{
			isNeedTemp = true;
		}
		else
		{
			if(isNeedTemp)
			{
				isNeedTemp = false;
				readHumiture(dat_humiture);
			}
		}
		//LED操作
		if(isLed)
		{
			setLedState(true);
		}
		else
		{
			setLedState(false);
		}
		if(isLedNotification)
		{
			if((count%16)>=8)
			{
				isLedNotificating = true;
				setLedState(true);
			}
			else
			{
				if(isLedNotificating)
				{
					isLedNotificating = false;
					ledNotificationLeft--;
				}
				setLedState(false);
			}
			if(ledNotificationLeft<=0)
			{
				isLedNotification = false;
				isLedNotificating = false;
			}
		}
		//蜂鸣器处理
		if(isBuzzer)
		{
			if(count%16>=8)
			{
				setBuzzerState(true);
			}
			else
			{
				setBuzzerState(false);
			}
		}
		//消息通知震动处理
		if(isNotification){
			if(count%(notificationFrequency)*2>=notificationFrequency)
			{
				setNotificationState(true);
				isNotificating = true;
			}
			else
			{
				if(isNotificating)
				{
					isNotificating = false;
					notificationViberaLeft--;
				}
				setNotificationState(false);
			}
			if(notificationViberaLeft <=0)
			{
				isNotification = false;
			}
		}
		//按摩处理
		if(isMassage)
		{
			if(count<massageFrequency)
			{
				setMassageState(true);
			}
			else
			{
				setMassageState(false);
			}
		}
	}
}


//以下为中断服务函数

void USART1_IRQHandler(void)
{
	uint8_t getChar;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
		getChar = USART_ReceiveData(USART1);
		usart1SendByte(getChar);
		if(getChar == '0'){//回传温度
			sendTempData();
		}else if(getChar == '1'){//开灯
			isLed = true;
		}else if(getChar == '2'){//关灯
			isLed = false;
		}else if(getChar == '3'){//LED通知闪烁
			isLedNotification = true;
			isLedNotificating = false;
			ledNotificationLeft = 5;
		}else if(getChar == 'a'){//按摩等级1
			isMassage = true;
			massageFrequency = 2;
		}else if(getChar == 'b'){//按摩等级2
			isMassage = true;
			massageFrequency = 4;
		}else if(getChar == 'c'){//按摩等级3
			isMassage = true;
			massageFrequency = 8;
		}else if(getChar == 'd'){//按摩等级4
			isMassage = true;
			massageFrequency = 16;
		}else if(getChar == 'e'){//按摩等级5
			isMassage = true;
			massageFrequency = 32;
		}else if(getChar == 'f'){//按摩等级0(关闭按摩)
			isMassage = false;
			massageFrequency = 0;
			setMassageState(false);
		}else if(getChar == '6'){//消息通知
			isNotification = true;
			isNotificating = false;
			notificationViberaLeft = 5;
			notificationFrequency = 8;
		}else if(getChar == '7'){//开蜂鸣器
			isBuzzer = true;
		}else if(getChar == '8'){//关蜂鸣器
			isBuzzer = false;
			setBuzzerState(false);
		}else if(getChar == '4'){//开始加热
			setWarmState(true);
		}else if(getChar == '5'){//停止加热
			setWarmState(false);
		}
	}
}

void sendTempData()
{
	usart1SendString("|");
	usart1SendByte(0x30+(dat_humiture[0]%1000/100));
	usart1SendByte(0x30+(dat_humiture[0]%100/10));
	usart1SendByte(0x30+(dat_humiture[0]%10));
	usart1SendByte('.');
	usart1SendByte(0x30+(dat_humiture[1]%1000/100));
	usart1SendByte(0x30+(dat_humiture[1]%100/10));
	usart1SendByte(0x30+(dat_humiture[1]%10));
	usart1SendString("|");
	usart1SendByte(0x30+(dat_humiture[2]%1000/100));
	usart1SendByte(0x30+(dat_humiture[2]%100/10));
	usart1SendByte(0x30+(dat_humiture[2]%10));
	usart1SendByte('.');
	usart1SendByte(0x30+(dat_humiture[3]%1000/100));
	usart1SendByte(0x30+(dat_humiture[3]%100/10));
	usart1SendByte(0x30+(dat_humiture[3]%10));
	usart1SendByte('|');
	usart1SendByte(0x30+(ADC_ConvertedValue/10000));
	usart1SendByte(0x30+(ADC_ConvertedValue%10000/1000));
	usart1SendByte(0x30+(ADC_ConvertedValue%1000/100));
	usart1SendByte(0x30+(ADC_ConvertedValue%100/10));
	usart1SendByte(0x30+(ADC_ConvertedValue%10));
}

//----- 闪烁定时器中断
void TIM6_DAC_IRQHandler(){
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)){
		count++;
		if(count>=countMax)
		{
			count = 0;
		}
		TIM_ClearFlag(TIM6,TIM_FLAG_Update);
	}
	
}

void SysTick_Handler()
{
	delayTimeDecrement();
}
