/*
 * sd.c
 *
 * Created: 10/17/2023 5:33:43 PM
 * Author : Sam Stockmann, Lavender Shannon
 */

#include "sd.h"

#include <stdio.h>
#include "sd_spi.h"
#include "spi.h"
#include "board.h"
#include "uart_print.h"


uint8_t sd_card_init(SDInfo *sd_info) {
	sprintf(export_print_buffer(), "Initializing SD Card!\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);
	cs_pin_init();

	cs_pin_set(1);
	// we need to send 74 clock cycles, which we can do by sending 10 bytes because 8*10=80 which is >= 74
	for (uint8_t i = 0; i < 10; i++) {
		uint8_t transfer_val;
		uint8_t transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return SD_INIT_ERROR_COM;
		}
	}

	{ // CMD0 block
		cs_pin_set(0);
		
		if (send_command(0, 0) != 0) {
			// Since this is for command 0, pretty much the only possible error is a communication error
			return SD_INIT_ERROR_COM;
		}
		
		uint8_t r1_response;
		uint8_t response_err = receive_response(1, &r1_response); // by doing &r1_response, it's essentially an array :)
		
		if (response_err != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		if (r1_response != 1) {
			//sprintf(export_print_buffer(), "R1 response is: %i!\r\n", r1_response);
			//uart_transmit_string(UART1, export_print_buffer(), 0);
			return SD_INIT_R1_ERROR;
		}
		
		cs_pin_set(1);
	}

	//sprintf(export_print_buffer(), "Sending CMD0 successful!\r\n");
	//uart_transmit_string(UART1, export_print_buffer(), 0);
	//{ // (temporary?)  transfer to fix stuff maybe
		//uint8_t transfer_val;
		//uint8_t transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
	//}

	{ // CMD8 block
		cs_pin_set(0);
		
		if (send_command(8, 0x000001AA) != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		uint8_t r7_response[5];
		uint8_t response_err = receive_response(5, r7_response);
		sd_info -> card_type = SD_CARD_NON_V1;
		
		if (response_err == SD_RECEIVE_R1_ERROR) {
			if ((r7_response[0] & (1<<2)) != 0) { // if illegal command flag is set in R1
				sd_info -> card_type = SD_CARD_V1;
			} else {
				return SD_INIT_R1_ERROR;
			}
		} else if (response_err != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		if (sd_info -> card_type == SD_CARD_NON_V1) {
			//for (uint8_t i = 0; i < 5; i++) {
				//sprintf(export_print_buffer(), "R7 %i: %i\r\n", i, r7_response[i]);
				//uart_transmit_string(UART1, export_print_buffer(), 0);
			//}
			if (r7_response[0] != 1) { // check for unexpected R1 status values
				//sprintf(export_print_buffer(), "R1 response is: %i!\r\n", r7_response);
				//uart_transmit_string(UART1, export_print_buffer(), 0);
				return SD_INIT_R1_ERROR;
			}
			uint8_t voltage_flags = r7_response[3];
			if ((voltage_flags & 1) == 0) { // check for correct voltage range
				
				//sprintf(export_print_buffer(), "Got voltage error! voltage_flags: %i\r\n", voltage_flags);
				//uart_transmit_string(UART1, export_print_buffer(), 0);
				return SD_INIT_VOLTAGE_ERROR;
			}
			if (r7_response[4] != 0xAA) { // check for echo value
				
				//sprintf(export_print_buffer(), "Echo error! echo_value: %i\r\n", r7_response[4]);
				//uart_transmit_string(UART1, export_print_buffer(), 0);
				return SD_INIT_R7_ECHO_ERROR;
			}
		}
		
		cs_pin_set(1);
	}
	if (sd_info -> card_type == SD_CARD_V1) {
		return SD_INIT_V1_NOT_SUPPORTED;
	}
	
	//sprintf(export_print_buffer(), "Sending CMD8 successful!\r\n");
	//uart_transmit_string(UART1, export_print_buffer(), 0);
	
	{ // CMD58
		cs_pin_set(0);
		
		if (send_command(58, 0) != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		uint8_t r3_response[5];
		uint8_t response_err = receive_response(5, r3_response);
		
		if (response_err == SD_RECEIVE_R1_ERROR) {
			return SD_INIT_R1_ERROR;
		} else if (response_err != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		if (r3_response[0] != 1) { // check for unexpected R1 status values
			return SD_INIT_R1_ERROR;
		}
		
// 		for (uint8_t i = 0; i < 5; i++) {
// 			sprintf(export_print_buffer(), "R3 %i: %i\r\n", i, r3_response[i]);
// 			uart_transmit_string(UART1, export_print_buffer(), 0);
// 		}
		//uint32_t ocr = (((uint32_t) r3_response[1]) << 24UL) | (((uint32_t) r3_response[2]) << 16UL) | (((uint32_t) r3_response[3]) << 8UL) | (uint32_t) r3_response[4];
		
		if ((r3_response[2] & 0b00110000) == 0) { // this is true when the operating voltage is out of the range 3.2-3.4
			return SD_INIT_VOLTAGE_ERROR;
		}
		
		cs_pin_set(1);
	}
	
	{ // ACMD41
		uint8_t acmd41_r1_response;
		uint16_t timeout = 0;
		do {
			cs_pin_set(0);
			
			{ // CMD55
				if (send_command(55, 0) != 0) {
					return SD_INIT_ERROR_COM;
				}
				
				uint8_t r1_response;
				uint8_t response_err = receive_response(1, &r1_response); // by doing &r1_response, it's essentially an array :)
				
				if (response_err == SD_RECEIVE_R1_ERROR) {// remember that response_err will be non-zero if any error flag is set
					return SD_INIT_R1_ERROR;
				} else if (response_err != 0) {
					return SD_INIT_ERROR_COM;
				}
				// we don't care what R1 is right here, as it must be 0 or 1, and we will check that after receiving the ACMD41 response
			}
			
			{ // ACMD41
				// remember that we can hard code 0x40... here because we don't support V1 cards
				if (send_command(41, 0x40000000) != 0) {
					return SD_INIT_ERROR_COM;
				}
				uint8_t response_err = receive_response(1, &acmd41_r1_response);
				
				if (response_err == SD_RECEIVE_R1_ERROR) {
					return SD_INIT_R1_ERROR;
				} else if (response_err != 0) {
					return SD_INIT_ERROR_COM;
				}
			}
			
			cs_pin_set(1);
			timeout++;
		} while (acmd41_r1_response != 0 && timeout != 0);
		if (timeout == 0) {
			return SD_INIT_TIMEOUT;
		}
	}
	
	{ // CMD58 Part 2 Electric Boogaloo
		cs_pin_set(0);
		
		if (send_command(58, 0) != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		uint8_t r3_response[5];
		uint8_t response_err = receive_response(5, r3_response);
		
		if (response_err == SD_RECEIVE_R1_ERROR) {
			return SD_INIT_R1_ERROR;
		} else if (response_err != 0) {
			return SD_INIT_ERROR_COM;
		}
		
		if (r3_response[0] != 0) { // check for unexpected R1 status values
			return SD_INIT_R1_ERROR;
		}
		if ((r3_response[1] & 0x80) == 0) { // Most significant bit must be set to 1
			return SD_INIT_NOT_POWERED_UP;
		}
		if ((r3_response[1] & 0x40) == 0) { // CCS=0, so standard capacity
			sd_info -> capacity = SD_CARD_STANDARD_CAPACITY;
		} else {
			sd_info -> capacity = SD_CARD_HIGH_CAPACITY;
		}
		
		// we already checked the voltage earlier, so we're good :)
		cs_pin_set(1);
	}

	return 0;
}

