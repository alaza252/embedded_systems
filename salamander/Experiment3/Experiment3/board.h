/*
 * board.h
 *
 * Created: 9/11/2023 2:45:19 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#include <avr/io.h>

#ifndef BOARD_H
#define BOARD_H

#define F_CPU (16000000UL)
#define OSC_DIV (1)

// region GPIO
typedef struct portRegs
{
	/** Used to read the value of a pin when configured as an input */
	volatile uint8_t PIN_REG;
	/** Used to configure what pins are outputs and what pins are inputs*/
	volatile uint8_t DDR_REG;
	/** Used to control the output when a pin is in output mode, and is used to control the pullup resistor when configured for input*/
	volatile uint8_t PORT_REG;
} GpioPort_t;

#define LED0_REG ((volatile GpioPort_t *) & PINC)
#define LED0_PIN_MASK (1<<7)

#define LED1_REG ((volatile GpioPort_t *) & PINB)
#define LED1_PIN_MASK (1<<3)

#define LED2_REG ((volatile GpioPort_t *) & PINE)
#define LED2_PIN_MASK (1<<4)

#define LED3_REG ((volatile GpioPort_t *) & PINA)
#define LED3_PIN_MASK (1<<7)
// endregion

// region UART
typedef struct UART
{
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t UCSRD;
	volatile uint8_t BRRL;
	volatile uint8_t BRRH;
	volatile uint8_t UDR;
} UART_t;

#define UART0 ((volatile UART_t*) &UCSR0A)
#define UART1 ((volatile UART_t*) &UCSR1A)
#define UART2 ((volatile UART_t*) &UCSR2A)

#define ASYNC_MODE (0 << 6)
#define SYNC_MODE (1 << 6)
#define SPI_MODE (3 << 6)

#define NO_PARITY (0 << 4)
#define EVEN_PARITY (2 << 4)
#define ODD_PARITY (3 << 4)

#define ONE_STOP_BIT (0 << 3)
#define TWO_STOP_BITS (1 << 3)

#define FIVE_DATA_BITS (0 << 1)
#define SIX_DATA_BITS (1 << 1)
#define SEVEN_DATA_BITS (2 << 1)
#define EIGHT_DATA_BITS (3 << 1)
#define NINE_DATA_BITS (3 << 1)
// endregion

// region SPI

typedef struct SPI
{
	// Lecture 09 contains good information for all of these registers.
	/** SPCR - SPI Control Register.
	 *
	 * Note that two least significant bits are used to determine the SPI clock rate. SPI2X also supplements these values
	 */
	volatile uint8_t CONTROL;
	/**
	 * SPCR - SPI Status Register.
	 * Contains SPIF, WCOL, ..., SPI2X bits
	 */
	volatile uint8_t STATUS;
	/**
	 * SPDR - SPI Data Register
	 */
	volatile uint8_t DATA;
} SPI_t;



// endregion

#endif
