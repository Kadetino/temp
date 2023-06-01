
_Move_Delay:
;Lcd.c,43 :: 		void Move_Delay() {                  // Function used for text moving
;Lcd.c,44 :: 		Delay_ms(500);                     // You can change the moving speed here
	MOV R5, 4
	MOV R6, 43
	MOV R7, 157
	DJNZ R7, 
	DJNZ R6, 
	DJNZ R5, 
;Lcd.c,45 :: 		}
	RET
; end of _Move_Delay

_main:
	MOV SP+0, #128
;Lcd.c,50 :: 		void main(){
;Lcd.c,52 :: 		Lcd_Init();                        // Initialize LCD
	LCALL _Lcd_Init+0
;Lcd.c,54 :: 		Lcd_Cmd(_LCD_CLEAR);               // Clear display
	MOV FARG_Lcd_Cmd_out_char+0, #1
	LCALL _Lcd_Cmd+0
;Lcd.c,55 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);          // Cursor off
	MOV FARG_Lcd_Cmd_out_char+0, #12
	LCALL _Lcd_Cmd+0
;Lcd.c,73 :: 		UART1_Init(4800);               // Initialize UART module at 4800 bps
	ORL PCON+0, #128
	MOV TH1+0, #245
	MOV TL1+0, #245
	LCALL _UART1_Init+0
;Lcd.c,74 :: 		Delay_ms(100);                  // Wait for UART module to stabilize
	MOV R6, 163
	MOV R7, 30
	DJNZ R7, 
	DJNZ R6, 
	NOP
;Lcd.c,77 :: 		while (1) {                     // Endless loop
L_main0:
;Lcd.c,78 :: 		if (UART1_Data_Ready()) {     // If data is received,
	LCALL _UART1_Data_Ready+0
	MOV A, R0
	JZ L_main2
;Lcd.c,79 :: 		uart_rd = UART1_Read();     //   read the received data,
	LCALL _UART1_Read+0
	MOV _uart_rd+0, 0
;Lcd.c,80 :: 		UART1_Write(uart_rd);       //   and send data via UART
	MOV FARG_UART1_Write_data_+0, 0
	LCALL _UART1_Write+0
;Lcd.c,81 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOV FARG_Lcd_Cmd_out_char+0, #1
	LCALL _Lcd_Cmd+0
;Lcd.c,82 :: 		LCD_Chr(2,1,uart_rd);
	MOV FARG_Lcd_Chr_row+0, #2
	MOV FARG_Lcd_Chr_column+0, #1
	MOV FARG_Lcd_Chr_out_char+0, _uart_rd+0
	LCALL _Lcd_Chr+0
;Lcd.c,83 :: 		}
L_main2:
;Lcd.c,84 :: 		}
	SJMP L_main0
;Lcd.c,99 :: 		}
	SJMP #254
; end of _main
