/*
 * SD.c
 *
 * Created: 10/18/2023 12:49:11 PM
 *  Author: dnsdyy
 */ 
#include "SD.h"

//step3
uint8_t send_command(volatile SPI_t *SPI_addr, uint8_t command, uint32_t argument)
{
	if(command < 63)
	{
		command |= 0x40;
		SPI_transmit(SPI_addr,command);
		uint8_t arg_8 = (uint8_t)(argument&0xF000);
		
		SPI_transmit(SPI_addr,arg_8);
		arg_8 = (uint8_t)(argument&0x0F00);
		SPI_transmit(SPI_addr,arg_8);
		arg_8 = (uint8_t)(argument&0x00F0);
		SPI_transmit(SPI_addr,arg_8);
		arg_8 = (uint8_t)(argument&0x000F);
		SPI_transmit(SPI_addr,arg_8);
		if(command == CMD0)
		{
			arg_8 = 0x95; //check if check
		}
		else if(command == CMD8)
		{
			if(arg_8 == 0xAA)
			{
				arg_8=0x87;
			}
		}
		else
		{
			arg_8 = 0x01;
		}
		SPI_transmit(SPI_addr,arg_8);
		return 1U;
	}
	else{
		return ERROR_CODE;
	}
}



//step 4
uint8_t receive_response(volatile SPI_t *SPI_addr, uint8_t number_of_bytes, uint8_t * rec_array)
{
	//uint8_t return_value;
	//return_value=no_errors;
	uint8_t timeout;
	uint8_t rcvd_value;
	timeout=0;
	
	
	do
	{
		rcvd_value=SPI_transfer(SPI_addr,0xFF); // SPI_Receive
		timeout++;
	}while((rcvd_value==0xFF)&&(timeout!=0));

	

	if(timeout==0)
	{
		return ERROR_CODE;
	}
	else if((rcvd_value&0xFE)!=0x00) 
	{
		*rec_array=rcvd_value; 
		return ERROR_CODE;
	}
	else
	{
		*rec_array=rcvd_value; 
		if(number_of_bytes>1)
		{
			for(uint8_t index=1;index<number_of_bytes;index++)
			{
				rcvd_value=SPI_transfer(SPI_addr,0xFF);
				rec_array[index]=rcvd_value;
			}
		}
	}
	rcvd_value=SPI_transfer(SPI_addr,0xFF);
	return rcvd_value;
}


//step 5
uint8_t sd_card_init(volatile SPI_t *SPI_addr)
{
	
	uint8_t * rec_array;
	uint8_t R1_response;
	uint32_t ACMD41_arg;
	GPIO_Output_Init(PB,CHIP_SELECT);
	
	//A
	GPIO_Output_Set(PB,CHIP_SELECT);
	SPI_master_init(SPI0,400000U);
	for (uint8_t num=0;num<10;num++)
	{
		SPI_transmit(SPI0,0xFF);
	}
	
	
	
	//B
	GPIO_Output_Clear(PB,CHIP_SELECT);
	send_command(SPI_addr,CMD0,CMD0_ARG);
	R1_response= receive_response(SPI_addr,1,&R1_response);
	if(R1_response!=0x01)
	{
		return ERROR_CODE;
	}
	GPIO_Output_Set(PB,CHIP_SELECT);




	//C
	GPIO_Output_Clear(PB,CHIP_SELECT);
	send_command(SPI_addr,CMD8,CMD8_ARG);
	rec_array=receive_response(SPI_addr,5,rec_array);//this is not right but a placeholder
	GPIO_Output_Set(PB,CHIP_SELECT);
	if(rec_array[0]==0x01)
	{
		if((rec_array[3]==0x01)&&(rec_array[4]==0xAA))
		{
			ACMD41_arg=0x40000000;
		}
		else
		{
			//incompatible_voltage
			return ERROR_CODE;
		}
	}
	else if (rec_array[0]==0x05)//version 1.1 or older card
	{
		ACMD41_arg=0x00000000;
		return ERROR_CODE;
	}
	else
	{
		return ERROR_CODE;
	}
	
	//D
	GPIO_Output_Clear(PB,CHIP_SELECT);
	send_command(SPI_addr,CMD58,CMD58_ARG);
	
	
	
	//E
	
	
	
	
	//F
	
	
	
	
	//G
	
}
