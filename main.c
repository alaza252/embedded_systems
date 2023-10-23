/*
 * Experiment3_Aloof.c
 *
 * Created: 10/16/2023 2:04:42 PM
 * Author : dnsdyy
 */ 

#include "board.h"
#include "GPIO_Outputs.h"
#include "UART_Print.h"
#include "UART.h"
#include "print_memory.h"
#include "SD.h"
#include <avr/io.h>
/*#include <util/delay.h>*/


int main(void)
{
    /* Replace with your application code */
	//initialize
	UART_init(UART1,9600UL);
	SPI_master_init(SPI0, 400000UL);
  	GPIO_Output_Init(LED0_PORT,LED0_PIN);
	//char * arr = "Enter a block number to read from the SD card";
	
	sd_card_init();
	SPI_master_init(SPI0,8000000UL);
	
	
	for (;;)
 	{
		 uint32_t output_value;
		 output_value - long_serial_input(UART1);
		 //UART_transmit_string(UART1, arr, 0); //prompting the user
		 GPIO_Output_Clear(PB,CHIP_SELECT);
		 send_command(CMD17,output_value);
		 
		 uint8_t resp_array[512];
		 read_block(512U,resp_array);
		 
		 print_memory(resp_array,512U);
		 GPIO_Output_Set(PB,CHIP_SELECT);
	}
	
	return 1U;
}

