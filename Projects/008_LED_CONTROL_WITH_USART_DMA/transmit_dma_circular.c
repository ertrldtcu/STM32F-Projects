/*
 * transmit_dma_circular.c
 *
 *  Created on: Aug 20, 2023
 *      Author: edutc
 */
#include "stm32f1xx_hal.h"
#include "transmit_dma_circular.h"

TX_Circular_Buffer_TypeDef tx_buffer = { .queue = { 0 }, .head = 0, .tail = 0 };

void int_to_str(uint16_t num, uint8_t *str) {
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

uint8_t len(uint8_t *str) {
	uint8_t i = 0;
	while (*str != '\0') {
		i++;
		str++;
	}
	return i;
}

void Transmit_Next_Available(UART_HandleTypeDef *huart) {
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_TXE) == SET && tx_buffer.queue[tx_buffer.tail] != 0)
		HAL_UART_Transmit_DMA(huart, tx_buffer.queue[tx_buffer.tail], len(tx_buffer.queue[tx_buffer.tail]));
}

void Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *msg) {

	tx_buffer.queue[tx_buffer.head++] = msg;

	if (tx_buffer.head == 30)
		tx_buffer.head = 0;

	Transmit_Next_Available(huart);

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

	tx_buffer.queue[tx_buffer.tail++] = 0;

	if (tx_buffer.tail == 30)
		tx_buffer.tail = 0;

	Transmit_Next_Available(huart);
}
