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
#include "stdlib.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MEMORY_SIZE 0x1FFFFF // Total size of the SST25VF016B memory in bytes
#define PAGE_SIZE 256 // Size of a memory page in bytes
#define BUFFER_SIZE 1024 // Size of the buffer to read data from the memory

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

uint8_t dataI[BUFFER_SIZE] = { 0 };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

void readDataFromFlash();
void writeByteDataInFlash();
void writeAAIDataInFlash(char* message);
void openBlockDataProtectionFlash();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef enum
{
	NO =  0,
	YES = 1
} read_data_t;

read_data_t readData = NO;

char textForSend[] = 	" From: Svyatoslav Dombrovskyi, svyat.xxi@gmail.com \r\n"
						" Mentor: Artem Dovhal, artem.dovhal@globallogic.com \r\n"
						" Date: 28.03.2023 \r\n"
						" TIME CAPSULE \r\n"
						"Real human being \r\n"
						"And a real hero \r\n"
						"Back against the wall and odds \r\n"
						"With the strength of a will and a cause \r\n"
						"Your pursuits are called outstanding \r\n"
						"You're emotionally complex \r\n"
						"Against the grain of dystopic claims \r\n"
						"Not the thoughts your actions entertain \r\n"
						"And you have proved to be \r\n"
						"A real human being and a real hero \r\n"
						"Real human being and a real hero x3 \r\n"
						"Real human being \r\n";
/*						"A pilot on a cold, cold morn' \r\n"
						"155 people on board \r\n"
						"All safe and all rescued \r\n"
						"From the slowly sinking ship \r\n"
						"Water warmer than his head so cool \r\n"
						"In that tight bind knew what to do \r\n"
						"And you have proved to be \r\n"
						"A real human being and a real hero \r\n"
						"Real human being and a real hero x3 \r\n"
						"Real human being x3 \r\n";*/

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
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  // CS = HIGH
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
  HAL_Delay(100);

  // Prepare Transmit and Receive Arrays
  uint8_t TransmitArray[20] = { 0 };
  uint8_t ReceiveArray[20]  = { 0 };

  //Prepare READ-ID command
  TransmitArray[0] = 0x05;

  while (1)
  {
		uint8_t rcvBuf[1];
		HAL_StatusTypeDef result;

		result = HAL_UART_Receive(&huart3, rcvBuf, 1, 10);

		if (result == HAL_OK)
		{
			if (rcvBuf[0] == 's') //show data
			{
				if (readData == YES)
				{
					HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
					HAL_UART_Transmit(&huart3, (uint8_t *)dataI, BUFFER_SIZE, HAL_MAX_DELAY);
				}
				else
				{
					char message[] = "First read data from flash\r\n";
					HAL_UART_Transmit(&huart3, (uint8_t *) message, sizeof(message), 100);
				}
				HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'r') //read data from flash
			{
				  HAL_UART_Transmit(&huart3, (uint8_t *)"Reading...\r\n", 12, 100);
				  readDataFromFlash();
				  HAL_UART_Transmit(&huart3, (uint8_t *)"\r\ndata was read", 16, 100);
				  HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'w') //write data in flash
			{
				  HAL_UART_Transmit(&huart3, (uint8_t *)"Writing...", 10, 100);
				  writeByteDataInFlash();
				  HAL_UART_Transmit(&huart3, (uint8_t *)"Data was write", 14, 100);
				  HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'a') //auto address increment write data in flash
			{
				  HAL_UART_Transmit(&huart3, (uint8_t *)"Writing AAI...", 13, 100);
				  writeAAIDataInFlash(textForSend);
				  HAL_UART_Transmit(&huart3, (uint8_t *)"Data was write", 14, 100);
				  HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'c') //auto address increment write data in flash
			{
				  TransmitArray[0] = 0x06; //WREN
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				  HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

				  TransmitArray[0] = 0x60;  //clear cheap

				  HAL_UART_Transmit(&huart3, (uint8_t *)"Clear chip...", 13, 100);

				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				  HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 1000);
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

				  HAL_UART_Transmit(&huart3, (uint8_t *)"Cleared chip...", 15, 100);
				  HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'x') //read register data from flash
			{
				  TransmitArray[0] = 0x05;

				  HAL_UART_Transmit(&huart3, (uint8_t *)"Read register data...\r\n", 23, 100);

				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				  HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
				  HAL_SPI_Receive(&hspi1, ReceiveArray, 1, 100);
				  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

				  char binString[8] = { '0' };
				  itoa(ReceiveArray[0], binString, 2);
				  HAL_UART_Transmit(&huart3, (uint8_t *)binString, strlen(binString), 100);
				  HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'e') //WREN
			{
				TransmitArray[0] = 0x06;

				HAL_UART_Transmit(&huart3, (uint8_t *)"WREN...", 8, 100);

				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
				HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

				HAL_UART_Transmit(&huart3, (uint8_t *)"WREN.OK", 8, 100);
				HAL_Delay(1000);
			}

			if (rcvBuf[0] == 'o') //Open block write protection
			{
				HAL_UART_Transmit(&huart3, (uint8_t *)"Open BWP...\r\n", 13, 100);
				openBlockDataProtectionFlash();
				HAL_UART_Transmit(&huart3, (uint8_t *)"BWP is disable", 14, 100);
				HAL_Delay(1000);
			}
		}

	HAL_Delay(100);

    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD14 PD15 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void readDataFromFlash()
{
	uint8_t buffer[BUFFER_SIZE] = { 0 };
	uint32_t address;

	for (uint32_t j = 0; j < BUFFER_SIZE; ++j)
	{
		dataI[j] = 0;
	}

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); //red LED on
	address = 0;
	for (address = 0; address < MEMORY_SIZE+1; address += BUFFER_SIZE)
	{
		// Set the chip select signal low to select the SST25VF016B memory
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);

		// Send the Read command (0x03) and address to the memory
		uint8_t address_bytes[20];
		address_bytes[0] = (address >> 16) & 0xFF;
		address_bytes[1] = (address >> 8) & 0xFF;
		address_bytes[2] = address & 0xFF;
		HAL_SPI_Transmit(&hspi1, (uint8_t[]) {0x03, address_bytes[0], address_bytes[1], address_bytes[2]}, 4, 100);

		// Receive data from the memory into the buffer
		HAL_SPI_Receive(&hspi1, buffer, BUFFER_SIZE, 1000);

		// Set the chip select signal high to deselect the SST25VF016B memory
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);

		for (uint16_t i = 0; i < BUFFER_SIZE; ++i)
		{
			if (buffer[i] != 255)
			{
				dataI[dataCounter++] = buffer[i];
			}
		}
  }

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); //red LED off

  readData = YES;

  HAL_UART_Transmit(&huart3, (uint8_t *)dataI, BUFFER_SIZE, 1000);
}

