//************************************************************
// File    : KPM.c
// Purpose : Implements 4x4 Matrix Keypad interfacing,
//           key scanning and numeric input functions.
//************************************************************

#include "defines.h"
#include "lcd_defines.h"
#include "delay.h"
#include <lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "kpm.h"

/* Lookup table to map row and column indices to keypad values */
u8 KpmLUT[4][4] =
{
    {1,2,3,'+'},
    {4,5,6,'-'},
    {7,8,9,'*'},
    {10,0,15,16}
};

/*
u8 KpmLUT[4][4]=
{
    {'1','2','3','+'},
    {'4','5','6','-'},
    {'7','8','9','*'},
    {'C','0','=','/'}
};
*/

/*-----------------------------------------------------------
 * Function : InitKPM()
 * Purpose  : Configures keypad row pins as outputs.
 *----------------------------------------------------------*/
void InitKPM(void)
{
    /* Configure P1.16-P1.19 as output pins */
    WRITENIBBLE(IODIR1,ROW0,15);
}

/*-----------------------------------------------------------
 * Function : ColScan()
 * Purpose  : Checks whether any key is pressed.
 * Returns  : 0 -> Key Pressed
 *            1 -> No Key Pressed
 *----------------------------------------------------------*/
u8 ColScan(void)
{
    if((READNIBBLE(IOPIN1,COL0)) < 15)
        return 0;
    else
        return 1;
}

/*-----------------------------------------------------------
 * Function : RowCheck()
 * Purpose  : Identifies the row containing the pressed key.
 * Returns  : Row number (0-3)
 *----------------------------------------------------------*/
u8 RowCheck(void)
{
    u8 rno;

    for(rno = 0; rno <= 3; rno++)
    {
        /* Drive one row LOW at a time */
        WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));

        if(ColScan() == 0)
        {
            break;
        }
    }

    /* Restore all rows */
    WRITENIBBLE(IOPIN1,ROW0,0x0);

    return rno;
}

/*-----------------------------------------------------------
 * Function : ColCheck()
 * Purpose  : Identifies the column containing the pressed key.
 * Returns  : Column number (0-3)
 *----------------------------------------------------------*/
u8 ColCheck(void)
{
    u8 cno;

    for(cno = 0; cno <= 3; cno++)
    {
        if(STATUSBIT(IOPIN1,(COL0+cno)) == 0)
        {
            break;
        }
    }

    return cno;
}

/*-----------------------------------------------------------
 * Function : KeyScan()
 * Purpose  : Detects a key press and returns the
 *            corresponding keypad value.
 *----------------------------------------------------------*/
u8 KeyScan(void)
{
    u8 keyv, rno, cno;

    /* Wait until a key is pressed */
    while(ColScan());

    /* Determine row and column */
    rno = RowCheck();
    cno = ColCheck();

    /* Retrieve key value from lookup table */
    keyv = KpmLUT[rno][cno];

    /* Wait until key is released */
    while(!ColScan());

    return keyv;
}

/*-----------------------------------------------------------
 * Function : ReadNumber()
 * Purpose  : Reads a maximum two-digit number from keypad.
 * Arguments: max - Maximum allowed value.
 * Returns  : Valid user input.
 *----------------------------------------------------------*/
u32 ReadNumber(int max)
{
    s8 k;
    u32 num = 0;
    u32 cnt = 0;

    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        /* Read keypad input */
        k = KeyScan();

        /* Accept numeric keys */
        if(k >= 0 && k <= 9)
        {
            if(cnt < 2)
            {
                num = (num * 10) + k;
                CharLCD(k + '0');
                cnt++;
            }
        }

        /* Delete previously entered digit */
        else if(k == 15)
        {
            if(cnt > 0)
            {
                num /= 10;
                cnt--;

                CmdLCD(0x10);
                CharLCD(' ');
                CmdLCD(0x10);
            }
        }

        /* Validate entered number */
        else if(k == 16)
        {
            if(cnt == 0)
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("NO INPUT");

                delay_ms(1000);

                CmdLCD(CLEAR_LCD);
                StrLCD("ENTER AGAIN");

                CmdLCD(GOTO_LINE2_POS0);
                continue;
            }

            if(num <= max)
            {
                return num;
            }
            else
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("INVALID");

                delay_ms(1000);

                CmdLCD(CLEAR_LCD);
                StrLCD("TRY AGAIN");

                num = 0;
                cnt = 0;

                CmdLCD(GOTO_LINE2_POS0);
            }
        }
    }
}

/*-----------------------------------------------------------
 * Function : Read4Number()
 * Purpose  : Reads a maximum four-digit number from keypad.
 * Arguments: max - Maximum allowed value.
 * Returns  : Valid user input.
 *----------------------------------------------------------*/
u32 Read4Number(int max)
{
    s8 k;
    u32 num = 0;
    u32 cnt = 0;

    CmdLCD(GOTO_LINE2_POS0);

    while(1)
    {
        /* Read keypad input */
        k = KeyScan();

        /* Accept numeric keys */
        if(k >= 0 && k <= 9)
        {
            if(cnt < 4)
            {
                num = (num * 10) + k;
                CharLCD(k + '0');
                cnt++;
            }
        }

        /* Delete previously entered digit */
        else if(k == 15)
        {
            if(cnt > 0)
            {
                num /= 10;
                cnt--;

                CmdLCD(0x10);
                CharLCD(' ');
                CmdLCD(0x10);
            }
        }

        /* Validate entered number */
        else if(k == 16)
        {
            if((num <= max) && (cnt > 0))
            {
                return num;
            }
            else
            {
                CmdLCD(CLEAR_LCD);
                StrLCD("INVALID");

                delay_ms(1000);

                CmdLCD(CLEAR_LCD);
                StrLCD("TRY AGAIN");

                num = 0;
                cnt = 0;

                CmdLCD(GOTO_LINE2_POS0);
            }
        }
    }
}
