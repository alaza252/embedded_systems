/*
 * mount_drive.h
 *
 * Created: 11/26/2023 2:58:43 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef MOUNT_DRIVE_H
#define MOUNT_DRIVE_H

#include "directory_functions.h"

/** We define the sector size as a constant because we only support FAT32. This value may also be used to assume the  */
#define FAT_SECTOR_SIZE 512

#define MOUNT_DRIVE_BPB_NOT_FOUND 1
#define MOUNT_DRIVE_FAT_16_NOT_SUPPORTED 16
#define MOUNT_DRIVE_UNEXPECTED_FAT12_VAL 12
#define MOUNT_DRIVE_UNEXPECTED_FAT16_VAL 161U

#define FIND_NEXT_CLUST_READ_ERROR 1

/**
 * (Step 5)
 *
 * Does these steps:
 *   a) Find the BIOS Parameter Block (BPB).
 *      This is done by reading sector 0, and checking if that is the BPB or the Master Boot Record (MBR)
 *   b) Perform calculations to determine the FAT type. (We only support FAT32)
 *   c) Perform calculations to determine the starting sector number
 *
 * Parameters:
 *   fat_info: An uninitialized pointer to a FatInfo. This function will initialize it
 *   buffer: A pointer to the array that has 512 (FAT_SECTOR_SIZE) elements.
 *
 * Returns: An error code. 0 represents success. A non-zero error represents some error, defined in this file.
 */
uint8_t mount_drive(FatInfo *fat_info, uint8_t buffer[]);


/**
 * (Step 6)
 * Calculates the starting sector of a cluster
 */
uint32_t first_sect(FatInfo *fat_info, uint32_t clust_num);


/**
 * (Step 7)
 *
 *
 */
uint8_t find_next_clust(FatInfo *fat_info, uint32_t clust_num, uint8_t arr[], uint32_t *next_cluster_out);

#endif
