#ifndef _print_bytes_H
#define _print_bytes_H





// ------ Public function prototypes -------------------------------

/***********************************************************************
DESC: Prints an array from memory in Hexadecimal and then ASCII
INPUT: Pointer to an array, number of bytes to print
RETURNS: nothing
CAUTION: Prints to print_port (define as a constant: #define print_port (UART1))
         It is the address of the UART used to print out values
************************************************************************/

void print_memory(uint8_t * array_in, uint16_t number_of_bytes);


#endif