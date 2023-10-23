/*
 * SD.c
 *
 * Created: 10/18/2023 12:49:11 PM
 *  Author: dnsdyy
 */ 
#include "SD.h"
#include <util/delay.h>
#define CRC_POLY (0x89u)
#define SDHC (1U)
#define NOT_SDHC (0)
uint8_t SDHC_status; 

uint8_t get_crc(uint8_t message[], uint8_t length) {
	uint8_t crc = 0;
	for (uint8_t i = 0; i < length; i++) {
		uint8_t message_byte = message[i];
		uint8_t lookup_value = (crc << 1) ^ message_byte;

		uint8_t value = (lookup_value & 0x80) != 0 ? lookup_value ^ CRC_POLY : lookup_value;
		for (uint8_t j = 1; j < 8; j++) {
			value <<= 1;
			if ((value & 0x80) != 0) {
				value ^= CRC_POLY;
			}
		}
		crc = value;
	}
	return crc;
}

//step3
uint8_t send_command( uint8_t command, uint32_t argument)
{
	if(command >= 64)
	{
		return ERROR_CODE;
	}
	
	uint8_t transfer_val,error_flag;
	uint8_t message[5];
	
	message[0]=(0x40|command);
	error_flag=SPI_transfer(SD_SPI_port,(0x40|command),&transfer_val);

	if (error_flag!=0)
	{
		return COM_ERROR;
	}
	for (uint8_t i=0; i<4; i++)
	{
		uint8_t send_val = (uint8_t) (argument>>(24-(i*8)));
		message[i+1]=send_val;
		error_flag=SPI_transfer(SD_SPI_port,send_val,&transfer_val);
		if(error_flag!=0)
		{
			return COM_ERROR;
		}
	}
	
	error_flag=SPI_transfer(SD_SPI_port,(get_crc(message,5)<<1)|1,&transfer_val);
	if(error_flag !=0)
	{
		return COM_ERROR;
	}
	return 0U;	
}

//step 4
uint8_t receive_response(uint8_t number_of_bytes, uint8_t *rec_array)
{
	uint8_t transfer_val;
	uint8_t transfer_err;

	uint16_t timeout = 0;
	do {
		transfer_err = SPI_transfer(SD_SPI_port, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return COM_ERROR;
		}
		timeout++;
	} while (((transfer_val & 0x80) != 0 )&& (timeout != 0));

	if (timeout == 0) { 
		return timeout_error;
	}

	rec_array[0] = transfer_val;
	if ((transfer_val & 0b01111100) != 0) {
		SPI_transfer(SD_SPI_port, 0xFF, &transfer_val);
		return R1_ERROR;
	}

	for (uint8_t i = 1; i < number_of_bytes; i++) {
		transfer_err = SPI_transfer(SD_SPI_port, 0xFF, &transfer_val);
		rec_array[i] = transfer_val;
		if (transfer_err != 0) {

			return COM_ERROR;
		}
	}

	SPI_transfer(SD_SPI_port, 0xFF, &transfer_val);
	return 0;
}

