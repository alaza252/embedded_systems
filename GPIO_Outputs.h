/*
 * GPIO_Outputs.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef _GPIO_OUTPUTS_H_
#define _GPIO_OUTPUTS_H_

void GPIO_Output_Init(volatile GPIO_t * port_addr, uint8_t pin_mask);
static inline void GPIO_Output_Set(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)|=(pin_mask);    // Set PORTx PORTx pin to value of '1'
}
static inline void GPIO_Output_Clear(volatile GPIO_t * port_addr, uint8_t pin_mask)
{
	(port_addr->GPIO_PORT)&=~(pin_mask);    // Clear PORTx pin to value of '0'
}


#endif /* _GPIO_OUTPUTS_H_ */