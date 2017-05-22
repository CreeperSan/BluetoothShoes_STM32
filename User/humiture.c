
#include "stm32f4xx.h"
#include "humiture.h"
#include "define.h"
#include "delay.h"
#include "uart.h"

void initHumitureClock(void)//��ʼ��ʱ��
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
}
void initHumitureAsOutput(void)//����Ϊ���ģʽ
{
	GPIO_InitTypeDef gpioDef;
	gpioDef.GPIO_Mode = GPIO_Mode_OUT;
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_Pin = GPIO_Pin_Humiture;
	gpioDef.GPIO_PuPd = GPIO_PuPd_UP;
	gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Port_Humiture,&gpioDef);
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture);
}
void initHumitureAsInput(void)//����Ϊ����ģʽ
{
	GPIO_InitTypeDef gpioDef;
	gpioDef.GPIO_Mode = GPIO_Mode_IN;
	gpioDef.GPIO_Pin = GPIO_Pin_Humiture;
	gpioDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Port_Humiture,&gpioDef);
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture);
}

bool readHumiture(unsigned char humitureDat[5])//��ȡ����ֵ������
{
	unsigned char humitureBuffer[5];//��ʪ�����ݻ�����
	unsigned int tempCount;//��ʱ����������ֵ
	
	initHumitureAsOutput(); //�л������ģʽ
	GPIO_ResetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //������������18ms
	delay_us(20000); //��ʱ�ȴ�
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //��������20-40us
	delay_us(30); //��ʱ�ȴ�
	initHumitureAsInput(); //����Ϊ����ģʽ���ȴ���Ӧ
	//����Ƿ���40-50us�͵�ƽ��Ӧ���źţ����û��������ѭ��
	if( getHumiturePinStatus() == 0){
		tempCount = 0;
		while( getHumiturePinStatus() == 0){
			tempCount++;
			if(tempCount>100)return false;
			delay_us(10);
		}
	}else{
		return false;
	}
	usart1SendByte('*');
	//����Ƿ���40-50us�������źţ����û��������ѭ��
	tempCount = 0;
	while( getHumiturePinStatus() == 1 ){
		tempCount ++;
		if(tempCount>100)return false;
		delay_us(10);
	}
	
	usart1SendByte('#');
	//��ʼ��ȡ40bit����������
	humitureBuffer[0] = readHumitureByte();
	humitureBuffer[1] = readHumitureByte();
	humitureBuffer[2] = readHumitureByte();
	humitureBuffer[3] = readHumitureByte();
	humitureBuffer[4] = readHumitureByte();
	initHumitureAsOutput();//����Ϊ���ģʽ
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //��������
	//���������ȷ��
	if(humitureBuffer[4] == humitureBuffer[0]+humitureBuffer[1]+humitureBuffer[2]+humitureBuffer[3]){
		humitureDat[0] = humitureBuffer[0];
		humitureDat[1] = humitureBuffer[1];
		humitureDat[2] = humitureBuffer[2];
		humitureDat[3] = humitureBuffer[3];
		humitureDat[4] = humitureBuffer[4];
		usart1SendByte('@');
		return true;
	}else{
		humitureDat[0] = humitureBuffer[0];
		humitureDat[1] = humitureBuffer[1];
		humitureDat[2] = humitureBuffer[2];
		humitureDat[3] = humitureBuffer[3];
		humitureDat[4] = humitureBuffer[4];
		usart1SendByte('$');
		return false;
	}
}

static bool readHumitureByte()//��ȡ����һ���ֽڵ�����
{
	unsigned char i,result = 0;
	for(i = 0;i<8;i++){
		//���ȶ�ȡ��ʼ��ƽ
		while(getHumiturePinStatus() == 0);//ֱ���˳�
		//��ʱ��������0�ߵ�ƽ����ʱ��С������1�ߵ�ƽ����ʱ��
		delay_us(40);
		if(getHumiturePinStatus() == 1){//������Ǹߵ�ƽ����Ϊ����1
			while(getHumiturePinStatus() == 1);//�ȴ���һ���ݽ���
			//result |= 0x01<<(7-i);
			result|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� (Ұ��д�ģ���ʱ���ٿ�)
		}
	}
	return result;
}

static bool getHumiturePinStatus()//��ȡ�����߽ӿڵ�ƽ�������Ƚ�IO�����ó�����ģʽ��
{
	return GPIO_ReadInputDataBit(GPIO_Port_Humiture,GPIO_Pin_Humiture);
}





































