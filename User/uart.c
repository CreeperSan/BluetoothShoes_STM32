
#include "stm32f4xx.h"
#include "uart.h"

void initUSART1()
{
	GPIO_InitTypeDef gpioDef;//引脚
	USART_InitTypeDef usartDef;//串口
	NVIC_InitTypeDef nvicDef;//中断
	
	//-----此处开启时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//串口
	
	//-----GPIO的初始化
	gpioDef.GPIO_Mode = GPIO_Mode_AF;//模式设置为复用引脚功能
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_PuPd = GPIO_PuPd_UP;
	gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
	gpioDef.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,&gpioDef);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//配置复用功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//配置复用功能
	
	//-----串口配置
	usartDef.USART_BaudRate = 115200;//波特率
	usartDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制
	usartDef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//可接受，可发送
	usartDef.USART_Parity = USART_Parity_No;//无检验位
	usartDef.USART_StopBits = USART_StopBits_1;//1位停止位
	usartDef.USART_WordLength = USART_WordLength_8b;//字长为8位
	USART_Init(USART1,&usartDef);
	
	//-----接收中断配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvicDef.NVIC_IRQChannel = USART1_IRQn;
	nvicDef.NVIC_IRQChannelCmd = ENABLE;
	nvicDef.NVIC_IRQChannelPreemptionPriority = 1;
	nvicDef.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicDef);
	
	//-----使能
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //*RXNE接收缓冲非空
	USART_Cmd(USART1,ENABLE);//使能串口
}


void usart1SendByte(uint8_t dat)
{
	USART_SendData(USART1,dat);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}
void usart1SendString(char *dat)
{
	unsigned char i = 0;
	while( *(dat + i) != '\0' )
	{
		usart1SendByte(*(dat+i));
		i++;
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET );
}











