/*
 * uart.h
 *
 * Created: 9/26/2023 4:35:46 PM
 *  Author: Lavender Shannon, Sam Stockmann
 */

#ifndef UART_H
#define UART_H

#include "board.h"


#define UART_RECEIVE_SUCCESS 0
/** The mask value used in uart_receive_nb for an incomplete receive*/
#define UART_RECEIVE_INCOMPLETE_MASK (1 << 0)

/**
 * Initializes the UART with the given baud rate
 */
void uart_init(volatile UART_t *addr, uint32_t baud_rate);

/**
 * Blocks until the UART is able to send the given value on the given UART
 *
 * Current implementation always returns 0. This is a blocking function, but realistically it won't block for any considerable length of time.
 */
uint8_t uart_transmit(volatile UART_t *addr, uint8_t send_val);

/**
 * Waits until a character is available on the UART and returns it
 */
uint8_t uart_receive(volatile UART_t *addr);

/**
 * If the returned value is UART_RECEIVE_SUCCESS, rcvd_val will be set to contain the received byte.
 * If the returned value is not UART_RECEIVE_SUCCESS, it may have one or multiple bits set indicating the error(s) that occurred during receive
 *
 * This function checks to see if there is a value in the buffer to receive. If there is, it returns it immediately, otherwise a response containing the mask of UART_RECEIVE_INCOMPLETE_MASK is returned
 */
uint8_t uart_receive_nb(volatile UART_t *addr, uint8_t *rcvd_val);


#endif