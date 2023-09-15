/*
 * Expirement1_Aloof.c
 *
 * Created: 9/11/2023 2:37:32 PM
 * Author : dnsdyy
 */ 

#include <avr/io.h>
#include "board.h"
//#include "GPIO_inputs.h"
//#include "GPIO_outputs.h"
#include <util/delay.h>


int main(void)
{
	LEDS_init(LED0,LED0_MASK);//Initialize our LED as off
	SWITCH_init(SW0,SW0_MASK);//Initialize our switch
	volatile uint8_t numpressed;
	numpressed = 0U;
	LEDS_OFF(LED0,LED0_MASK);
    while(1)
	{
		if (SWITCH_press(SW0,SW0_MASK) == 1U)
		{
			numpressed++;
			switch (numpressed)
			{
				case 0U:
					LEDS_OFF(LED0,LED0_MASK);
					break;
				case 1U:
					LEDS_ON(LED0,LED0_MASK);
					break;
				case 2U:
					while(1)
					{
						LEDS_ON(LED0,LED0_MASK);
						_delay_ms(100);
						LEDS_OFF(LED0,LED0_MASK);
						_delay_ms(400);
						if (SWITCH_press(SW0,SW0_MASK) == 1U)
						{
							break;
						}
					}
					numpressed=0U;
					LEDS_OFF(LED0,LED0_MASK);
					break;
				default:
					LEDS_OFF(LED0,LED0_MASK);
					break;
			}
		}
		
	}
}



/*LED_Flash_Init();//Turns it on
		LED_Flash_Change_State();//turns it of
		_delay_ms(5000);//Wait for button press
		LED_Flash_Change_State();//Turns it on
		_delay_ms(5000);//Simulate that its just on
		//Flashing
		while(1)
		{
			LED_Flash_Change_State();
			_delay_ms(500);
			//Button press would exit this loop
		}*/