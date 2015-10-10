/*
	*
  ******************************************************************************
  * @file    delay.c
  * @author  Sercan ERAT
  * @version V1.0.0
  ******************************************************************************
	*
*/

#include <stm32f4xx.h>
#include "delay.h"
 
static uint32_t uwTimingDelay;
			   
void delay_init(){
	
	if (SysTick_Config(SystemCoreClock / 1000)){ 
		
    /* Capture error */ 
    while (1);
  } 
}								    

void delay_ms(uint32_t nTime){
	
  uwTimingDelay = nTime;
  while(uwTimingDelay != 0);
}

void TimingDelay_Decrement(void){
	
  if (uwTimingDelay != 0){ 
		
    uwTimingDelay--;
  }
}
