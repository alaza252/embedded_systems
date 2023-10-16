/*
 * Experiment3.c
 *
 * Created: 10/16/2023 2:10:02 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef SPI_H
#define SPI_H

#include "board.h"

uint8_t spi_master_init(volatile SPI_t *addr, uint32_t clock_rate);

// Return val used for error flags
uint8_t spi_transfer(volatile SPI_t *addr, uint8_t send_val, uint8_t *rcv_val);

uint8_t spi_transmit(volatile SPI_t *addr, uint8_t send_val);

#endif
