/***************************************************************************************************
 * hmi_ecu_appl.c
 * 		Application Software for System Human-Interface ECU.
 * 			- Controlling LCD Display
 * 			- Controlling Keypad 3x4
 * 			- Controlling Input Buttons
 * 			- Send needed Data for Control-ECU through UART
 *
 *  Created on: Sep 29, 2020
 *      Author: Ahmed Montasser
 **************************************************************************************************/



/* Preprocessor Section ***************************************************************************/
#include "DIO.h"
#include "external_interrupt.h"
#include "UART.h"
#include "LCD.h"
#include "keypad.h"


/* Application S/W #defines */
#define FOREVER                       0x01

#define ENTER_BUTTON                  PA7

/* Table of defined Communication Messages between the two ECUs */
#define HMI__UART_READY	              0xFF
#define CONT_UART_READY	              0x7F

//Received Messages from HMI-ECU:-
#define SETUP_NEW_PASSWORD		      0x01
#define CHANGE_PASSWORD			      0x02
#define CHECK_LOGIN_CREDENTIALS	      0x03
#define OPEN_DOOR				      0x04
#define CLOSE_DOOR				      0x05

//Replies Sent to HMI-ECU:-
#define PASSWORD_SAVED			      0x11
#define USER_AUTHENTICATED            0x12
#define PASSWORD_NOT_MATCHED	      0x21
/**************************************************************************************************/



/* Global Variables *******************************************************************************/

/* Variable saves Password Send by UART Module */
uint8 tx_password[13];

/* Flag allows the functionality of Opening/Closing Door when user login success */
uint8 user_authenticated_flag = FALSE;
/**************************************************************************************************/



/* Functions Declaration **************************************************************************/
void DLS_hardwarePeripheralInit(void);
void DLS_systemWelcome_setupNewPassword(void);
void DLS_changeOldPassword(void);
void DLS_userLogin(void);
void DLS_INT0_ISR_callBackFunction_CloseDoorButton(void);
void DLS_INT1_ISR_callBackFunction_OpenDoorButton(void);
/**************************************************************************************************/



/* APPL Software: MAIN FUNCTION *******************************************************************/
int main(void)
{
	uint8 task_status, pressed_key, fail_counter=0;
	uint8 lcd_message_1[16] = "Select Option:-";
	uint8 lcd_message_2[32] = "[*]Change Passwd [#]User Authen";
	uint8 lcd_message_3[9]  = "Success!";
	uint8 lcd_message_4[7]  = "ALARM!";
	uint8 lcd_message_5[6]  = "Fail!";

	/* Hardware Peripherals init Functions */
	DLS_hardwarePeripheralInit();

	/* System Welcome and 1st time Password Setup Function */
	do
	{
		/* Perform Password Setup Function, Take user Inputs */
		DLS_systemWelcome_setupNewPassword();

		/* Send CONTROL-ECU to be ready to Perform Saving if Passwords Matching */
		__UART_sendByte(HMI__UART_READY);
		while( __UART_receiveByte() != CONT_UART_READY );
		__UART_sendByte(SETUP_NEW_PASSWORD);

		/* Send the Entered Password */
		while( __UART_receiveByte() != CONT_UART_READY );
		__UART_sendByte(8);
		__UART_sendString(tx_password);

		/* Check the Status of the Sent Password either Check Succeeded or Failed */
		while( __UART_receiveByte() != CONT_UART_READY );
		__UART_sendByte(HMI__UART_READY);
		task_status = __UART_receiveByte();

		/* Output Failed */
		if(task_status == PASSWORD_NOT_MATCHED)
		{
			_LCD_displayString(lcd_message_5);
			APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
			APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
		}
	}
	while(task_status == PASSWORD_NOT_MATCHED);

	/* Output Succeeded if passes above infinite loop until process success */
	_LCD_displayString(lcd_message_3);
	APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
	APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

	while(FOREVER)
	{
		/*
		 * To be stuck doing nothing, while user_authentication 
		 * until user open then close door..
		 */
		while( user_authenticated_flag );

		/* Display Main Options over Screen: Home Mode */
		_LCD_clearScreen();
		_LCD_displayString(lcd_message_1);
		_LCD_adjustCursorPosition(1, 0);
		_LCD_displayString(lcd_message_2);

		/* fetch pressed key, either * or # */
		do
		{
			pressed_key = _KEYPAD_getCurrentPressedKey();
		}
		while( pressed_key != '*' && pressed_key != '#' );
		
		switch( pressed_key )
		{
		/*---------------------------------------------------------------*/
		case '#':   /*Key for User Login*/
			do
			{
				/* Perform User Login Function, Take user Inputs */
				DLS_userLogin();

				/* Send CONTROL-ECU to be ready to Perform Check user credentials */
				__UART_sendByte(HMI__UART_READY);
				while( __UART_receiveByte() != CONT_UART_READY );
				__UART_sendByte(CHECK_LOGIN_CREDENTIALS);

				/* Send the Entered Password */
				while( __UART_receiveByte() != CONT_UART_READY );
				__UART_sendByte(4);
				__UART_sendString(tx_password);

				/* Check the Status of the Sent Password either Check Succeeded or Failed */
				APPROX_1SEC_DELAY;
				__UART_sendByte(HMI__UART_READY);
				task_status = __UART_receiveByte();

				if( task_status == PASSWORD_NOT_MATCHED )
				{
					fail_counter++;

					/* Output System Initiated Alarm for exceeded maximum number of trials*/
					if(fail_counter == 2)
					{
						_LCD_clearScreen();
						_LCD_displayString_atCursor(lcd_message_4, 1, 0);
						fail_counter = 0 ;
						APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
						APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
						APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
						break;
					}

					/* Output User Login Fail */
					_LCD_displayString(lcd_message_5);
					APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
					APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
				}
			}
			while( task_status == PASSWORD_NOT_MATCHED);
			
			/* Output User Login Success */
			if( task_status == USER_AUTHENTICATED )
			{
				fail_counter = 0 ;
				_LCD_displayString(lcd_message_3);
				user_authenticated_flag = TRUE;
			}
		break;
		/*---------------------------------------------------------------*/

		case '*':   /*Key for Change Password*/
		/*---------------------------------------------------------------*/
			/* Perform Change Password Function, Take user Inputs */
			DLS_changeOldPassword();

			/* Send CONTROL-ECU to be ready to Perform Change Password */
			__UART_sendByte(HMI__UART_READY);
			while( __UART_receiveByte() != CONT_UART_READY );
			__UART_sendByte(CHANGE_PASSWORD);

			/* Send the Entered Passwords */
			__UART_sendByte(12);
			__UART_sendString(tx_password);

			/* Check the Status of the Sent Password either Succeeded or Failed */
			while( __UART_receiveByte() != CONT_UART_READY );
			APPROX_1SEC_DELAY;APPROX_HALF_SEC_DELAY;
			__UART_sendByte(HMI__UART_READY);
			task_status = __UART_receiveByte();

			/* Output Success */
			if( task_status == PASSWORD_SAVED )
			{
				_LCD_displayString(lcd_message_3);;
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

			}
			/* Output Failed */
			else if( task_status == PASSWORD_NOT_MATCHED )
			{
				_LCD_displayString(lcd_message_5);
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
				APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
			}
		/*---------------------------------------------------------------*/
		break;

		default:
			/*Do Nothing*/
		break;
		}

	}

	return 0;

}
/**************************************************************************************************/

