/*
 * read_sector.h
 *
 * Created: 11/26/2023 2:52:07 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 


#ifndef READ_SECTOR_H
#define READ_SECTOR_H

#define READ_SECTOR_COM_ERROR 1

/**
 * (Step 3)
 * Reads one sector (block) from the SD card.
 * This function assumes the SD card version is >=v2.X and capacity is high
 *
 * This function handles setting the state of the CS pin
 */
uint8_t read_sector(uint32_t sect_num, uint16_t sect_size, uint8_t * data_arr);


// (Step 4)
// These values read from arr. The values in this array are assumed to be little-endian

uint8_t read_val_8(uint16_t offset, uint8_t arr[]);

uint16_t read_val_16(uint16_t offset, uint8_t arr[]);

uint32_t read_val_32(uint16_t offset, uint8_t arr[]);


#endif
