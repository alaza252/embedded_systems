/*
 * SPI.c
 *
 * Created: 10/16/2023 2:19:01 PM
 *  Author: dnsdyy
 */ 

#include "SPI.h"

uint8_t SPI_master_init(volatile SPI_t *SPI_addr, uint32_t clock_freq)
{
	uint8_t divider;
	divider = ((F_CPU/F_DIV)/(clock_freq));
	
	(SPI_addr->SPCR) = ((1<<SPE)|(1<<MSTR)|(CPOL_OFFSET)|(CPHA_OFFSET<<CPHA)|(0<<0));
	
	if(divider<2|| ((4<=divider)&&(divider<8)) || ((16<=divider)&& (divider<32)) || ((64<=divider)&&(divider<128)))
	{
		SPI_addr->SPSR=1U;
	}
	else if(divider<4|| ((8<=divider)&&(divider<16)) || ((32<=divider)&&(divider<64)))
	{
		SPI_addr->SPSR=0U;
	}
	else
	{
		return ERROR_CODE;
	}
	
	
	
	uint8_t CPOL_BIT= (SPI_addr->SPCR);
	CPOL_BIT&=CPOL_OFFSET;
	if(&(SPI_addr->SPCR) == SPI0)
	{
		//MOSI initialized to a 1 output. port B bit 5
		GPIO_Output_Set(PB,SPI0_MOSI);
		GPIO_Output_Init(PB,SPI0_MOSI);
		if(CPOL_BIT==0)
		{
			GPIO_Output_Clear(PB,SPI0_SCK);
			GPIO_Output_Init(PB,SPI0_SCK);
		}
		else
		{
			GPIO_Output_Set(PB,SPI0_SCK);
			GPIO_Output_Init(PB,SPI0_SCK);
		}
	}
	else  //SPI1
	{
		GPIO_Output_Set(PE,SPI1_MOSI);
		GPIO_Output_Init(PE,SPI1_MOSI);
		if(CPOL_BIT==0)
		{
			GPIO_Output_Clear(PD,SPI1_SCK);
			GPIO_Output_Init(PD,SPI1_SCK);
		}
		else
		{
			GPIO_Output_Set(PD,SPI1_SCK);
			GPIO_Output_Init(PD,SPI1_SCK);
		}
	}
	return 1U;
}

uint8_t SPI_transmit(volatile SPI_t *SPI_addr, uint8_t send_value)
{
	//wait until SPIF bit in SPSR register is high
	uint8_t status;
	do
	{
		status = (SPI_addr -> SPSR)&(1<<3);
	}
	while(status == 0);
	SPI_addr -> SPDR = send_value; //put send_value into data register
	return 1U;
}

uint8_t SPI_receive(volatile SPI_t *SPI_addr)
{
	//wait until SPIF bit in SPSR register is high
	uint8_t status;
	do
	{
		status = (SPI_addr -> SPSR)&(1<<3);
	}while(status==0);
	return SPI_addr -> SPDR;	// return value in SPDR
}

uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value)
{
	uint8_t status;
	SPI_addr->SPDR=send_value;
	do
	{
		status=(SPI_addr->SPSR);
	}while((status&0x80)==0);
	return (SPI_addr-> SPDR);
}