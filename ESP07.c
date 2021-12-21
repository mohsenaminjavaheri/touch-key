
#include <string.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"

#include "delay.h"
#include "gpio.h"
#include "lcd16_2.h"
#include "usart.h"


#define Reset_Command         "AT+RST\r\n"
#define AT_Command             "AT\r\n"
#define Equalizer_Command    "ATE0\r\n"
#define UART_Command         "AT+UART=115200,8,1,0,0\r\n"
#define Mode_Command         "AT+CWMODE=2\r\n"
#define Server_Command        "AT+CWJAP=\"Houshmand\",\"11005310$$$\"\r\n"                         //+CWMODE = 1
#define SoftAP_Command       "AT+CWSAP=\"ESP07\",\"123456789\",1,3,4,0\r\n"                       //+CWMODE = 2
#define IP_Command              "AT+CIPAP=\"192.168.5.1\",\"192.168.5.1\",\"255.255.255.0\"\r\n"
#define TCP1_Command         "AT+CIPMUX=1\r\n"
#define TCP2_Command         "AT+CIPSERVER=1,550\r\n"


uint8_t L;

uint16_t flag;

char buf[256];

uint16_t p;

USART_InitTypeDef USART_InitStructure;

void RCC_Configuration(void)
{
  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USART1 Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  /* Configure USART1 Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void USART_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	// Set System Timer IRQ at higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
  
  /* Enable the USARTy Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Configuration(void)
{
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
	
	/* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
}




void ESP07_Configuration(void)
{
//	USART1_PutString(Reset_Command);
//		
//	DelayMs(100);
	
	USART1_PutString(AT_Command);
		
	DelayMs(100);
	
//	USART1_PutString(Equalizer_Command);
//		
//	DelayMs(100);
	
	USART1_PutString(UART_Command);
		
	DelayMs(100);
	
	USART1_PutString(Mode_Command);
		
	DelayMs(100);

	USART1_PutString(SoftAP_Command);
		
	DelayMs(100);
//	
////	USART1_PutString(Server_Command);
////		
////	DelayMs(100);
//		
//	USART1_PutString(IP_Command);
//		
//	DelayMs(100);

//	USART1_PutString(TCP1_Command);
//		
//	DelayMs(100);

//	USART1_PutString(TCP2_Command);
//		
//	DelayMs(100);
	
	while(p != 1)
	{
			lcd16x2_clrscr();
		
			p = lcd16x2_ShowDisplayShiftLeft(buf,L,flag);
			
			flag = 0;
		
			DelayMs(5000);
			
	}
//	
//	n = 0;
//	
//	for(int p=0 ; p<128 ; p++)
//	{ 
//		buf[p]=0;
//	}
}
