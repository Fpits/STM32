/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "platform.h"
#include "sx1276-Hal.h"
#include "sx12xxEiger.h"
#include "sx1276-LoRa.h"
#include "sx1276-LoRaMisc.h"
#include "radio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define BUFFERSIZE      4

uint8_t PingMsg[] = "PING";
uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFERSIZE;
uint8_t Buffer[BUFFERSIZE];


#ifdef MASTER
uint8_t EnableMaster = true;
#else
uint8_t EnableMaster = false;
#endif


uint32_t Master_TxNumber = 0;
uint32_t Master_RxNumber = 0;

uint32_t Slave_TxNumber = 0;
uint32_t Slave_RxNumber = 0;


tRadioDriver *Radio = NULL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch,FILE *p)
{
    while(!(USART1->SR & (1<<7)));
    
    USART1->DR = ch;
    
    return ch;
}



/**************************************************************************
 * Name    @@Master_Task
 * Summary @@主机无线任务
 * Params  @@无
 * Return  @@无
 *************************************************************************/
void Master_Task(void)
{
  switch(Radio->Process())
  {
      case RF_RX_DONE:
        Radio->GetRxPacket(Buffer,&BufferSize);
        printf("Master RF_RX_DONE:%s\r\n",Buffer);
        if(strncmp((const char*)Buffer,(const char*)PongMsg,strlen((const char*)PongMsg)) == 0)
        {
          Master_RxNumber++;
          Radio->SetTxPacket(PingMsg,strlen((const char*)PingMsg));
        }
        HAL_Delay(500);
        break;
      case RF_TX_DONE:
        printf("Master RF_TX_DONE\r\n");
        Master_TxNumber++;
        Radio->StartRx();
        break;
      default :
      break;
  }
}


/**************************************************************************
 * Name    @@Slave_Task
 * Summary @@从机无线任务
 * Params  @@无
 * Return  @@无
 *************************************************************************/
void Slave_Task(void)
{
  switch(Radio->Process())
  {
      case RF_RX_DONE:
        Radio->GetRxPacket(Buffer,&BufferSize);
        printf("Slave RF_RX_DONE:%s\r\n",Buffer);
        if(strncmp((const char*)Buffer,(const char*)PingMsg,strlen((const char*)PingMsg)) == 0)
        {
          Slave_RxNumber++;
          Radio->SetTxPacket(PongMsg,strlen((const char*)PongMsg));
          HAL_Delay(500);
        }
        break;
      case RF_TX_DONE:
        printf("Slave RF_TX_DONE\r\n");
        Slave_TxNumber++;
        Radio->StartRx();
        break;
      default :
      break;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t RegVersion = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  HAL_SPI_MspInit(&hspi1);
  SX1276Read( REG_LR_VERSION, &RegVersion );
  if(RegVersion != 0x12)
  {
    printf("LoRa read Error !\r\n");
    printf("LoRa RegVersion = %d\r\n",RegVersion);
  }
  else
  {
    printf("LoRa read ok !\r\n");
    printf("LoRa RegVersion = %d\r\n",RegVersion);
  }
  
  Radio = RadioDriverInit();
  Radio->Init();

#ifdef MASTER
  Radio->SetTxPacket(PingMsg,strlen((const char*)PingMsg));
  printf("I am Master !\r\n");
#else
  Radio->StartRx();
  printf("I am Slave !\r\n");
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    if(EnableMaster == true)
    {
      Master_Task();
    }
    else
    {
      Slave_Task();
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
