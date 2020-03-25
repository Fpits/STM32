//#include "User_usart.h"
//#include "usart.h"
//#include "cJSON.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>


//uint8_t Rdata[4096];
//uint8_t buff[2048];



//void doit(char *text)
//{
//	char *out;cJSON *json;
//	
//	json=cJSON_Parse(text);
//	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
//	else
//	{
//		out=cJSON_Print(json);
//		printf("json = %s\n",out);
//		cJSON *results = cJSON_GetObjectItem(json,"results");
//		if(results)
//		{
//			char *locationName = cJSON_GetObjectItem(results,"location")->valuestring;
//			printf("locationName = %s\n",locationName);
//			
//			cJSON *daily = cJSON_GetObjectItem(results,"daily");
//			
//			char *daily0 = cJSON_GetObjectItem(daily,"0")->valuestring;
//			printf("daily0 = %s\n",daily0);
//			cJSON *daily1 = cJSON_GetObjectItem(daily,"1");
//			cJSON *daily2 = cJSON_GetObjectItem(daily,"2");
//		}
//		cJSON_Delete(json);
//		printf("json = %s\n",out);
//		free(out);
//	}
//}



//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance == USART2)
//	{
//		//截取有效的json数据
//		char * index = strchr((const char *)Rdata,'{');
//		strcpy((char *)buff,(char *)index);
//		//json数据解析
//		doit((char *)buff);
//		
//		//清空接收的数组
//		memset(Rdata,0,sizeof(Rdata));
//		//开启接收中断
//		HAL_UART_Receive_IT(&huart2,Rdata,2096);
//	}
//}








