/*
 * Experiment3.c
 *
 * Created: 10/16/2023 3:18:32 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef SD_H
#define SD_H

#include "board.h"

#define SD_ILLEGAL_CMD (1<<0)

// Return value is an error flag
uint8_t send_command(uint8_t cmd_val, uint32_t arg);



#endif