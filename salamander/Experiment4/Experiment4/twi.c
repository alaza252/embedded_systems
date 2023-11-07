/*
 * twi.c
 *
 * Created: 11/7/2023 4:18:49 PM
 * Author: Sam Stockmann, Lavender Shannon
 */

#include "twi.h"

uint8_t twi_master_init(volatile TWI_t* addr, uint32_t i2c_freq) {
	uint16_t prescale = (((F_CPU / OSC_DIV) / i2c_freq) - 16) / (2UL * 255);
	uint8_t prescale_code;
	
	if (prescale < 1) {
		prescale_code = 0;
		prescale = 1;
	} else if (prescale < 4) {
		prescale_code = 1;
		prescale = 4;
	} else if (prescale < 16) {
		prescale_code = 2;
		prescale = 16;
	} else if (prescale < 64) {
		prescale_code = 3;
		prescale = 64;
	} else {
		return INVALID_FREQ;
	}
	
	uint8_t baud_val = (((F_CPU / OSC_DIV) / i2c_freq) - 16) / (2UL * prescale);
	
	addr -> BAUD_RATE = baud_val;
	addr -> STATUS = prescale_code;
	
	return 0;
}

uint8_t twi_master_receive(volatile uint8_t * addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr) {
	return 0;
}

