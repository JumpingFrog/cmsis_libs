#include "lpc17xx_pinsel.h"
#include "lpc_types.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_i2c.h"
#include "debug_frmwrk.h"
#include "pcf2119x_mbed.h"
#include "keypad_mbed.h"
#include "stdlib.h"
#include "tty_mbed.h"

#define ASCII_OFFSET 0x80

char cur;

void delay(void) {
	int i = 0;
	for (; i< 2000000; i++) {
	}
}

int main(void) {
	i2c_init();
	uint32_t cur_adr = 0;
	uint8_t rx_buf[64];
	uint8_t dev_count = 0;
	char tmp[30];
	I2C_M_SETUP_Type i2cMcfg;
	i2cMcfg.tx_data = NULL;
	i2cMcfg.tx_length = 0;
	i2cMcfg.rx_data = rx_buf;
	i2cMcfg.rx_length = 1;
	i2cMcfg.retransmissions_max = 3;
	i2cMcfg.callback = NULL;
	for ( ; cur_adr < 128; cur_adr++) {
		i2cMcfg.sl_addr7bit = cur_adr;
		if (I2C_MasterTransferData(LPC_I2C1, &i2cMcfg, I2C_TRANSFER_POLLING) == SUCCESS) {
			sprintf(tmp, "%x, ", cur_adr<<1);
			dev_count++;
		}
	}
	sprintf(tmp, "\r\n%d i2c devices found.\r\n", dev_count);
	lcd_init();
	lcd_blank();
	keypad_scan();
	tty_init();

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
	sprintf(out,"%d", res);
	lcd_write(out);
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
