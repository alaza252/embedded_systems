/*
 * uart_print.h
 *
 * Created: 9/4/2020 4:36:37 PM
 *  Author: youngerr
 */ 


#ifndef UART_PRINT_H_
#define UART_PRINT_H_

#define print_port (UART1)

char * export_print_buffer(void);
uint8_t copy_string_to_buffer(const char flash_string[], char buffer_p[], uint8_t num_bytes);
void uart_transmit_string(volatile UART_t *UART_addr, char string_name[], uint8_t num_bytes);



#endif /* UART_PRINT_H_ */