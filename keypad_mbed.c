#include "keypad_mbed.h"
#include "tty_mbed.h"

uint8_t send(uint8_t txd) {
		uint8_t ret;
		i2c_transfer(&txd, 1, &ret, 1, EXPAND_ADDR);	
		return ret;
	}

void keypad_scan(void) { 
	tty_init();
	int bit = 0; //scan upper nibble
	int row = 0;
	uint8_t mask = 0xF0;
	uint8_t tx = 0x00;
	uint8_t rx = 0x00;
	for (; bit < 4; bit++) {
		tx = 0xF0 ^ (1<<(4 + bit));
		rx = send(tx) & 0x0F;
		if (rx != 0x0F) { // something is pressed
			tty_printf("Low Col\n\r");
			for (row = 0; row < 4; row++) {
				if (rx ^ (1<<bit)) {
					char bufa[2];
					tty_printf("Row: ");
					bufa[0] = row + '0';
					bufa[1] = '\n';
					tty_printf(bufa);
					bufa[0] = bit + '0';
					tty_printf("Col: ");
					tty_printf(bufa);
										
				}
			}
		} 
	}
}
