/*
 * SD_Card_solution_struct.c
 *
 * Created: 10/5/2021 3:06:09 PM
 * Author : Roger Younger
 */ 

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
#include "SD_Read_Sector.h"


const char test_string[28] PROGMEM = {"SD Initialization Program\n\r\0"};
const char LSI_Prompt[16] PROGMEM = {"Enter block #: "};
const char Complete[9] PROGMEM = {"  OK!\n\r\0"};
const char High_Cap[15] PROGMEM = {"High Capacity\0"};
const char Stnd_Cap[19] PROGMEM = {"Standard Capacity\0"};

uint8_t buffer1_g[512];
uint8_t buffer2_g[512];


int main(void)
{
	char temp8,error_flag,SD_type;
	char *string_p;
	uint32_t temp32;
	
	
	
	//INIT ALL OF THE LEDS
	LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	LEDS_init(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	LEDS_off(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	LEDS_init(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	LEDS_off(LED2_PORT, LED2_PIN,ACTIVE_LOW);
	LEDS_init(LED2_PORT, LED2_PIN,ACTIVE_LOW);
	LEDS_off(LED3_PORT, LED3_PIN,ACTIVE_LOW);
	LEDS_init(LED3_PORT, LED3_PIN,ACTIVE_LOW);
	
	
	
	
	//INIT UART
	UART_init(print_port,9600);
	string_p=export_print_buffer();
	copy_string_to_buffer(test_string,string_p,0);
	UART_transmit_string(print_port,string_p,0);
	LEDS_on(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	_delay_ms(100);
	LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
	
	
	
	
	//*** Make sure all SPI devices are disabled before accessing any of them
	SD_Card_CS_Init(SD_CS_Port,SD_CS_Pin);
	SD_Card_CS_Inactive(SD_CS_Port,SD_CS_Pin);	
	OLED_SS_init();
	//*** SPI initialization for OLED and SD Card
	error_flag=SPI_master_init(SPI0,400000);
	if(error_flag!=no_errors)
	{
		while(1);
		LEDS_on(LED1_PORT, LED1_PIN,ACTIVE_LOW);
	}
	OLED_init(OLED_SPI_Port);
	OLED_set_line (OLED_SPI_Port,OLED_LINE_0);
	copy_string_to_buffer(test_string,string_p,0);
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
	
	
	uint8_t TempArr [512];
	FS_values_t FAT_INFO;
	uint8_t error;
	error=mount_drive(&FAT_INFO,TempArr);
	
	
	
	
	//Main Wile Loop
	while (1)
	{
		copy_string_to_buffer(LSI_Prompt,string_p,0);
		UART_transmit_string(print_port,string_p,0);
		temp32=long_serial_input(print_port);
		sprintf(string_p," %lu \n\r",temp32);
		UART_transmit_string(print_port,string_p,0);
		SD_type=Return_SD_Card_Type();
		temp32=(temp32<<SD_type);
		LEDS_on(LED0_PORT, LED0_PIN,ACTIVE_LOW);
		SD_Card_CS_Active(SD_CS_Port,SD_CS_Pin);
		Send_Command(CMD17,temp32);
		Read_Block(512,buffer1_g);
		LEDS_off(LED0_PORT, LED0_PIN,ACTIVE_LOW);
		SD_Card_CS_Inactive(SD_CS_Port,SD_CS_Pin);
		print_memory(buffer1_g,512);
		
	}
}

