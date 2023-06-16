// LCD Databus is connected to P3.4:P3.7
#define LcdDataBus P3
#define MAX_ROWS 2

// LCD module connections
sbit LCD_RS at P0_4_bit;
sbit LCD_RW at P0_5_bit;    // not necessary
sbit LCD_EN at P0_6_bit;

// not necessary (using databus)
sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


// declaring vars
char storedSymbols[MAX_ROWS][16];
char tmp[MAX_ROWS][16];
unsigned char cursor, displayRowPos;
char uart_rd;
unsigned char shift, vCursor;
unsigned char i, j;


// delay function
void delay(int cnt) {
    for (i = 0; i < cnt; i++);
}


// **********************
// UART FUNCTIONS
// **********************



// **********************
// INTERNAL LCD FUNCTIONS
// **********************


// send command to LCD in 4-bit mode
void Lcd_Cmd(char cmd) {
    LcdDataBus = (cmd & 0xF0);     // Send higher half-byte
    LCD_RS = 0;                    // Send LOW pulse on RS pin for selecting Command register
    LCD_RW = 0;                    // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                    // Generate a High-to-low pulse on EN pin
    delay(1000);
    LCD_EN = 0;

    delay(10000);

    LcdDataBus = ((cmd<<4) & 0xF0); //Send Lower half-byte
    LCD_RS = 0;                     // Send LOW pulse on RS pin for selecting Command register
    LCD_RW = 0;                     // Send LOW pulse on RW pin for Write operation
    LCD_EN = 1;                     // Generate a High-to-low pulse on EN pin
    delay(1000);
    LCD_EN = 0;

    delay(10000);
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

    Lcd_Cmd(pos);                // Change cursor position
}

// send symbol to LCD in specified row and column
void Lcd_Chr(int row, int col, char symbol) {
    Lcd_Cursor(row, col);          // Update cursor position
    Lcd_Write(symbol);             // Write char to LCD
}

// initialize LCD with default params
void Lcd_Init() {
    Lcd_Cmd(0x02);        // Move the cursor to home position
    Lcd_Cmd(0x28);        // Initialize LCD in 4-bit mode, enable 5x7 mode for chars
    Lcd_Cmd(0x0E);        // Display OFF, Cursor ON (underline)
    Lcd_Cmd(0x01);        // Clear Display
    Lcd_Cmd(0x80);        // Move the cursor to beginning of first line
}


void shiftSymbols() {
    shift = 16 - (cursor % 16);
    if (cursor / 16 == 0) {
        for (i = cursor; i < 16; i++) {
            storedSymbols[(i + 16) / 16][(i + 16) % 16] = '\0';
            Lcd_Chr((i + 16) / 16, (i + 16) % 16), ' ';

            storedSymbols[(i + shift) / 16][(i + shift) % 16] = storedSymbols[i / 16][i % 16];

            if (storedSymbols[(i + shift) / 16][(i + shift) % 16] == '\n' || storedSymbols[(i + shift) / 16][(i + shift) % 16] == '\0') {
                 Lcd_Chr((i + shift) / 16, (i + shift) % 16, ' ');
            }
            else {
                Lcd_Chr((i + shift) / 16, (i + shift) % 16, storedSymbols[(i + shift) / 16][(i + shift) % 16]);
            }


            storedSymbols[i / 16][i % 16] = '\0';
            Lcd_Chr(i / 16, i % 16, ' ');
        }

        displayRowPos = 1;
        cursor += shift;
        Lcd_Cursor(cursor / 16, cursor % 16);

        if (storedSymbols[0][0] == '\0') {
            storedSymbols[0][0] = '\n';
        }
    }

    else {
        for (i = cursor; i < 32; i++) {
            storedSymbols[i / 16][i % 16] = '\0';
            Lcd_Chr(i / 16, i % 16, ' ');
        }
        if (storedSymbols[1][0] == '\0') {
            storedSymbols[1][0] = '\n';
        }
    }
}


