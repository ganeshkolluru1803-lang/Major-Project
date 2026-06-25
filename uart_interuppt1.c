#include <LPC214x.H>  /* LPC21xx definitions         */
#include"uart_interuppt.h"
#include <string.h>
#include<time.h>
#include"lcd.h"
#include "rtc.h"
#define UART_INT_ENABLE 1
 
char buff1[200],dummy1;

unsigned char i1=0,ch1,r_flag1;
void InitUART1 (void);

void UART1_isr(void) __irq

{

  if((U1IIR & 0x04)) //check if receive interrupt

  {

		ch1= U1RBR;
		if(ch1==0x02)
		{	/* Read to Clear Receive Interrupt */

			//buff1[i1++] = ch1; 
			r_flag1=1;
		/*if(i>=8)

		{

			tag[8] = '\0';

			i=0;

			//r_flag = 1;

		} */
		}
		else if((ch1!=0x03)&&(r_flag1==1))
		{
		buff1[i1++]=ch1;
		}
		else 
		{
		 buff1[i1]='\0';
		 i1=0;
		 r_flag1=2;
		}

  }

  else

  {

      dummy1=U1IIR; //Read to Clear transmit interrupt

  

  }

   VICVectAddr = 0; /* dummy write */

}


void InitUART1 (void) /* Initialize Serial Interface       */ 

{  

            		

  PINSEL0 |= 0x00050000; /* Enable RxD0 and TxD0              */

  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */

  U1DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */

  U1LCR = 0x03;         /* DLAB = 0  */

  

  #if UART_INT_ENABLE > 0


  VICIntSelect = 0x00000000; // IRQ

  VICVectAddr1 = (unsigned)UART1_isr;

  VICVectCntl1 = 0x20 | 7; /* UART0 Interrupt */

  VICIntEnable |= 1 << 7; 

  

  

  /* Enable UART0 Interrupt */

 

 // U0IIR = 0xc0;

 // U0FCR = 0xc7;

  U1IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   

             

  #endif

						

}


void UART1_Tx(char ch)  /* Write character to Serial Port    */  

{ 

  while (!(U1LSR & 0x20));

  U1THR = ch;                

}


char UART1_Rx(void)    /* Read character from Serial Port   */

{                     

  while (!(U1LSR & 0x01));

  return (U1RBR);

}





void UART1_Str(char *s)

{

   while(*s)

       UART1_Tx(*s++);

}


void UART1_Int(unsigned int n)

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

	   UART1_Tx(a[i]);

	 }

   }

}


void UART1_Float(float f)

{

  int x;

  float temp;

  x=f;

  UART1_Int(x);

  UART1_Tx('.');

  temp=(f-x)*100;

  x=temp;

  UART1_Int(x);

}



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


