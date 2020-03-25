#ifndef __TIME_H_
#define __TIME_H_
#include "stm32f1xx.h"



typedef struct
{
	uint32_t TimeStart;
	uint32_t TimeInter;
}tsTimeType;


void SetTime(tsTimeType *TimeType,uint32_t TimeInter);

uint8_t CompareTime(tsTimeType *TimeType);


#endif


