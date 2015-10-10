#include <stdio.h>
#include "stm32f4xx.h"
 
int fputc(int c, FILE *stream){
	
   return(ITM_SendChar(c));
}
