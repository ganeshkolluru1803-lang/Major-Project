#ifndef DEFINES_H
#define DEFINES_H

/*-----------------------------------------------------------
 * File    : defines.h
 * Purpose : Contains commonly used bit manipulation macros
 *           for configuring and accessing hardware registers.
 *----------------------------------------------------------*/

/* Set the specified bit */
#define SETBIT(WORD,BP) (WORD |= (1 << BP))

/* Clear the specified bit */
#define CLRBIT(WORD,BP) (WORD &= ~(1 << BP))

/* Read the value of the specified bit */
#define STATUSBIT(WORD,BP) ((WORD >> BP) & 1)

/* Write a single bit (0 or 1) at the specified position */
#define WRITEBIT(WORD,BP,BIT)  (WORD = ((WORD & ~(1 << BP)) | (BIT << BP)))

/* Write a 4-bit nibble at the specified bit position */
#define WRITENIBBLE(WORD,SBP,NIBBLE) (WORD = ((WORD & ~(0xF << SBP)) | (NIBBLE << SBP)))

/* Write an 8-bit byte at the specified bit position */
#define WRITEBYTE(WORD,SBP,BYTE)  (WORD = ((WORD & ~(0xFF << SBP)) | (BYTE << SBP)))

/* Write a 16-bit half-word at the specified bit position */
#define WRITEHWORD(WORD,SBP,HWORD)  (WORD = ((WORD & ~(0xFFFF << SBP)) | (HWORD << SBP)))

/* Read a 4-bit nibble from the specified bit position */
#define READNIBBLE(WORD,SBP) ((WORD >> SBP) & 0x0F)

/* Set only the specified bit (used with IOSET registers) */
#define SSETBIT(WORD,BP) (WORD = (1 << BP))

/* Clear only the specified bit (used with IOCLR registers) */
#define SCLRBIT SSETBIT

#endif
