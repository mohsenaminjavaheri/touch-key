

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
uint8_t v=10;
uint16_t f1=0;
uint16_t f2=0;
uint16_t f11=0;
uint16_t f22=0;
char num[32];
uint16_t regvalue;
uint16_t regvalue2;

uint8_t Exit=0;
uint8_t Enter=0;
uint8_t Enter1=0;
uint8_t Enter2=0;
uint8_t Enter3=0;
char men[32];
uint16_t num_menu=0;
uint16_t num_menu_1=0;
uint16_t ip_1=192;
uint16_t ip_1_1=192;
uint16_t ip_2=168;
uint16_t ip_2_2=168;
uint16_t ip_3=1;
uint16_t ip_3_3=1;
uint16_t ip_4=1;
uint16_t ip_4_4=1;

uint8_t Num_ofMenu=0;
uint8_t Num_ofSubMenu = 0;

extern uint16_t volume;
extern uint16_t volume1;
extern uint16_t light;
extern uint16_t light1;



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


uint8_t Hold(uint32_t v_input,uint8_t x, uint8_t y)
{
	uint16_t INC_PAD = Pad_2;
	
	uint16_t DEC_PAD = Pad_3;
	
	//uint16_t RESET_PAD = Pad_2;
	
	
	v = v_input;
//	read_register(TS1 , &value);
	if(value2 != 0)
		{
			regvalue2 = value2;
			
			while(value2 == regvalue2)
			{
				if(value2 == INC_PAD)
				{
					DelayMs(50);
					if(value2 == INC_PAD) 
					{
						v++;
						if(v>255) v=255;
					}
				}
				if(value2 == DEC_PAD)
				{
					DelayMs(50);
					if(value2 == DEC_PAD) 
					{
						v--;
						if(v==0) v=0;
					}
				}
				
				DelayMs(150);
				if(kk>=4)
				{
					if(value2 == INC_PAD)
					{
						if(v>255) v=255;
						else if(kk>=10) v+=10;
						else v++;						
					}
					else if(value2 == DEC_PAD)
					{
						if(v==0) v=0;
						else if(kk>=10) v-=10;
						else v++;	
					}
//					else if(value2 == RESET_PAD)
//					{
//						v=10;
//					}
					else
						break;
					
					sprintf(num,"%d",v);	

					//lcd16x2_clrscr();
					
					lcd16x2_gotoxy(x,y);
					
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
		
		DelayMs(100);
		
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

void Menu (void)
{
	lcd16x2_clrscr();
	while (Exit == 0)
	{
		num_menu_1 = Wheel(num_menu);
		sprintf(num,"n=%d  ",num_menu_1);
		lcd16x2_gotoxy(7,0);
		lcd16x2_puts(num);
		if(value2 == 1)
		{
			DelayMs(200);
			if(value2 == 1) 
			{
				Enter=1;
			}
		}
		switch (num_menu_1) 
		{
			case 0:
			case 1:
				sprintf(men,"Menu\n1-Volume");
				lcd16x2_gotoxy(0,0);
				lcd16x2_puts(men);
				
				while(Enter == 1)
				{
					sprintf(men,"\nVolume:%d  ",volume1);
					lcd16x2_gotoxy(0,0);
					lcd16x2_puts(men);
					volume1= Slider(volume);
					volume = volume1;
					if(value2 == 8)
					{
						DelayMs(200);
						if(value2 == 8) 
						{
							Enter=0;
							lcd16x2_clrscr();
						}
					}
				}
				break;
			
			case 2:
			case 3:
				sprintf(men,"Menu\n2-Light    ");
				lcd16x2_gotoxy(0,0);
				lcd16x2_puts(men);
				light1= Slider(light);
				light  = light1;
				while(Enter == 1)
				{
					sprintf(men,"\nLight:%d  ",light1);
					lcd16x2_gotoxy(0,0);
					lcd16x2_puts(men);
					light1= Slider(light);
					light  = light1;
					if(value2 == 8)
					{
						DelayMs(200);
						if(value2 == 8) 
						{
							Enter=0;
							lcd16x2_clrscr();
						}
					}
				}
				break;
				
			case 4:
			case 5:
				sprintf(men,"Menu\n3-Wifi Setting");
				lcd16x2_gotoxy(0,0);
				lcd16x2_puts(men);
				while(Enter == 1)
				{
					sprintf(men,"IP:%d       \n%3d.%3d.%3d.%3d",ip_1_1,ip_1_1,ip_2_2,ip_3_3,ip_4_4);
					lcd16x2_gotoxy(0,0);
					lcd16x2_puts(men);
					ip_1_1= Hold(ip_1,3,0);
					ip_1  = ip_1_1;
					if(value2 == 1)		Enter1=1;
					if(value2 == 8)		Enter=0;
						
						while(Enter1 ==1)
						{
							sprintf(men,"IP:%d   \n%3d.%3d.%3d.%3d",ip_2_2,ip_1_1,ip_2_2,ip_3_3,ip_4_4);
							lcd16x2_gotoxy(0,0);
							lcd16x2_puts(men);
							ip_2_2= Hold(ip_2,3,0);
							ip_2  = ip_2_2;
							if(value2 == 1) Enter2=1;
							if(value2 == 8)		Enter1=0;

						
							while(Enter2==1)
							{
								sprintf(men,"IP:%d   \n%3d.%3d.%3d.%3d",ip_3_3,ip_1_1,ip_2_2,ip_3_3,ip_4_4);
								lcd16x2_gotoxy(0,0);
								lcd16x2_puts(men);
								ip_3_3= Hold(ip_3,3,0);
								ip_3  = ip_3_3;
								if(value2 == 1) Enter3=1;
								if(value2 == 8)		Enter2=0;

								while(Enter3==1)
								{
									sprintf(men,"IP:%d   \n%3d.%3d.%3d.%3d",ip_4_4,ip_1_1,ip_2_2,ip_3_3,ip_4_4);
									lcd16x2_gotoxy(0,0);
									lcd16x2_puts(men);
									ip_4_4= Hold(ip_4,3,0);
									ip_4  = ip_4_4;
									if(value2 == 1)
									{
										DelayMs(100);
										if(value2 == 1) 
										{
											Enter=0;
											Enter1=0;
											Enter2=0;
											Enter3=0;
										}									
									}
									if(value2 == 8)	Enter3=0;
								}
							}
						}
					}
			}					
				
				

		if(value2 == 8)
		{
			DelayMs(100);
			if(value2 == 8) 
			{
				Exit=1;
				Enter=0;
			}
		}
		
	num_menu = num_menu_1;
	}
	num_menu=0;
	Exit=0;
	
}





int read_touchkey(uint16_t reg_touchkey)
{
	if(value2 == reg_touchkey)
	{
		while(value2 == reg_touchkey);
		return 1;
	}
	else
	{
		return 0;
	}
		
}



int State_inMenu(void)
{
	if(read_touchkey(8) == 1)
	{
		Num_ofMenu = 1;
		lcd16x2_clrscr();
	}
	
	while(Num_ofMenu == 1) 
	{
		num_menu_1 = Wheel(num_menu);
		sprintf(num,"n=%d  ",num_menu_1);
		lcd16x2_gotoxy(7,0);
		lcd16x2_puts(num);
		num_menu = num_menu_1;
		if(num_menu_1 == 2 )
		{
			Num_ofMenu = 2;
			lcd16x2_clrscr();
		}
		
		
		sprintf(men,"Menu\n1-Volume");
		lcd16x2_gotoxy(0,0);
		lcd16x2_puts(men);
		if(read_touchkey(8) == 1) 
		{
			Num_ofMenu = 0;
			lcd16x2_clrscr();
//			break;
		}
		if(read_touchkey(1) == 1)
		{
			lcd16x2_clrscr();
			Num_ofSubMenu = 1;
		}
		while(Num_ofSubMenu == 1)
		{
			sprintf(men,"    Volume:%d  ",volume1);
			lcd16x2_gotoxy(0,0);
			lcd16x2_puts(men);
			volume1= Slider(volume);
			volume = volume1;
			if(read_touchkey(8) == 1) 
			{
				Num_ofSubMenu = 0;
				lcd16x2_clrscr();
//				break;
			}
		}
	}
	
	
	
	while(Num_ofMenu == 2) 
	{
		num_menu_1 = Wheel(num_menu);
		sprintf(num,"n=%d  ",num_menu_1);
		lcd16x2_gotoxy(7,0);
		lcd16x2_puts(num);
		num_menu = num_menu_1;
		if(num_menu_1 == 0 )
		{
			Num_ofMenu = 1;
			lcd16x2_clrscr();
		}
//		else if(num_menu_1 == 4 )
//		{
//			Num_ofMenu = 3;
//			lcd16x2_clrscr();
//		}
		
		
		sprintf(men,"Menu\n2-Light");
		lcd16x2_gotoxy(0,0);
		lcd16x2_puts(men);
		if(read_touchkey(8) == 1) 
		{
			Num_ofMenu = 0;
			lcd16x2_clrscr();
		}
		if(read_touchkey(1) == 1)
		{
			lcd16x2_clrscr();
			Num_ofSubMenu = 1;
		}
		while(Num_ofSubMenu == 1)
		{
			sprintf(men,"    Light:%d  ",light1);
			lcd16x2_gotoxy(0,0);
			lcd16x2_puts(men);
			light1= Slider(light);
			light = light1;
			if(read_touchkey(8) == 1) 
			{
				Num_ofSubMenu = 0;
				lcd16x2_clrscr();
			}
		}
		
	}
	

	
	
}


