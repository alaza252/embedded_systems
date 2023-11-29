/*
 * print_file.c
 *
 * Created: 11/28/2023 5:03:05 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "print_file.h"
#include "mount_drive.h"
#include "read_sector.h"
#include "print_memory.h"

#include "uart.h"
#include "uart_print.h"
#include <stdio.h>

uint8_t print_file(FatInfo *fat_info, uint32_t file_first_cluster_num, uint8_t buffer[]) {
	uint8_t error;

	uint32_t cluster = file_first_cluster_num;
	while (cluster != 0x0FFFFFFF) {

		for (uint8_t i = 0; i < fat_info->SecPerClus; i++) {
			uint32_t sector = first_sect(fat_info, cluster) + i;
			error = read_sector(sector, fat_info->BytesPerSec, buffer);
			if (error != 0) {
				sprintf(export_print_buffer(), "Error occurred while reading sector. Error: %i\r\n", error);
				uart_transmit_string(UART1, export_print_buffer(), 0);
				return PRINT_FILE_READ_ERROR;
			}
			sprintf(export_print_buffer(), "Printing sector: %i (%lu) of cluster: %lu.\r\n", i, sector, cluster);
			uart_transmit_string(UART1, export_print_buffer(), 0);
			print_memory(buffer, fat_info->BytesPerSec);
		}
		error = find_next_clust(fat_info, cluster, buffer, &cluster);
		if (error != 0) {
			sprintf(export_print_buffer(), "Error occurred while finding next cluster. Error: %i\r\n", error);
			uart_transmit_string(UART1, export_print_buffer(), 0);
			return PRINT_FILE_READ_ERROR;
		}
	}

	return 0;
}
