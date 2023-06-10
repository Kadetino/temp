
_var_preset:
;Lcd.c,36 :: 		void var_preset(){
;Lcd.c,38 :: 		cur_col = 1; // Настроить курсор столбцов
	MOV _cur_col+0, #1
;Lcd.c,39 :: 		cur_row = 1; // Настроить курсор строк
	MOV _cur_row+0, #1
;Lcd.c,40 :: 		}
	RET
; end of _var_preset

_temptemp:
;Lcd.c,42 :: 		void temptemp(short int target_row, short int new_row){
;Lcd.c,44 :: 		strncpy(temp,storage[(new_row-1)*LASTCOL_LCD],LASTCOL_LCD);
	MOV FARG_strncpy_to+0, #temptemp_temp_L0+0
	MOV R0, FARG_temptemp_new_row+0
	MOV A, FARG_temptemp_new_row+0
	RLC A
	CLR A
	SUBB A, 224
	MOV R1, A
	CLR C
	MOV A, R0
	SUBB A, #1
	MOV R3, A
	MOV A, R1
	SUBB A, #0
	MOV R4, A
	MOV R2, #4
	MOV R1, 4
	MOV A, R3
	INC R2
	SJMP L__temptemp34
L__temptemp35:
	CLR C
	RLC A
	XCH A, R1
	RLC A
	XCH A, R1
L__temptemp34:
	DJNZ R2, L__temptemp35
	MOV R0, A
	MOV A, #_storage+0
	ADD A, R0
	MOV R0, A
	MOV FARG_strncpy_from+0, @R0
	MOV FARG_strncpy_size+0, #16
	MOV FARG_strncpy_size+1, #0
	LCALL _strncpy+0
;Lcd.c,45 :: 		Lcd_Out(target_row, 1, temp);
	MOV FARG_LCD_Out_row+0, FARG_temptemp_target_row+0
	MOV FARG_LCD_Out_column+0, #1
	MOV FARG_LCD_Out_text+0, #temptemp_temp_L0+0
	LCALL _LCD_Out+0
;Lcd.c,46 :: 		}
	RET
; end of _temptemp

_main:
	MOV SP+0, #128
