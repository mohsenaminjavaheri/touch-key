


#include "usart.h"

void USART1_Init(uint16_t Baudrate , uint16_t Interrupt_Flag , FunctionalState NewState)
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: USART2 initialization
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_InitStruct.USART_BaudRate = Baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	// Step 2: GPIO initialization for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	// Tx pin initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	if(NewState == ENABLE)
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
		
		USART_ITConfig(USART1, Interrupt_Flag, ENABLE);
	}
	else
	{
		USART_ITConfig(USART1, Interrupt_Flag, DISABLE);
	}
}


void USART1_PutChar(char c)
{
	// Wait until transmit data register is empty
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	// Send a char using USART2
	USART_SendData(USART1, c);
}

void USART1_PutString(char *s)
{
	// Send a string
	while (*s)
	{
		USART1_PutChar(*s++);
		
		USART_ClearFlag(USART2 , USART_FLAG_TXE);
	}
}

uint16_t USART1_GetChar(void)
{
	// Wait until data is received
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET);
	// Read received char
	return USART_ReceiveData(USART1);
}




void USART2_Init(uint32_t Baudrate , uint16_t Interrupt_Flag , FunctionalState NewState)
{
	// Initialization struct
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: USART2 initialization
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	USART_InitStruct.USART_BaudRate = Baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
	// Step 2: GPIO initialization for Tx and Rx pin
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	// Tx pin initialization as push-pull alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// Rx pin initialization as input floating
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	if(NewState == ENABLE)
	{
		NVIC_InitTypeDef NVIC_InitStructure;

		/* Configure the NVIC Preemption Priority Bits */  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		
		// Set System Timer IRQ at higher priority
		NVIC_SetPriority(SysTick_IRQn, 0);
		
		/* Enable the USARTy Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		USART_ITConfig(USART2, Interrupt_Flag, ENABLE);
	}
	else
	{
		USART_ITConfig(USART2, Interrupt_Flag, DISABLE);
	}
}

void USART2_PutChar(char c)
{
	// Wait until transmit data register is empty
	while (!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
	// Send a char using USART2
	USART_SendData(USART2, c);
}

void USART2_PutString(char *s)
{
	// Send a string
	while (*s)
	{
		USART2_PutChar(*s++);
		
		USART_ClearFlag(USART2 , USART_FLAG_TXE);
	}
}

uint16_t USART2_GetChar(void)
{
	// Wait until data is received
	while (!USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
	// Read received char
	return USART_ReceiveData(USART2);
}


