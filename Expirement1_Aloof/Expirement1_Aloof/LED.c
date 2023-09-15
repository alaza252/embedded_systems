/*
 * LED.c
 *
 * Created: 9/11/2023 4:00:01 PM
 *  Author: dnsdyy
 */ 

#include "GPIO_outputs.h"
#include "LED.h"







void LEDS_init(volatile GPIO_t * port_addr, uint8_t led_pin)
{
	GPIO_Output_init(port_addr, led_pin);
	GPIO_Output_set(port_addr,led_pin);
	LEDS_OFF(port_addr,led_pin);	
}

void LEDS_ON(volatile GPIO_t * port_addr, uint8_t led_pin)
{
	GPIO_Output_clear(port_addr,led_pin);
}

void LEDS_OFF(volatile GPIO_t * port_addr, uint8_t led_pin)
{
	GPIO_Output_set(port_addr,led_pin);
}