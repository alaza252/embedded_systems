/*
 * board.h
 *
 * Created: 9/11/2023 2:45:19 PM
 *  Author: sjsmdd
 */


#include <avr/io.h>

#ifndef BOARD_H
#define BOARD_H

#define F_CPU (16000000UL)
#define F_DIV (1)

typedef struct portRegs
{
	volatile uint8_t PIN_REG;
	volatile uint8_t DDR_REG;
	volatile uint8_t PORT_REG;
	} GpioPort_t;
	

#endif