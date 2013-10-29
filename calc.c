#include "lpc_types.h"
#include "debug_frmwrk.h"
#include "pcf2119x_mbed.h"
#include "keypad_mbed.h"
#include "stdlib.h"
#include "tty_mbed.h" //terminal lib
#include "saa1064_mbed.h" //7seg lib

char cur; //used to store last char read

void delay(void) { //delay for between char inputs
	int i = 0;
	for (; i< 2000000; i++) {
	}
}

int main(void) {

	tty_init();
	tty_dump_regs();


	i2c_init();
	lcd_init();
	lcd_blank();

	char tmp[128];
	uint8_t * devs = i2c_scan();
	int dev;
	tty_printf("Scanning i2c bus...\n\r");
	for (dev = 0; dev<128; dev++) { //read out i2c scan results
		if (devs[dev]) {
			tty_printf("Device at 0x%x found.\n\r", dev);	
		}
	}
	//Calculator
	int op1 = getNum();
	delay();
	lcd_writec(' ');
	int op2 = getNum();
	delay();
	lcd_writec(' ');
	char op = cur;
	int res = 0;
	switch (op) {
		case 'A':
			lcd_writec('+');
			res = op1 + op2;
			break;
		case 'B':
			lcd_writec('-');
			res = op1 - op2;
			break;
		case 'C':
			lcd_writec('*');
			res = op1 * op2;
			break;
		case 'D':
			lcd_writec('/');
			res = op1 / op2;
			break;
		default:
			lcd_blank();
			break;	
	}
	lcd_setpos(0,1);
	char out[16];
	sprintf(out,"%04d", res);
	lcd_write(out);
	led_ctrl(1, 1, 1); //enable all digits on LED
	led_digit((out[0] - '0'), 1, 0);
	led_digit((out[1] - '0'), 2, 0);
	led_digit((out[2] - '0'), 3, 0);
	led_digit((out[3] - '0'), 4, 0);
	return 0;
}

int getNum() { //reads a number from the keypad until NaN
	int ret = 0;
	cur = keypad_scan();
	while (cur >= '0' && cur <= '9') {
		delay();
		lcd_writec(cur);
		ret = ret*10 + (cur - '0');
		cur = keypad_scan();
	}
	return ret;
}
