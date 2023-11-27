#include <stdio.h>
#include "board.h"
#include "UART.h"
#include "UART_Print.h"
#include "Directory_Functions_globals.h"
#include "File_System_globals.h"
#include "Read_Sector.h"

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A)

static uint32_t FirstDataSec_g, StartofFAT_g, FirstRootDirSec_g, RootDirSecs_g;
static uint16_t BytesPerSec_g;
static uint8_t SecPerClus_g, FATtype_g, BytesPerSecShift_g,FATshift_g;

/************************************************************************
Export Global Variable Functions
Returns the Value of all the global variables
***********************************************************************/

uint32_t Export_FirstDataSec()
{
	return FirstDataSec_g;
} 

uint32_t Export_StartofFAT()
{
	return StartofFAT_g;
}

uint32_t Export_FirstRootDirSec()
{
	return FirstRootDirSec_g;
}

uint32_t Export_RootDirSecs()
{
	return RootDirSecs_g;
}

uint16_t Export_BytesPerSec()
{
	return BytesPerSec_g;
}

uint8_t Export_SecPerClus()
{
	return SecPerClus_g;
}

uint8_t Export_FATtype()
{
	return FATtype_g;
}

/************************************************************************
Write Global Variable Functions
Stores the Value of all the global variables
***********************************************************************/

uint32_t Write_FirstDataSec(uint32_t input)
{
	FirstDataSec_g=input;
	return FirstDataSec_g;
}

uint32_t Write_StartofFAT(uint32_t input)
{
	StartofFAT_g=input;
	return StartofFAT_g;
}

uint32_t Write_FirstRootDirSec(uint32_t input)
{
	FirstRootDirSec_g=input;
	return FirstRootDirSec_g;
}

uint32_t Write_RootDirSecs(uint32_t input)
{
	RootDirSecs_g=input;
	return RootDirSecs_g;
}

uint16_t Write_BytesPerSec(uint16_t input)
{
	BytesPerSec_g=input;
	return BytesPerSec_g;
}

uint8_t Write_SecPerClus(uint8_t input)
{
	SecPerClus_g=input;
	return SecPerClus_g;
}

uint8_t Write_FATtype(uint8_t input)
{
	FATtype_g=input;
	return FATtype_g;
}

/***********************************************************************
DESC: Prints all short file name entries for a given directory 
INPUT: Starting Sector of the directory and the pointer to a 
block of memory in xdata that can be used to read blocks from the SD card
RETURNS: uint16_t number of entries found in the directory
CAUTION: Supports FAT16, SD_shift must be set before using this function
************************************************************************/



uint16_t  print_directory(uint32_t Sector_num, uint8_t *array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t *values;
   char *prnt_bffr;

   prnt_bffr=export_print_buffer();
   values=array_in;
   entries=0;
   i=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector, BytesPerSec_g, values);
   if(error_flag==no_errors)
   {
     do
     {
 
	    temp8=read_value_8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read_value_8(0x0b+i,values);
		   if((attr&0x0E)==0)   // if hidden, system or Vol_ID bit is set do not print
		   {
		      entries++;
			  sprintf(prnt_bffr,"%5d. ",entries);  // print entry number with a fixed width specifier
			  UART_transmit_string(UART1,prnt_bffr,0);
		      for(j=0;j<8;j++)
			  {
			     out_val=read_value_8(i+j,values);   // print the 8 byte name
			     UART_transmit(UART1,out_val);
			  }
              if((attr&0x10)==0x10)  // indicates directory
			  {
			     for(j=8;j<11;j++)
			     {
			        out_val=read_value_8(i+j,values);
			        UART_transmit(UART1,out_val);
			     }
			     sprintf(prnt_bffr,"[DIR]\r\n");
				 UART_transmit_string(UART1,prnt_bffr,0);
			  }
			  else       // print a period and the three byte extension for a file
			  {
			     UART_transmit(UART1,0x2E);       
			     for(j=8;j<11;j++)
			     {
			        out_val=read_value_8(i+j,values);
			        UART_transmit(UART1,out_val);
			     }
			     UART_transmit(UART1,CR);
                 UART_transmit(UART1,LF);
			  }
		    }

		}
		i=i+32;  // next entry

		if(i>510)
		{
		  Sector++;
          if((Sector-Sector_num)<max_sectors)
		  {
              error_flag=read_sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=no_errors)
			    {
			      entries=0;   // no entries found indicates disk read error
				  temp8=0;     // forces a function exit
			    }
			    i=0;
		  }
		  else
		  {
			  entries=entries|more_entries;  // set msb to indicate more entries in another cluster
			  temp8=0;                       // forces a function exit
		  }
		}
       
	  }while(temp8!=0);
	}
	else
	{
	   entries=0;    // no entries found indicates disk read error
	}
    return entries;
 }


/***********************************************************************
DESC: Uses the same method as Print_Directory to locate short file names,
      but locates a specified entry and returns and cluster  
INPUT: Starting Sector of the directory, an entry number and a pointer to a 
block of memory in SRAM that can be used to read blocks from the SD card
RETURNS: uint32_t with cluster in lower 28 bits.  Bit 28 set if this is 
         a directory entry, clear for a file.  Bit 31 set for error.
CAUTION: 
************************************************************************/

uint32_t read_dir_entry(uint32_t Sector_num, uint16_t Entry, uint8_t *array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (Sector_num<FirstDataSec_g)  // included for FAT16 compatibility
   { 
      max_sectors=RootDirSecs_g;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=SecPerClus_g;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector, BytesPerSec_g, values);
   if(error_flag==no_errors)
   {
     do
     {
        temp8=read_value_8(0+i,values);  // read first byte to see if empty
        if((temp8!=0xE5)&&(temp8!=0x00))
	    {  
	       attr=read_value_8(0x0b+i,values);
		   if((attr&0x0E)==0)    // if hidden do not print
		   {
		      entries++;
              if(entries==Entry)
              {
			    if(FATtype_g==FAT32)
                {
                   return_clus=read_value_8(21+i,values);
				   return_clus&=0x0F;            // makes sure upper four bits are clear
				   return_clus=return_clus<<8;
                   return_clus|=read_value_8(20+i,values);
                   return_clus=return_clus<<8;
                }
                return_clus|=read_value_8(27+i,values);
			    return_clus=return_clus<<8;
                return_clus|=read_value_8(26+i,values);
			    attr=read_value_8(0x0b+i,values);
			    if(attr&0x10) return_clus|=directory_bit;
                temp8=0;    // forces a function exit
              }
              
		   }
        }
		i=i+32;  // next entry
		if(i>510)
		{
		   Sector++;
		   if((Sector-Sector_num)<max_sectors)
		   {
              error_flag=read_sector(Sector, BytesPerSec_g, values);
			  if(error_flag!=no_errors)
			  {
			     return_clus=no_entry_found;
                 temp8=0; 
			  }
			  i=0;
		   }
		   else
		   {
			  temp8=0;                       // forces a function exit
		   }
		}
        
	 }while(temp8!=0);
   }
   else
   {
	 return_clus=no_entry_found;
   }
   if(return_clus==0) return_clus=no_entry_found;
   return return_clus;
}





