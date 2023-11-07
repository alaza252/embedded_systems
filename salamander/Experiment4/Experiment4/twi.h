/*
 * twi.h
 *
 * Created: 11/7/2023 4:18:19 PM
 * Author: Sam Stockmann, Lavender Shannon
 */

#ifndef TWI_H
#define TWI_H

#include "board.h"

#define INVALID_FREQ 1

uint8_t twi_master_init(volatile TWI_t* addr, uint32_t i2c_freq);

uint8_t twi_master_receive(volatile uint8_t * addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr);

#endif