/* APPL Software: DLS_hardwarePeripheralInit ******************************************************/
void  DLS_hardwarePeripheralInit(void)
{
	/*UART Configuration Parameters*/
	UART_config uart_configs = {9600, _8_BIT_DATA, ONE_STOP_BIT, DISABLE_PARITY};

	/* Initialize LCD Hardware */
	_LCD_init();

	/* Initialize ENTER Button: Digital Output Pin */
	__DIO_setPinDirection(DDR_A, ENTER_BUTTON, INPUT_PIN);
	__DIO_enableInternalPullupResistance(PORT_A, ENTER_BUTTON);

	/* Initialize Open/Close Door Buttons; External Interrupt */
	__INT0_init(FALLING_EDGE, EN_PULLUP);
	__INT1_init(FALLING_EDGE, EN_PULLUP);
	__INT0_setISRCallBackFuncPointer(DLS_INT0_ISR_callBackFunction_CloseDoorButton);
	__INT1_setISRCallBackFuncPointer(DLS_INT1_ISR_callBackFunction_OpenDoorButton);

	/* Initialize UART Module */
	__UART_init(&uart_configs);

}
/**************************************************************************************************/

/* APPL Software:DLS_systemWelcome_setupNewPassword ***********************************************/
void DLS_systemWelcome_setupNewPassword(void)
{
	uint8 passwd_length = 0;
	uint8 lcd_message_1[17] = "-Welcome to DLS-";
	uint8 lcd_message_2[18] = "Type new Passwd: ";
	uint8 lcd_message_3[4]  = "Re-";
	uint8 lcd_message_4[8]  = "SUBMIT?";
	uint8 lcd_message_5[17] = "Saving Passwd...";


	/* Display the Welcome Message */
	_LCD_clearScreen();
	_LCD_displayString_atCursor(lcd_message_1, 0, 8);
	APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;


	/* Fill the Password Entries 1st time */
	_LCD_displayString_atCursor(lcd_message_2, 1, 0);
	while(FOREVER)
	{

		/* Check if the password reach maximum length */
		if( passwd_length == 4 )
		{
			_LCD_displayString_atCursor(lcd_message_4, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7)==HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}
	}


	_LCD_clearScreen();
	_LCD_displayString_atCursor(lcd_message_1, 0, 8);

	/* Fill the Password Entries 2nd time */
	_LCD_displayString_atCursor(lcd_message_3, 1, 0);_LCD_displayString(lcd_message_2);
	while(FOREVER)
	{

		/* Check if the password reach maximum length */
		if( passwd_length == 8 )
		{
			tx_password[passwd_length] = '\0';
			_LCD_displayString_atCursor(lcd_message_4, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7)==HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}
	}

	/* Output Saving Password and return to Main Function */
	_LCD_clearScreen();
	_LCD_displayString_atCursor(lcd_message_5, 1, 0);
}
/**************************************************************************************************/

