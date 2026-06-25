#include "types.h"
/* Write a byte to the LCD */
void WriteLCD(u8 data);
/* Send a command to the LCD */
void CmdLCD(u8 cmd);
/* Display a character on the LCD */
void CharLCD(u8 ascii);
/* Initialize the LCD */
void InitLCD(void);
/* Display a string on the LCD */
void StrLCD(char *str);
/* Display an unsigned integer */
void U32LCD(u32 n);
/* Display a signed integer */
void S32LCD(s32 n);
/* Display a number in binary format */
void BinLCD(u32 n, u8 nbd);
/* Create custom characters in CGRAM */
void BuildCGRAM(u8 *p, u8 nBytes);
/* Display a floating-point number */
void f32LCD(f32 fnum, u8 ndp);
/* Display a number in octal format */
void OctLCD(u32 n);
/* Display a number in hexadecimal format */
void HexLCD(u32 n);
/* Scroll a message across the LCD */
void ScrollLCD(const char *msg);
