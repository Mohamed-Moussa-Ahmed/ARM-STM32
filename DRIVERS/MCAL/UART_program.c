/********************************************/
/* Author     :  Mohamed Moussa             */
/* Version    :  V01                        */
/* Date       : 19 Sep 2020                 */             
/* Description: UART driver                 */
/********************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"


#define THRESHOULD 900000UL

u8 RecString[30] ;

void MUSART1_voidInit(void)
{
	/*9600   Buad Rate BRR= 0x341 */
	/*115200 Buad Rate BRR=  0x45 */
	MUART1-> BRR = 0x341 ;
	/*
	1-Enable RX
	2-Enable TX
	3-Enable USART
	                 */
	SET_BIT(MUART1->CR1 ,2 );
	SET_BIT(MUART1->CR1 ,3 );
	SET_BIT(MUART1->CR1 ,13);
	
	/*Frame length */
	#if UART_WORD_LENGTH_LENGTH == EIGHT_BITS
		CLR_BIT(MUART1->CR1 ,12);
	#elif UART_WORD_LENGTH_LENGTH == NINE_BITS
		SET_BIT(MUART1->CR1 ,12);
	#else 
		#error "wrong word length"
	#endif
	
	/*Stop Bits*/
	#if UART_STOP_BITS == HALF_BIT
		SET_BIT(MUART1->CR2 ,12);
		CLR_BIT(MUART1->CR2 ,13);
	#elif UART_STOP_BITS == ONE_BIT
		CLR_BIT(MUART1->CR2 ,12);
		CLR_BIT(MUART1->CR2 ,13);
	#elif UART_STOP_BITS == ONE_AND_HALF_BIT
		SET_BIT(MUART1->CR2 ,12);
		SET_BIT(MUART1->CR2 ,13);
	#elif UART_STOP_BITS == ONE_AND_HALF_BIT
		CLR_BIT(MUART1->CR2 ,12);
		SET_BIT(MUART1->CR2 ,13);
	#else 
		#error WRONG UART_STOP_BITS
	#endif
		
		/*CHECK DMA TX */
	#if UART_DMA_TX == ENABLE
		SET_BIT(MUART1->CR3 ,7);
	#elif UART_DMA_TX == DISABLE
		CLR_BIT(MUART1->CR3 ,7);
	#endif
	
	/*CHECK DMA RX */
	#if UART_DMA_RX == ENABLE
		SET_BIT(MUART1->CR3 ,6);
	#elif UART_DMA_RX == DISABLE
		CLR_BIT(MUART1->CR3 ,6);
	#endif
	/*Clear Status Register */
	MUART1-> SR = 0;
	
} 


void MUSART1_voidTransmit(u8 arr[])
{
	u8 Local_Itterator = 0;
	while (arr[Local_Itterator]!='\0')
	{
		MUART1-> DR = arr[Local_Itterator];
		
		/*WAIT TILL TX Complete*/
		/*GET_BIT(MUART1-> SR ,6) == 0*/
		while((MUART1-> SR )& 0x40 == 0);
		
		Local_Itterator++;
	}
	
}


u8 MUSART1_u8ReceiveChar(void)
{
	 u8 LOC_u8ReceivedData = 0 ;
    u32 Local_u32TimeOut=0;
	
	/*WAIT TILL RX Complete or THRESHOULD TIME passed*/
	while(((MUART1-> SR )& (1<<5) == 0)|| Local_u32TimeOut < THRESHOULD)
	{
		Local_u32TimeOut++;
	}
	if (Local_u32TimeOut == THRESHOULD)
	{
		LOC_u8ReceivedData = 255;
	}
	
	else
	{
		LOC_u8ReceivedData = (MUART1-> DR)& 0xff ; // masking to asure receive 8 bits
	}
	return LOC_u8ReceivedData;
	
	
}


u8* MUSART1_u8ReceiveString(void)
{
	u8 Local_Data ,Local_Iterator = 0;
	
	while((Local_Data = MUSART1_u8ReceiveChar())!= 255)
	{
		RecString[Local_Iterator] = Local_Data;
		Local_Iterator++;
	}
	RecString[Local_Iterator] = '\0';
	
	return (RecString);
}


void MUSAT1_Disable(void)
{
	/*Disable uart*/
	CLR_BIT(MUART1->CR1 ,13);
}


void MUSAT1_TXInterruptEnable(void)
{
	SET_BIT(MUART1->CR1 ,6);
}

void MUSAT1_RXInterruptEnable(void)
{
	SET_BIT(MUART1->CR1 ,5);
}
