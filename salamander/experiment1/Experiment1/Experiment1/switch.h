
#ifndef LEDS_H
#define LEDS_H

#include "board.h"

#define SW0_REG ((volatile GpioPort_t *) & PINC)
#define SW0_PIN_MASK (1<<6)

#define SW1_REG ((volatile GpioPort_t *) & PINB)
#define SW1_PIN_MASK (1<<2)

#define SW2_REG ((volatile GpioPort_t *) & PINA)
#define SW2_PIN_MASK (1<<4)

#define SW3_REG ((volatile GpioPort_t *) & PINA)
#define SW3_PIN_MASK (1<<5)


void gpio_switch_init(volatile GpioPort_t *port_addr, uint8_t pin_mask);
uint8_t gpio_switch_pressed(volatile GpioPort_t *port_addr, uint8_t pin_mask);


#endif
