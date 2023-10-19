/*
 * SD.h
 *
 * Created: 10/18/2023 12:49:24 PM
 *  Author: dnsdyy
 */ 


#ifndef SD_H_
#define SD_H_

#include <stdio.h>
#include "board.h"
#include "Long_Serial_In.h"
#include <stdlib.h>
#include "UART.h"
#include "GPIO_Outputs.h"
#include "SPI.h"




//step 3
uint8_t send_command(volatile SPI_t *SPI_addr, uint8_t command, uint32_t argument);

//step 4
uint8_t receive_response(volatile SPI_t *SPI_addr, uint8_t number_of_bytes, uint8_t * arrary_name);

//step 5
uint8_t sd_card_init(volatile SPI_t *SPI_addr)

#endif /* SD_H_ */