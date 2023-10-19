/*
 * sd.h
 *
 * Created: 10/17/2023 5:32:56 PM
 * Author : Sam Stockmann, Lavender Shannon
 */

#ifndef SD_H
#define SD_H

#include "board.h"

/** Represents SD Card v1.x types */
#define SD_CARD_V1 (0)
/** Represents SD Card v2.x+ types, which have high capacity support. */
#define SD_CARD_NON_V1 (1)

#define SD_CARD_STANDARD_CAPACITY (0)
#define SD_CARD_HIGH_CAPACITY (1)

typedef struct SDInfo {
	uint8_t card_type;
	uint8_t capacity;
} SDInfo;

#define SD_INIT_ERROR_COM (1)
#define SD_INIT_R1_ERROR (2)
#define SD_INIT_VOLTAGE_ERROR (3)
#define SD_INIT_R7_ECHO_ERROR (4)
/** Although we have some logic for checking and storing card type, we are not implementing V1 support at this time. */
#define SD_INIT_V1_NOT_SUPPORTED (5)
#define SD_INIT_TIMEOUT (6)
#define SD_INIT_NOT_POWERED_UP (7)


uint8_t sd_card_init(SDInfo *sd_info);

uint8_t read_block(uint16_t num_bytes, uint8_t data_arr[]);

#endif
