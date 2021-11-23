
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rtc.h"


#include "delay.h"
#include "gpio.h"
#include "lcd16_2.h"
#include "mpr121.h"
#include "mpr121_def.h"
#include "EXTI-config.h"
#include "touch.h"
#include "RTC.h"


uint16_t value;
uint16_t value2;

uint16_t volume=50;
uint16_t volume1=50;

uint16_t light=30;
uint16_t light1=30;


uint8_t flag;

char str[32];

void EXTI9_5_IRQHandler(void)
{
	// Checks whether the interrupt is from EXTI0 or not
	if (EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		read_register(TS1 , &value);
			
		read_register(TS2 , &value2);
		
//		value2 |= 0xF0;
		
		flag = 1;
		

		// Clear pending bit			
		EXTI_ClearFlag(EXTI_Line8);
		EXTI_ClearITPendingBit(EXTI_Line8);
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
	
	char password[3]={1,4,8};
	
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
	
	
	
	while(1)
	{
		
		Time_Display(RTC_GetCounter());
		
		sprintf(str,"vol:%d  Li=%d  ",volume1,light1);	
		
		lcd16x2_gotoxy(0,1);
			
		lcd16x2_puts(str);
		
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

	}
}
