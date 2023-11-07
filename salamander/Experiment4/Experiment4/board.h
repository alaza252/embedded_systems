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


#define PA ((volatile GpioPort_t *) & PINA)
#define PB ((volatile GpioPort_t *) & PINB)
#define PC ((volatile GpioPort_t *) & PINC)
#define PD ((volatile GpioPort_t *) & PIND)
#define PE ((volatile GpioPort_t *) & PINE)

#define LED0_REG (PC)
#define LED0_PIN_MASK (1<<7)

#define LED1_REG (PB)
#define LED1_PIN_MASK (1<<3)

#define LED2_REG (PE)
#define LED2_PIN_MASK (1<<4)

#define LED3_REG (PA)
#define LED3_PIN_MASK (1<<7)
// endregion GPIO

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

// we define these UART constants here because they are specific to the board itself.
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
// endregion UART

// region SPI

typedef struct SPI
{
	// Lecture 09 contains good information for all of these registers.
	// Notice the naming of these registers. Naming these registers sensible things makes code much easier to read, even if it isn't as consistent with the documentation now.
	/** SPCR - SPI Control Register.
	 *
	 * Note that two least significant bits are used to determine the SPI clock rate. SPI2X also supplements these values
	 */
	volatile uint8_t CONTROL;
	/**
	 * SPSR - SPI Status Register.
	 * Contains SPIF, WCOL, ..., SPI2X bits
	 */
	volatile uint8_t STATUS;
	/**
	 * SPDR - SPI Data Register
	 */
	volatile uint8_t DATA;
} SPI_t;

#define SD_CS_PIN_MASK (1<<4)
#define SD_CS_REG (PB)

#define SPI0 ((volatile SPI_t*) & SPCR0)
#define SPI0_MOSI_PIN_MASK (1<<5)
#define SPI0_MOSI_REG (PB)

#define SPI0_SCK_PIN_MASK (1<<7)
#define SPI0_SCK_REG (PB)

#define SPI1 ((volatile SPI_t*) & SPCR1)
#define SPI1_MOSI_PIN_MASK (1<<3)
#define SPI1_MOSI_REG (PE)

#define SPI1_SCK_PIN_MASK (1<<7)
#define SPI1_SCK_REG (PD)



// endregion SPI


// region TWI

typedef struct TWI
{
	volatile uint8_t BAUD_RATE; // TWBR
	volatile uint8_t STATUS; // TWSR
	volatile uint8_t ADDR; // TWAR
	volatile uint8_t DATA; // TWDR
	volatile uint8_t CONTROL; // TWCR
	volatile uint8_t ADDR_MASK; // TWAMR
} TWI_t;

#define TWI0 ((volatile TWI_t *) &TWBR0)
#define TWI1 ((volatile TWI_t *) &TWBR1)

// endregion TWI

#endif
