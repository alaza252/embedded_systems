#ifndef _SPI_H
#define _SPI_H

#include "board.h"

//------- Public Constant definitions --------------------------------
// error values
#define init_okay (0)
#define no_errors (0)
#define SPI_Write_Collision (0xC0)
#define illegal_clockrate (0x0F)
#define illegal_port (0x0E)
#define SPI_TIMEOUT (0x80)


// ------ Public function prototypes -------------------------------
/***********************************************************************
DESC:    Sets up the SPI to master mode with the clock as close
         to the input parameter as possible.
         clock=32-bit 
RETURNS: Error Flag
CAUTION: Sets the CPHA to 0 and CPOL to 0
         Disables SS and sets master mode 
************************************************************************/
uint8_t SPI_master_init(volatile SPI_t *SPI_addr, uint32_t clock_rate);

/***********************************************************************
DESC:    Sends one byte using the SPI port and returns an error flag
          
RETURNS: An error flag for timeout or write collision

CAUTION: Waits for the SPI transfer to be complete
************************************************************************/
uint8_t SPI_transmit(volatile SPI_t *SPI_addr, uint8_t data_input);

/***********************************************************************
DESC:    Sends idle input using the SPI port and returns the received byte
          
RETURNS: received byte
         
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/
uint8_t SPI_receive(volatile SPI_t *SPI_addr, uint8_t idle_input, uint8_t * data_output);

/***********************************************************************
DESC:    Sends one byte using the SPI port and returns the received byte
          
RETURNS: SPI Error Flags | received byte
         or a timeout error
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/
uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t data_input, uint8_t * data_output);

#endif