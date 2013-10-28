#include "pcf2119x_mbed.h"

void lcd_init() { //Reset sequence from p21 http://www-module.cs.york.ac.uk/hapr/resources/mbed_resources/datasheets/batron_operating_instructions_312175.pdf
	lcd_clear();	
	
	uint8_t buf[11];
	buf[0] = 0x00; //NOP
	buf[1] = 0x34; // D4 = Data Len (1 for 8 bit), D2 = Num Lines (1 for 2x16), D1 = MUX (0 for 2x16/1x32), D0 = Instruction set (0 for normal)
	buf[2] = 0x0C; // D2 = Display Ctrl (1 for on), D1 = Cursor ctrl (0 for off), D0 = Cursor Blink (0 for off)

	buf[3] = 0x06; //D1 = Cursor Dir (1 for inc), D0 = shift (0 for off)
	buf[4] = 0x35; //D4 = Data Len (1 for 8 bit), D2 = Num Lines (1 for 2x16), D1 = MUX (0 for 2x16/1x32), D0 = Instruction set (1 for extended)
	buf[5] = 0x04; //D1 = Col data (0 for L to R), D0 = row data (0 for Top to Bot)
	buf[6] = 0x10; //D1 = TC1, D2=TC2 (colour tempreature)
	buf[7] = 0x9F; //Set Vlcd, D5:D0 = voltage, D6 = V register (0 for VA)
	buf[8] = 0x34; // D4 = Data Len (1 for 8 bit), D2 = Num Lines (1 for 2x16), D1 = MUX (0 for 2x16/1x32), D0 = Instruction set (0 for normal)
	buf[9] = 0x80; //DRAM to 0x00
	buf[10] = 0x02; //return home
	i2c_transfer(buf, 11, NULL, 0, LCD_ADDR);
}

void lcd_clear() {
	uint8_t clr	= 0x01;
	i2c_transfer(&clr, 1, NULL, 0, LCD_ADDR);
	lcd_busy(); //wait for the screen to clear
}

void lcd_setpos(int x, int line) {
	int8_t buf[2] = {0x00, 0x80 + (line * 0x40) };
	i2c_transfer(buf, 2, NULL, 0, LCD_ADDR);
}

void lcd_busy() { //returns only when the LCD is no longer busy.
	uint8_t rxb = 0x00;
	i2c_transfer(NULL, 0, &rxb, 1, LCD_ADDR);
	while (rxb & 0x80) {
		i2c_transfer(NULL, 0, &rxb, 1, LCD_ADDR);		
	}
}

void lcd_blank() {
	lcd_setpos(0,0);
	lcd_write("                ");
	lcd_setpos(0,1);
	lcd_write("                ");
	lcd_setpos(0,0);
}

void lcd_write(char * c) {
	char * tmp = c;
	int len = 1;
	while (*tmp++ != '\0') {
		len++;	
	}
	uint8_t txd[len]; //ignores null
	txd[0] = 0x40; //why?
	int i = 1;
	for (; i < len; i++) {
		txd[i] = (uint8_t)(*c++ + ASCII_OFFSET);
	}
	i2c_transfer(txd, len, NULL, 0, LCD_ADDR);
}

void lcd_writec(char c) {
	c = (char)(c + ASCII_OFFSET);
	uint8_t txd[] = {0x40, ((uint8_t)c)};
	i2c_transfer(txd, 2, NULL, 0, LCD_ADDR);
}

