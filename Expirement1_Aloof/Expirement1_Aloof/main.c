/*
 * Expirement1_Aloof.c
 *
 * Created: 9/11/2023 2:37:32 PM
 * Authors: Drew Schulte and Alisa Lazareva
 
 
	Our task for this assignment was to get the board to start off not flashing
		and once pressed rotate through 2 other states with every button press.
		those states in order go Off->On->Flashing and then back to off.
		
	Our main.c includes all of the other files in this project in order to 
		utilize the function we built in them. This allows for more compact
		and focused files which is easier to read and debug.
		
	We have also included the delay library in order to use the
		_delay_ms() function
	
 */ 

#include <avr/io.h>
#include "board.h"
#include "GPIO_inputs.h"
#include "GPIO_outputs.h"
#include "switches.h"
#include <util/delay.h>


int main(void)
{
	//Initializing LEDs & Switches
	GPIO_Output_init(LED0,LED0_MASK);
	SWITCH_init(SW0,SW0_MASK);
			
	volatile uint8_t numpressed;	//represents amount of button pressed (mode #)
	numpressed = 0U;				//initialize our variable to mode 0
	
	GPIO_Output_set(LED0,LED0_MASK);		//Setting our LED to OFF
	
    while(1)
	{
		if (SWITCH_press(SW0,SW0_MASK) == 1U)	//if pressed we change modes	
		{
			numpressed++;						//increment counter to indicate button press (mode #)
			switch (numpressed)					//enter the mode switch case
			{
				case 0U:						//button pressed 0 times (LED off)
					GPIO_Output_set(LED0,LED0_MASK);
					break;
				case 1U:						//button pressed 1 time (LED on)
					GPIO_Output_clear(LED0,LED0_MASK);
					break;
				case 2U:						//button pressed 2 times (flashing mode)
					while(1)
					{
						GPIO_Output_clear(LED0,LED0_MASK);	//LED on
						_delay_ms(100);						//wait 0.1s
						GPIO_Output_set(LED0,LED0_MASK);	//LED off
						_delay_ms(400);						//wait 0.4s
						if (SWITCH_press(SW0,SW0_MASK) == 1U) //if a button is pressed, stop flashing
						{		
							break;
						}
					}
					numpressed=0U;	//reset amount of times pressed
					GPIO_Output_set(LED0,LED0_MASK);	//LED off
					break;
				default:								//default mode is off
					GPIO_Output_set(LED0,LED0_MASK);
					break;
			}
		}
	}
}