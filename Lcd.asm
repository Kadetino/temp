
_main:
	MOV SP+0, #128
;Lcd.c,32 :: 		void main(){
;Lcd.c,34 :: 		Lcd_Init();                        // Initialize LCD
	LCALL _Lcd_Init+0
;Lcd.c,36 :: 		Lcd_Cmd(_LCD_CLEAR);               // Clear display
	MOV FARG_Lcd_Cmd_out_char+0, #1
	LCALL _Lcd_Cmd+0
;Lcd.c,38 :: 		Lcd_Cmd(_LCD_UNDERLINE_ON);
	MOV FARG_Lcd_Cmd_out_char+0, #14
	LCALL _Lcd_Cmd+0
;Lcd.c,41 :: 		UART1_Init(4800);               // Initialize UART module at 4800 bps
	ORL PCON+0, #128
	MOV TH1+0, #245
	MOV TL1+0, #245
	LCALL _UART1_Init+0
;Lcd.c,42 :: 		Delay_ms(500);                  // Wait for UART module to stabilize
	MOV R5, 4
	MOV R6, 43
	MOV R7, 157
	DJNZ R7, 
	DJNZ R6, 
	DJNZ R5, 
;Lcd.c,45 :: 		cur_col = 1;
	MOV _cur_col+0, #1
;Lcd.c,46 :: 		cur_row = 1;
	MOV _cur_row+0, #1
;Lcd.c,47 :: 		while (1) {                     // Endless loop
L_main0:
;Lcd.c,48 :: 		if (UART1_Data_Ready()) {     // If data is received,
	LCALL _UART1_Data_Ready+0
	MOV A, R0
	JNZ #3
	LJMP L_main2
;Lcd.c,49 :: 		uart_rd = UART1_Read();     //   read the received data,
	LCALL _UART1_Read+0
	MOV _uart_rd+0, 0
;Lcd.c,64 :: 		if ((97 <= uart_rd && uart_rd <= 122)||
	SETB C
	MOV A, #97
	SUBB A, R0
	JNC L__main28
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #122
	JNC L__main28
	SJMP L__main22
;Lcd.c,65 :: 		(65 <= uart_rd && uart_rd <= 90) ||
L__main28:
	SETB C
	MOV A, #65
	SUBB A, _uart_rd+0
	JNC L__main27
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #90
	JNC L__main27
	SJMP L__main22
L__main27:
;Lcd.c,66 :: 		(48 <= uart_rd && uart_rd <= 57) ||
	SETB C
	MOV A, #48
	SUBB A, _uart_rd+0
	JNC L__main26
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #57
	JNC L__main26
	SJMP L__main22
L__main26:
;Lcd.c,67 :: 		(33 <= uart_rd && uart_rd <= 47) ||
	SETB C
	MOV A, #33
	SUBB A, _uart_rd+0
	JNC L__main25
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #47
	JNC L__main25
	SJMP L__main22
L__main25:
;Lcd.c,68 :: 		(58 <= uart_rd && uart_rd <= 64) ||
	SETB C
	MOV A, #58
	SUBB A, _uart_rd+0
	JNC L__main24
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #64
	JNC L__main24
	SJMP L__main22
L__main24:
;Lcd.c,69 :: 		(91 <= uart_rd && uart_rd <= 96))
	SETB C
	MOV A, #91
	SUBB A, _uart_rd+0
	JNC L__main23
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #96
	JNC L__main23
	SJMP L__main22
L__main23:
	SJMP L_main17
L__main22:
;Lcd.c,71 :: 		UART1_Write(uart_rd);
	MOV FARG_UART1_Write_data_+0, _uart_rd+0
	LCALL _UART1_Write+0
;Lcd.c,72 :: 		LCD_Chr(cur_row,cur_col,uart_rd);
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, _cur_col+0
	MOV FARG_Lcd_Chr_out_char+0, _uart_rd+0
	LCALL _Lcd_Chr+0
;Lcd.c,73 :: 		cur_col+=1;
	INC _cur_col+0
;Lcd.c,74 :: 		}
L_main17:
;Lcd.c,79 :: 		if (uart_rd == 13) // Enter  13
	MOV A, _uart_rd+0
	XRL A, #13
	JNZ L_main18
;Lcd.c,81 :: 		UART1_Write(13);
	MOV FARG_UART1_Write_data_+0, #13
	LCALL _UART1_Write+0
;Lcd.c,82 :: 		cur_row +=1;
	INC _cur_row+0
;Lcd.c,83 :: 		if (cur_col != 1) cur_col = 1; //
	MOV A, _cur_col+0
	XRL A, #1
	JZ L_main19
	MOV _cur_col+0, #1
L_main19:
;Lcd.c,84 :: 		}
L_main18:
;Lcd.c,85 :: 		if (uart_rd == 8) // Backspace  8
	MOV A, _uart_rd+0
	XRL A, #8
	JNZ L_main20
;Lcd.c,87 :: 		UART1_Write(8);
	MOV FARG_UART1_Write_data_+0, #8
	LCALL _UART1_Write+0
;Lcd.c,88 :: 		cur_col-=1;
	DEC _cur_col+0
;Lcd.c,89 :: 		}
L_main20:
;Lcd.c,90 :: 		if (uart_rd == 127) // Delete  127
	MOV A, _uart_rd+0
	XRL A, #127
	JNZ L_main21
;Lcd.c,91 :: 		{UART1_Write(127);}
	MOV FARG_UART1_Write_data_+0, #127
	LCALL _UART1_Write+0
L_main21:
;Lcd.c,99 :: 		}
L_main2:
;Lcd.c,100 :: 		}
	LJMP L_main0
;Lcd.c,101 :: 		}
	SJMP #254
; end of _main
