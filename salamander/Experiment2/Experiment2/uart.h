/*
 * uart.h
 *
 * Created: 9/26/2023 4:35:46 PM
 *  Author: Lavender Shannon, Sam Stockmann
 */

#ifndef UART_H
#define UART_H

typedef struct UART
{
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t UCSRD;
	volatile uint8_t BRRL;
	volatile uint8_t BRRH;
	volatile uint8_t UDR;
} UART_t;

#define UART0 ((volatile UART_t*) &UCSR0A)
#define UART1 ((volatile UART_t*) &UCSR1A)
#define UART2 ((volatile UART_t*) &UCSR2A)

void uart_init(volatile UART_t *addr, uint32_t baud_rate);

uint8_t uart_transmit(volatile UART_t *addr, uint8_t send_val);

uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val);


#endif