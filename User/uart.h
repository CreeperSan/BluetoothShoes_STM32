#ifndef __UART_H_
#define __UART_H_

#include "stm32f4xx.h"

//�˴�����Ҫʹ�õ�����
#define GPIO_PIN_USART1_TX		GPIO_Pin_9		//���ڷ���
#define GPIO_PIN_USART1_RX		GPIO_Pin_10		//���ڽ���


void initUSART1(void);//��ʼ������
void usart1SendByte(uint8_t dat);//���͵����ֽ�
void usart1SendString(char *dat);//�����ַ���


#endif



