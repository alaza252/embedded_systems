/*
 * CFile1.c
 *
 * Created: 7/1/2020 3:45:29 PM
 *  Author: youngerr
 */ 

#include <avr/io.h>
#include "board.h"
#include "SPI.h"
#include "GPIO_Outputs.h"
#include <avr/pgmspace.h>
#include "OLED.h"


/* The initialization command codes for the OLED display (128x32 display). */
const  uint8_t OLED_Init_Commands[24] PROGMEM = {0xAE, 0xD5, 0x80, 0xA8, 0x1F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 0xA1, 0xC8, 0xDA, 0x02, 0x81, 0x8F, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF, 0xFF};
/* Refers to the character map file "OLED_Char_Map.c" */
extern const uint8_t OLED_ASCII[127][5];

#define RESET_ACTIVE (0)
#define RESET_INACTIVE (1)
#define COMMAND (0)
#define DATA (1)
#define SS_ACTIVE (0)
#define SS_INACTIVE (1)

/********* Private Function Declarations *************/
void OLED_reset_init(void);
void OLED_reset_control(uint8_t value);
void OLED_CMD_init(void);
void OLED_CMD_control(uint8_t value);


/**************************************************************
*   OLED_reset_init
*   Inputs: None (Uses predefined port and pin values)
*   Output: None
**************************************************************
*   Function: Pin set to be an output by setting that bit in the DDR
*
*   Caution: The pin is set high (reset inactive) as a default
***************************************************************/
void OLED_reset_init(void)
{
	GPIO_output_set(OLED_Reset_Port,OLED_Reset_Pin);
	GPIO_output_init(OLED_Reset_Port,OLED_Reset_Pin); //OLED_Reset_Init();
}

/**************************************************************
*   OLED_reset_control
*   Inputs: Value for reset pin (RESET_ACTIVE and RESET_INACTIVE) 
*          Uses predefine port and pin values.
*   Output: None
**************************************************************
*   Function: Pin set to the input value
*             
*   Caution: Call OLED_reset_init first to set GPIO as output 
***************************************************************/									
void OLED_reset_control(uint8_t value)
{
	if(RESET_ACTIVE==value)
	{
		GPIO_output_clear(OLED_Reset_Port,OLED_Reset_Pin);
	}
	else
	{
		GPIO_output_set(OLED_Reset_Port,OLED_Reset_Pin);
	}
	
}


/**************************************************************
*   OLED_CMD_init
*   Inputs: None (Uses predefined port and pin values)
*   CMD selects between a command ('1') or display data ('0')
*   Output: None
**************************************************************
*   Function: Pin set to be an output by setting that bit in the DDR
*
*   Caution: The pin is set high (command) as a default
***************************************************************/
void OLED_CMD_init(void)
{
	GPIO_output_set(OLED_CMD_Port,OLED_CMD_Pin);
	GPIO_output_init(OLED_CMD_Port,OLED_CMD_Pin); //OLED_CMD_Init();
}


/**************************************************************
*   OLED_CMD_control
*   Inputs: Value for CMD pin (COMMAND and DATA)
*          Uses predefine port and pin values.
*   Output: None
**************************************************************
*   Function: Pin set to the input value
*
*   Caution: Call OLED_CMD_init first to set GPIO as output
***************************************************************/
void OLED_CMD_control(uint8_t value)
{
	if(COMMAND==value)
	{
		GPIO_output_clear(OLED_CMD_Port,OLED_CMD_Pin);
	}
	else  //DATA
	{
		GPIO_output_set(OLED_CMD_Port,OLED_CMD_Pin);
	}
	
}

/**** These are public functions to control the SPI Slave Select Pin *****/

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
void OLED_SS_init(void)
{
	GPIO_output_set(OLED_SS_Port,OLED_SS_Pin);
	GPIO_output_init(OLED_SS_Port,OLED_SS_Pin); //OLED_SS_Init();
}


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
void OLED_SS_control(uint8_t value)
{
    if(SS_ACTIVE==value)
	{
		GPIO_output_clear(OLED_SS_Port,OLED_SS_Pin);
	}
	else
	{
		GPIO_output_set(OLED_SS_Port,OLED_SS_Pin);
	}	
}

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
void OLED_init(volatile SPI_t *OLED_port)
{
	uint16_t index;
	//uint8_t * commands_p;
	uint8_t command;
	OLED_reset_init();
	OLED_CMD_init();
	OLED_SS_init();
	// Reset
	OLED_reset_control(RESET_ACTIVE); //OLED_Reset_Control(0);
	for(index=0;index<500;index++);
	OLED_reset_control(RESET_INACTIVE); //OLED_Reset_Control(1);
	// loop to send initialization commands
	OLED_CMD_control(COMMAND);// OLED_CMD_Control(0);  // All SPI values send will be commands
	//commands_p=OLED_Init_Commands;
	index=0;
	command=pgm_read_byte(&OLED_Init_Commands[index]);//*commands_p;
	
	while(command!=0xFF)
	{
		OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
		SPI_transmit(OLED_port, command);
		OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
		index++;
		command=pgm_read_byte(&OLED_Init_Commands[index]);//*(commands_p+index);
	}
	// Clear Display by filling each line (page) with all 0x00's
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	SPI_transmit(OLED_port, 0x40);            //Set cursor to home
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	OLED_clear_display(OLED_port);
	OLED_set_line(OLED_port,OLED_LINE_0);  //Set cursor at start of line 0
	
}

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
void OLED_set_line (volatile SPI_t *OLED_port, uint8_t line_number)
{
	
	OLED_CMD_control(COMMAND); //OLED_CMD_Control(0);  // All SPI values send will be commands
	//Sets to line 0 (page 0)
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	SPI_transmit(OLED_port, line_number);  // Page 0
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	//Sets the cursor address to 0x00 (start of line (page))  
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	SPI_transmit(OLED_port, 0x00);   // least significant nibble of cursor address
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	SPI_transmit(OLED_port, 0x10);   // most significant nibble of cursor address
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
}

