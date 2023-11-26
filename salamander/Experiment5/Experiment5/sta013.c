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
#include "config_arrays.h"
#include <util/delay.h>


#define STA013_TWI_DEVICE_ADDRESS 0x43

uint8_t sta013_write_array(const uint8_t *array) {
	uint8_t ff_count = 0;

	uint16_t index = 0;
	while (ff_count < 1) {
		uint8_t address = pgm_read_byte(&array[index * 2]);
		uint8_t val = pgm_read_byte(&array[index * 2 + 1]);
		if (address == 0xFF && val == 0xFF) {
			ff_count++;
		} else {
			uint8_t values[] = {address, val};
			uint8_t err = twi_master_transmit(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 2, values);
			if (err != 0) {
				return err;
			}
		}
		

		index++;
	}
	return 0;
}


uint8_t sta013_init() {
	gpio_output_init(STA013_RESET_REG, STA013_RESET_PIN_MASK);
	twi_master_init(STA013_TWI, 44100UL);
	
	sta013_reset_pin_set(0);
	
	_delay_us(1); // delay for 1000 ns (at least 100 ns)
	
	sta013_reset_pin_set(1);
	
	
	sprintf(export_print_buffer(), "We gon init bayybee\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);

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
	sprintf(export_print_buffer(), "Step 3 done\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);

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
	
	
	sprintf(export_print_buffer(), "On to step 6\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	{ // step 6 code
		uint8_t err;
		
		err = sta013_write_array(CONFIG);
		
		if (err != 0) {
			return STA013_INIT_TWI_ERROR;
		}
		
		_delay_ms(50); // "a short pause"
		
		err = sta013_write_array(CONFIG2);
		
		if (err != 0) {
			return STA013_INIT_TWI_ERROR;
		}
	}
	{ // step 7
		// note that I believe CONFIG2 writes to PCMCONF (0x55), which does the I2S part of step 7
		
		uint8_t check_registers[] = {
			0x54, 0x55, 0x06, 0x0B, 0x52, 0x51, 0x65, 0x64, 0x50, 0x61, 0x05,
			0xff
		};
		uint8_t index = 0;
		while (check_registers[index] != 0xff) {
			uint8_t err;
			uint8_t val;
			err = twi_master_transmit(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 1, &check_registers[index]);
			if (err != 0) {
				return STA013_INIT_TWI_ERROR;
			}
			err = twi_master_receive(STA013_TWI, STA013_TWI_DEVICE_ADDRESS, 1, &val);
			
			if (err != 0) {
				return STA013_INIT_TWI_ERROR;
			}
			sprintf(export_print_buffer(), "Register: %i has value: %i\r\n", check_registers[index], val);
			uart_transmit_string(UART1, export_print_buffer(), 0);
			index++;
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
