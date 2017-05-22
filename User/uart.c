
#include "stm32f4xx.h"
#include "uart.h"

void initUSART1()
{
	GPIO_InitTypeDef gpioDef;//����
	USART_InitTypeDef usartDef;//����
	NVIC_InitTypeDef nvicDef;//�ж�
	
	//-----�˴�����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//����
	
	//-----GPIO�ĳ�ʼ��
	gpioDef.GPIO_Mode = GPIO_Mode_AF;//ģʽ����Ϊ�������Ź���
	gpioDef.GPIO_OType = GPIO_OType_PP;
	gpioDef.GPIO_PuPd = GPIO_PuPd_UP;
	gpioDef.GPIO_Speed = GPIO_Speed_50MHz;
	gpioDef.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,&gpioDef);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);//���ø��ù���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);//���ø��ù���
	
	//-----��������
	usartDef.USART_BaudRate = 115200;//������
	usartDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������
	usartDef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//�ɽ��ܣ��ɷ���
	usartDef.USART_Parity = USART_Parity_No;//�޼���λ
	usartDef.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	usartDef.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ
	USART_Init(USART1,&usartDef);
	
	//-----�����ж�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	nvicDef.NVIC_IRQChannel = USART1_IRQn;
	nvicDef.NVIC_IRQChannelCmd = ENABLE;
	nvicDef.NVIC_IRQChannelPreemptionPriority = 1;
	nvicDef.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicDef);
	
	//-----ʹ��
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //*RXNE���ջ���ǿ�
	USART_Cmd(USART1,ENABLE);//ʹ�ܴ���
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











