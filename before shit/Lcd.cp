#line 1 "C:/Users/Администратор/Desktop/kal/йцйуцуц/Lcd.c"




sbit LCD_RS at P0_4_bit;
sbit LCD_RW at P0_5_bit;
sbit LCD_EN at P0_6_bit;


sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char uart_rd;
short int i;
char sigma[32];


void delay(short int ms) {
 for (i=0; i<ms; i++){};
}




void CMD_LCD(char cmd) {
  P3  = (cmd & 0xF0);
 LCD_RS = 0;
 LCD_RW = 0;
 LCD_EN = 1;
 delay(100);
 LCD_EN = 0;

 delay(1000);

  P3  = ((cmd<<4) & 0xF0);
 LCD_RS = 0;
 LCD_RW = 0;
 LCD_EN = 1;
 delay(100);
 LCD_EN = 0;

 delay(100);
}


void Lcd_Write(char symbol) {
  P3  = (symbol & 0xF0);
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_EN = 1;
 delay(1000);
 LCD_EN = 0;

 delay(10000);

  P3  = ((symbol<<4) & 0xF0);
 LCD_RS = 1;
 LCD_RW = 0;
 LCD_EN = 1;
 delay(1000);
 LCD_EN = 0;

 delay(10000);
}


void Lcd_Cursor(int row, int col) {
 char pos = 0;
 if (row == 0)
 pos = 0x80 + (col);
 else if (row == 1)
 pos = 0xC0 + (col);

 CMD_LCD(pos);
}


void CHR_LCD(int row, int col, char symbol) {
 Lcd_Cursor(row, col);
 Lcd_Write(symbol);
}


void INIT_LCD() {
 CMD_LCD(0x02);
 CMD_LCD(0x28);
 CMD_LCD(0x0E);
 CMD_LCD(0x01);
 CMD_LCD(0x80);
}



void UART_Init()
{
 SCON = 0x50;
 TMOD = 0x20;
 TH1 = 0xF5;

 TR1_bit = 1;
}

void UART_WRITEC(char ch)
{
 SBUF = ch;
 while(TI_bit==0);
 TI_bit = 0;
}


char UART_READC(void)
{
 while(RI_bit==0);
 RI_bit=0;
 return(SBUF);
}
void UART_RxString()
 {
 char ch;
 i = 0;



 while(1)
 {
 ch=UART_READC();
#line 140 "C:/Users/Администратор/Desktop/kal/йцйуцуц/Lcd.c"
 Lcd_Write(ch);

 i++;


 }
 }
 char k;
void main() {
 for (i=0;i<32;i++) {
 sigma[i] = ' ';
 }

 Lcd_Init();

 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);



 delay(10000);


 UART_Init();
 delay(10000);





 while (1) {

 k = UART_READC();
 UART_WRITEC(k);
 delay(1000);
 Lcd_Chr_Cp(k);
 delay(1000);



 }
}
