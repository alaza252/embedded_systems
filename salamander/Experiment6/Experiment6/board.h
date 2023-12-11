/*
 * board.h
 *
 * Created: 9/4/2020 11:57:19 AM
 *  Author: youngerr
 */ 

#include <avr/io.h>

// Microcontroller: ATMEGA324PB
#ifndef BOARD_H_
#define BOARD_H_

#define F_CPU (16000000UL)
#define OSC_DIV (1)

#define __DELAY_BACKWARD_COMPATIBLE__

/* GPIO */
typedef struct GPIO
{
	volatile uint8_t GPIO_PIN;
	volatile uint8_t GPIO_DDR;
	volatile uint8_t GPIO_PORT;
} GPIO_t;

#define PA ((volatile GPIO_t *)(&PINA))
#define PB ((volatile GPIO_t *)(&PINB))
#define PC ((volatile GPIO_t *)(&PINC))
#define PD ((volatile GPIO_t *)(&PIND))
#define PE ((volatile GPIO_t *)(&PINE))

/* LEDS */
#define LED0_PORT (PC)
#define LED0_PIN (1<<7)
/* Assumes the OLED board is connected to Connector EXT4 */
#define LED1_PORT (PB)
#define LED1_PIN (1<<3)
#define LED2_PORT (PE)
#define LED2_PIN (1<<4)
#define LED3_PORT (PA)
#define LED3_PIN (1<<7)

/* Buttons */
#define SW0_PORT (PC)
#define SW0_PIN (1<<6)
/* Assumes the OLED board is connected to Connector EXT4 */
#define SW1_PORT (PB)
#define SW1_PIN (1<<2)
#define SW2_PORT (PA)
#define SW2_PIN (1<<4)
#define SW3_PORT (PA)
#define SW3_PIN (1<<5)

/***** UARTS ******/
typedef struct UART
{
	volatile uint8_t UCSRA;
	volatile uint8_t UCSRB;
	volatile uint8_t UCSRC;
	volatile uint8_t UCSRD;
	volatile uint8_t UBBRL;
	volatile uint8_t UBBRH;
	volatile uint8_t UDR;
} UART_t;

#define UART0 ((volatile UART_t *) &UCSR0A)  //0xC0
#define UART1 ((volatile UART_t *) &UCSR1A)  //0xC8
#define UART2 ((volatile UART_t *) &UCSR2A)  //0xD0

//****** SPI definitions *****

typedef struct SPI
{
	
	volatile uint8_t SPCR;
	volatile uint8_t SPSR;
	volatile uint8_t SPDR;
} SPI_t;

#define SPI0 ((volatile SPI_t *) &SPCR0)  //0x4C
#define SPI1 ((volatile SPI_t *) &SPCR1)  //0xAC

//***** SPI Ports and Pins
#define SPI0_SCK_PORT (PB)
#define SPI0_SCK_PIN  (1<<7)
#define SPI0_MOSI_PORT (PB)
#define SPI0_MOSI_PIN (1<<5)
#define SPI0_MISO_PORT (PB)
#define SPI0_MISO_PIN (1<<6)
#define SPI0_SS_PORT (PB)
#define SPI0_SS_PIN (1<<4)
//*****
#define SPI1_SCK_PORT (PD)
#define SPI1_SCK_PIN  (1<<7)
#define SPI1_MOSI_PORT (PE)
#define SPI1_MOSI_PIN (1<<3)
#define SPI1_MISO_PORT (PE)
#define SPI1_MISO_PIN (1<<2)
#define SPI1_SS_PORT (PD)
#define SPI1_SS_PIN (1<<6)

/****** TWI ********/
typedef struct TWI
{
	volatile uint8_t TWI_TWBR;
	volatile uint8_t TWI_TWSR;
	volatile uint8_t TWI_TWAR;
	volatile uint8_t TWI_TWDR;
	volatile uint8_t TWI_TWCR;
	volatile uint8_t TWI_TWAMR;
} TWI_t;

#define TWI0 ((volatile TWI_t *)&TWBR0)  //0xB8
#define TWI1 ((volatile TWI_t *)&TWBR1)  //0xD8

#define TWI0_SCL_PORT (PC)
#define TWI0_SCL_PIN (1<<0)
#define TWI0_SDA_PORT (PC)
#define TWI0_SDA_PIN (1<<1)
#define TWI1_SCL_PORT (PE)
#define TWI1_SCL_PIN (1<<6)
#define TWI1_SDA_PORT (PE)
#define TWI1_SDA_PIN (1<<5)


#endif /* BOARD_H_ */