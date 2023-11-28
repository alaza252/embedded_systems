/*
 * print_file.c
 *
 * Created: 11/28/2023 5:03:05 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "print_file.h"
#include "mount_drive.h"


uint8_t print_file(FatInfo *fat_info, uint32_t file_cluster_num, uint8_t buffer[]) {
	uint8_t sector = first_sect(fat_info, file_cluster_num);
	
	
	return 0;
}
