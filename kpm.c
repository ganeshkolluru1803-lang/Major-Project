//KPM.c
#include "defines.h"
#include "lcd_defines.h"
#include "delay.h"
#include<lpc21xx.h>
#include "types.h"
#include "lcd.h"
#include "kpm.h"
u8 KpmLUT[4][4]=  {{1,2,3,'+'},
									{4,5,6,'-'},
									{7,8,9,'*'},
									{10,0,15,16}};
									

/*u8 KpmLUT[4][4]={{'1','2','3','+'},
									{'4','5','6','-'},
									{'7','8','9','*'},
									{'C','0','=','/'}};*/
void InitKPM(void)
{
	//cfg row pins as outputs(p1.16 to p1.19)
	WRITENIBBLE(IODIR1,ROW0,15);
}	

u8 ColScan(void)
{
	if((READNIBBLE(IOPIN1,COL0))<15)
		return 0;
	
	else 
		return 1;
}
u8 RowCheck(void)
{
	u8 rno;
	for(rno=0; rno<=3; rno++)
	{
		WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));
		if(ColScan()==0)
		{
			break;
		}
	}
	WRITENIBBLE(IOPIN1,ROW0,0x0);
	return rno;
}
u8 ColCheck(void)
{
	u8 cno;
	for(cno=0; cno<=3; cno++)
	{
		if(STATUSBIT(IOPIN1,(COL0+cno))==0)
		{
			break;
		}
	}
	return cno;	
}

u8 KeyScan(void)
{
	u8 keyv,rno,cno;
	//wait switch press
	while(ColScan());
	//take the rno
	rno =RowCheck();
	//take the cno
	cno=ColCheck();
	//collect key value from KpmLUT
	keyv=KpmLUT[rno][cno];
	//wait for switch relese
	while(!ColScan());
	return keyv;
}
u32 ReadNumber(int max)
{
    s8 k;
    u32 num = 0;

    u32 cnt = 0;
	CmdLCD(GOTO_LINE2_POS0);
    while(1)
    {
        k = KeyScan();
      //while(KeyScan()!=0);
        if(k >= 0 && k <= 9)
        {
            if(cnt < 2)
            {
                num = num*10 + k  ;
                CharLCD(k+'0');     
                cnt++;
            }
        }

        /* BACKSPACE */
        else if(k == 15)
        {
            if(cnt > 0)
            {
                num = num / 10;
                cnt--;

                CmdLCD(0x10);
                CharLCD(' ');
                CmdLCD(0x10);
            }
        }

        /* ENTER */
        else if(k == 16)
        {
		 if(cnt==0)
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

u32 Read4Number(int max)
{
    s8 k;
    u32 num = 0;

    u32 cnt = 0;
	CmdLCD(GOTO_LINE2_POS0);
    while(1)
    {
        k = KeyScan();
      //while(KeyScan()!=0);
        if(k >= 0 && k <= 9)
        {
            if(cnt < 4)
            {
                num = num*10 + k  ;
                CharLCD(k+'0');     
                cnt++;
            }
        }

        /* BACKSPACE */
        else if(k == 15)
        {
            if(cnt > 0)
            {
                num = num / 10;
                cnt--;

                CmdLCD(0x10);
                CharLCD(' ');
                CmdLCD(0x10);
            }
        }

        /* ENTER */
        else if(k == 16)
        {
            if(num <= max && cnt > 0 )
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

