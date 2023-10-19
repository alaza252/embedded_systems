/*
 * Experiment2_Aloof.c
 *
 * Created: 9/29/2023 2:07:35 PM
 * Author : Alisa Lazareva and Drew Schulte
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "UART.h"
#include <util/delay.h>
#include "GPIO_inputs.h"
#include "GPIO_outputs.h"
#include "UART_Print.h"
#include "print_memory.h"
#include <stdio.h>	
	
	
int main(void)
{
	GPIO_Output_init(LED0,LED0_MASK);
	GPIO_Output_set(LED0,LED0_MASK);		//Setting our LED to OFF
	
	const char test_string[15] PROGMEM = {"Hello World!\n\r\0"};
	uint8_t TEMP_REC;
    UART_init(UART1, 9600);
	
	//step 6
	sprintf(export_print_buffer(),"dummystring\n\r");
	UART_transmit_string(UART1,export_print_buffer(),0);
	
	
	//step 7
	copy_string_to_buffer(test_string, export_print_buffer(),2);
	UART_transmit_string(UART1 ,test_string,0);
	
	
	//step 8
	const char Name_String[36] PROGMEM = {"Alisa Lazareva and Drew Schulte!\n\r\0"};
	copy_string_to_buffer(Name_String, export_print_buffer(),2);
	UART_transmit_string(UART1 ,Name_String,0);
	
	while(1)
	{
		TEMP_REC = UART_recieve(UART1);
		UART_transmit(UART1,TEMP_REC);
		GPIO_Output_clear(LED0,LED0_MASK);	//LED on
		_delay_ms(100);						//wait 0.1s
		GPIO_Output_set(LED0,LED0_MASK);	//LED off
		_delay_ms(400);						//wait 0.4s
	}
}