uint8_t receive_R1_resonse(uint8_t *r1_response)
{
	uint8_t transfer_val;
	uint8_t transfer_err;

	uint16_t timeout = 0;
	do {
		transfer_err = SPI_transfer(SD_SPI_port, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return COM_ERROR;
		}
		timeout++;
	} while (((transfer_val & 0x80) != 0 )&& (timeout != 0));

	if (timeout == 0) {
		return timeout_error;
	}

	*r1_response = transfer_val;
	return 0;
	
}
//step 5
uint8_t sd_card_init(void)
{
	//uint8_t * rec_array;
	//uint8_t R1_response;
	//uint32_t ACMD41_arg;
	GPIO_Output_Init(PB,CHIP_SELECT);
	
	//part A
	{
		
	GPIO_Output_Set(PB,CHIP_SELECT);
	for (uint8_t num=0;num<10;num++) //supposed to send FF 10x
	{
		uint8_t temp_val;
		uint8_t transfer_error=SPI_transfer(SD_SPI_port,0xFF,&temp_val);
		if (transfer_error!=0)
		{
			return COM_ERROR;
		}
	}
	}
	
	//part B CMD0
	{
		
	GPIO_Output_Clear(PB,CHIP_SELECT);
	if(send_command(0,0)!=0){
		return COM_ERROR;
	}
	
	uint8_t R1_response;
	uint8_t R1_error_flag= receive_response(1, &R1_response);
	if(R1_error_flag!=0)
	{
		return COM_ERROR;
	}
	if(R1_response!=1)
	{
		return ERROR_CODE;
	}
	GPIO_Output_Set(PB,CHIP_SELECT);
	}

	//part c cmd8
	{
	GPIO_Output_Clear(PB,CHIP_SELECT);
	if (send_command(8,CMD8_ARG)!=0)
	{
		return COM_ERROR;
	}
	uint8_t r7_arr[5];
	uint8_t r7_error_flag =receive_response(5,r7_arr);
	uint8_t cardtype =(0x00);
	if(r7_error_flag==R1_ERROR)
	{
		if((r7_arr[0]&(1<<2))!=0)
		{
			cardtype=0x01;
		}
		else
		{
			return R1_ERROR;
		}
	}
	else if(r7_error_flag!=0)
	{
		return SDINITERROR;
	}
	if(cardtype==0x00)
	{
		if(r7_arr[0]!=1)
		{
			return SDINITERROR;
		}
		uint8_t voltageflag=r7_arr[3];
		if((voltageflag&1)==0)
		{
			return VOLTAGE_ERROR;
		}
		if(r7_arr[4]!=0xAA)
		{
			return SDINITERROR;
		}
	}
	GPIO_Output_Set(PB,CHIP_SELECT);
	if(cardtype==0x01)
	{
		return CARD_TYPE_ERROR;
	}
	}
	
	////D CMD58
	{
		
	GPIO_Output_Clear(PB,CHIP_SELECT);
	send_command(CMD58,CMD58_ARG);
	uint8_t r3_arr[5];
	uint8_t R1_response;
	uint8_t r3_error_flag =receive_response(5,r3_arr);
	GPIO_Output_Set(PB,CHIP_SELECT);
	
	if(r3_error_flag!=0)
	{
		return ERROR_CODE;
	}
	if(r3_arr[0] != 0x01)
	{
		return ERROR_CODE;
	}
	else if(r3_arr[2]&0b00110000 == 0x00)
	{
		return VOLTAGE_ERROR;
	}
	GPIO_Output_Set(PB,CHIP_SELECT);
	}
	
	
	//Part E ACMD41(CMD 55) + 58 again
	{
		uint8_t ACMD41_R1_response;
		uint16_t timeout = 0;
		do
		{
			GPIO_Output_Clear(PB,CHIP_SELECT);
			{//CMD55
				
				if(send_command(55,0)!=0)
				{
					return ERROR_CODE;
				}
				uint8_t R1_response;
				uint8_t r1_response_error = receive_response(1,&R1_response);
				if(r1_response_error != 0)
				{
					return SDINITERROR;
				}
				
			}
			
			
			{//ACMD41
				if(send_command(41,0x40000000)!=0)
				{
					return SDINITERROR;
				}
				uint8_t response_error=receive_response(1,&ACMD41_R1_response);
				if (response_error!=0)
				{
					return SDINITERROR;
				}
			}
			GPIO_Output_Set(PB,CHIP_SELECT);
			timeout++;
		} while (ACMD41_R1_response != 0 && timeout!=0);
		if(timeout==0)
		{
			return timeout_error;
		}
	{//58 again
		
		GPIO_Output_Clear(PB,CHIP_SELECT);
		send_command(CMD58,CMD58_ARG);
		uint8_t r3_arr[5];
		uint8_t R1_response;
		uint8_t r3_error_flag =receive_response(5,r3_arr);
		GPIO_Output_Set(PB,CHIP_SELECT);
		
		if(r3_error_flag!=0)
		{
			return ERROR_CODE;
		}
		if(r3_arr[0] != 0x01)
		{
			return ERROR_CODE;
		}
		else if(r3_arr[2]&0b00110000 == 0x00)
		{
			return VOLTAGE_ERROR;
		}
		GPIO_Output_Set(PB,CHIP_SELECT);
	}
		
	}
	
	
	
	//G
	return 0U;
}

uint8_t read_block(uint16_t number_of_bytes, uint8_t *array)
{
	GPIO_Output_Clear(PB,CHIP_SELECT);
	uint8_t volatile timeout = 0;
	uint8_t R1_response;
	uint8_t R1_response_error;
	uint8_t T_val;
	
	R1_response_error = receive_R1_resonse(&R1_response);
	if(R1_response_error!=0)
	{
		return ERROR_CODE;
	}
	if(R1_response!=0)
	{
		return ERROR_CODE;
	}
	
	
	do 
	{
		R1_response_error=SPI_transfer(SD_SPI_port,0xFF,&T_val);
		if(R1_response_error!=0)
		{
			return ERROR_CODE;
		}
		timeout++;
	} while (T_val!=0xFE && (T_val&0xF0)!=0 && timeout!=0);
	
	if (timeout==0)
	{
		return timeout_error;
	}
	
	if((T_val & 0xF0) == 0)
	{
		return ERROR_CODE;
	}
	
	
	
	
	for(uint16_t i = 0; i<number_of_bytes; i++)
	{
		R1_response_error= SPI_transfer(SD_SPI_port,0xFF,&T_val);
		if(R1_response_error!=0)
		{
			return ERROR_CODE;
		}
		array[i]  = T_val;
	}
	
	
	SPI_transfer(SD_SPI_port,0xFF,&T_val);
	SPI_transfer(SD_SPI_port,0xFF,&T_val);
	SPI_transfer(SD_SPI_port,0xFF,&T_val);
	return 0U;
	
}