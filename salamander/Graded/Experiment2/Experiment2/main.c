/*
 * Experiment2.c
 *
 * Created: 9/26/2023 4:27:07 PM
 * Author : sjsmdd
 */ 

#include "board.h"
#include "leds.h"
#include "switch.h"
#include "uart.h"
#include "uart_print.h"
#include "print_memory.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>


const char STEP_7_FLASH_STRING[64] PROGMEM = {"This is an example of a string stored in flash!\n\r"};

int main(void)
{
	uart_init(UART1, 9600UL);
	
	// Step 6
	sprintf(export_print_buffer(), "Hello! This line uses sprintf and the print buffer!\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	// Step 7
	copy_string_to_buffer(STEP_7_FLASH_STRING, export_print_buffer(), 0);
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	// Step 8
	unsigned char in_memory_string[] = "This is a string with more than 30 characters. Sam and Lavender worked on this project together!\r\n";
	print_memory(in_memory_string, 0);
	
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);

	uint8_t led_state = 0;

	while(1)
	{
		
		uint8_t rcvd_val;
		
		while (uart_receive_nb(UART1, &rcvd_val) == UART_RECEIVE_SUCCESS) {
			uart_transmit(UART1, rcvd_val);
		}
		
		led_state = !led_state;
		if (led_state) {
			gpio_led_on(LED0_REG, LED0_PIN_MASK);
		} else {
			gpio_led_off(LED0_REG, LED0_PIN_MASK);
		}
		
		_delay_ms(500);
	}
}


