
_delay:
;Lcd.c,30 :: 		void delay(short int ms) {
;Lcd.c,31 :: 		for (i=0; i<ms; i++){};
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
;Lcd.c,32 :: 		}
	RET
; end of _delay

_LCD_CmdWrite:
;Lcd.c,35 :: 		void LCD_CmdWrite(char cmd)
;Lcd.c,38 :: 		databus=(cmd & 0xf0);        // Send the Higher Nibble of the command to LCD
	MOV A, FARG_LCD_CmdWrite_cmd+0
	ANL A, #240
	MOV P2+0, A
;Lcd.c,39 :: 		rs=0;                      // Select the Command Register by pulling RS LOW
	CLR P0_4_bit+0
;Lcd.c,40 :: 		rw=0;                      // Select the Write Operation  by pulling RW LOW
	CLR P0_5_bit+0
;Lcd.c,41 :: 		en=1;                      // Send a High-to-Low Pusle at Enable Pin
	SETB P0_6_bit+0
;Lcd.c,42 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,43 :: 		en=0;
	CLR P0_6_bit+0
;Lcd.c,45 :: 		delay(1000);                   // wait for some time
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,47 :: 		databus=((cmd<<4) & 0xf0);   // Send the Lower Nibble of the command to LCD
	MOV R0, #4
	MOV A, FARG_LCD_CmdWrite_cmd+0
	INC R0
	SJMP L__LCD_CmdWrite20
L__LCD_CmdWrite21:
	CLR C
	RLC A
L__LCD_CmdWrite20:
	DJNZ R0, L__LCD_CmdWrite21
	MOV R1, A
	MOV R0, #240
	MOV A, R1
	ANL A, R0
	MOV P2+0, A
;Lcd.c,48 :: 		rs=0;                      // Select the Command Register by pulling RS LOW
	CLR P0_4_bit+0
;Lcd.c,49 :: 		rw=0;                      // Select the Write Operation  by pulling RW LOW
	CLR P0_5_bit+0
;Lcd.c,50 :: 		en=1;                      // Send a High-to-Low Pusle at Enable Pin
	SETB P0_6_bit+0
;Lcd.c,51 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,52 :: 		en=0;
	CLR P0_6_bit+0
;Lcd.c,53 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,54 :: 		}
	RET
; end of _LCD_CmdWrite

_Lcd_Cursor:
;Lcd.c,57 :: 		void Lcd_Cursor(int row, int col) {
;Lcd.c,59 :: 		char pos = 0;
	MOV Lcd_Cursor_pos_L0+0, #0
;Lcd.c,60 :: 		if (row == 0)
	MOV A, FARG_Lcd_Cursor_row+0
	ORL A, FARG_Lcd_Cursor_row+1
	JNZ L_Lcd_Cursor3
;Lcd.c,61 :: 		pos = 0x80 + (col);  // First line
	MOV A, #128
	ADD A, FARG_Lcd_Cursor_col+0
	MOV Lcd_Cursor_pos_L0+0, A
	SJMP L_Lcd_Cursor4
L_Lcd_Cursor3:
;Lcd.c,62 :: 		else if (row == 1)
	MOV A, #1
	XRL A, FARG_Lcd_Cursor_row+0
	JNZ L__Lcd_Cursor22
	MOV A, #0
	XRL A, FARG_Lcd_Cursor_row+1
L__Lcd_Cursor22:
	JNZ L_Lcd_Cursor5
;Lcd.c,63 :: 		pos = 0xC0 + (col);  // Second line
	MOV A, #192
	ADD A, FARG_Lcd_Cursor_col+0
	MOV Lcd_Cursor_pos_L0+0, A
L_Lcd_Cursor5:
L_Lcd_Cursor4:
;Lcd.c,64 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,65 :: 		LCD_CmdWrite(pos);                // Change cursor position
	MOV FARG_LCD_CmdWrite_cmd+0, Lcd_Cursor_pos_L0+0
	LCALL _LCD_CmdWrite+0
;Lcd.c,66 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,67 :: 		}
	RET
; end of _Lcd_Cursor

_LCD_Init1:
;Lcd.c,69 :: 		void LCD_Init1()
;Lcd.c,71 :: 		delay_us(5000);
	MOV R6, 9
	MOV R7, 25
	DJNZ R7, 
	DJNZ R6, 
	NOP
;Lcd.c,72 :: 		LCD_CmdWrite(0x02);  //Initilize the LCD in 4bit Mode
	MOV FARG_LCD_CmdWrite_cmd+0, #2
	LCALL _LCD_CmdWrite+0
;Lcd.c,73 :: 		LCD_CmdWrite(0x28);
	MOV FARG_LCD_CmdWrite_cmd+0, #40
	LCALL _LCD_CmdWrite+0
