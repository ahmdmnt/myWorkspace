/*
 * app.c
 * 		Digital Watch Software:
 * 			- Digital Clock Mode.
 * 			- Stop Watch Mode.
 *
 *  Created on: Oct 9, 2020
 *      Author: montasse
 */

/* Preprocessor Section *********************************************************************/
#include "timer.h"
#include "external_interrupt.h"

#define DIGITAL_CLOCK_MODE	0
#define STOP_WATCH_MODE		1

#define ADJUST_OFF			0
#define ADJUST_SEC			1
#define ADJUST_MIN			2
#define ADJUST_HRS			3

#define SECONDS_1ST_DIGIT	0
#define SECONDS_2ND_DIGIT	1
#define MINUTES_1ST_DIGIT	2
#define MINUTES_2ND_DIGIT	3
#define HOURS_1ST_DIGIT		4
#define HOURS_2ND_DIGIT		5

#define DC_LED				PD0
#define SW_LED				PD1
#define _7SEG_CTRL_PORT		PORTC
#define _7SEG_DATA_PORT		PORTA
/********************************************************************************************/

/* Function Prototypes **********************************************************************/
void DW_systemPeripheralInit(void);
void DW_7SEG_screenDisplayFunction(void);
void DW_TIMER0_ISR_callBackFunction_StopWatch(void);
void DW_TIMER1_ISR_callBackFunction_DigitalClock(void);
void DW_INT0_ISR_callBackFunction_Reset_Adjust_Buttons(void);
void DW_INT1_ISR_callBackFunction_Start_Inc_Buttons(void);
void DW_INT2_ISR_callBackFunction_Stop_Dec_Buttons(void);
/********************************************************************************************/


/* Global Variables *************************************************************************/

configType_TIMER timer1_configs = {TIMER1, TIMER_F_CPU_64, 8000, OCx_DISABLED};
configType_TIMER timer0_configs = {TIMER0, TIMER_F_CPU_1024, 254, OCx_DISABLED};

uint8 mode_status = DIGITAL_CLOCK_MODE;
uint8 adjust_mode = ADJUST_SEC;

uint8 SW_seconds=0;
uint8 SW_minutes=0;
uint8 SW_hours=0;

uint8 DC_seconds=0;
uint8 DC_minutes=0;
uint8 DC_hours=0;
/********************************************************************************************/


/* Digital Watch Software Application *******************************************************/
int main(void)
{

	DW_systemPeripheralInit();

	while(1)
	{
		/* MODE Button: Navigate (Toggle) between Two Digital Watch Modes */
		if( READ_BIT(PINB, PB1)==LOW )
		{
			if(mode_status == DIGITAL_CLOCK_MODE)
			{
				mode_status = STOP_WATCH_MODE;
				PORTD |=  (1<<SW_LED);
				PORTD &= ~(1<<DC_LED);
			}
			else if(mode_status == STOP_WATCH_MODE)
			{
				mode_status = DIGITAL_CLOCK_MODE;
				PORTD |=  (1<<DC_LED);
				PORTD &= ~(1<<SW_LED);
			}

			while( READ_BIT(PINB, PB1)==LOW );	//Block Code until user release button pressing
		}

		/* Continuously Display the 7 Segment Output Data */
		DW_7SEG_screenDisplayFunction();

	}

	return 0;
}


/******************************************************************/
void DW_systemPeripheralInit(void)
{
	/* Initiate the 7 Segment Control Pins */
	DDRC = 0b00111111;
	PORTC = 0x00;

	/* Initiate the 7 Segment Data Pins*/
	DDRA = 0b00001111;
	PORTA = 0x00;

	/* Set Timer1 Call Back function */
	__TIMER_CTC_setISRCallBackFuncPointer(TIMER0, DW_TIMER0_ISR_callBackFunction_StopWatch);
	__TIMER_CTC_setISRCallBackFuncPointer(TIMER1, DW_TIMER1_ISR_callBackFunction_DigitalClock);

	/* Initiate External Interrupt 0 : Reset & Adjust Button */
	__INT0_init(FALLING_EDGE, EN_PULLUP);
	__INT0_setISRCallBackFuncPointer(DW_INT0_ISR_callBackFunction_Reset_Adjust_Buttons);

	/* Initiate External Interrupt 1 : Start & Inc Button */
	__INT1_init(FALLING_EDGE, EN_PULLUP);
	__INT1_setISRCallBackFuncPointer(DW_INT1_ISR_callBackFunction_Start_Inc_Buttons);

	/* Initiate External Interrupt 2 : Stop & Dec Button */
	__INT2_init(FALLING_EDGE, EN_PULLUP);
	__INT2_setISRCallBackFuncPointer(DW_INT2_ISR_callBackFunction_Stop_Dec_Buttons);

	/* Initiate DIO Pin (PB1) : Mode Button, Input Pin & Enable Internal Pullup Resistor */
	DDRB  &= ~(1<<PB1);
	PORTB |=  (1<<PB1);

	/* Initiate Mode LED Indicators */
	DDRD  |=  (1<<DC_LED) |  (1<<SW_LED);
	PORTD &= ~(1<<SW_LED);
	PORTD |=  (1<<DC_LED);
}

