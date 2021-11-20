

/* ---------------- Inclusions ----------------- */
#include <stdint.h>
#include <stdbool.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"
#include "i2c.h"

#include "mpr121.h"
#include "mpr121_def.h"


/* Number of electrodes of MPR121 */
#define NUM_OF_ELECTRODES	13


/* MPR121 I2C bus address */
#define MPR121_ADDRESS_BYTE	0x5A



/* function to write a value to a register into the MPR121. Returned value is not used at the moment */
bool write_register(uint8_t reg_addr, uint16_t value)
{
	bool success = true;

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	/* send START and wait for completion */
	I2C_GenerateSTART(I2C1, ENABLE);
	 while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* send device address, r/w request and wait for completion */
	I2C_Send7bitAddress(I2C1, MPR121_ADDRESS_BYTE<<1, I2C_Direction_Transmitter);
	 while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* check SR2 and go on if OK */
	if ((I2C1->SR2 & I2C_SR2_MSL)		/* master mode */
	&&  (I2C1->SR2 & I2C_SR2_BUSY)) {		/* communication ongoing  */

		/* send register address */
		I2C_SendData(I2C1, reg_addr);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* send data byte */
		I2C_SendData(I2C1, value);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
		/* send stop */
		I2C_GenerateSTOP(I2C1, ENABLE);
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
		/* ATTENTION: consider to wait for a while */
	} else {
		/* error */
		success = false;
	}

	return success;
}

/* function to read a register value from the MPR121. Returned value is not used at the moment */
bool read_register(uint8_t reg_addr, uint16_t *value_ptr)
{
	bool success = true;

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	/* send START and wait for completion */
	I2C_GenerateSTART(I2C1, ENABLE);
	 while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* send device address, r/w request and wait for completion */
	I2C_Send7bitAddress(I2C1, MPR121_ADDRESS_BYTE<<1, I2C_Direction_Transmitter);
	 while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* check SR2 and go on if OK */
	if ((I2C1->SR2 & I2C_SR2_MSL)		/* master mode */
	&&	(I2C1->SR2 & I2C_SR2_BUSY)) {	/* communication ongoing  */

		/* send register address */
		I2C_SendData(I2C1, reg_addr);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

		/* send START and wait for completion */
		I2C_GenerateSTART(I2C1, ENABLE);
	 while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

		/* send device address, read request and wait for completion */
		I2C_Send7bitAddress(I2C1, MPR121_ADDRESS_BYTE<<1, I2C_Direction_Receiver);
	 while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ));

		/* if communication ongoing  */
		if (I2C1->SR2 & I2C_SR2_BUSY) {
			/* read received byte */
			while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
			*value_ptr = I2C_ReceiveData(I2C1);
			/* send stop */
			I2C_GenerateSTOP(I2C1, ENABLE);
			while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF));
		} else {
			/* error */
			success = false;
		}
	} else {
		/* error */
		success = false;
	}

	return success;
}


/* MPR121 init */
bool mpr121_init( void )
{
	bool success;
	uint8_t electrodes_count;
  	uint16_t reg_value = 0;

	success = true;

	
	/* init I2C interface */
	i2c1_init();

	/* soft reset */
	write_register(SRST, 0x63); 

	/* read AFE Configuration 2 */
	read_register(AFE2, &reg_value);
	/* check default value */
	if (reg_value != 0x24) {
		/* error */
		success = false;
	} else {
		/* OK */
	}

	/* read Touch Status register */
	read_register(TS2, &reg_value);
	/* check default value */
	if ((reg_value & 0x80) != 0) {
		/* error */
		success = false;
	} else {
		/* OK */
	}	

	/* if no previous error */
	if (success == true)
	{
		/* turn off all electrodes to stop */
		write_register(ECR, 0x00); 

		/* write register with initial values */
		write_register(E0BV, 0x88);
		write_register(E1BV, 0x88);
		write_register(E2BV, 0x88);
		write_register(E3BV, 0x88);
		write_register(E4BV, 0x88);
		write_register(E5BV, 0x88);
		write_register(E6BV, 0x88);
		write_register(E7BV, 0x88);
		write_register(E8BV, 0x88);
		write_register(E9BV, 0xA5);
		write_register(E10BV, 0xA5);
		write_register(E11BV, 0xA5);
		write_register(E12BV, 0xB9);
		
		
		write_register(MHDR, 0x01);
		write_register(NHDR, 0x01);
		write_register(NCLR, 0x00);
		write_register(FDLR, 0x00);
		write_register(MHDF, 0x01);
		write_register(NHDF, 0x01);
		write_register(NCLF, 0xFF);
		write_register(FDLF, 0xFF);
		write_register(NHDT, 0x01);
		write_register(NCLT, 0x10);
		write_register(FDLT, 0x02);
		write_register(MHDPROXR, 0x0F);
		write_register(NHDPROXR, 0x0F);
		write_register(NCLPROXR, 0x00);
		write_register(FDLPROXR, 0x00);
		write_register(MHDPROXF, 0x01);
		write_register(NHDPROXF, 0x01);
		write_register(NCLPROXF, 0xFF);
		write_register(FDLPROXF, 0xFF);
		write_register(NHDPROXT, 0x00);
		write_register(NCLPROXT, 0x00);
		write_register(FDLPROXT, 0x00);
	  write_register(DTR, 0x00);
		write_register(AFE1, 0x10);  
		write_register(AFE2, 0x04);
		write_register(ACCR0, 0x0B);
		write_register(ACCR1, 0x00);
		write_register(USL, 0xCA); 
		write_register(LSL, 0x83); 
		write_register(TL, 0xB6); 
		write_register(ECR, 0x4C); // default to fast baseline startup and 12 electrodes enabled, no prox
	
		/* apply next setting for all electrodes */
		for (electrodes_count = 0; electrodes_count < NUM_OF_ELECTRODES; electrodes_count++) {
			write_register((E0TTH + (electrodes_count<<1)), 0x0A); 
			write_register((E0RTH + (electrodes_count<<1)), 0xFF); 
		}

		/* enable electrodes and set the current to 16uA */
		write_register(ECR, 0x4C); 
	}
	else
	{
		/* init error */
	}

	return success;
}

uint16_t mpr121_get_touch( void )
{
	uint16_t reg_value = 0;
	uint16_t touch_flags = 0; 

	/* read Touch 1 Status register */
	read_register(TS1, &reg_value);
	/* store lower 8 electrodes status flags */
	touch_flags = reg_value;
	/* read Touch 2 Status register */
	read_register(TS2, &reg_value);	
	/* clear unused higher flags */
	reg_value &= 0x1F;
	/* store higher 5 electrodes status flags */
	touch_flags |= (reg_value << 8);

	return touch_flags;
}
