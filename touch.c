

#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_i2c.h"

#include "i2c.h"
#include "mpr121.h"
#include "mpr121_def.h"
#include "delay.h"
#include "lcd16_2.h"
#include "touch.h"

extern uint16_t value;
extern uint16_t value2;

uint8_t i=0;

int j=0;
	
unsigned char kk=0;
	
uint8_t k=0;

uint8_t pass=0;

uint8_t ff=0;

uint16_t v=10;

uint16_t f1=0;
uint16_t f2=0;

uint16_t f11=0;
uint16_t f22=0;


char num[32];

uint16_t regvalue;

uint16_t regvalue2;


uint8_t Slider(uint32_t v_input)
{
	v = v_input;
//	read_register(TS1 , &regvalue);
	
	if(value != 0  && value > 15)
	{	
		if(value != regvalue2)
		{
			if(regvalue2%3==0)
			{
			
				f1=regvalue2-value;
				f11=f1<<1;
				f22=f1>>1;

				f11-=value;
				f22-=value;
				
				if(f11==0)
				{
					if(v==0) v=0;
					else v++;
				}
				else if(f22==0)
				{
					if(v==100) v=100;
					else v--;
				}
				
			}
			else
			{
				
				f1=regvalue2<<1;
				f2=regvalue2>>1;	
				
				f11=value-f1;
				f22=value-f2;
				
				
				if(f11==0)
				{
					if(v==0) v=0;
					else v++;
				}
				else if(f22==0)
				{
					if(v==100) v=100;
					else v--;
				}
				else
				{
					f11-=regvalue2;
					f22-=regvalue2;
					if(f11==0)
					{
						if(v==0) v=0;
						else v++;
					}
					else if(f22==0)
					{
						if(v==100) v=100;
						else v--;
					}				
				}
			}
		}				
	}	
				
			
					
			

	regvalue2 = value;	
	return v;
}


uint8_t Wheel(uint32_t v_input)
{
	v = v_input;
//	read_register(TS1 , &regvalue);
	
	if(value != 0 && value < 16)
	{	
		if(value != regvalue)
		{
			if(regvalue%3==0)
			{
				if(regvalue==Rotaries_Pad_1 + Rotaries_Pad_4)   
				{
					f1=regvalue-value;
					f11=f1>>3;
					f22=f1<<3;
				}
				else
				{
					f1=regvalue-value;
					f11=f1<<1;
					f22=f1>>1;
				}
				
				f11-=value;
				f22-=value;
				
				if(f11==0)
				{
					if(v==0) v=0;
					else v--;
				}
				else if(f22==0)
				{
					if(v==100) v=100;
					else v++;
				}
				
			}
			else
			{
				if(value==Rotaries_Pad_1 + Rotaries_Pad_4)
				{
					f1=regvalue>>3; 
					f2=regvalue<<3;
				}
				else
				{
					f1=regvalue<<1;
					f2=regvalue>>1;
				}
				
				f11=value-f1;
				f22=value-f2;
				
				
				if(f11==0)
				{
					if(v==0) v=0;
					else v--;
				}
				else if(f22==0)
				{
					if(v==100) v=100;
					else v++;
				}
				else
				{
					f11-=regvalue;
					f22-=regvalue;
					if(f11==0)
					{
						if(v==0) v=0;
						else v--;
					}
					else if(f22==0)
					{
						if(v==100) v=100;
						else v++;
					}				
				}
			}
		}				
	}	
				
			
					
			

	regvalue = value;	
	return v;
}


uint8_t Hold(uint32_t v_input)
{
	uint16_t INC_PAD = Pad_1;
	
	uint16_t DEC_PAD = Pad_4;
	
	uint16_t RESET_PAD = Pad_2;
	
	
	v = v_input;
//	read_register(TS1 , &value);
	if(value2 != 0)
		{
			regvalue2 = value2;
			
			while(value2 == regvalue2)
			{
				
				DelayMs(100);
				if(kk>=3)
				{
					if(value2 == INC_PAD)
					{
						if(v==100) v=100;
						else v++;						
					}
					else if(value2 == DEC_PAD)
					{
						if(v==0) v=0;
						else v--;
					}
					else if(value2 == RESET_PAD)
					{
						v=10;
					}
					else
						break;
					
					sprintf(num,"   Volume=%d",v);	

					lcd16x2_clrscr();
					
					lcd16x2_gotoxy(0,0);
					
					lcd16x2_puts(num);
				
				}
				
				regvalue2 = value2;
				
				kk++;
				
				DelayMs(100);
				
				if(value2 == 0)
					break;
			}
			
		}

		kk=0;
		return v;
}


void Password(uint8_t PASS_Length , char PASS[5] )
{
//	if(PASS_Length<3 || PASS_Length>5)
//	{
//		return;
//	}	
	char touch[PASS_Length];
	
	lcd16x2_clrscr();
		
	lcd16x2_gotoxy(0,0);
	
	sprintf(num,"Password:");	
	
	lcd16x2_puts(num);
	
	while(pass!=1)
	{
		lcd16x2_clrscr();
		
		read_register(TS2 , &value);
		
		lcd16x2_gotoxy(0,0);
		
		lcd16x2_puts(num);
		
		DelayMs(300);
		
		if(value==0)
		{
			ff++;
			DelayMs(10);
			if(ff>=10){i=0;k=0;ff=0;sprintf(num,"Password:");}
			
		}
		if(value != regvalue2  &  value!=0)
		{
			ff=0;
			touch[i]=value;
			i++;
			switch (PASS_Length)
			{
				case 3 :
									switch (i)
									{
										case 1: sprintf(num,"Password:%d",value);	break;
										case 2: sprintf(num,"Password:*%d",value); break;
										case 3: sprintf(num,"Password:**%d",value);	break;
									}
									break;
				case 4 : 
									switch (i)
									{
										case 1: sprintf(num,"Password:%d",value);	break;
										case 2: sprintf(num,"Password:*%d",value);	break;
										case 3: sprintf(num,"Password:**%d",value);  break;
										case 4: sprintf(num,"Password:***%d",value);	 break;
									}
									break;
				case 5 :
									switch (i)
									{
										case 1: sprintf(num,"Password:%d",value);	break;
										case 2: sprintf(num,"Password:*%d",value);	break;
										case 3: sprintf(num,"Password:**%d",value);  break;
										case 4: sprintf(num,"Password:***%d",value);  break;
										case 5: sprintf(num,"Password:****%d",value);  break;
									}
									break;
			}
			
		}
		
		regvalue2 = value;
			
		if(i==PASS_Length)
		{
			i=0;
			for(j=0 ; j<PASS_Length ; j++)
			{
				if(touch[j] == PASS[j])
				{
					k++;
				}
				else
				{
					k=0;
				}
			}	
			if(k==PASS_Length)
			{
				pass=1;
			}
		}	
	}				
}





