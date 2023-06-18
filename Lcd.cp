#line 1 "C:/Users/Администратор/Desktop/kal/йцйуцуц/Lcd.c"



sbit rs at P0_4_bit;
sbit rw at P0_5_bit;
sbit en at P0_6_bit;
#line 18 "C:/Users/Администратор/Desktop/kal/йцйуцуц/Lcd.c"
sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char uart_rd;
char *data_uart = "                                ";
short int i;
short int j;


void delay(short int ms) {
 for (i=0; i<ms; i++){};
}


void LCD_CmdWrite(char cmd)
{

  P2 =(cmd & 0xf0);
 rs=0;
 rw=0;
 en=1;
 delay(1000);
 en=0;

 delay(1000);

  P2 =((cmd<<4) & 0xf0);
 rs=0;
 rw=0;
 en=1;
 delay(1000);
 en=0;
 delay(10000);
}


void Lcd_Cursor(int row, int col) {

 char pos = 0;
 if (row == 0)
 pos = 0x80 + (col);
 else if (row == 1)
 pos = 0xC0 + (col);
 delay(1000);
 LCD_CmdWrite(pos);
 delay(1000);
}

void LCD_Init1()
{
 delay_us(5000);
 LCD_CmdWrite(0x02);
 LCD_CmdWrite(0x28);
 LCD_CmdWrite(12);
 LCD_CmdWrite(0x01);
 LCD_CmdWrite(0x80);

}

void LCD_DataWrite( char dat)
{


  P2 =(dat & 0xf0);
 rs=1;
 rw=0;
 en=1;
 delay_us(1);
 en=0;

 delay_us(10);

  P2 =((dat <<4) & 0xf0);
 rs=1;
 rw=0;
 en=1;
 delay_us(1);
 en=0;

 delay_ms(1);

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

 while(1)
 {
 ch=UART_READC();
 delay(1000);
 if((ch=='\r') || (ch=='\n'))
 {
 break;
 }
 *data_uart=ch;

 delay(10000);
 LCD_DataWrite(ch);
 delay(10000);

 data_uart++;
 delay(1000);
 }
 }


void main() {

 LCD_Init1();
 delay(1000);
 UART_Init();
 delay(1000);

 delay(1000);






 j = 0;

 while (1) {
 uart_rd = UART_READC();
 if (uart_rd == '\r') {
 j = 0;
 Lcd_Cursor(0, 0);
 continue;
 }
 if (j == 16) {
 Lcd_Cursor(1, 0);
 }
 LCD_DataWrite(uart_rd);
 j++;
#line 195 "C:/Users/Администратор/Desktop/kal/йцйуцуц/Lcd.c"
 }
}
