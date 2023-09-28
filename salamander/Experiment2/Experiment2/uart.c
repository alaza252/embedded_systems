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
uint8_t uart_receive(volatile UART_t *addr, uint8_t *rcvd_val) {
	uint8_t status;
	uint16_t timeout_counter = 0;
	do {
		status = uart_receive_nb(addr, rcvd_val);
		timeout_counter++;
	} while ((status & UART_RECEIVE_NB_INCOMPLETE_MASK) != 0 && timeout_counter != 0)

	if ((status & UART_RECEIVE_NB_INCOMPLETE_MASK) != 0) {
		// If the receive is still incomplete after the loop, a timeout must have occurred
		return UART_RECEIVE_TIMEOUT_MASK;
	}
	return status;
}

uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val)
{
	if ((addr->UCSRA & (1 << RXC)) == 0) { // TODO is 1 << RXC correct? Do we need to include avr or something?
		// receive complete not set,
		return UART_RECEIVE_NB_INCOMPLETE_MASK;
	}
	// receive is complete, so let's read UDR. When we read UDR, the RXC flag will automatically be cleared
	*rcvd_val = addr->UDR;
	return UART_RECEIVE_NB_SUCCESS;
}