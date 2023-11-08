/*
 * Experiment3.c
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
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

int main(void)
{
	//init UART
	uart_init(UART1, 9600UL);
	
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);

	sta013_init();
}


