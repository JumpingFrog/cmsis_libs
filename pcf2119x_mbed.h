//soon to be an i2c lcd driver
#include "i2c_mbed.h"
#define LCD_ADDR 0x3b
#define ASCII_OFFSET 0x80

void lcd_init(void); //initialise the LCD

void lcd_setpos(int x, int line); //set the LCD to desired char

void lcd_write(char * c); //write the desired string to the lcd from pos

void lcd_clear(); //clear the LCD

void lcd_busy(); //wait until lcd is not busy

void lcd_blank();


