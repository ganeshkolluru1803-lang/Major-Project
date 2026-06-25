
void InitUART0 (void);
void UART0_Tx(char ch);
char UART0_Rx(void);
void UART0_Str(char *s);
extern char buff[];
extern unsigned char r_flag;
void UART1_Tx(char ch);
char UART1_Rx(void);
void UART1_Str(char *s);
extern char buff1[];
extern unsigned char r_flag1;
void eint1_isr(void)__irq;
void enable_ent1(void);
void adminmode(void);
void rtc_edit(void);
extern int gsm_flag;

