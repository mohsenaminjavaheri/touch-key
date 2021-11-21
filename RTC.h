

#ifndef __RTC_H
#define __RTC_H


#include <stdint.h>



void RTC_config (void);
void RTC_Configuration(void);
void Time_Display(uint32_t TimeVar);
void NVIC_Configuration(void);
void Alarm_Adjust(uint32_t Alm_HH ,uint32_t Alm_MM,uint32_t Alm_SS);
void Time_Adjust(uint32_t Tmp_HH ,uint32_t Tmp_MM,uint32_t Tmp_SS);



#endif

