/*
 * mount_drive.c
 *
 * Created: 11/26/2023 2:59:26 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "read_sector.h"
#include "mount_drive.h"
#include "uart.h"
#include "uart_print.h"
#include <stdio.h>

// look at lecture 15b for these steps
uint8_t mount_drive(FatInfo *fat_info, uint8_t buffer[]) {
	read_sector(0, FAT_SECTOR_SIZE, buffer);
	
	uint32_t mbr_relative_sects; // this is the sector number of the BPB
	
	if (read_val_8(0, buffer) != 0xEB && read_val_8(0, buffer) != 0xE9) { // this is the MBR
		mbr_relative_sects = read_val_32(0x01C6, buffer); // this represents the sector number of the BPB
		read_sector(mbr_relative_sects, FAT_SECTOR_SIZE, buffer); // so, this reads the BPB into buffer
		
		if (read_val_8(0, buffer) != 0xEB && read_val_8(0, buffer) != 0xE9) { 
			return MOUNT_DRIVE_BPB_NOT_FOUND;
		}
	} else {
		mbr_relative_sects = 0;
	}
	
	// buffer contains BPB at this point
	
	uint16_t fat_size_16 = read_val_16(0x0016, buffer);
	
	if (fat_size_16 != 0) {
		return MOUNT_DRIVE_FAT_16_NOT_SUPPORTED;
	}
	
	fat_info -> FATtype = FAT32;

	fat_info -> BytesPerSec = read_val_16(0x000B, buffer);
	fat_info -> SecPerClus = read_val_8(0x000D, buffer);
	
	uint32_t fat_size = read_val_32(0x0024, buffer); // read from BPB_FATSz32 b/c we only support FAT32
	
	// Step 1 of lecture 15b
	
	fat_info -> RootDirSecs = 0; // on FAT32 this val is always 0
	
	// Step 2 lecture 15b
	
	uint32_t total_sects;
	
	if (read_val_16(0x0013, buffer) != 0) { // TotSec16 is usually 0 on small cap disks and does not indicate FAT16 or FAT32
		total_sects = read_val_16(0x0013, buffer);
	} else {
		total_sects = read_val_32(0x0020, buffer);
	}
	
	uint16_t reserved_sect_count = read_val_16(0x000E, buffer);
	uint8_t num_fats = read_val_8(0x0010, buffer);
	
	uint32_t data_sects = total_sects - (reserved_sect_count + (num_fats * fat_size) + (fat_info -> RootDirSecs)); // we expect RootDirSecs to be 0 but are including for completeness
	
	// Step 3 lecture 15b
	
	uint8_t sects_per_clust = read_val_8(0x000D, buffer);
	
	uint32_t clust_count = data_sects / sects_per_clust;
	
	if (clust_count < 4085) {
		return MOUNT_DRIVE_UNEXPECTED_FAT12_VAL;
	} else if (clust_count < 65525) {
		return MOUNT_DRIVE_UNEXPECTED_FAT16_VAL;
	} // else vol is FAT32
	
	// Step 4 lecture 15b
	
	fat_info -> StartofFAT = reserved_sect_count + mbr_relative_sects;
	
	// Step 5 lecture 15b
	
	fat_info -> FirstDataSec = reserved_sect_count + (num_fats * fat_size) + (fat_info -> RootDirSecs) + mbr_relative_sects;
	
	// Step 6 lecture 15b
	
	uint32_t root_clust = read_val_32(0x002C, buffer);
	
	fat_info -> FirstRootDirSec = ((root_clust - 2) * sects_per_clust) + (fat_info -> FirstDataSec);
	
// 	sprintf(export_print_buffer(), "SectsPerClust is : %i!\r\n", sects_per_clust);
// 	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	return 0;
}

uint32_t first_sect(FatInfo *fat_info, uint32_t clust_num) {
	if (clust_num == 0) {
		return fat_info -> FirstRootDirSec;
	}
	
	return (((clust_num - 2) * (fat_info -> SecPerClus)) + (fat_info -> FirstDataSec));
}

uint8_t find_next_clust(FatInfo *fat_info, uint32_t clust_num, uint8_t arr[], uint32_t *next_cluster_out) {
	uint32_t fat_offset = clust_num * 4; // multiply by 4 is specific to FAT32
	
	uint32_t cur_clust_sect_num = (fat_info -> StartofFAT) + (fat_offset / (fat_info -> BytesPerSec));
	uint8_t error = read_sector(cur_clust_sect_num, FAT_SECTOR_SIZE, arr);
	if (error != 0) {
		return FIND_NEXT_CLUST_READ_ERROR;
	}
	uint32_t entry_offset = fat_offset % fat_info -> BytesPerSec;
	
	(*next_cluster_out) = read_val_32(entry_offset, arr) & 0x0FFFFFFF;
	
	return 0;
}
