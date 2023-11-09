/*
 * Main.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: Drew and Alisa
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






int main(void)
{
	UART_init(UART1 , 9600UL);
	GPIO_output_init(LED0_PIN,LED0_PIN);
	sta013_init();

}


