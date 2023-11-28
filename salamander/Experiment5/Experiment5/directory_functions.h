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


//----------------- Typedefs ----------------------------------------
typedef struct FatInfo
{
  uint8_t SecPerClus;
  uint8_t FATtype;
  // removed shift stuff because I don't care about efficiency
  uint16_t BytesPerSec;
  uint32_t FirstRootDirSec;
  uint32_t FirstDataSec;
  uint32_t StartofFAT;
  uint32_t RootDirSecs;
} FatInfo; // FS_values_t

// ------ Public function prototypes -------------------------------
/***********************************************************************
DESC: Prints all short file name entries for a given directory
INPUT: Starting Sector of the directory and the pointer to a
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/
uint16_t print_directory(FatInfo *fat_info, uint32_t Sector_num, uint8_t * array_in);

/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster
INPUT: Starting Sector of the directory, an entry number and a pointer to a
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION:
************************************************************************/
uint32_t read_dir_entry(FatInfo *fat_info, uint32_t Sector_num, uint16_t Entry, uint8_t * array_in);



#endif
