/*
 * Experiment3.c
 *
 * Created: 10/16/2023 3:16:56 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "sd.h"
#include "spi.h"

#define SD_SPI_PORT SPI0

uint8_t send_command(uint8_t cmd_val, uint32_t arg) {
	if (cmd_val >= 64) {
		return SD_ILLEGAL_CMD;
	}
	
	uint8_t transfer_val;
	uint8_t transfer_err;
	
	transfer_err = spi_transfer(SD_SPI_PORT, (0x40|cmd_val), &transfer_val);
	// TODO check if transfer_err is non-zero and return error flag
	for(uint8_t i = 0; i < 4; i++) {
		transfer_err = spi_transfer(SD_SPI_PORT, ((uint8_t) (arg >> (24-(i*8)))), &transfer_val);
		// TODO check if transfer_err is non-zero and return error flag
	}
	
	// TODO 0x95 is hard coded to work ONLY for CMD0
	transfer_err = spi_transfer(SD_SPI_PORT, 0x95, &transfer_val);
	// TODO check if transfer_err is non-zero and return error flag
	return 0;
}