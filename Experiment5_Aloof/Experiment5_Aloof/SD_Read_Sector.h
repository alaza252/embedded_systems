/*
 * SD_Read_Sector.h
 *
 * Created: 11/28/2023 9:56:25 AM
 *  Author: dnsdyy
 */ 


#ifndef SD_READ_SECTOR_H_
#define SD_READ_SECTOR_H_
#include "Directory_Functions_struct.h"

//Error Values
#define No_Disk_Error (0)


//Variables
volatile uint16_t BytesPerSecond;
volatile uint8_t		SectorsPerCluster;
volatile uint32_t	RootCluster;
volatile uint32_t FATsz;
volatile uint16_t FirstDataSector;
volatile uint16_t ThisFATSecNum;
volatile uint32_t FirstSectorOfCluster;
volatile uint32_t CountOfClusters;
volatile uint8_t NumFats;
volatile uint32_t TotalSectors;
volatile uint32_t DataSec;
volatile uint16_t RootDirSectors;
volatile uint8_t BPB;
#define  FATSIZE16ERROR (0xBF)
#define  ROOTDIRSECTORSERROR (0xBE)
#define	 FATVAL16OR12 (0xBD)


uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data);
uint8_t read_value_8 (uint16_t offset, uint8_t array_name[]);
uint16_t read_value_16 (uint16_t offset, uint8_t array_name[]);
uint32_t read_value_32 (uint16_t offset, uint8_t array_name[]);
uint8_t mount_drive(FS_values_t *FAT_INFO,uint8_t array_name[]);
uint32_t first_sect(FS_values_t *FAT_INFO,uint32_t clust_num);



#endif /* SD_READ_SECTOR_H_ */