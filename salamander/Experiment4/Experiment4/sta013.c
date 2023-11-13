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


#define STA013_TWI_DEVICE_ADDRESS 0x43


uint8_t sta013_write_array(uint8_t array[]) {
	uint8_t ff_count = 0;

	uint8_t index = 0;
	while (1) {
		uint8_t address = array[index * 2];
		uint8_t address = array[index * 2];


		index++;
	}
}


uint8_t sta013_init() {
	gpio_output_init(STA013_RESET_REG, STA013_RESET_PIN_MASK);
	twi_master_init(STA013_TWI, 44100UL);
	
	sta013_reset_pin_set(0);
	
	_delay_us(1); // delay for 1000 ns (at least 100 ns)
	
	sta013_reset_pin_set(1);

	{ // step 3 code - (not strictly necessary, but kept for grading)
		uint8_t arr[3];

		uint8_t err;
		uint8_t i = 0;

		do {
			err = twi_master_receive(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 3, arr);
			i++;
		} while(i < 10 && err != 0);
		if (err != 0) {
			sprintf(export_print_buffer(), "Error: %i!\r\n", err);
			uart_transmit_string(UART1, export_print_buffer(), 0);
			return STA013_INIT_TWI_ERROR;
		}
		// in the "step 3" case, the internal address starts at the end, and then loops back around, so arr[2] represents address 1
		if (arr[2] != 0xAC) {
			return STA013_INIT_CHECK_FAILED_ERROR;
		}
	}

	{ // step 5 code
		uint8_t err;
		uint8_t transmit_array[] = {1};
		uint8_t address_1_check_value;
		uint8_t i = 0;
		do {
			err = twi_master_transmit(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 1, transmit_array);
			if (err == 0) {
				err = twi_master_receive(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 1, &address_1_check_value);
			}
		} while (i < 10 && err != 0);

		if (err != 0) {
			return STA013_INIT_TWI_ERROR;
		}
		if (address_1_check_value != 0xAC) {
			return STA013_INIT_CHECK_FAILED_ERROR;
		}

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
