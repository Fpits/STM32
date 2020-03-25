#include "oledkey.h"
#include "gpio.h"
#include "oled.h"
#include "bmp.h"
#include "string.h"


uint8_t func_index = 0;//函数索引
void (*current_operation_index)(void);//索引函数声明



void fun1(void)
{

	OLED_Clear();
//	printf("fun1\r\n");
//	uint8_t t;
//	OLED_ShowCHinese(0,0,0);//中
//	OLED_ShowCHinese(18,0,1);//景
//	OLED_ShowCHinese(36,0,2);//园
//	OLED_ShowCHinese(54,0,3);//电
//	OLED_ShowCHinese(72,0,4);//子
//	OLED_ShowCHinese(90,0,5);//科
//	OLED_ShowCHinese(108,0,6);//技
//	OLED_ShowString(12,3,"1.3 OLED TEST",16);

//	OLED_ShowString(0,6,"ASCII:",16);  
//	OLED_ShowString(63,6,"CODE:",16);  
//	OLED_ShowChar(48,6,t,16);//显示ASCII字符	   
//	t++;
//	if(t>'~')t=' ';
//	OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值
//	HAL_Delay(1000);


}

void fun2(void)
{
	uint8_t i;
	OLED_Clear();
	
	OLED_ShowString(25,0,"2020-1-28",10);
	OLED_ShowString(0,1,"----------------",10);
	OLED_ShowCHinese_Hx12(1,2,12,6);//最低
	OLED_ShowCHinese_Hx12(15,2,12,7);
	OLED_ShowString(0,4,"----------------",10);
	OLED_ShowCHinese_Hx12(1,5,12,4);//夜晚
	OLED_ShowCHinese_Hx12(15,5,12,5);
	OLED_ShowString(0,7,"----------------",10);
	
	//画点
	for(i=1;i<8;i++)
	{
		OLED_ShowString(61,i,".",10);
	}
	
	OLED_ShowCHinese_Hx12(67,2,12,6);//最高
	OLED_ShowCHinese_Hx12(81,2,12,8);
	
	OLED_ShowCHinese_Hx12(67,5,12,2);//白天
	OLED_ShowCHinese_Hx12(81,5,12,3);
}

void fun3(void)
{
	uint8_t i;
	OLED_Clear();
	
	OLED_ShowString(25,0,"2020-1-28",10);
	OLED_ShowString(0,1,"----------------",10);
	OLED_ShowCHinese_Hx12(1,2,12,6);//最低
	OLED_ShowCHinese_Hx12(15,2,12,7);
	OLED_ShowString(0,4,"----------------",10);
	OLED_ShowCHinese_Hx12(1,5,12,4);//夜晚
	OLED_ShowCHinese_Hx12(15,5,12,5);
	OLED_ShowString(0,7,"----------------",10);
	
	//画点
	for(i=1;i<8;i++)
	{
		OLED_ShowString(61,i,".",10);
	}
	
	OLED_ShowCHinese_Hx12(67,2,12,6);//最高
	OLED_ShowCHinese_Hx12(81,2,12,8);
	
	OLED_ShowCHinese_Hx12(67,5,12,2);//白天
	OLED_ShowCHinese_Hx12(81,5,12,3);
}


key_table code_table[30]=
{

	{0,1,0,4,(*fun1)},

	{1,2,0,8,(*fun2)},
	
	{2,0,1,8,(*fun3)},
                                                       
};


uint8_t key_pullup(GPIO_TypeDef  *GPIOx,uint16_t GPIO_Pin)
{
	HAL_Delay(10);
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == SET)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == SET)
		{
			while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == SET);
			printf("key_pullup\r\n");
			return 1;
		}
	}
	return 0;
}

uint8_t key_pulldown(GPIO_TypeDef  *GPIOx,uint16_t GPIO_Pin)
{
	HAL_Delay(10);
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == RESET)
	{
		HAL_Delay(20);
		if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == RESET)
		{
			while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == RESET);
			printf("key_pulldown\r\n");
			return 1;
		}
	}
	return 0;
}


uint8_t KeyFunction()
{
	if(key_pullup(GPIOE,KEY1_Pin) == 1)
	{
		return 1;
	}
	else if(key_pullup(GPIOE,KEY0_Pin) == 1)
	{
		return 2;
	}
}


/*

mode=0--单词扫描

mode=1--连续扫描

mode=0--按键按下到松开为单次扫描

mode=1--按键按下时到松开为多次扫描
*/
#define key1	HAL_GPIO_ReadPin(GPIOE,KEY1_Pin)
#define key0	HAL_GPIO_ReadPin(GPIOE,KEY0_Pin)
uint8_t ReadKey(uint8_t mode)
{

	static u8 key = 1;

	if(key==1&&(key1==0||key0==0))
	{
		HAL_Delay(10);
		key = 0;

		if(key1==0)
		{
			return 1;
		}
		else if(key0==0)
		{
			return 2;
		}
	}
	else if(key1==1&&key0==1)
	{
		key = 1;
	}
	if(mode==1)
	{
		key = 1;
	}
	return 0;

}











