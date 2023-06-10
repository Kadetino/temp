/*
        Сonfiguration:
    MCU:             AT89S8253
    Oscillator:      External Clock 10.0000 MHz
    SW:              mikroC PRO for 8051
*/
#define LASTCOL_LCD 16
#define LASTROW_LCD 16

#define KEY_ARROWUP 24
#define KEY_ARROWDOWN 25
#define KEY_ARROWRIGHT 26
#define KEY_ARROWLEFT 27

#define KEY_DELETE 127
#define KEY_ENTER 13
#define KEY_BACKSPACE 8

#define KEY_EMPTY 0

#include <string.h>
// LCD module connections
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;
//
sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char storage[256];
char uart_rd;
short int cur_col, cur_row;

void var_preset(){
        //memset(storage, KEY_EMPTY, 256); // Заполнить филлерами KEY_EMPTY
        cur_col = 1; // Настроить курсор столбцов
        cur_row = 1; // Настроить курсор строк
}

void temptemp(short int target_row, short int new_row){
        char temp[LASTCOL_LCD];
        strncpy(temp,storage[(new_row-1)*LASTCOL_LCD],LASTCOL_LCD);
        Lcd_Out(target_row, 1, temp);
}

void main(){
        var_preset(); // свои переменные
        
        // Настройка LCD
        Lcd_Init();
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Cmd(_LCD_UNDERLINE_ON);
        Lcd_Cmd(_LCD_BLINK_CURSOR_ON);
        //  Lcd_Cmd(_LCD_CURSOR_OFF);
        
        // Серийный порт
        UART1_Init(4800); // Установить Baud rate 4800
        Delay_ms(500); // Wait for UART module to stabilize

        // Endless loop
        while (1) { 
        if (UART1_Data_Ready()){
                uart_rd = UART1_Read(); // Прочитать полученное


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
        // <space>                          32
        if (32 <= uart_rd && uart_rd <= 122){
                UART1_Write(uart_rd);                                        // Уведомить QT
                if (cur_col < (LASTCOL_LCD + 1)){                                                // В строке ещё есть место
                        LCD_Chr(cur_row,cur_col,uart_rd);        // Напечатать символ
                        //storage[LASTROW_LCD*(cur_row-1)+(cur_col-1)] = uart_rd;
                        cur_col+=1;                                                        // Обновить позицию (столбец) курсора
                        if (cur_col >= (LASTCOL_LCD + 1)){        // Если это был последний столбец, перенос строки
                                if (cur_row >= LASTROW_LCD) cur_col = LASTCOL_LCD; // Последний столбец и последний символ - оставить курсор на месте
                                else { // Перенос строки
                                        cur_col = 1;
                                        cur_row+= 1;
                                }
                        }
                }
                /*
                // unreachable code?
                else {                                                                        // Если курсор на 17м символе (строка заполнена и почему-то курсор на 17ом)
                        if (cur_row < LASTROW_LCD){                                        // Можно осуществить переход на следуюущую строку
                                cur_col = 1;                                        // поставить позицию курсора в начало        
                                cur_row+= 1;                                        // перейти на следующую строку
                                LCD_Chr(cur_row,cur_col,uart_rd);        // Напечатать символ
                        }
                        else {                                                                // Текущая строка 16я
                                
                        }
                        */
                        

        }

    //##############
    //SPECIAL KEYS
    //##############
        
        // ENTER
    if (uart_rd == KEY_ENTER){
                UART1_Write(KEY_ENTER);                                                // Уведомить QT
                if (cur_row < LASTROW_LCD){                                                // Если переход возможен (==количество строк позволяет)
                        cur_row +=1;                                                // Поменять строку
                        if (cur_col != 1) cur_col = 1;                 // Переместить курсор на 1й символ, если он был на первом символе
                }
    }
        
        // BACKSPACE
    if (uart_rd == KEY_BACKSPACE){
                UART1_Write(KEY_BACKSPACE);                                                        // Уведомить QT
                if (cur_col > 1){                                                // Если символ не первый в строке, удалить предыдуший курсору символ и оставить курсор там
                        cur_col-=1;                                                        // Вернуть курсор назад
                        LCD_Chr(cur_row,cur_col,KEY_EMPTY);                // Удалить символ
                        //storage[LASTROW_LCD*(cur_row-1)+(cur_col-1)] = KEY_EMPTY;
                }        
                else {                                                                        // Курсор на 1ом символе
                        if (cur_row > 1){                                          // Курсор НЕ на 1й строке, иначе игнорируем
                                cur_row -=1;                                        // Временно изменить строку
                                cur_col = LASTCOL_LCD;                                        // Временно изменить столбец
                                LCD_Chr(cur_row,cur_col,KEY_EMPTY);        // Удалить символ
                                //storage[LASTROW_LCD*(cur_row-1)+(cur_col-1)] = KEY_EMPTY;
                                cur_col = 1;                                        // Вернуть столбец в изначальное положение
                                cur_row +=1;                                        // Вернуть строку в изначальное положение
                        }
                }
      
    }
        
        // DELETE
    if (uart_rd == KEY_DELETE){
                UART1_Write(KEY_DELETE);                                                  // Уведомить QT
                if (cur_col < LASTCOL_LCD){                                                   // Если символ не последний в строке, удалить последующий символ после курсора
                        cur_col+=1;                                                          // Временно изменить столбец
                        LCD_Chr(cur_row,cur_col,KEY_EMPTY);                  // Удалить символ
                        //storage[LASTROW_LCD*(cur_row-1)+(cur_col-1)] = KEY_EMPTY;
                        cur_col-=1;                                                          // Вернуть столбец в изначальное положение
                }         
                else {                                                                          // Курсор на 16ом символе
                        if (cur_row < LASTROW_LCD){                                          // Курсор НЕ на 16й строке, иначе игнорируем
                                cur_row +=1;                                        // Временно изменить строку
                                cur_col = 1;                                        // Временно изменить столбец
                                LCD_Chr(cur_row,cur_col,KEY_EMPTY);        // Удалить символ
                                //storage[LASTROW_LCD*(cur_row-1)+(cur_col-1)] = KEY_EMPTY;
                                cur_col = LASTCOL_LCD;                                        // Вернуть столбец в изначальное положение
                                cur_row -=1;                                        // Вернуть строку в изначальное положение
                        }
                }
    }
    
    //##############
    //ARROW KEYS
    //##############
        
        // ARROW UP
    if (uart_rd == KEY_ARROWUP){
                UART1_Write(KEY_ARROWUP);                                                  // Уведомить QT
                if(cur_row > 1) {cur_row-=1;}                         // Если текущий ряд не самый первый, то подняться на 1 строку
      
    }
        
        // ARROW DOWN
    if (uart_rd == KEY_ARROWDOWN){
                UART1_Write(KEY_ARROWDOWN);                                                    // Уведомить QT
                if(cur_row < LASTROW_LCD) {cur_row+=1;}                         // Если текущий ряд не самый последний, то опуститься на 1 строку
    }
        
        // ARROW RIGHT
    if (uart_rd == KEY_ARROWRIGHT){
                UART1_Write(KEY_ARROWRIGHT);                                                // Уведомить QT
                if (cur_col < LASTCOL_LCD) {cur_col+=1;}                        // Если текущая позиция курсора не на последнем столбце, то смещение вправо на 1
                else {                                                                        // иначе (==курсор на последнем 16ом символе)
                        if (cur_row < LASTROW_LCD){                                        // Проверка что ещё есть строки снизу на которые можно перейти. Иначе игнорируем. (else последняя строка;последний символ)
                                cur_row+=1;                                                // переход на новую строку
                                cur_col=1;                                                // обновить столбец курсора на первый символ
           }
      }
      
    }
        
        // ARROW LEFT
    if (uart_rd == KEY_ARROWLEFT){
                UART1_Write(KEY_ARROWLEFT);                                                // Уведомить QT
                if (cur_col > 1) {cur_col-=1;}                        // Если текущая позиция курсора не на первом столбце, то смещение влево на 1
                else {                                          // иначе (==курсор на 1ом символе)
                        if (cur_row > 1){                           // Проверка что ещё есть строки сверху на которые можно перейти. Иначе игнорируем. (else первая строка;первый символ)
                                cur_row-=1;                                                // переход на новую строку
                                cur_col=LASTCOL_LCD;                                                // обновить столбец курсора на последний символ
                        }
                }
    }
    
        } // if UART1_Data_Ready()
  } // While
} // main