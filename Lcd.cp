#line 1 "C:/Users/Администратор/Desktop/kal/Lcd.c"
#line 17 "C:/Users/Администратор/Desktop/kal/Lcd.c"
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;

sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char txt1[] = "asdf";

char i;
char uart_rd;
short int cur_col, cur_row;

void main(){

 Lcd_Init();

 Lcd_Cmd(_LCD_CLEAR);

 Lcd_Cmd(_LCD_UNDERLINE_ON);


 UART1_Init(4800);
 Delay_ms(500);


 cur_col = 1;
 cur_row = 1;
 while (1) {
 if (UART1_Data_Ready()) {
 uart_rd = UART1_Read();
#line 64 "C:/Users/Администратор/Desktop/kal/Lcd.c"
 if ((97 <= uart_rd && uart_rd <= 122)||
 (65 <= uart_rd && uart_rd <= 90) ||
 (48 <= uart_rd && uart_rd <= 57) ||
 (33 <= uart_rd && uart_rd <= 47) ||
 (58 <= uart_rd && uart_rd <= 64) ||
 (91 <= uart_rd && uart_rd <= 96))
 {
 UART1_Write(uart_rd);
 LCD_Chr(cur_row,cur_col,uart_rd);
 cur_col+=1;
 }




 if (uart_rd == 13)
 {
 UART1_Write(13);
 cur_row +=1;
 if (cur_col != 1) cur_col = 1;
 }
 if (uart_rd == 8)
 {
 UART1_Write(8);
 cur_col-=1;
 }
 if (uart_rd == 127)
 {UART1_Write(127);}







 }
 }
}
