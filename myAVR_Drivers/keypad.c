/*****************************************************************************
 * keypad.c
 * 
 *		**keypad - Driver**
 *      Designed for AVR ATmega16 Microcontroller
 *
 *  Created on: Sep 6, 2020
 *      Author: Ahmed Montasser
 *****************************************************************************/

/** Used Directives **********************************************************/
#include "keypad.h"
/*****************************************************************************/


/** Private Function Prototypes **********************************************/
#if (N_col == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static uint8 _KEYPAD_4x3_keyMapping(uint8 button_number);
#elif (N_col == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x4 keypad
 */
static uint8 _KEYPAD_4x4_keyMapping(uint8 button_number);
#endif
/*****************************************************************************/


/** Functions Definitions ****************************************************/
uint8 _KEYPAD_getCurrentPressedKey(void)
{
	uint8 col,row;
	while(1)
	{
		for(col=0;col<N_col;col++) /* loop for columns */
		{
			/* 
			 * each time only one of the column pins will be output and 
			 * the rest will be input pins include the row pins 
			 */ 
			KEYPAD_PORT_DIR = (0b00010000<<col); 

			/* 
			 * clear the output pin column in this trace and enable the internal 
			 * pull up resistors for the rows pins
			 */ 
			KEYPAD_PORT_OUT = (~(0b00010000<<col));

			for(row=0;row<N_row;row++) /* loop for rows */
			{
				if(READ_BIT(KEYPAD_PORT_IN,row)==LOW) /* if the switch is press in this row */
				{
#if (N_col == 3)
					return _KEYPAD_4x3_keyMapping((row*N_col)+col+1);
#elif (N_col == 4)
					return _KEYPAD_4x4_keyMapping((row*N_col)+col+1);
#endif
				}
			}
		}
	}	
}

#if (N_col == 3) 

static uint8 _KEYPAD_4x3_keyMapping(uint8 button_number)
{
	switch(button_number)
	{
	case 1: return '1'; // ASCII Code of 1
	break;
	case 2: return '2';	// ASCII Code of 2
	break;
	case 3: return '3'; // ASCII Code of 3
	break;
	case 4: return '4'; // ASCII Code of 4
	break;
	case 5: return '5';	// ASCII Code of 5
	break;
	case 6: return '6'; // ASCII Code of 6
	break;
	case 7: return '7'; // ASCII Code of 7
	break;
	case 8: return '8';	// ASCII Code of 8
	break;
	case 9: return '9'; // ASCII Code of 9
	break;
	case 10: return '*'; // ASCII Code of *
	break;
	case 11: return '0'; // ASCII Code of 0
	break;
	case 12: return '#'; // ASCII Code of #
	break;
	default: return button_number;
	}
} 

#elif (N_col == 4)

static uint8 _KEYPAD_4x4_keyMapping(uint8 button_number)
{
	switch(button_number)
	{
	case 1: return 7;
	break;
	case 2: return 8;
	break;
	case 3: return 9;
	break;
	case 4: return '%'; // ASCII Code of %
	break;
	case 5: return 4;
	break;
	case 6: return 5;
	break;
	case 7: return 6;
	break;
	case 8: return '*'; /* ASCII Code of '*' */
	break;
	case 9: return 1;
	break;
	case 10: return 2;
	break;
	case 11: return 3;
	break;
	case 12: return '-'; /* ASCII Code of '-' */
	break;
	case 13: return 13;  /* ASCII of Enter */
	break;
	case 14: return 0;
	break;
	case 15: return '='; /* ASCII Code of '=' */
	break;
	case 16: return '+'; /* ASCII Code of '+' */
	break;
	default: return button_number;
	}
} 
/*****************************************************************************/
#endif
