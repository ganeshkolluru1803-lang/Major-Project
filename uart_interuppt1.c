//************************************************************
// File    : uart1_interrupt.c
// Purpose : Implements UART1 driver with interrupt support
//           for RFID reader communication.
//************************************************************

#include <LPC214x.H>      /* LPC214x Register Definitions */
#include "uart_interuppt.h"
#include <string.h>
#include <time.h>
#include "lcd.h"
#include "rtc.h"

#define UART_INT_ENABLE 1      /* Enable UART1 interrupts */

/* UART1 receive buffer */
char buff1[200], dummy1;

/* UART1 status variables */
unsigned char i1 = 0, ch1, r_flag1;

/* UART1 initialization function */
void InitUART1(void);

/*-----------------------------------------------------------
 * Function : UART1_isr()
 * Purpose  : UART1 Interrupt Service Routine.
 *            Receives RFID data frame and stores the tag ID.
 *----------------------------------------------------------*/
void UART1_isr(void) __irq
{
    /* Check for receive interrupt */
    if(U1IIR & 0x04)
    {
        /* Read received byte */
        ch1 = U1RBR;

        /* Detect start of RFID frame */
        if(ch1 == 0x02)
        {
            r_flag1 = 1;
        }

        /* Store RFID tag characters */
        else if((ch1 != 0x03) && (r_flag1 == 1))
        {
            buff1[i1++] = ch1;
        }

        /* Detect end of RFID frame */
        else
        {
            buff1[i1] = '\0';
            i1 = 0;
            r_flag1 = 2;
        }
    }
    else
    {
        /* Clear transmit interrupt */
        dummy1 = U1IIR;
    }

    /* Notify VIC that interrupt processing is complete */
    VICVectAddr = 0;
}

/*-----------------------------------------------------------
 * Function : InitUART1()
 * Purpose  : Initializes UART1 for 9600 baud communication
 *            and enables UART1 interrupts.
 *----------------------------------------------------------*/
void InitUART1(void)
{
    /* Configure UART1 TXD1 and RXD1 pins */
    PINSEL0 |= 0x00050000;

    /* Configure UART: 8-bit, No parity, 1 stop bit */
    U1LCR = 0x83;

    /* Set baud rate to 9600 bps */
    U1DLL = 97;

    /* Disable DLAB */
    U1LCR = 0x03;

#if UART_INT_ENABLE > 0

    /* Configure UART1 interrupt as IRQ */
    VICIntSelect = 0x00000000;

    /* Register UART1 ISR */
    VICVectAddr1 = (unsigned)UART1_isr;

    /* Assign UART1 interrupt to vector slot 1 */
    VICVectCntl1 = 0x20 | 7;

    /* Enable UART1 interrupt */
    VICIntEnable |= (1 << 7);

    /* Enable Receive and THRE interrupts */
    U1IER = 0x03;

#endif
}

/*-----------------------------------------------------------
 * Function : UART1_Tx()
 * Purpose  : Transmits a single character through UART1.
 *----------------------------------------------------------*/
void UART1_Tx(char ch)
{
    /* Wait until transmitter is ready */
    while(!(U1LSR & 0x20));

    U1THR = ch;
}

/*-----------------------------------------------------------
 * Function : UART1_Rx()
 * Purpose  : Receives a single character through UART1.
 *----------------------------------------------------------*/
char UART1_Rx(void)
{
    /* Wait until data is available */
    while(!(U1LSR & 0x01));

    return U1RBR;
}

/*-----------------------------------------------------------
 * Function : UART1_Str()
 * Purpose  : Transmits a string through UART1.
 *----------------------------------------------------------*/
void UART1_Str(char *s)
{
    while(*s)
    {
        UART1_Tx(*s++);
    }
}

/*-----------------------------------------------------------
 * Function : UART1_Int()
 * Purpose  : Transmits an unsigned integer.
 *----------------------------------------------------------*/
void UART1_Int(unsigned int n)
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
        UART1_Tx(a[i]);
    }
}

/*-----------------------------------------------------------
 * Function : UART1_Float()
 * Purpose  : Transmits a floating-point number.
 *----------------------------------------------------------*/
void UART1_Float(float f)
{
    int x;
    float temp;

    /* Transmit integer part */
    x = f;
    UART1_Int(x);

    UART1_Tx('.');

    /* Transmit fractional part */
    temp = (f - x) * 100;
    x = temp;

    UART1_Int(x);
}

/*
-------------------------------------------------------------
 Test Code (Commented)
-------------------------------------------------------------
Purpose:
- Used during UART1/RFID module testing.
- Displays the received RFID tag.
- Performs basic access verification.

Retained for debugging and future reference.
-------------------------------------------------------------
*/
