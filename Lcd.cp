#line 1 "C:/Users/Kadet/Documents/GitHub/temp/Lcd.c"
#line 1 "c:/users/kadet/documents/mikroelektronika/mikroc pro for 8051/include/string.h"





void * memchr(void *p, char n, unsigned int v);
int memcmp(void *s1, void *s2, int n);
void * memcpy(void * d1, void * s1, int n);
void * memmove(void * to, void * from, int n);
void * memset(void * p1, char character, int n);
char * strcat(char * to, char * from);
char * strchr(char * ptr, char chr);
int strcmp(char * s1, char * s2);
char * strcpy(char * to, char * from);
int strlen(char * s);
char * strncat(char * to, char * from, int size);
char * strncpy(char * to, char * from, int size);
int strspn(char * str1, char * str2);
char strcspn(char * s1, char * s2);
int strncmp(char * s1, char * s2, char len);
char * strpbrk(char * s1, char * s2);
char * strrchr(char *ptr, char chr);
char * strstr(char * s1, char * s2);
char * strtok(char * s1, char * s2);
#line 23 "C:/Users/Kadet/Documents/GitHub/temp/Lcd.c"
sbit LCD_RS at P0_4_bit;
sbit LCD_EN at P0_6_bit;

sbit LCD_D4 at P3_4_bit;
sbit LCD_D5 at P3_5_bit;
sbit LCD_D6 at P3_6_bit;
sbit LCD_D7 at P3_7_bit;


char storage[256];
char uart_rd;
short int cur_col, cur_row;

void var_preset(){

 cur_col = 1;
 cur_row = 1;
}

void temptemp(short int target_row, short int new_row){
 char temp[ 16 ];
 strncpy(temp,storage[(new_row-1)* 16 ], 16 );
 Lcd_Out(target_row, 1, temp);
}

void main(){
 var_preset();


 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_UNDERLINE_ON);
 Lcd_Cmd(_LCD_BLINK_CURSOR_ON);



 UART1_Init(4800);
 Delay_ms(500);


 while (1) {
 if (UART1_Data_Ready()){
 uart_rd = UART1_Read();
#line 80 "C:/Users/Kadet/Documents/GitHub/temp/Lcd.c"
 if (32 <= uart_rd && uart_rd <= 122){
 UART1_Write(uart_rd);
 if (cur_col < ( 16  + 1)){
 LCD_Chr(cur_row,cur_col,uart_rd);

 cur_col+=1;
 if (cur_col >= ( 16  + 1)){
 if (cur_row >=  16 ) cur_col =  16 ;
 else {
 cur_col = 1;
 cur_row+= 1;
 }
 }
 }
#line 108 "C:/Users/Kadet/Documents/GitHub/temp/Lcd.c"
 }






 if (uart_rd ==  13 ){
 UART1_Write( 13 );
 if (cur_row <  16 ){
 cur_row +=1;
 if (cur_col != 1) cur_col = 1;
 }
 }


 if (uart_rd ==  8 ){
 UART1_Write( 8 );
 if (cur_col > 1){
 cur_col-=1;
 LCD_Chr(cur_row,cur_col, 0 );

 }
 else {
 if (cur_row > 1){
 cur_row -=1;
 cur_col =  16 ;
 LCD_Chr(cur_row,cur_col, 0 );

 cur_col = 1;
 cur_row +=1;
 }
 }

 }


 if (uart_rd ==  127 ){
 UART1_Write( 127 );
 if (cur_col <  16 ){
 cur_col+=1;
 LCD_Chr(cur_row,cur_col, 0 );

 cur_col-=1;
 }
 else {
 if (cur_row <  16 ){
 cur_row +=1;
 cur_col = 1;
 LCD_Chr(cur_row,cur_col, 0 );

 cur_col =  16 ;
 cur_row -=1;
 }
 }
 }






 if (uart_rd ==  24 ){
 UART1_Write( 24 );
 if(cur_row > 1) {cur_row-=1;}

 }


 if (uart_rd ==  25 ){
 UART1_Write( 25 );
 if(cur_row <  16 ) {cur_row+=1;}
 }


 if (uart_rd ==  26 ){
 UART1_Write( 26 );
 if (cur_col <  16 ) {cur_col+=1;}
 else {
 if (cur_row <  16 ){
 cur_row+=1;
 cur_col=1;
 }
 }

 }


 if (uart_rd ==  27 ){
 UART1_Write( 27 );
 if (cur_col > 1) {cur_col-=1;}
 else {
 if (cur_row > 1){
 cur_row-=1;
 cur_col= 16 ;
 }
 }
 }

 }
 }
}
