
#ifndef __I2C_H
#define __I2C_H

#include "stdint.h"

//#define I2C1             I2C1
#define I2C1_RCC         RCC_APB1Periph_I2C1
#define I2C1_GPIO_RCC    RCC_APB2Periph_GPIOB
#define I2C1_GPIO        GPIOB
#define I2C1_PIN_SDA     GPIO_Pin_7
#define I2C1_PIN_SCL     GPIO_Pin_6

//#define I2C2             I2C2
#define I2C2_RCC         RCC_APB1Periph_I2C2
#define I2C2_GPIO_RCC    RCC_APB2Periph_GPIOB
#define I2C2_GPIO        GPIOB
#define I2C2_PIN_SDA     GPIO_Pin_11
#define I2C2_PIN_SCL     GPIO_Pin_10
 
#define SLAVE_ADDRESS    0x08


void i2c1_init(void);
void i2c1_start(void);
void i2c1_stop(void);
void i2c1_address_direction(uint8_t address, uint8_t direction);
void i2c1_transmit(uint8_t byte);
uint8_t i2c1_receive_ack(void);
uint8_t i2c1_receive_nack(void);
void i2c1_write(uint8_t address, uint8_t data);
void i2c1_read(uint8_t address, uint8_t* data);

void i2c2_init(void);
void i2c2_start(void);
void i2c2_stop(void);
void i2c2_address_direction(uint8_t address, uint8_t direction);
void i2c2_transmit(uint8_t byte);
uint8_t i2c2_receive_ack(void);
uint8_t i2c2_receive_nack(void);
void i2c2_write(uint8_t address, uint8_t data);
void i2c2_read(uint8_t address, uint8_t* data);



#endif

