/************************************************************/
/************************************************************/
/***************       Author  : Amr Gafar    ***************/
/***************       Layer   : HAL          ***************/
/***************       SWC     : CLCD         ***************/
/***************       Version : 1.00         ***************/
/************************************************************/
/************************************************************/
/* preprocessor file guard */

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include <util/delay.h>

#include "DIO_interface.h"

#include "CLCD_config.h"
#include "CLCD_interface.h"
#include "CLCD_private.h"

void CLCD_voidSendCommand(u8 Copy_u8Command)
{
	/* Set RS Pin to low for command */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_LOW);

	/* Set RW Pin to low to write */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);

	/* Set command to data pins */
	DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Command);

	/* send enable pulse */
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_LOW);


}

void CLCD_voidSendData(u8 Copy_u8Data)
{
	   /* Set RS Pin to high for Data */
		DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RS_PIN,DIO_u8PIN_HIGH);

		/* Set RW Pin to low to write */
		DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_RW_PIN,DIO_u8PIN_LOW);

		/* Set Data to data pins */
		DIO_u8SetPortValue(CLCD_DATA_PORT,Copy_u8Data);

		/* send enable pulse */
		DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_HIGH);
		_delay_ms(2);
		DIO_u8SetPinValue(CLCD_CTRL_PORT,CLCD_E_PIN,DIO_u8PIN_LOW);
}

void CLCD_voidInit(void)
{
	/* wait foe more than 30 ms */
	_delay_ms(40);

	/* function set command : 2 lines , 5*8 font size */
	CLCD_voidSendCommand(0b00111000);

	/* Display On/Off control : display enable, disable cursor, no blink cursor */
	CLCD_voidSendCommand(0b00001100);

	/* Display Clear */
	CLCD_voidSendCommand(0b00000001);

	/* Entry Mode Set */
	/* Is an advanced command for know so wont use it */

}

void CLCD_voidSendString(const char* Copy_pcString )
{
	u8 Local_u8Counter = 0 ;
	while(Copy_pcString[Local_u8Counter] != '\0')
	{
		CLCD_voidSendData(Copy_pcString[Local_u8Counter]);
		Local_u8Counter++ ;
	}

}

void CLCD_voidGoToXY(u8 Copy_u8XPos, u8 Copy_u8YPos)
{
	u8 Local_u8Address;
	if(Copy_u8XPos == 0)
	{
		/* Location is at first line */
		Local_u8Address = Copy_u8YPos;
	}
	else if(Copy_u8XPos == 1)
	{
		/* Location is at second line */

		Local_u8Address = Copy_u8YPos + 0x40 ;
	}
	/* Set bit number 7 for DDRAM Address command then send the command */
	/* we set the 7th bit by Adding 128 */
	CLCD_voidSendCommand(Local_u8Address + 128);

}

void CLCD_voidWriteSpecialCharacter(u8* Copy_u8Pattern, u8 Copy_u8PatternNumber, u8 Copy_u8XPos, u8 Copy_u8YPos)
{
	u8 Local_u8CGRAMAddress = 0 , Local_u8Iterator;

	/* Calculate the CGram address whose each block is 8 bytes */
	Local_u8CGRAMAddress = Copy_u8PatternNumber * 8 ;

	/* Send CGram address command to LCD , with setting bit 6 , clearing bit 7 */
	CLCD_voidSendCommand(Local_u8CGRAMAddress + 64);

	/* write the pattern into the CGram */
	for(Local_u8Iterator = 0 ;Local_u8Iterator <8 ;Local_u8Iterator++ )
	{
		CLCD_voidSendData(Copy_u8Pattern[Local_u8Iterator]);
	}

	/* Go back to the DDram to display the pattern */
	CLCD_voidGoToXY(Copy_u8XPos, Copy_u8YPos);

	/* Display the pattern written in the CGram */
	CLCD_voidSendData(Copy_u8PatternNumber);



}







