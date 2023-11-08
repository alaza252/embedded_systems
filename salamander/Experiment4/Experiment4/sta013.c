/*
 * sta013.c
 *
 * Created: 11/7/2023 8:12:18 PM
 * Author: Sam Stockmann, Lavender Shannon
 */

#include "sta013.h"
#include "io.h"
#include "twi.h"
#include "uart.h"
#include "uart_print.h"
#include <util/delay.h>

uint8_t sta013_init() {
	gpio_output_init(STA013_RESET_REG, STA013_RESET_PIN_MASK);
	twi_master_init(STA013_TWI, 44100UL);
	
	sta013_reset_pin_set(0);
	
	_delay_us(1); // delay for 1000 ns (at least 100 ns)
	
	sta013_reset_pin_set(1);
	
	uint8_t arr[3];
	
	uint8_t err;
	uint8_t i = 0;
	
	do {
		err = twi_master_receive(STA013_TWI, 0x43, 3, arr);
		i++;
	} while(i < 10 && err != 0);
	if (err == 0) { // no errors
		sprintf(export_print_buffer(), "Success! second byte is: %i!\r\n", arr[2]);
		uart_transmit_string(UART1, export_print_buffer(), 0);
	} else {
		sprintf(export_print_buffer(), "Error: %i!\r\n", err);
		uart_transmit_string(UART1, export_print_buffer(), 0);
	}
	
	return 0;
}

void sta013_reset_pin_set(uint8_t bit) {
	if (bit == 0) {
		gpio_output_clear(STA013_RESET_REG, STA013_RESET_PIN_MASK);
	} else {
		gpio_output_set(STA013_RESET_REG, STA013_RESET_PIN_MASK);
	}
}
