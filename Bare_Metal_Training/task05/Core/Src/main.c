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
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

const float RESULUTION = 4096;
float voltage = 2.95;//3.3;


typedef enum
{
	  LED_OFF,
	  LED_ON
} led_status_t;

typedef enum
{
	  LED_Blue,
	  LED_Red,
	  LED_Orange,
	  LED_Green
} led_color_t;

led_status_t led_state[4] = {};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

char* getStatusText(led_status_t led_state);
char* convertVoltageToCelsius(float adcVoltage_Value);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  HAL_TIM_Base_Start(&htim2);
  HAL_ADC_Start_IT(&hadc1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	uint8_t rcvBuf[1];
	HAL_StatusTypeDef result;
	char* statusText;

	result = HAL_UART_Receive(&huart3, rcvBuf, 1, 10);

	if (result == HAL_OK)
	{
		switch (rcvBuf[0])
		{
			case 'b':
				HAL_GPIO_TogglePin(Blue_LED_GPIO_Port, Blue_LED_Pin);
				led_state[LED_Blue] = (led_state[LED_Blue] + 1) % 2;
				HAL_UART_Transmit(&huart3, (uint8_t *)"Blue ", 5, 10);

				statusText = getStatusText(led_state[LED_Blue]);
				HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
				break;

			case 'r':
				HAL_GPIO_TogglePin(Red_LED_GPIO_Port, Red_LED_Pin);
				led_state[LED_Red] = (led_state[LED_Red] + 1) % 2;
				HAL_UART_Transmit(&huart3, (uint8_t *)  "Red ", 4, 10);

				statusText = getStatusText(led_state[LED_Red]);
				HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
				break;

			case 'o':
				HAL_GPIO_TogglePin(Orange_LED_GPIO_Port, Orange_LED_Pin);
				led_state[LED_Orange] = (led_state[LED_Orange] + 1) % 2;
				HAL_UART_Transmit(&huart3, (uint8_t *)"Orange ", 7, 10);

				statusText = getStatusText(led_state[LED_Orange]);
				HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
				break;

			case 'g':
				HAL_GPIO_TogglePin(Green_LED_GPIO_Port, Green_LED_Pin);
				led_state[LED_Green] = (led_state[LED_Green] + 1) % 2;
				HAL_UART_Transmit(&huart3, (uint8_t *)"Green ", 6, 10);

				statusText = getStatusText(led_state[LED_Green]);
				HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
				break;

			default:
				HAL_UART_Transmit(&huart3, (uint8_t *)"\r\nUnexpCmd", 10, 10);
				break;
		}
	}

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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16000-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 5000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Green_LED_Pin|Orange_LED_Pin|Red_LED_Pin|Blue_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Green_LED_Pin Orange_LED_Pin Red_LED_Pin Blue_LED_Pin */
  GPIO_InitStruct.Pin = Green_LED_Pin|Orange_LED_Pin|Red_LED_Pin|Blue_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : SWT4_Pin SWT5_Pin SWT3_Pin SWT1_Pin */
  GPIO_InitStruct.Pin = SWT4_Pin|SWT5_Pin|SWT3_Pin|SWT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SWT2_Pin */
  GPIO_InitStruct.Pin = SWT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SWT2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	char* statusText;

	if (GPIO_Pin == SWT1_Pin)
	{
		HAL_GPIO_TogglePin(Blue_LED_GPIO_Port, Blue_LED_Pin);
		led_state[LED_Blue] = (led_state[LED_Blue] + 1) % 2;
		HAL_UART_Transmit(&huart3, (uint8_t *)"\r\nBlue ", 8, 10);

		statusText = getStatusText(led_state[LED_Blue]);
		HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
	}
	if (GPIO_Pin == SWT2_Pin)
	{
		//Debug_Button
		//voltage-=0.05;
	}
	if (GPIO_Pin == SWT3_Pin)
	{
		HAL_GPIO_TogglePin(Orange_LED_GPIO_Port, Orange_LED_Pin);
		led_state[LED_Orange] = (led_state[LED_Orange] + 1) % 2;
		HAL_UART_Transmit(&huart3, (uint8_t *)"\r\nOrange ", 9, 10);

		statusText = getStatusText(led_state[LED_Orange]);
		HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
	}
	if (GPIO_Pin == SWT4_Pin)
	{
		HAL_GPIO_TogglePin(Red_LED_GPIO_Port, Red_LED_Pin);
		led_state[LED_Red] = (led_state[LED_Red] + 1) % 2;
		HAL_UART_Transmit(&huart3, (uint8_t *)"\r\nRed ", 6, 10);

		statusText = getStatusText(led_state[LED_Red]);
		HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
	}
	if (GPIO_Pin == SWT5_Pin)
	{
		HAL_GPIO_TogglePin(Green_LED_GPIO_Port, Green_LED_Pin);
		led_state[LED_Green] = (led_state[LED_Green] + 1) % 2;
		HAL_UART_Transmit(&huart3, (uint8_t *)"\r\nGreen ", 8, 10);

		statusText = getStatusText(led_state[LED_Green]);
		HAL_UART_Transmit(&huart3, (uint8_t *)  statusText, 3, 10);
	}

}

char* getStatusText(led_status_t led_state)
{
	switch (led_state) {
		case LED_ON:
			return "ON";
			break;

		case LED_OFF:
			return "OFF";
			break;

		default:
			return "Error";
			break;
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	float adc_Value = (float)HAL_ADC_GetValue(&hadc1);
	char* txbuf = convertVoltageToCelsius(adc_Value);

	HAL_UART_Transmit(&huart3, (uint8_t*)txbuf, 7, 10);
}


// Celsius from 0 to 99.99
char* convertVoltageToCelsius(float adcVoltage_Value)
{
	float celsius;
	float stepVoltage = RESULUTION / voltage;

	celsius = (2.02 - adcVoltage_Value / stepVoltage) / 0.02;

	static char txbuf[7];
	txbuf[0] = '\r';
	txbuf[1] = '\n';
	txbuf[2] = (char) (celsius / 10) % 10 + '0';
	txbuf[3] = (char)celsius % 10 + '0';
	txbuf[4] = '.';
	txbuf[5] = (char)(celsius*10) % 10 + '0';
	txbuf[6] = (char)(celsius*100) % 10 + '0';

	return txbuf;
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
