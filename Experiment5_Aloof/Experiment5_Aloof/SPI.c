
#include <avr/io.h>
#include "board.h"
#include "SPI.h"
#include "GPIO_Outputs.h"

/* SPCR Settings and Bits */
#define SPI_Interrupt_Enable (1<<7)
#define SPI_Enable (1<<6)
#define lsb_First (0<<5)
#define msb_First (1<<5)
#define Master_Mode (1<<4)
#define Slave_Mode (0<<4)
#define CPOL_0 (0)
#define CPOL_1 (1)
#define CPHA_First_Edge (0)
#define CPHA_Second_Edge (1)
#define FOSC_4 (0<<0)
#define FOSC_16 (1<<0)
#define FOSC_64 (2<<0)
#define FOSC_128 (3<<0)
/* SPSR Settings and Bits */
#define FOSCx2 (1<<0)
#define FOSCx1 (0<<0)
#define SPI_Flag (1<<7)
#define WCOL_Flag (1<<6)
/* CPOL and CPHA selections */
#define CPOL_select (CPOL_0)
#define CPHA_select (CPHA_First_Edge)


/***********************************************************************
DESC:    Sets up the SPI to master mode with the clock as close
         to the input parameter as possible.
         clock=32-bit 
RETURNS: Error Flag
CAUTION: Sets the CPHA to 0 and CPOL to 0
         Disables SS and sets master mode 
************************************************************************/

uint8_t SPI_master_init(volatile SPI_t *SPI_addr, uint32_t clock_rate)
{
  uint8_t divider,return_val;
  
  if(SPI_addr==SPI0)
  {
	  GPIO_output_set(SPI0_SS_PORT,SPI0_SS_PIN);
      GPIO_output_init(SPI0_SS_PORT,SPI0_SS_PIN);  // Set SS as output
	  return_val=no_errors;
  }
  else if(SPI_addr==SPI1)
  {
	  GPIO_output_set(SPI1_SS_PORT,SPI1_SS_PIN);
      GPIO_output_init(SPI1_SS_PORT,SPI1_SS_PIN);  // Set SS as output
	  return_val=no_errors;
  }
  else
  {
	  return_val=illegal_port;
  }
  
  if(return_val==no_errors)
  {
	  
  divider=(uint8_t)((F_CPU/OSC_DIV)/clock_rate);
  if(divider<2)
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_4));
	 (SPI_addr->SPSR)=FOSCx2;
  }
  else if((divider>2)&&(divider<4))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(Master_Mode)|(FOSC_4));
	 (SPI_addr->SPSR)=FOSCx1;
  }
  else if((divider>4)&&(divider<8))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_16));
	 (SPI_addr->SPSR)=FOSCx2;
  }
  else if((divider>8)&&(divider<16))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_16));
	 (SPI_addr->SPSR)=FOSCx1;
  } 
  else if((divider>16)&&(divider<32))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_64));
	 (SPI_addr->SPSR)=FOSCx2;
  }
  else if((divider>32)&&(divider<64))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_64));
	 (SPI_addr->SPSR)=FOSCx1;
  }
  else if((divider>64)&&(divider<128))
  {
	 (SPI_addr->SPCR)=((SPI_Enable)|(Master_Mode)|(CPOL_select<<CPOL)|(CPHA_select<<CPHA)|(FOSC_128));
	 (SPI_addr->SPSR)=FOSCx1;
  }
  else  // if the SPI clock rate is too slow, a divider cannot be found
  {
    return_val=illegal_clockrate;
  }
  }
  if(SPI_addr==SPI0)
  {
	  // Set Port B pins for SPI
	  GPIO_output_set(SPI0_MOSI_PORT,SPI0_MOSI_PIN);
	  GPIO_output_init(SPI0_MOSI_PORT,SPI0_MOSI_PIN);  // Set MOSI and SCK as outputs
	  
	  if(CPOL_select==CPOL_0)
	  {
		  GPIO_output_clear(SPI0_SCK_PORT,SPI0_SCK_PIN);
		  GPIO_output_init(SPI0_SCK_PORT,SPI0_SCK_PIN);
	  }
	  else
	  {
		  GPIO_output_set(SPI0_SCK_PORT,SPI0_SCK_PIN);
		  GPIO_output_init(SPI0_SCK_PORT,SPI0_SCK_PIN);
	  }
  }
  else if(SPI_addr==SPI1)
  {
	  // Set
	  GPIO_output_set(SPI1_MOSI_PORT,SPI1_MOSI_PIN);
	  GPIO_output_init(SPI1_MOSI_PORT,SPI1_MOSI_PIN);
	  if(CPOL_select==CPOL_0)
	  {
		  GPIO_output_clear(SPI1_SCK_PORT,SPI1_SCK_PIN);
		  GPIO_output_init(SPI1_SCK_PORT,SPI1_SCK_PIN);
	  }
	  else
	  {
		  GPIO_output_set(SPI1_SCK_PORT,SPI1_SCK_PIN);
		  GPIO_output_init(SPI1_SCK_PORT,SPI1_SCK_PIN);
	  }
  }
  else
  {
	  return_val=illegal_port;
  }
  return return_val;
}

