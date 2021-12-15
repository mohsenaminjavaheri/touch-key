

#ifndef __USART_H
#define __USART_H



#include "stm32f10x.h"


void USART1_Init(uint32_t Baudrate , uint16_t Interrupt_Flag , FunctionalState NewState);
void USART1_PutChar(char c);
void USART1_PutString(char *s);
uint16_t USART1_GetChar(void);

void USART2_Init(uint32_t Baudrate , uint16_t Interrupt_Flag , FunctionalState NewState);
void USART2_PutChar(char c);
void USART2_PutString(char *s);
uint16_t USART2_GetChar(void);



#endif
