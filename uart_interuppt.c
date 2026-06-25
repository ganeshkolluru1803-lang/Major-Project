//************************************************************
// File    : uart_interrupt.c
// Purpose : Implements UART0 driver with interrupt support
//           for RFID data reception and serial communication.
//************************************************************

#include <LPC214x.H>      /* LPC214x Register Definitions */
#include "uart_interuppt.h"
#include <string.h>
#include <time.h>
#include "lcd.h"
#include "rtc.h"

#define UART_INT_ENABLE 1     /* Enable UART interrupt */

/* UART receive buffer */
char buff[200], dummy;

/* UART status variables */
unsigned char i = 0, ch, r_flag;

/*-----------------------------------------------------------
 * Function : UART0_isr()
 * Purpose  : UART0 Interrupt Service Routine.
 *            Receives RFID data and stores it in the buffer.
 *----------------------------------------------------------*/
void UART0_isr(void) __irq
{
    /* Check whether receive interrupt occurred */
    if(U0IIR & 0x04)
    {
        /* Read received character */
        ch = U0RBR;

        /* Ignore carriage return */
        if(ch != 0x0D)
        {
            buff[i++] = ch;
        }
        else if(i > 7)
        {
            /* Terminate received RFID string */
            buff[8] = '\0';

            /* Reset buffer index */
            i = 0;

            /* Indicate RFID data received */
            r_flag = 1;
        }
    }
    else
    {
        /* Clear transmit interrupt */
        dummy = U0IIR;
    }

    /* Notify VIC that interrupt servicing is complete */
    VICVectAddr = 0;
}

/*-----------------------------------------------------------
 * Function : InitUART0()
 * Purpose  : Initializes UART0 and enables interrupt mode.
 *----------------------------------------------------------*/
void InitUART0(void)
{
    /* Configure UART0 TXD0 and RXD0 pins */
    PINSEL0 = 0x00000005;

    /* Configure UART: 8-bit, No parity, 1 stop bit */
    U0LCR = 0x83;

    /* Set baud rate to 9600 bps */
    U0DLL = 97;

    /* Disable DLAB */
    U0LCR = 0x03;

#if UART_INT_ENABLE > 0

    /* Configure UART0 interrupt as IRQ */
    VICIntSelect = 0x00000000;

    /* Register UART0 ISR */
    VICVectAddr0 = (unsigned)UART0_isr;

    /* Assign UART0 interrupt to vector slot 0 */
    VICVectCntl0 = 0x20 | 6;

    /* Enable UART0 interrupt */
    VICIntEnable = 1 << 6;

    /* Enable Receive and THRE interrupts */
    U0IER = 0x03;

#endif
}

/*-----------------------------------------------------------
 * Function : UART0_Tx()
 * Purpose  : Transmits a single character.
 *----------------------------------------------------------*/
void UART0_Tx(char ch)
{
    /* Wait until transmitter is ready */
    while(!(U0LSR & 0x20));

    /* Send character */
    U0THR = ch;
}

/*-----------------------------------------------------------
 * Function : UART0_Rx()
 * Purpose  : Receives a single character.
 *----------------------------------------------------------*/
char UART0_Rx(void)
{
    /* Wait until data is available */
    while(!(U0LSR & 0x01));

    return U0RBR;
}

/*-----------------------------------------------------------
 * Function : UART0_Str()
 * Purpose  : Transmits a null-terminated string.
 *----------------------------------------------------------*/
void UART0_Str(char *s)
{
    while(*s)
        UART0_Tx(*s++);
}

/*-----------------------------------------------------------
 * Function : UART0_Int()
 * Purpose  : Transmits an unsigned integer.
 *----------------------------------------------------------*/
void UART0_Int(unsigned int n)
{
    unsigned char a[10] = {0};
    int i = 0;

    if(n == 0)
    {
        UART0_Tx('0');
        return;
    }

    /* Convert integer into ASCII digits */
    while(n > 0)
    {
        a[i++] = (n % 10) + '0';
        n /= 10;
    }

    /* Transmit digits */
    while(--i >= 0)
    {
        UART0_Tx(a[i]);
    }
}

/*-----------------------------------------------------------
 * Function : UART0_Float()
 * Purpose  : Transmits a floating-point number.
 *----------------------------------------------------------*/
void UART0_Float(float f)
{
    int x;
    float temp;

    /* Transmit integer part */
    x = f;
    UART0_Int(x);

    UART0_Tx('.');

    /* Transmit fractional part */
    temp = (f - x) * 100;
    x = temp;

    UART0_Int(x);
}

/*-----------------------------------------------------------
 * Function : DelayS()
 * Purpose  : Generates an approximate delay in seconds.
 *----------------------------------------------------------*/
void DelayS(unsigned int dly)
{
    unsigned int i;

    while(dly--)
    {
        for(i = 1200000; i > 0; i--);
    }
}

#include <LPC214X.H>

/*
-------------------------------------------------------------
 Test Code (Commented)
-------------------------------------------------------------
Purpose:
- Reads RFID tag through UART.
- Displays RFID tag on LCD.
- Generates OTP using RTC time.
- Grants or denies access based on RFID validation.

Retained for testing and debugging purposes.
-------------------------------------------------------------
*/
