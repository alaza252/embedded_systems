#include "io.h"

#define PULL_UP_ENABLED_FOR_SWITCH (1)

void gpio_switch_init(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	gpio_input_init(port_addr, pin_mask, PULL_UP_ENABLED_FOR_SWITCH);
}
uint8_t gpio_switch_pressed(volatile GpioPort_t *port_addr, uint8_t pin_mask)
{
	return gpio_input_read_pin(port_addr, pin_mask);
}
