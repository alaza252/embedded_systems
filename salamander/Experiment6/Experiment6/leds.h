/*
 * LEDS.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef _LEDS_H_
#define _LEDS_H_

#define ACTIVE_LOW (0)
#define ACTIVE_HIGH (1)

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

void LEDS_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);

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
***************************************************************/
void LEDS_on(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);

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
***************************************************************/
void LEDS_off(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);



#endif /* _LEDS_H_ */