;Lcd.c,48 :: 		void main(){
;Lcd.c,49 :: 		var_preset(); // свои переменные
	LCALL _var_preset+0
;Lcd.c,52 :: 		Lcd_Init();
	LCALL _Lcd_Init+0
;Lcd.c,53 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOV FARG_Lcd_Cmd_out_char+0, #1
	LCALL _Lcd_Cmd+0
;Lcd.c,54 :: 		Lcd_Cmd(_LCD_UNDERLINE_ON);
	MOV FARG_Lcd_Cmd_out_char+0, #14
	LCALL _Lcd_Cmd+0
;Lcd.c,55 :: 		Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
	MOV FARG_Lcd_Cmd_out_char+0, #15
	LCALL _Lcd_Cmd+0
;Lcd.c,59 :: 		UART1_Init(4800); // Установить Baud rate 4800
	ORL PCON+0, #128
	MOV TH1+0, #245
	MOV TL1+0, #245
	LCALL _UART1_Init+0
;Lcd.c,60 :: 		Delay_ms(500); // Wait for UART module to stabilize
	MOV R5, 4
	MOV R6, 43
	MOV R7, 157
	DJNZ R7, 
	DJNZ R6, 
	DJNZ R5, 
;Lcd.c,63 :: 		while (1) {
L_main0:
;Lcd.c,64 :: 		if (UART1_Data_Ready()){
	LCALL _UART1_Data_Ready+0
	MOV A, R0
	JNZ #3
	LJMP L_main2
;Lcd.c,65 :: 		uart_rd = UART1_Read(); // Прочитать полученное
	LCALL _UART1_Read+0
	MOV _uart_rd+0, 0
;Lcd.c,80 :: 		if (32 <= uart_rd && uart_rd <= 122){
	SETB C
	MOV A, #32
	SUBB A, R0
	JNC L_main5
	SETB C
	MOV A, _uart_rd+0
	SUBB A, #122
	JNC L_main5
L__main33:
;Lcd.c,81 :: 		UART1_Write(uart_rd);                                        // Уведомить QT
	MOV FARG_UART1_Write_data_+0, _uart_rd+0
	LCALL _UART1_Write+0
;Lcd.c,82 :: 		if (cur_col < (LASTCOL_LCD + 1)){                                                // В строке ещё есть место
	CLR C
	MOV A, #17
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JNC L_main6
;Lcd.c,83 :: 		LCD_Chr(cur_row,cur_col,uart_rd);        // Напечатать символ
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, _cur_col+0
	MOV FARG_Lcd_Chr_out_char+0, _uart_rd+0
	LCALL _Lcd_Chr+0
;Lcd.c,85 :: 		cur_col+=1;                                                        // Обновить позицию (столбец) курсора
	INC _cur_col+0
;Lcd.c,86 :: 		if (cur_col >= (LASTCOL_LCD + 1)){        // Если это был последний столбец, перенос строки
	CLR C
	MOV A, #17
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JC L_main7
;Lcd.c,87 :: 		if (cur_row >= LASTROW_LCD) cur_col = LASTCOL_LCD; // Последний столбец и последний символ - оставить курсор на месте
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JC L_main8
	MOV _cur_col+0, #16
	SJMP L_main9
L_main8:
;Lcd.c,89 :: 		cur_col = 1;
	MOV _cur_col+0, #1
;Lcd.c,90 :: 		cur_row+= 1;
	INC _cur_row+0
;Lcd.c,91 :: 		}
L_main9:
;Lcd.c,92 :: 		}
L_main7:
;Lcd.c,93 :: 		}
L_main6:
;Lcd.c,108 :: 		}
L_main5:
;Lcd.c,115 :: 		if (uart_rd == KEY_ENTER){
	MOV A, _uart_rd+0
	XRL A, #13
	JNZ L_main10
;Lcd.c,116 :: 		UART1_Write(KEY_ENTER);                                                // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #13
	LCALL _UART1_Write+0
;Lcd.c,117 :: 		if (cur_row < LASTROW_LCD){                                                // Если переход возможен (==количество строк позволяет)
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JNC L_main11
;Lcd.c,118 :: 		cur_row +=1;                                                // Поменять строку
	INC _cur_row+0
;Lcd.c,119 :: 		if (cur_col != 1) cur_col = 1;                 // Переместить курсор на 1й символ, если он был на первом символе
	MOV A, _cur_col+0
	XRL A, #1
	JZ L_main12
	MOV _cur_col+0, #1
L_main12:
;Lcd.c,120 :: 		}
L_main11:
;Lcd.c,121 :: 		}
L_main10:
;Lcd.c,124 :: 		if (uart_rd == KEY_BACKSPACE){
	MOV A, _uart_rd+0
	XRL A, #8
	JNZ L_main13
;Lcd.c,125 :: 		UART1_Write(KEY_BACKSPACE);                                                        // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #8
	LCALL _UART1_Write+0
;Lcd.c,126 :: 		if (cur_col > 1){                                                // Если символ не первый в строке, удалить предыдуший курсору символ и оставить курсор там
	SETB C
	MOV A, #1
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JC L_main14
;Lcd.c,127 :: 		cur_col-=1;                                                        // Вернуть курсор назад
	DEC _cur_col+0
;Lcd.c,128 :: 		LCD_Chr(cur_row,cur_col,KEY_EMPTY);                // Удалить символ
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, _cur_col+0
	MOV FARG_Lcd_Chr_out_char+0, #0
	LCALL _Lcd_Chr+0
;Lcd.c,130 :: 		}
	SJMP L_main15
L_main14:
;Lcd.c,132 :: 		if (cur_row > 1){                                          // Курсор НЕ на 1й строке, иначе игнорируем
	SETB C
	MOV A, #1
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JC L_main16
;Lcd.c,133 :: 		cur_row -=1;                                        // Временно изменить строку
	DEC _cur_row+0
;Lcd.c,134 :: 		cur_col = LASTCOL_LCD;                                        // Временно изменить столбец
	MOV _cur_col+0, #16
;Lcd.c,135 :: 		LCD_Chr(cur_row,cur_col,KEY_EMPTY);        // Удалить символ
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, #16
	MOV FARG_Lcd_Chr_out_char+0, #0
	LCALL _Lcd_Chr+0
;Lcd.c,137 :: 		cur_col = 1;                                        // Вернуть столбец в изначальное положение
	MOV _cur_col+0, #1
;Lcd.c,138 :: 		cur_row +=1;                                        // Вернуть строку в изначальное положение
	INC _cur_row+0
;Lcd.c,139 :: 		}
L_main16:
;Lcd.c,140 :: 		}
L_main15:
;Lcd.c,142 :: 		}
L_main13:
;Lcd.c,145 :: 		if (uart_rd == KEY_DELETE){
	MOV A, _uart_rd+0
	XRL A, #127
	JNZ L_main17
;Lcd.c,146 :: 		UART1_Write(KEY_DELETE);                                                  // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #127
	LCALL _UART1_Write+0
;Lcd.c,147 :: 		if (cur_col < LASTCOL_LCD){                                                   // Если символ не последний в строке, удалить последующий символ после курсора
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JNC L_main18
;Lcd.c,148 :: 		cur_col+=1;                                                          // Временно изменить столбец
	INC _cur_col+0
;Lcd.c,149 :: 		LCD_Chr(cur_row,cur_col,KEY_EMPTY);                  // Удалить символ
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, _cur_col+0
	MOV FARG_Lcd_Chr_out_char+0, #0
	LCALL _Lcd_Chr+0
;Lcd.c,151 :: 		cur_col-=1;                                                          // Вернуть столбец в изначальное положение
	DEC _cur_col+0
;Lcd.c,152 :: 		}
	SJMP L_main19
L_main18:
;Lcd.c,154 :: 		if (cur_row < LASTROW_LCD){                                          // Курсор НЕ на 16й строке, иначе игнорируем
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JNC L_main20
;Lcd.c,155 :: 		cur_row +=1;                                        // Временно изменить строку
	INC _cur_row+0
;Lcd.c,156 :: 		cur_col = 1;                                        // Временно изменить столбец
	MOV _cur_col+0, #1
;Lcd.c,157 :: 		LCD_Chr(cur_row,cur_col,KEY_EMPTY);        // Удалить символ
	MOV FARG_Lcd_Chr_row+0, _cur_row+0
	MOV FARG_Lcd_Chr_column+0, #1
	MOV FARG_Lcd_Chr_out_char+0, #0
	LCALL _Lcd_Chr+0
;Lcd.c,159 :: 		cur_col = LASTCOL_LCD;                                        // Вернуть столбец в изначальное положение
	MOV _cur_col+0, #16
;Lcd.c,160 :: 		cur_row -=1;                                        // Вернуть строку в изначальное положение
	DEC _cur_row+0
;Lcd.c,161 :: 		}
L_main20:
;Lcd.c,162 :: 		}
L_main19:
;Lcd.c,163 :: 		}
L_main17:
;Lcd.c,170 :: 		if (uart_rd == KEY_ARROWUP){
	MOV A, _uart_rd+0
	XRL A, #24
	JNZ L_main21
;Lcd.c,171 :: 		UART1_Write(KEY_ARROWUP);                                                  // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #24
	LCALL _UART1_Write+0
;Lcd.c,172 :: 		if(cur_row > 1) {cur_row-=1;}                         // Если текущий ряд не самый первый, то подняться на 1 строку
	SETB C
	MOV A, #1
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JC L_main22
	DEC _cur_row+0
L_main22:
;Lcd.c,174 :: 		}
L_main21:
;Lcd.c,177 :: 		if (uart_rd == KEY_ARROWDOWN){
	MOV A, _uart_rd+0
	XRL A, #25
	JNZ L_main23
;Lcd.c,178 :: 		UART1_Write(KEY_ARROWDOWN);                                                    // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #25
	LCALL _UART1_Write+0
;Lcd.c,179 :: 		if(cur_row < LASTROW_LCD) {cur_row+=1;}                         // Если текущий ряд не самый последний, то опуститься на 1 строку
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JNC L_main24
	INC _cur_row+0
L_main24:
;Lcd.c,180 :: 		}
L_main23:
;Lcd.c,183 :: 		if (uart_rd == KEY_ARROWRIGHT){
	MOV A, _uart_rd+0
	XRL A, #26
	JNZ L_main25
;Lcd.c,184 :: 		UART1_Write(KEY_ARROWRIGHT);                                                // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #26
	LCALL _UART1_Write+0
;Lcd.c,185 :: 		if (cur_col < LASTCOL_LCD) {cur_col+=1;}                        // Если текущая позиция курсора не на последнем столбце, то смещение вправо на 1
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JNC L_main26
	INC _cur_col+0
	SJMP L_main27
L_main26:
;Lcd.c,187 :: 		if (cur_row < LASTROW_LCD){                                        // Проверка что ещё есть строки снизу на которые можно перейти. Иначе игнорируем. (else последняя строка;последний символ)
	CLR C
	MOV A, #16
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JNC L_main28
;Lcd.c,188 :: 		cur_row+=1;                                                // переход на новую строку
	INC _cur_row+0
;Lcd.c,189 :: 		cur_col=1;                                                // обновить столбец курсора на первый символ
	MOV _cur_col+0, #1
;Lcd.c,190 :: 		}
L_main28:
;Lcd.c,191 :: 		}
L_main27:
;Lcd.c,193 :: 		}
L_main25:
;Lcd.c,196 :: 		if (uart_rd == KEY_ARROWLEFT){
	MOV A, _uart_rd+0
	XRL A, #27
	JNZ L_main29
;Lcd.c,197 :: 		UART1_Write(KEY_ARROWLEFT);                                                // Уведомить QT
	MOV FARG_UART1_Write_data_+0, #27
	LCALL _UART1_Write+0
;Lcd.c,198 :: 		if (cur_col > 1) {cur_col-=1;}                        // Если текущая позиция курсора не на первом столбце, то смещение влево на 1
	SETB C
	MOV A, #1
	XRL A, #128
	MOV R0, A
	MOV A, _cur_col+0
	XRL A, #128
	SUBB A, R0
	JC L_main30
	DEC _cur_col+0
	SJMP L_main31
L_main30:
;Lcd.c,200 :: 		if (cur_row > 1){                           // Проверка что ещё есть строки сверху на которые можно перейти. Иначе игнорируем. (else первая строка;первый символ)
	SETB C
	MOV A, #1
	XRL A, #128
	MOV R0, A
	MOV A, _cur_row+0
	XRL A, #128
	SUBB A, R0
	JC L_main32
;Lcd.c,201 :: 		cur_row-=1;                                                // переход на новую строку
	DEC _cur_row+0
;Lcd.c,202 :: 		cur_col=LASTCOL_LCD;                                                // обновить столбец курсора на последний символ
	MOV _cur_col+0, #16
;Lcd.c,203 :: 		}
L_main32:
;Lcd.c,204 :: 		}
L_main31:
;Lcd.c,205 :: 		}
L_main29:
;Lcd.c,207 :: 		} // if UART1_Data_Ready()
L_main2:
;Lcd.c,208 :: 		} // While
	LJMP L_main0
;Lcd.c,209 :: 		} // main
	SJMP #254
; end of _main