/* APPL Software: DLS_userLogin *******************************************************************/
void DLS_userLogin(void)
{
	uint8 passwd_length = 0;
	uint8 lcd_message_1[13] = "User Login:-";
	uint8 lcd_message_2[14] = "Type Passwd: ";
	uint8 lcd_message_3[8]  = "SUBMIT?";
	uint8 lcd_message_4[9]  = "Login...";


	/* Display the Welcome Message */
	_LCD_clearScreen();
	_LCD_displayString(lcd_message_1);


	/* Fill the Password Entries */
	_LCD_displayString_atCursor(lcd_message_2, 1, 0);
	while(FOREVER)
	{

		if( passwd_length == 4 )
		{
			tx_password[passwd_length] = '\0';
			_LCD_displayString_atCursor(lcd_message_3, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7) == HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}

	}

	/* Output Saving Password and return to Main Function */
	_LCD_clearScreen();
	_LCD_displayString_atCursor(lcd_message_4, 1, 0);
}
/**************************************************************************************************/

/* APPL Software: DLS_changeOldPassword ***********************************************************/
void DLS_changeOldPassword(void)
{
	uint8 passwd_length = 0;
	uint8 lcd_message_0[16]	= "Change Passwd:-";
	uint8 lcd_message_1[18] = "Type old Passwd: ";
	uint8 lcd_message_2[18] = "Type new Passwd: ";
	uint8 lcd_message_3[4]  = "Re-";
	uint8 lcd_message_4[8]  = "SUBMIT?";
	uint8 lcd_message_5[17] = "Saving Passwd...";


	_LCD_clearScreen();
	_LCD_displayString(lcd_message_0);

	/* Fill the old Password Entry */
	_LCD_displayString_atCursor(lcd_message_1, 1, 0);
	while(FOREVER)
	{

		/* Check if the password reach maximum length */
		if( passwd_length == 4 )
		{
			_LCD_displayString_atCursor(lcd_message_4, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7)==HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}
	}


	_LCD_clearScreen();
	_LCD_displayString(lcd_message_0);

	/* Fill the Password Entries 1st time */
	_LCD_displayString_atCursor(lcd_message_2, 1, 0);
	while(FOREVER)
	{

		/* Check if the password reach maximum length */
		if( passwd_length == 8 )
		{
			_LCD_displayString_atCursor(lcd_message_4, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7)==HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}
	}


	_LCD_clearScreen();
	_LCD_displayString(lcd_message_0);

	/* Fill the Password Entries 2nd time */
	_LCD_displayString_atCursor(lcd_message_3, 1, 0);_LCD_displayString(lcd_message_2);
	while(FOREVER)
	{

		/* Check if the password reach maximum length */
		if( passwd_length == 12 )
		{
			tx_password[passwd_length] = '\0';
			_LCD_displayString_atCursor(lcd_message_4, 0, 25);
			while( __DIO_readInputPinValue(PIN_A, PA7)==HIGH );
			break;
		}

		/* Delay; in order not to receive multiple values from one press */
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;

		/* Get pressed key value and save it in tx_password array */
		tx_password[passwd_length] = _KEYPAD_getCurrentPressedKey();

		/* if pressed key not a number; do nothing and overwrite this value */
		if( tx_password[passwd_length]>='0' && tx_password[passwd_length]<='9' )
		{
			_LCD_displayCharacter(tx_password[passwd_length]);
			passwd_length++;
		}
	}

	/* Output Saving Password and return to Main Function */
	_LCD_clearScreen();
	_LCD_displayString_atCursor(lcd_message_5, 1, 0);
}
/**************************************************************************************************/

/* APPL Software: DLS_INT0_ISR_callBackFunction_CloseDoorButton ***********************************/
void DLS_INT0_ISR_callBackFunction_CloseDoorButton(void)
{
	uint8 lcd_message_temp[11] = "Door Close";

	/* The below functionality won't be done until the user is logged in */
	if( user_authenticated_flag )
	{
		__UART_sendByte( HMI__UART_READY );
		while( __UART_receiveByte() != CONT_UART_READY );
		__UART_sendByte( CLOSE_DOOR );
		_LCD_clearScreen();
		_LCD_displayString_atCursor(lcd_message_temp, 1, 0);
		
		user_authenticated_flag = FALSE;
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
		APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;APPROX_1SEC_DELAY;
	}
}
/**************************************************************************************************/

/* APPL Software: DLS_INT1_ISR_callBackFunction_OpenDoorButton ************************************/
void DLS_INT1_ISR_callBackFunction_OpenDoorButton(void)
{
	uint8 lcd_message_temp[10] = "Door Open";

	/* The below functionality won't be done until the user is logged in */
	if( user_authenticated_flag )
	{
		__UART_sendByte( HMI__UART_READY );
		while( __UART_receiveByte() != CONT_UART_READY );
		__UART_sendByte( OPEN_DOOR );
		_LCD_clearScreen();
		_LCD_displayString_atCursor(lcd_message_temp, 1, 0);
	}
}
/**************************************************************************************************/
