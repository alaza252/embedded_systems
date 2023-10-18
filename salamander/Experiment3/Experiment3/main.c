/*
 * Experiment3.c
 *
 * Created: 10/16/2023 2:08:07 PM
 * Author : Sam Stockmann, Lavender Shannon
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
	//init UART
	uart_init(UART1, 9600UL);
	
	//init SPI
	spi_master_init(SPI0, 400000UL);
	
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);

	SDInfo sd_info;
	uint8_t sd_card_init_error = sd_card_init(sd_info);
	
	sprintf(export_print_buffer(), "Hello! This line uses sprintf and the print buffer!\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	//unsigned char in_memory_string[] = "This is a string with more than 30 characters. Sam and Lavender worked on this project together!\r\n";
	//print_memory(in_memory_string, 0);

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

