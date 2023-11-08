/*
 * GPIO_Outputs.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: youngerr
 */ 

#include "board.h"

void GPIO_Output_Init(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	//Set Port Pin to be an output with initial value '1'
	(port_addr->GPIO_DDR)|=(pin_mask);    // Set DDRx to make port pin an output
}




