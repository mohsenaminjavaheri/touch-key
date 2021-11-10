

#include "gpio.h"
#include "stdint.h"


	
void Config_GPIO(GPIO_TypeDef* GPIOx , uint32_t GPIO_PIN , uint32_t GPIO_MODE)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	
	if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
  }
  else if (GPIOx == GPIOG)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	}
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_MODE;
	GPIO_Init(GPIOx, &GPIO_InitStruct);
	
}
