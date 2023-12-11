/*
 * GPIO_Inputs.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: Roger Younger
 */ 

#include "board.h"
#include "GPIO_Inputs.h"

/**************************************************************
*   GPIO_input_init
*   Inputs: pointer to a GPIO port struct, 
*           pin_mask to select a GPIO pin,
*           pullup set to PU_ENABLE or PU_DISABLE 
*   Output: None
**************************************************************
*   Function: Pin set to be an input by clearing that bit in the DDR
*             Pull-up enabled by setting that bit in the PORT reg.
*
*   Caution: Must use the defined constants PU_ENABLE, PU_DISABLE
***************************************************************/
void GPIO_input_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t pullup)
{
	if(PU_ENABLED==pullup)
	{
		(port_addr->GPIO_PORT)|=(pin_mask);
	}
	else
	{
		(port_addr->GPIO_PORT)&=(~pin_mask);
	}
	(port_addr->GPIO_DDR)&=(~pin_mask);    // Set DDRx to make port pin an output
}

/**************************************************************
*   GPIO_read_input
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select a GPIO pin
*   Output: State of input pin (0 or 1)
**************************************************************
*   Function: Checks the value of an input pin and returns that value   
*
*   Caution: Intended to work with only one pin, output is 0 or 1
*            Will work with multiple pins, but all must be 0 to get output=0  
***************************************************************/
uint8_t GPIO_read_input(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	uint8_t input;
	input=(port_addr->GPIO_PIN);
	if(0!=(input&pin_mask))
	{
		input=1;
	} 
	else
	{
		input=0;
	}
	return input;
}

