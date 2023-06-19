/************************************************************/
/************************************************************/
/***************       Author  : Amr Gafar    ***************/ 
/***************       Layer   : MCAL         ***************/ 
/***************       SWC     : EXTI         ***************/ 
/***************       Version : 1.00         ***************/
/************************************************************/
/************************************************************/

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_


#define LOW_LEVEL        1
#define ON_CHANGE        2
#define FALLING_EDGE     3
#define RISING_EDGE      4

void EXTI_voidInt0Init(void);

void EXTI_voidInt1Init(void);

void EXTI_voidInt2Init(void);
// Kol el t7t dol btre2t el post build momkn ast5dmhm aw ast5dm el fo2 .

/* A Function to set the required sense control of INT0 using Post Build conf. */
/* Inputs :  1- LOW_LEVEL
             2- ON_CHANGE
             3- FALLING_EDGE
             4- RISING_EDGE
   Outputs: Error Status with type u8               */
u8 EXTI_u8Int0SetSenseControl(u8 Copy_u8Sense);

u8 EXTI_u8Int1SetSenseControl(u8 Copy_u8Sense);

u8 EXTI_u8Int2SetSenseControl(u8 Copy_u8Sense);

u8 EXTI_u8IntEnable(u8 Copy_u8Int);

u8 EXTI_u8IntDisable(u8 Copy_u8Int);

u8 EXTI_u8Int0SetCallBack(void (*Copy_pvInt0Func)(void));



#endif
