/*
 * io.h
 *
 * Created: 9/13/2023 2:08:10 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#include "io.h"


void gpio_output_init(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	port_addr->DDR_REG  |= pin_mask;
}


void gpio_output_set(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	port_addr->PORT_REG |= pin_mask;
}


void gpio_output_clear(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	port_addr->PORT_REG &= ~pin_mask;
}

void gpio_input_init(volatile GpioPort_t *port_addr, uint8_t pin_mask, uint8_t pullup_enable)
{
	port_addr->DDR_REG &= ~pin_mask;
	if (pullup_enable)
	{
		// When pullup enabled, PORT bit is high
		port_addr->PORT_REG |= pin_mask;
	}
	else
	{
		// When pullup disabled, PORT bit is low
		port_addr->PORT_REG &= ~pin_mask;
	}
}

uint8_t gpio_input_read_port(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	return port_addr->PIN_REG & pin_mask;
}

uint8_t gpio_input_read_pin(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	return gpio_input_read_port(port_addr, pin_mask) != 0 ? 1 : 0;
}
