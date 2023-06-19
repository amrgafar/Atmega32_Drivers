#include "STD_TYPES.h"

#include "CLCD_interface.h"
#include "DIO_interface.h"

int main(void)
{
	/* Initialize LCD Pins Direction */
	DIO_u8SetPortDirection(DIO_u8PORTA,DIO_u8PORT_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8PORTC,DIO_u8PIN0,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8PORTC,DIO_u8PIN1,DIO_u8PIN_OUTPUT);
	DIO_u8SetPinDirection(DIO_u8PORTC,DIO_u8PIN2,DIO_u8PIN_OUTPUT);

	CLCD_voidInit();

    // print 7arf el 3een
	u8 pattern[8] = {
			  0b00000000,
			  0b00000011,
			  0b00000100,
			  0b00000011,
			  0b00000100,
			  0b00001000,
			  0b00001000,
			  0b00000111
			};
	CLCD_voidWriteSpecialCharacter(pattern,0,0,0);

	//CLCD_voidSendString("AMR");


	while(1)
	{


	}

return 0;
}
