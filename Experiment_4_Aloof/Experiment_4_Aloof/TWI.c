/*
 * TWI.c
 *
 * Created: 11/8/2023 5:17:12 PM
 *  Author: dnsdyy
 */ 

#include "twi.h"

uint8_t twi_master_init(volatile TWI_t *addr, uint32_t i2c_freq) {
    uint16_t prescale = (((F_CPU / F_DIV) / i2c_freq) - 16) / (2UL * 255);
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

    uint8_t baud_val = (((F_CPU / F_DIV) / i2c_freq) - 16) / (2UL * prescale);

    addr->Baud_rate = baud_val;
    addr->Status = prescale_code;

    return 0;
}


uint8_t twi_master_receive(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr) {
	// Initialize variables to keep track of the number of bytes left to receive and the current index.
	uint16_t bytes_left = num_bytes;
	uint16_t index = 0;

	// Check if the device address has the most significant bit set. If so, return an error.
	if ((dev_addr & 0x80) != 0) {
		return TWI_INVALID_DEV_ADDR;
	}

	// Generate a start condition and wait for the TWI module to complete it.
	addr->Data=(dev_addr<<1)|1;
	addr->Control = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTA));
	while (((addr->Control) & (1 << TWINT)) == 0) {}

	// Read and check the status after the start condition.
	uint8_t status = (addr->Status) & 0xF8;
	if ((status != 0x08) && (status != 0x10)) {
		return TWI_UNEXPECTED_STATUS;
	}

	// Send the device address with the read bit (1) set.
	addr->Data = (dev_addr << 1) | 1;
	addr->Control = ((1 << TWINT) | (1 << TWEN));
	while (((addr->Control) & (1 << TWINT)) == 0) {}
	status = (addr->Status) & 0xF8;

	// Check the status after sending the device address.
	if (status != 0x40) {
		return TWI_UNEXPECTED_STATUS;
	}

	// Receive data bytes from the device.
	while (bytes_left > 0) {
		if (bytes_left == 1) {
			// If there's only one byte left to receive, don't send an ACK (0).
			addr->Control = ((1 << TWINT) | (0 << TWEA) | (1 << TWEN));
			} else {
			// If there are more bytes to receive, send an ACK (1).
			addr->Control = ((1 << TWINT) | (1 << TWEA) | (1 << TWEN));
		}

		// Wait for the TWI module to complete the operation and check the status.
		while (((addr->Control) & (1 << TWINT)) == 0) {}
		status = (addr->Status) & 0xF8;

		if (status == 0x50 || status == 0x58) {
			// Data received successfully, store it in the array and increment the index.
			arr[index] = addr->Data;
			} else {
			return TWI_UNEXPECTED_STATUS; // Unexpected status, return an error.
		}

		if (status == 0x58) {
			// If it's the last byte received, generate a stop condition.
			addr->Control = ((1 << TWINT) | (1 << TWSTO) | (1 << TWEN));
			while (((addr->Control) & (1 << TWSTO)) == 1) {
				// Wait for the stop condition to complete.
			}
		}

		bytes_left--;
		index++;
	}

	return 0; // Return 0 to indicate successful reception.
}


uint8_t TWI_master_transmit(volatile TWI_t* addr, uint8_t device_addr, uint32_t int_addr, uint8_t int_addr_sz, uint16_t num_bytes, uint8_t *array_name) {
	uint8_t status;
	uint8_t i;
	
	// Generate start condition
	addr->Control = (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);

	// Wait until start condition is sent
	uint8_t timeout = 0;
	uint8_t MAX_TIMEOUT = 255;
	while (!(addr->Status & (1 << TWINT))) {
		timeout++;
		if (timeout >= MAX_TIMEOUT) {
			// Handle timeout error
			return 0xFF;  // You can choose a different error code
		}
	}

	//status = TWSR & 0xF8;
	//if (status != I2C_START && status != I2C_REP_START)
	//return status;

	// Send slave device address with write bit (LSB = 0)
	addr->Data = (device_addr << 1) | 0;  // 7-bit address format

	// Wait until address is sent and ACK is received
	timeout=0;
	while (!(addr->Status & (1 << TWINT))) {
		timeout++;
		if (timeout >= MAX_TIMEOUT) {
			// Handle timeout error
			return 0xFF;  // You can choose a different error code
		}
	}

	//status = TWSR & 0xF8;
	//if (status != I2C_MT_SLA_ACK)
	//return status;

	// Send internal address if int_addr_sz > 0
	for (i = 0; i < int_addr_sz; i++) {
		// Send the most significant byte of internal address first
		addr->Data = (uint8_t)(int_addr >> ((int_addr_sz - 1 - i) * 8));

		// Wait until address byte is sent and ACK is received
		timeout=0;
		while (!(addr->Status & (1 << TWINT))) {
			timeout++;
			if (timeout >= MAX_TIMEOUT) {
				// Handle timeout error
				return 0xFF;  // You can choose a different error code
			}
		}

		//status = TWSR & 0xF8;
		//if (status != I2C_MT_DATA_ACK)
		//return status;
	}

	// Send data bytes
	for (i = 0; i < num_bytes; i++) {
		addr->Data = array_name[i];

		// Wait until data byte is sent and ACK is received
		timeout=0;
		while (!(addr->Status & (1 << TWINT))) {
			timeout++;
			if (timeout >= MAX_TIMEOUT) {
				// Handle timeout error
				return 0xFF;  // You can choose a different error code
			}
		}

//		status = TWSR & 0xF8;
//		if (status != I2C_MT_DATA_ACK)
//		return status;
	}

	// Generate stop condition
	addr->Control = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);

	return 0;  // Success
}
