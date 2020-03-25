#ifndef __LED_H_
#define __LED_H_
#include "stm32f1xx.h"


#define LED_NUMBER	3


typedef enum
{
	LED_NET = 0,
	LED_RX,
	LED_TX
}teLedNums;


typedef enum
{
	LED_STOP = 0,
	LED_RUN,
	LED_DELAY
}teLedTaskStatus;



void LedOn(teLedNums LedNums);
void LedOff(teLedNums LedNums);
void SetLedRun(teLedNums LedNums);
void LedTask(void);
void LedInit(void);


#endif

