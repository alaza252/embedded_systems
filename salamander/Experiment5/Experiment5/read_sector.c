/*
 * Read_Sector.c
 *
 * Created: 11/26/2023 2:52:51 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "read_sector.h"

#include "sd_spi.h"
#include "spi.h"

// Step 3
uint8_t read_sector(uint32_t sect_num, uint16_t sect_size, uint8_t * data_arr) {
	cs_pin_set(0);

	uint8_t error;

	error = send_command(17, sect_num); // TODO can we just pass in sect_num as the block_num here?
	if (error != 0) {
		cs_pin_set(1);
		return READ_SECTOR_COM_ERROR;
	}

	error = read_block(sect_size, data_arr);
	if (error != 0) {
		cs_pin_set(1);
		return READ_SECTOR_COM_ERROR;
	}

	cs_pin_set(1);
	return 0;
}


// Step 4 - all 3 of these functions

uint8_t read_val_8(uint16_t offset, uint8_t arr[]) {
	return arr[offset];
}

uint16_t read_val_16(uint16_t offset, uint8_t arr[]) {
	uint16_t r = 0;
	for (uint16_t i = offset; i < offset + 2; i++) {
		r <<= 8;
		r |= arr[i];
	}
	return r;
}

uint32_t read_val_32(uint16_t offset, uint8_t arr[]) {
	uint32_t r = 0;
	for (uint16_t i = offset; i < offset + 4; i++) {
		r <<= 8;
		r |= arr[i];
	}
	return r;
}
