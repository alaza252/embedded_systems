/*
 * GPIO_inputs.h
 *
 * Created: 9/11/2023 3:21:57 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 


#ifndef GPIO_INPUTS_H_
#define GPIO_INPUTS_H_

#include "Board.h"

/*
 * Function:  void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  initializes a chosen pin as an input
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *  GPIO_DDR determines if a pin is an input or output, and 0 means input
 *  GPIO_PIN controls the pullup resistors for those pins
 *  returns: none
 */
void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask);

/*
 * uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *  input_temp: temporary variable storing the current input state at that pin number
 *  returns: 1U if pin is high, 0U if low
 */
uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin);

#endif /* GPIO_INPUTS_H_ */