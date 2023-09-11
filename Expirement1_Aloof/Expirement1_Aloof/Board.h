/*
 * Board.h
 *
 * Created: 9/11/2023 3:19:09 PM
 *  Author: dnsdyy
 */ 


#ifndef BOARD_H_
#define BOARD_H_
#include <avr/io.h>

#define F_CPU (16000000UL)
#define F_DIV (1)

typedef struct GPIO
{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;


#define PA(( voltatile GPIO_t *) &PINA
#define PB(( voltatile GPIO_t *) &PINB
#define PC(( voltatile GPIO_t *) &PINC
#define PD(( voltatile GPIO_t *) &PIND
#define PE(( voltatile GPIO_t *) &PINE




#endif /* BOARD_H_ */