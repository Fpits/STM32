#include "led.h"
#include "time.h"



static teLedTaskStatus LedTaskStatus[LED_NUMBER];
static tsTimeType TimeLeds[LED_NUMBER];

const uint16_t LedPins[LED_NUMBER] = 
{
	GPIO_PIN_0,
	GPIO_PIN_1,
	GPIO_PIN_2,
};


void LedOn(teLedNums LedNums)
{
	HAL_GPIO_WritePin(GPIOB,LedPins[LedNums],GPIO_PIN_RESET);
}


void LedOff(teLedNums LedNums)
{
	HAL_GPIO_WritePin(GPIOB,LedPins[LedNums],GPIO_PIN_SET);
}


void LedInit(void)
{
	int i;
	for(i = 0;i < LED_NUMBER;i++)
	{
		LedOff(1);
	}
}


void SetLedRun(teLedNums LedNums)
{
	LedTaskStatus[LedNums] = LED_RUN;
	//SetTime(&TimeLeds[LedNums],2000);
}


void LedTask(void)
{
	int i;
	
	for(i = 0;i < LED_NUMBER;i++)
	{
		if(LedTaskStatus[i] == LED_RUN)
		{
			LedOn(i);
			SetTime(&TimeLeds[i],100);
			LedTaskStatus[i] = LED_DELAY;
		}
		else if(LedTaskStatus[i] == LED_DELAY)
		{
			if(CompareTime(&TimeLeds[i]))
			{
				LedOff(1);
				LedTaskStatus[i] = LED_STOP;
			}
		}
	}
}


