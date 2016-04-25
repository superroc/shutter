#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "stm32f10x_type.h"

#define EN_USART1_RX 			1

void uart_init(u32 pclk2,u32 bound);
void uart_put_data_noint(uint8 *data, uint32 length);
void uart_puts_noint(char *s);

#endif

