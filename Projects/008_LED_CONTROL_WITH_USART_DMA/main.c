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
#include "transmit_dma_circular.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CMD_FORMAT		"syntax: LED:<state[0-1]>:<ledon_time_in_ms>:<ledoff_time_in_ms>\r\n"
#define CMD_ERR			"Wrong command syntax!\r\n"
#define LED_STATE		"Current led state: "
#define LED_STATE_ON	"On\r\n"
#define LED_STATE_OFF	"Off\r\n"
#define LED_TIME_ON		"Current led on time: "
#define LED_TIME_OFF	"Current led off time: "
#define MS				"ms\r\n"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t led_state = 1; // 0: led off, 1: led on
uint16_t ledon=500, ledoff=500; // ledon: on time as ms, ledoff: off time as ms
uint8_t rx_buffer[30];
uint8_t temp_buffer;
int head = 0, tail = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(temp_buffer != '\r' && temp_buffer != '\n')
		rx_buffer[head++] = temp_buffer;
	else {
		rx_buffer[head] = '\0';
		int match_count = 0; // increased by 1 after each successful reading, expected 3 after the reading is finished
		int temp_led_state = -1, temp_ledon = -1, temp_ledoff = -1; // checks if the read is successful by checking if this value is -1

		if(rx_buffer[0] == 'L' && rx_buffer[1] == 'E' && rx_buffer[2] == 'D' && rx_buffer[3] == ':') {

			// check led state arg is 0 or 1
			temp_led_state = (rx_buffer[4] == '0') ? (0) : (rx_buffer[4] == '1' ? 1 : -1);
			if (temp_led_state != -1)
				match_count++;

			// check if the character after the led status is ':'
			if(rx_buffer[5] == ':') {
				tail = 6;
				while(rx_buffer[tail] >= '0' && rx_buffer[tail] <= '9') { // check characters until non-digit
					if(temp_ledon == -1) temp_ledon = 0; // first digit found, -1 cleared
					temp_ledon *= 10;
					temp_ledon += rx_buffer[tail++] - '0';
				}
				if(temp_ledon != -1) // if the value read is different from -1 led on time arg is correct
					match_count++;
			}

			// check led off time arg is digit between two ':'
			if(rx_buffer[tail] == ':') {
				tail++;
				while(rx_buffer[tail] >= '0' && rx_buffer[tail] <= '9') { // check characters until non-digit
					if(temp_ledoff == -1) temp_ledoff = 0; // first digit found, -1 cleared
					temp_ledoff *= 10;
					temp_ledoff += rx_buffer[tail++] - '0';
				}
				if(temp_ledoff != -1) // if the value read is different from -1 led off time arg is correct
					match_count++;
			}

		}
		if (match_count == 3 && head == tail) {

			led_state = temp_led_state;
			Transmit_DMA(&huart1, (uint8_t *) LED_STATE);
			Transmit_DMA(&huart1, (uint8_t *) (led_state ? LED_STATE_ON : LED_STATE_OFF));

			ledon = temp_ledon;
			uint8_t dec_on[5];
			int_to_str(ledon, dec_on);
			Transmit_DMA(&huart1, (uint8_t *) LED_TIME_ON);
			Transmit_DMA(&huart1, (uint8_t *) dec_on);
			Transmit_DMA(&huart1, (uint8_t *) MS);

			ledoff = temp_ledoff;
			uint8_t dec_off[5];
			int_to_str(ledoff, dec_off);
			Transmit_DMA(&huart1, (uint8_t *) LED_TIME_OFF);
			Transmit_DMA(&huart1, (uint8_t *) dec_off);
			Transmit_DMA(&huart1, (uint8_t *) MS);
		}
		else {
			Transmit_DMA(&huart1, (uint8_t *) CMD_ERR);
			Transmit_DMA(&huart1, (uint8_t *) CMD_FORMAT);
		}
		while(head > 0)
			rx_buffer[head--] = '\0';

		rx_buffer[0] = '\0';
		head = 0;
		tail = 0;
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
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_DMA(&huart1, &temp_buffer, sizeof(temp_buffer));
  Transmit_DMA(&huart1, (uint8_t *) CMD_FORMAT);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (led_state) {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		  HAL_Delay(ledon);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
		  HAL_Delay(ledoff);
	  }
	  else
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
