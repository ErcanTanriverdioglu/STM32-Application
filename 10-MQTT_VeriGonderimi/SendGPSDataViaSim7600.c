/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
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
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

	/*SIM MODÜLÜ-CLOUD BAĞLANTISI-VERİ GÖNDERİMİ İÇİN DEĞİŞKENLER*/

const char url[] = "tcp://rattlesnake.rmq.cloudamqp.com";	
const int  port =	1883;									
const char kullaniciadi[] = "xxxxxxxx";						
const char sifre[] = "12345";								
const char kuyruk[] = "kuyruk1";							
char ATData[150];											
uint8_t Buffer[300] = {0};									
uint8_t ATReady = 0;										
uint8_t CREGReady = 0;										
uint8_t SimError = 0;										
char DataBuffer[750];										

					/*SIM MODÜLÜ-GPS VERİSİ İÇİN DEĞİŞKENLER*/

char GPSBuffer[300];										
int  i=0,j=0,x=0,y=0;										
char RawData[9][15]={0};									
char Latitude[15];											
char N[15];													
char Longitude[15];										
char E[15];													
char Date[15];												
char Time[15];												
char Altitude[15];											
char Speed[15];												
char Empty[15];												


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void SIMVeriGonder(char *komut)
{
   memset(Buffer,0,sizeof(Buffer));
   HAL_UART_Transmit(&huart1,(uint8_t *)komut,strlen(komut),100);
   HAL_UART_Receive (&huart1, Buffer, 100, 100);
   if(strstr((char *)Buffer,"ERROR")){SimError = 1;}
   else{SimError = 0;}
}

void MQTT_Protocol_Settings(void)
{
    ATReady = 0;
    CREGReady = 0;

    SIMVeriGonder("AT+CMQTTDISC=0,120\r\n");
    SIMVeriGonder("AT+CMQTTREL=0\r\n");
    SIMVeriGonder("AT+CMQTTSTOP\r\n");

    SIMVeriGonder("AT\r\n");
    HAL_Delay(50);
    if(strstr((char *)Buffer,"OK"))
    {
      ATReady = 1;
    }
  if(ATReady)
  {
    SIMVeriGonder("AT+CREG?\r\n");
    if(strstr((char *)Buffer,"+CREG: 0,1"))
     {
        CREGReady = 1;
     }
  }
  if(CREGReady)
  {
   SIMVeriGonder("AT+CVAUXV=3050\r\n");
   SIMVeriGonder("AT+CVAUXS=1\r\n");
   SIMVeriGonder("AT+CMQTTSTART\r\n");
   SIMVeriGonder("AT+CMQTTACCQ=0,\"kullanici\"\r\n");
   sprintf(ATData,"AT+CMQTTCONNECT=0,\"%s:%d\",60,1,\"%s\",\"%s\"\r\n",url,port,kullaniciadi,sifre);
   SIMVeriGonder(ATData);
   HAL_Delay(2000);
  }
}


void SIM_ReadGPS(void)
{
   SIMVeriGonder("AT+CGPS=1\r\n");
   SIMVeriGonder("AT+CGPSINFO\r\n");
   if(strstr((char *)Buffer,"CGPSINFO:"))
    {
   sprintf(GPSBuffer,Buffer);
   SIM_GPS_Edit();
   memset(GPSBuffer,0,strlen(GPSBuffer));
    }
}

void MQTT_Protocol_Send(void)
{
   sprintf(ATData,"AT+CMQTTTOPIC=0,%d\r\n",strlen(kuyruk));
   SIMVeriGonder(ATData);
   sprintf(ATData,"%s\r\n",kuyruk);
   SIMVeriGonder(ATData);
   sprintf(ATData,"AT+CMQTTPAYLOAD=0,%d\r\n",strlen(DataBuffer));
   SIMVeriGonder(ATData);
   SIMVeriGonder(DataBuffer);
   SIMVeriGonder("AT+CMQTTPUB=0,1,60\r\n");
}


void SIM_GPS_Edit(void)
{
	for(i=0;GPSBuffer[j]!=':';j++)
		HAL_Delay(50);

	while(GPSBuffer[i]!='\0')
	{
		while(GPSBuffer[j]!=','&& GPSBuffer[j]!='\0')
		{
			RawData[x][y]=GPSBuffer[j];
			j++;
			y++;
			HAL_Delay(50);
		}
		if(GPSBuffer[j]==',')
		{
			x++;
			y=0;
			j++;
			HAL_Delay(50);
		}
		i++;
		HAL_Delay(50);
	}

	for(x=0;x<9;x++)
	{
		for(y=0;y<=12;y++)
		{
			if(x==0)
				Latitude[y]=RawData[x][y];
			if(x==1)
				N[y]=RawData[x][y];
			if(x==2)
				Longitude[y]=RawData[x][y];
			if(x==3)
				E[y]=RawData[x][y];
			if(x==4)
				Date[y]=RawData[x][y];
			if(x==5)
				Time[y]=RawData[x][y];
			if(x==6)
				Altitude[y]=RawData[x][y];
			if(x==7)
				Speed[y]=RawData[x][y];
			if(x==8)
				Empty[y]=RawData[x][y];
		}
	}
	i=j=x=y=0;
	for(int a=0;a<=strlen(Latitude);a++)
		{Latitude[a]=Latitude[a+2];
		Latitude[a+2]='0';
		}
}

void SetData(void)
{
	sprintf(DataBuffer,"%s,%s,%s,%s,%s",Latitude,Longitude,Date,Time,Speed);
	MQTT_Protocol_Send();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  MQTT_Protocol_Settings();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  while(!CREGReady)
	  {
		  MQTT_Protocol_Settings();
	  }

	  SIM_ReadGPS();
	  SetData();


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  /** Initializes the CPU, AHB and APB buses clocks
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
