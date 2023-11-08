/*
 * SPI.c
 *
 * Created: 10/16/2023 2:19:01 PM
 *  Author: dnsdyy
 */ 

#include "SPI.h"

uint8_t SPI_master_init(volatile SPI_t *SPI_addr, uint32_t clock_freq)
{
	uint8_t volatile SPR0_value;
	uint32_t volatile divider;
	divider = (uint32_t)((F_CPU/F_DIV)/(clock_freq)); //should be 40
	
	if(divider < 2)
	{
		SPI_addr->SPSR=1U;
		SPR0_value = 0U;
	}
	else if(divider < 4)
	{
		SPI_addr->SPSR=0U;
		SPR0_value = 0U;
	}
	else if(divider < 8)
	{
		SPI_addr->SPSR=1U;
		SPR0_value = 1U;
	}
	else if(divider < 16)
	{
		SPI_addr->SPSR=0U;
		SPR0_value = 1U;
	}
	else if(divider < 32)
	{
		SPI_addr->SPSR=1U;
		SPR0_value = 2U;
	}
	else if(divider <  64)
	{
		SPI_addr->SPSR=0U;
		SPR0_value = 2U;
	}
	else if(divider < 128)
	{
		SPI_addr->SPSR=0U;
		SPR0_value = 0x3U;		
	}
	else {
		return ERROR_CODE;
	}
	
	(SPI_addr->SPCR) = ((1<<SPE)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(SPR0_value));
	
	uint8_t CPOL_BIT= (SPI_addr->SPCR);
	CPOL_BIT&=(1<<CPOL);

	if(SPI_addr == SPI0)
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
	uint8_t dummy_var;
	return SPI_transfer(SPI_addr,send_value,&dummy_var);
}


uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value, uint8_t *rec_val)
{
	SPI_addr->SPDR=send_value;
	while((SPI_addr->SPSR & (1<<SPIF))==0)
	{
	}
	(*rec_val)=SPI_addr->SPDR;
	return 0;

}


