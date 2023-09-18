/*
 * GPIO_inputs.c
 *
 * Created: 9/11/2023 4:00:36 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 
#include "GPIO_inputs.h"

/*
 * Function:  void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  initializes a chosen pin as an input
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *  GPIO_DDR determines if a pin is an input or output, and 0 means input
 *  GPIO_PIN controls the pullup resistors for those pins
 *  returns: none
 */
void GPIO_Input_init( volatile GPIO_t * port_addr, uint8_t pinmask)
{
	port_addr -> GPIO_DDR &= ~pinmask;
	port_addr -> GPIO_PIN |= pinmask;
}

/*
 * uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *  input_temp: temporary variable storing the current input state at that pin number
 *  returns: 1U if pin is high, 0U if low
 */
uint8_t GPIO_Input_read( volatile GPIO_t * port_addr, uint8_t pin)
{
	volatile uint8_t input_temp;
	input_temp = (port_addr -> GPIO_PIN) & pin;
	if(input_temp > 0U)
	{
		return 1U;
	}
	return 0U;
}