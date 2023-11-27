#include <stdio.h>
#include "board.h"
#include "UART.h"
//#include "File_System_struct.h"
#include "directory_functions.h"
#include "read_sector.h"
#include "uart_print.h"

/******* Private Constants *************/
#define CR (0x0D)
#define LF (0x0A) 


uint16_t  print_directory(FatInfo *fat_info, uint32_t Sector_num, uint8_t * array_in)
{ 
   uint32_t Sector, max_sectors;
   uint16_t i, entries;
   uint8_t temp8, j, attr, out_val, error_flag;
   uint8_t * values;
   char *prnt_bffr;

   prnt_bffr=export_print_buffer();
   
   values=array_in;
   entries=0;
   i=0;
   if (fat_info->FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=fat_info->RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors= fat_info->SecPerClus;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector,fat_info->BytesPerSec,values);
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
                 error_flag=read_sector(Sector,fat_info->BytesPerSec,values);
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


uint32_t read_dir_entry(FatInfo *fat_info, uint32_t Sector_num, uint16_t Entry, uint8_t * array_in)
{ 
   uint32_t Sector, max_sectors, return_clus;
   uint16_t i, entries;
   uint8_t temp8, attr, error_flag;
   uint8_t * values;

   values=array_in;
   entries=0;
   i=0;
   return_clus=0;
   if (fat_info->FATtype==FAT16)  // included for FAT16 compatibility
   { 
      max_sectors=fat_info->RootDirSecs;   // maximum sectors in a FAT16 root directory
   }
   else
   {
      max_sectors=fat_info->SecPerClus;
   }
   Sector=Sector_num;
   error_flag=read_sector(Sector,fat_info->BytesPerSec,values);
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
			    if(fat_info->FATtype==FAT32)
                {
                   return_clus=read_value_8(21+i,values);
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
                 error_flag=read_sector(Sector,fat_info->BytesPerSec,values);
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



