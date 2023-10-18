/*
 * sd.h
 */

#ifndef SD_H
#define SD_H

/** Represents SD Card v1.x types */
#define SD_CARD_V1 (0)
/** Represents SD Card v2.x+ types, which have high capacity support. */
#define SD_CARD_NON_V1 (1)

typedef struct SDInfo {
	uint8_t card_type;
} SDInfo;

#define SD_INIT_ERROR_COM (1)
#define SD_INIT_ERROR_CMD0_FAIL (2)

uint8_t sd_card_init(SDInfo &sd_info);


#endif
