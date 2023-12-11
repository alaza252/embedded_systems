#include <stdio.h>
#include "board.h"
#include "SPI.h"
#include "File_System_struct.h"
#include "Directory_Functions_struct.h"
#include "Read_Sector.h"
#include "GPIO_Outputs.h"
#include "LEDS.h"
#include "GPIO_Inputs.h"
#include "STA013_Config.h"
#include "UART_Print.h"
#include "Play_Song.h"
extern uint8_t buffer1_g[512];
extern uint8_t buffer2_g[512]; 

void Play_Song(uint32_t Start_Cluster)
{
   uint16_t index1, index2;
   uint8_t buf_flag1, buf_flag2, temp8;
   uint32_t sector, sector_offset;
   char *prnt_bffr;
   
   uint32_t current_cluster = Start_Cluster;
   uint8_t sectors_per_cluster = export_drive_values()->SecPerClus;
   
   prnt_bffr=export_print_buffer();
   //**** LEDS initialized for debugging ****//
   LEDS_off(LED3_PORT,LED3_PIN,ACTIVE_LOW);
   LEDS_init(LED3_PORT,LED3_PIN,ACTIVE_LOW);
   LEDS_off(LED1_PORT,LED1_PIN,ACTIVE_LOW);
   LEDS_init(LED1_PORT,LED1_PIN,ACTIVE_LOW);
   LEDS_off(LED0_PORT,LED0_PIN,ACTIVE_LOW);
   LEDS_init(LED0_PORT,LED0_PIN,ACTIVE_LOW);
   LEDS_off(LED2_PORT,LED2_PIN,ACTIVE_LOW);
   LEDS_init(LED2_PORT,LED2_PIN,ACTIVE_LOW);
   GPIO_output_clear(STA013_BITEN_Port,STA013_BITEN_Pin);
   GPIO_output_init(STA013_BITEN_Port,STA013_BITEN_Pin);
   GPIO_input_init(STA013_DATAREQ_Port,STA013_DATAREQ_Pin,PU_ENABLED);
   sprintf(prnt_bffr,"Starting Cluster = %lu\n\r",Start_Cluster);
   UART_transmit_string(UART1,prnt_bffr,0);
   sector=first_sector(current_cluster);
   sprintf(prnt_bffr,"Starting Sector = %lu\n\r",sector);
   UART_transmit_string(UART1,prnt_bffr,0);
   sector_offset=0;
   buf_flag1=1;
   buf_flag2=0;
   //P3_2=ON;
   LEDS_on(LED1_PORT,LED1_PIN,ACTIVE_LOW);
   index1=0;
   
   read_sector(sector+sector_offset, 512, buffer1_g);

   sector_offset++;
   LEDS_off(LED1_PORT,LED1_PIN,ACTIVE_LOW);
   LEDS_on(LED2_PORT,LED2_PIN,ACTIVE_LOW);
   index2=0;
   
   read_sector(sector+sector_offset, 512, buffer2_g);
   sector_offset++;
   LEDS_off(LED2_PORT,LED2_PIN,ACTIVE_LOW);
   do
  {      
     do
     {
        temp8=GPIO_read_input(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
		if(temp8==0)
        {
           //GREENLED=ON;
           GPIO_output_set(STA013_BITEN_Port,STA013_BITEN_Pin);
           SPI_transmit(STA013_SPI_Port,buffer1_g[index1]);
		   GPIO_output_clear(STA013_BITEN_Port,STA013_BITEN_Pin);
	       //GREENLED=OFF;
	       index1++;
           if(index1>511)
           {
              if(index2>511)
              {
                  //BIT_EN=0;              
                  LEDS_on(LED2_PORT,LED2_PIN,ACTIVE_LOW);
				  index2=0;
				  
				  read_sector(sector+sector_offset, 512, buffer2_g);
				  sector_offset++;
				  if (sector_offset == sectors_per_cluster) {
					  sector_offset = 0;
					  current_cluster = find_next_clus(current_cluster, buffer1_g);
					  // read into buffer that does not have data we care about
					  sector=first_sector(current_cluster);
				  }
                  LEDS_off(LED2_PORT,LED2_PIN,ACTIVE_LOW);
              }
              buf_flag1=0;
              buf_flag2=1;

          }
       }
       else
       {
          if(index2>511)
          {
              //BIT_EN=0;
              LEDS_on(LED2_PORT,LED2_PIN,ACTIVE_LOW);
			  index2=0;
			  
			  read_sector(sector+sector_offset, 512, buffer2_g);
			  sector_offset++;
			  if (sector_offset == sectors_per_cluster) {
				  sector_offset = 0;
				  current_cluster = find_next_clus(current_cluster, buffer1_g);
				  // read into buffer that does not have data we care about
				  sector=first_sector(current_cluster);
			  }
              LEDS_off(LED2_PORT,LED2_PIN,ACTIVE_LOW);
          }
          else
          {
              if(index1>511)
              {
                  buf_flag1=0;
                  buf_flag2=1;
              }
          }
      }
   }while(buf_flag1==1);
   do
   {
	   temp8=GPIO_read_input(STA013_DATAREQ_Port,STA013_DATAREQ_Pin);
      if(temp8==0)
      {
          //REDLED=ON;
          GPIO_output_set(STA013_BITEN_Port,STA013_BITEN_Pin);
          SPI_transmit(STA013_SPI_Port,buffer2_g[index2]);
		  GPIO_output_clear(STA013_BITEN_Port,STA013_BITEN_Pin);
          //REDLED=OFF;
          index2++;
          if(index2>511)
          {
              if(index1>511)
              {
                  //BIT_EN=0; 
                  LEDS_on(LED1_PORT,LED1_PIN,ACTIVE_LOW);
				  index1=0;
				  
				  read_sector(sector+sector_offset, 512, buffer1_g);
				  sector_offset++;
				  if (sector_offset == sectors_per_cluster) {
					  sector_offset = 0;
					  current_cluster = find_next_clus(current_cluster, buffer2_g);
					  // read into buffer that does not have data we care about
					  sector=first_sector(current_cluster);
				  }
                  LEDS_off(LED1_PORT,LED1_PIN,ACTIVE_LOW);
              }
              buf_flag2=0;
              buf_flag1=1;
         
           }
        }
        else
        {
           if(index1>511)
           {
              //BIT_EN=0; 
              LEDS_on(LED1_PORT,LED1_PIN,ACTIVE_LOW);
			  index1=0;
			  
			  read_sector(sector+sector_offset, 512, buffer1_g);
			  sector_offset++;
			  if (sector_offset == sectors_per_cluster) {
				  sector_offset = 0;
				  current_cluster = find_next_clus(current_cluster, buffer2_g);
				  // read into buffer that does not have data we care about
				  sector=first_sector(current_cluster);
			  }
              LEDS_off(LED1_PORT,LED1_PIN,ACTIVE_LOW);
           }
           else
           {
               if(index2>511)
               {
                  buf_flag2=0;
                  buf_flag1=1;
               }
           }
        }
      }while(buf_flag2==1);
  }while(current_cluster != 0x0FFFFFFF);
} 

