/*
	*
  ******************************************************************************
  * @file    usart.c
  * @author  Sercan ERAT
  * @version V1.0.0
  ******************************************************************************
	*
*/ 

/*

PB6 -> TX
PB7 -> RX

*/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include <stdio.h>
#include <string.h>

#include "usart.h"

#define MAX_STRLEN 24 

char received_string[MAX_STRLEN+1]; 

uint8_t cnt = 0;
int i=0;

void Init_USART1(void){
	
	GPIO_InitTypeDef GPIO_InitStruct; 
	USART_InitTypeDef USART_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) , 7 (RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			
	GPIO_Init(GPIOB, &GPIO_InitStruct);					
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	USART_InitStruct.USART_BaudRate = 115200;				
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	
	USART_InitStruct.USART_Parity = USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(USART1, &USART_InitStruct);					
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);						

	USART_Cmd(USART1, ENABLE);
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) ); 
		USART_SendData(USARTx, *s);
		*s++;
	}
}

void USART1_IRQHandler(void){
	
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
		
		char t = USART1->DR; 
		
		if(cnt == 0) {
			for(i=0; i<MAX_STRLEN; i++)
				received_string[i] = '\0';
		}
		
		if( (t != '\n') && (cnt < MAX_STRLEN) ){ 			
			
			received_string[cnt] = t;
			cnt++;
		}
		else { 
		
			
			cnt = 0;
			if(strcmp(received_string,"LED6") == 0){
					STM_EVAL_LEDOn(LED6);
			}
			
			
		}
	}
}
