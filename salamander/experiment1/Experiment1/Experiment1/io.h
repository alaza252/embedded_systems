/*
 * io.h
 *
 * Created: 9/13/2023 2:08:01 PM
 *  Author: sjsmdd
 */


#ifndef IO_H
#define IO_H

#include "board.h"


void gpio_output_init(volatile GpioPort_t *port_addr, uint8_t pin_mask);
void gpio_output_set(volatile GpioPort_t *port_addr, uint8_t pin_mask);
void gpio_output_clear(volatile GpioPort_t *port_addr, uint8_t pin_mask);

void gpio_input_init(volatile GpioPort_t *port_addr, uint8_t pin_mask, uint8_t pullup_enable);
/**
 * Returns the value of the port logical AND'd with the pin_mask
 */
uint8_t gpio_input_read_port(volatile GpioPort_t *port_addr, uint8_t pin_mask);
/**
 * Returns 1 if any bit of the pin_mask is set on both the pin_mask and the port. Returns 0 otherwise
 */
uint8_t gpio_input_read_pin(volatile GpioPort_t *port_addr, uint8_t pin_mask);

#endif