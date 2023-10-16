/*
 * uart.h
 *
 * Created: 10/15/2023
 *  Author: Lavender Shannon, Sam Stockmann
 */

#ifndef SPI_H
#define SPI_H

#include "board.h"

uint8_t spi_master_init(volatile SPI_t *SPI_addr, uint32_t clock_rate);

#endif
