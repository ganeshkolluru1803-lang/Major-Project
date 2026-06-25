#include<stdio.h>
#include<stdlib.h>
#include"delay.h" 
#include "uart_interuppt.h"
#include"lcd.h"
#include"lcd_defines.h"
#include <string.h>
extern char buff[200];
extern unsigned char i;
u32 cnt;
int gsm_flag;
void gsm_init(void)
{

    CmdLCD(0x01);

	CmdLCD(0x80);

	StrLCD("AT");

	delay_ms(100);

	UART0_Str("AT\r\n");

	i=0;memset(buff,'\0',200);

	while(cnt<5000)
	{
	if(strstr(buff,"OK"))
	break;
	if (i>0)
	break;
	delay_ms(1);
	cnt++;
	}

	delay_ms(500);

	buff[i] = '\0';

	CmdLCD(0x01);

	CmdLCD(0x80);

	StrLCD(buff);
	delay_ms(2000);
	if(cnt>=5000)
	{
	 StrLCD("Gsm not connected");
	 gsm_flag=0;
	 delay_ms(500);
	 CmdLCD(0x01);
	 return;
	}

	if(strstr(buff,"OK"))

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

    CmdLCD(0x01);

	CmdLCD(0x80);

    StrLCD("ATE0");

	delay_ms(1000);

	UART0_Str("ATE0\r\n");

	i=0;memset(buff,'\0',200);

	while(cnt<5000)
	{
	if(strstr(buff,"OK"))
	break;
	if (i>0)
	break;
	delay_ms(1);
	cnt++;
	}

	delay_ms(500);

	buff[i] = '\0';

    CmdLCD(0x02);

	CmdLCD(0x80);

	StrLCD(buff);

	delay_ms(2000);
	if(cnt>=5000)
	{
	 StrLCD("Gsm not connected");
	  gsm_flag=0;
	 delay_ms(500);
	 CmdLCD(0x01);
	 return;
	}


	if(strstr(buff,"OK"))

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

    CmdLCD(0x01);

	CmdLCD(0x80);

    StrLCD("AT+CMGF=1");

	delay_ms(1000);

	UART0_Str("AT+CMGF=1\r\n");

	i=0;memset(buff,'\0',200);

	while(cnt<5000)
	{
	if(strstr(buff,"OK"))
	break;
	if (i>0)
	break;
	delay_ms(1);
	cnt++;
	}

	delay_ms(500);

	buff[i] = '\0';

    CmdLCD(0x01);

	CmdLCD(0x80);

	StrLCD(buff);

	delay_ms(2000);
	if(cnt>=5000)
	{
	 StrLCD("Gsm not connected");
	  gsm_flag=0;
	 delay_ms(500);
	 CmdLCD(0x01);
	 return;
	}

	if(strstr(buff,"OK"))

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
	 gsm_flag=1;
}
 

