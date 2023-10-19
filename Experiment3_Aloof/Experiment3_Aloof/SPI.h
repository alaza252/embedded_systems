/*
 * SPI.h
 *
 * Created: 10/16/2023 2:18:33 PM
 *  Author: dnsdyy
 */ 


#ifndef SPI_H_
#define SPI_H_


#include <stdio.h>
#include "board.h"
#include "Long_Serial_In.h"
#include <stdlib.h>
#include "UART.h"
#include "GPIO_Outputs.h"


uint8_t SPI_master_init(volatile SPI_t *SPI_addr, uint32_t clock_freq);
uint8_t SPI_transmit(volatile SPI_t *SPI_addr, uint8_t send_value);
uint8_t SPI_receive(volatile SPI_t *SPI_addr,uint8_t send_value,uint8_t *rcv_value);
uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value);

#endif /* SPI_H_ */