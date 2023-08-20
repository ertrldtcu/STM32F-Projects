/*
 * transmit_ddma_circular.h
 *
 *  Created on: Aug 20, 2023
 *      Author: edutc
 */

#ifndef INC_TRANSMIT_DMA_CIRCULAR_H_
#define INC_TRANSMIT_DMA_CIRCULAR_H_

typedef struct {
	uint8_t *queue[30];
	int head;
	int tail;
} TX_Circular_Buffer_TypeDef;

void int_to_str(uint16_t, uint8_t*);
uint8_t len(uint8_t*);
void Transmit_Next_Available(UART_HandleTypeDef*);
void Transmit_DMA(UART_HandleTypeDef*, uint8_t*);

#endif /* INC_TRANSMIT_DMA_CIRCULAR_H_ */
