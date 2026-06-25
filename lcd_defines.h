//************************************************************
// File    : lcd_defines.h
// Purpose : Defines LCD pin assignments and command codes
//           for interfacing a 16x2 character LCD.
//************************************************************

//================ LCD Pin Definitions ================

#define LCD_DATA 16      // LCD data bus starts at P0.16
#define RS       28      // Register Select (RS) pin
#define RW       30      // Read/Write (RW) pin
#define EN       29      // Enable (EN) pin

//================ LCD Command Definitions ================

#define CLEAR_LCD          0x01   // Clear display
#define RET_CUR_HOME       0x02   // Return cursor to home position

#define MODE_4BIT_1LINE    0x20   // 4-bit interface, 1-line display
#define MODE_4BIT_2LINE    0x28   // 4-bit interface, 2-line display
#define MODE_8BIT_1LINE    0x30   // 8-bit interface, 1-line display
#define MODE_8BIT_2LINE    0x38   // 8-bit interface, 2-line display

#define DISP_OFF           0x08   // Display OFF
#define DISP_ON_CUR_OFF    0x0C   // Display ON, cursor OFF
#define DISP_ON_CUR_ON     0x0E   // Display ON, cursor ON
#define DISP_ON_CUR_BLINK  0x0F   // Display ON, blinking cursor

#define GOTO_LINE1_POS0    0x80   // Move cursor to Line 1, Position 0
#define GOTO_LINE2_POS0    0xC0   // Move cursor to Line 2, Position 0
#define GOTO_CGRAM         0x40   // Select CGRAM for custom characters

#define SHIFT_CUR_RIGHT    0x06   // Increment cursor after each character
