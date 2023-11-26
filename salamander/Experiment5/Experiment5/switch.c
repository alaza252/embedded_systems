/*
 * leds.c
 *
 * Created: 9/15/2023 2:03:21 PM
 * Author: Sam Stockmann, Lavender Shannon
 */

#include "switch.h"
#include "io.h"
#include <util/delay.h>

#define PULL_UP_ENABLED_FOR_SWITCH (1)


void gpio_switch_init(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	gpio_input_init(port_addr, pin_mask, PULL_UP_ENABLED_FOR_SWITCH);
}

uint8_t gpio_switch_pressed(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	return gpio_input_read_pin(port_addr, pin_mask) == 0 ? 1 : 0;
}

uint8_t read_switch(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	uint8_t ctr = 0; // counter
	uint8_t ret_val = 1; //return value
	while(ctr < 8 && ret_val == 1)
	{
		if (ctr != 0) // if not first iteration
		{
			_delay_ms(10);
		}
		
		if (gpio_switch_pressed(port_addr, pin_mask) == 0) // if switch not pressed
		{
			ret_val = 0;
		}
		
		ctr++;
	}
	
	return ret_val;
}
