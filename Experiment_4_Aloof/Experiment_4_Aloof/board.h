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



/*******TWI********/
typedef struct TWI
{
	volatile uint8_t Baud_rate;
	volatile uint8_t Status;
	volatile uint8_t Addr;
	volatile uint8_t Data;
	volatile uint8_t Control;
	volatile uint8_t Addr_mask;
} TWI_t;


#define TWI0 ((volatile TWI_t *) &TWBR0)
#define TWI1 ((volatile TWI_t *) &TWBR1)


#define TWBR (0)
#define	TWSR (1)
#define	TWAR (2)
#define	TWDR (3)
#define	TWCR (4)
#define	TWAMR (5)



#define TWI_RECIEVE_START_ERROR (0xFF)
#define TWI_RECIEVE_ADDRESS_ERROR (0xFE)
#define TWI_RECIEVE_DATA_ERROR (0xFD)
#define TWI_TRANSMIT_START_ERROR (0xFC)
#define TWI_TRANSMIT_ADDRESS_ERROR (0xFB)
#define TWI_TRANSMIT_DATA_ERROR (0xFA)

#define I2C_START 0x08
#define I2C_REP_START 0x10
#define I2C_MT_SLA_ACK 0x18
#define I2C_MT_DATA_ACK 0x28

#define STA013_TWI (TWI1)
#define PB1_PIN (PB)
#define PB1_MASK (1<<1)



#endif /* BOARD_H_ */