/*
 * UART.h
 *
 * Created: 9/29/2023 2:15:30 PM
 *  Author: Alisa Lazareva and Drew Schulte
 */ 


#ifndef UART_H_
#define UART_H_

#include "board.h"
typedef struct UART //this would be in UART.h
{
	volatile uint8_t UART_UCSRA;
	volatile uint8_t UART_UCSRB;
	volatile uint8_t UART_UCSRC;
	volatile uint8_t UART_UCSRD;
	volatile uint8_t UART_UBRRL; //can declare these two as a volatile uint16_t UART_BRR; (as long as no padding)
	volatile uint8_t UART_UBRRH; //all examples will be done using this method
	volatile uint8_t UART_UDR;
} UART_t;

#define UART0 ((volatile UART_t*)0xC0) //0xC0
#define UART1 ((volatile UART_t*)0xC8) //0xCB
#define UART2 ((volatile UART_t*)0xD0) //0x00
#define UDRE_bit (1<<5)
#define RXC_bit (1<<7)

#define asynch_mode (0<<6)
#define synch_mode (1<<6)
#define SPI_mode (3<<6)

#define no_parity (0<<4)
#define even_parity (2<<4)
#define odd_parity (3<<4)

#define one_stop_bit (0<<3)
#define two_stop_bit (1<<3)

#define U2X_bit (0)

#define transmit_enable (1<<3)
#define receive_enable (1<<4)

#define Eight_Bit_Data (3<<1)



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