/*
 * switches.c
 *
 * Created: 9/15/2023 1:42:34 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 

#include "switches.h"
#include "GPIO_inputs.h"

#include <util/delay.h>

#define PRESSED 0U
#define DEBOUNCE_DELAY  70U

/*
 * void SWITCH_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  calls GPIO_Input_init
 *
 *  returns: none
 */
void SWITCH_init(volatile GPIO_t *port_addr, uint8_t pinmask)
{
	GPIO_Input_init( port_addr,  pinmask);	
}


/*
 * uint8_t SWITCH_press( volatile GPIO_t *port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  readvalue: stores the most-recent switch-state
 *  
 *  after being PRESSED (having a value of 0U), we delay by 70ms and read the input again
 *  we then wait until the switch is no longer being pressed (to count it as one button press)
 *
 *  returns: 1U if there was a button press, 0U if no press was detected
 */
uint8_t SWITCH_press( volatile GPIO_t *port_addr, uint8_t pinmask)
{
	volatile uint8_t readvalue;
	readvalue = GPIO_Input_read(port_addr,pinmask);
	if (readvalue == PRESSED)
	{
		_delay_ms(DEBOUNCE_DELAY);
		readvalue = GPIO_Input_read(port_addr,pinmask);
		if (readvalue  == PRESSED)
		{	
			while(1)
			{
				readvalue = GPIO_Input_read(port_addr,pinmask);
				if (readvalue != PRESSED)
				{
					return 1U;
				}
			}
		}
	}
	return 0U;
}