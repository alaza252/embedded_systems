/*
 * SD_Read_Sector.c
 *
 * Created: 11/28/2023 9:56:14 AM
 *  Author: dnsdyy
 */ 

#include "SDCard.h"
#include "SD_Read_Sector.h"
#include "Directory_Functions_struct.h"
#include "print_memory.h"
#include "UART_Print.h"
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
	uint8_t temp= array_name[offset];
	return temp;
}


//DONE
uint16_t read_value_16 (uint16_t offset, uint8_t array_name[])
{
	//Little Endian
	uint16_t return_val=0;
	for( uint16_t i = offset +1; i>= offset; i--)
	{
		return_val <<=8;
		return_val |= array_name[i];
	}
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
uint8_t mount_drive(FS_values_t *FAT_INFO,uint8_t array_name[])
{
	
	uint32_t MBR;
	uint32_t temp;
	read_sector(0x00,512, array_name);
	BPB = read_value_8(0,array_name);
	if(BPB != 0xE8 && BPB != 0xE9)//it is most likely the MBR. if it is then use read 32 at offset 0x01C6
	{
		MBR = read_value_32(0x01C6,array_name);
		read_sector(MBR,512,array_name);
		temp = read_value_8(0,array_name);
		if(temp != 0xEB && temp!=0xE9)
		{
			return Disk_Error;
		}
	}
	else
	{
		MBR=0;
	}
	//the array contains BRB now
	
	
	uint16_t BytsPerSec = read_value_16(BytesPerSectorOFFSET, array_name);
	
	
	
	uint16_t FATSize16 = read_value_16(FATsize16OFFSET, array_name);
	uint32_t FATSize32 = read_value_32(FATSize32OFFSET, array_name);
	FATsz=FATSize32;
	
	if(FATSize16!= 0)	//this code only supports size 32
		return FATSIZE16ERROR;
		
	FAT_INFO->FATtype = FAT32;
	FAT_INFO->BytesPerSec=read_value_16(0x0B,array_name);
	FAT_INFO->SecPerClus=read_value_8(0x0D,array_name);
	
	
	
	//Step 1: Determine how many sectors are in the Root Dir.
	RootDirSectors =0;//on 32 it should always be 0
	FAT_INFO->RootDirSecs=0;
	
	
	//Step 2: Determine how many sectors are data sectors
	
	uint16_t TotalSectors16 = read_value_16(0x13, array_name);
	uint16_t TotalSectors32 = read_value_32(0x20, array_name);
	

	
	


	if(TotalSectors16 != 0)
	{
		TotalSectors = TotalSectors16;
	}
	else
	{
		TotalSectors = TotalSectors32;
	}
	
	
	uint16_t RsvdSectorCount = read_value_16(0x0E, array_name);
	NumFats = read_value_8(0x10, array_name);
	DataSec = TotalSectors - (RsvdSectorCount + (NumFats * FATsz) + RootDirSectors);
		
	
	
		
	//Step 3: Determine the count of clusters and FAT type
	uint16_t SectorsPerCluster = read_value_8(SectorsPerClusterOFFSET, array_name);
	CountOfClusters = DataSec / SectorsPerCluster;
	if(CountOfClusters<65525)//check to make sure that our cluster count is correct
		return FATVAL16OR12;
	
	
	//Step 4: Determine the first sector of the file allocation table
	ThisFATSecNum = RsvdSectorCount + MBR;
	FAT_INFO->StartofFAT=ThisFATSecNum;
	
	//Step 5: Determine the first sector of the data area
	FirstDataSector = RsvdSectorCount + (NumFats * FATSize32) + RootDirSectors + MBR;
	FAT_INFO->FirstDataSec=FirstDataSector;
	
	//Step 6: Determine the first sector of the Root Directory (FAT32)
	uint16_t RootCluster = read_value_32(RootClusterOFFSET,array_name); 
	FirstSectorOfCluster = ((RootCluster-2) * SectorsPerCluster) + FirstDataSector;
	FAT_INFO->FirstRootDirSec=FirstSectorOfCluster;
	
	return 0;
}


uint32_t first_sect(FS_values_t *FAT_INFO,uint32_t clust_num){
	if(clust_num==0)
		return FAT_INFO->FirstRootDirSec;
	return(((clust_num -2)*(FAT_INFO->SecPerClus))+(FAT_INFO->FirstDataSec));
}