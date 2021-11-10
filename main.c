
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

#include "delay.h"
#include "gpio.h"




int main(void)
{
	DelayInit();
	
	Config_GPIO(GPIOB,GPIO_Pin_15,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOA,GPIO_Pin_8,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOB,GPIO_Pin_9,GPIO_Mode_Out_PP);	

	Config_GPIO(GPIOC,GPIO_Pin_13,GPIO_Mode_Out_PP);	
	
	for(int i=0;i<=4;i++)
	{
		GPIOB->BSRR=GPIO_Pin_9;
		DelayMs(200);
		GPIOC->BSRR=GPIO_Pin_13;
		DelayMs(200);
		GPIOB->BSRR=GPIO_Pin_15;
		DelayMs(200);
		GPIOA->BSRR=GPIO_Pin_8;
		DelayMs(200);
		
		GPIOB->BRR=GPIO_Pin_9;
		DelayMs(200);
		GPIOC->BRR=GPIO_Pin_13;
		DelayMs(200);
		GPIOB->BRR=GPIO_Pin_15;
		DelayMs(200);
		GPIOA->BRR=GPIO_Pin_8;
		DelayMs(200);
		
		
		
	}
	
	while(1)
	{
		
	}
}
