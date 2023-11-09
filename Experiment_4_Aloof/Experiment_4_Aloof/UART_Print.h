/*
 * UART_Print.h
 *
 * Created: 9/4/2020 4:36:37 PM
 *  Author: youngerr
 */ 


#ifndef UART_PRINT_H_
#define UART_PRINT_H_

/***** Defines the default port for printing functions  ******/
#define print_port (UART1)


/***** This function just provides a pointer to the print_buffer for access from other source files ****/
/***** Use: Declare a local char pointer;  char *buffer;
******      Execute the function and place the return value in the local pointer;  buffer=export_print_buffer();
******      Use the pointer to store/read strings of values in print_buffer;  sprintf(buffer,"Hello");
******                                                                        UART_trnasmit_string(UART1,buffer,0);
******/
char * export_print_buffer(void);

/**************************************************************
*   copy_string_to_buffer
*   Inputs: pointer to a string stored in Flash memory (must be in Flash)
*           pointer to a string in SRAM (must be in SRAM)
*           Number of bytes to copy
*             If number is 0, copies until a NULL (0x00) termination is found
*   Output: The number of bytes that was copied.
**************************************************************
*   Function: This is used to copy strings from Flash memory
*             which requires a special function to access to
*             normal SRAM memory where regular pointers can address the values
*
*   Caution: Will not work properly if first string is not in Flash
***************************************************************/
uint8_t copy_string_to_buffer(const char flash_string[], char buffer_p[], uint8_t num_bytes);

/**************************************************************
*   UART_transmit_string
*   Inputs: pointer to a UART struct
*           pointer to a string in SRAM (must be in SRAM)
*           Number of bytes to copy
*             If number is 0, copies until a NULL (0x00) termination is found
*   Output: nothing
**************************************************************
*   Function: This is used to copy strings from SRAM memory to the
*             specified UART one byte at a time.  Uses the UART_transmit
*             function to check if the next byte is ready to be sent.
*
*   Caution: Loops in the function until the entire string is printed.
*            May not be suitable for real-time use.
***************************************************************/
void UART_transmit_string(volatile UART_t *UART_addr, char string_name[], uint8_t num_bytes);



#endif /* UART_PRINT_H_ */