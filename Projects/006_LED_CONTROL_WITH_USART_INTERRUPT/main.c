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
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define BUFFER_SIZE 64

typedef struct UART_Buffer_Type{
	uint8_t buffer[BUFFER_SIZE];
	uint32_t head_pointer;
	uint32_t tail_pointer;
}UART_Buffer_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define START_MSG 			"System starting..\r\n"
#define PROCESS_MSG 		"Command processing..\r\n"
#define LED_ON_TIME_MSG		"Current led on time: "
#define LED_OFF_TIME_MSG	"Current led off time: "
#define MS_MSG				"miliseconds\r\n"
#define COMMANDS_MSG		"Command syntax: ledon=???ms or ledoff=???ms\r\n"
#define WRONG_CMD_MSG		"Wrong command syntax!\r\n"
#define LED_ON_CMD_MSG		"Syntax: ledon=???ms\r\n"
#define WRONG_LED_ON_MSG	"Wrong led on command syntax!\r\n"
#define LED_OFF_CMD_MSG		"Syntax: ledoff=???ms\r\n"
#define WRONG_LED_OFF_MSG	"Wrong led off command syntax!\r\n"

#define LED_ON_PREFIX	"ledon="
#define LED_OFF_PREFIX	"ledoff="
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define STRLEN(str) (sizeof(str) - 1)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
/* USER CODE BEGIN PV */
volatile UART_Buffer_t UART_BufferRX;
int led_on_time = 250, led_off_time = 250;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void get_command(uint8_t * buffer) {
	int c = 0;
	while(UART_BufferRX.tail_pointer <= UART_BufferRX.head_pointer) {
		buffer[c] = UART_BufferRX.buffer[UART_BufferRX.tail_pointer];
		UART_BufferRX.tail_pointer++;
		c++;
	}
	buffer[c] = '\0';
}
void int_to_str(int num, uint8_t * str) {
    int i = 0;

    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0);

    int start = 0;
    int end = i - 1;
    while (start < end) {
    	uint8_t temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
    str[i] = '\0';
}
int starts_with(const char *restrict string, const char *restrict prefix) {
    while(*prefix)
        if(*prefix++ != *string++)
            return 0;
    return 1;
}
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Transmit(&huart2, (uint8_t *) START_MSG, STRLEN(START_MSG), 100);
  HAL_UART_Transmit(&huart2, (uint8_t *) COMMANDS_MSG, STRLEN(COMMANDS_MSG), 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
	  HAL_Delay(led_on_time);
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
	  HAL_Delay(led_off_time);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  SET_BIT(USART2->CR1, USART_CR1_RXNEIE);


 HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
 HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	uint32_t sr = USART2->SR;
	uint32_t cr = USART2->CR1;
	uint8_t data = 0;

	if ((sr & USART_SR_RXNE) != RESET && (cr & USART_CR1_RXNEIE) != RESET) {
		data = (uint8_t) USART2->DR;

		UART_BufferRX.buffer[UART_BufferRX.head_pointer] = data;
		UART_BufferRX.head_pointer++;

		if(UART_BufferRX.head_pointer == BUFFER_SIZE)
			UART_BufferRX.head_pointer = 0;

		if (data == '\r') {
			HAL_UART_Transmit(&huart2, (uint8_t *) PROCESS_MSG, STRLEN(PROCESS_MSG), 200);

			uint8_t * command = malloc(sizeof(uint8_t) * UART_BufferRX.head_pointer);
			get_command(command);

			if (starts_with((char *) command, LED_ON_PREFIX)) {
				int interval = 0;
				int i = STRLEN(LED_ON_PREFIX);
				while(command[i] >= '0' && command[i] <= '9' ) {
					interval *= 10;
					interval += command[i] - '0';
					i++;
				}
				uint8_t * interval_str = malloc(sizeof(uint8_t) * 9);
				int_to_str(interval, interval_str);
				if (command[i] == 'm' && command[i+1] == 's' && interval != 0) {
					led_on_time = interval;
					HAL_UART_Transmit(&huart2, (uint8_t *) LED_ON_TIME_MSG,STRLEN(LED_ON_TIME_MSG), 200);
					HAL_UART_Transmit(&huart2, interval_str, sizeof(uint8_t) * 9, 200);
					HAL_UART_Transmit(&huart2, (uint8_t *) MS_MSG, STRLEN(MS_MSG), 200);
				}
				else {
					HAL_UART_Transmit(&huart2, (uint8_t *) WRONG_LED_ON_MSG, STRLEN(WRONG_LED_ON_MSG), 200);
					HAL_UART_Transmit(&huart2, (uint8_t *) LED_ON_CMD_MSG, STRLEN(LED_ON_CMD_MSG), 200);
				}

			}
			else if (starts_with((char *) command, LED_OFF_PREFIX)) {
				int interval = 0;
				int i = STRLEN(LED_OFF_PREFIX);
				while(command[i] >= '0' && command[i] <= '9' ) {
					interval *= 10;
					interval += command[i] - '0';
					i++;
				}
				uint8_t * interval_str = malloc(sizeof(uint8_t) * 9);
				int_to_str(interval, interval_str);
				if (command[i] == 'm' && command[i+1] == 's' && interval != 0) {
					led_off_time = interval;
					HAL_UART_Transmit(&huart2, (uint8_t *) LED_OFF_TIME_MSG,STRLEN(LED_OFF_TIME_MSG), 200);
					HAL_UART_Transmit(&huart2, interval_str, sizeof(uint8_t) * 9, 200);
					HAL_UART_Transmit(&huart2, (uint8_t *) MS_MSG, STRLEN(MS_MSG), 200);
				}
				else {
					HAL_UART_Transmit(&huart2, (uint8_t *) WRONG_LED_OFF_MSG, STRLEN(WRONG_LED_OFF_MSG), 200);
					HAL_UART_Transmit(&huart2, (uint8_t *) LED_OFF_CMD_MSG, STRLEN(LED_OFF_CMD_MSG), 200);
				}
			}
			else {
				HAL_UART_Transmit(&huart2, (uint8_t *) WRONG_CMD_MSG, STRLEN(WRONG_CMD_MSG), 200);
				HAL_UART_Transmit(&huart2, (uint8_t *) COMMANDS_MSG, STRLEN(COMMANDS_MSG), 200);
			}
			for (int i = 0; i < BUFFER_SIZE; i++) {
				UART_BufferRX.buffer[i] = 0;
			}
			UART_BufferRX.head_pointer = 0;
			UART_BufferRX.tail_pointer = 0;
		}

	}
  /* USER CODE END USART2_IRQn 0 */
  // HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
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
