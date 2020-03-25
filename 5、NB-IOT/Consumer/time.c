#include "time.h"




//设置定时器
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{
	TimeType->TimeStart = HAL_GetTick();
	TimeType->TimeInter = TimeInter;
	
}

//比较定时器
uint8_t CompareTime(tsTimeType *TimeType)
{
	return ((HAL_GetTick()-TimeType->TimeStart) >= TimeType->TimeInter);
}











