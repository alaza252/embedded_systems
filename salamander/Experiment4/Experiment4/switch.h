/*
 * leds.c
 *
 * Created: 9/15/2023 2:02:56 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#ifndef SWITCH_H
#define SWITCH_H

#include "board.h"

#define SW0_REG ((volatile GpioPort_t *) & PINC)
#define SW0_PIN_MASK (1<<6)

#define SW1_REG ((volatile GpioPort_t *) & PINB)
#define SW1_PIN_MASK (1<<2)

#define SW2_REG ((volatile GpioPort_t *) & PINA)
#define SW2_PIN_MASK (1<<4)

#define SW3_REG ((volatile GpioPort_t *) & PINA)
#define SW3_PIN_MASK (1<<5)


/**
 * Initializes the given pin(s) to be an input for a switch (button).
 * The implementation will handle whether or not the pullup resistor needs to be enabled
 */
void gpio_switch_init(volatile GpioPort_t *port_addr, uint8_t pin_mask);

/**
 * Returns 1 if the switch is pressed down, 0 otherwise.
 * Note that this function does not perform any debouncing. This value may fluctuate after its state changes.
 */
uint8_t gpio_switch_pressed(volatile GpioPort_t *port_addr, uint8_t pin_mask);

/**
 * Returns 1 if the switch is pressed down, 0 otherwise.
 * This function debounces the switch being pressed.
 */
uint8_t read_switch(volatile GpioPort_t *port_addr, uint8_t pin_mask);


#endif
