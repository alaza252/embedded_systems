/*
 * LED.h
 *
 * Created: 9/11/2023 3:59:50 PM
 *  Author: dnsdyy
 */ 


#ifndef LED_H_
#define LED_H_

#define LED0 (1<<7)
#define LED_bit (1<<7)
char LED_State_g;

#include "Board.h"


void LED_Flash_Init();
void LED_Flash_Change_State();


void LEDS_init(volatile GPIO_t * port_addr, uint8_t led_pin);
void LEDS_ON(volatile GPIO_t * port_addr, uint8_t led_pin);
void LEDS_OFF(volatile GPIO_t * port_addr, uint8_t led_pin);



#endif /* LED_H_ */