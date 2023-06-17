
_delay:
;Lcd.c,21 :: 		void delay(short int ms) {
;Lcd.c,22 :: 		for (i=0; i<ms; i++){};
	MOV _i+0, #0
L_delay0:
	CLR C
	MOV A, FARG_delay_ms+0
	XRL A, #128
	MOV R0, A
	MOV A, _i+0
	XRL A, #128
	SUBB A, R0
	JNC L_delay1
	INC _i+0
	SJMP L_delay0
L_delay1:
;Lcd.c,23 :: 		}
	RET
; end of _delay

_CMD_LCD:
;Lcd.c,28 :: 		void CMD_LCD(char cmd) {
;Lcd.c,29 :: 		LcdDataBus = (cmd & 0xF0);     // Send higher half-byte
	MOV A, FARG_CMD_LCD_cmd+0
	ANL A, #240
	MOV P3+0, A
;Lcd.c,30 :: 		LCD_RS = 0;                    // Send LOW pulse on RS pin for selecting Command register
	CLR P0_4_bit+0
;Lcd.c,31 :: 		LCD_RW = 0;                    // Send LOW pulse on RW pin for Write operation
	CLR P0_5_bit+0
;Lcd.c,32 :: 		LCD_EN = 1;                    // Generate a High-to-low pulse on EN pin
	SETB P0_6_bit+0
;Lcd.c,33 :: 		delay(100);
	MOV FARG_delay_ms+0, #100
	LCALL _delay+0
;Lcd.c,34 :: 		LCD_EN = 0;
	CLR P0_6_bit+0
;Lcd.c,36 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,38 :: 		LcdDataBus = ((cmd<<4) & 0xF0); //Send Lower half-byte
	MOV R0, #4
	MOV A, FARG_CMD_LCD_cmd+0
	INC R0
	SJMP L__CMD_LCD17
L__CMD_LCD18:
	CLR C
	RLC A
L__CMD_LCD17:
	DJNZ R0, L__CMD_LCD18
	MOV R1, A
	MOV R0, #240
	MOV A, R1
	ANL A, R0
	MOV P3+0, A
;Lcd.c,39 :: 		LCD_RS = 0;                     // Send LOW pulse on RS pin for selecting Command register
	CLR P0_4_bit+0
;Lcd.c,40 :: 		LCD_RW = 0;                     // Send LOW pulse on RW pin for Write operation
	CLR P0_5_bit+0
;Lcd.c,41 :: 		LCD_EN = 1;                     // Generate a High-to-low pulse on EN pin
	SETB P0_6_bit+0
;Lcd.c,42 :: 		delay(100);
	MOV FARG_delay_ms+0, #100
	LCALL _delay+0
;Lcd.c,43 :: 		LCD_EN = 0;
	CLR P0_6_bit+0
;Lcd.c,45 :: 		delay(100);
	MOV FARG_delay_ms+0, #100
	LCALL _delay+0
;Lcd.c,46 :: 		}
	RET
; end of _CMD_LCD

_Lcd_Write:
;Lcd.c,49 :: 		void Lcd_Write(char symbol) {
;Lcd.c,50 :: 		LcdDataBus = (symbol & 0xF0);      //Send higher nibble
	MOV A, FARG_Lcd_Write_symbol+0
	ANL A, #240
	MOV P3+0, A
;Lcd.c,51 :: 		LCD_RS = 1;                        // Send HIGH pulse on RS pin for selecting data register
	SETB P0_4_bit+0
;Lcd.c,52 :: 		LCD_RW = 0;                        // Send LOW pulse on RW pin for Write operation
	CLR P0_5_bit+0
;Lcd.c,53 :: 		LCD_EN = 1;                        // Generate a High-to-low pulse on EN pin
	SETB P0_6_bit+0
;Lcd.c,54 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,55 :: 		LCD_EN = 0;
	CLR P0_6_bit+0
;Lcd.c,57 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,59 :: 		LcdDataBus = ((symbol<<4) & 0xF0);  // Send Lower nibble
	MOV R0, #4
	MOV A, FARG_Lcd_Write_symbol+0
	INC R0
	SJMP L__Lcd_Write19
L__Lcd_Write20:
	CLR C
	RLC A
L__Lcd_Write19:
	DJNZ R0, L__Lcd_Write20
	MOV R1, A
	MOV R0, #240
	MOV A, R1
	ANL A, R0
	MOV P3+0, A
;Lcd.c,60 :: 		LCD_RS = 1;                         // Send HIGH pulse on RS pin for selecting data register
	SETB P0_4_bit+0
;Lcd.c,61 :: 		LCD_RW = 0;                         // Send LOW pulse on RW pin for Write operation
	CLR P0_5_bit+0
;Lcd.c,62 :: 		LCD_EN = 1;                         // Generate a High-to-low pulse on EN pin
	SETB P0_6_bit+0
;Lcd.c,63 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,64 :: 		LCD_EN = 0;
	CLR P0_6_bit+0
;Lcd.c,66 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,67 :: 		}
	RET
