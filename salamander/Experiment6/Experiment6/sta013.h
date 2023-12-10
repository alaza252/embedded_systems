/*
 * sta013.h
 *
 * Created: 11/7/2023 8:11:25 PM
 * Author: Sam Stockmann, Lavender Shannon
 */


#ifndef STA013_H
#define STA013_H

#include "board.h"

/** Indicates an internal TWI error. If a future implementation uses something besides TWI/I2C, this error code may be used to indicate that internal error as well*/
#define STA013_INIT_TWI_ERROR 1
/** Indicates one of the internal checks has failed during initialization */
#define STA013_INIT_CHECK_FAILED_ERROR 2

uint8_t sta013_init();

void sta013_reset_pin_set(uint8_t bit);

uint8_t sta013_read_data_req();

#endif
