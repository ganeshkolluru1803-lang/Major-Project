#include <lpc214x.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "rtc.h"

#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

#define PREINT_VAL (int)((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

#define RTC_ENABLE (1<<0)
#define RTC_RESET  (1<<1)
#define RTC_CLKSRC (1<<4)

#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define _LPC2148
void RTC_Init(void)
{
      CCR = RTC_RESET;
     #ifndef _LPC2148
     PREINT = PREINT_VAL;
     PREFRAC = PREFRAC_VAL;
      CCR = RTC_ENABLE;
        #else
         CCR = RTC_ENABLE  | RTC_CLKSRC;
        #endif
}

char week[][4]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
void GetRTCTimeInfo(s32 *hour,s32 *min,s32 *sec)
{
    *hour = HOUR;
    *min = MIN;
    *sec = SEC;
}

void DisplayRTCTime(u32 hour,u32 min,u32 sec)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD(hour/10+48);
    CharLCD(hour%10+48);
    CharLCD(':');

    CharLCD(min/10+48);
    CharLCD(min%10+48);
    CharLCD(':');

    CharLCD(sec/10+48);
    CharLCD(sec%10+48);
}

void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)
{
    *date = DOM;
    *month = MONTH;
    *year = YEAR;
}

void DisplayRTCDate(u32 date,u32 month,u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);

    CharLCD(date/10+48);
    CharLCD(date%10+48);
    CharLCD('/');
    CharLCD(month/10+48);
    CharLCD(month%10+48);
    CharLCD('/');

    U32LCD(year);
}

void SetRTCTimeInfo(u32 hour,u32 min,u32 sec)
{
    HOUR = hour;
    MIN = min;
    SEC = sec;
}

void SetRTCDateInfo(u32 date,u32 month,u32 year)
{
    DOM = date;
    MONTH = month;
    YEAR = year;
}

void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}

void DisplayRTCDay(u32 dow)
{
    CmdLCD(GOTO_LINE1_POS0+10);
    StrLCD(week[dow]);
}

void SetRTCDay(u32 dow)
{
    DOW = dow;
}

void sethour(u32 hour)
{
	HOUR=hour;
}

void setminute(u32 minute)
{
	MIN=minute;
}
void setsecond(u32 second)
{
	SEC=second;
}
void setdate(u32 date)
{
	DOM=date;
}
void setmonth(u32 month)
{
	MONTH=month;
}
void setweek(u32 week)
{
   DOW=week;
}	
void setyear(u32 year)
{
YEAR=year;
}



