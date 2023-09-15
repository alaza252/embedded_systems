/*
 * switches.c
 *
 * Created: 9/15/2023 1:42:34 PM
 *  Author: dnsdyy
 */ 

#include "switches.h"
#include "GPIO_inputs.h"
#include <util/delay.h>
#define PRESSED 0U
#define DEBOUNCE_DELAY  70U

void SWITCH_init(volatile GPIO_t *port_addr, uint8_t pinmask)
{
	GPIO_Input_init( port_addr,  pinmask);
	
}
uint8_t SWITCH_press( volatile GPIO_t *port_addr, uint8_t pinmask)
{
	volatile uint8_t readvalue;
	readvalue = GPIO_Input_read(port_addr,pinmask);
	if (readvalue == PRESSED)
	{
		_delay_ms(DEBOUNCE_DELAY);
		readvalue = GPIO_Input_read(port_addr,pinmask);
		if (readvalue  == PRESSED)
		{	
			while(1)
			{
				readvalue = GPIO_Input_read(port_addr,pinmask);
				if (readvalue != PRESSED)
				{
					return 1U;
				}
			}
			
		}
	}
	return 0U;
}