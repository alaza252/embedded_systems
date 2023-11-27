/*
 * Read_Sector.h
 *
 * Created: 11/26/2023 2:52:07 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 


#ifndef READ_SECTOR_H
#define READ_SECTOR_H

uint8_t read_sector(uint32_t sect_num, uint16_t sect_size, uint8_t * data_arr);

uint8_t read_val_8(uint16_t offset, uint8_t arr[]);

uint16_t read_val_16(uint16_t offset, uint8_t arr[]);

uint32_t read_val_32(uint16_t offset, uint8_t arr[]);


#endif