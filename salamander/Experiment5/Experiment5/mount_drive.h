/*
 * mount_drive.h
 *
 * Created: 11/26/2023 2:58:43 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef MOUNT_DRIVE_H
#define MOUNT_DRIVE_H

/**
 * (Step 5)
 *
 * Does these steps:
 *   a) Find the BIOS Parameter Block (BPB).
 *      This is done by reading sector 0, and checking if that is the BPB or the Master Boot Record (MBR)
 *   b) Perform calculations to determine the FAT type. (We only support FAT32)
 *   c) Perform calculations to determine the starting sector number
 *
 * Returns: An error code. 0 represents success. A non-zero error represents some error, defined in this file.
 */
uint8_t mount_drive(void); // or uint8_t mount_drive(uint8_t * arr);


/**
 * (Step 6)
 * Calculates the starting sector of a cluster
 */
uint32_t first_sect(uint32_t clust_num);


/**
 * (Step 7)
 *
 *
 */
uint32_t find_next_clust(uint32_t clust_num, uint8_t arr[]);

#endif
