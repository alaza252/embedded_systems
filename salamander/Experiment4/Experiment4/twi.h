/*
 * twi.h
 *
 * Created: 11/7/2023 4:18:19 PM
 * Author: Sam Stockmann, Lavender Shannon
 */

#ifndef TWI_H
#define TWI_H

#include "board.h"

#define TWI_INIT_INVALID_FREQ 1
#define TWI_INVALID_DEV_ADDR 2
#define TWI_UNEXPECTED_STATUS 3


uint8_t twi_master_init(volatile TWI_t* addr, uint32_t i2c_freq);

/**
 * This function reads from the TWI device
 *
 * @param arr Values are written to this as they are received
 */
uint8_t twi_master_receive(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr);

/**
 * This function writes to the TWI device
 *
 * @param arr An array of size num_bytes that will be read by this function and transmitted out
 */
uint8_t twi_master_transmit(volatile TWI_t* addr, uint8_t dev_addr, uint16_t num_bytes, uint8_t *arr);

#endif
