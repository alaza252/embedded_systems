/*
 * GPIO_Outputs.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef _GPIO_OUTPUTS_H_
#define _GPIO_OUTPUTS_H_

/**************************************************************
*   GPIO_output_init
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*   Output: None
**************************************************************
*   Function: Pin set to be an output by setting that bit in the DDR
*
*   Caution: Does not set the pins to a default value
***************************************************************/

void GPIO_output_init(volatile GPIO_t * port_addr, uint8_t pin_mask);

/**************************************************************
*   GPIO_output_set
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*   Output: None
**************************************************************
*   Function: Pin set to '1' by setting that bit in the PORT reg.
*             Inline function for quicker GPIO output response
*   Caution: Can change any PORT reg. bit, not just outputs
***************************************************************/
static inline void GPIO_output_set(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)|=(pin_mask);    // Set PORTx PORTx pin to value of '1'
}


/**************************************************************
*   GPIO_output_clear
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*   Output: None
**************************************************************
*   Function: Pin cleared to '0' by clearing that bit in the PORT reg.
*             Inline function for quicker GPIO output response
*   Caution: Can change any PORT reg. bit, not just outputs
***************************************************************/
static inline void GPIO_output_clear(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)&=~(pin_mask);    // Clear PORTx pin to value of '0'
}


#endif /* _GPIO_OUTPUTS_H_ */