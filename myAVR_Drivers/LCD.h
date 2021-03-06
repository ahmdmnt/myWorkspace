/**********************************************************************************************
 * LCD.h
 * 
 *		**LCD - Driver**
 *      Designed for AVR ATmega16 Microcontroller
 *
 *  Created on: Sep 6, 2020
 *      Author: Ahmed Montasser
 **********************************************************************************************
 * > When Using this Module :-
 * -----------------------------
 * 1. Edite Config Parameters with correct configs:
 * 
 * 2. Input Cursor status in init function:
 *      - CURSOR_ON
 *      - CURSOR_OFF
 * 
 **********************************************************************************************/


#ifndef LCD_H_
#define LCD_H_

/**** Used Directories ****/
#include "datatypes.h"
#include "macros.h"
#include "registers.h"
#include "DIO.h"

/**** Configuration Parameters [TO BE EDITED BY DEVELOPER] ****/
//#define LCD_2LINE_8BIT_MODE   
/*_OR_ #define LCD_2LINE_4BIT_MODE*/        

#define RS  PD5
#define RW  PD6
#define EN  PD7
#define LCD_CTRL_PORT PORT_D
#define LCD_CTRL_PORT_DIR DDR_D

#define LCD_DATA_PORT PORT_B
#define LCD_DATA_PORT_DIR DDR_B
/******************* End *******************/

/**** Defined Symbolic Constants ****/ 
#define CLEAR_SCREEN_COMMAND 0x01
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define CURSOR_STATE CURSOR_OFF
#define SET_CURSOR 0x80 


/**** Function Prototypes ****/
void _LCD_init(void);
void _LCD_sendCommand(uint8 command);
void _LCD_displayCharacter(uint8 data);
void _LCD_displayString(uint8 *string_data);
void _LCD_displayIntegerVariable(uint16 data);
void _LCD_adjustCursorPosition(uint8 row, uint8 col);
void _LCD_displayString_atCursor(uint8 *string_data, uint8 row, uint8 col);
void _LCD_clearScreen(void);
//void _LCD_displayCustomizedCharacter(uint8 data);

#endif /* LCD_H_ */
