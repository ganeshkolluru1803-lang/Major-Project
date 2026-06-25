#include<stdio.h>
#include<lpc21xx.h>

void eint1_isr(void)__irq
{
	adminmode();
	EXTINT=(1<<1);
	VICVectAddr=0;
}


void enable_ent1(void)
{
PINSEL0&=~(3<<28);
PINSEL0|=(2<<28);
VICIntEnable=1<<15;
VICVectAddr2 = (unsigned)eint1_isr;
VICVectCntl2 = (1<<5)|15;
EXTMODE|=1<<1;
//EXTPOLAR=0<<1; 
}

