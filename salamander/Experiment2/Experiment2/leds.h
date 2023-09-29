/*
 * leds.h
 *
 * Created: 9/13/2023 2:26:24 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#ifndef LEDS_H
#define LEDS_H

#include "board.h"


/**
 * Initializes the given pin(s) to be LED outputs.
 */
void gpio_led_init(volatile GpioPort_t *port_addr, uint8_t pin_mask);

/**
 * Turns the LED on for the given pin(s)
 */
void gpio_led_on(volatile GpioPort_t *port_addr, uint8_t pin_mask);

/**
 * Turns the LED off for the given pin(s)
 */
void gpio_led_off(volatile GpioPort_t *port_addr, uint8_t pin_mask);


#endif