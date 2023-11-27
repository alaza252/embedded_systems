/*
 * main.c
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
#include "spi.h"
#include "sd_spi.h"
#include "sd.h"
#include "long_serial_in.h"
#include "print_memory.h"
#include "sta013.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
// 	//init UART
// 	uart_init(UART1, 9600UL);
// 	
// 	//init LEDs
// 	gpio_led_init(LED0_REG, LED0_PIN_MASK);
// 
// 	uint8_t err = sta013_init();
// 	
// 	sprintf(export_print_buffer(), "Error: %i!\r\n", err);
// 	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	//init UART
	uart_init(UART1, 9600UL);
	
	//init SPI
	spi_master_init(SPI0, 400000UL);
	
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);

	SDInfo sd_info;
	uint8_t sd_card_init_error = sd_card_init(&sd_info);
	
	if (sd_card_init_error != 0) {
		sprintf(export_print_buffer(), "Error was: %i!\r\n", sd_card_init_error);
		uart_transmit_string(UART1, export_print_buffer(), 0);
		return 0;
	}
	sprintf(export_print_buffer(), "Card type value: %i, card capacity value: %i\r\n", sd_info.card_type, sd_info.capacity);
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	//spi_master_init(SPI0, 25000000UL);
	spi_master_init(SPI0, 8000000UL); // 8MHz is the maximum of our board, if we try to use a value higher than this, the function that calculates the divider will get very angry
	
	for (;;) {
		uint32_t block_num = long_serial_input(UART1);
		sprintf(export_print_buffer(), "Block number: %i!\r\n", (uint8_t) block_num); // this only supports showing 8 bit numbers, but that's OK. higher values are still supported, they just aren't printed correctly
		uart_transmit_string(UART1, export_print_buffer(), 0);
		uint8_t error;
		
		cs_pin_set(0);
		error = send_command(17, block_num);
		if (error != 0) {
			sprintf(export_print_buffer(), "Got error when sending: %i!\r\n", error);
			uart_transmit_string(UART1, export_print_buffer(), 0);
		} else {
			uint8_t data[512];
			error = read_block(512, data);
			if (error != 0) {
				sprintf(export_print_buffer(), "Got error when reading: %i!\r\n", error);
				uart_transmit_string(UART1, export_print_buffer(), 0);
			} else {
				print_memory(data, 512);
			}
		}
		
		cs_pin_set(1);
	}
	
}