void setSymbol(const char &symbol) {
    // Enter
    if (symbol == 0x0d) {
        if (cursor == MAX_ROWS * 16) {
            return;
        }
        shiftSymbols();
    }

    // Up Arrow
    else if (symbol == 0x18) {
        if (cursor / 16 == 0) {
            return;
        }
        shift = cursor % 16;
        for (i = cursor-16; i >= cursor-shift-16; i--) {
            if (storedSymbols[i / 16][i % 16] == '\0') {
                continue;
            }

            if (i % 16 != cursor % 16 && cursor % 16 != 15 && storedSymbols[(i+1) / 16][(i+1) % 16] == '\0' && storedSymbols[i / 16][i % 16] != '\n') {
                cursor = i + 1;
            }
            else {
                cursor = i;
            }

            displayRowPos = 0;
            Lcd_Cursor(displayRowPos, cursor % 16);

            return;
        }

    }

    // Down Arrow
    else if (symbol == 0x19) {
        if (cursor / 16 == MAX_ROWS-1) {
            return;
        }
        shift = cursor % 16;
        for (i = cursor+16; i >= cursor-shift+16; i--) {
            if (storedSymbols[i / 16][i % 16] == '\0') {
                continue;
            }

            if (i % 16 != cursor % 16 && cursor % 16 != 15 && storedSymbols[(i+1) / 16][(i+1) % 16] == '\0' && storedSymbols[i / 16][i % 16] != '\n') {
                cursor = i + 1;
            }
            else {
                cursor = i;
            }

            displayRowPos = 1;
            Lcd_Cursor(displayRowPos, cursor % 16);

            return;
        }
    }

    // Right Arrow
    else if (symbol == 0x1a) {
        if (cursor == MAX_ROWS * 16 - 1) {
            return;
        }
        if (cursor == 16 && storedSymbols[cursor / 16][cursor % 16] == '\n') {
            return;
        }
        else if (cursor % 16 == 15 && storedSymbols[(cursor+1) / 16][(cursor+1) % 16] != '\0') {
            cursor++;
            displayRowPos = 1;
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
        else if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\n' && cursor / 16 == 0) {
            for (i = cursor+1; i < MAX_ROWS * 16; i++) {
                if (storedSymbols[i / 16][i % 16] != '\0') {
                    cursor = i;
                    displayRowPos = 1;
                    Lcd_Cursor(displayRowPos, cursor % 16);
                    break;
                }
            }

        }
        else if (storedSymbols[cursor / 16][cursor % 16] == '\0') {
            for (i = cursor+1; i < MAX_ROWS * 16; i++) {
                if (storedSymbols[i / 16][i % 16] != '\0') {
                    cursor = i;
                    displayRowPos = 1;
                    Lcd_Cursor(displayRowPos, cursor % 16);
                    break;
                }
            }
        }
        else if (storedSymbols[cursor / 16][cursor % 16] != '\0') {
            // check \n
            cursor++;
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
    }

    // Left Arrow
    else if (symbol == 0x1b) {
        if (cursor == 0) {
            return;
        }
        else if (cursor % 16 == 0) {
            do {
                cursor--;
                if (cursor == 0 || (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0')) {
                    break;
                }
            } while(storedSymbols[cursor / 16][cursor % 16] == '\0');

            if (cursor % 16 != 15 && storedSymbols[cursor / 16][cursor % 16] != '\n') {
                cursor++;
            }

            displayRowPos = 0;
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
        else {
            cursor--;
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
    }

    // Delete
    else if (symbol == 0x7f) {
        if (cursor % 16 == 15) {
            storedSymbols[cursor / 16][cursor % 16] = '\0';
            Lcd_Chr(cursor / 16, cursor % 16, ' ');

            return;
        }
        if (cursor % 16 != 15 && storedSymbols[cursor / 16][cursor % 16] != '\0') {
            if (storedSymbols[cursor / 16][cursor % 16] == '\n') {
                for (i = 0; i < MAX_ROWS; i++) {
                    for (j = 0; j < 16; j++) {
                        tmp[i][j] = storedSymbols[i][j];
                    }
                }


                vCursor = cursor + 16;
                while (vCursor < MAX_ROWS * 16 - 1) {
                    storedSymbols[(vCursor-16) / 16][(vCursor-16) % 16] = tmp[vCursor / 16][vCursor % 16];
                    vCursor++;
                }
                // need to update display
                if (displayRowPos == 0) {
                    for (i = cursor; i < cursor + 16; i++) {
                        if (storedSymbols[i / 16][i % 16] != '\n' && storedSymbols[i / 16][i % 16] != '\0') {
                            Lcd_Chr(0, i % 16, storedSymbols[i / 16][i % 16]);
                        }
                        else {
                            Lcd_Chr(0, i % 16, ' ');
                        }

                        if (storedSymbols[1][(i+16) % 16] != '\n' && storedSymbols[1][(i+16) % 16] != '\0') {
                            Lcd_Chr(1, (i+16) % 16, storedSymbols[(i+16) / 16][(i+16) % 16]);
                        }
                        else {
                            Lcd_Chr(1, (i+16) % 16, ' ');
                        }
                    }
                }
                else {
                    for (i = cursor; i < cursor + 16; i++) {
                        if (storedSymbols[i / 16][i % 16] != '\n' && storedSymbols[i / 16][i % 16] != '\0') {
                            Lcd_Chr(1, (i+16) % 16, storedSymbols[i / 16][i % 16]);
                        }
                        else {
                            Lcd_Chr(1, (i+16) % 16, ' ');
                        }
                    }
                }
                if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                    storedSymbols[cursor / 16][cursor % 16] = '\n';
                }

                return;
            }

            shift = 16 - (cursor % 16);
            for (i = 0; i < shift; i++) {
                storedSymbols[(cursor+i) / 16][(cursor+i) % 16] = storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16];

                if (storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16] != '\n' && storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16] != '\0') {
                    Lcd_Chr(displayRowPos, (cursor+i) % 16, storedSymbols[(cursor+i+1) / 16][(cursor+i+1) % 16]);
                }
                else {
                    Lcd_Chr(displayRowPos, (cursor+i) % 16, ' ');
                }

            }
            storedSymbols[(cursor+shift-1) / 16][(cursor+shift-1) % 16] = '\0';
            Lcd_Chr(displayRowPos, (cursor+shift-1) % 16, ' ');

            if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                storedSymbols[cursor / 16][cursor % 16] = '\n';
            }

        }
        else if (storedSymbols[(cursor+1) / 16][(cursor+1) % 16] == '\0') {
            shift = 16 - (cursor % 16);
            if ((cursor + shift) >= MAX_ROWS * 16 - 1) {
                return;
            }
            if (storedSymbols[(cursor+shift) / 16][(cursor+shift) % 16] == '\n') {
                for (i = 0; i < MAX_ROWS; i++) {
                    for (j = 0; j < 16; j++) {
                        tmp[i][j] = storedSymbols[i][j];
                    }
                }
                vCursor = cursor + 16 + shift;

                if (vCursor >= MAX_ROWS * 16 - 1) {
                    return;
                }

                while (vCursor < MAX_ROWS * 16 - 1) {
                    storedSymbols[(vCursor-16) / 16][(vCursor-16) % 16] = tmp[vCursor / 16][vCursor % 16];
                    vCursor++;
                }
                // need to update display
                if (displayRowPos == 0) {
                    for (i = cursor + shift; i < cursor + 16 + shift; i++) {

                        if (storedSymbols[i / 16][i % 16] != '\n' && storedSymbols[i / 16][i % 16] != '\0') {
                            Lcd_Chr(1, i % 16, storedSymbols[i / 16][i % 16]);
                        }
                        else {
                            Lcd_Chr(1, i % 16, ' ');
                        }
                    }
                }
            }
            if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                storedSymbols[cursor / 16][cursor % 16] = '\n';
            }
        }

    }
    // Backspace
    else if (symbol == 0x08) {
        if (cursor == 0) {
            return;
        }

        if (cursor % 16 != 0) {
            shift = 16 - (cursor % 16);
            for (i = cursor-1; i < cursor+shift-1; i++) {
                storedSymbols[i / 16][i % 16] = storedSymbols[(i+1) / 16][(i+1) % 16];

                if (storedSymbols[i / 16][i % 16] == '\n' || storedSymbols[i / 16][i % 16] == '\0') {
                    Lcd_Chr(displayRowPos, i % 16, ' ');
                }
                else {
                    Lcd_Chr(displayRowPos, i % 16, storedSymbols[i / 16][i % 16]);
                }
            }
            storedSymbols[(cursor+shift-1) / 16][(cursor+shift-1) % 16] = '\0';
            display[displayRowPos][(cursor+shift-1) % 16]->setText(QString(' '));
            Lcd_Chr(displayRowPos, (cursor+shift-1) % 16, ' ');

            cursor--;

            if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                storedSymbols[cursor / 16][cursor % 16] = '\n';
            }
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
        else {
            if (cursor / 16 == 0) {
                return;
            }
            for (int i = cursor-1; i>=cursor-16; i--) {
                if (storedSymbols[i / 16][i % 16] == '\0') {
                    continue;
                }
                if (storedSymbols[i / 16][i % 16] == '\n') {
                    vCursor = i;

                    for (i = 0; i < MAX_ROWS; i++) {
                        for (j = 0; j < 16; j++) {
                            tmp[i][j] = storedSymbols[i][j];
                        }
                    }

                    if (vCursor >= MAX_ROWS * 16 - 1 || vCursor < 0) {
                        return;
                    }

                    while (vCursor+16 < MAX_ROWS * 16 - 1) {
                        storedSymbols[vCursor / 16][vCursor % 16] = tmp[(vCursor+16) / 16][(vCursor+16) % 16];
                        vCursor++;
                    }

                    cursor = i;
                    displayRowPos = 0;
                    Lcd_Cursor(displayRowPos, cursor % 16);
                    
                    if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                        storedSymbols[cursor / 16][cursor % 16] = '\n';
                    }

                    return;
                }
                else {
                    cursor = i;
                    displayRowPos = 0;
                    Lcd_Chr(displayRowPos, cursor % 16, ' ');
                    Lcd_Cursor(displayRowPos, cursor % 16);

                    storedSymbols[cursor / 16][cursor % 16] = '\0';

                    if (cursor % 16 == 0 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                        storedSymbols[cursor / 16][cursor % 16] = '\n';
                    }

                    return;
                }
            }
        }
    }
    // Other allowed symbols
    else if (symbol >= 0x20 && symbol <= 0x7e) {
        // qDebug() << "Symbol: " << symbol << "; Hex: " << hex << (int)symbol;

        // check forbidden cursor pos
        if (cursor == MAX_ROWS * 16){
            return;
        }
        // check last cursor pos
        if (cursor == MAX_ROWS * 16 - 1) {
            storedSymbols[cursor / 16][cursor % 16] = symbol;
            Lcd_Chr(displayRowPos, cursor % 16, symbol);
            return;
        }
        // check for last position
        if (cursor % 16 == 15){
            // update all stored chars
            storedSymbols[cursor / 16][cursor % 16] = symbol;

            // update display
            Lcd_Chr(displayRowPos, cursor % 16, symbol);

            // update cursors
            cursor++;
            displayRowPos = 1;

            if (cursor < MAX_ROWS * 16 && storedSymbols[cursor / 16][cursor % 16] == '\0') {
                storedSymbols[cursor / 16][cursor % 16] = '\n';
            }

            Lcd_Cursor(displayRowPos, cursor % 16);
        }

        else {
            storedSymbols[cursor / 16][cursor % 16] = symbol;
            Lcd_Chr(displayRowPos, cursor % 16, symbol);
            cursor++;
            Lcd_Cursor(displayRowPos, cursor % 16);
        }
    }
}




void main() {
    cursor, displayRowPos = 0;
    
    // filling stored symbols with null-byte
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            storedSymbols[row][col] = '\0';
        }
    }

    Lcd_Init();
    delay(1000);

    UART1_Init(4800); 
    delay(1000);

    while(1) {
        if (UART1_Data_Ready()) {       // If data is received,
            uart_rd = UART1_Read();     // read the received data,
            UART1_Write(uart_rd);       // and send data via UART

            setSymbol(uart_rd);         // execute command on LCD
            delay(2000);
        }
    }

}