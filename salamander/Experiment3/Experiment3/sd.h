/*
 * Experiment3.c
 *
 * Created: 10/16/2023 3:18:32 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef SD_H
#define SD_H

#include "board.h"

/** This error is returned when an invalid cmd_val is provided. */
#define SD_ILLEGAL_CMD (1<<0)
/** An error indicating the internal communication has had an error. With the current implementation, this indicates a SPI error */
#define SD_COM_ERROR (1<<1)

// Return value is an error flag
uint8_t send_command(uint8_t cmd_val, uint32_t arg);



#endif
