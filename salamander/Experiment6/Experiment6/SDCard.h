#ifndef _SDCard_H
#define _SDCard_H

#include "board.h"

// ------ Public Constant definitions -----------------------------

/****** SD Card IO Definitions ******/
#define SD_Card_Port (SPI0)  // Defines which SPI port is used for SD Card communication
#define SD_CS_Port (PB)  // Defines the GPIO Port for SD Card Chip Select
#define SD_CS_Pin (1<<4) // Defines the GPIO Pin for SD Card Chip Select

// error values
#define no_errors (0)
#define rr_timeout_error (0x81)
#define fe_timeout_error (0x88)
#define acmd41_timeout_error (89)
#define illegal_cmnd (0x82)
#define response_error (0x83)
#define data_error (0x84)
#define voltage_error (0x85)
#define card_inactive (0x86)
#define SPI_error (0x87)

// SD Card types
#define unknown (0xFF)
#define Ver2 (0x02)
#define Standard_Capacity (9)  // used to shift address for block to byte address conversion
#define High_Capacity (0)      // no shift when no conversion is required

// SD Card Commands 
#define CMD0 0
#define CMD8 8
#define CMD16 16
#define CMD17 17
#define CMD55 55
#define CMD58 58
#define ACMD41 41


// ------ Public function prototypes -------------------------------
/***********************************************************************
DESC:    GPIO Hardware Abstraction Functions
         Initialization, Active and Inactive 
RETURNS: nothing
CAUTION: Uses GPIO functions 
************************************************************************/
void SD_Card_CS_Init(volatile GPIO_t * port, uint8_t pin);

void SD_Card_CS_Active(volatile GPIO_t * port, uint8_t pin);

void SD_Card_CS_Inactive(volatile GPIO_t * port, uint8_t pin);

/***********************************************************************
DESC:    Sends Commands to SD card using the SPI peripheral
         cmnd=Command number to be sent
         argum=32bit argument for a command
RETURNS: error flag
CAUTION: Must be followed by a read to capture SD card response
         nCS0 must be set/cleared outside of this routine 
************************************************************************/
uint8_t Send_Command(uint8_t cmnd, uint32_t argum);

/***********************************************************************
DESC:    Read values from SPI port, 0xff sent for each byte read 
		 num_bytes=number of bytes to read
RETURNS: error flag
         *valout=address of array of bytes read
CAUTION: Used to capture SD card response
         nCS0 must be set/cleared outside of this routine 
************************************************************************/
uint8_t Receive_Response(uint8_t num, uint8_t * valout);

/**********************************************************************
DESC:    Reads a specified number of bytes from SD card,
         0xff sent for each byte read 
RETURNS: error flag
         block of data stored in array_out[]
CAUTION: Used to capture SD card response
         nCS0 must be set/cleared outside of this routine 
************************************************************************/
uint8_t Read_Block(uint16_t num, uint8_t * array_out);

/**********************************************************************
DESC:    Sends the commands needed to initialize the SD card
RETURNS: error flag
CAUTION: The SPI port for the SD Card is defined as SD_Card_Port in SDCard.h
************************************************************************/
uint8_t SD_Card_Init(void);

uint8_t Return_SD_Card_Type(void);

void Print_Error(uint8_t error);


#endif