/*
 * GPIO_outputs.c
 *
 * Created: 9/11/2023 4:00:49 PM
 *  Author: dnsdyy
 */ 
#include "LED.h"
#include "switches.h"
#include "GPIO_outputs.h"
 
 
 void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_DDR |= pinmask;
 }
 
 void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_PORT |= pinmask;
 }
 
 void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_PORT &= ~pinmask;
 }