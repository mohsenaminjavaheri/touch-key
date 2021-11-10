
#ifndef __GPIO_H_
#define __GPIO_H_

#include "stdint.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"



void Config_GPIO(GPIO_TypeDef* GPIOx , uint32_t GPIO_PIN , uint32_t GPIO_MODE);


#endif
