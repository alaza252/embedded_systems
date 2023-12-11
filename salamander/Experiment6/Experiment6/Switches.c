/*
 * Switches.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: Roger Younger
 */ 

#include "board.h"
#include <util/delay.h>
#include "GPIO_Inputs.h"
#include "Switches.h"

/**************************************************************
*   switch_init
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select a GPIO pin,
*           pullup set to PU_ENABLE or PU_DISABLE
*   Output: None
**************************************************************
*   Function: Calls GPIO_input_init
*
*   Caution: Must use the defined constants PU_ENABLE, PU_DISABLE
***************************************************************/
void switch_init(volatile GPIO_t *port_addr, uint8_t pin_mask,uint8_t pullup)
{
	GPIO_input_init(port_addr,pin_mask,pullup);
}


/**************************************************************
*   read_switch
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select a GPIO pin,
*           debounce time in ms
*   Output: Value indicating SW_PRESSED or SW_NOT_PRESSED
**************************************************************
*   Function: Reads an input pin, if low, then it waits for
*             debounce time and reads again.
*             If low, then the switch is pressed
*
*   Caution: Assumes active low switch, not for real-time because of delay
***************************************************************/
uint8_t read_switch(volatile GPIO_t *port_addr, uint8_t pin_mask, uint8_t debounce_time)
{
	uint8_t input=SW_NOT_PRESSED;
	input=GPIO_read_input(port_addr,pin_mask);
	if(0==input)  // switch is pressed
	{
		_delay_ms(debounce_time);
		input=GPIO_read_input(port_addr,pin_mask);
		if(0==input)
		{
			input=SW_PRESSED;
		}
	}
	return input;
}