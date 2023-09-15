/*
 * GPIO_inputs.c
 *
 * Created: 9/11/2023 4:00:36 PM
 *  Author: dnsdyy
 */ 
#include "LED.h"
#include "GPIO_inputs.h"


void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask)
{
	port_addr -> GPIO_DDR &= ~pinmask;
	port_addr -> GPIO_PIN |= pinmask;
}



uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin)
{
	volatile uint8_t input_temp;
	input_temp = (port_addr -> GPIO_PIN) & pin;
	if(input_temp > 0U)
	{
		return 1U;
	}
	return 0U;
}