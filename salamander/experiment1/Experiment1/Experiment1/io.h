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


#endif