/*
 * Experiment1.c
 *
 * Created: 9/11/2023 2:40:46 PM
 * Author: Sam Stockmann, Lavender Shannon
 */ 


#include "board.h"
#include "leds.h"
#include "switch.h"
#include <avr/io.h>
#include <util/delay.h>

#define LED_OFF 0
#define LED_ON 1
#define LED_BLINK 2


int main(void)
{
	//init LEDs
	gpio_led_init(LED0_REG, LED0_PIN_MASK);
	gpio_led_init(LED1_REG, LED1_PIN_MASK);
	gpio_led_init(LED2_REG, LED2_PIN_MASK);
	gpio_led_init(LED3_REG, LED3_PIN_MASK);

	gpio_switch_init(SW0_REG, SW0_PIN_MASK);
	gpio_switch_init(SW1_REG, SW1_PIN_MASK);
	gpio_switch_init(SW2_REG, SW2_PIN_MASK);
	gpio_switch_init(SW3_REG, SW3_PIN_MASK);

	uint8_t old_val = 0; // previous value of switch
	uint8_t led_state = 0;

	while(1)
	{
		uint8_t current_val = read_switch(SW0_REG, SW0_PIN_MASK); // current value of switch
		
		if (current_val != 0 && current_val != old_val) // if switch is pressed and it was not pressed before
		{
			led_state = (led_state + 1) % 3; // total of 3 states, loop back to state 0 after state 2
		}
		
		old_val = current_val;
		
		switch (led_state)
		{
			case LED_OFF:
				gpio_led_off(LED0_REG, LED0_PIN_MASK);
				break;
			case LED_ON:
				gpio_led_on(LED0_REG, LED0_PIN_MASK);
				break;
			case LED_BLINK:
				gpio_led_on(LED0_REG, LED0_PIN_MASK);
				_delay_ms(100);
				gpio_led_off(LED0_REG, LED0_PIN_MASK);
				_delay_ms(400);
		}
	}
}


