/*
 * LEDS.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: youngerr
 */ 

#include "board.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"

void LEDS_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	//Set Port Pin to be an output with initial value '1'
	if(ACTIVE_LOW==active)
	{
		GPIO_Output_Set(port_addr,pin_mask);
	}
	else
	{
		GPIO_Output_Clear(port_addr,pin_mask);
	}
	GPIO_Output_Init(port_addr,pin_mask);
}

void LEDS_on(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	if(ACTIVE_LOW==active)
	{
		GPIO_Output_Clear(port_addr,pin_mask); 
	}
	else
	{
		GPIO_Output_Set(port_addr,pin_mask); 
	}
}

void LEDS_off(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	if(ACTIVE_LOW==active)
	{
		GPIO_Output_Set(port_addr,pin_mask);
	}
	else
	{
		GPIO_Output_Clear(port_addr,pin_mask);
	}
}