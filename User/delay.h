
#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f4xx.h"

void delay_ms(unsigned int time);
void delay_us(unsigned int time);

void initSystemTick(void);
static void delayUs(__IO u32 mTime);
void delayTimeDecrement(void);

#endif