/******************************************************************/
void DW_7SEG_screenDisplayFunction(void)
{
	static uint8 _7seg_display[6];
	static uint8 disp_selector;


	/* Adjust the Clock Correct Values to Displayed, Depends on the Clock Mode */
	switch(mode_status)
	{
	case DIGITAL_CLOCK_MODE:
		_7seg_display[SECONDS_1ST_DIGIT] = DC_seconds%10;
		_7seg_display[SECONDS_2ND_DIGIT] = (DC_seconds/10)%10;
		_7seg_display[MINUTES_1ST_DIGIT] = DC_minutes%10;
		_7seg_display[MINUTES_2ND_DIGIT] = (DC_minutes/10)%10;
		_7seg_display[HOURS_1ST_DIGIT]   = DC_hours%10;
		_7seg_display[HOURS_2ND_DIGIT]   = (DC_hours/10)%10;
	break;

	case STOP_WATCH_MODE:
		_7seg_display[SECONDS_1ST_DIGIT] = SW_seconds%10;
		_7seg_display[SECONDS_2ND_DIGIT] = (SW_seconds/10)%10;
		_7seg_display[MINUTES_1ST_DIGIT] = SW_minutes%10;
		_7seg_display[MINUTES_2ND_DIGIT] = (SW_minutes/10)%10;
		_7seg_display[HOURS_1ST_DIGIT]   = SW_hours%10;
		_7seg_display[HOURS_2ND_DIGIT]   = (SW_hours/10)%10;
	break;
	default:break;
	}

	/* Loop over the [6] 7 Segments to Display Each number */
	for(disp_selector=0 ; disp_selector<6 ; disp_selector++)
	{
		/*Enable the desired 7 Segment Display*/
		_7SEG_CTRL_PORT = (1<<disp_selector);
		/*Output the desired value in Display*/
		_7SEG_DATA_PORT = _7seg_display[disp_selector];
		/*Delay in uSeconds*/
		MILLI_DELAY;MILLI_DELAY;
	}

}

/******************************************************************/
void DW_TIMER0_ISR_callBackFunction_StopWatch(void) // STOP WATCH TIMER
{
	SW_seconds++;

	if(SW_seconds == 60)
	{
		SW_seconds = 0;
		SW_minutes++;
	}
	if(SW_minutes == 60)
	{
		SW_minutes = 0;
		SW_hours++;
	}
}

/******************************************************************/
void DW_TIMER1_ISR_callBackFunction_DigitalClock(void) // DIGITAL CLOCK TIMER
{
	DC_seconds++;

	if(DC_seconds == 60)
	{
		DC_seconds = 0;
		DC_minutes++;
	}
	if(SW_minutes == 60)
	{
		DC_minutes = 0;
		DC_hours++;
	}
}

/******************************************************************/
void DW_INT0_ISR_callBackFunction_Reset_Adjust_Buttons(void)
{
	/* Digital Clock Mode: Adjust Button functionality */
	if( mode_status == DIGITAL_CLOCK_MODE )
	{
		adjust_mode++;
		if( adjust_mode == 4 )
		{
			adjust_mode = 0;
		}

		if( adjust_mode == 0 )
		{
			__TIMER_CTC_startTimer(&timer1_configs);
		}
		else
		{
			__TIMER_CTC_stopTimer(TIMER1);
		}
	}
	/* Stop Watch Mode: Reset Button functionality */
	else if( mode_status == STOP_WATCH_MODE )
	{
		SW_seconds = 0;
		SW_minutes = 0;
		SW_hours   = 0;
	}
}

/******************************************************************/
void DW_INT1_ISR_callBackFunction_Start_Inc_Buttons(void)
{
	/* Digital Clock Mode: Increment Button functionality */
	if( mode_status == DIGITAL_CLOCK_MODE )
	{
		switch(adjust_mode)
		{
		case ADJUST_SEC:	// Case adjusting Seconds
			DC_seconds++;
		break;
		case ADJUST_MIN:	// Case adjusting Minutes
			DC_minutes++;
		break;
		case ADJUST_HRS:	// Case adjusting Hours
			DC_hours++;
		break;
		default:break;
		}
	}
	/* Stop Watch Mode: Start Button functionality */
	else if( mode_status == STOP_WATCH_MODE )
	{
		__TIMER_CTC_startTimer(&timer0_configs);
	}
}

/******************************************************************/
void DW_INT2_ISR_callBackFunction_Stop_Dec_Buttons(void)
{
	/* Digital Clock Mode: Decrement Button functionality */
	if( mode_status == DIGITAL_CLOCK_MODE )
	{
		switch(adjust_mode)
		{
		case ADJUST_SEC:	// Case adjusting Seconds
			DC_seconds--;
		break;
		case ADJUST_MIN:	// Case adjusting Minutes
			DC_minutes--;
		break;
		case ADJUST_HRS:	// Case adjusting Hours
			DC_hours--;
		break;
		default:break;
		}
	}
	/* Stop Watch Mode: Stop Button functionality */
	else if( mode_status == STOP_WATCH_MODE )
	{
		__TIMER_CTC_stopTimer(TIMER0);
	}
}
/********************************************************************************************/
