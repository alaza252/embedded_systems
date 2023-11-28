/*
 * SD_Read_Sector.c
 *
 * Created: 11/28/2023 9:56:14 AM
 *  Author: dnsdyy
 */ 

#include "SDCard.h"
#include "SD_Read_Sector.h"
#include "Directory_Functions_struct.h"

uint8_t read_sector(uint32_t sector_number, uint16_t sector_size, uint8_t * array_for_data)
{
	uint8_t SDtype,error_flag=No_Disk_Error;
	SDtype=Return_SD_Card_Type(); // HC=0, no change to sector number
	// SC=9, multiplies sector number by 512 to convert to byte addr.
	SD_Card_CS_Active(SD_CS_Port, SD_CS_Pin); //nCS0=0;
	error_flag=Send_Command(17,(sector_number<<SDtype));
	if(error_flag==no_errors)
	{	
		error_flag=Read_Block(sector_size,array_for_data);
	}
	SD_Card_CS_Inactive(SD_CS_Port, SD_CS_Pin); // nCS0=1;
	if(error_flag!=no_errors)
	{
		error_flag=Disk_Error;
	}
	return error_flag;
}


//DONE
uint8_t read_value_8 (uint16_t offset, uint8_t array_name[])
{
	//Little Endian
	uint32_t return_val;
	return_val=*(array_name+offset);
	return return_val;
}


//DONE
uint16_t read_value_16 (uint16_t offset, uint8_t array_name[])
{
	//Little Endian
	uint32_t return_val;
	uint8_t temp;
	return_val=*(array_name+offset);
	return_val=return_val<<8;
	temp= *(array_name+offset+0x01);
	return_val|=temp;
	return return_val;
}


//DONE
uint32_t read_value_32 (uint16_t offset, uint8_t array_name[])
{
	//Little Endian
	uint32_t return_val;
	uint8_t temp, i;
	return_val=0;
	for(i=0;i<4;i++)
	{
		temp=*(array_name+offset+(3-i));
		return_val=return_val<<8;
		return_val|=temp;
	}
	return return_val;
}


/* BIG ENDIAN
uint32_t read32(uint16_t offset, uint8_t array_name[])
{
	uint32_t return_val;
	uint8_t temp, i;
	return_val=0;
	for(i=0;i<4;i++)
	{
		temp=*(array_name+offset+i);
		return_val=return_val<<8;
		return_val|=temp;
	}
	return return_val;
}*/

//DONE, please make sure that BPB is the right value, and that all the values are being read right
uint8_t mount_drive(uint8_t array_name[])
{
	//Step 1: Find the BIOS Parameter Block (BPB)
	read_sector(0x00,0x08, *array_name);
	BPB = read_value_8(0x00,*array_name);
	if(BPB != 0xE8 || BPB != 0xE9)
	{
		BPB = read_value_32(0x01C6,*array_name);
		
		if(BPB&0x00FF != 0xE8 || BPB&0x00FF != 0xE9)
		{
			return Disk_Error;
		}
	}
	uint16_t RootEntCnt = read_value_16(RootEntryCountOFFSET, BPB);
	uint16_t BytsPerSec = read_value_16(BytesPerSectorOFFSET, BPB);
	uint16_t FATSize16 = read_value_16(FATsize16OFFSET, BPB);
	uint32_t FATSize32 = read_value_32(FATSize32OFFSET, BPB);
	
	
	//Step 2: Determine how many sectors are in the Root Dir.
	RootDirSectors =(((RootEntCnt * 32) + BytsPerSec - 1)) / BytsPerSec;
	
	
	//Step 3: Determine how many sectors are data sectors
	if(FATSize16 != 0)
	{
		FATsz = FATSize16;
	}
	else
	{
		FATsz = FATSize32;
	}
	uint16_t TotalSectors16 = read_value_16(TotalSectors16OFFSET, BPB);
	uint16_t TotalSectors32 = read_value_16(TotalSectors16OFFSET, BPB);

	if(TotalSectors16 != 0)
	{
		TotalSectors = TotalSectors16;
	}
	else
	{
		TotalSectors = TotalSectors32;
	}
	uint16_t RsvdSectorCount = read_value_16(RsvdSectorCountOFFSET, BPB);
	NumFats = read_value_16(NumberFATsOFFSET, BPB);
	DataSec = TotalSectors - (RsvdSectorCount + (NumFats * FATsz) + RootDirSectors);
		
		
	//Step 4: Determine the count of clusters and FAT type
	uint16_t SectorsPerCluster = read_value_16(SectorsPerClusterOFFSET, BPB);
	CountOfClusters = DataSec / SectorsPerCluster;
	
	
	//Step 5: Determine the first sector of the file allocation table
	ThisFATSecNum = RsvdSectorCount + (FATSize32OFFSET / BytsPerSec);
	
	//Step 6: Determine the first sector of the data area
	FirstDataSector = RsvdSectorCount + (NumFats * FATsz) + RootDirSectors;
	
	
	//Step 7: Determine the first sector of the Root Directory (FAT32)
	uint16_t RootCluster = read_value_16(RootClusterOFFSET,BPB); 
	FirstSectorOfCluster = ((RootCluster-2) * SectorsPerCluster) + FirstDataSector;
	
	
	return 0;
}