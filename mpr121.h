

#ifndef __MPR121_H
#define __MPR121_H


#include "stdint.h"
#include "stdbool.h"

bool write_register(uint8_t reg_addr, uint16_t value);
bool read_register(uint8_t reg_addr, uint16_t *value_ptr);
bool mpr121_init( void );
uint16_t mpr121_get_touch( void );



#endif 
