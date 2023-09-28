/*
 * board.h
 *
 * Created: 9/11/2023 2:45:19 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#include <avr/io.h>

#ifndef BOARD_H
#define BOARD_H

#define F_CPU (16000000UL)
#define OSC_DIV (1)

typedef struct portRegs
{
	/** Used to read the value of a pin when configured as an input */
	volatile uint8_t PIN_REG;
	/** Used to configure what pins are outputs and what pins are inputs*/
	volatile uint8_t DDR_REG;
	/** Used to control the output when a pin is in output mode, and is used to control the pullup resistor when configured for input*/
	volatile uint8_t PORT_REG;
} GpioPort_t;
	

#endif