;Lcd.c,74 :: 		LCD_CmdWrite(12);  // 0x0E Display ON cursor ON
	MOV FARG_LCD_CmdWrite_cmd+0, #12
	LCALL _LCD_CmdWrite+0
;Lcd.c,75 :: 		LCD_CmdWrite(0x01);  // Clear the LCD
	MOV FARG_LCD_CmdWrite_cmd+0, #1
	LCALL _LCD_CmdWrite+0
;Lcd.c,76 :: 		LCD_CmdWrite(0x80);  // Move the Cursor to First line First Position
	MOV FARG_LCD_CmdWrite_cmd+0, #128
	LCALL _LCD_CmdWrite+0
;Lcd.c,78 :: 		}
	RET
; end of _LCD_Init1

_LCD_DataWrite:
;Lcd.c,80 :: 		void LCD_DataWrite( char dat)
;Lcd.c,84 :: 		databus=(dat & 0xf0);        // Send the Higher Nibble of the Data to LCD
	MOV A, FARG_LCD_DataWrite_dat+0
	ANL A, #240
	MOV P2+0, A
;Lcd.c,85 :: 		rs=1;                      // Select the Data Register by pulling RS HIGH
	SETB P0_4_bit+0
;Lcd.c,86 :: 		rw=0;                      // Select the Write Operation  by pulling RW LOW
	CLR P0_5_bit+0
;Lcd.c,87 :: 		en=1;                      // Send a High-to-Low Pusle at Enable Pin
	SETB P0_6_bit+0
;Lcd.c,88 :: 		delay_us(1);
	NOP
;Lcd.c,89 :: 		en=0;
	CLR P0_6_bit+0
;Lcd.c,91 :: 		delay_us(10);                 // wait for some time.
	MOV R7, 3
	DJNZ R7, 
	NOP
;Lcd.c,93 :: 		databus=((dat <<4) & 0xf0);  // Send the Lower Nibble of the Data to LCD
	MOV R0, #4
	MOV A, FARG_LCD_DataWrite_dat+0
	INC R0
	SJMP L__LCD_DataWrite23
L__LCD_DataWrite24:
	CLR C
	RLC A
L__LCD_DataWrite23:
	DJNZ R0, L__LCD_DataWrite24
	MOV R1, A
	MOV R0, #240
	MOV A, R1
	ANL A, R0
	MOV P2+0, A
;Lcd.c,94 :: 		rs=1;                      // Select the Data Register by pulling RS HIGH
	SETB P0_4_bit+0
;Lcd.c,95 :: 		rw=0;                      // Select the Write Operation  by pulling RW LOW
	CLR P0_5_bit+0
;Lcd.c,96 :: 		en=1;                      // Send a High-to-Low Pusle at Enable Pin
	SETB P0_6_bit+0
;Lcd.c,97 :: 		delay_us(1);
	NOP
;Lcd.c,98 :: 		en=0;
	CLR P0_6_bit+0
;Lcd.c,100 :: 		delay_ms(1);
	MOV R6, 2
	MOV R7, 157
	DJNZ R7, 
	DJNZ R6, 
	NOP
;Lcd.c,102 :: 		}
	RET
; end of _LCD_DataWrite

_UART_Init:
;Lcd.c,105 :: 		void UART_Init()
;Lcd.c,107 :: 		SCON = 0x50;  // Asynchronous mode, 8-bit data and 1-stop bit
	MOV SCON+0, #80
;Lcd.c,108 :: 		TMOD = 0x20;  //Timer1 in Mode2.
	MOV TMOD+0, #32
;Lcd.c,109 :: 		TH1 = 0xF5;
	MOV TH1+0, #245
;Lcd.c,111 :: 		TR1_bit = 1;      //Turn ON the timer for Baud rate generation
	SETB TR1_bit+0
;Lcd.c,112 :: 		}
	RET
; end of _UART_Init

_UART_WRITEC:
;Lcd.c,114 :: 		void UART_WRITEC(char ch)
;Lcd.c,116 :: 		SBUF = ch;      // Load the data to be transmitted
	MOV SBUF+0, FARG_UART_WRITEC_ch+0
;Lcd.c,117 :: 		while(TI_bit==0);   // Wait till the data is trasmitted
L_UART_WRITEC6:
	JB TI_bit+0, L_UART_WRITEC7
	NOP
	SJMP L_UART_WRITEC6
L_UART_WRITEC7:
;Lcd.c,118 :: 		TI_bit = 0;         //Clear the Tx flag for next cycle.
	CLR TI_bit+0
;Lcd.c,119 :: 		}
	RET
; end of _UART_WRITEC

