#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "uart_interuppt.h"
#include "lcd.h"
#include "lcd_defines.h"
#include <string.h>

/* UART receive buffer and index */
extern char buff[200];
extern unsigned char i;

/* Timeout counter and GSM status flag */
u32 cnt;
int gsm_flag;

/*-----------------------------------------------------------
 * Function : gsm_init()
 * Purpose  : Initializes the GSM module by sending the
 *            required AT commands and verifying the
 *            responses from the module.
 *----------------------------------------------------------*/
void gsm_init(void)
{
    /* Clear LCD and send basic AT command */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("AT");

    delay_ms(100);

    /* Check GSM module communication */
    UART0_Str("AT\r\n");

    /* Clear receive buffer before waiting for response */
    i = 0;
    memset(buff, '\0', 200);

    /* Wait until "OK" is received or timeout occurs */
    while(cnt < 5000)
    {
        if(strstr(buff, "OK"))
            break;

        if(i > 0)
            break;

        delay_ms(1);
        cnt++;
    }

    delay_ms(500);

    buff[i] = '\0';

    /* Display received response */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD(buff);

    delay_ms(2000);

    /* Exit if GSM module is not responding */
    if(cnt >= 5000)
    {
        StrLCD("Gsm not connected");
        gsm_flag = 0;
        delay_ms(500);
        CmdLCD(0x01);
        return;
    }

    /* Verify successful communication */
    if(strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    /* Disable command echo */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("ATE0");

    delay_ms(1000);

    UART0_Str("ATE0\r\n");

    /* Clear receive buffer */
    i = 0;
    memset(buff, '\0', 200);

    /* Wait for acknowledgement */
    while(cnt < 5000)
    {
        if(strstr(buff, "OK"))
            break;

        if(i > 0)
            break;

        delay_ms(1);
        cnt++;
    }

    delay_ms(500);

    buff[i] = '\0';

    /* Display GSM response */
    CmdLCD(0x02);
    CmdLCD(0x80);
    StrLCD(buff);

    delay_ms(2000);

    /* Check communication timeout */
    if(cnt >= 5000)
    {
        StrLCD("Gsm not connected");
        gsm_flag = 0;
        delay_ms(500);
        CmdLCD(0x01);
        return;
    }

    /* Verify echo is disabled */
    if(strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    /* Configure GSM module for SMS Text Mode */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("AT+CMGF=1");

    delay_ms(1000);

    UART0_Str("AT+CMGF=1\r\n");

    /* Clear receive buffer */
    i = 0;
    memset(buff, '\0', 200);

    /* Wait for acknowledgement */
    while(cnt < 5000)
    {
        if(strstr(buff, "OK"))
            break;

        if(i > 0)
            break;

        delay_ms(1);
        cnt++;
    }

    delay_ms(500);

    buff[i] = '\0';

    /* Display GSM response */
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD(buff);

    delay_ms(2000);

    /* Check communication timeout */
    if(cnt >= 5000)
    {
        StrLCD("Gsm not connected");
        gsm_flag = 0;
        delay_ms(500);
        CmdLCD(0x01);
        return;
    }

    /* Verify SMS text mode configuration */
    if(strstr(buff, "OK"))
    {
        CmdLCD(0xC0);
        StrLCD("OK");
        delay_ms(1000);
    }
    else
    {
        CmdLCD(0xC0);
        StrLCD("ERROR");
        delay_ms(1000);
        return;
    }

    /* GSM initialization completed successfully */
    gsm_flag = 1;
}
