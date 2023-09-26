/*
 * UART.c
 *
 * Created: 9/26/2023 4:34:38 PM
 *  Author: Lavender Shannon, Sam Stockmann
 */

#include "uart.h"

void uart_init(volatile UART_t *addr, uint32_t baud_rate)
{
	
}

uint8_t uart_transmit(volatile UART_t *addr, uint8_t send_val)
{
	return 0;
}

uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val)
{
	return 0;
}