
#ifndef __TOUCH_H_
#define __TOUCH_H_

#include <stdint.h>

#define Rotaries_Pad_1   1	
#define Rotaries_Pad_2   2	
#define Rotaries_Pad_3   4	
#define Rotaries_Pad_4   8


#define Slider_Pad_1   16
#define Slider_Pad_2   32
#define Slider_Pad_3   64
#define Slider_Pad_4   128


#define Pad_1   1
#define Pad_2   2
#define Pad_3   4
#define Pad_4   8



uint8_t Slider(uint32_t v_input);
uint8_t Wheel(uint32_t v_input);
uint8_t Hold(uint32_t v_input,uint8_t x, uint8_t y);
void Password(uint8_t PASS_Length , char PASS[5] );
void Menu (void);
int read_touchkey(uint16_t reg_touchkey);
int State_inMenu(void);



#endif
