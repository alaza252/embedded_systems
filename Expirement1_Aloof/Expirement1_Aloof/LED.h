/*
 * LED.h
 *
 * Created: 9/11/2023 3:59:50 PM
 *  Author: dnsdyy
 */ 


#ifndef LED_H_
#define LED_H_

#define LED0 (1<<7)



LEDS_init(volatile GPIO_t * port_addr, uint8_t led_pin);
LEDS_ON(volatile GPIO_t * port_addr, uint8_t led_pin);
LEDS_OFF(volatile GPIO_t * port_addr, uint8_t led_pin);



#endif /* LED_H_ */