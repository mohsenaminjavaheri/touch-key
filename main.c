
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
	DelayInit();
	
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	
	lcd16x2_gotoxy(0,0);
		
	lcd16x2_puts("   Hello HoE");
	
	DelayMs(500);
	
	lcd16x2_clrscr();
	
	while(1)
	{
		
	}
}