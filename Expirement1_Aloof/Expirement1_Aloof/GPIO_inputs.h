/*
 * GPIO_inputs.h
 *
 * Created: 9/11/2023 3:21:57 PM
 *  Author: dnsdyy
 */ 


#ifndef GPIO_INPUTS_H_
#define GPIO_INPUTS_H_
#include "Board.h"

void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask);
uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin);




#endif /* GPIO_INPUTS_H_ */