#ifndef __OLEDKEY_H
#define __OLEDKEY_H
#include "gpio.h"


typedef struct
{

	uint8_t current;

	uint8_t up;

	uint8_t down;

	uint8_t enter;

	void (*current_operation)();

}key_table;


uint8_t key_pullup(GPIO_TypeDef  *GPIOx,uint16_t GPIO_Pin);
uint8_t key_pulldown(GPIO_TypeDef  *GPIOx,uint16_t GPIO_Pin);
uint8_t ReadKey(uint8_t mode);
uint8_t KeyFunction();




#endif


