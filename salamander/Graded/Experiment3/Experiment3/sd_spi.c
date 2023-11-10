/*
 * sd_spi.c
 *
 * Created: 10/16/2023 3:16:56 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "sd_spi.h"
#include "spi.h"
#include "io.h"

#include <stdio.h>
#include "uart.h"
#include "uart_print.h"

#define CRC_POLY (0x89u)

//*** Needs prototype ****

uint8_t get_crc(uint8_t message[], uint8_t length) {
	// https://electronics.stackexchange.com/questions/602105/how-can-i-initialize-use-sd-cards-with-spi
	// Thanks https://github.com/hazelnusse/crc7/blob/master/crc7.cc
	uint8_t crc = 0;
	for (uint8_t i = 0; i < length; i++) {
		uint8_t message_byte = message[i];
		uint8_t lookup_value = (crc << 1) ^ message_byte;

		uint8_t value = (lookup_value & 0x80) != 0 ? lookup_value ^ CRC_POLY : lookup_value;
		for (uint8_t j = 1; j < 8; j++) {
			value <<= 1;
			if ((value & 0x80) != 0) {
				value ^= CRC_POLY;
			}
		}
		crc = value;
	}
	return crc;
}
//**** Needs prototype ****
/** Receives the R1 response, but DOES NOT send a 0xFF at the end*/
uint8_t receive_r1_response(uint8_t *r1_response) {
	uint8_t transfer_val;
	uint8_t transfer_err;

	uint8_t timeout = 0; // Since this is an 8 bit value, we expect the timeout to be 256 tries
	do {
		transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return SD_COM_ERROR;
		}
		timeout++;
	} while (((transfer_val & 0x80) != 0 )&& (timeout != 0));

	if ((transfer_val & 0x80) != 0) { // The most significant bit must be 0 for this to be a valid R1 response.
		return SD_RECEIVE_TIMEOUT;
	}
	// Now that we are getting a response from the SD card, before this function exits we MUST send a 0xFF at the end.
	//   That means that if we have ANY early returns after this we need to send that value before exiting the function.

	// we now know that transfer_val is in fact an R1 response, but that doesn't mean there aren't errors in R1

	*r1_response = transfer_val;
	
	if ((transfer_val & SD_R1_ERROR_MASK) != 0) {
		return SD_RECEIVE_R1_ERROR;
	}
	return 0;
}

uint8_t send_command(uint8_t cmd_val, uint32_t arg) {
	// Note: This function has multiple returns which are used as guard clauses. These guard clauses prevent many nested if statements to make this function more readable.
	//   Remember that early returns are only bad practice in C because resource cleanup usually occurs at the end of the function, but we have nothing to clean up here.
	if (cmd_val >= 64) {
		return SD_ILLEGAL_CMD;
	}
	
	uint8_t transfer_val;
	uint8_t transfer_err;

	uint8_t raw_message[5]; // keep track of the raw message for the CRC calculation
	raw_message[0] = (0x40|cmd_val);
	transfer_err = spi_transfer(SD_SPI_PORT, (0x40|cmd_val), &transfer_val);
	
	if (transfer_err != 0) {
		return SD_COM_ERROR;
	}
		
	for(uint8_t i = 0; i < 4; i++) {
		uint8_t send_value = (uint8_t) (arg >> (24-(i*8))); // send MSB first
		raw_message[i + 1] = send_value;
		transfer_err = spi_transfer(SD_SPI_PORT, send_value, &transfer_val);
		if (transfer_err != 0) {
			return SD_COM_ERROR;
		}
	}
	
	transfer_err = spi_transfer(SD_SPI_PORT, (get_crc(raw_message, 5) << 1) | 1, &transfer_val);
	if (transfer_err != 0) {
		return SD_COM_ERROR;
	}
	
	return 0;
}


uint8_t receive_response(uint8_t num_bytes, uint8_t receive_array[]) {
	uint8_t transfer_val;
	uint8_t transfer_err;

	transfer_err = receive_r1_response(&transfer_val);
	if (transfer_err == SD_RECEIVE_R1_ERROR) {
		receive_array[0] = transfer_val;
		spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		return SD_RECEIVE_R1_ERROR;
	} else if (transfer_err != 0) {
		return transfer_err;
	}
	receive_array[0] = transfer_val;

	for (uint8_t i = 1; i < num_bytes; i++) {
		transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		receive_array[i] = transfer_val; // needed to add this when implementing CMD8
		if (transfer_err != 0) {
			// We don't have to send 0xFF on the bus here because the caller of the function should know that an error like this requires
			//   reinitialization of the SD card for it to be usable again.
			return SD_COM_ERROR;
		}
	}

	spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
	return 0;
}

uint8_t read_block(uint16_t num_bytes, uint8_t data_arr[]) {
	uint8_t r1_response;
	uint8_t transfer_err;
	
	transfer_err = receive_r1_response(&r1_response);
	if (transfer_err != 0) {
		return transfer_err;
	}
	if (r1_response != 0) { // receive_r1_response already checks the R1 error bits, but we will need to make sure the card is not in the idle state
		return SD_RECEIVE_R1_ERROR;
	}
	// now we have successfully received the R1 response

	uint8_t transfer_val;

	uint16_t timeout = 0; // Since this is an 8 bit value, we expect the timeout to be 256 tries
	do {
		transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return SD_COM_ERROR;
		}
		timeout++;
	} while (transfer_val != 0xFE && (transfer_val & 0xF0) != 0 && (timeout != 0)); // 0xFE is the data start token block that we are waiting for
		
	if (timeout == 0) {
		return SD_RECEIVE_TIMEOUT;
	}
	if ((transfer_val & 0xF0) == 0) { // upper 4 bits are cleared
		uint8_t data_error = transfer_val & 0x0F; // lower 4 bits indicate error
		sprintf(export_print_buffer(), "Data error: %i, token: %i timeout: %i\r\n", data_error, transfer_val, timeout);
		uart_transmit_string(UART1, export_print_buffer(), 0);
		return SD_READ_DATA_TOKEN_ERROR;		
	}
	// now if we are here, we have just received the data start token, and we can read in some values :)
	for (uint16_t i = 0; i < num_bytes; i++) {
		transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return SD_COM_ERROR;
		}
		data_arr[i] = transfer_val;
	}
	
	// first 2 are for checksum which we will ignore for now
	transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
	if (transfer_err != 0) {
		return SD_COM_ERROR;
	}
	
	transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
	if (transfer_err != 0) {
		return SD_COM_ERROR;
	}
	
	// this transfer allows SD Card to return to standby state
	transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
	if (transfer_err != 0) {
		return SD_COM_ERROR;
	}

	return 0;
}

void cs_pin_init() {
	gpio_output_init(SD_CS_REG, SD_CS_PIN_MASK);
}

void cs_pin_set(uint8_t pin_val) {
	if (pin_val == 0) {
		gpio_output_clear(SD_CS_REG, SD_CS_PIN_MASK);
	} else {
		gpio_output_set(SD_CS_REG, SD_CS_PIN_MASK);
	}
}
