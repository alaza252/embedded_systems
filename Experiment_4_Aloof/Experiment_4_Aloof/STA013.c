/*
 * STA013.c
 *
 * Created: 11/8/2023 5:59:19 PM
 *  Author: dnsdyy
 */ 

#include "STA013.h"
#include "TWI.h"
#include "GPIO_Outputs.h"
#include "UART.h"
#include "UART_Print.h"
#include <stdio.h>
#include <util/delay.h>

uint8_t sta013_init()
{
	//need to call our init functions
	GPIO_output_init(PB1_PIN,PB1_MASK);
	twi_master_init(STA013_TWI, 44100UL);

	//We need to Pulse PB1 low for at least 100 n sec.
	GPIO_output_clear(PB1_PIN,PB1_MASK);
	_delay_us(1);
	GPIO_output_set(PB1_PIN,PB1_MASK);
	
	uint8_t arr[3];
	
	
	
	uint8_t err;
	//timeout variable
	uint8_t i = 0;
	do {
		err = twi_master_receive(STA013_TWI, 0x43, 3, arr);
		i++;
	} while(i < 10 && err != 0);
	sprintf(export_print_buffer(), "Received Value: %i!\r\n", arr[2]);
	UART_transmit_string(UART1, export_print_buffer(), 0);
		
	
	return 0;	
}


