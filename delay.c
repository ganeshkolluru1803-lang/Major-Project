#include "types.h"

/*-----------------------------------------------------------
 * Function : delay_us()
 * Purpose  : Generates an approximate delay in microseconds
 *            using a software delay loop.
 * Input    : tdly - Delay duration in microseconds
 *----------------------------------------------------------*/
void delay_us(u32 tdly)
{
    /* Scale the delay count for microsecond timing */
    tdly *= 12;

    /* Execute delay loop */
    while(tdly--);
}

/*-----------------------------------------------------------
 * Function : delay_ms()
 * Purpose  : Generates an approximate delay in milliseconds
 *            using a software delay loop.
 * Input    : tdly - Delay duration in milliseconds
 *----------------------------------------------------------*/
void delay_ms(u32 tdly)
{
    /* Scale the delay count for millisecond timing */
    tdly *= 12000;

    /* Execute delay loop */
    while(tdly--);
}

/*-----------------------------------------------------------
 * Function : delay_s()
 * Purpose  : Generates an approximate delay in seconds
 *            using a software delay loop.
 * Input    : tdly - Delay duration in seconds
 *----------------------------------------------------------*/
void delay_s(u32 tdly)
{
    /* Scale the delay count for second timing */
    tdly *= 12000000;

    /* Execute delay loop */
    while(tdly--);
}
