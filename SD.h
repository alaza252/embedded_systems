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
#include <stdlib.h>
#include "UART.h"
#include "GPIO_Outputs.h"
#include "SPI.h"


uint8_t get_crc(uint8_t message[], uint8_t length);

//step 3
uint8_t send_command(uint8_t command, uint32_t argument);

//step 4
uint8_t receive_response( uint8_t number_of_bytes, uint8_t * arrary_name);

uint8_t receive_R1_resonse(uint8_t *r1_response);
//step 5
uint8_t sd_card_init();

//step 7
uint8_t read_block(uint16_t number_of_bytes, uint8_t *array);

#endif /* SD_H_ */