#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_usart.h"

void Init_USART1(void);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);

#endif /* __USART_H */
