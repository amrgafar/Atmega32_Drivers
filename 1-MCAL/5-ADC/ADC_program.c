/************************************************************/
/************************************************************/
/***************       Author  : Amr Gafar    ***************/
/***************       Layer   : MCAL         ***************/
/***************       SWC     : ADC          ***************/
/***************       Version : 1.00         ***************/
/************************************************************/
/************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "ADC_config.h"
#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_register.h"

static u8* ADC_pu8Reading= NULL ;
static void (*ADC_pvCallBackNotificationFunc)(void)= NULL;

u8 ADC_u8BusyState = IDLE;

void ADC_voidInit(void)
{
	/* AVCC as reference voltage */
     SET_BIT(ADMUX,ADMUX_REFS0);
     CLR_BIT(ADMUX,ADMUX_REFS1);

     /* Activate left adjust result */
     SET_BIT(ADMUX,ADMUX_ADLAR);

     /* set prescaler to divide by 128 */
     SET_BIT(ADCSRA,ADCSRA_ADPS2);
     SET_BIT(ADCSRA,ADCSRA_ADPS1);
     SET_BIT(ADCSRA,ADCSRA_ADPS0);

     /* enable peripheral */
     SET_BIT(ADCSRA,ADCSRA_ADEN);
}

u8 ADC_u8StartConversionSynch(u8 Copy_u8Channel, u8* Copy_pu8Reading)
{
	u32 Local_u32Counter=0;
	u8 Local_u8ErrorState= OK;

	if(ADC_u8BusyState == IDLE)
	{
		/* ADC is now busy */
		ADC_u8BusyState = BUSY_FUNC;

	/* clear the MUX bits in ADMUX register */
	ADMUX &= 0b11100000;

	/* set the required channel unto the MUX bits */
	ADMUX |= Copy_u8Channel;

	/* start conversion */
	SET_BIT(ADCSRA,ADCSRA_ADSC);

	/* polling(busy waiting) until the conversion complete flag is set or counter reaching timeout value */
	while(((GET_BIT(ADCSRA,ADCSRA_ADIF))==0) && (Local_u32Counter!= ADC_u32TIMEOUT))
	{
		Local_u32Counter++;
	}
	if(Local_u32Counter == ADC_u32TIMEOUT)
	{
		//loop is broken because the timeout is reached
		Local_u8ErrorState = NOK ;
	}
	else
	{
		// loop is broken because flag is raised
		/* clear the conversion complete flag */
			SET_BIT(ADCSRA,ADCSRA_ADIF);

			/* return the reading */
			Copy_pu8Reading = ADCH;

			/* ADC is finished return it to idle */
			ADC_u8BusyState = IDLE;
	}

    }
	else
	{
			Local_u8ErrorState = BUSY_FUNC;
	}

	return Local_u8ErrorState;

}

u8 ADC_u8StartConversionAsynch(u8 Copy_u8Channel, u8* Copy_pu8Reading, void(*Copy_pvNotificationFunc)(void))
{
	   u8 Local_u8ErrorState = OK ;

	if(ADC_u8BusyState == IDLE)
	{
	   if((Copy_pu8Reading == NULL) || (Copy_pvNotificationFunc == NULL))
	   {
	    	Local_u8ErrorState = NULL_PTR_ERR;
	   }
	 else
	   {
		    /* make ADC busy in order not tp work until bring idle */
		    ADC_u8BusyState = BUSY;

		    /* Initalize the reading variable globaly  */
		    ADC_pu8Reading = Copy_pu8Reading ;

		    /* Initalize the callback notification function globaly  */
		    ADC_pvCallBackNotificationFunc = Copy_pvNotificationFunc ;

		    /* clear the MUX bits in ADMUX register */
			ADMUX &= 0b11100000;

			/* set the required channel unto the MUX bits */
			ADMUX |= Copy_u8Channel;

			/* start conversion */
			SET_BIT(ADCSRA,ADCSRA_ADSC);

			/* ADC interrupt enable  */
			SET_BIT(ADCSRA,ADCSRA_ADIE);
	   }
	}
	else
    	{
		Local_u8ErrorState = BUSY_FUNC;
	 	}

	 return Local_u8ErrorState;
}


void __vector_16 (void)   __attribute__((signal));
void __vector_16 (void)
{
	/* read ADC result */
	*ADC_pu8Reading = ADCH;

	/* make ADC state be idle because it finished */
	ADC_u8BusyState = IDLE;

	/* Invoke the callback Notification function */
	ADC_pvCallBackNotificationFunc();

	/* Diable ADC conversion complete interrupt */
	CLR_BIT(ADCSRA,ADCSRA_ADIE);

}



