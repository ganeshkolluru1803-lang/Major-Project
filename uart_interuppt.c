#include <LPC214x.H>  /* LPC21xx definitions         */
#include"uart_interuppt.h"
#include <string.h>
#include<time.h>
#include"lcd.h"
#include "rtc.h"
#define UART_INT_ENABLE 1
 
char buff[200],dummy;

unsigned char i=0,ch,r_flag;


void UART0_isr(void) __irq

{

  if((U0IIR & 0x04)) //check if receive interrupt

  {

		ch = U0RBR;
		if(ch!=0x0d)
		{	/* Read to Clear Receive Interrupt */

			buff[i++] = ch; 

		/*if(i>=8)

		{

			tag[8] = '\0';

			i=0;

			//r_flag = 1;

		} */
		}
		else if(i>7)
		{
		buff[8] = '\0';

			i=0;
		r_flag= 1;
	
		}

  }

  else

  {

      dummy=U0IIR; //Read to Clear transmit interrupt

  

  }

   VICVectAddr = 0; /* dummy write */

}


void InitUART0 (void) /* Initialize Serial Interface       */ 

{  

            		

  PINSEL0 = 0x00000005; /* Enable RxD0 and TxD0              */

  U0LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */

  U0DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */

  U0LCR = 0x03;         /* DLAB = 0  */

  

  #if UART_INT_ENABLE > 0


  VICIntSelect = 0x00000000; // IRQ

  VICVectAddr0 = (unsigned)UART0_isr;

  VICVectCntl0 = 0x20 | 6; /* UART0 Interrupt */

  VICIntEnable = 1 << 6; 

  

  

  /* Enable UART0 Interrupt */

 

 // U0IIR = 0xc0;

 // U0FCR = 0xc7;

  U0IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   

             

  #endif

						

}


void UART0_Tx(char ch)  /* Write character to Serial Port    */  

{ 

  while (!(U0LSR & 0x20));

  U0THR = ch;                

}


char UART0_Rx(void)    /* Read character from Serial Port   */

{                     

  while (!(U0LSR & 0x01));

  return (U0RBR);

}





void UART0_Str(char *s)

{

   while(*s)

       UART0_Tx(*s++);

}


void UART0_Int(unsigned int n)

{

  unsigned char a[10]={0,0,0,0,0,0,0,0,0,0};

  int i=0;

  if(n==0)

  {

    UART0_Tx('0');

	return;

  }

  else

  {

     while(n>0)

	 {

	   a[i++]=(n%10)+48;

	   n=n/10;

	 }

	 --i;

	 for(;i>=0;i--)

	 {

	   UART0_Tx(a[i]);

	 }

   }

}


void UART0_Float(float f)

{

  int x;

  float temp;

  x=f;

  UART0_Int(x);

  UART0_Tx('.');

  temp=(f-x)*100;

  x=temp;

  UART0_Int(x);

}


void  DelayS(unsigned int  dly)

{  unsigned int  i;


   for(; dly>0; dly--) 

      for(i=1200000; i>0; i--);

}

#include <LPC214X.H>

/*int main()

{
	s32 hour,min,sec;
	char otp[5],tag1[10]="12527137";
   int h1,h2,m1,m2,s1,s2; 
	 InitUART0();
	 //InitUART1();
  InitLCD();
  StrLCD("scan Card");

					
  while(1)

  {

		i=0;r_flag=0;
	  UART0_Str("RFID\r\n");
    while(r_flag==0);
	UART0_Str("");
	 CmdLCD(0x01);
	 StrLCD("RFID TAG:");
	 CmdLCD(0xc0);
	 StrLCD(buff);
	 delay_ms(1000);
	CmdLCD(0x01);
	GetRTCTimeInfo(&hour,&min,&sec);
	SetRTCTimeInfo(7,34,55); 
    h1=hour/10;
    h2=hour%10;
    m1=min/10;
    m2=min%10;
    s1=sec/10;
    s2=sec%10;
	otp[0]=(((h1+s2))*3%10)+'0';
	otp[1]=(((m2+h2*s1)*7)%10)+'0';
	otp[2]=(((h1*m2+s2)*11)%10)+'0';
	otp[3]=(((h2+m2+s1+s2)*13)%10)+'0';
  	otp[4]='\0'; 
   if(strcmp(buff,tag1)==0)
   {
      CmdLCD(0x80);
      StrLCD(otp);
   	  CmdLCD(0xc0);
	  StrLCD("Acesss Granted");
	 
   }
   else 
   {
   	  
	  CmdLCD(0xc0);
	  StrLCD("Access Denied");
   }
	  r_flag=0;

  }

}  */


