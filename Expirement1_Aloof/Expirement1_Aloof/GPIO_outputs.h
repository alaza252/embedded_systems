/*
 * GPIO_outputs.h
 *
 * Created: 9/11/2023 3:22:16 PM
 *  Author: dnsdyy
 */ 


#ifndef GPIO_OUTPUTS_H_
#define GPIO_OUTPUTS_H_
#include "board.h"


void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask);
void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask);
void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask);


#endif /* GPIO_OUTPUTS_H_ */