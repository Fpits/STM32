#include "time.h"




//���ö�ʱ��
void SetTime(tsTimeType *TimeType,uint32_t TimeInter)
{
	TimeType->TimeStart = HAL_GetTick();
	TimeType->TimeInter = TimeInter;
	
}

//�Ƚ϶�ʱ��
uint8_t CompareTime(tsTimeType *TimeType)
{
	return ((HAL_GetTick()-TimeType->TimeStart) >= TimeType->TimeInter);
}