/***********************************************************************
DESC:    Sends one byte using the SPI port and returns an error flag
          
RETURNS: An error flag for timeout or write collision

CAUTION: Waits for the SPI transfer to be complete
************************************************************************/


uint8_t SPI_transmit(volatile SPI_t *SPI_addr, uint8_t data_input)
{
   uint8_t test; 
   uint16_t timeout;
   timeout=0;
   (SPI_addr->SPDR)=data_input;
   do
   {
      test=(SPI_addr->SPSR);
	  timeout++;
   }while(((test&SPI_Flag)==0)&&(timeout!=0));
   if(timeout!=0)
   {
     if((test&0x40)==0)  // no errors
     {
         timeout=no_errors;
     }
     else
     {
         timeout=SPI_Write_Collision;
     }
   }
   else
   {
     timeout=SPI_TIMEOUT;
   }
   return timeout;
}
 
/***********************************************************************
DESC:    Sends idle input using the SPI port and returns the received byte
          
RETURNS: received byte
         
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/


uint8_t SPI_receive(volatile SPI_t *SPI_addr, uint8_t idle_input, uint8_t * data_output)
{
   uint8_t test, timeout;
   timeout=0;
   (SPI_addr->SPDR)=idle_input;
   do
   {
      test=(SPI_addr->SPSR);
	  timeout++;
   }while(((test&SPI_Flag)==0)&&(timeout!=0));
   if(timeout!=0)
   {
     if((test&0x40)==0)  // no errors
     {
         *data_output=(SPI_addr->SPDR);
         timeout=no_errors;
     }
     else
     {
         *data_output=0xff;
         timeout=SPI_Write_Collision;
     }
   }
   else
   {
     *data_output=0xff;
     timeout=SPI_TIMEOUT;
   }
   return timeout;
}
 


/***********************************************************************
DESC:    Sends one byte using the SPI port and returns the received byte
          
RETURNS: SPI Error Flags | received byte
         or a timeout error
CAUTION: Waits for the SPI transfer to be complete
************************************************************************/


uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t data_input, uint8_t * data_output)
{
   uint8_t test, timeout;
   timeout=0;
   (SPI_addr->SPDR)=data_input;
   do
   {
      test=(SPI_addr->SPSR);
	  timeout++;
   }while(((test&0xC0)==0)&&(timeout!=0));
   if(timeout!=0)
   {
     if((test&0x40)==0)  // no errors
     {
         *data_output=(SPI_addr->SPDR);
         timeout=no_errors;
     }
     else
     {
         *data_output=0xff;
         timeout=SPI_Write_Collision;
     }
   }
   else
   {
     *data_output=0xff;
     timeout=SPI_TIMEOUT;
   }
   return timeout;
}
 

