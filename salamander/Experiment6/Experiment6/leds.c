/*
 * LEDS.c
 *
 * Created: 6/30/2020 11:32:36 AM
 *  Author: Roger Younger
 */ 

#include "board.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"

/**************************************************************
*   LEDS_init
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*           active selects when LED is on(active), ACTIVE_HIGH or ACTIVE_LOW
*   Output: None
**************************************************************
*   Function: Switches the LED off (inactive)
*             Enables the GPIO pin(s) as output(s)
*   Caution: Must use the defined constants: ACTIVE_HIGH or ACTIVE_LOW
***************************************************************/

void LEDS_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	//Set Port Pin to be an output with initial value '1'
	if(ACTIVE_LOW==active)
	{
		GPIO_output_set(port_addr,pin_mask);
	}
	else
	{
		GPIO_output_clear(port_addr,pin_mask);
	}
	GPIO_output_init(port_addr,pin_mask);
}

/*******************************************************************************
*    This section uses conditional execution to select the correct function
*    based on the defined constants ACTIVE_LOW or ACTIVE_HIGH
********************************************************************************/

/******* Comment out the other definitions of LEDS_on and LEDS_off to use these ****/

/**************************************************************
*   LEDS_on
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*           active selects when LED is on(active), ACTIVE_HIGH or ACTIVE_LOW
*   Output: None
**************************************************************
*   Function: Switches the selected LEDs on (active)
*          
*   Caution: Must use the defined constants: ACTIVE_HIGH or ACTIVE_LOW
*            Alternative version below eliminates the if for faster execution
***************************************************************/

void LEDS_on(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	if(ACTIVE_LOW==active)
	{
		GPIO_output_clear(port_addr,pin_mask); 
	}
	else
	{
		GPIO_output_set(port_addr,pin_mask); 
	}
}

/**************************************************************
*   LEDS_off
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select GPIO pin(s)
*           active selects when LED is on(active), ACTIVE_HIGH or ACTIVE_LOW
*   Output: None
**************************************************************
*   Function: Switches the selected LEDs off (inactive)
*
*   Caution: Must use the defined constants: ACTIVE_HIGH or ACTIVE_LOW
*            Alternative version below eliminates the if for faster execution
***************************************************************/

void LEDS_off(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
{
	if(ACTIVE_LOW==active)
	{
		GPIO_output_set(port_addr,pin_mask);
	}
	else
	{
		GPIO_output_clear(port_addr,pin_mask);
	}
}

/*******************************************************************************
*    This section uses conditional compilation to select the correct function 
*    based on the defined constant LED_ACTIVE_LOW or LED_ACTIVE_HIGH          
********************************************************************************/

/******* Comment out the previous definitions of LEDS_on and LEDS_off to use these ****/

//#define LED_ACTIVE_LOW
///**************************************************************
//*   LEDS_on
//*   Inputs: pointer to a GPIO port struct,
//*           pin_mask to select GPIO pin(s)
//*           active selects when LED is on(active), ACTIVE_HIGH or ACTIVE_LOW
//*   Output: None
//**************************************************************
//*   Function: Switches the selected LEDs on (active)
//*
//*   Caution: Must use the defined constants: ACTIVE_HIGH or ACTIVE_LOW
//*            Alternative version below eliminates the if for faster execution
//***************************************************************/
//
//void LEDS_on(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
//{
//#ifdef LED_ACTIVE_LOW
		//GPIO_output_clear(port_addr,pin_mask);
//#else
		//GPIO_output_set(port_addr,pin_mask);
//#endif
//}
//
///**************************************************************
//*   LEDS_off
//*   Inputs: pointer to a GPIO port struct,
//*           pin_mask to select GPIO pin(s)
//*           active selects when LED is on(active), ACTIVE_HIGH or ACTIVE_LOW
//*   Output: None
//**************************************************************
//*   Function: Switches the selected LEDs off (inactive)
//*
//*   Caution: Must use the defined constants: ACTIVE_HIGH or ACTIVE_LOW
//*            Alternative version below eliminates the if for faster execution
//***************************************************************/
//
//void LEDS_off(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active)
//{
//#ifdef LED_ACTIVE_LOW
     //GPIO_output_set(port_addr,pin_mask);
//#else
     //GPIO_output_clear(port_addr,pin_mask);
//#endif
//}