_UART_READC:
;Lcd.c,122 :: 		char UART_READC(void)
;Lcd.c,124 :: 		while(RI_bit==0);     // Wait till the data is received
L_UART_READC8:
	JB RI_bit+0, L_UART_READC9
	NOP
	SJMP L_UART_READC8
L_UART_READC9:
;Lcd.c,125 :: 		RI_bit=0;             // Clear Receive Interrupt Flag for next cycle
	CLR RI_bit+0
;Lcd.c,126 :: 		return(SBUF);     // return the received char
	MOV R0, SBUF+0
;Lcd.c,127 :: 		}
	RET
; end of _UART_READC

_UART_RxString:
;Lcd.c,128 :: 		void UART_RxString()
;Lcd.c,132 :: 		while(1)
L_UART_RxString10:
;Lcd.c,134 :: 		ch=UART_READC();    //Reaceive a char
	LCALL _UART_READC+0
	MOV UART_RxString_ch_L0+0, 0
;Lcd.c,135 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,136 :: 		if((ch=='\r') || (ch=='\n')) //read till enter key is pressed
	MOV A, UART_RxString_ch_L0+0
	XRL A, #13
	JZ L__UART_RxString19
	MOV A, UART_RxString_ch_L0+0
	XRL A, #10
	JZ L__UART_RxString19
	SJMP L_UART_RxString14
L__UART_RxString19:
;Lcd.c,138 :: 		break;                //and break the loop
	SJMP L_UART_RxString11
;Lcd.c,139 :: 		}
L_UART_RxString14:
;Lcd.c,140 :: 		*data_uart=ch;              //copy the char into string.
	MOV R0, _data_uart+0
	MOV @R0, UART_RxString_ch_L0+0
;Lcd.c,142 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,143 :: 		LCD_DataWrite(ch);
	MOV FARG_LCD_DataWrite_dat+0, UART_RxString_ch_L0+0
	LCALL _LCD_DataWrite+0
;Lcd.c,144 :: 		delay(10000);
	MOV FARG_delay_ms+0, #16
	LCALL _delay+0
;Lcd.c,146 :: 		data_uart++;
	INC _data_uart+0
;Lcd.c,147 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,148 :: 		}
	SJMP L_UART_RxString10
L_UART_RxString11:
;Lcd.c,149 :: 		}
	RET
; end of _UART_RxString

_main:
	MOV SP+0, #128
;Lcd.c,152 :: 		void main() {
;Lcd.c,154 :: 		LCD_Init1();                        // Initialize LCD
	LCALL _LCD_Init1+0
;Lcd.c,155 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,156 :: 		UART_Init();
	LCALL _UART_Init+0
;Lcd.c,157 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,159 :: 		delay(1000);
	MOV FARG_delay_ms+0, #232
	LCALL _delay+0
;Lcd.c,166 :: 		j = 0;
	MOV _j+0, #0
;Lcd.c,168 :: 		while (1) {
L_main15:
;Lcd.c,169 :: 		uart_rd = UART_READC();
	LCALL _UART_READC+0
	MOV _uart_rd+0, 0
;Lcd.c,170 :: 		if (uart_rd == '\r') {
	MOV A, R0
	XRL A, #13
	JNZ L_main17
;Lcd.c,171 :: 		j = 0;
	MOV _j+0, #0
;Lcd.c,172 :: 		Lcd_Cursor(0, 0);
	MOV FARG_Lcd_Cursor_row+0, #0
	MOV FARG_Lcd_Cursor_row+1, #0
	MOV FARG_Lcd_Cursor_col+0, #0
	MOV FARG_Lcd_Cursor_col+1, #0
	LCALL _Lcd_Cursor+0
;Lcd.c,173 :: 		continue;
	SJMP L_main15
;Lcd.c,174 :: 		}
L_main17:
;Lcd.c,175 :: 		if (j == 16) {
	MOV A, _j+0
	XRL A, #16
	JNZ L_main18
;Lcd.c,176 :: 		Lcd_Cursor(1, 0);
	MOV FARG_Lcd_Cursor_row+0, #1
	MOV FARG_Lcd_Cursor_row+1, #0
	MOV FARG_Lcd_Cursor_col+0, #0
	MOV FARG_Lcd_Cursor_col+1, #0
	LCALL _Lcd_Cursor+0
;Lcd.c,177 :: 		}
L_main18:
;Lcd.c,178 :: 		LCD_DataWrite(uart_rd);
	MOV FARG_LCD_DataWrite_dat+0, _uart_rd+0
	LCALL _LCD_DataWrite+0
;Lcd.c,179 :: 		j++;
	INC _j+0
;Lcd.c,195 :: 		}
	SJMP L_main15
;Lcd.c,196 :: 		}
	SJMP #254
; end of _main
