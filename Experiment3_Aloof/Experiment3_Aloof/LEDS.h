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

void LEDS_init(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);
void LEDS_on(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);
void LEDS_off(volatile GPIO_t * port_addr, uint8_t pin_mask, uint8_t active);



#endif /* _LEDS_H_ */