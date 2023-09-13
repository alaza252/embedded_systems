/*
 * Experiment1.c
 *
 * Created: 9/11/2023 2:40:46 PM
 * Author : sjsmdd
 */ 


#include "board.h"
#include "leds.h"
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);
	gpio_led_init(LED1_REG, LED1_PIN_MASK);
	gpio_led_init(LED2_REG, LED2_PIN_MASK);
	gpio_led_init(LED3_REG, LED3_PIN_MASK);

	while(1)
	{
		gpio_led_on(LED0_REG, LED0_PIN_MASK);
		gpio_led_on(LED1_REG, LED1_PIN_MASK);
		gpio_led_on(LED2_REG, LED2_PIN_MASK);
		gpio_led_on(LED3_REG, LED3_PIN_MASK);
		_delay_ms(100);
		gpio_led_off(LED0_REG, LED0_PIN_MASK);
		gpio_led_off(LED1_REG, LED1_PIN_MASK);
		gpio_led_off(LED2_REG, LED2_PIN_MASK);
		gpio_led_off(LED3_REG, LED3_PIN_MASK);
		_delay_ms(400);
	}
}


