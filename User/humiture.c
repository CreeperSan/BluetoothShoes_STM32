
#include "stm32f4xx.h"
#include "humiture.h"
#include "define.h"
#include "delay.h"
#include "uart.h"

void initHumitureClock(void)//初始化时钟
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
}
void initHumitureAsOutput(void)//设置为输出模式
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
void initHumitureAsInput(void)//设置为输入模式
{
	GPIO_InitTypeDef gpioDef;
	gpioDef.GPIO_Mode = GPIO_Mode_IN;
	gpioDef.GPIO_Pin = GPIO_Pin_Humiture;
	gpioDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Port_Humiture,&gpioDef);
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture);
}

bool readHumiture(unsigned char humitureDat[5])//读取并赋值到数据
{
	unsigned char humitureBuffer[5];//温湿度数据缓冲区
	unsigned int tempCount;//临时计数器计数值
	
	initHumitureAsOutput(); //切换到输出模式
	GPIO_ResetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //主机拉低至少18ms
	delay_us(20000); //延时等待
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //主机拉高20-40us
	delay_us(30); //延时等待
	initHumitureAsInput(); //设置为输入模式，等待响应
	//检测是否有40-50us低电平响应信信号，如果没有则跳出循环
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
	//检测是否有40-50us的拉高信号，如果没有则跳出循环
	tempCount = 0;
	while( getHumiturePinStatus() == 1 ){
		tempCount ++;
		if(tempCount>100)return false;
		delay_us(10);
	}
	
	usart1SendByte('#');
	//开始读取40bit的所有数据
	humitureBuffer[0] = readHumitureByte();
	humitureBuffer[1] = readHumitureByte();
	humitureBuffer[2] = readHumitureByte();
	humitureBuffer[3] = readHumitureByte();
	humitureBuffer[4] = readHumitureByte();
	initHumitureAsOutput();//重置为输出模式
	GPIO_SetBits(GPIO_Port_Humiture,GPIO_Pin_Humiture); //主机拉高
	//检查数据正确性
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

static bool readHumitureByte()//读取其中一个字节的数据
{
	unsigned char i,result = 0;
	for(i = 0;i<8;i++){
		//首先读取起始电平
		while(getHumiturePinStatus() == 0);//直到退出
		//延时大于数据0高电平持续时间小于数据1高电平持续时间
		delay_us(40);
		if(getHumiturePinStatus() == 1){//如果还是高电平，则为数据1
			while(getHumiturePinStatus() == 1);//等待这一数据结束
			//result |= 0x01<<(7-i);
			result|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 (野火写的，到时候再看)
		}
	}
	return result;
}

static bool getHumiturePinStatus()//获取单总线接口电平（需事先将IO口配置成输入模式）
{
	return GPIO_ReadInputDataBit(GPIO_Port_Humiture,GPIO_Pin_Humiture);
}





































