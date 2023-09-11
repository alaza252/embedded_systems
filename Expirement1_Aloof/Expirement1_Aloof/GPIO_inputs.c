/*
 * GPIO_inputs.c
 *
 * Created: 9/11/2023 4:00:36 PM
 *  Author: dnsdyy
 */ 

#include <GPIO_inputs.h>

void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask)
{
	port_addr -> GPIO_DDR &= ~pinmask;
}


uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin)
{
	return (port_addr -> GPIO_PORT) & pin;
}