/**********************************************************************************
*          Sets the cursor to the specified position (0 to 127) in a page (line).
***********************************************************************************/
void OLED_set_cursor (volatile SPI_t *OLED_port, uint8_t position)
{
	uint8_t temp8;
	OLED_CMD_control(COMMAND); //OLED_CMD_Control(0);  // All SPI values send will be commands
	temp8=(position&0x0F);
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	temp8=SPI_transmit(OLED_port, (0x00|temp8));    // least significant nibble of cursor address
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	temp8=(position>>4);
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	temp8=SPI_transmit(OLED_port, (0x10|temp8));    // most significant nibble of cursor address
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
}

/************************************************************************************
*         Sends one ASCII character to the OLED display connected to the specified SPI
*         The pixels for a character are defined in the file "OLED_Char_Map.c"
*         A '1' is on, a '0' is off.  The pixels are numbered, msb bottom to lsb top.
*         The character map defines 5x7 characters to leave a space at the top (lsb is '0').
*         One column of pixels is placed after the character for spacing. 
*************************************************************************************/

void OLED_send_char (volatile SPI_t *OLED_port, char ASCII_input)
{
	uint8_t output;
	uint8_t index, i;
	index=(ASCII_input);
	OLED_CMD_control(DATA); //OLED_CMD_Control(1);  // Send Data
	for(i=0;i<5;i++)
	{
		output=pgm_read_byte(&OLED_ASCII[index][i]);
		OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
		SPI_transmit(OLED_port, output);  // Write pixels
		OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	}
	OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
	SPI_transmit(OLED_port, 0x00); // One row of pixels following character
	OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
}

/**********************************************************************************
*         Clears the current line (0 to 3).  Use OLED_Set_LineX to specify line to clear.
***********************************************************************************/
void OLED_clear_line(volatile SPI_t *OLED_port, uint8_t line_number)
{
	uint8_t index;
	OLED_set_line(OLED_port,line_number);
	OLED_CMD_control(DATA); //OLED_CMD_Control(1);  // Write data
	for(index=0;index<128;index++)
	{
		OLED_SS_control(SS_ACTIVE); //OLED_SS_Control(0);
		SPI_transmit(OLED_port, 0x00);
		OLED_SS_control(SS_INACTIVE); //OLED_SS_Control(1);
	}
}

/**********************************************************************************
*         Clears all four lines of the display.
***********************************************************************************/
void OLED_clear_display(volatile SPI_t *OLED_port)
{
	OLED_clear_line(OLED_port,OLED_LINE_0); 
	OLED_clear_line(OLED_port,OLED_LINE_1);
	OLED_clear_line(OLED_port,OLED_LINE_2);
	OLED_clear_line(OLED_port,OLED_LINE_3);
}

/*********************************************************************************************
*           Transmits a string of characters to the OLED display using the OLED_Send_Char function
*           The SPI port for the OLED display must be specified as a volatile pointer.
*           If the number of bytes to print is 0, then the string is printed until a NULL is found,
*           otherwise the specified number of bytes is printed.
***********************************************************************************************/ 
void OLED_transmit_string(volatile SPI_t *OLED_port,char string_name[], uint8_t num_bytes)
{
	int8_t temp8,index;
	if(num_bytes==0)
	{
		index=0;
		temp8=string_name[index];
		while (temp8!=0)
		{
			if((temp8>15)&&(temp8<128))  // ignores control characters
			{
			    OLED_send_char(OLED_port, temp8);
			}
			index++;
			temp8=string_name[index];
		}
	}
	else
	{
		for(index=0;index<num_bytes;index++)
		{
			temp8=string_name[index];
			if((temp8>15)&&(temp8<128))  // ignores control characters
			{
				OLED_send_char(OLED_port, temp8);
			}
			
		}
	}
}
	
	
	