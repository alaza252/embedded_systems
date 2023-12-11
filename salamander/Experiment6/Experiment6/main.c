/*
 * FS_solution_struct.c
 *
 * Created: 10/5/2021 3:06:09 PM
 * Author : Roger Younger
 */ 

#include <avr/io.h>
#include "board.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "UART.h"
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "UART_Print.h"
#include "print_memory.h"
#include "Long_Serial_In.h"
#include <stdio.h>
#include "SPI.h"
#include "SDCard.h"
#include "OLED.h"
#include "Directory_Functions_struct.h"
#include "File_System_struct.h"
#include "TWI.h"
#include "STA013_Config.h"
#include "Temperature_Sensor.h"
#include "Play_Song.h"


const char test_string[28] PROGMEM = {"SD Initialization Program\n\r\0"};
const char LSI_Prompt[16] PROGMEM = {"Enter block #: "};
const char Complete[9] PROGMEM = {"  OK!\n\r\0"};
const char High_Cap[15] PROGMEM = {"High Capacity\0"};
const char Stnd_Cap[19] PROGMEM = {"Standard Capacity\0"};

uint8_t buffer1_g[512];
uint8_t buffer2_g[512];


int main(void)
{
	char temp8,error_flag;
	uint32_t input32, Current_directory, num_entries, Entry_clus;
	char *string_p;
	FS_values_t * Drive_p;
	LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	LEDS_init(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	LEDS_off(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	LEDS_init(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	LEDS_off(LED2_PORT, LED2_PIN,ACTIVE_LOW);
	LEDS_init(LED2_PORT, LED2_PIN,ACTIVE_LOW);
	LEDS_off(LED3_PORT, LED3_PIN,ACTIVE_LOW);
	LEDS_init(LED3_PORT, LED3_PIN,ACTIVE_LOW);
	UART_init(UART1,9600);
	string_p=export_print_buffer();
	copy_string_to_buffer(test_string,string_p,0);
	UART_transmit_string(UART1,string_p,0);
	TWI_Master_Init(STA013_TWI_Port,50000UL);
	
	LEDS_on(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	_delay_ms(100);
	LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	//**** Make sure all SPI devices are disabled ****
	SD_Card_CS_Inactive(SD_CS_Port, SD_CS_Pin);
	SD_Card_CS_Init(SD_CS_Port, SD_CS_Pin);
	OLED_SS_init();
	error_flag=SPI_master_init(SPI0,400000);
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_on(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	}
	OLED_init(OLED_SPI_Port);
	OLED_set_line(OLED_SPI_Port,OLED_LINE_0);
	copy_string_to_buffer(test_string,string_p,7);
	OLED_transmit_string(OLED_SPI_Port,string_p,7);
	error_flag=SD_Card_Init();
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_on(LED2_PORT, LED2_PIN,ACTIVE_LOW);
	}
	error_flag=SPI_master_init(SPI0,10000000);
	copy_string_to_buffer(Complete,string_p,0);
	OLED_transmit_string(OLED_SPI_Port,string_p,5);
	OLED_set_line (OLED_SPI_Port,OLED_LINE_1);
	temp8=Return_SD_Card_Type();
	if(temp8==Standard_Capacity)
	{
		copy_string_to_buffer(Stnd_Cap,string_p,0);
		OLED_transmit_string(OLED_SPI_Port,string_p,0);
	}
	else
	{
		copy_string_to_buffer(High_Cap,string_p,0);
		OLED_transmit_string(OLED_SPI_Port,string_p,0);
	}
	LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	
	STA013_Init();
	
	mount_drive(buffer1_g);
	Drive_p=export_drive_values();
	Current_directory=Drive_p->FirstRootDirSec;
	while (1)
	{
			sprintf(string_p,"Current Directory = %lu\n\r",Current_directory);
			UART_transmit_string(UART1,string_p,0);
			num_entries=print_directory(Current_directory, buffer1_g);
			sprintf(string_p,"Enter Selection = ");
			UART_transmit_string(UART1,string_p,0);
			input32=(uint16_t)long_serial_input(UART1);
			if(input32<=num_entries)
			{
				Entry_clus=read_dir_entry(Current_directory, input32, buffer1_g);
				if((Entry_clus&directory_bit)==directory_bit)
				{
					Entry_clus&=0x0FFFFFFF;
					Current_directory=first_sector(Entry_clus);
				}
				else
				{
					//open_file(Entry_clus, buffer1_g);
					Play_Song(Entry_clus);
				}
				
			}
			else
			{
				sprintf(string_p,"Invalid Selection\n\r");
				UART_transmit_string(UART1,string_p,0);
			}
			
	}
}
