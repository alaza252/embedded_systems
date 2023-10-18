

#include "sd.h"

#include <stdio.h>
#include "sd_spi.h"
#include "spi.h"
#include "board.h"
#include "uart_print.h"


uint8_t sd_card_init(SDInfo &sd_info) {
	sprintf(export_print_buffer(), "Initializing SD Card!\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);

	// TODO set /CS=1
	// we need to send 74 clock cycles, which we can do by sending 10 bytes because 8*10=80 which is >= 74
	for (uint8_t i = 0; i < 10; i++) {
		uint8_t transfer_val;
		uint8_t transfer_err = spi_transfer(SD_SPI_PORT, 0xFF, &transfer_val);
		if (transfer_err != 0) {
			return SD_INIT_ERROR_COM;
		}
	}

	{ // CMD0 block
		// TODO set /CS=0
		if (send_command(0, 0) != 0) {
			// Since this is for command 0, pretty much the only possible error is a communication error
			return SD_INIT_ERROR_COM;
		}
		uint8_t r1_response;
		uint8_t response_err = receive_response(1, &r1_response); // by doing &r1_response, it's essentially an array :)
		if (response_err != 0 && response_err != SD_RECEIVE_R1_ERROR) { // we expect an R1 error, as we expect R1 to be 1
			return SD_INIT_ERROR_COM;
		}
		if (r1_response != 1) {
			return SD_INIT_ERROR_CMD0_FAIL;
		}
		// TODO set /CS=1
	}

	sprintf(export_print_buffer(), "Sending CMD0 successful!\r\n");
	uart_transmit_string(UART1, export_print_buffer(), 0);

	{ // CMD8 block

	}


	// TODO complete this function
	return 0;
}
