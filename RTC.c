

#include <stdio.h>

#include "rtc.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_rtc.h"

#include "delay.h"
#include "lcd16_2.h"

char s_time[64];

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void RTC_config (void)
{
	if (BKP_ReadBackupRegister(BKP_DR2) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    sprintf(s_time,"RTC not yet config");
		
		lcd16x2_clrscr();		
		lcd16x2_gotoxy(0,0);
		lcd16x2_puts(s_time);

    /* RTC Configuration */
    RTC_Configuration();
		
		DelayMs(500);

    sprintf(s_time,"RTC configured....");
		
		lcd16x2_clrscr();		
		lcd16x2_gotoxy(0,0);
		lcd16x2_puts(s_time);
		
		DelayMs(500);

    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust(12,46,00);

    BKP_WriteBackupRegister(BKP_DR2, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      sprintf(s_time,"Power On Reset occurred....");
			
			lcd16x2_clrscr();		
			lcd16x2_gotoxy(0,0);
			lcd16x2_puts(s_time);
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      sprintf(s_time,"External Reset occurred....");
			
			lcd16x2_clrscr();		
			lcd16x2_gotoxy(0,0);
			lcd16x2_puts(s_time);
			
			DelayMs(500);
    }

    sprintf(s_time,"No need to configure RTC....");
		
		lcd16x2_clrscr();		
		lcd16x2_gotoxy(0,0);
		lcd16x2_puts(s_time);
		
		DelayMs(500);
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }

	#ifdef RTCClockOutput_Enable
		/* Enable PWR and BKP clocks */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

		/* Allow access to BKP Domain */
		PWR_BackupAccessCmd(ENABLE);

		/* Disable the Tamper Pin */
		BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
																	 functionality must be disabled */

		/* Enable RTC Clock Output on Tamper Pin */
		BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif

  /* Clear reset flags */
  RCC_ClearFlag();

  /* Display time in infinite loop */
//  Time_Show();

	lcd16x2_clrscr();
	
}
	




void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
//	RCC_LSICmd(ENABLE);
//	RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_ALR, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}


void Time_Adjust(uint32_t Tmp_HH ,uint32_t Tmp_MM,uint32_t Tmp_SS)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
	uint32_t Time_Regulate;
	
	Time_Regulate = Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS;
	
  RTC_SetCounter(Time_Regulate);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void Alarm_Adjust(uint32_t Alm_HH ,uint32_t Alm_MM,uint32_t Alm_SS)
{
	/* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
	uint32_t Time_Regulate;
	
	Time_Regulate = Alm_HH*3600 + Alm_MM*60 + Alm_SS;
	
  RTC_SetAlarm(Time_Regulate);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;
  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  sprintf(s_time,"Time:%0.2d:%0.2d:%0.2d", THH, TMM, TSS);
	
//	lcd16x2_clrscr();		
	lcd16x2_gotoxy(0,0);
	lcd16x2_puts(s_time);

}

