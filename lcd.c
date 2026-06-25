//************************************************************
// File    : lcd.c
// Purpose : Provides LCD driver functions for initialization,
//           command execution, data display, and number
//           conversion on a 16x2 LCD.
//************************************************************

#include "DEFINES.H"
#include "types.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "delay.h"

/*-----------------------------------------------------------
 * Function : WriteLCD()
 * Purpose  : Writes a byte to the LCD.
 *----------------------------------------------------------*/
void WriteLCD(u8 data)
{
    /* Select write operation */
    SCLRBIT(IOCLR0,RW);

    /* Place data on LCD data pins */
    WRITEBYTE(IOPIN0,LCD_DATA,data);

    /* Generate enable pulse */
    SSETBIT(IOSET0,EN);
    delay_us(1);
    SCLRBIT(IOCLR0,EN);

    /* Wait for LCD processing */
    delay_ms(2);
}

/*-----------------------------------------------------------
 * Function : CmdLCD()
 * Purpose  : Sends a command to the LCD.
 *----------------------------------------------------------*/
void CmdLCD(u8 cmd)
{
    /* Select command register */
    SCLRBIT(IOCLR0,RS);

    /* Write command */
    WriteLCD(cmd);
}

/*-----------------------------------------------------------
 * Function : CharLCD()
 * Purpose  : Displays a single character on the LCD.
 *----------------------------------------------------------*/
void CharLCD(u8 ascii)
{
    /* Select data register */
    SSETBIT(IOSET0,RS);

    /* Write character */
    WriteLCD(ascii);
}

/*-----------------------------------------------------------
 * Function : InitLCD()
 * Purpose  : Initializes the LCD in 8-bit, 2-line mode.
 *----------------------------------------------------------*/
void InitLCD(void)
{
    /* Configure LCD data pins as outputs */
    WRITEBYTE(IODIR0,LCD_DATA,0xFF);

    /* Configure LCD control pins as outputs */
    SETBIT(IODIR0,RS);
    SETBIT(IODIR0,RW);
    SETBIT(IODIR0,EN);

    /* LCD initialization sequence */
    delay_ms(15);

    CmdLCD(MODE_8BIT_1LINE);
    delay_ms(5);

    CmdLCD(MODE_8BIT_1LINE);
    delay_us(100);

    CmdLCD(MODE_8BIT_1LINE);

    /* Configure display settings */
    CmdLCD(MODE_8BIT_2LINE);
    CmdLCD(DISP_ON_CUR_OFF);
    CmdLCD(CLEAR_LCD);
    CmdLCD(SHIFT_CUR_RIGHT);
}

/*-----------------------------------------------------------
 * Function : StrLCD()
 * Purpose  : Displays a string on the LCD.
 *----------------------------------------------------------*/
void StrLCD(u8 *str)
{
    while(*str)
    {
        CharLCD(*str++);
    }
}

/*-----------------------------------------------------------
 * Function : U32LCD()
 * Purpose  : Displays an unsigned 32-bit integer.
 *----------------------------------------------------------*/
void U32LCD(u32 n)
{
    u8 a[10];
    s32 i = 0;

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        /* Convert number into ASCII digits */
        while(n)
        {
            a[i++] = (n % 10) + '0';
            n /= 10;
        }

        /* Display digits in correct order */
        for(--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}

/*-----------------------------------------------------------
 * Function : S32LCD()
 * Purpose  : Displays a signed 32-bit integer.
 *----------------------------------------------------------*/
void S32LCD(s32 n)
{
    /* Display negative sign if required */
    if(n < 0)
    {
        n = -n;
        CharLCD('-');
        U32LCD(n);
    }
}

/*-----------------------------------------------------------
 * Function : BinLCD()
 * Purpose  : Displays a number in binary format.
 *----------------------------------------------------------*/
void BinLCD(u32 n, u8 nbd)
{
    s32 i;

    for(i = nbd - 1; i >= 0; i--)
    {
        CharLCD(((n >> i) & 1) + '0');
    }
}

/*-----------------------------------------------------------
 * Function : BuildCGRAM()
 * Purpose  : Loads custom characters into LCD CGRAM.
 *----------------------------------------------------------*/
void BuildCGRAM(u8 *p, u8 nBytes)
{
    s32 i;

    /* Select CGRAM */
    CmdLCD(GOTO_CGRAM);

    /* Write custom character data */
    for(i = 0; i < nBytes; i++)
    {
        CharLCD(p[i]);
    }

    /* Return to DDRAM */
    CmdLCD(GOTO_LINE2_POS0);
}

/*-----------------------------------------------------------
 * Function : f32LCD()
 * Purpose  : Displays a floating-point number.
 *----------------------------------------------------------*/
void f32LCD(f32 fnum, u8 ndp)
{
    u32 inum;

    /* Handle negative numbers */
    if(fnum < 0.0)
    {
        CharLCD('-');
        fnum = -fnum;
    }

    /* Display integer part */
    inum = fnum;
    U32LCD(inum);

    /* Display decimal point */
    CharLCD('.');

    /* Display fractional part */
    while(ndp)
    {
        fnum = (fnum - inum) * 10;
        inum = fnum;

        CharLCD(inum + '0');

        ndp--;
    }
}

/*-----------------------------------------------------------
 * Function : HexLCD()
 * Purpose  : Displays a number in hexadecimal format.
 *----------------------------------------------------------*/
void HexLCD(u32 n)
{
    u8 a[8], rem;
    s32 i = 0;

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        /* Convert decimal to hexadecimal */
        while(n)
        {
            rem = n % 16;
            (rem < 10) ? (rem += '0') : (rem += 55);

            a[i++] = rem;
            n /= 16;
        }

        /* Display hexadecimal digits */
        for(--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}

/*-----------------------------------------------------------
 * Function : OctLCD()
 * Purpose  : Displays a number in octal format.
 *----------------------------------------------------------*/
void OctLCD(u32 n)
{
    s32 i = 0;
    u8 a[12];

    if(n == 0)
    {
        CharLCD('0');
    }
    else
    {
        /* Convert decimal to octal */
        while(n)
        {
            a[i++] = (n % 8) + '0';
            n /= 8;
        }

        /* Display octal digits */
        for(--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}
