
#include <stdio.h>

#include "stm32f10x.h"

#include "delay.h"
#include "gpio.h"
#include "lcd16_2.h"
#include "mpr121.h"
#include "mpr121_def.h"
#include "EXTI-config.h"
#include "touch.h"
#include "RTC.h"
#include "usart.h"
#include "ESP07.h"



uint16_t value;
uint16_t value2;

uint16_t volume=50;
uint16_t volume1=50;

uint16_t light=30;
uint16_t light1=30;


extern uint8_t flag;

char str[32];


char c;
extern char buf[256];
uint8_t F;
uint32_t u=0;
extern uint32_t d;
extern uint8_t L;

void PWM_Init(void);

void EXTI9_5_IRQHandler(void)
{
	// Checks whether the interrupt is from EXTI0 or not
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		read_register(TS1 , &value);
			
		read_register(TS2 , &value2);

		// Clear pending bit			
		EXTI_ClearFlag(EXTI_Line8);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}			
}


void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    c = USART_ReceiveData(USART1);
		
		// Read chars until newline
		if (c != '\n')
		{
					F = 0;	
					// Concat char to buffer
					// If maximum buffer size is reached, then reset i to 0
					buf[d] = c;
					d++;			
		}
		else
		{
					// Display received string to LCD
					F=1;		
					flag = 1;	
					L=d;
		}
  }
}



void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    c = USART2_GetChar();
		
		// Read chars until newline
		if (c != '!')
		{
					F = 0;	
					// Concat char to buffer
					// If maximum buffer size is reached, then reset i to 0
					buf[u] = c;
					u++;			
		}
		else
		{
					// Display received string to LCD
					F=1;		
		}
  }
}




int main(void)
{
	DelayInit();
	
	Config_GPIO(GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOB,GPIO_Pin_9,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOC,GPIO_Pin_13,GPIO_Mode_Out_PP);	
	
	Config_GPIO(GPIOA,GPIO_Pin_0,GPIO_Mode_Out_PP);
	
	Config_GPIO(GPIOA,GPIO_Pin_1,GPIO_Mode_Out_PP);
	
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	
	mpr121_init();
	
	EXTI9_5_Init();
	
//	char password[3]={1,4,8};
	
//	Password(3,password);
	
//	DelayMs(100);
	
	lcd16x2_gotoxy(0,0);
		
	lcd16x2_puts("Please Wait...");
	
	for(int i=0;i<1;i++)
	{
		GPIOC->BSRR=GPIO_Pin_13;
		DelayMs(200);
		GPIOB->BSRR=GPIO_Pin_9;
		DelayMs(200);
		GPIOB->BSRR=GPIO_Pin_15;
		DelayMs(200);
		GPIOA->BSRR=GPIO_Pin_8;
		DelayMs(200);

		GPIOC->BRR=GPIO_Pin_13;
		DelayMs(200);		
		GPIOB->BRR=GPIO_Pin_9;
		DelayMs(200);
		GPIOB->BRR=GPIO_Pin_15;
		DelayMs(200);
		GPIOA->BRR=GPIO_Pin_8;
		DelayMs(200);		
	}
	
	lcd16x2_gotoxy(0,0);
		
	lcd16x2_puts(" ***Hi HoE*** ");
	
	DelayMs(1000);
	
	lcd16x2_clrscr();
	
	RTC_config();
	
	PWM_Init();
	
	USART1_Init( 115200 , USART_IT_RXNE , ENABLE);
	
	USART2_Init ( 115200 , USART_IT_RXNE, ENABLE);

	
	while(1)
	{
		
		Time_Display(RTC_GetCounter());
		
//		sprintf(str,"vol:%d  Li=%d  ",volume1,light1);	
//		
//		lcd16x2_gotoxy(0,1);
//			
//		lcd16x2_puts(str);
		
		State_inMenu();
		
		
		if(read_touchkey(1) == 1)
		{
			GPIOA->ODR ^= GPIO_Pin_8;
		}
		if(read_touchkey(2) == 1)
		{
			GPIOB->ODR ^= GPIO_Pin_15;
		}
		if(read_touchkey(12) == 1)
		{
			GPIOA->ODR ^= GPIO_Pin_1;
		}
		if(read_touchkey(3) == 1)
		{
			GPIOA->ODR ^= GPIO_Pin_0;
		}
		
		volume1 = Slider(volume);		
		volume = volume1;
		
		TIM4->CCR4 = (light1*719) / 100;
		
		if(F==1)
		{
			lcd16x2_clrscr();
			
			lcd16x2_gotoxy(0,1);
					
			lcd16x2_puts(buf);
			
			USART2_PutString("ok");
			
			u=0;
			
			F=0;
		}

	}
}


void PWM_Init()
{
	// Initialization struct
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// Step 1: Initialize TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	// Create 1kHz PWM
	// TIM2 is connected to APB1 bus that have default clock 72MHz
	// So, the frequency of TIM2 is 72MHz
	// We use prescaler 10 here
	// So, the frequency of TIM2 now is 72MHz
	TIM_TimeBaseInitStruct.TIM_Prescaler = 100;
	// TIM_Period determine the PWM frequency by this equation:
	// PWM_frequency = timer_clock / (TIM_Period + 1)
	// If we want 1kHz PWM we can calculate:
	// TIM_Period = (timer_clock / PWM_frequency) - 1
	// TIM_Period = (7.2MHz / 1kHz) - 1 = 7199
	TIM_TimeBaseInitStruct.TIM_Period = 719;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	// Start TIM2
	TIM_Cmd(TIM4, ENABLE);
	
	// Step 2: Initialize PWM
	// Common PWM settings
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	// Duty cycle calculation equation:
	// TIM_Pulse = (((TIM_Period + 1) * duty_cycle) / 100) - 1
	// Ex. 25% duty cycle:
	// 		TIM_Pulse = (((7199 + 1) * 25) / 100) - 1 = 1799
	//		TIM_Pulse = (((7199 + 1) * 75) / 100) - 1 = 5399
	// We initialize PWM value with duty cycle of 0%
	TIM_OCInitStruct.TIM_Pulse =0;
	TIM_OC4Init(TIM4, &TIM_OCInitStruct);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	// Step 3: Initialize GPIOA (PA0)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// Initialize PA0 as push-pull alternate function (PWM output) for LED
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}


