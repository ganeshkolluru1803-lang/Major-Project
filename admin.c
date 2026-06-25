#include <stdlib.h>
#include "lcd_defines.h"
#include "lcd.h"
#include "kpm.h"
#include "rtc.h"
#include "delay.h"

/*------------------------------------------------------------
 * Function : rtc_edit()
 * Purpose  : Displays the RTC edit menu and allows the user
 *            to modify the current date and time parameters.
 *-----------------------------------------------------------*/
void rtc_edit()
{
    u8 key1;
    u8 hour;
    u8 minute;
    u8 second;
    u8 date;
    u8 month;
    u32 year;
    u8 week;

    while(1)
    {
        /* Display RTC editing options */
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1.H 2.MI 3.S 4.D");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD("5.M 6.YR 7.W 8.E");

        /* Read user selection from keypad */
        key1 = KeyScan();

        switch(key1)
        {
            /* Edit hour (24-hour format) */
            case 1:
                CmdLCD(CLEAR_LCD);
                StrLCD("Enter Hour(0-23)");

                /* Read hour value */
                hour = ReadNumber(23);

                /* Update RTC if the value is valid */
                if(hour >= 0 && hour <= 23)
                    sethour(hour);

                delay_ms(1000);
                break;

            /* Edit minute */
            case 2:
                CmdLCD(CLEAR_LCD);
                StrLCD("MIN EDIT(0-59)");

                /* Read minute value */
                minute = ReadNumber(59);

                /* Update RTC minute */
                setminute(minute);

                delay_ms(1000);
                break;

            /* Edit second */
            case 3:
                CmdLCD(CLEAR_LCD);
                StrLCD("SEC EDIT(0-59)");

                /* Read second value */
                second = ReadNumber(59);

                /* Update RTC second */
                setsecond(second);

                delay_ms(1000);
                break;

            /* Edit date */
            case 4:
                CmdLCD(CLEAR_LCD);
                StrLCD("DATE EDIT(1-31)");

                /* Read date value */
                date = ReadNumber(31);

                /* Update RTC date */
                setdate(date);

                delay_ms(1000);
                break;

            /* Edit month */
            case 5:
                CmdLCD(CLEAR_LCD);
                StrLCD("MONTH EDIT(1-12)");

                /* Read month value */
                month = ReadNumber(12);

                /* Update RTC month */
                setmonth(month);

                delay_ms(1000);
                break;

            /* Edit year */
            case 6:
                CmdLCD(CLEAR_LCD);
                StrLCD("YEAR EDIT");

                /* Read four-digit year */
                year = Read4Number(4095);

                /* Update RTC year */
                setyear(year);

                delay_ms(1000);
                break;

            /* Edit day of the week */
            case 7:
                CmdLCD(CLEAR_LCD);
                StrLCD("WEEK EDIT(0-6)");

                /* Read weekday value */
                week = ReadNumber(6);

                /* Update RTC weekday */
                SetRTCDay(week);

                delay_ms(1000);
                break;

            /* Exit RTC edit menu */
            case 8:
                CmdLCD(CLEAR_LCD);
                return;

            /* Handle invalid keypad input */
            default:
                CmdLCD(CLEAR_LCD);
                StrLCD("INVALID CHOICE");
                delay_ms(1000);
        }
    }
}


/*------------------------------------------------------------
 * Function : adminmode()
 * Purpose  : Displays the administrator menu and provides
 *            access to RTC configuration settings.
 *-----------------------------------------------------------*/
void adminmode()
{
    u8 key;

    while(1)
    {
        /* Display administrator menu */
        CmdLCD(CLEAR_LCD);
        CmdLCD(GOTO_LINE1_POS0);
        StrLCD("1.RTC 2.EXIT");

        delay_ms(100);

        /* Read administrator choice */
        key = KeyScan();

        switch(key)
        {
            /* Open RTC editing menu */
            case 1:
                CmdLCD(CLEAR_LCD);
                StrLCD("RTC EDIT");
                delay_ms(200);

                rtc_edit();
                break;

            /*
            // Enable when Event Edit is implemented
            case 2:
                CmdLCD(CLEAR_LCD);
                StrLCD("EVENT EDIT");
                delay_ms(200);
                event_edit();
                break;
            */

            /* Exit administrator mode */
            case 2:
                CmdLCD(CLEAR_LCD);
                delay_ms(200);
                return;

            /* Display error for invalid selection */
            default:
                CmdLCD(CLEAR_LCD);
                StrLCD("INVALID CHOICE");
                delay_ms(1000);
        }
    }
}