; end of _Lcd_Write

_Lcd_Cursor:
;Lcd.c,70 :: 		void Lcd_Cursor(int row, int col) {
;Lcd.c,71 :: 		char pos = 0;
	MOV Lcd_Cursor_pos_L0+0, #0
;Lcd.c,72 :: 		if (row == 0)
	MOV A, FARG_Lcd_Cursor_row+0
	ORL A, FARG_Lcd_Cursor_row+1
	JNZ L_Lcd_Cursor3
;Lcd.c,73 :: 		pos = 0x80 + (col);  // First line
	MOV A, #128
	ADD A, FARG_Lcd_Cursor_col+0
	MOV Lcd_Cursor_pos_L0+0, A
	SJMP L_Lcd_Cursor4
L_Lcd_Cursor3:
;Lcd.c,74 :: 		else if (row == 1)
	MOV A, #1
	XRL A, FARG_Lcd_Cursor_row+0
	JNZ L__Lcd_Cursor21
	MOV A, #0
	XRL A, FARG_Lcd_Cursor_row+1
L__Lcd_Cursor21:
	JNZ L_Lcd_Cursor5
;Lcd.c,75 :: 		pos = 0xC0 + (col);  // Second line
	MOV A, #192
	ADD A, FARG_Lcd_Cursor_col+0
	MOV Lcd_Cursor_pos_L0+0, A
L_Lcd_Cursor5:
L_Lcd_Cursor4:
;Lcd.c,77 :: 		CMD_LCD(pos);                // Change cursor position
	MOV FARG_CMD_LCD_cmd+0, Lcd_Cursor_pos_L0+0
	LCALL _CMD_LCD+0
;Lcd.c,78 :: 		}
	RET
; end of _Lcd_Cursor

_CHR_LCD:
;Lcd.c,81 :: 		void CHR_LCD(int row, int col, char symbol) {
;Lcd.c,82 :: 		Lcd_Cursor(row, col);          // Update cursor position
	MOV FARG_Lcd_Cursor_row+0, FARG_CHR_LCD_row+0
	MOV FARG_Lcd_Cursor_row+1, FARG_CHR_LCD_row+1
	MOV FARG_Lcd_Cursor_col+0, FARG_CHR_LCD_col+0
	MOV FARG_Lcd_Cursor_col+1, FARG_CHR_LCD_col+1
	LCALL _Lcd_Cursor+0
;Lcd.c,83 :: 		Lcd_Write(symbol);             // Write char to LCD
	MOV FARG_Lcd_Write_symbol+0, FARG_CHR_LCD_symbol+0
	LCALL _Lcd_Write+0
;Lcd.c,84 :: 		}
	RET
; end of _CHR_LCD

_INIT_LCD:
;Lcd.c,87 :: 		void INIT_LCD() {
;Lcd.c,88 :: 		CMD_LCD(0x02);        // Move the cursor to home position
	MOV FARG_CMD_LCD_cmd+0, #2
	LCALL _CMD_LCD+0
;Lcd.c,89 :: 		CMD_LCD(0x28);        // Initialize LCD in 4-bit mode, enable 5x7 mode for chars
	MOV FARG_CMD_LCD_cmd+0, #40
	LCALL _CMD_LCD+0
;Lcd.c,90 :: 		CMD_LCD(0x0E);        // 0x0E Display OFF, Cursor ON (underline)
	MOV FARG_CMD_LCD_cmd+0, #14
	LCALL _CMD_LCD+0
;Lcd.c,91 :: 		CMD_LCD(0x01);        // Clear Display
	MOV FARG_CMD_LCD_cmd+0, #1
	LCALL _CMD_LCD+0
;Lcd.c,92 :: 		CMD_LCD(0x80);        // Move the cursor to beginning of first line
	MOV FARG_CMD_LCD_cmd+0, #128
	LCALL _CMD_LCD+0
;Lcd.c,93 :: 		}
	RET
; end of _INIT_LCD

_UART_Init:
;Lcd.c,97 :: 		void UART_Init()
;Lcd.c,99 :: 		SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
	MOV SCON+0, #80
