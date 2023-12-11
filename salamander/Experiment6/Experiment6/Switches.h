/*
 * Switches.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: Roger Younger
 */ 


#ifndef _SWITCHES_H_
#define _SWITCHES_H_

#include "GPIO_Inputs.h"

/******** Defined Constants for read_switch return values *******/
#define SW_PRESSED (0x00)
#define SW_NOT_PRESSED (0xFF)

#define DEBOUNCE_75_MS (75)

/**************************************************************
*   switch_init
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select a GPIO pin,
*           pullup set to PU_ENABLE or PU_DISABLE
*   Output: None
**************************************************************
*   Function: Calls GPIO_input_init
*
*   Caution: Must use the defined constants PU_ENABLE, PU_DISABLE
***************************************************************/
void switch_init(volatile GPIO_t *port_addr, uint8_t pin_mask,uint8_t pullup);

/**************************************************************
*   read_switch
*   Inputs: pointer to a GPIO port struct,
*           pin_mask to select a GPIO pin,
*           debounce time in ms
*   Output: Value indicating SW_PRESSED or SW_NOT_PRESSED
**************************************************************
*   Function: Reads an input pin, if low, then it waits for
*             debounce time and reads again.
*             If low, then the switch is pressed
*
*   Caution: Assumes active low switch, not for real-time because of delay
***************************************************************/
uint8_t read_switch(volatile GPIO_t *port_addr, uint8_t pin_mask, uint8_t debounce_time);


#endif /* _GPIO_OUTPUTS_H_ */