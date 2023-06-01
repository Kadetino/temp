/*
 * Project name:
     Lcd_Test (Demonstration of the LCD library routines)
 * Copyright:
     (c) Mikroelektronika, 2009.
 * Revision History:
     20071210:
       - initial release;
 * Description:
     This code demonstrates how to use LCD 4-bit library. LCD is first
     initialized, then some text is written, then the text is moved.
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

sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;
// End LCD module connections

char txt1[] = "TUV";
char txt2[] = "Krasava";
char txt3[] = "Lutshui";
char txt4[] = "aagf";

char i;                              // Loop variable

void Move_Delay() {                  // Function used for text moving
  Delay_ms(500);                     // You can change the moving speed here
}

char uart_rd;


void main(){

  Lcd_Init();                        // Initialize LCD

  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
  //LCD_Out(1,6,txt3);                 // Write text in first row

  //LCD_Out(2,6,txt4);                 // Write text in second row
  //Delay_ms(2000);
  //Lcd_Cmd(_LCD_CLEAR);               // Clear display

  //LCD_Out(1,1,txt1);                 // Write text in first row
  //LCD_Out(2,5,txt2);                 // Write text in second row

  //Delay_ms(2000);

  // Moving text
  //for(i=0; i<4; i++) {               // Move text to the right 4 times
  //  Lcd_Cmd(_LCD_SHIFT_RIGHT);
  //  Move_Delay();
  //}
  
  UART1_Init(4800);               // Initialize UART module at 4800 bps
  Delay_ms(100);                  // Wait for UART module to stabilize

  //UART1_Write_Text("Start");
  while (1) {                     // Endless loop
    if (UART1_Data_Ready()) {     // If data is received,
      uart_rd = UART1_Read();     //   read the received data,
      UART1_Write(uart_rd);       //   and send data via UART
      Lcd_Cmd(_LCD_CLEAR);
      LCD_Chr(2,1,uart_rd);
    }
  }

  /*
  while(1) {                         // Endless loop
    for(i=0; i<8; i++) {             // Move text to the left 7 times
      Lcd_Cmd(_LCD_SHIFT_LEFT);
      Move_Delay();
    }

    for(i=0; i<8; i++) {             // Move text to the right 7 times
      Lcd_Cmd(_LCD_SHIFT_RIGHT);
      Move_Delay();
    }
  }
  */
}