#define databus P2       // LCD databus connected to PORT0

// LCD module connections
sbit rs at P0_4_bit;
sbit rw at P0_5_bit;    // not necessary
sbit en at P0_6_bit;

/* 16x2 LCD Specification */
#define LCDMaxLines 2
#define LCDMaxChars 16
#define LineOne 0x80
#define LineTwo 0xc0

#define BlankSpace ' '


// not necessary (using databus)
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

   databus=(cmd & 0xf0);        // Send the Higher Nibble of the command to LCD
     rs=0;                      // Select the Command Register by pulling RS LOW
     rw=0;                      // Select the Write Operation  by pulling RW LOW
     en=1;                      // Send a High-to-Low Pusle at Enable Pin
    delay(1000);
     en=0;

   delay(1000);                   // wait for some time

   databus=((cmd<<4) & 0xf0);   // Send the Lower Nibble of the command to LCD
     rs=0;                      // Select the Command Register by pulling RS LOW
     rw=0;                      // Select the Write Operation  by pulling RW LOW
     en=1;                      // Send a High-to-Low Pusle at Enable Pin
    delay(1000);
     en=0;
   delay(10000);
}

// set cursor in LCD to given position
void Lcd_Cursor(int row, int col) {

    char pos = 0;
    if (row == 0)
        pos = 0x80 + (col);  // First line
    else if (row == 1)
        pos = 0xC0 + (col);  // Second line
    delay(1000);
    LCD_CmdWrite(pos);                // Change cursor position
    delay(1000);
}

void LCD_Init1()
{
    delay_us(5000);
   LCD_CmdWrite(0x02);  //Initilize the LCD in 4bit Mode
   LCD_CmdWrite(0x28);
   LCD_CmdWrite(12);  // 0x0E Display ON cursor ON
   LCD_CmdWrite(0x01);  // Clear the LCD
   LCD_CmdWrite(0x80);  // Move the Cursor to First line First Position

}

void LCD_DataWrite( char dat)
{


   databus=(dat & 0xf0);        // Send the Higher Nibble of the Data to LCD
     rs=1;                      // Select the Data Register by pulling RS HIGH
     rw=0;                      // Select the Write Operation  by pulling RW LOW
     en=1;                      // Send a High-to-Low Pusle at Enable Pin
    delay_us(1);
     en=0;

   delay_us(10);                 // wait for some time.

   databus=((dat <<4) & 0xf0);  // Send the Lower Nibble of the Data to LCD
     rs=1;                      // Select the Data Register by pulling RS HIGH
     rw=0;                      // Select the Write Operation  by pulling RW LOW
     en=1;                      // Send a High-to-Low Pusle at Enable Pin
    delay_us(1);
     en=0;

   delay_ms(1);

}


void UART_Init()
{ 
    SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;  //Timer1 in Mode2.
    TH1 = 0xF5;
    TR1_bit = 1;      //Turn ON the timer for Baud rate generation
}

void UART_WRITEC(char ch)
{
    SBUF = ch;      // Load the data to be transmitted
    while(TI_bit==0);   // Wait till the data is trasmitted
    TI_bit = 0;         //Clear the Tx flag for next cycle.
}


char UART_READC(void)
{
    while(RI_bit==0);     // Wait till the data is received
    RI_bit=0;             // Clear Receive Interrupt Flag for next cycle
    return(SBUF);     // return the received char
}
void UART_RxString()
 {
     char ch;

     while(1)
       {
         ch=UART_READC();    //Reaceive a char
         delay(1000);
         if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
           {                          //once enter key is pressed
                break;                //and break the loop
             }
         *data_uart=ch;              //copy the char into string.
         
         delay(10000);
         LCD_DataWrite(ch);
         delay(10000);
         
         data_uart++;
         delay(1000);
      }
 }
 
 
void main() {
  LCD_Init1();                        // Initialize LCD
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
  }
}