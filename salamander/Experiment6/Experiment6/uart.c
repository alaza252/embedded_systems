
#include "board.h"
#include "UART.h"
#include <avr/pgmspace.h>

/****** Private Constants *******/
#define U2X_bit (0)  // 0 - regular baud rate, 1 - double baud rate
/****** Register Bit Settings *****/
/****** UCSRA bits *******/
#define MPCM_shift (0)
#define U2X_shift (1)
#define TXC_shift (6)
/****** UCSRB bits *******/
#define TXB8_shift (0)
#define Nine_Data_Bits_B (1<<2)
#define TX_enable (1<<3)
#define RX_enable (1<<4)
#define UDRE_interrupt_enable (1<<5)
#define TX_interrupt_enable (1<<6)
#define RX_interrupt_enable (1<<7)
/****** UCSRC bits *******/
#define Asynchronous (0<<6)
#define Synchronous (1<<6)
#define Master_SPI (3<<6)
#define No_Parity (0<<4)
#define Even_Parity (2<<4)
#define Odd_Parity (3<<4)
#define One_Stop_Bit (0<<3)
#define Two_Stop_Bits (1<<3)
#define Five_Data_Bits (0<<1)
#define Six_Data_Bits (1<<1)
#define Seven_Data_Bits (2<<1)
#define Eight_Data_Bits (3<<1)
#define Nine_Data_Bits_C (3<<1)
#define SPI_MSB_First (0<<2)
#define SPI_LSB_First (1<<2)
#define SPI_MSB_First (0<<2)
#define SPI_CPHA_Leading_Edge (0<<1)
#define SPI_CPHATrailing_Edge (1<<1)
#define SPI_CPOL_0 (0<<0)
#define SPI_CPOL_1 (1<<0)



/**************************************************************
*   UART_init
*   Inputs: pointer to a UART struct,
*           baud rate value as a 32-bit integer
*   Output: None
**************************************************************
*   Function: Sets the UART registers as defined in the source code
*             These can be changed using the defined constants 
*                for each register
*             Calculates and sets the baud rate reload register
*
*   Caution: Does not check the for valid input parameters
***************************************************************/
void UART_init (volatile UART_t * UART_addr, uint32_t Baud_Rate)
{
	uint16_t Baud_Rate_Reload;
	Baud_Rate_Reload = (uint16_t)((((F_CPU/OSC_DIV)+(4UL*(2-U2X_bit)*Baud_Rate))/(8UL*(2-U2X_bit)*Baud_Rate))-1);
	
	(UART_addr->UBBRH) = (Baud_Rate_Reload / 256);
	(UART_addr->UBBRL) = (Baud_Rate_Reload % 256);
		
	(UART_addr->UCSRA) = U2X_bit<<U2X_shift;
	(UART_addr->UCSRC) = Asynchronous|No_Parity|One_Stop_Bit|Eight_Data_Bits;
	(UART_addr->UCSRB) = RX_enable|TX_enable;
}

/**************************************************************
*   UART_transmit
*   Inputs: pointer to a UART struct,
*           value to transmit as a char
*   Output: Returns an error if a timeout occurs
**************************************************************
*   Function: Waits for the Data Reg. Empty bit (UDRE)
*             to be set and then places the char to send into
*             the data register (UDR).  
*
*   Caution: Will not transmit if TXEN is not set in UART_init
***************************************************************/

char UART_transmit(volatile UART_t * UART_addr, char c)
{
	uint8_t status, error_flag;
	uint16_t timeout;
	timeout=0;
	do
	{
		status=(UART_addr->UCSRA);
		timeout++;
	} while (((status & (1 << UDRE))!=(1 << UDRE))&&(timeout!=0));
	if(0==timeout)
	{
		error_flag=UART_TX_TIMEOUT;
	}
	else
	{
		(UART_addr->UDR) = c;
		error_flag=UART_NO_ERRORS;
	}
	return error_flag;
}

/**************************************************************
*   UART_receive
*   Inputs: pointer to a UART struct,
*   Output: Returns the value received as a char
**************************************************************
*   Function: Waits for the Received Char bit (RXC)
*             to be set and then reads the char from
*             the data register (UDR).
*
*   Caution: Will not receive if RXEN is not set in UART_init
*            Waits forever until a value is received
***************************************************************/

char UART_receive(volatile UART_t * UART_addr)
{
	uint8_t status;
	do 
	{
		status=(UART_addr->UCSRA);
	} while ((status & (1 << RXC))!=(1 << RXC));
	return (UART_addr->UDR);  // return the value in the receive buffer
}

/**************************************************************
*   UART_receive_nb
*   Inputs: pointer to a UART struct,
*           pointer to a variable for the received char
*   Output: Returns an error flag indicating:
*           NO_DATA, GOOD_DATA, PARITY_ERROR, FRAME_ERROR, and OVERRUN_ERROR
**************************************************************
*   Function: Waits for the Received Char bit (RXC)
*             to be set and then reads the char from
*             the data register (UDR).
*             The error bits are checked and error bits will be 
*             set in the return 
*
*   Caution: Will not receive if RXEN is not set in UART_init
*            Waits forever until a value is received
***************************************************************/

uint8_t UART_receive_nb(volatile UART_t * UART_addr, char *rcv_value)
{
	uint8_t status, return_value;
	return_value=NO_DATA;
	status=(UART_addr->UCSRA);
	if((status & (1 << RXC))==(1 << RXC))
	{
		*rcv_value=UART_addr->UDR;
		return_value=GOOD_DATA;
		if((status & (1<<FE))==(1<<FE))
		{
			return_value|=FRAME_ERROR;
		}
		if((status & (1<<DOR))==(1<<DOR))
		{
			return_value|=OVERRUN_ERROR;
		}
		if((status & (1<<UPE))==(1<<UPE))
		{
			return_value|=PARITY_ERROR;
		}
	}
	return return_value;  // return the value in the receive buffer
}