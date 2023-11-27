#ifndef _Directory_Func_H
#define _Directory_Func_H

#include "board.h"

//------- Public Constants  -----------------------------------------
#define FAT32 (4)
#define FAT16 (2)
#define FAT32_shift (2)
#define FAT16_shift (1)
#define Disk_Error (0xF0)
#define no_errors (0)
#define more_entries (0x8000)   
#define no_entry_found (0x80000000)  // msb set to indicate error
#define directory_bit  (0x10000000)  // lsb of first nibble (bit28)

// ------ Public function prototypes -------------------------------

uint16_t print_directory(uint32_t Sector_num, uint8_t *array_in);

uint32_t read_dir_entry(uint32_t Sector_num, uint16_t Entry, uint8_t *array_in);

// ------ Export Global Values ---------------------------------

uint32_t Export_FirstDataSec();
uint32_t Export_StartofFAT();
uint32_t Export_FirstRootDirSec();
uint32_t Export_RootDirSecs();
uint16_t Export_BytesPerSec();
uint8_t Export_SecPerClus();
uint8_t Export_FATtype();

// ------ Write Global Values ---------------------------------

uint32_t Write_FirstDataSec(uint32_t input);
uint32_t Write_StartofFAT(uint32_t input);
uint32_t Write_FirstRootDirSec(uint32_t input);
uint32_t Write_RootDirSecs(uint32_t input);
uint16_t Write_BytesPerSec(uint16_t input);
uint8_t Write_SecPerClus(uint8_t input);
uint8_t Write_FATtype(uint8_t input);


// ------ Function prototypes needed (These can be defined in a separate file) -------------

//uint8 read_value_8(uint16 offset, uint8 * array_name);

//uint16 read_value_16(uint16 offset, uint8 * array_name);

//uint32 read_value_32(uint16 offset, uint8 * array_name);

//uint8 Mount_Drive(uint8 xdata * array_name);

//uint32 First_Sector (uint32 Cluster_num);

//uint32 Find_Next_Clus(uint32 Cluster_num, uint8 xdata * array_name);

//uint8 Open_File(uint32 Cluster, uint8 xdata * array_in);

#endif