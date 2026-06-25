/* Initialize UART0 */
void InitUART0(void);

/* Transmit a character through UART0 */
void UART0_Tx(char ch);

/* Receive a character through UART0 */
char UART0_Rx(void);

/* Transmit a string through UART0 */
void UART0_Str(char *s);

/* UART0 receive buffer */
extern char buff[];

/* UART0 receive completion flag */
extern unsigned char r_flag;

/* Transmit a character through UART1 */
void UART1_Tx(char ch);

/* Receive a character through UART1 */
char UART1_Rx(void);

/* Transmit a string through UART1 */
void UART1_Str(char *s);

/* UART1 receive buffer */
extern char buff1[];

/* UART1 receive completion flag */
extern unsigned char r_flag1;

/* External Interrupt 1 ISR */
void eint1_isr(void) __irq;

/* Configure External Interrupt 1 */
void enable_ent1(void);

/* Enter administrator mode */
void adminmode(void);

/* Edit RTC settings */
void rtc_edit(void);

/* GSM initialization status */
extern int gsm_flag;
