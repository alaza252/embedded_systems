/*
 * GPIO_Inputs.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: Roger Younger
 */ 


#ifndef _GPIO_INPUTS_H_
#define _GPIO_INPUTS_H_

/*** Defined constants for GPIO_input_init ****/
#define PU_ENABLED (1)
#define PU_DISABLED (0)

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
void GPIO_input_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t pullup);
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
uint8_t GPIO_read_input(volatile GPIO_t * port_addr, uint8_t pin_mask);


/**************************************************************
*   GPIO_read_port
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*   Output: State of GPIO pin(s) in their bit locations
**************************************************************
*   Function: Checks the value of an GPIO pin and returns that value
*             Inline function to reduce read time
*   Caution: Output is pin_mask & pins that are '1'
*            Output value is in the bit location of the port
*               and is not purely '0' or '1'
***************************************************************/
static inline uint8_t GPIO_read_port(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	return ((port_addr->GPIO_PIN)&pin_mask);
}


#endif /* _GPIO_INPUTS_H_ */