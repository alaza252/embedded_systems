/*
 * io.h
 *
 * Created: 9/13/2023 2:08:10 PM
 *  Author: sjsmdd
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

