/*
 * uart.h
 *
 * Created: 9/26/2023 4:35:46 PM
 *  Author: Lavender Shannon, Sam Stockmann
 */

#ifndef UART_H
#define UART_H

#include "board.h"

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

#define UART_RECEIVE_SUCCESS 0
/** The mask value used in uart_receive_nb for an incomplete receive*/
#define UART_RECEIVE_INCOMPLETE_MASK (1 << 0)
/** The mask value used in uart_receive for a timeout while trying to receive*/
#define UART_RECEIVE_TIMEOUT_MASK (1 << 0)

#define ASYNC_MODE (0 << 6)
#define SYNC_MODE (1 << 6)
#define SPI_MODE (3 << 6)

#define NO_PARITY (0 << 4)
#define EVEN_PARITY (2 << 4)
#define ODD_PARITY (3 << 4)

#define ONE_STOP_BIT (0 << 3)
#define TWO_STOP_BITS (1 << 3)

#define FIVE_DATA_BITS (0 << 1)
#define SIX_DATA_BITS (1 << 1)
#define SEVEN_DATA_BITS (2 << 1)
#define EIGHT_DATA_BITS (3 << 1)
#define NINE_DATA_BITS (3 << 1)

void uart_init(volatile UART_t *addr, uint32_t baud_rate);

uint8_t uart_transmit(volatile UART_t *addr, uint8_t send_val);

/**
 * If the returned value is UART_RECEIVE_NB_SUCCESS, rcvd_val will be set to contain the received byte.
 * If the returned value is not UART_RECEIVE_NB_SUCCESS, it may have one or multiple bits set indicating the error(s) that occurred during receive
 *
 * This function blocks until
 */
uint8_t uart_receive(volatile UART_t *addr, uint8_t *rcvd_val);

/**
 * If the returned value is UART_RECEIVE_NB_SUCCESS, rcvd_val will be set to contain the received byte.
 * If the returned value is not UARt_RECEIVE_NB_SUCCESS, it may have one or multiple bits set indicating the error(s) that occurred during receive
 */
uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val);


#endif