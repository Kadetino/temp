// LCD Databus is connected to P3.4:P3.7
#define LcdDataBus P3

// LCD module connections
sbit LCD_RS at P0_4_bit;
sbit LCD_RW at P0_5_bit;    // not necessary
sbit LCD_EN at P0_6_bit;

// not necessary (using databus)
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



// send command to LCD in 4-bit mode
void CMD_LCD(char cmd) {
    LcdDataBus = (cmd & 0xF0);     // Send higher half-byte
    LCD_RS = 0;                    // Send LOW pulse on RS pin for selecting Command register
    LCD_RW = 0;                    // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                    // Generate a High-to-low pulse on EN pin
    delay(100);
    LCD_EN = 0;

    delay(1000);

    LcdDataBus = ((cmd<<4) & 0xF0); //Send Lower half-byte
    LCD_RS = 0;                     // Send LOW pulse on RS pin for selecting Command register
    LCD_RW = 0;                     // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                     // Generate a High-to-low pulse on EN pin
    delay(100);
    LCD_EN = 0;

    delay(100);
}

// write symbol to lcd
void Lcd_Write(char symbol) {
    LcdDataBus = (symbol & 0xF0);      //Send higher nibble
    LCD_RS = 1;                        // Send HIGH pulse on RS pin for selecting data register
    LCD_RW = 0;                        // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                        // Generate a High-to-low pulse on EN pin
    delay(1000);
    LCD_EN = 0;

    delay(10000);

    LcdDataBus = ((symbol<<4) & 0xF0);  // Send Lower nibble
    LCD_RS = 1;                         // Send HIGH pulse on RS pin for selecting data register
    LCD_RW = 0;                         // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                         // Generate a High-to-low pulse on EN pin
    delay(1000);
    LCD_EN = 0;

    delay(10000);
}

// set cursor in LCD to given position
void Lcd_Cursor(int row, int col) {
    char pos = 0;
    if (row == 0)
        pos = 0x80 + (col);  // First line
    else if (row == 1)
        pos = 0xC0 + (col);  // Second line

    CMD_LCD(pos);                // Change cursor position
}

// send symbol to LCD in specified row and column
void CHR_LCD(int row, int col, char symbol) {
    Lcd_Cursor(row, col);          // Update cursor position
    Lcd_Write(symbol);             // Write char to LCD
}

// initialize LCD with default params
void INIT_LCD() {
    CMD_LCD(0x02);        // Move the cursor to home position
    CMD_LCD(0x28);        // Initialize LCD in 4-bit mode, enable 5x7 mode for chars
    CMD_LCD(0x0E);        // 0x0E Display OFF, Cursor ON (underline)
    CMD_LCD(0x01);        // Clear Display
    CMD_LCD(0x80);        // Move the cursor to beginning of first line
}



void UART_Init()
{ 
    SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
    TMOD = 0x20;  //Timer1 in Mode2.
    TH1 = 0xF5;
    //TL1 = 0xF5;
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
     i = 0;
     
     //CMD_LCD(1);
     //Lcd_Cursor(0, 0);
     while(1)
       {
          ch=UART_READC();    //Reaceive a char
         /*
         if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
           {                          //once enter key is pressed
                break;                //and break the loop
             }

         
         if (i == 16) {
            Lcd_Cursor(1, 0);
         }*/
         Lcd_Write(ch);

         i++;
         //*string_ptr=ch;              //copy the char into string.
         //string_ptr++;                //and increment the pointer
      }
 }
  char k;
void main() {
  for (i=0;i<32;i++) {
      sigma[i] = ' ';
  }

  Lcd_Init();                        // Initialize LCD

  Lcd_Cmd(_LCD_CLEAR);               // Clear display
  Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off


  //INIT_LCD();          // Initialize LCD
  delay(10000);
  //Lcd_Init();
  //delay(10000);
  UART_Init();
  delay(10000);

  //CMD_LCD(1);
  //CMD_LCD(12);                         // Cursor off


  while (1) {

        k = UART_READC();
        UART_WRITEC(k);
        delay(1000);
        Lcd_Chr_Cp(k);
        delay(1000);
        
        //UART_RxString();
        //CHR_LCD(0, 0, '3');
  }
}