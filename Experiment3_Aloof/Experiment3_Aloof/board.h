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
#define F_DIV (1)

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

#define UART0 ((volatile UART_t *) 0xC0)
#define UART1 ((volatile UART_t *) 0xC8)
#define UART2 ((volatile UART_t *) 0xD0)

#define UDRE_bit (1<<5)
#define RXC_bit (1<<7)

#define asynch_mode (0<<6)
#define synch_mode (1<<6)
#define SPI_mode (3<<6)

#define no_parity (0<<4)
#define even_parity (2<<4)
#define odd_parity (3<<4)

#define one_stop_bit (0<<3)
#define two_stop_bit (1<<3)

#define U2X_bit (0)

#define transmit_enable (1<<3)
#define receive_enable (1<<4)

#define Eight_Bit_Data (3<<1)




/* SPI */
typedef struct SPI{
	volatile uint8_t SPCR;  //control register
	volatile uint8_t SPSR;  //status register
	volatile uint8_t SPDR;  //Data register
}SPI_t;


/***** SPI ******/
#define SPI0 ((volatile uint8_t *) 0x4C)
#define SPI0_PORT (PB)
#define SPI0_MOSI (1<<5)//needs to be set as an output with an initial value of 1
#define SPI0_MISO (1<<6)
#define SPI0_SCK (1<<7)

#define SPI1 ((volatile uint8_t *) 0xAC)
#define SPI1_PORTE (PE)
#define SPI1_PORTD (PD)
#define SPI1_MOSI (1<<3)//needs to be set as an output with an initial value of 1
#define SPI1_MISO (1<<2)
#define SPI1_SCK (1<<6)

#define CPOL_OFFSET	(1<<3)//bits should be set to 0
#define CPHA_OFFSET	(1<<2)// bits should be set to 0
#define SPI_ENABLE	(1<<7)


/***** SD ******/
#define SD_CARD_PORT
#define ERROR_CODE (0xFF)
#define SD_CS_PORT (PB)
#define CHIP_SELECT (1<<4)
#define SD_CS_PIN  (1<<4)

#define CMD0 (0U)
#define CMD0_ARG (0x00000000)
#define CMD8 (8U)
#define CMD8_ARG (0x000001AA)
#define CMD58 (58U)
#define CMD58_ARG (0x00000000)


#endif /* BOARD_H_ */