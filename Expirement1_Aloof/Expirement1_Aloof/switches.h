/*
 * switches.h
 *
 * Created: 9/15/2023 1:43:05 PM
 *  Author: dnsdyy
 */ 

#ifndef SWITCHES_H
#define SWITCHES_H
#include "board.h"




void SWITCH_init(volatile GPIO_t *port_addr, uint8_t pinmask);
uint8_t SWITCH_press(volatile GPIO_t *port_addr, uint8_t pinmask);


#endif /* SWITCHES_H */