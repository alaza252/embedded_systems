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
	SPI_master_init(SPI0,400000UL);
	GPIO_Output_Init(LED0_PORT,LED0_PIN);
	
	uint8_t sd_init_error = sd_card_init();
	
// 	
	//SPI_master_init(SPI0, 25000000UL);
	for (;;)
 	{
	}
	
	return 1U;
}

