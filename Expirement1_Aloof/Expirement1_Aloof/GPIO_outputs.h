/*
 * GPIO_outputs.h
 *
 * Created: 9/11/2023 3:22:16 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 


#ifndef GPIO_OUTPUTS_H_
#define GPIO_OUTPUTS_H_

#include "board.h"

/*
 * void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_DDR determines the pin output/input status, 1 meaning output
 *
 *  returns: none
 */
void GPIO_Output_init( volatile GPIO_t * port_addr, uint8_t pinmask);

 /*
 * void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_PORT determines the output value, and here it is set high
 *
 *  returns: none
 */
void GPIO_Output_set( volatile GPIO_t * port_addr, uint8_t pinmask);

 /*
 * void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  GPIO_PORT determines the output value, and here it is set low
 *
 *  returns: none
 */
void GPIO_Output_clear( volatile GPIO_t * port_addr, uint8_t pinmask);

#endif /* GPIO_OUTPUTS_H_ */