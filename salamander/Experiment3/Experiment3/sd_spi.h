/*
 * sd_spi.h
 *
 * Created: 10/16/2023 3:18:32 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef SD_SPI_H
#define SD_SPI_H

#include "board.h"

// This file contains functions to interface with the SD Card's SPI communication. These functions should not be used directly.

#define SD_SPI_PORT SPI0

#define SD_R1_ERROR_MASK (0b01111100)

// Note the use of error codes here rather than error flags.
/** When returned by send_command, this indicates an argument error. When returned by receive_response, this indicates the R1 response is indicating an illegal command. */
#define SD_ILLEGAL_CMD (1)
/**
 * An error indicating the internal communication has had an error. With the current implementation, this indicates a SPI error.
 * When this occurs, the implementation is not guaranteed to get the SD card back into a good state for you to use it again.
 * If this error occurs, you should perform SD card initialization again.
 */
#define SD_COM_ERROR (2)

#define SD_RECEIVE_TIMEOUT (32)
/** This error indicates that R1 was read into receive_array[0], but not subsequent bytes were read because there was an error in R1*/
#define SD_RECEIVE_R1_ERROR (33)
#define SD_READ_DATA_TOKEN_ERROR (45)

/**
 * Return value is an error code (not an error flag). A value of 0 represents no error.
 */
uint8_t send_command(uint8_t cmd_val, uint32_t arg);

/**
 * num_bytes: The number of bytes to receive. This must be greater than 0. This counts the R1 response as a byte too.
 * receive_array: An array which should have num_bytes allocated to it
 *
 * Return value is an error code. A value of 0 represents all bytes being successfully received.
 */
uint8_t receive_response(uint8_t num_bytes, uint8_t receive_array[]);


/**
 * Similar to receive_response, but this function expects to be reading large amounts of data, rather than a couple
 *
 * num_bytes: number of bytes to read after the data start token (this does not include the R1 byte!)
 */
uint8_t read_block(uint16_t num_bytes, uint8_t data_arr[]);


void cs_pin_init();

void cs_pin_set(uint8_t pin_val);


#endif
