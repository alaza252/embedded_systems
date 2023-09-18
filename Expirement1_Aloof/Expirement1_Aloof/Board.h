/*
 * Board.h
 *
 * Created: 9/11/2023 3:19:09 PM
 *  Authors: Drew Schulte and Alisa Lazareva
 
 
	This is our board.h file. This file contains the necessary information for our hardware. 
	It includes the board specific pins and CPU frequency
	It also has information for the other LEDs and switches in case they were needed for following projects
 */ 


#include <avr/io.h>

#ifndef BOARD_H_
#define BOARD_H_


#define F_CPU (16000000UL)
#define F_DIV (1)

typedef struct GPIO
{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

#define LED0 (( volatile GPIO_t *) & PINC)
#define LED0_MASK (1<<7)
#define SW0  (( volatile GPIO_t *) & PINC)
#define SW0_MASK  (1<<6)

/*#define PA (( volatile GPIO_t *) & PINA)
#define PB (( volatile GPIO_t *) & PINB)
#define PD (( volatile GPIO_t *) & PIND)
#define PE (( volatile GPIO_t *) & PINE)

#define PA_MASK (1<<7)
#define PB_MASK (1<<3)

//#define PD_MASK (1<<????)
#define PE_MASK (1<<4)*/

#endif /* BOARD_H_ */