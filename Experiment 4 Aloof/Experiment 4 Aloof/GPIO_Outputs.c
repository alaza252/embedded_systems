/*
 * GPIO_Outputs.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: Roger Younger
 */ 

#include "board.h"
#include "GPIO_Outputs.h"

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

void GPIO_output_init(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//Set Port Pin to be an output with initial value '1'
	(port_addr->GPIO_DDR)|=(pin_mask);    // Set DDRx to make port pin an output
}







