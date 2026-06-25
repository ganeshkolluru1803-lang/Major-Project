#include <stdio.h>
#include <lpc21xx.h>

/*-----------------------------------------------------------
 * Function : eint1_isr()
 * Purpose  : Interrupt Service Routine (ISR) for External
 *            Interrupt 1 (EINT1). Enters Admin Mode when the
 *            interrupt is triggered and clears the interrupt
 *            before returning.
 *----------------------------------------------------------*/
void eint1_isr(void) __irq
{
    /* Launch Admin Mode */
    adminmode();

    /* Clear the EINT1 interrupt flag */
    EXTINT = (1 << 1);

    /* Indicate end of interrupt to the VIC */
    VICVectAddr = 0;
}

/*-----------------------------------------------------------
 * Function : enable_ent1()
 * Purpose  : Configures External Interrupt 1 (EINT1) and
 *            registers its Interrupt Service Routine with
 *            the Vectored Interrupt Controller (VIC).
 *----------------------------------------------------------*/
void enable_ent1(void)
{
    /* Configure P0.14 as EINT1 pin */
    PINSEL0 &= ~(3 << 28);
    PINSEL0 |= (2 << 28);

    /* Enable EINT1 interrupt in the VIC */
    VICIntEnable = (1 << 15);

    /* Register ISR address in VIC Vector Slot 2 */
    VICVectAddr2 = (unsigned)eint1_isr;

    /* Configure Vector Slot 2 for EINT1 */
    VICVectCntl2 = (1 << 5) | 15;

    /* Configure EINT1 as edge-triggered interrupt */
    EXTMODE |= (1 << 1);

    /* Uncomment below to configure interrupt polarity
       0 = Falling Edge
       1 = Rising Edge
    */
    // EXTPOLAR &= ~(1 << 1);
}
