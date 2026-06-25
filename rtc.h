#ifndef _RTC_H
#define _RTC_H

#include "types.h"

/* Initialize the RTC module */
void RTC_Init(void);

/* Read current RTC time */
void GetRTCTimeInfo(s32 *, s32 *, s32 *);

/* Display time on the LCD */
void DisplayRTCTime(u32, u32, u32);

/* Read current RTC date */
void GetRTCDateInfo(s32 *, s32 *, s32 *);

/* Display date on the LCD */
void DisplayRTCDate(u32, u32, u32);

/* Set RTC time */
void SetRTCTimeInfo(u32, u32, u32);

/* Set RTC date */
void SetRTCDateInfo(u32, u32, u32);

/* Read current day of the week */
void GetRTCDay(s32 *);

/* Display day of the week */
void DisplayRTCDay(u32);

/* Set day of the week */
void SetRTCDay(u32);

/* Set hour */
void sethour(u32 hour);

/* Set minute */
void setminute(u32 minute);

/* Set second */
void setsecond(u32 second);

/* Set date */
void setdate(u32 date);

/* Set month */
void setmonth(u32 month);

/* Set year */
void setyear(u32 year);

/* Set day of the week */
void setweek(u32 week);

#endif
