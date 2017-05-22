
#ifndef __HUMITURE_H_
#define __HUMITURE_H_

#include "define.h"

#define GPIO_Port_Humiture		GPIOH
#define GPIO_Pin_Humiture 		GPIO_Pin_8

void initHumitureClock(void);
void initHumitureAsOutput(void);
void initHumitureAsInput(void);

static bool getHumiturePinStatus(void);
static bool readHumitureByte(void);
bool readHumiture(unsigned char humitureDat[5]);




#endif





























