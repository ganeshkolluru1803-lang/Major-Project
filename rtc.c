//************************************************************
// File    : rtc.c
// Purpose : Provides RTC initialization, configuration,
//           and display functions for date and time.
//************************************************************

#include <lpc214x.h>
#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "rtc.h"

/* System clock configuration */
#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)

/* RTC prescaler values */
#define PREINT_VAL  (int)((PCLK/32768)-1)
#define PREFRAC_VAL (PCLK-((PREINT_VAL+1)*32768))

/* RTC Control Register bits */
#define RTC_ENABLE (1<<0)
#define RTC_RESET  (1<<1)
#define RTC_CLKSRC (1<<4)

/* Day of week definitions */
#define SUN 0
#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6

#define _LPC2148

/*-----------------------------------------------------------
 * Function : RTC_Init()
 * Purpose  : Initializes and enables the Real-Time Clock.
 *----------------------------------------------------------*/
void RTC_Init(void)
{
    /* Reset RTC */
    CCR = RTC_RESET;

#ifndef _LPC2148
    /* Configure RTC prescaler */
    PREINT = PREINT_VAL;
    PREFRAC = PREFRAC_VAL;

    /* Enable RTC */
    CCR = RTC_ENABLE;

#else
    /* Enable RTC using external clock source */
    CCR = RTC_ENABLE | RTC_CLKSRC;
#endif
}

/* Day names */
char week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

/*-----------------------------------------------------------
 * Function : GetRTCTimeInfo()
 * Purpose  : Reads the current RTC time.
 *----------------------------------------------------------*/
void GetRTCTimeInfo(s32 *hour,s32 *min,s32 *sec)
{
    *hour = HOUR;
    *min  = MIN;
    *sec  = SEC;
}

/*-----------------------------------------------------------
 * Function : DisplayRTCTime()
 * Purpose  : Displays the current time on the LCD.
 *----------------------------------------------------------*/
void DisplayRTCTime(u32 hour,u32 min,u32 sec)
{
    CmdLCD(GOTO_LINE1_POS0);

    CharLCD(hour/10+'0');
    CharLCD(hour%10+'0');
    CharLCD(':');

    CharLCD(min/10+'0');
    CharLCD(min%10+'0');
    CharLCD(':');

    CharLCD(sec/10+'0');
    CharLCD(sec%10+'0');
}

/*-----------------------------------------------------------
 * Function : GetRTCDateInfo()
 * Purpose  : Reads the current RTC date.
 *----------------------------------------------------------*/
void GetRTCDateInfo(s32 *date,s32 *month,s32 *year)
{
    *date  = DOM;
    *month = MONTH;
    *year  = YEAR;
}

/*-----------------------------------------------------------
 * Function : DisplayRTCDate()
 * Purpose  : Displays the current date on the LCD.
 *----------------------------------------------------------*/
void DisplayRTCDate(u32 date,u32 month,u32 year)
{
    CmdLCD(GOTO_LINE2_POS0);

    CharLCD(date/10+'0');
    CharLCD(date%10+'0');
    CharLCD('/');

    CharLCD(month/10+'0');
    CharLCD(month%10+'0');
    CharLCD('/');

    U32LCD(year);
}

/*-----------------------------------------------------------
 * Function : SetRTCTimeInfo()
 * Purpose  : Updates RTC time registers.
 *----------------------------------------------------------*/
void SetRTCTimeInfo(u32 hour,u32 min,u32 sec)
{
    HOUR = hour;
    MIN  = min;
    SEC  = sec;
}

/*-----------------------------------------------------------
 * Function : SetRTCDateInfo()
 * Purpose  : Updates RTC date registers.
 *----------------------------------------------------------*/
void SetRTCDateInfo(u32 date,u32 month,u32 year)
{
    DOM   = date;
    MONTH = month;
    YEAR  = year;
}

/*-----------------------------------------------------------
 * Function : GetRTCDay()
 * Purpose  : Reads the current day of the week.
 *----------------------------------------------------------*/
void GetRTCDay(s32 *dow)
{
    *dow = DOW;
}

/*-----------------------------------------------------------
 * Function : DisplayRTCDay()
 * Purpose  : Displays the weekday on the LCD.
 *----------------------------------------------------------*/
void DisplayRTCDay(u32 dow)
{
    CmdLCD(GOTO_LINE1_POS0 + 10);
    StrLCD(week[dow]);
}

/*-----------------------------------------------------------
 * Function : SetRTCDay()
 * Purpose  : Updates the day-of-week register.
 *----------------------------------------------------------*/
void SetRTCDay(u32 dow)
{
    DOW = dow;
}

/*-----------------------------------------------------------
 * Function : sethour()
 * Purpose  : Updates the RTC hour register.
 *----------------------------------------------------------*/
void sethour(u32 hour)
{
    HOUR = hour;
}

/*-----------------------------------------------------------
 * Function : setminute()
 * Purpose  : Updates the RTC minute register.
 *----------------------------------------------------------*/
void setminute(u32 minute)
{
    MIN = minute;
}

/*-----------------------------------------------------------
 * Function : setsecond()
 * Purpose  : Updates the RTC second register.
 *----------------------------------------------------------*/
void setsecond(u32 second)
{
    SEC = second;
}

/*-----------------------------------------------------------
 * Function : setdate()
 * Purpose  : Updates the RTC date register.
 *----------------------------------------------------------*/
void setdate(u32 date)
{
    DOM = date;
}

/*-----------------------------------------------------------
 * Function : setmonth()
 * Purpose  : Updates the RTC month register.
 *----------------------------------------------------------*/
void setmonth(u32 month)
{
    MONTH = month;
}

/*-----------------------------------------------------------
 * Function : setweek()
 * Purpose  : Updates the RTC day-of-week register.
 *----------------------------------------------------------*/
void setweek(u32 week)
{
    DOW = week;
}

/*-----------------------------------------------------------
 * Function : setyear()
 * Purpose  : Updates the RTC year register.
 *----------------------------------------------------------*/
void setyear(u32 year)
{
    YEAR = year;
}
