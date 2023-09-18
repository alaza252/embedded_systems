/*
 * GPIO_outputs.c
 *
 * Created: 9/11/2023 4:00:49 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 
#include "switches.h"
#include "GPIO_outputs.h"
 
 /*
 * void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_DDR determines the pin output/input status, 1 meaning output
 *
 *  returns: none
 */
 void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_DDR |= pinmask;
 }
 
 /*
 * void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_PORT determines the output value, and here it is set high
 *
 *  returns: none
 */
 void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_PORT |= pinmask;
 }
 
  /*
 * void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_PORT determines the output value, and here it is set low
 *
 *  returns: none
 */
 void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask)
 {
	 port_addr -> GPIO_PORT &= ~pinmask;
 }