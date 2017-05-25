
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

unsigned char dat_humiture[5];		//��ʪ������
extern __IO uint16_t ADC_ConvertedValue;//AD��ԭʼ��ֵ
float ADC_Vol; 						//ת�����AD

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
	initTimTwinkle();//��ʼ����˸��ʱ��
	initUSART1();//��ʼ������
	initSystemTick();//��ʼ��ϵͳ�δ�ʱ�� (������ʱ)
	initLed();//��ʼ��LED
	initMassage();
	initNotification();
	setLedState(false);
	initBuzzer();
	Rheostat_Init();
	initHumitureClock();//��ʼ����ʪ��ʱ��
	initHumitureAsOutput();//��ʼ����ʪ�ȿ���
	initWarm();
	usart1SendString("System Start!");
	while(1)
	{
		//��ʪ�����ݲɼ�
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
		//LED����
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
		//����������
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
		//��Ϣ֪ͨ�𶯴���
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
		//��Ħ����
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


//����Ϊ�жϷ�����

void USART1_IRQHandler(void)
{
	uint8_t getChar;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)){
		getChar = USART_ReceiveData(USART1);
		usart1SendByte(getChar);
		if(getChar == '0'){//�ش��¶�
			sendTempData();
		}else if(getChar == '1'){//����
			isLed = true;
		}else if(getChar == '2'){//�ص�
			isLed = false;
		}else if(getChar == '3'){//LED֪ͨ��˸
			isLedNotification = true;
			isLedNotificating = false;
			ledNotificationLeft = 5;
		}else if(getChar == 'a'){//��Ħ�ȼ�1
			isMassage = true;
			massageFrequency = 2;
		}else if(getChar == 'b'){//��Ħ�ȼ�2
			isMassage = true;
			massageFrequency = 4;
		}else if(getChar == 'c'){//��Ħ�ȼ�3
			isMassage = true;
			massageFrequency = 8;
		}else if(getChar == 'd'){//��Ħ�ȼ�4
			isMassage = true;
			massageFrequency = 16;
		}else if(getChar == 'e'){//��Ħ�ȼ�5
			isMassage = true;
			massageFrequency = 32;
		}else if(getChar == 'f'){//��Ħ�ȼ�0(�رհ�Ħ)
			isMassage = false;
			massageFrequency = 0;
			setMassageState(false);
		}else if(getChar == '6'){//��Ϣ֪ͨ
			isNotification = true;
			isNotificating = false;
			notificationViberaLeft = 5;
			notificationFrequency = 8;
		}else if(getChar == '7'){//��������
			isBuzzer = true;
		}else if(getChar == '8'){//�ط�����
			isBuzzer = false;
			setBuzzerState(false);
		}else if(getChar == '4'){//��ʼ����
			setWarmState(true);
		}else if(getChar == '5'){//ֹͣ����
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

//----- ��˸��ʱ���ж�
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
