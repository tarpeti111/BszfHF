/*
 * uart.h
 *
 *  Created on: Nov 16, 2025
 *      Author: tarpe
 */

#ifndef UART_H_
#define UART_H_

#include "em_usart.h"

void uart_init(void);
char USART_RxNonblocking(USART_TypeDef *usart);
void USART_TxString(USART_TypeDef * usart, const char* str);

#endif /* UART_H_ */
