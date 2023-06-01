/*
 * Test configuration:
     MCU:             AT89S8253
                      http://www.atmel.com/dyn/resources/prod_documents/doc3286.pdf
     Dev.Board:       Easy8051v6 - ac:LCD
                      http://www.mikroe.com/easy8051/
     Oscillator:      External Clock 10.0000 MHz
     Ext. Modules:    Lcd 2x16
                      http://www.mikroe.com/store/components/various/#other
     SW:              mikroC PRO for 8051
                      http://www.mikroe.com/mikroc/8051/
 * NOTES:
     - Turn on LCD backlight switch on development board (SW7.6).
*/

// LCD module connections
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;
//
sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char txt1[] = "asdf";

char i;                              // Loop variable
char uart_rd;
short int cur_col, cur_row;

void main(){

  Lcd_Init();                        // Initialize LCD

  Lcd_Cmd(_LCD_CLEAR);               // Clear display
//  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  Lcd_Cmd(_LCD_UNDERLINE_ON);
  Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
  
  UART1_Init(4800);               // Initialize UART module at 4800 bps
  Delay_ms(500);                  // Wait for UART module to stabilize

  // cursor preset
  cur_col = 1;
  cur_row = 1;
  while (1) {                     // Endless loop
    if (UART1_Data_Ready()) {     // If data is received,
      uart_rd = UART1_Read();     //   read the received data,
//      Lcd_Cmd(_LCD_CLEAR);
//      UART1_Write(uart_rd);       //   and send data via UART

      //##############
      //REGULAR LETTERS
      //##############
      // NAME               INT
      //Lower case latin    97-122
      //Upper case latin    65-90
      //Numbers             48-57
      // !"#$%&'()*+,-./    33-47
      // :;<=>?@            58-64
      // [\]^_`             91-96     ??91 & ??93 []
      // {|}~               123-126   ??123 & ??125 {}
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

      //##############
      //SPECIAL KEYS
      //##############
      if (uart_rd == 13) // Enter  13
      {
        UART1_Write(13);
        cur_row +=1;
        if (cur_col != 1) cur_col = 1; //
      }
      if (uart_rd == 8) // Backspace  8
      {
        UART1_Write(8);
        cur_col-=1;
      }
      if (uart_rd == 127) // Delete  127
      {UART1_Write(127);}
      
      //##############
      //ARROW KEYS
      //##############
      // there are no ascii codes
      // replace with 91 93 123 && 125?
      // https://stackoverflow.com/questions/2876275/what-are-the-ascii-values-of-up-down-left-right
    }
  }
}