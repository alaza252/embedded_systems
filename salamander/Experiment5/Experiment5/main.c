/*
 * main.c
 *
 * Created: 10/16/2023 2:08:07 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#include "board.h"
#include "leds.h"
#include "switch.h"
#include "uart.h"
#include "uart_print.h"
#include "print_memory.h"
#include "spi.h"
#include "sd_spi.h"
#include "sd.h"
#include "long_serial_in.h"
#include "print_memory.h"
#include "sta013.h"
#include "directory_functions.h"
#include "read_sector.h"
#include "mount_drive.h"
#include "print_file.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
// 	//init UART
// 	uart_init(UART1, 9600UL);
// 	
// 	//init LEDs
// 	gpio_led_init(LED0_REG, LED0_PIN_MASK);
// 
// 	uint8_t err = sta013_init();
// 	
// 	sprintf(export_print_buffer(), "Error: %i!\r\n", err);
// 	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	//init UART
	uart_init(UART1, 9600UL);
	
	//init SPI
	spi_master_init(SPI0, 400000UL);
	
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);

	SDInfo sd_info;
	uint8_t sd_card_init_error = sd_card_init(&sd_info);
	
	if (sd_card_init_error != 0) {
		sprintf(export_print_buffer(), "SD Card Init Error was: %i!\r\n", sd_card_init_error);
		uart_transmit_string(UART1, export_print_buffer(), 0);
		return 0;
	}
	sprintf(export_print_buffer(), "Card type value: %i, card capacity value: %i\r\n", sd_info.card_type, sd_info.capacity);
	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	//spi_master_init(SPI0, 25000000UL);
	spi_master_init(SPI0, 8000000UL); // 8MHz is the maximum of our board, if we try to use a value higher than this, the function that calculates the divider will get very angry

	FatInfo fat_info;
	uint8_t data[512];
	uint8_t error;
	
	//mount_drive(&fat_info, data);
	error = mount_drive(&fat_info, data);
	
	if (error != 0) {
		sprintf(export_print_buffer(), "Error while mounting drive: %i!\r\n", error);
		uart_transmit_string(UART1, export_print_buffer(), 0);
	}
	
// 	
// 	sprintf(export_print_buffer(), "Some values BytesPerSec: %i   SecPerClus: %i   StartofFAT: %lu\r\n", fat_info.BytesPerSec, fat_info.SecPerClus, fat_info.StartofFAT);
// 	uart_transmit_string(UART1, export_print_buffer(), 0);
	
	
	uint32_t current_working_directory_sector_num = fat_info.FirstRootDirSec;
	
	for (;;) {
		uint16_t number_of_entries = print_directory(&fat_info, current_working_directory_sector_num, data);
		uint16_t entry_number = (uint16_t) long_serial_input(UART1); // case to 16 bit because no one should give us a huge value (right?)
		if (entry_number > number_of_entries) {
			sprintf(export_print_buffer(), "Hey! entry number: %i is too big!!\r\n", entry_number);
			uart_transmit_string(UART1, export_print_buffer(), 0);
		} else {
			uint32_t entry_cluster_value = read_dir_entry(&fat_info, current_working_directory_sector_num, entry_number, data);
			uint32_t entry_cluster = entry_cluster_value & 0x0FFFFFFF;
			uint32_t is_error = entry_cluster_value & 0x80000000;
			uint32_t is_directory = entry_cluster_value & 0x10000000;
			if (is_error != 0) {
				sprintf(export_print_buffer(), "Error occurred!\r\n");
				uart_transmit_string(UART1, export_print_buffer(), 0);
			} else if (is_directory != 0) {
				sprintf(export_print_buffer(), "Changing working directory!\r\n");
				uart_transmit_string(UART1, export_print_buffer(), 0);
				current_working_directory_sector_num = first_sect(&fat_info, entry_cluster);
			} else {
				// it is a file
				sprintf(export_print_buffer(), "You selected a file!\r\n");
				uart_transmit_string(UART1, export_print_buffer(), 0);
				print_file(&fat_info, entry_cluster, data);
			}
		}
	}
	
}


