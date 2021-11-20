

#include "i2c.h"
#include "stm32f10x_i2c.h"




void i2c1_init()
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
 
    // Step 1: Initialize I2C
    RCC_APB1PeriphClockCmd(I2C1_RCC, ENABLE);
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitStruct);
    I2C_Cmd(I2C1, ENABLE);
 
    // Step 2: Initialize GPIO as open drain alternate function
    RCC_APB2PeriphClockCmd(I2C1_GPIO_RCC, ENABLE);
    GPIO_InitStruct.GPIO_Pin = I2C1_PIN_SCL | I2C1_PIN_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(I2C1_GPIO, &GPIO_InitStruct);
}
 
void i2c1_start()
{
    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
 
    // Generate start condition
    I2C_GenerateSTART(I2C1, ENABLE);
 
    // Wait for I2C EV5. 
    // It means that the start condition has been correctly released 
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}
 
void i2c1_stop()
{
    // Generate I2C stop condition
    I2C_GenerateSTOP(I2C1, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
}
 
void i2c1_address_direction(uint8_t address, uint8_t direction)
{
    // Send slave address
    I2C_Send7bitAddress(I2C1, address, direction);
 
    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2C1,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        while (!I2C_CheckEvent(I2C1,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}
 
void i2c1_transmit(uint8_t byte)
{
    // Send data byte
    I2C_SendData(I2C1, byte);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and 
    // output on the bus)
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
 
uint8_t i2c1_receive_ack()
{
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C1);
}
 
uint8_t i2c1_receive_nack()
{
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C1);
}
 
void i2c1_write(uint8_t address, uint8_t data)
{
    i2c1_start();
    i2c1_address_direction(address << 1, I2C_Direction_Transmitter);
    i2c1_transmit(data);
    i2c1_stop();
}
 
void i2c1_read(uint8_t address, uint8_t* data)
{
		i2c1_start();
    i2c1_address_direction(address<<1 , I2C_Direction_Receiver);		
    *data = i2c1_receive_nack();
    i2c1_stop();
}


uint8_t read_e(uint8_t addr){
uint8_t Data = 0;
I2C_GenerateSTART(I2C1, ENABLE);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
I2C_Send7bitAddress(I2C1, 0x5A, I2C_Direction_Transmitter);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
I2C_SendData(I2C1, addr);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
I2C_GenerateSTART(I2C1, ENABLE);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
I2C_Send7bitAddress(I2C1, 0x5A, I2C_Direction_Receiver);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
I2C_AcknowledgeConfig(I2C1, DISABLE);
while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
I2C_GenerateSTOP(I2C1, ENABLE);
Data = I2C_ReceiveData(I2C1);
return Data;  
}


// i2c2 configured for slave
void i2c2_init()
{
    // Initialization struct
    I2C_InitTypeDef I2C_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
  	NVIC_InitTypeDef NVIC_InitStructure;
 
    // Step 1: Enable i2c and Gpio Clock
    RCC_APB1PeriphClockCmd(I2C2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(I2C2_GPIO_RCC, ENABLE);
 
    // Step 2: Initialize GPIO as open drain alternate function
    GPIO_InitStruct.GPIO_Pin = I2C2_PIN_SCL | I2C2_PIN_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(I2C2_GPIO, &GPIO_InitStruct);
		
		// Step 3: Configure and enable I2Cx event interrupt
		NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	 
		// Configure and enable I2C1 error interrupt 
		NVIC_InitStructure.NVIC_IRQChannel = I2C2_ER_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_Init(&NVIC_InitStructure);
		
		
		I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C2, &I2C_InitStruct);
    I2C_Cmd(I2C2, ENABLE);
		
		/* Enable I2C1 event and buffer interrupts */
		I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
		/* Enable I2C1 Error interrupts */
		I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);

}
 
void i2c2_start()
{
    // Wait until I2Cx is not busy anymore
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
 
    // Generate start condition
    I2C_GenerateSTART(I2C2, ENABLE);
 
    // Wait for I2C EV5. 
    // It means that the start condition has been correctly released 
    // on the I2C bus (the bus is free, no other devices is communicating))
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
}
 
void i2c2_stop()
{
    // Generate I2C stop condition
    I2C_GenerateSTOP(I2C2, ENABLE);
    // Wait until I2C stop condition is finished
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF));
}
 
void i2c2_address_direction(uint8_t address, uint8_t direction)
{
    // Send slave address
    I2C_Send7bitAddress(I2C2, address, direction);
 
    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2C2,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        while (!I2C_CheckEvent(I2C1,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}
 
void i2c2_transmit(uint8_t byte)
{
    // Send data byte
    I2C_SendData(I2C2, byte);
    // Wait for I2C EV8_2.
    // It means that the data has been physically shifted out and 
    // output on the bus)
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}
 
uint8_t i2c2_receive_ack()
{
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C2);
}
 
uint8_t i2c2_receive_nack()
{
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2C2, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
 
    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2C2);
}
 
void i2c2_write(uint8_t address, uint8_t data)
{
    i2c2_start();
    i2c2_address_direction(address << 1, I2C_Direction_Transmitter);
    i2c2_transmit(data);
    i2c2_stop();
}
 
void i2c2_read(uint8_t address, uint8_t* data)
{
    i2c2_start();
    i2c2_address_direction(address << 1, I2C_Direction_Receiver);
    *data = i2c2_receive_nack();
    i2c2_stop();
}
