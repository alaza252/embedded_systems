#include <stdio.h>
#include "board.h"
#include "UART.h"
#include "UART_Print.h"
#include "print_memory.h"
#include <avr/pgmspace.h>

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A)

const char Prnt_bytes_prnt[64] PROGMEM = {"Addr. Offset   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n\r"}; 

/****** Private Functions **************/

void print_16bytes(uint8_t * array_in, uint16_t offset);

/***********************************************************************
DESC:    Prints 8 bytes in Hexadecimal and then ASCII
INPUT: Pointer to an array of eight bytes
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_16bytes(uint8_t * array_in, uint16_t offset)
{
   uint8_t * input;
   uint8_t index, dat;
   char * prnt_bffr;
   
   prnt_bffr=export_print_buffer();
   
   input = array_in+offset;
   sprintf(prnt_bffr, "%p ",input);
   UART_transmit_string(print_port,prnt_bffr,0);
   sprintf(prnt_bffr,"0x%4.4X  ",offset);
   UART_transmit_string(print_port,prnt_bffr,0);
   for (index=0;index<16;index++)
   {
      dat=*(input+index);
      sprintf(prnt_bffr, "%2.2X ",dat);   // Changed from %2.2bX because this printf function can correctly determine number of bytes
	  UART_transmit_string(print_port,prnt_bffr,0);
   }
   for (index=0;index<16;index++)
   { 
      dat=*(input+index);
      if (dat<32 || dat>127) dat=46;
      UART_transmit(print_port,dat);
   }
   UART_transmit(print_port,CR);
   UART_transmit(print_port,LF);
}


/***********************************************************************
DESC: Prints an array from memory in Hexadecimal and then ASCII
INPUT: Pointer to an array, number of bytes to print
RETURNS: nothing
CAUTION: 
************************************************************************/
void print_memory(uint8_t * array_in, uint16_t number_of_bytes)
{
   uint16_t input;
   char * prnt_bffr;
   
   prnt_bffr=export_print_buffer();
   
   input = 0;
   copy_string_to_buffer(Prnt_bytes_prnt,prnt_bffr,0);
   UART_transmit_string(print_port,prnt_bffr,0);
   do
   {
      print_16bytes(array_in,input);
      input+=16;
   }while(input<(number_of_bytes));
}
		