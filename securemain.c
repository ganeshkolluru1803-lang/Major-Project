//************************************************************
// File    : main.c
// Purpose : Implements an RFID-based door access system with
//           GSM OTP verification and RTC support.
//************************************************************

#include <stdio.h>
#include <lpc21xx.h>
#include <string.h>
#include "lcd.h"
#include "rtc.h"
#include "uart_interuppt.h"
#include "gsm.h"
#include "kpm.h"
#include "delay.h"

/* Authorized RFID tag */
char vaildtag[] = "12603355";

/* RTC variables */
s32 hour, min, sec;
s32 date, month, year, day;

/* OTP buffers */
char otp[5];
char enter_otp[5];

/* OTP timing variables */
s32 otp_time;
s32 now_time;

/* Invalid OTP attempt counter */
int attempts;

/* Function Prototypes */
void generate_otp(void);
int check_otp(void);
void send_otp(void);
int validate_otp(void);

int main()
{
    /* Configure door control pin as output */
    IODIR0 |= 1 << 12;

    /* Initialize peripherals */
    InitUART0();
    InitUART1();
    InitLCD();
    RTC_Init();
    gsm_init();
    InitKPM();
    enable_ent1();

    while(1)
    {
        /* Clear RFID receive flag */
        r_flag = 0;

        /* Continuously display RTC information until RFID is detected */
        do
        {
            GetRTCTimeInfo(&hour,&min,&sec);
            GetRTCDateInfo(&date,&month,&year);
            GetRTCDay(&day);

            DisplayRTCTime(hour,min,sec);
            DisplayRTCDate(date,month,year);
            DisplayRTCDay(day);

        } while(r_flag1 != 2);

        /* Display scanned RFID tag */
        CmdLCD(CLEAR_LCD);
        StrLCD("RFID TAG:");
        CmdLCD(GOTO_LINE2_POS0);
        StrLCD(buff1);

        delay_ms(2000);

        CmdLCD(CLEAR_LCD);

        /* Reset RFID flag */
        r_flag1 = 0;

        /* Compare scanned tag with authorized tag */
        if(strcmp(vaildtag,buff1) == 0)
        {
            CmdLCD(CLEAR_LCD);
            StrLCD("Valid Tag");

            delay_ms(1000);

            /* Store OTP generation time */
            GetRTCTimeInfo(&hour,&min,&sec);
            otp_time = (hour * 3600) + (min * 60) + sec;

            /* Generate and send OTP */
            generate_otp();
            send_otp();

            attempts = 0;

            /* Allow maximum three OTP attempts */
            while(attempts < 3)
            {
                /* Validate entered OTP */
                if(validate_otp())
                {
                    /* Check OTP validity period */
                    if(check_otp())
                    {
                        CmdLCD(CLEAR_LCD);
                        StrLCD("Access Granted");

                        delay_ms(500);

                        /* Open door */
                        CmdLCD(CLEAR_LCD);
                        StrLCD("Door Open");

                        IOSET0 = 1 << 12;

                        delay_ms(500);

                        /* Close door */
                        CmdLCD(CLEAR_LCD);
                        StrLCD("Door Close");

                        IOCLR0 = 1 << 12;

                        delay_ms(2000);

                        break;
                    }
                    else
                    {
                        CmdLCD(CLEAR_LCD);
                        StrLCD("OTP EXPIRED");

                        delay_ms(1000);

                        break;
                    }
                }
                else
                {
                    attempts++;

                    CmdLCD(CLEAR_LCD);
                    StrLCD("WRONG OTP");

                    delay_ms(500);

                    /* Deny access after three failed attempts */
                    if(attempts >= 3)
                    {
                        CmdLCD(CLEAR_LCD);
                        StrLCD("ACCESS DENIED");

                        delay_ms(2000);
                    }
                }
            }
        }
        else
        {
            /* Invalid RFID tag detected */
            CmdLCD(CLEAR_LCD);
            StrLCD("Invalid Card");

            delay_ms(500);
        }

        CmdLCD(CLEAR_LCD);
    }
}

/*-----------------------------------------------------------
 * Function : generate_otp()
 * Purpose  : Generates a 4-digit OTP using the current
 *            RTC time.
 *----------------------------------------------------------*/
void generate_otp(void)
{
    otp[0] = (hour % 10) + '0';
    otp[1] = (min % 10) + '0';
    otp[2] = (sec % 10) + '0';
    otp[3] = ((hour + min + sec) % 10) + '0';
    otp[4] = '\0';
}

/*-----------------------------------------------------------
 * Function : send_otp()
 * Purpose  : Sends the generated OTP via GSM SMS.
 *----------------------------------------------------------*/
void send_otp(void)
{
    if(gsm_flag == 1)
    {
        UART0_Str("AT+CMGS=\"8096261977\"\r\n");

        delay_ms(1000);

        UART0_Str("YOUR OTP IS:");
        UART0_Str(otp);
        UART0_Str("Valid for 3 mins");

        UART0_Tx(0x1A);

        delay_ms(1000);
    }
}

/*-----------------------------------------------------------
 * Function : validate_otp()
 * Purpose  : Reads OTP from keypad and compares it with
 *            the generated OTP.
 * Returns  : 1 -> Valid OTP
 *            0 -> Invalid OTP
 *----------------------------------------------------------*/
int validate_otp(void)
{
    int i;
    u8 key;

    CmdLCD(CLEAR_LCD);
    CmdLCD(GOTO_LINE1_POS0);
    StrLCD("ENTER OTP");

    CmdLCD(GOTO_LINE2_POS0);

    i = 0;

    while(1)
    {
        if(ColScan() == 1)
        {
            key = KeyScan();

            /* Backspace key */
            if(key == 10)
            {
                if(i > 0)
                {
                    i--;

                    enter_otp[i] = '\0';

                    CmdLCD(0xC0 + i);
                    CharLCD(' ');
                    CmdLCD(0xC0 + i);
                }
            }

            /* Enter key */
            else if(key == 16)
            {
                if(i == 4)
                    break;
            }

            /* Numeric key */
            else
            {
                if(i < 4)
                {
                    enter_otp[i] = key + '0';
                    CharLCD(key + '0');
                    i++;
                }
            }
        }
    }

    enter_otp[4] = '\0';

    return (strcmp(otp, enter_otp) == 0);
}

/*-----------------------------------------------------------
 * Function : check_otp()
 * Purpose  : Verifies whether the OTP has expired.
 * Returns  : 1 -> OTP Valid
 *            0 -> OTP Expired
 *----------------------------------------------------------*/
int check_otp()
{
    GetRTCTimeInfo(&hour,&min,&sec);

    now_time = (hour * 3600) + (min * 60) + sec;

    /* OTP validity period: 60 seconds */
    if((now_time - otp_time) > 60)
    {
        return 0;
    }

    return 1;
}
