/*
 * leds.c
 *
 * Created: 9/13/2023 2:26:24 PM
 *  Author: sjsmdd
 */


#include "leds.h"
#include "io.h"


void gpio_led_init(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	gpio_output_init(port_addr, pin_mask);
	gpio_led_off(port_addr, pin_mask);
}


void gpio_led_on(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	gpio_output_clear(port_addr, pin_mask);
}


void gpio_led_off(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	gpio_output_set(port_addr, pin_mask);
}

