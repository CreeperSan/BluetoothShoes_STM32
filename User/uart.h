#ifndef __UART_H_
#define __UART_H_

#include "stm32f4xx.h"

//此处定义要使用的引脚
#define GPIO_PIN_USART1_TX		GPIO_Pin_9		//串口发送
#define GPIO_PIN_USART1_RX		GPIO_Pin_10		//串口接收


void initUSART1(void);//初始化串口
void usart1SendByte(uint8_t dat);//发送单个字节
void usart1SendString(char *dat);//发送字符串


#endif



