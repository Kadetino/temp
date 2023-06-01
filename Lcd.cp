#line 1 "C:/Users/Kadet/Desktop/Новая папка/2я практика - kal-20230525T103228Z-001/2я практика - kal/Lcd.c"
#line 27 "C:/Users/Kadet/Desktop/Новая папка/2я практика - kal-20230525T103228Z-001/2я практика - kal/Lcd.c"
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;

sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char txt1[] = "TUV";
char txt2[] = "Krasava";
char txt3[] = "Lutshui";
char txt4[] = "aagf";

char i;

void Move_Delay() {
 Delay_ms(500);
}

char uart_rd;


void main(){

 Lcd_Init();

 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
#line 73 "C:/Users/Kadet/Desktop/Новая папка/2я практика - kal-20230525T103228Z-001/2я практика - kal/Lcd.c"
 UART1_Init(4800);
 Delay_ms(100);


 while (1) {
 if (UART1_Data_Ready()) {
 uart_rd = UART1_Read();
 UART1_Write(uart_rd);
 Lcd_Cmd(_LCD_CLEAR);
 LCD_Chr(2,1,uart_rd);
 }
 }
#line 99 "C:/Users/Kadet/Desktop/Новая папка/2я практика - kal-20230525T103228Z-001/2я практика - kal/Lcd.c"
}
