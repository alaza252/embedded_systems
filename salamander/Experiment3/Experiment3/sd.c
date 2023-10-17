/*
 * Experiment3.c
 *
 * Created: 10/16/2023 3:16:56 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "sd.h"
#include "spi.h"

#define SD_SPI_PORT SPI0
#define CRC_POLY (0x89u)

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

uint8_t send_command(uint8_t cmd_val, uint32_t arg) {
	// Note: This function has multiple returns which are used as guard clauses. These guard clauses prevent many nested if statements to make this function more readable.
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
