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
		return TWI_INIT_INVALID_FREQ;
	}
	
	uint8_t baud_val = (((F_CPU / OSC_DIV) / i2c_freq) - 16) / (2UL * prescale);
	
	addr -> BAUD_RATE = baud_val;
	addr -> STATUS = prescale_code;
	
	return 0;
}


void twint_wait(volatile TWI_t* addr) {
	// wait for TWINT to be 1
	while (((addr -> CONTROL) & (1 << TWINT)) == 0) {
		// do nothing
	}
}


uint8_t twi_master_start(volatile TWI_t* addr, uint8_t dev_addr) {
	if ((dev_addr & 0x80) != 0) {
		return TWI_INVALID_DEV_ADDR;
	}
	
	addr -> CONTROL = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTA));
	
	twint_wait(addr);
	
	uint8_t status = (addr -> STATUS) & 0xF8;
	
	if ((status != 0x08) && (status != 0x10)) { // if the status is not a start sent and the status is not a repeated start
		return TWI_UNEXPECTED_STATUS;
	}
	
	return 0;
}


uint8_t twi_master_receive(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr) {
	uint8_t start_err = twi_master_start(addr, dev_addr);
	
	if (start_err != 0) {
		return start_err;
	}
	
	addr -> DATA = (dev_addr << 1) | 1; // LSB will be 1 for read
	addr -> CONTROL = ((1 << TWINT) | (1 << TWEN));
	twint_wait(addr);
	
	uint8_t status;
	status = (addr -> STATUS) & 0xF8;
	
	if (status != 0x40) { // if the status is not a SLA+R sent, ACK received
		return TWI_UNEXPECTED_STATUS;
	}
	
	uint16_t bytes_left = num_bytes;
	
	uint16_t index = 0;
	
	while (bytes_left > 0) {
		if (bytes_left == 1) {
			addr -> CONTROL = ((1 << TWINT) | (0 << TWEA) | (1 << TWEN)); // send NACK, no more bytes to send
		} else {
			addr -> CONTROL = ((1 << TWINT) | (1 << TWEA) | (1 << TWEN)); // send ACK, more bytes to send
		}
		
		twint_wait(addr);
		status = (addr -> STATUS) & 0xF8;
		
		if (status == 0x50 || status == 0x58) { // we have received a byte, and we have either sent an ACK or a NACK
			arr[index] = addr -> DATA;
		} else {
			return TWI_UNEXPECTED_STATUS;
		}
		if (status == 0x58) { // we have received a byte and have sent a NACK
			// If we have sent a NACK, then we assume that bytes_left is 1, and after this iteration we have no more bytes to send
			// So... We need to now send a stop and wait for that stop to be fully transmitted
			addr -> CONTROL = ((1 << TWINT) | (1 << TWSTO) | (1 << TWEN));
			// wait for TWSTO to be 0
			while (((addr -> CONTROL) & (1 << TWSTO)) == 1) {
				// do nothing
			}
		}

		bytes_left--;
		index++;
	}
	
	return 0;
}

uint8_t twi_master_transmit(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr) {
	uint8_t start_err = twi_master_start(addr, dev_addr);
	
	if (start_err != 0) {
		return start_err;
	}
	
	return 0;
}

