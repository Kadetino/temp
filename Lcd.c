/*
	Сonfiguration:
    MCU:             AT89S8253
    Oscillator:      External Clock 10.0000 MHz
    SW:              mikroC PRO for 8051
*/
/*
#define FIRSTPOS_LCD 1
#define LASTPOS_LCD 16

#define KEY_ARROWUP 24
#define KEY_ARROWDOWN 25
#define KEY_ARROWRIGHT 26
#define KEY_ARROWLEFT 27

#define KEY_DELETE 127
#define KEY_ENTER 13
#define KEY_BACKSPACE 8
*/

// LCD module connections
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;
//
sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char storage[256];

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
    // Lower case latin   97-122
    // Upper case latin   65-90
    // Numbers            48-57
    // !"#$%&'()*+,-./    33-47
    // :;<=>?@            58-64
    // [\]^_`             91-96
    // {|}~               123-126
	// <space>			  32
	if (32 <= uart_rd && uart_rd <= 122){
		UART1_Write(uart_rd);					// Уведомить QT
		if (cur_col < 17){						// В строке ещё есть место
			LCD_Chr(cur_row,cur_col,uart_rd);	// Напечатать символ
			cur_col+=1;							// Обновить позицию (столбец) курсора
		}
		else {									// Если курсор на 17м символе (строка заполнена)
			if (cur_row < 16){					// Можно осуществить переход на следуюущую строку
				cur_col = 1;					// поставить позицию курсора в начало	
				cur_row+= 1;					// перейти на следующую строку
				LCD_Chr(cur_row,cur_col,uart_rd);	// Напечатать символ
			}
			else {								// Текущая строка 16я
				
			}
			
			
		}					
	}

    //##############
    //SPECIAL KEYS
    //##############
	
	// ENTER
    if (uart_rd == 13){
		UART1_Write(13);						// Уведомить QT
		if (cur_row < 16){						// Если переход возможен (==количество строк позволяет)
			cur_row +=1;						// Поменять строку
			if (cur_col != 1) cur_col = 1; 		// Переместить курсор на 1й символ, если он был на первом символе
		}
    }
	
	// BACKSPACE
    if (uart_rd == 8){
		UART1_Write(8);							// Уведомить QT
		if (cur_col > 1){						// Если символ не первый в строке, удалить предыдуший курсору символ и оставить курсор там
			cur_col-=1;							// Вернуть курсор назад
			LCD_Chr(cur_row,cur_col,32);		// Удалить символ
		}	
		else {									// Курсор на 1ом символе
			if (cur_row > 1){			  		// Курсор НЕ на 1й строке, иначе игнорируем
				cur_row -=1;					// Временно изменить строку
				cur_col = 16;					// Временно изменить столбец
				LCD_Chr(cur_row,cur_col,32);	// Удалить символ
				cur_col = 1;					// Вернуть столбец в изначальное положение
				cur_row +=1;					// Вернуть строку в изначальное положение
			}
		}
      
    }
	
	// DELETE
    if (uart_rd == 127){
		UART1_Write(127);				  		// Уведомить QT
		if (cur_col < 16){			 	  		// Если символ не последний в строке, удалить последующий символ после курсора
			cur_col+=1;					  		// Временно изменить столбец
			LCD_Chr(cur_row,cur_col,32);  		// Удалить символ
			cur_col-=1;					  		// Вернуть столбец в изначальное положение
		}	 
		else {							  		// Курсор на 16ом символе
			if (cur_row < 16){			  		// Курсор НЕ на 16й строке, иначе игнорируем
				cur_row +=1;					// Временно изменить строку
				cur_col = 1;					// Временно изменить столбец
				LCD_Chr(cur_row,cur_col,32);	// Удалить символ
				cur_col = 16;					// Вернуть столбец в изначальное положение
				cur_row -=1;					// Вернуть строку в изначальное положение
			}
		}
    }
    
    //##############
    //ARROW KEYS
    //##############
	
	// ARROW UP
    if (uart_rd == 24){
		UART1_Write(24);			  			// Уведомить QT
		if(cur_row > 1) {cur_row-=1;} 			// Если текущий ряд не самый первый, то подняться на 1 строку
      
    }
	
	// ARROW DOWN
    if (uart_rd == 25){
		UART1_Write(25); 			   			// Уведомить QT
		if(cur_row < 16) {cur_row+=1;} 			// Если текущий ряд не самый последний, то опуститься на 1 строку
    }
	
	// ARROW RIGHT
    if (uart_rd == 26){
		UART1_Write(26);						// Уведомить QT
		if (cur_col < 16) {cur_col+=1;}			// Если текущая позиция курсора не на последнем столбце, то смещение вправо на 1
		else {									// иначе (==курсор на последнем 16ом символе)
			if (cur_row < 16){					// Проверка что ещё есть строки снизу на которые можно перейти. Иначе игнорируем. (else последняя строка;последний символ)
				cur_row+=1;						// переход на новую строку
				cur_col=1;						// обновить столбец курсора на первый символ
           }
      }
      
    }
	
	// ARROW LEFT
    if (uart_rd == 27){
		UART1_Write(27);						// Уведомить QT
		if (cur_col > 1) {cur_col-=1;}			// Если текущая позиция курсора не на первом столбце, то смещение влево на 1
		else {                          		// иначе (==курсор на 1ом символе)
			if (cur_row > 1){           		// Проверка что ещё есть строки сверху на которые можно перейти. Иначе игнорируем. (else первая строка;первый символ)
				cur_row-=1;						// переход на новую строку
				cur_col=16;						// обновить столбец курсора на последний символ
			}
		}
    }
    
	} // if UART1_Data_Ready()
  } // While
} // main