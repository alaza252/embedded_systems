/*
 * UART.c
 *
 * Created: 9/29/2023 2:15:45 PM
 *  Author: Alisa Lazareva and Drew Schulte
 */ 

#include "UART.h"





 /*
 * UART_init( volatile UART_t *UART_addr, uint32_t baud_rate)
 * --------------------
 *  Parameters: UART Address and the Baud Rate get passed in
 *
 *  We calculate the UBBR Value using the baud rate
 *  We set up the message settings
 *  returns: 0
 */
uint8_t UART_init( volatile UART_t *UART_addr, uint32_t baud_rate)
{
	UART_addr -> UCSRA =U2X_bit;
	
	uint16_t UBBR_value;
	UBBR_value = (uint16_t)(((F_CPU/F_DIV)/(8UL*(2-U2X_bit)*baud_rate))-1);
	
	UART_addr -> UBBRH = UBBR_value >> 8;//divide by 256 
	UART_addr -> UBBRL = UBBR_value &=0xFF;//% 256
	
	UART_addr-> UCSRC = (asynch_mode|no_parity|one_stop_bit|Eight_Bit_Data); //setting up the size of message
	UART_addr -> UCSRB = (transmit_enable|receive_enable); //enable transmit and receive
	return 0;
}


 /*
 * UART_transmit(volatile UART_t *UART_addr, uint8_t send_value)
 * --------------------
 *  Parameters:  UART Address and the message to transmit
 *  Waits until the UDRE bit is set to write to UDR
 *
 *  returns: 0
 */
uint8_t UART_transmit(volatile UART_t *UART_addr, uint8_t send_value)
{
	uint8_t status;
	do
	{
		status = UART_addr -> UCSRA;	
	}
	while((status &(UDRE_bit)) == 0);

	UART_addr -> UDR= send_value;
	return 0;
}



 /*
 * UART_recieve(volatile UART_t *UART_addr)
 * --------------------
 *  Parameters: UART Address
 *  Wait until the receive complete bit is set
 *  
 *
 *  returns: the UDR register
 */
uint8_t UART_recieve(volatile UART_t *UART_addr)
{
	uint8_t status;
	uint8_t rec_value;
	do
	{
		status = UART_addr -> UCSRA;
	}
	while((status &(RXC_bit)) == 0);

	rec_value = UART_addr-> UDR;
	
	return rec_value;
}