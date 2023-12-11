/*
 * OLED.h
 *
 * Created: 7/1/2020 4:30:27 PM
 *  Author: youngerr
 */ 


#ifndef OLED_H_
#define OLED_H_

/****** OLED IO Definitions *******/
#define OLED_SPI_Port (SPI0)  // Defines which SPI port is used for SD Card communication
#define OLED_SS_Port (PB)  // Defines the GPIO Port for OLED Slave Select
#define OLED_SS_Pin (1<<0) // Defines the GPIO Pin for OLED Slave Select
#define OLED_Reset_Port (PD)  // Defines the GPIO Port for OLED Reset
#define OLED_Reset_Pin (1<<7) // Defines the GPIO Pin for OLED Reset
#define OLED_CMD_Port (PA)  // Defines the GPIO Port for OLED Slave Select
#define OLED_CMD_Pin (1<<6) // Defines the GPIO Pin for OLED Slave Select

/******* OLED Line Number Definitions ******/
#define OLED_LINE_0 (0xB0)
#define OLED_LINE_1 (0xB1)
#define OLED_LINE_2 (0xB2)
#define OLED_LINE_3 (0xB3)

/**************************************************************
*   OLED_SS_init
*   Inputs: None (Uses predefined port and pin values)
*   SPI Slave Select selects between a SS_ACTIVE ('0') or SS_INACTIVE ('1')
*   Output: None
**************************************************************
*   Function: Pin set to be an output by setting that bit in the DDR
*
*   Caution: The pin is set high (SS_INACTIVE) as a default
***************************************************************/
void OLED_SS_init(void);

/**************************************************************
*   OLED_SS_control
*   Inputs: Value for SPI Slave Select pin (SS_ACTIVE and SS_INACTIVE)
*          Uses predefine port and pin values.
*   Output: None
**************************************************************
*   Function: Pin set to the input value
*
*   Caution: Call OLED_SS_init first to set GPIO as output
***************************************************************/
void OLED_SS_control(uint8_t value);

/**************************************************************
*   OLED_init
*   Inputs: SPI_port connected to the OLED
*
*   Output: None
**************************************************************
*   Function: Performs all the steps needed to initialize and
*       clear the display, so it is ready to use.
*
*   Caution: Assumes a 128x32 pixel OLED display
***************************************************************/
void OLED_init(volatile SPI_t *OLED_port);

/*********************************************************************************
*          The display is 128x32 pixel display.  The display memory layout is set to
*          look like four lines that are 8 pixels high.  A companion source code file
*          defines 5x7 pixel characters (OLED_Char_Map.c) that can be used display
*          ASCII characters on the four lines of the display.
**********************************************************************************/
/*********************************************************************************
*          The following functions are used to position the cursor
*          The OLED_Set_Line_X functions are used to place the cursor
*              at the start of the specified line (page).  If only the line (page)
*              number is sent, then the cursor moves to that line, but does not
*              reset back to the start of the line (like a LF without a CR).
*
*          The OLED_Set_Cursor function can be used to place the cursor at any point
*              (in pixels) within a line (page).  The character width is 6 pixels.
*              Setting the cursor to position 0 is the same as a CR without a LF.
**********************************************************************************/


/**********************************************************************************
*          Sets the cursor to the start of a line in page mode
*          defined values: OLED_LINE_0, OLED_LINE_1, OLED_LINE2, OLED_LINE_3
***********************************************************************************/
void OLED_set_line (volatile SPI_t *OLED_port, uint8_t line_number);

/**********************************************************************************
*          Sets the cursor to the specified position (0 to 127) in a page (line).
***********************************************************************************/
void OLED_set_cursor (volatile SPI_t *OLED_port, uint8_t position);

/************************************************************************************
*         Sends one ASCII character to the OLED display connected to the specified SPI
*         The pixels for a character are defined in the file "OLED_Char_Map.c"
*         A '1' is on, a '0' is off.  The pixels are numbered, msb bottom to lsb top.
*         The character map defines 5x7 characters to leave a space at the top (lsb is '0').
*         One column of pixels is placed after the character for spacing.
*************************************************************************************/
void OLED_send_char (volatile SPI_t *OLED_port, char ASCII_input);

/**********************************************************************************
*         Clears the current line (0 to 3).  Use OLED_Set_LineX to specify line to clear.
***********************************************************************************/
void OLED_clear_line(volatile SPI_t *OLED_port, uint8_t line_number);

/**********************************************************************************
*         Clears all four lines of the display.
***********************************************************************************/
void OLED_clear_display(volatile SPI_t *OLED_port);

/*********************************************************************************************
*           Transmits a string of characters to the OLED display using the OLED_Send_Char function
*           The SPI port for the OLED display must be specified as a volatile pointer.
*           If the number of bytes to print is 0, then the string is printed until a NULL is found,
*           otherwise the specified number of bytes is printed.
***********************************************************************************************/
void OLED_transmit_string(volatile SPI_t *OLED_port, char string_name[], uint8_t num_bytes);


#endif /* OLED_H_ */