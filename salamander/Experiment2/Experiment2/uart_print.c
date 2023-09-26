/*
 * uart_print.c
 *
 * Created: 9/4/2020 4:34:38 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board.h"
// TODO create UART.h file (and rename it to uart.h)
#include "UART.h"
#include <avr/pgmspace.h>

static char print_buffer[80];

char * export_print_buffer(void)
{
	
	return print_buffer;
}

uint8_t copy_string_to_buffer(const char flash_string[], char buffer_p[], uint8_t num_bytes)
{
	uint8_t index;
	char value;
	if(num_bytes==0)
	{
		index=0;
		do
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
			index++;
		} while (value!=0);
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			value=pgm_read_byte(&flash_string[index]);
			buffer_p[index]=value;
		}
		index=num_bytes;
	}
	return index;
}

void uart_transmit_string(volatile UART_t * addr, char string_name[], uint8_t num_bytes)
{
	char temp8;
	uint8_t index;
	if(num_bytes==0)
	{
		temp8=string_name[0];
		index=0;
		while (temp8!=0)
		{
			uart_transmit(addr, temp8);
			index++;
			temp8=string_name[index];
		}
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			uart_transmit(addr, string_name[index]);
		}
	}
}

