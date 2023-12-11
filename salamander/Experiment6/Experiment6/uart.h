
#ifndef  _UART_H
#define  _UART_H

/**** Public Constants ********/
#define NO_DATA (0x02)
#define GOOD_DATA (0x01)
#define FRAME_ERROR (0x82)
#define OVERRUN_ERROR (0x84)
#define PARITY_ERROR (0x88)
#define UART_TX_TIMEOUT (0x90)
#define UART_NO_ERRORS  (0x00)

/***** Public Function prototypes ********/


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
void UART_init (volatile UART_t * UART_addr, uint32_t Baud_Rate);

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
char UART_transmit(volatile UART_t * UART_addr, char c);

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
char UART_receive(volatile UART_t * UART_addr);

/**************************************************************
*   UART_receive_nb
*   Inputs: pointer to a UART struct,
*           pointer to a variable for the received char
*   Output: Returns an error flag indicating:
*           NO_DATA, GOOD_DATA, PARITY_ERROR, FRAME_ERROR, and OVERRUN_ERROR
*           Return Values defined as constants:
*           NO_DATA=0x02, GOOD_DATA=0x01, ERROR=0x80|(PARITY=0x08,OVERRUN=0x04,FRAME=0x02)
*           Example: Data received with parity and overrun errors => 0x8D => 0b1000 1101
*           Check the lsb to see if data was received
*           Check the msb to see if some error occurred
*           Check bits 1,2, and 3 to see the specific error
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

uint8_t UART_receive_nb(volatile UART_t * UART_addr, char *rcv_value);

#endif