;Lcd.c,100 :: 		TMOD = 0x20;  //Timer1 in Mode2.
	MOV TMOD+0, #32
;Lcd.c,101 :: 		TH1 = 0xF5;
	MOV TH1+0, #245
;Lcd.c,103 :: 		TR1_bit = 1;      //Turn ON the timer for Baud rate generation
	SETB TR1_bit+0
;Lcd.c,104 :: 		}
	RET
; end of _UART_Init

_UART_WRITEC:
;Lcd.c,106 :: 		void UART_WRITEC(char ch)
;Lcd.c,108 :: 		SBUF = ch;      // Load the data to be transmitted
	MOV SBUF+0, FARG_UART_WRITEC_ch+0
;Lcd.c,109 :: 		while(TI_bit==0);   // Wait till the data is trasmitted
L_UART_WRITEC6:
	JB TI_bit+0, L_UART_WRITEC7
	NOP
	SJMP L_UART_WRITEC6
L_UART_WRITEC7:
;Lcd.c,110 :: 		TI_bit = 0;         //Clear the Tx flag for next cycle.
	CLR TI_bit+0
;Lcd.c,111 :: 		}
	RET
; end of _UART_WRITEC

_UART_READC:
;Lcd.c,114 :: 		char UART_READC(void)
;Lcd.c,116 :: 		while(RI_bit==0);     // Wait till the data is received
L_UART_READC8:
	JB RI_bit+0, L_UART_READC9
	NOP
	SJMP L_UART_READC8
L_UART_READC9:
;Lcd.c,117 :: 		RI_bit=0;             // Clear Receive Interrupt Flag for next cycle
	CLR RI_bit+0
;Lcd.c,118 :: 		return(SBUF);     // return the received char
	MOV R0, SBUF+0
;Lcd.c,119 :: 		}
	RET
; end of _UART_READC

_UART_RxString:
;Lcd.c,120 :: 		void UART_RxString()
;Lcd.c,123 :: 		i = 0;
	MOV _i+0, #0
;Lcd.c,127 :: 		while(1)
L_UART_RxString10:
;Lcd.c,129 :: 		ch=UART_READC();    //Reaceive a char
	LCALL _UART_READC+0
;Lcd.c,140 :: 		Lcd_Write(ch);
	MOV FARG_Lcd_Write_symbol+0, 0
	LCALL _Lcd_Write+0
;Lcd.c,142 :: 		i++;
	INC _i+0
;Lcd.c,145 :: 		}
	SJMP L_UART_RxString10
;Lcd.c,146 :: 		}
	RET
; end of _UART_RxString

_main:
	MOV SP+0, #128
;Lcd.c,148 :: 		void main() {
;Lcd.c,149 :: 		for (i=0;i<32;i++) {
	MOV _i+0, #0
L_main12:
	CLR C
	MOV A, #32
	XRL A, #128
	MOV R0, A
	MOV A, _i+0
	XRL A, #128
	SUBB A, R0
	JNC L_main13
;Lcd.c,150 :: 		sigma[i] = ' ';
	MOV A, #_sigma+0
	ADD A, _i+0
	MOV R0, A
	MOV @R0, #32
;Lcd.c,149 :: 		for (i=0;i<32;i++) {
	INC _i+0
;Lcd.c,151 :: 		}
	SJMP L_main12
L_main13:
;Lcd.c,153 :: 		Lcd_Init();                        // Initialize LCD
	LCALL _Lcd_Init+0
;Lcd.c,155 :: 		Lcd_Cmd(_LCD_CLEAR);               // Clear display
	MOV FARG_Lcd_Cmd_out_char+0, #1
	LCALL _Lcd_Cmd+0
;Lcd.c,156 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
	MOV FARG_Lcd_Cmd_out_char+0, #12
	LCALL _Lcd_Cmd+0
;Lcd.c,160 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,163 :: 		UART_Init();
	LCALL _UART_Init+0
;Lcd.c,164 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,170 :: 		while (1) {
L_main15:
;Lcd.c,172 :: 		k = UART_READC();
	LCALL _UART_READC+0
	MOV _k+0, 0
;Lcd.c,173 :: 		UART_WRITEC(k);
	MOV FARG_UART_WRITEC_ch+0, 0
	LCALL _UART_WRITEC+0
;Lcd.c,174 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,175 :: 		Lcd_Chr_Cp(k);
	MOV FARG_Lcd_Chr_CP_out_char+0, _k+0
	LCALL _Lcd_Chr_CP+0
;Lcd.c,176 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,180 :: 		}
	SJMP L_main15
;Lcd.c,181 :: 		}
	SJMP #254
; end of _main
