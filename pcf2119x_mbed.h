//soon to be an i2c lcd driver
#include "i2c_mbed.h"

void lcd_init(void); //initialise the LCD

void lcd_setpos(int x, int line); //set the LCD to desired char

void lcd_write(int x, int line, char * str); //write the desired string to the lcd from pos

void lcd_clear(); //clear the LCD


