/*
 * leds.h
 *
 * Created: 9/13/2023 2:26:24 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#ifndef LEDS_H
#define LEDS_H

#include "board.h"

#define LED0_REG ((volatile GpioPort_t *) & PINC)
#define LED0_PIN_MASK (1<<7)

#define LED1_REG ((volatile GpioPort_t *) & PINB)
#define LED1_PIN_MASK (1<<3)

#define LED2_REG ((volatile GpioPort_t *) & PINE)
#define LED2_PIN_MASK (1<<4)

#define LED3_REG ((volatile GpioPort_t *) & PINA)
#define LED3_PIN_MASK (1<<7)


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