void writeByteDataInFlash()
{
	uint8_t TransmitArray[5] = { 0 };

	//Prepare WRITE-ENABLE (WREN) command
	TransmitArray[0] = 0x06;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
	HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH

	//Prepare BYTE-PROGRAM command with address and data
	TransmitArray[0] = 0x02;
	TransmitArray[1] = 0x00;
	TransmitArray[2] = 0x00;
	TransmitArray[3] = 0x02;
	//data
	TransmitArray[4] = 0x23;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);	// CS = LOW
	HAL_SPI_Transmit(&hspi1, TransmitArray, 5, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);		// CS = HIGH

	//Prepare WRITE-DISABLE (WRDI) command
	TransmitArray[0] = 0x04;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
	HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH

}

void writeAAIDataInFlash(char* message)
{
	uint8_t TransmitArray[6] = { 0 };
	uint8_t counterAddr = 0;

//	//Enable SO to output (EBSY) command
//	TransmitArray[0] = 0x70;
//
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
//	HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH

	uint8_t address[3] = { 0 };

	address[0] = 0x00;
	address[1] = 0x00;
	address[2] = 0x00;

	uint16_t i = 0;

	while(i < strlen(message))
	{
		//Prepare WRITE-ENABLE (WREN) command
		TransmitArray[0] = 0x06;

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
		HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH

		//AII command
		TransmitArray[0] = 0xAD;
		TransmitArray[1] = address[0];
		TransmitArray[2] = address[1];
		TransmitArray[3] = address[2];
		//data
		TransmitArray[4] = message[i++];
		TransmitArray[5] = message[i++];

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);	// CS = LOW
		HAL_SPI_Transmit(&hspi1, TransmitArray, 6, 100);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);		// CS = HIGH

		for ( ; i < strlen(message); i+=2)
		{
			//AAI command
			TransmitArray[0] = 0xAD;
			//data
			TransmitArray[1] = message[i];
			TransmitArray[2] = message[i+1];

			if (message[i] == '\n')
			{
				TransmitArray[2] = 0;
				i--;
			}

			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);	// CS = LOW
			HAL_SPI_Transmit(&hspi1, TransmitArray, 3, 100);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);		// CS = HIGH

			if (message[i+1] == '\n' || message[i] == '\n')
			{
				counterAddr++;
				address[0] = (0x10 * counterAddr) / 100;
				address[1] = (0x10 * counterAddr) % 100;
				address[2] = 0x00;
				i+=2;
				break;
			}

		}

		//Prepare WRITE-DISABLE (WRDI) command
		TransmitArray[0] = 0x04;

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
		HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH
	}

//	//Disable SO as RY/BY (DBSY) command
//	TransmitArray[0] = 0x80;
//
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
//	HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH
}

void openBlockDataProtectionFlash()
{
	uint8_t TransmitArray[2] = { 0 };

	//Write-Status-Register
	TransmitArray[0] = 0x50;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
	HAL_SPI_Transmit(&hspi1, TransmitArray, 1, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH

	//Enable-Write-Status-Registerb
	TransmitArray[0] = 0x01;
	TransmitArray[1] = 0x00; //all 0

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET); 	// CS = LOW
	HAL_SPI_Transmit(&hspi1, TransmitArray, 2, 100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET); 	// CS = HIGH
}

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
