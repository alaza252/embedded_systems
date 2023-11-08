/*
 * UART_solution_struct.c
 *
 * Created: 10/5/2021 3:06:09 PM
 * Author : ryoun
 */ 

#include <avr/io.h>
#include "board.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "UART.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "UART_Print.h"
#include "print_memory.h"
#include "Long_Serial_In.h"
#include <stdio.h>



const char test_string[15] PROGMEM = "Hello World!\n\r";


int main(void)
{
	/* Replace with your application code */
	char temp8;
	char string_in_SRAM [41] = "This is a string in SRAM - Roger Younger";
	char *string_p;
	uint32_t temp32;

	LEDS_init(LED0_PORT, LED0_PIN, ACTIVE_LOW);

	LEDS_init(LED1_PORT, LED1_PIN, ACTIVE_LOW);

	LEDS_init(LED2_PORT, LED2_PIN, ACTIVE_LOW);

	LEDS_init(LED3_PORT, LED3_PIN, ACTIVE_LOW);
	UART_init(UART1,9600);
	string_p=export_print_buffer();
	copy_string_to_buffer(test_string,string_p,0);
	UART_transmit_string(print_port,string_p,0);
	print_memory((uint8_t *)string_in_SRAM,50);
	while (1)
	{
		temp8=UART_receive(UART1);
		UART_transmit(UART1,temp8);
		
		LEDS_on(LED0_PORT, LED0_PIN, ACTIVE_LOW);
		UART_transmit(UART1,'U');
		_delay_ms(100);
		
		LEDS_off(LED0_PORT, LED0_PIN, ACTIVE_LOW);
		_delay_ms(900);
		
		LEDS_on(LED1_PORT, LED1_PIN, ACTIVE_LOW);
		UART_transmit(UART1,CR);
		_delay_ms(100);
		
		LEDS_off(LED1_PORT, LED1_PIN, ACTIVE_LOW);
		_delay_ms(900);
		
		LEDS_on(LED2_PORT, LED2_PIN, ACTIVE_LOW);
		UART_transmit(UART1,LF);
		_delay_ms(100);
		
		LEDS_off(LED2_PORT, LED2_PIN, ACTIVE_LOW);
		_delay_ms(900);
		
		LEDS_on(LED3_PORT, LED3_PIN, ACTIVE_LOW);
		UART_transmit(UART1,':');
		_delay_ms(100);
		
		LEDS_off(LED3_PORT, LED3_PIN, ACTIVE_LOW);
		_delay_ms(900);
		temp32=long_serial_input(UART1);
		sprintf(string_p,"%lu \n\r",temp32);   // %lu is used for an unsigned long
		UART_transmit_string(UART1,string_p,0);
		
	}
}


