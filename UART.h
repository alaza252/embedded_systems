/*
 * UART.h
 *
 * Created: 9/29/2023 2:15:30 PM
 *  Author: Alisa Lazareva and Drew Schulte
 */ 


#ifndef UART_H_
#define UART_H_

#include "board.h"







 /*
 * UART_init( volatile UART_t *UART_addr, uint32_t baud_rate)
 * --------------------
 *  Parameters: UART Address and the Baud Rate get passed in
 *
 *  We calculate the UBBR Value using the baud rate
 *  We set up the message settings
 *  returns: 0
 */
uint8_t UART_init( volatile UART_t *UART_addr, uint32_t baud_rate);

/*
 * UART_transmit(volatile UART_t *UART_addr, uint8_t send_value)
 * --------------------
 *  Parameters:  UART Address and the message to transmit
 *  Waits until the UDRE bit is set to write to UDR
 *
 *  returns: 0
 */
uint8_t UART_transmit(volatile UART_t *UART_addr, uint8_t send_value);


/*
 * UART_recieve(volatile UART_t *UART_addr)
 * --------------------
 *  Parameters: UART Address
 *  Wait until the receive complete bit is set
 *  
 *
 *  returns: the UDR register
 */
uint8_t UART_recieve(volatile UART_t *UART_addr);

#endif /* UART_H_ */