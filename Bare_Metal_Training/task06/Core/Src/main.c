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
#include "stdlib.h"

#include "led_controller.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RxBuf_SIZE   10
#define MainBuf_SIZE 20

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */

led_controller_t pwm_controler;
uint8_t selectedLedNumber;

uint8_t RxBuf[RxBuf_SIZE];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

void doComand(uint8_t* MainBuf);

void changeLedAllState(uint8_t* MainBuf);
void changeLedFrequency(uint8_t* MainBuf);
void changeLedSleepMode(uint8_t* MainBuf);
void setLedDelayAndDuty(uint8_t* MainBuf, uint8_t activeSent);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char COMMAND[] ="LED_ALL: {ON,OFF}\r\n"
				"LED_FREQ: {number between 25 and 1500}\r\n"
				"LED_SLEEP: {ON,OFF}\r\n"
				"LED_SET N: {N - number between 1 and 16}";

typedef enum
{
	LED_СOM_UNSELECTED = 0,
	LED_ALL =   1,
	LED_FREQ =  2,
	LED_SLEEP = 3,
	LED_SET_N = 4
} command_t;

command_t command = LED_СOM_UNSELECTED;

typedef enum
{
	DELAY = 0,
	DUTY = 1
} sent_user_delay_or_duty_t;

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_Delay(10);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxBuf, RxBuf_SIZE);
  __HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  uint8_t devId = 0x80;
  led_controller_init(&pwm_controler, devId);

  while (1)
  {
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 50000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void sendI2C(led_controller_t* self)
{
	HAL_I2C_Master_Transmit(&hi2c1, self->deviceID, (uint8_t*) &self->TxBuffer, 2, 1000);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART3)
	{
		uint8_t MainBuf[MainBuf_SIZE] = "";
		memcpy(MainBuf, RxBuf, Size);

		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, RxBuf, RxBuf_SIZE);
		__HAL_DMA_DISABLE_IT(&hdma_usart3_rx, DMA_IT_HT);

		doComand(MainBuf);

		if (!strcmp((char*)MainBuf, "COMMAND"))
		{
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)COMMAND, sizeof(COMMAND));
		}

		if (!strcmp((char*)MainBuf, "LED_ALL"))
		{
			command = LED_ALL;
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)"", 1);
		}
		else if (!strcmp((char*)MainBuf, "LED_FREQ"))
		{
			command = LED_FREQ;
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)"", 1);
		}
		else if (!strcmp((char*)MainBuf, "LED_SLEEP"))
		{
			command = LED_SLEEP;
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)"", 1);
		}
		else if (!strncmp((char*)MainBuf, "LED_SET", 7))
		{
			command = LED_SET_N;

			selectedLedNumber = atoi((char*)(MainBuf + 8)); //Number after LED_SET
			if (!(selectedLedNumber >=1 && selectedLedNumber <= 16))
			{
				HAL_UART_Transmit_IT(&huart3, (uint8_t *)"Incorrect select", 16);
			}
			else
			{
				HAL_UART_Transmit_IT(&huart3, (uint8_t *)"Set delay and next duty (0-100):", 32);
			}
		}
		else
		{
			char MessageUART[] = "Incorrect command\r\nType COMMAND for help";
			HAL_UART_Transmit_IT(&huart3, (uint8_t *)MessageUART, sizeof(MessageUART) - 1);
		}
	}

}

void doComand(uint8_t* MainBuf)
{
	static sent_user_delay_or_duty_t activeSent;

	switch (command)
	{
		case LED_СOM_UNSELECTED:
			//continue
			break;
		case LED_ALL:
			changeLedAllState(MainBuf);
			break;
		case LED_FREQ:
			changeLedFrequency(MainBuf);
			break;
		case LED_SLEEP:
			changeLedSleepMode(MainBuf);
			break;
		case LED_SET_N:
			setLedDelayAndDuty(MainBuf, activeSent);
			activeSent++;
			activeSent%=2;
			break;
		default:
			command = LED_СOM_UNSELECTED;
			break;
	}
}

void changeLedAllState(uint8_t* MainBuf)
{
	if (!strcmp((char*)MainBuf, "ON"))
	{
		led_controller_on_all(&pwm_controler);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED ALL ON", 10);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED ALL ON1", 11);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED ALL ON2", 11);
	}
	else if (!strcmp((char*)MainBuf, "OFF"))
	{
		led_controller_off_all(&pwm_controler);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED ALL OFF", 11);
	}
	else
	{
		command = LED_СOM_UNSELECTED;
	}
}

void changeLedFrequency(uint8_t* MainBuf)
{
	uint16_t frequency = atoi((char*)MainBuf);
	if (frequency > 25 && frequency < 1500)
	{
		led_controller_set_PWM_frequency(&pwm_controler, frequency);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"Frequency is set", 16);
	}
	else
	{
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"Frequency is incorrect", 22);
		command = LED_СOM_UNSELECTED;
	}
}

void changeLedSleepMode(uint8_t* MainBuf)
{
	if (!strcmp((char*)MainBuf, "ON"))
	{
		led_controller_sleep_enable(&pwm_controler);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED SLEEP ON", 12);
	}
	else if (!strcmp((char*)MainBuf, "OFF"))
	{
		led_controller_sleep_disable(&pwm_controler);
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"LED SLEEP OFF", 13);
	}
	else
	{
		command = LED_СOM_UNSELECTED;
	}
}

void setLedDelayAndDuty(uint8_t* MainBuf, uint8_t activeSent)
{
	static uint8_t DelayAndDuty[2];
	static char Message[2][12] = {"Delay is set", "Duty is set"};

	DelayAndDuty[activeSent] = atoi((char*)MainBuf);
	if (DelayAndDuty[activeSent] >= 0 && DelayAndDuty[activeSent] <= 100)
	{
		HAL_UART_Transmit_IT(&huart3, (uint8_t *) Message[activeSent], sizeof(Message[activeSent]));
	}
	else
	{
		HAL_UART_Transmit_IT(&huart3, (uint8_t *)"Delay is not correct", 20);
		command = LED_СOM_UNSELECTED;
		return;
	}

	if (activeSent == DUTY)
	{
		set_PWM_DelayAndDuty(&pwm_controler, DelayAndDuty[0], DelayAndDuty[1], selectedLedNumber-1);
		command = LED_СOM_UNSELECTED;
	}
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
