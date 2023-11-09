/*
 * TWI.h
 *
 * Created: 11/8/2023 5:17:23 PM
 *  Author: dnsdyy
 */ 


#ifndef TWI_H
#define TWI_H

#include "board.h"

#define TWI_INIT_INVALID_FREQ 1
#define TWI_INVALID_DEV_ADDR 2
#define TWI_UNEXPECTED_STATUS 3



uint8_t twi_master_init(volatile TWI_t* addr, uint32_t i2c_freq);

uint8_t twi_master_receive(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr);

uint8_t TWI_master_transmit(volatile TWI_t* addr, uint8_t device_addr, uint32_t int_addr, uint8_t int_addr_sz, uint16_t num_bytes, uint8_t *array_name);
#endif