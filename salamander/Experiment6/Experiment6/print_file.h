/*
 * print_file.h
 *
 * Created: 11/28/2023 5:02:30 PM
 * Author : Sam Stockmann, Lavender Shannon
 */ 

#ifndef PRINT_FILE_H
#define PRINT_FILE_H

#include "board.h"
#include "directory_functions.h"

#define PRINT_FILE_READ_ERROR 1

uint8_t print_file(FatInfo *fat_info, uint32_t file_first_cluster_num, uint8_t buffer[]);


#endif
