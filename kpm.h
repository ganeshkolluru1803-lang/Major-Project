#include "types.h"

/* Initializes the 4x4 matrix keypad */
void InitKPM(void);
/* Checks whether any key is pressed */
u8 ColScan(void);
/* Determines the row of the pressed key */
u8 RowCheck(void);
/* Determines the column of the pressed key */
u8 ColCheck(void);
/* Scans the keypad and returns the pressed key value */
u8 KeyScan(void);
/* Reads a valid 2-digit number from the keypad */
u32 ReadNumber(int max);
/* Reads a valid 4-digit number from the keypad */
u32 Read4Number(int max);
