
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

#include "delay.h"
#include "gpio.h"
#include "lcd16_2.h"



int main(void)
{
	

	Config_GPIO(GPIOB,GPIO_Pin_9,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOC,GPIO_Pin_13,GPIO_Mode_Out_PP);	
	
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	
	lcd16x2_gotoxy(0,0);
		
	lcd16x2_puts(" please wait...");
	
	for(int i=0;i<3;i++)
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
	lcd16x2_clrscr();
	
	lcd16x2_puts("  ***Hi HoE***");
	
	while(1)
	{
		
	}
}
