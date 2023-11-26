/*
 * UART.c
 *
 * Created: 9/26/2023 4:34:38 PM
 *  Author: Lavender Shannon, Sam Stockmann
 */

#include "uart.h"

// https://onlinedocs.microchip.com/pr/GUID-80B1922D-872B-40C8-A8A5-0CBE009FD908-en-US-3/index.html?GUID-6D0F74A4-A5A6-4207-A450-635C8547E437
void uart_init(volatile UART_t *addr, uint32_t baud_rate)
{
	addr -> UCSRA = 0; // This sets U2X to 0
	
	uint8_t u2x = (addr -> UCSRA & (1 << U2X)) != 0 ? 1 : 0; // read U2X anyway, so that if we change U2X in the future we have an up to date value
	uint16_t ubbr_val = (uint16_t) (((F_CPU / OSC_DIV) / (8UL * (2 - u2x) * baud_rate)) - 1);
	
	addr -> BRRH = ubbr_val >> 8;
	addr -> BRRL = ubbr_val & 0xFF;
	
	addr -> UCSRC = (ASYNC_MODE|NO_PARITY|ONE_STOP_BIT|EIGHT_DATA_BITS); //0X06
	
	addr -> UCSRB = (1 << RXEN)|(1 << TXEN); //0x18
}

uint8_t uart_transmit(volatile UART_t *addr, uint8_t send_val)
{
	uint8_t status;
	
	do {
		status = addr -> UCSRA;
	} while ((status & (1 << UDRE)) == 0);
	
	addr -> UDR = send_val;
	
	return 0;
}

uint8_t uart_receive(volatile UART_t *addr) {
	uint8_t status;
	uint8_t val;

	do {
		status = uart_receive_nb(addr, &val);
	} while ((status & UART_RECEIVE_INCOMPLETE_MASK) != 0);

	return val;
}

uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val)
{
	if ((addr->UCSRA & (1 << RXC)) == 0) {
		// receive complete not set,
		return UART_RECEIVE_INCOMPLETE_MASK;
	}
	// receive is complete, so let's read UDR. When we read UDR, the RXC flag will automatically be cleared
	*rcvd_val = addr->UDR;
	return UART_RECEIVE_SUCCESS;
}