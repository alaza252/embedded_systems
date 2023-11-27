/*
 * mount_drive.h
 *
 * Created: 11/26/2023 2:58:43 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef MOUNT_DRIVE_H
#define MOUNT_DRIVE_H

uint8_t mount_drive(void); // or uint8_t mount_drive(uint8_t * arr);

uint32_t first_sect(uint32_t clust_num);

uint32_t find_next_clust(uint32_t clust_num, uint8_t arr[]);

#endif