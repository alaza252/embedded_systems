/*
 * switches.h
 *
 * Created: 9/15/2023 1:43:05 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 */ 

#ifndef SWITCHES_H
#define SWITCHES_H

#include "board.h"

/*
 * void SWITCH_init( volatile GPIO_t * port_addr, uint8_t pinmask)
 * --------------------
 *  Parameters: the address to the port address, pinmask contains the pin wanting to change
 *
 *  calls GPIO_Input_init
 *
 *  returns: none
 */
void SWITCH_init(volatile GPIO_t *port_addr, uint8_t pinmask);

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
uint8_t SWITCH_press(volatile GPIO_t *port_addr, uint8_t pinmask);

#endif /* SWITCHES_H */