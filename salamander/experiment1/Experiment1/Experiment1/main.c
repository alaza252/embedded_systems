/*
 * Experiment1.c
 *
 * Created: 9/11/2023 2:40:46 PM
 * Author : sjsmdd
 */ 

#include <avr/io.h>
#include "board.h"
#include "util/delay.h"

void ledInit(void);
void ledOn(void);
void ledOff(void);

int main(void)
{
	//init LED
	ledInit();

	while(1)
	{
		ledOn();
		_delay_ms(500);
		ledOff();
		_delay_ms(500);
	}
}


void ledInit(void)
{
	LED0_REG->DDR_REG  |= LED0_PIN_MASK;
	ledOff();
}

void ledOn(void)
{
	LED0_REG->PORT_REG |= LED0_PIN_MASK;
}

void ledOff(void)
{
	LED0_REG->PORT_REG &= ~LED0_PIN_MASK;
}

